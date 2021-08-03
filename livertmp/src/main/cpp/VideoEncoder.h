//
// Created by Administrator on 2021/1/18.
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

/**
 * 视频编码器
 */
class VideoEncoder {
public:
    VideoEncoder();

    /**
     * 设置编码器参数
     * @param width     宽
     * @param height    高
     * @param fps       帧率
     * @param bitrate   码率
     */
    void setVideoEncInfo(int width, int height, int fps, int bitrate);

    /**
     * 设置回调函数
     * @param callback 回调
     */
    void setCallback(VideoEncoderCallback callback);

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
    VideoEncoderCallback mCallback;

    //yuv-->h264 平台 容器 x264_picture_t=bytebuffer
    x264_picture_t *pic_in = nullptr;
    //编码器
    x264_t *videoCodec = nullptr;
};

#endif
