#include "rtmp_wrap.h"
#include <jni.h>
#include <string>
#include <android/log.h>
#include  "rtmp.h"
#include "log_abs.h"
#include "hwutil.h"
#include "log.h"

// <editor-fold defaultstate="collapsed" desc="全局变量定义">
namespace {
    const int INVALIDE_DEF_RTMP_STREAM_ID = -1;
    RtmpWrap::Live *globalLive = nullptr;
    uint32_t startTime = 0;
    // 日志TAG
    const char *TAG_LIBRTMP = "LIBRTMP";

    /**
     * 找到以 0x000001 开头的 nalu
     * @param Buf nalu数据
     * @return 1表示是，0表示不是
     */
    int findStartCode2(const int8_t *Buf) {
        if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 1) return 0; //0x000001?
        else return 1;
    }

    /**
     * 找到以 0x00000001 开头的 nalu
     * @param Buf nalu数据
     * @return 1表示是，0表示不是
     */
    int findStartCode3(const int8_t *Buf) {
        if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 0 || Buf[3] != 1) return 0;//0x00000001?
        else return 1;
    }

    /**
     *
     * @param buf
     * @param len
     * @return
     */
    bool isFrameSPS(const int8_t *buf, int len) {
        if (len < 4) {
            return false;
        }
        if (findStartCode2(buf)) {
            return HWUtils::parseHexNaluType(buf[3]) == HWUtils::nal_unit_type_e::NAL_SPS;
        } else if (findStartCode3(buf)) {
            return HWUtils::parseHexNaluType(buf[4]) == HWUtils::nal_unit_type_e::NAL_SPS;
        } else {
            return false;
        }
    }

    // 是否是PPS帧
    bool isFramePPS(const int8_t *buf, int len) {
        if (len < 4) {
            return false;
        }
        if (findStartCode2(buf)) {
            return HWUtils::parseHexNaluType(buf[3]) == HWUtils::nal_unit_type_e::NAL_PPS;
        } else if (findStartCode3(buf)) {
            return HWUtils::parseHexNaluType(buf[4]) == HWUtils::nal_unit_type_e::NAL_PPS;
        } else {
            return false;
        }
    }

    // 是否是IDR帧
    bool isFrameIDR(const int8_t *buf, int len) {
        if (len < 4) {
            return false;
        }
        if (findStartCode2(buf)) {
            return HWUtils::parseHexNaluType(buf[3]) == HWUtils::nal_unit_type_e::NAL_SLICE_IDR;
        } else if (findStartCode3(buf)) {
            return HWUtils::parseHexNaluType(buf[4]) == HWUtils::nal_unit_type_e::NAL_SLICE_IDR;
        } else {
            return false;
        }
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="初始化sps pps">
namespace {
    //传递第一帧      00 00 00 01 67 64 00 28ACB402201E3CBCA41408681B4284D4  0000000168  EE 06 F2 C0
    /**
     * 初始化live中的sps和pps数据，其中data数组中同时包含了sps和pps的数据
     * @param data sps+pps数据
     * @param len  长度
     * @param live RtmpWrap::Live
     */
    void initSpsPps(int8_t *data, int len, RtmpWrap::Live *live) {
        for (int i = 0; i < len; i++) {
//        防止越界
            if (i + 4 < len) {
                if (data[i] == 0x00 && data[i + 1] == 0x00
                    && data[i + 2] == 0x00
                    && data[i + 3] == 0x01) {
                    if (data[i + 4] == 0x68) {
                        live->sps_len = i - 4;
//                    new一个数组
                        live->sps = static_cast<int8_t *>(malloc(live->sps_len));
//                    sps解析出来了
                        memcpy(live->sps, data + 4, live->sps_len);

//                    解析pps
                        live->pps_len = len - (4 + live->sps_len) - 4;
//                    实例化PPS 的数组
                        live->pps = static_cast<int8_t *>(malloc(live->pps_len));
//                    rtmp  协议

                        memcpy(live->pps, data + 4 + live->sps_len + 4, live->pps_len);
                        MyLog::v("sps:%d pps:%d", live->sps_len, live->pps_len);
                        break;
                    }
                }

            }
        }
    }

    /**
     * 初始化SPS
     * @param data SPS数据
     * @param len  长度
     * @param live live
     */
    void initSPS(int8_t *data, int len, RtmpWrap::Live *live) {
        if (len < 4) {
            MyLog::d("len < 4 , failed to init SPS");
            return;
        }
        live->sps_len = len - 4;
        live->sps = static_cast<int8_t *>(malloc(live->sps_len));
        memcpy(live->sps, data + 4, live->sps_len);
    }

    void initPPS(int8_t *data, int len, RtmpWrap::Live *live) {
        if (len < 4) {
            MyLog::d("len < 4 , failed to init PPS");
            return;
        }
        live->pps_len = len - 4;
        live->pps = static_cast<int8_t *>(malloc(live->pps_len));
        memcpy(live->pps, data + 4, live->pps_len);
    }
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="创建包">
static RTMPPacket *createVideoPackage(RtmpWrap::Live *live) {
    RTMP *rtmp = live->rtmp;
    if (!rtmp) {
        return nullptr;
    }
//sps  pps 的 packaet
    int body_size = 16 + live->sps_len + live->pps_len;
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
//    实例化数据包
    RTMPPacket_Alloc(packet, body_size);
    int i = 0;
    packet->m_body[i++] = 0x17;
    //AVC sequence header 设置为0x00
    packet->m_body[i++] = 0x00;
    //CompositionTime
    packet->m_body[i++] = 0x00;
    packet->m_body[i++] = 0x00;
    packet->m_body[i++] = 0x00;
    //AVC sequence header
    packet->m_body[i++] = 0x01;
//    原始 操作

    packet->m_body[i++] = live->sps[1]; //profile 如baseline、main、 high

    packet->m_body[i++] = live->sps[2]; //profile_compatibility 兼容性
    packet->m_body[i++] = live->sps[3]; //profile level
    packet->m_body[i++] = 0xFF;//已经给你规定好了
    packet->m_body[i++] = 0xE1; //reserved（111） + lengthSizeMinusOne（5位 sps 个数） 总是0xe1
//高八位
    packet->m_body[i++] = (live->sps_len >> 8) & 0xFF;
//    低八位
    packet->m_body[i++] = live->sps_len & 0xff;
//    拷贝sps的内容
    memcpy(&packet->m_body[i], live->sps, live->sps_len);
    i += live->sps_len;
//    pps
    packet->m_body[i++] = 0x01; //pps number
//rtmp 协议
    //pps length
    packet->m_body[i++] = (live->pps_len >> 8) & 0xff;
    packet->m_body[i++] = live->pps_len & 0xff;
//    拷贝pps内容
    memcpy(&packet->m_body[i], live->pps, live->pps_len);
//packaet
//视频类型
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
//
    packet->m_nBodySize = body_size;
//    视频 04
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = RTMP_GetTime() - startTime;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = live->rtmp->m_stream_id;
    return packet;
}

static RTMPPacket *createVideoPackage(int8_t *buf, int len, const long tms, RtmpWrap::Live *live) {
    if (findStartCode2(buf)) {
        buf += 3;
    } else if (findStartCode3(buf)) {
        buf += 4;
    } else {
        MyLog::eTag(TAG_LIBRTMP, "createVideoPackage -> find no annexB anlu header");
        return nullptr;
    }
//长度
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    int body_size = len + 9;
//初始化RTMP内部的body数组
    RTMPPacket_Alloc(packet, body_size);


    if (buf[0] == 0x65) {
        packet->m_body[0] = 0x17;
        MyLog::v("发送关键帧 data");
    } else {
        packet->m_body[0] = 0x27;
        MyLog::v("发送非关键帧 data");
    }
//    固定的大小
    packet->m_body[1] = 0x01;
    packet->m_body[2] = 0x00;
    packet->m_body[3] = 0x00;
    packet->m_body[4] = 0x00;

    //长度
    packet->m_body[5] = (len >> 24) & 0xff;
    packet->m_body[6] = (len >> 16) & 0xff;
    packet->m_body[7] = (len >> 8) & 0xff;
    packet->m_body[8] = (len) & 0xff;

    //数据
    memcpy(&packet->m_body[9], buf, len);
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = body_size;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = tms;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = live->rtmp->m_stream_id;
    return packet;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="发送包">
/**
 * 发送RTMP包数据
 * @param packet 包数据
 * @return 1表示发送成功，0表示发送失败
 */
static int sendPacket(RTMPPacket *packet) {
    if (!globalLive->rtmp) {
        return 0;
    }
    int r = RTMP_SendPacket(globalLive->rtmp, packet, 1);
    RTMPPacket_Free(packet);
    free(packet);
    return r;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="释放">
static void freeGlobalLive() {
    if (globalLive) {
        free(globalLive);
        globalLive = nullptr;
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="回调函数">
// rtmp日志回调
static void My_RTMP_LogCallback(int level, const char *fmt, va_list args) {
    char *msg_to_print = nullptr;
    vasprintf(&msg_to_print, fmt, args);
    MyLog::dTag(TAG_LIBRTMP, msg_to_print);
    delete msg_to_print;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="API">
int RtmpWrap::connect(const char *url) {
    RTMP_LogSetCallback(My_RTMP_LogCallback);
    //实例化
    globalLive = (Live *) malloc(sizeof(Live));
    memset(globalLive, 0, sizeof(Live));

    RTMP *rtmp = RTMP_Alloc();
    globalLive->rtmp = rtmp;
    RTMP_Init(rtmp);
    rtmp->Link.timeout = 10;
    if (!url || strlen(url) == 0) {
        MyLog::v("input url is empty");
        return false;
    }
    MyLog::v("rtmp_wrap_connect %s", url);
    if (!RTMP_SetupURL(rtmp, (char *) url)) {
        MyLog::v("RTMP_SetupURL failed");
        freeGlobalLive();
        return -1;
    }
    RTMP_EnableWrite(rtmp);
    MyLog::v("RTMP_Connect");
    if (!RTMP_Connect(rtmp, nullptr)) {
        MyLog::v("RTMP_Connect failed");
        freeGlobalLive();
        return -1;
    }
    MyLog::v("RTMP_ConnectStream ");
    if (!RTMP_ConnectStream(rtmp, 0)) {
        MyLog::v("RTMP_ConnectStream failed");
        freeGlobalLive();
        return -1;
    }
    MyLog::v("rtmp_wrap_connect success");
    startTime = RTMP_GetTime();
    return 1;
}

//传递第一帧      00 00 00 01 67 64 00 28ACB402201E3CBCA41408081B4284D4  0000000168 EE 06 F2 C0
int RtmpWrap::sendVideo(int8_t *buf, int len, long tms) {
    int ret = 0;
    if (isFrameSPS(buf, len)) {
//        缓存sps 和pps 到全局变量 不需要推流
        if (globalLive && (!globalLive->pps || !globalLive->sps)) {
//缓存 没有推流
            initSpsPps(buf, len, globalLive);
        }
        return ret;
    }
//    I帧
    if (isFrameIDR(buf, len)) {//关键帧
//         推两个
//sps 和 ppps 的paclet  发送sps pps
        RTMPPacket *packet = createVideoPackage(globalLive);
        if (packet) {
            sendPacket(packet);
        }
//        发送I帧
    }
//    两个   I帧  0x17  2B P 0x7
    RTMPPacket *packet2 = createVideoPackage(buf, len, tms, globalLive);
    if (packet2) {
        ret = sendPacket(packet2);
    }
    return ret;
}

static bool encounterSPS = false;

void RtmpWrap::createRtmpPacket(RTMPPacket **packet, int8_t *buf, int len) {
    if (!globalLive) {
        return;
    }
    if (encounterSPS) {
        encounterSPS = false;
        MyLog::dTag(TAG_LIBRTMP, "PPS %x%x%x%x%x", buf[0], buf[1], buf[2], buf[3], buf[4]);
    }
    // 初始化sps和pps
    if (isFrameSPS(buf, len)) {
        MyLog::dTag(TAG_LIBRTMP, "SPS %x%x%x%x%x", buf[0], buf[1], buf[2], buf[3], buf[4]);
        encounterSPS = true;
        MyLog::dTag(TAG_LIBRTMP, "遇到SPS");
        if (!globalLive->sps) {
            initSPS(buf, len, globalLive);
            MyLog::dTag(TAG_LIBRTMP, "初始化SPS");
        }
    } else if (isFramePPS(buf, len)) {
        MyLog::dTag(TAG_LIBRTMP, "遇到PPS");
        if (!globalLive->pps) {
            initPPS(buf, len, globalLive);
            MyLog::dTag(TAG_LIBRTMP, "初始化PPS");
        }
        *packet = createVideoPackage(globalLive);
        MyLog::dTag(TAG_LIBRTMP, "发送SPS和PPS");
    } else {
        *packet = createVideoPackage(buf, len, RTMP_GetTime() - startTime, globalLive);
    }
}

void RtmpWrap::createAudioRtmpPacket(RTMPPacket **packet, int8_t *buf, int len, bool isHead) {
    *packet = new RTMPPacket();
    RTMPPacket_Alloc(*packet, len + 2);
    RTMPPacket *p = *packet;
    p->m_body[0] = 0xAF;
    if (isHead) {
        p->m_body[1] = 0x00;
    } else {
        p->m_body[1] = 0x01;
    }
    memcpy(&p->m_body[2], buf, len);
    p->m_hasAbsTimestamp = 0;
    p->m_nBodySize = len + 2;
    p->m_nTimeStamp = RTMP_GetTime() - startTime;
    p->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    p->m_nChannel = 0x11;
    p->m_headerType = RTMP_PACKET_SIZE_LARGE;
}

int RtmpWrap::sendVideo(RTMPPacket &packet) {
    return sendPacket(&packet);
}

int RtmpWrap::getRtmpStreamId() {
    if (globalLive && globalLive->rtmp) {
        return globalLive->rtmp->m_stream_id;
    } else {
        return INVALIDE_DEF_RTMP_STREAM_ID;
    }
}

void RtmpWrap::destroy() {
    if (globalLive) {
        RTMP *rtmp = globalLive->rtmp;
        if (rtmp) {
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
            globalLive->rtmp = nullptr;
        }
        freeGlobalLive();
    }
}
// </editor-fold>
