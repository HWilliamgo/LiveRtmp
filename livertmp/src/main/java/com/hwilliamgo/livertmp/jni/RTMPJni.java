package com.hwilliamgo.livertmp.jni;

/**
 * date: 2021/7/4
 * author: HWilliamgo
 * description: Rtmp的jni调用类
 */
public class RTMPJni {
    static {
        JniLibLoader.loadLibOnce();
    }

    /**
     * 初始化
     */
    public static native void init();

    /**
     * 用rtmp协议连接服务器
     *
     * @param url 连接地址
     * @return 是否连接成功
     */
    public static native boolean connect(String url);

    /**
     * 发送编码数据
     *
     * @param data 编码好的数据
     * @param len  长度
     * @param tms  时间戳
     * @return 是否发送成功
     */
    public static native boolean sendData(byte[] data, int len, long tms);

    /**
     * 销毁
     */
    public static native void destroy();
}
