#include "x264_wrap.h"
#include "VideoEncoder.h"
#include "log_abs.h"
#include "hwutil.h"

namespace {
    VideoEncoder *videoEncoder = nullptr;
    const char *TAG = nullptr;
}

namespace X264Wrap {
    void videoEncoderLogCallback(const char *msg) {
        MyLog::dTag(TAG, "%s", msg);

    }

    void errorCallback(int errorCode, const char *msg) {
        MyLog::dTag(TAG, "errorCode=%d, msg=%s", errorCode, msg);
    }
}

void X264Wrap::init() {
    TAG = HWUtils::getFileNameFromPath(__FILE__);
    if (videoEncoder) {
        delete videoEncoder;
        videoEncoder = nullptr;
    }
    videoEncoder = new VideoEncoder();
    videoEncoder->setErrorCallback(errorCallback);
    VideoEncoder::setLogger(X264Wrap::videoEncoderLogCallback);
}

void X264Wrap::setVideoCodecInfo(int width, int height, int fps, int bitrate) {
    if (videoEncoder) {
        videoEncoder->setVideoEncInfo(width, height, fps, bitrate);
    }
}

void X264Wrap::setVideoEncodeCallback(VideoEncoderCallback callback) {
    if (videoEncoder) {
        videoEncoder->setVideoEncodeCallback(callback);
    }
}

void X264Wrap::encode(int8_t *yuvData) {
    if (videoEncoder) {
        videoEncoder->encodeData(yuvData);
    }
}

void X264Wrap::destroy() {
    if (videoEncoder) {
        delete videoEncoder;
        videoEncoder = nullptr;
    }
    if (TAG) {
        delete TAG;
        TAG = nullptr;
    }
}
