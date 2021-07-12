//
// Created by HWilliam on 2021/5/16.
//

/**
 * jni函数，面向Java
 *
 * 初始化
 * RTMP_init
 *
 * 连接
 * RTMP_JNI_Connect
 *
 * 发送数据
 * RTMP_JNI_SendData
 *
 * 销毁
 * RTMP_destroy
 */

#include <jni.h>
#include "rtmp_wrap.h"
#include <android/log.h>
#include <cassert>
#include "log/include/log_abs.h"

namespace {
    const char *TAG = "LiveRtmp";
    const char *CLASS_RMP_JNI = "com/hwilliamgo/livertmp/RTMPJni";
    jclass jni_class;
}

static void RTMP_init(JNIEnv *env, jclass clazz) {
    MyLog::init_log(LogType::SimpleLog, TAG);
    MyLog::v(__func__);
}

static jboolean RTMP_JNI_SendData(JNIEnv *env, jclass clazz, jbyteArray data_,
                                  jint len,
                                  jlong tms) {
    int ret;
    jbyte *data = env->GetByteArrayElements(data_, nullptr);
    ret = RtmpWrap::sendVideo(data, len, tms);
    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

static jboolean RTMP_JNI_Connect(JNIEnv *env, jclass clazz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, nullptr);
    if (url) {
        MyLog::v("Java_com_hwilliamgo_livertmp_ScreenLive_connect, url=%s", url);
    }
//    链接   服务器   重试几次
    int ret = RtmpWrap::connect(url);
    env->ReleaseStringUTFChars(url_, url);
    return ret;
}

static void RTMP_destroy(JNIEnv *env, jclass clazz) {
    MyLog::destroy_log();
}

static JNINativeMethod g_methods[] = {
        {"init",     "()V",                   (void *) RTMP_init},
        {"sendData", "([BIJ)Z",               (void *) RTMP_JNI_SendData},
        {"connect",  "(Ljava/lang/String;)Z", (void *) RTMP_JNI_Connect},
        {"destroy",  "()V",                   (void *) RTMP_destroy}
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != nullptr);

    jni_class = env->FindClass(CLASS_RMP_JNI);
    env->RegisterNatives(jni_class, g_methods, sizeof(g_methods) / sizeof(g_methods[0]));

    //初始化

    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {

}


