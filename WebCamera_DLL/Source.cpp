#include "Source.h"

DLLExport void* getCamera(int device)
{
    // ウィンドウを開く
    cv::namedWindow("web camera");
    return static_cast<void*>(new cv::VideoCapture(device));
}

DLLExport void setCameraProp(void* camera, int width, int height, int fps)
{
    auto vc = static_cast<cv::VideoCapture*>(camera);

	vc->set(CV_CAP_PROP_FRAME_WIDTH, width);
	vc->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	vc->set(CV_CAP_PROP_FPS, fps);
}

DLLExport void releaseCamera(void* camera)
{
    // ウィンドウを閉じる
    cv::destroyWindow("web camera");
    auto vc = static_cast<cv::VideoCapture*>(camera);
    delete vc;
}

DLLExport void getCameraTexture(void* camera, unsigned char* data)
{
    auto vc = static_cast<cv::VideoCapture*>(camera);
    
    // カメラ画の取得
    cv::Mat img;
    *vc >> img;

	//std::cout << "img size:" << img.rows << " * " << img.cols << std::endl;
    
    // リサイズ
	cv::Mat resized_img(img.rows * 0.5, img.cols * 0.5, img.type());
    cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);
    
    // 別ウィンドウの画を更新
    cv::imshow("web camera", resized_img);
    
	//↓↓処理するのはこっちに戻ってやるのだからx反転色変換いらないのでは？？
    // RGB --> ARGB 変換
 //   cv::Mat argb_img, flip_img;
 //   //cv::cvtColor(resized_img, argb_img, CV_RGB2BGRA);
 //   cv::cvtColor(img, argb_img, CV_RGB2BGRA);
 //   std::vector<cv::Mat> bgra;
 //   cv::split(argb_img, bgra);
 //   std::swap(bgra[0], bgra[3]);
 //   std::swap(bgra[1], bgra[2]);
	////x軸反転
	//cv::flip(argb_img, flip_img, 0);
 //   std::memcpy(data, flip_img.data, flip_img.total() * flip_img.elemSize());

    std::memcpy(data, img.data, img.total() * img.elemSize());
}