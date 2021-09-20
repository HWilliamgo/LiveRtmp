#ifndef LIVERTMP_X264_WRAP
#define LIVERTMP_X264_WRAP

#include <cstdint>
#include "VideoEncoder.h"
/**
 * x264 编码器封装
 */
namespace X264Wrap {
    /**
     * 初始化
     */
    void init();

    /**
     * 设置编码器信息
     * @param width     宽
     * @param height    高
     * @param fps       fps
     * @param bitrate   码率
     */
    void setVideoCodecInfo(int width, int height, int fps, int bitrate);

    void setVideoEncodeCallback(VideoEncoderCallback callback);

    /**
     * 编码
     * @param yuvData yuv数据
     */
    void encode(int8_t *yuvData);

    /**
     * 销毁
     */
    void destroy();
}

#endif //LIVERTMP_X264_WRAP