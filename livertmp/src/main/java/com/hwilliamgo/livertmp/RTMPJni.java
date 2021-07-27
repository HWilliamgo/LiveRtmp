package com.hwilliamgo.livertmp;

/**
 * date: 2021/7/4
 * author: HWilliamgo
 * description:
 */
public class RTMPJni {
    static {
        System.loadLibrary("hw-rtmp");
    }

    public static native void init();

    public static native boolean connect(String url);

    public static native boolean sendData(byte[] data, int len, long tms);

    public static native void destroy();
}
