//
// Created by HWilliam on 2022/1/24.
//
#include <faac.h>

#ifndef FUZZY_VIDEO_DRILL_AUDIOENCODER_H
#define FUZZY_VIDEO_DRILL_AUDIOENCODER_H

/**
 *
 * @param encodedData   编码后的aac数据
 * @param size          数据长度
 */
using AudioEncoderCallback = void (*)(unsigned char *encodedData, int size, bool isHead);
using ErrorCallback = void (*)(int errorCode, const char *msg);

/**
 * @param msg 要打印的日志
 */
using LogCallback = void (*)(const char *msg);

class AudioEncoder {
public:
    AudioEncoder();

    ~AudioEncoder();

    void setAudioEncoderInfo(int sampleRate, int channels);

    void encode(int32_t *data, int len);

    void setAudioEncoderCallback(AudioEncoderCallback cb);

    void setLogCallback(LogCallback cb);

    unsigned long getInputByteNum() const;

private:
    // core
    faacEncHandle encoder = nullptr;

    // 回调
    AudioEncoderCallback encodeCallback = nullptr;
    LogCallback logCallback = nullptr;


    unsigned char *outputBuffer = nullptr;

    // 编码器输出的数据的最大长度
    unsigned long maxOutputBytes = 0;
    // 编码器每次输入的数据的长度（注意，必须严格按照这个大小来输入，否则报崩溃）
    unsigned long inputByteNum = 0;

    // 是否是编码的第一帧
    int isFirstEncodeFrame = 1;
};

#endif //FUZZY_VIDEO_DRILL_AUDIOENCODER_H
