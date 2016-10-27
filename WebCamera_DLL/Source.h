#pragma once

#include <opencv2/opencv.hpp>


#define DLLExport __declspec (dllexport)


//using namespace cv;

//VideoWriter
cv::VideoWriter videoWriter;

extern "C" {
   DLLExport void* getCamera(int device);
   DLLExport void setCameraProp(void* camera, int device, int width, int height, int fps);
   DLLExport void releaseCamera(void* camera, int device);
   DLLExport void getCameraTexture(void* camera, unsigned char* data, bool isRecord, bool isShowWin);
}