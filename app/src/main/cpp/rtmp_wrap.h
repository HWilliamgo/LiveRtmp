//
// Created by HWilliam on 2021/5/16.
//

#ifndef RTMPBIBILI_RTMP_WRAP_H
#define RTMPBIBILI_RTMP_WRAP_H

#include "librtmp/rtmp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Live {
    RTMP *rtmp;

    int16_t sps_len;
    int8_t *sps;

    int16_t pps_len;
    int8_t *pps;
} Live;

/**
 * rtmp 建立连接
 * @param url url
 * @return 1表示成功，0表示失败
 */
int connect(const char *url);

/**
 * 发送视频编码数据
 * @param buf 编码视频数据
 * @param len 数据长度
 * @param tms 时间戳
 * @return
 */
int sendVideo(int8_t *buf, int len, long tms);

#ifdef __cplusplus
};
#endif

#endif //RTMPBIBILI_RTMP_WRAP_H
