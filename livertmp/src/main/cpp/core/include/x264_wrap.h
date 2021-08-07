#ifndef LIVERTMP_X264_WRAP
#define LIVERTMP_X264_WRAP

#include <cstdint>

namespace X264Wrap {
    void init();

    void setVideoCodecInfo(int width, int height, int fps, int bitrate);

    void encode(int8_t *yuvData);

    void destroy();
}

#endif //LIVERTMP_X264_WRAP