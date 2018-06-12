#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)


using namespace cv;

void bitmap2Mat(JNIEnv *env, jobject bitmap, Mat &mat) {


    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    //锁画布
    mat.create(info.height, info.width, CV_8UC4);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        //    CV_8UC4 对 ARGB_8888
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        //吧数据temp 复制到mat
        temp.copyTo(mat);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        //    CV_8UC2 对 RGB_565
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        //    CV_8UC4 ->CV_8UC2
        cvtColor(temp, mat, COLOR_BGR5652BGRA);
    }
    //解锁画布
    AndroidBitmap_unlockPixels(env, bitmap);
}

void mat2Bitmap(JNIEnv *env, Mat mat, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;
    //锁画布
    AndroidBitmap_getInfo(env, bitmap, &info);
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC4) {
            mat.copyTo(temp);
        } else if (mat.type() == CV_8UC2) {
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        } else if (mat.type() == CV_8UC1) {
            cvtColor(mat, temp, COLOR_GRAY2BGRA);
        }
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        if (mat.type() == CV_8UC4) {
            cvtColor(mat, temp, COLOR_BGRA2BGR565);
        } else if (mat.type() == CV_8UC2) {
            mat.copyTo(temp);
        } else if (mat.type() == CV_8UC1) {
            cvtColor(mat, temp, COLOR_GRAY2BGR565);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}
