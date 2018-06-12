package com.github.namelesspeople.opencvtool;

import android.content.Context;
import android.graphics.Bitmap;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

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

    public File copyCascadeFile(Context context){
        File mCascadeFile = null;
        try {
            // load cascade file from application resources
            InputStream is = context.getApplicationContext().getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = context.getApplicationContext().getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            if(mCascadeFile.exists())
                return mCascadeFile;
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
            cascadeDir.delete();

        } catch (IOException e) {
            e.printStackTrace();
        }
        return mCascadeFile;
    }

}
