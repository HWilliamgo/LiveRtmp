package com.hwilliamgo.livertmp.jni;

/**
 * date: 2021/7/29
 * author: HWilliamgo
 * description: RTMP X264调用类
 */
public class RTMPX264Jni {
    static {
        JniLibLoader.loadLibOnce();
    }

    /**
     * 初始化
     */
    public static native void native_init();

    /**
     * 设置视频编码器信息
     *
     * @param width   宽
     * @param height  高
     * @param fps     frame per second
     * @param bitrate 码率
     */
    public static native void native_setVideoEncoderInfo(int width, int height, int fps, int bitrate);

    /**
     * 设置音频编码器信息
     *
     * @param sampleRate 采样率
     * @param channel    声道数
     */
    public static native void native_setAudioEncoderInfo(int sampleRate, int channel);

    /**
     * 开始
     *
     * @param path 服务器地址
     */
    public static native void native_start(String path);

    /**
     * 发送视频数据
     *
     * @param yuvData 裸yuv数据
     */
    public static native void native_pushVideo(byte[] yuvData);

    /**
     * 发送音频数据
     *
     * @param pcmData 裸pcm数据
     */
    public static native void natvie_pushAudio(byte[] pcmData);

    /**
     * 停止
     */
    public static native void native_stop();

    /**
     * 销毁
     */
    public static native void native_release();
}
