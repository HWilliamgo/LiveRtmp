package com.hwilliamgo.livertmp.jni;

/**
 * date: 2021/8/4
 * author: HWilliamgo
 * description:make sure jni lib load just once
 */
public class JniLibLoader {
    private static boolean isLibLoad = false;

    public synchronized static void loadLibOnce() {
        if (!isLibLoad) {
            isLibLoad = true;
            System.loadLibrary("hw-rtmp");
        }
    }
}
