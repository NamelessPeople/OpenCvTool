package com.github.namelesspeople.opencvtool;

import android.graphics.Bitmap;

import java.io.File;

/**
 * 类描述：
 * 创建人： 张力
 * 创建时间： 2018/5/31
 * 版权： 成都智慧一生约科技有限公司
 */
public class OpenCvUtils {
    static {
        System.loadLibrary("native-lib");
    }
    public native void grayscalePhoto(Bitmap bitmap);

    public native void loadCascade(String mCascadePath);

    public native void faceRecognition(Bitmap bitmap);
    public native void pictrueRecognition(Bitmap bitmap1,Bitmap bitmap2);
}
