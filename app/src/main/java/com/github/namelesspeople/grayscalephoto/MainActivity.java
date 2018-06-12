package com.github.namelesspeople.grayscalephoto;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.github.namelesspeople.opencvtool.OpenCvUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    ImageView imageView;
    Button button;
    Bitmap bitmap;
    Bitmap bitmap2;
    ImageView grayImage;
    OpenCvUtils openCvUtils;
    File mCascadeFile;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView = findViewById(R.id.image);
        grayImage = findViewById(R.id.image2);
        button = findViewById(R.id.change);
        bitmap = BitmapFactory.decodeResource(getResources(),R.mipmap.timg2);
        bitmap2 = BitmapFactory.decodeResource(getResources(),R.mipmap.timg1);

        imageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imageView.setImageBitmap(bitmap);
            }
        });
        imageView.setImageBitmap(bitmap);
        grayImage.setImageBitmap(bitmap2);
        openCvUtils = new OpenCvUtils();
        mCascadeFile = openCvUtils.copyCascadeFile(this);
        openCvUtils.loadCascade(mCascadeFile.getAbsolutePath());

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openCvUtils.faceRecognition(bitmap);
            }
        });
    }

}
