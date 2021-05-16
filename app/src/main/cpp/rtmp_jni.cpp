//
// Created by HWilliam on 2021/5/16.
//
#include <jni.h>
#include "rtmp_wrap.h"

#ifdef  __cplusplus
extern "C" {
#endif


JNIEXPORT jboolean JNICALL
Java_com_hwilliamgo_livertmp_ScreenLive_connect(JNIEnv *env, jobject thiz, jstring url_) {

    const char *url = env->GetStringUTFChars(url_, 0);
//    链接   服务器   重试几次
    int ret = connect(url);
    env->ReleaseStringUTFChars(url_, url);
    return ret;

}

JNIEXPORT jboolean JNICALL
Java_com_hwilliamgo_livertmp_ScreenLive_sendData(JNIEnv *env, jobject thiz, jbyteArray data_,
                                                 jint len,
                                                 jlong tms) {
    int ret;
    jbyte *data = env->GetByteArrayElements(data_, nullptr);
    ret = sendVideo(data, len, tms);
    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

#ifdef  __cplusplus
}
#endif

