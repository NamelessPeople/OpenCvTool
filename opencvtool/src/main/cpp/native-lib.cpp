#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <android/log.h>
#include "bitmap2mat.cpp"
#include "bitmap2mat.h"

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "xfeatures2d/nonfree.hpp"
#include "xfeatures2d/cuda.hpp"
#include "xfeatures2d.hpp"
#include "surf.hpp"
#include "surf.cpp"
#include "sift.cpp"
#include "precomp.hpp"

#include "xfeatures2d_init.cpp"
#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
using namespace std;
using namespace xfeatures2d;

using namespace cv;
CascadeClassifier cascadeClassifier;
extern "C" {
JNIEXPORT void JNICALL

Java_com_github_namelesspeople_opencvtool_OpenCvUtils_grayscalePhoto(JNIEnv *env,
                                                                     jobject instance,
                                                                     jobject bitmap);
JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_loadCascade(JNIEnv *env, jobject instance,
                                                                  jstring mCascadePath_);

JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_faceRecognition(JNIEnv *env, jobject instance,
                                                                      jobject bitmap);
JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_pictrueRecognition(JNIEnv *env,
                                                                         jobject instance,
                                                                         jobject bitmap1,
                                                                         jobject bitmap2);
void detectKeypoints(const Mat& image, vector<KeyPoint>& keypoints);
void extractDescriptors(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptor);
void bestMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<DMatch>& matches);
//void knnMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<vector<DMatch>>& matches, int k);
void saveMatches(const Mat& queryImage,
                 const vector<KeyPoint>& queryKeypoints,
                 const Mat& trainImage,
                 const vector<KeyPoint>& trainKeypoints,
                 const vector<DMatch>& matches,
                 const string& saveFileName,
                 Mat& outMat);
}


JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_loadCascade(JNIEnv *env, jobject instance,
                                                                  jstring mCascadePath_) {
    const char *mCascadePath = env->GetStringUTFChars(mCascadePath_, 0);


    cascadeClassifier.load(mCascadePath);


    env->ReleaseStringUTFChars(mCascadePath_, mCascadePath);
}

JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_grayscalePhoto(JNIEnv *env,
                                                                     jobject instance,
                                                                     jobject bitmap) {

    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //灰度化
    Mat gray_mat;
    cvtColor(mat, gray_mat, COLOR_BGRA2GRAY);

    //灰度化的图片变成bitmap
    mat2Bitmap(env, gray_mat, bitmap);

}


JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_faceRecognition(JNIEnv *env, jobject instance,
                                                                      jobject bitmap) {
    Mat mat;
    bitmap2Mat(env, bitmap, mat);
    //灰度化
    Mat gray_mat;
    cvtColor(mat, gray_mat, COLOR_BGRA2GRAY);
    //直方均衡
    Mat equalize_mat;
    equalizeHist(gray_mat,equalize_mat);
    vector<Rect> faces;
    cascadeClassifier.detectMultiScale(equalize_mat,faces,1.1,5);

    if(faces.size()==1){
        Rect faceRect = faces[0];

        //在人脸部分画图
        rectangle(mat,faceRect,Scalar(255,155,155),4);

        //保存人脸信息 Mat，
    }

    //灰度化的图片变成bitmap
    mat2Bitmap(env, mat, bitmap);

}
void detectKeypoints(const Mat& image, vector<KeyPoint>& keypoints)
{
    assert(image.type() == CV_8UC1);
//    assert(!m_detectType.empty());

    keypoints.clear();
    Ptr<SurfFeatureDetector> m_detector = SURF::create(800);



    m_detector->detect(image, keypoints);

}

void extractDescriptors(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptor)
{
    assert(image.type() == CV_8UC1);
//    assert(!m_extractType.empty());

//    m_extractor = DescriptorExtractor::create(m_extractType);
    Ptr<SurfDescriptorExtractor>m_extractor=SURF::create(800);
    m_extractor->compute(image, keypoints, descriptor);

}

void bestMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<DMatch>& matches)
{
    assert(!queryDescriptor.empty());
    assert(!trainDescriptor.empty());
    matches.clear();
    Ptr<DescriptorMatcher> m_matcher = DescriptorMatcher::create("BruteForce");
    m_matcher->add(vector<Mat>(1, trainDescriptor));
    m_matcher->train();
    m_matcher->match(queryDescriptor, matches);

}


void saveMatches(const Mat& queryImage,
                          const vector<KeyPoint>& queryKeypoints,
                          const Mat& trainImage,
                          const vector<KeyPoint>& trainKeypoints,
                          const vector<DMatch>& matches,
                          const string& saveFileName,
                 Mat &outImage)
{
    assert(!saveFileName.empty());


    drawMatches(queryImage, queryKeypoints, trainImage, trainKeypoints, matches, outImage,
                    Scalar(255, 0, 0), Scalar(0, 255, 255), vector<char>(),  DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //
    string saveMatchImgName = saveFileName + "_" + "SIFT" + "_" + "SIFT" + "_" + "BruteForce" ;

}
int m( Mat queryImage,Mat trainImage ,Mat &outMat)
{




    vector<KeyPoint> queryKeypoints, trainKeypoints;
    detectKeypoints(queryImage, queryKeypoints);
    detectKeypoints(trainImage, trainKeypoints);


    Mat queryDescriptor, trainDescriptor;


    extractDescriptors(queryImage, queryKeypoints, queryDescriptor);
    extractDescriptors(trainImage, trainKeypoints, trainDescriptor);


    vector<DMatch> matches;
    bestMatch(queryDescriptor, trainDescriptor, matches);

//    vector<vector<DMatch>> knnmatches;
//    knnMatch(queryDescriptor, trainDescriptor, knnmatches, 2);


    saveMatches(queryImage, queryKeypoints, trainImage, trainKeypoints, matches, "../",outMat);


    return 0;
}

JNIEXPORT void JNICALL
Java_com_github_namelesspeople_opencvtool_OpenCvUtils_pictrueRecognition(JNIEnv *env,
                                                                         jobject instance,
                                                                         jobject bitmap1,
                                                                         jobject bitmap2) {

    Mat mat1,mat2;
    bitmap2Mat(env, bitmap1, mat1);
    bitmap2Mat(env, bitmap2, mat2);
    //灰度化
    Mat gray_mat1,gray_mat2;
    cvtColor(mat1, gray_mat1, COLOR_BGRA2GRAY);
    cvtColor(mat2, gray_mat2, COLOR_BGRA2GRAY);
    Mat outMat;
    m(gray_mat1,gray_mat2,outMat);

    //灰度化的图片变成bitmap
    mat2Bitmap(env, outMat, bitmap1);
}