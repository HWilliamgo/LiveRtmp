package com.hwilliamgo.livertmp;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {
    private MediaProjectionManager mediaProjectionManager;
    private MediaProjection mediaProjection;
    ScreenLive screenLive;
    String url = "rtmp://sendtc3.douyu.com/live/6458908rmvq5ljmC?wsSecret=52121ea8374c22e3a55ef008714b0594&wsTime=60ec5a06&wsSeek=off&wm=0&tw=0&roirecognition=0&record=flv&origin=tct";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        checkPermission();
    }
    public boolean checkPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(
                Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            }, 1);

        }
        return false;
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == 100 && resultCode == Activity.RESULT_OK) {
//         mediaProjection--->产生录屏数据
            mediaProjection = mediaProjectionManager.getMediaProjection
                    (resultCode, data);
            screenLive = new ScreenLive();
            screenLive.startLive(url, mediaProjection);
        }
    }



    public void startLive(View view) {
        this.mediaProjectionManager = (MediaProjectionManager)getSystemService(Context.MEDIA_PROJECTION_SERVICE);
        Intent captureIntent = mediaProjectionManager.createScreenCaptureIntent();
        startActivityForResult(captureIntent, 100);
    }
}
