package com.hwilliamgo.livertmp.jni;

/**
 * date: 2021/7/29
 * author: HWilliamgo
 * description:
 */
public class RTMPX264Jni {
    static {
        JniLibLoader.loadLibOnce();
    }

    public static native void native_init();

    public static native void native_setVideoEncoderInfo(int width, int height, int fps, int bitrate);

    public static native void native_start(String path);

    public static native void native_pushVideo(byte[] yuvData);

    public static native void native_stop();

    public static native void native_release();
}
