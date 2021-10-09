//
// Created by HWilliam on 2021/5/16.
//

/**
 * jni函数，面向Java
 *
 * 初始化
 * RTMP_init
 *
 * 连接
 * RTMP_JNI_Connect
 *
 * 发送数据
 * RTMP_JNI_SendData
 *
 * 销毁
 * RTMP_destroy
 */

#include <jni.h>
#include "rtmp_wrap.h"
#include "x264_wrap.h"
#include <cassert>
#include "log_abs.h"
#include "x264.h"
#include "VideoEncoder.h"
#include "safe_queue.h"
#include "hwutil.h"
#include "pthread.h"
#include "malloc.h"

// <editor-fold defaultstate="collapsed" desc="全局变量定义">
namespace {
    /**** 字符串常量定义 ****/
    const char *TAG = "LiveRtmp";
    const char *CLASS_RMP_JNI = "com/hwilliamgo/livertmp/jni/RTMPJni";
    const char *CLASS_X264_JNI = "com/hwilliamgo/livertmp/jni/X264Jni";
    const char *CLASS_RTMP_X264_JNI = "com/hwilliamgo/livertmp/jni/RTMPX264Jni";

    /**** java类定义 ****/
    jclass jni_class_rtmp = nullptr;
    jclass jni_class_x264 = nullptr;
    jclass jni_class_rtmp_x264 = nullptr;

    /**** 引擎定义 ****/
    VideoEncoder *videoEncoder = nullptr;

    JNIEnv *jniEnv = nullptr;
    /**
     * Rtmp状态
     */
    enum class RtmpStatus : int {
        INIT,
        START,
        STOP,
        DESTROY
    };
    RtmpStatus rtmpStatus = RtmpStatus::DESTROY;

    // 工作线程id
    pthread_t pid;
    //阻塞式队列
    SafeQueue<RTMPPacket *> *globalPackets = nullptr;
}
// </editor-fold>

static void createBlockingQueue(SafeQueue<RTMPPacket *> **packets) {
    *packets = new SafeQueue<RTMPPacket *>;
    (*packets)->setWork(1);
}

// <editor-fold defaultstate="collapsed" desc="JNI函数定义-RTMP">
static void RTMP_init(JNIEnv *env, jclass clazz) {
    MyLog::init_log(LogType::SimpleLog, TAG);
    MyLog::v(__func__);
}

static jboolean RTMP_JNI_Connect(JNIEnv *env, jclass clazz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, nullptr);
    if (url) {
        MyLog::v("Java_com_hwilliamgo_livertmp_ScreenLive_connect, url=%s", url);
    }
//    链接   服务器   重试几次
    int ret = RtmpWrap::connect(url);
    env->ReleaseStringUTFChars(url_, url);
    return ret;
}

