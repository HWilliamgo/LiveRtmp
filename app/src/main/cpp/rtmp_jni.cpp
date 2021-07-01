//
// Created by HWilliam on 2021/5/16.
//
#include <jni.h>
#include "rtmp_wrap.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"David",__VA_ARGS__)

#ifdef  __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL
Java_com_hwilliamgo_livertmp_ScreenLive_connect(JNIEnv *env, jobject thiz, jstring url_) {

    const char *url = env->GetStringUTFChars(url_, nullptr);
    if (url) {
        LOGI("Java_com_hwilliamgo_livertmp_ScreenLive_connect, url=%s", url);
    }
//    链接   服务器   重试几次
    int ret = RtmpWrap::connect(url);
    env->ReleaseStringUTFChars(url_, url);
    return ret;

}

JNIEXPORT jboolean JNICALL
Java_com_hwilliamgo_livertmp_ScreenLive_sendData(JNIEnv *env, jobject thiz, jbyteArray data_,
                                                 jint len,
                                                 jlong tms) {
    int ret;
    jbyte *data = env->GetByteArrayElements(data_, nullptr);
    ret = RtmpWrap::sendVideo(data, len, tms);
    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

#ifdef  __cplusplus
}
#endif

