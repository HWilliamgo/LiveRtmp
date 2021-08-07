package com.hwilliamgo.livertmp.jni;

/**
 * date: 2021/8/4
 * author: HWilliamgo
 * description:
 */
public class X264Jni {
    static {
        JniLibLoader.loadLibOnce();
    }

    public native static void init();

    public native static void setVideoCodecInfo(int width, int height, int fps, int bitrate);

    public native static void encode(byte[] yuvData);

    public native static void destroy();

    /**
     * 编码数据回调
     *
     * @param encodeData h264码流数据
     */
    public static void onEncode(byte[] encodeData) {

    }
}
