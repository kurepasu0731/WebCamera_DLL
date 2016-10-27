#include "Source.h"
#include "DebugLogWrapper.h"

DLLExport void* getCamera(int device)
{
    // ウィンドウを開く
    cv::namedWindow("web camera");


	//-1だったら再生モード
	if(device != -1)
	{
		//普通にWebCameraから取ってくるとき
		return static_cast<void*>(new cv::VideoCapture(device));
	}
	else
	{
		//録画再生用	
		return static_cast<void*>(new cv::VideoCapture("capture.avi"));
	}
}

DLLExport void setCameraProp(void* camera, int device, int width, int height, int fps)
{
    auto vc = static_cast<cv::VideoCapture*>(camera);

	vc->set(CV_CAP_PROP_FRAME_WIDTH, width);
	vc->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	vc->set(CV_CAP_PROP_FPS, fps);

	if(device != -1)
	{
		//録画用
		videoWriter = cv::VideoWriter("capture.avi", CV_FOURCC('X','V','I','D'), fps, cv::Size(width, height));
	}
}

DLLExport void releaseCamera(void* camera, int device)
{
    // ウィンドウを閉じる
    cv::destroyWindow("web camera");
	if(device != -1) videoWriter.release();
    auto vc = static_cast<cv::VideoCapture*>(camera);
    delete vc;
}

DLLExport void getCameraTexture(void* camera, unsigned char* data, bool isRecord, bool isShowWin)
{
    auto vc = static_cast<cv::VideoCapture*>(camera);

	if(isRecord && !videoWriter.isOpened())
	{
		//録画用
		videoWriter = cv::VideoWriter("capture.avi", CV_FOURCC('X','V','I','D'), vc->get(CV_CAP_PROP_FPS), cv::Size((int)(vc->get(CV_CAP_PROP_FRAME_WIDTH)), (int)(vc->get(CV_CAP_PROP_FRAME_WIDTH))));

	}


		// カメラ画の取得
		cv::Mat img;
		*vc >> img;

		if(!img.empty())
		{
			//録画フラグがたっていたら録画もする
			if(isRecord) videoWriter << img;

			//std::cout << "img size:" << img.rows << " * " << img.cols << std::endl;
    
			if(isShowWin)
			{
				// リサイズ
				cv::Mat resized_img(img.rows * 0.5, img.cols * 0.5, img.type());
				cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);
    
				// 別ウィンドウの画を更新
				cv::imshow("web camera", resized_img);
			}
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
}