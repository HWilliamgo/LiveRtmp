package com.hwilliamgo.livertmp.jni;

import androidx.annotation.Nullable;

/**
 * date: 2021/8/4
 * author: HWilliamgo
 * description: x264调用的jni类
 */
public class X264Jni {
    static {
        JniLibLoader.loadLibOnce();
    }

    @Nullable
    private static OnX264JniEncodeListener onX264JniEncodeListener;

    // <editor-fold defaultstate="collapsed" desc="API">
    public static void init() {
        native_init();
    }

    public static void setVideoCodecInfo(int width, int height, int fps, int bitrate) {
        native_setVideoCodecInfo(width, height, fps, bitrate);
    }

    public static void setOnX264JniEncodeListener(@Nullable OnX264JniEncodeListener li) {
        onX264JniEncodeListener = li;
    }

    public static void encode(byte[] yuvData) {
        native_encode(yuvData);
    }

    public static void destroy() {
        native_destroy();
    }
// </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="JNI">

    /**
     * 初始化
     */
    private native static void native_init();

    /**
     * 设置编码器信息
     *
     * @param width   宽
     * @param height  高
     * @param fps     frame per second
     * @param bitrate 码率
     */
    private native static void native_setVideoCodecInfo(int width, int height, int fps, int bitrate);

    /**
     * 编码
     *
     * @param yuvData 裸yuv数据
     */
    private native static void native_encode(byte[] yuvData);

    /**
     * 销毁
     */
    private native static void native_destroy();
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="CalledByJni">

    /**
     * 编码数据回调
     *
     * @param encodeData h264码流数据
     */
    public static void onEncode(byte[] encodeData) {
        if (onX264JniEncodeListener != null) {
            onX264JniEncodeListener.onEncode(encodeData);
        }
    }
// </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="接口声明">
    public interface OnX264JniEncodeListener {
        /**
         * 编码数据回调
         *
         * @param encodeData h264码流数据
         */
        void onEncode(byte[] encodeData);
    }
    // </editor-fold>
}
