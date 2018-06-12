//
// Created by netli on 2018/6/1.
//

#include <jni.h>
#include <opencv/cv.h>

#ifndef GRAYSCALEPHOTO_BITMAP2MAT_H
#define GRAYSCALEPHOTO_BITMAP2MAT_H

#endif //GRAYSCALEPHOTO_BITMAP2MAT_H
using namespace cv;
void bitmap2Mat(JNIEnv *env, jobject bitmap, Mat &mat);
void mat2Bitmap(JNIEnv *pEnv, Mat mat, jobject bitmap);