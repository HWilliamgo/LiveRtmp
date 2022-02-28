//
// Created by HWilliam on 2022/1/24.
//
#include <cstring>
#include "AudioEncoder.h"
#include "malloc.h"
#include "log_abs.h"

AudioEncoder::AudioEncoder() {

}

AudioEncoder::~AudioEncoder() {
    if (encoder) {
        faacEncClose(encoder);
        encoder = nullptr;
    }
    if (outputBuffer) {
        delete outputBuffer;
        outputBuffer = nullptr;
    }
}

void AudioEncoder::setAudioEncoderInfo(int sampleRate, int channels) {
    encoder = faacEncOpen(sampleRate, channels, &inputByteNum, &maxOutputBytes);
    MyLog::d("编码器inputByteNum=%d", inputByteNum);
    // 初始化输出容器
    outputBuffer = static_cast<unsigned char *>(calloc(maxOutputBytes, sizeof(unsigned char)));

    // 编码器配置
    faacEncConfigurationPtr config = faacEncGetCurrentConfiguration(encoder);
    config->mpegVersion = MPEG4;
    config->aacObjectType = LOW;
    //  (0 = Raw; 1 = ADTS)
    config->outputFormat = 1;
    config->inputFormat = FAAC_INPUT_16BIT;
    faacEncSetConfiguration(encoder, config);
}

void AudioEncoder::encode(int32_t *data, int len) {
    if (!encoder) {
        return;
    }
    if (!outputBuffer) {
        return;
    }

    if (isFirstEncodeFrame) {
        isFirstEncodeFrame = 0;
        u_char *headBuf = nullptr;
        u_long headLen;
        faacEncGetDecoderSpecificInfo(encoder, &headBuf, &headLen);
        if (encodeCallback) {
            encodeCallback(headBuf, headLen, true);
        }
        if (headBuf) {
            delete headBuf;
            headBuf = nullptr;
        }
    }

    int byteLen = faacEncEncode(encoder, data, len, outputBuffer, maxOutputBytes);
    if (byteLen > 0) {
        if (encodeCallback) {
            encodeCallback(outputBuffer, byteLen, false);
        }
    }
}

void AudioEncoder::setAudioEncoderCallback(AudioEncoderCallback cb) {
    this->encodeCallback = cb;
}

void AudioEncoder::setLogCallback(LogCallback cb) {
    this->logCallback = cb;
}

unsigned long AudioEncoder::getInputByteNum() const {
    return inputByteNum;
}
