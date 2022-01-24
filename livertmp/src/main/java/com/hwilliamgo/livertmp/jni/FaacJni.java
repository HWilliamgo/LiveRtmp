package com.hwilliamgo.livertmp.jni;

/**
 * @date: 2022/1/24
 * @author: HWilliamgo
 * @description: Faac的jni方法
 */
public class FaacJni {

    /**
     * 初始化音频编码器
     *
     * @param sampleRate 采样率
     * @param channels   频道
     */
    public static native void initAudioEncoder(int sampleRate, int channels);

    /**
     * 编码音频
     *
     * @param buffer pcm音频数据
     * @param len    长度
     */
    public static native void encodeAudio(byte[] buffer, int len);

    /**
     * 销毁
     */
    public static native void destroy();
}
