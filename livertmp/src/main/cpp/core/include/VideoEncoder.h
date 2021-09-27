//
// Created by HWilliamgo on 2021/8/3.
//
#ifndef BILIRTMP_VIDEOCHANNEL_H
#define BILIRTMP_VIDEOCHANNEL_H

#include <inttypes.h>
#include <jni.h>
#include "x264.h"

/**
 *
 * @param encodedData   编码后的h264数据
 * @param size          数据长度
 */
using VideoEncoderCallback = void (*)(char *encodedData, int size);
using ErrorCallback = void (*)(int errorCode, const char *msg);

/**
 * @param msg 要打印的日志
 */
using LogCallback = void (*)(const char *msg);

/**
 * 视频编码器
 */
class VideoEncoder {
public:
    const static int ERROR_FAIL_TO_CREATE_CODEC = 1;

    /**
     * 设置日志打印器
     */
    static void setLogger(LogCallback logCallback);

    VideoEncoder();

    /**
     * 设置编码器参数
     * @param width         宽
     * @param height        高
     * @param fps           帧率
     * @param bitrate       码率
     * @param repeatHeaders 是否在每个keframe前面加上SPS和PPS
     */
    void setVideoEncInfo(int width, int height, int fps, int bitrate, int repeatHeaders = 0);

    /**
     * 设置回调函数
     * @param callback 回调
     */
    void setVideoEncodeCallback(VideoEncoderCallback callback);

    /**
     * 设置错误回调
     * @param errorCode 错误码
     * @param msg       错误信息
     */
    void setErrorCallback(ErrorCallback errorCallback);

    /**
     * 编码yuv数据
     * @param data yuv数据
     */
    void encodeData(int8_t *data);

    ~VideoEncoder();


private:
    int mWidth;
    int mHeight;
    int mFps;
    int mBitrate;
    int ySize;
    int uvSize;
    int mRepeatHeaders = 0;
    // 是否是编码的第一帧
    int isFirstEncodeFrame;
    VideoEncoderCallback mVideoEncodeCallback = nullptr;
    ErrorCallback mErrorCallback = nullptr;

    //yuv-->h264 平台 容器 x264_picture_t=bytebuffer
    x264_picture_t *pic_in = nullptr;
    //编码器
    x264_t *videoCodec = nullptr;

    static void logHook(void *, int i_level, const char *psz, va_list arg);
};

#endif