static jboolean RTMP_JNI_SendData(JNIEnv *env, jclass clazz, jbyteArray data_,
                                  jint len,
                                  jlong tms) {
    int ret;
    jbyte *data = env->GetByteArrayElements(data_, nullptr);
    ret = RtmpWrap::sendVideo(data, len, tms);
    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

static void RTMP_destroy(JNIEnv *env, jclass clazz) {
    MyLog::destroy_log();
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="JNI函数定义-RTMP_X264">
static void onVideoEncoderCallback(char *encodedData, int size) {
    if (globalPackets) {
        RTMPPacket *packet = nullptr;
        RtmpWrap::createRtmpPacket(&packet, reinterpret_cast<int8_t *>(encodedData), size);
        if (packet) {
            globalPackets->push(packet);
        }
    }
}

/**
 * RTMPX264的队列线程
 * @param args  参数
 * @return      nullptr
 */
static void *RTMPX264_thread(void *args) {
    const char *url = static_cast<const char *>(args);
    int ret = RtmpWrap::connect(url);
    free((void *) url);
    url = nullptr;
    if (!ret) {
        // 连接失败
        MyLog::d("RtmpWrap::connect failed");
        return nullptr;
    }
    // 初始化包队列
    createBlockingQueue(&globalPackets);
    // 循环取出队列中的包
    while (rtmpStatus == RtmpStatus::START) {
        RTMPPacket *packet = nullptr;
        if (globalPackets) {
            globalPackets->pop(packet);
        }
        if (!packet) {
            continue;
        }
        // 给每个包设置流Id
        packet->m_nInfoField2 = RtmpWrap::getRtmpStreamId();
        // 加入rtmp_dump自带的发送队列发送
        ret = RtmpWrap::sendVideo(*packet);
        if (!ret) {
            MyLog::e("%s %s", __func__, "sendVideo failed");
            break;
        }
    }
    return nullptr;
}

static void RTMPX264Jni_native_init(JNIEnv *env, jclass clazz) {
    rtmpStatus = RtmpStatus::INIT;
    MyLog::init_log(LogType::SimpleLog, TAG);
    MyLog::v(__func__);
    videoEncoder = new VideoEncoder();
    videoEncoder->setVideoEncodeCallback(&onVideoEncoderCallback);
}

static void RTMPX264Jni_native_setVideoEncoderInfo
        (JNIEnv *env, jclass clazz, jint width, jint heigth, jint fps, jint bitrate) {
    if (videoEncoder) {
        videoEncoder->setVideoEncInfo(width, heigth, fps, bitrate, 1);
    }
}

static void RTMPX264Jni_native_start
        (JNIEnv *env, jclass clazz, jstring path) {
    // 设置状态
    rtmpStatus = RtmpStatus::START;
    // 建立连接
    jsize length = env->GetStringLength(path);
    char *url = static_cast<char *>(calloc(length + 1, sizeof(char)));
    env->GetStringUTFRegion(path, 0, length, url);
    // 创建工作线程
    pthread_create(&pid, nullptr, RTMPX264_thread, (void *) url);
}

static void RTMPX264Jni_native_pushVideo
        (JNIEnv *env, jclass clazz, jbyteArray yuvData) {
    int8_t *byte = env->GetByteArrayElements(yuvData, nullptr);
    if (!byte) {
        env->ReleaseByteArrayElements(yuvData, byte, JNI_ABORT);
        return;
    }
    if (videoEncoder) {
        videoEncoder->encodeData(byte);
    }
    env->ReleaseByteArrayElements(yuvData, byte, JNI_ABORT);
}

static void RTMPX264Jni_native_stop(JNIEnv *env, jclass clazz) {
    rtmpStatus = RtmpStatus::STOP;
    // 销毁工作线程
    if (globalPackets) {
        // setWork函数可以释放锁，否则线程是阻塞的
        globalPackets->setWork(0);
        globalPackets->clear();
    }
    pthread_join(pid, nullptr);
    // 销毁队列
    if (globalPackets) {
        delete globalPackets;
        globalPackets = nullptr;
    }

    RtmpWrap::destroy();
}

static void RTMPX264Jni_native_release
        (JNIEnv *env, jclass clazz) {
    rtmpStatus = RtmpStatus::DESTROY;
    if (videoEncoder) {
        delete videoEncoder;
        videoEncoder = nullptr;
    }
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="JNI函数定义-X264">
/**
 * x264编码回调到java层
 * @param encodedData   编码数据
 * @param size          长度
 */
static void X264Jni_encodeCallback(char *encodedData, int size) {
    jclass x264_jni_class = jniEnv->FindClass(CLASS_X264_JNI);
    if (!x264_jni_class) {
        return;
    }
    jmethodID onEncodeMethodId = jniEnv->GetStaticMethodID(x264_jni_class, "onEncode", "([B)V");
    if (!onEncodeMethodId) {
        return;
    }
    jbyteArray ret = jniEnv->NewByteArray(size);
    if (!ret) {
        return;
    }
    jniEnv->SetByteArrayRegion(ret, 0, size, reinterpret_cast<const signed char *>(encodedData));
    jniEnv->CallStaticVoidMethod(x264_jni_class, onEncodeMethodId, ret);
    jniEnv->DeleteLocalRef(ret);
}

static void X264Jni_init(JNIEnv *env, jclass clazz) {
    MyLog::init_log(LogType::SimpleLog, TAG);
    X264Wrap::init();
    X264Wrap::setVideoEncodeCallback(X264Jni_encodeCallback);
}

static void
X264Jni_setVideoCodecInfo(JNIEnv *env, jclass clazz, int width, int height, int fps, int bitrate) {
    X264Wrap::setVideoCodecInfo(width, height, fps, bitrate);
}

static void X264Jni_encode(JNIEnv *env, jclass clazz, jbyteArray yuvData) {
    int8_t *data = env->GetByteArrayElements(yuvData, nullptr);
    X264Wrap::encode(data);
    env->ReleaseByteArrayElements(yuvData, data, JNI_ABORT);
}

static void X264Jni_destroy(JNIEnv *env, jclass clazz) {
    X264Wrap::destroy();
    MyLog::destroy_log();
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="动态加载jni函数">
static JNINativeMethod g_methods_rtmp[] = {
        {"init",     "()V",                   (void *) RTMP_init},
        {"sendData", "([BIJ)Z",               (void *) RTMP_JNI_SendData},
        {"connect",  "(Ljava/lang/String;)Z", (void *) RTMP_JNI_Connect},
        {"destroy",  "()V",                   (void *) RTMP_destroy}
};
static JNINativeMethod g_methods_x264[] = {
        {"native_init",              "()V",     (void *) X264Jni_init},
        {"native_setVideoCodecInfo", "(IIII)V", (void *) X264Jni_setVideoCodecInfo},
        {"native_encode",            "([B)V",   (void *) X264Jni_encode},
        {"native_destroy",           "()V",     (void *) X264Jni_destroy}
};
static JNINativeMethod g_methods_rtmp_x264[] = {
        {"native_init",                "()V",                   (void *) RTMPX264Jni_native_init},
        {"native_setVideoEncoderInfo", "(IIII)V",               (void *) RTMPX264Jni_native_setVideoEncoderInfo},
        {"native_start",               "(Ljava/lang/String;)V", (void *) RTMPX264Jni_native_start},
        {"native_pushVideo",           "([B)V",                 (void *) RTMPX264Jni_native_pushVideo},
        {"native_stop",                "()V",                   (void *) RTMPX264Jni_native_stop},
        {"native_release",             "()V",                   (void *) RTMPX264Jni_native_release}
};

static void registerJniMethod(JNIEnv *env,
                              jclass *jniClass,
                              const char *classFullName,
                              JNINativeMethod *allMethods,
                              int methodLength) {
    *jniClass = env->FindClass(classFullName);
    env->RegisterNatives(*jniClass, allMethods, methodLength);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != nullptr);

    registerJniMethod(env, &jni_class_rtmp, CLASS_RMP_JNI,
                      g_methods_rtmp, NELEM(g_methods_rtmp));
    registerJniMethod(env, &jni_class_x264, CLASS_X264_JNI,
                      g_methods_x264, NELEM(g_methods_x264));
    registerJniMethod(env, &jni_class_rtmp_x264, CLASS_RTMP_X264_JNI,
                      g_methods_rtmp_x264, NELEM(g_methods_rtmp_x264));
    jniEnv = env;
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {
    jniEnv = nullptr;
}
// </editor-fold>


