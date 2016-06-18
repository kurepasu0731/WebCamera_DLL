#include "Source.h"

DLLExport void* getCamera(int device)
{
    // �E�B���h�E���J��
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
    // �E�B���h�E�����
    cv::destroyWindow("web camera");
    auto vc = static_cast<cv::VideoCapture*>(camera);
    delete vc;
}

DLLExport void getCameraTexture(void* camera, unsigned char* data)
{
    auto vc = static_cast<cv::VideoCapture*>(camera);
    
    // �J������̎擾
    cv::Mat img;
    *vc >> img;

	//std::cout << "img size:" << img.rows << " * " << img.cols << std::endl;
    
    // ���T�C�Y
	cv::Mat resized_img(img.rows * 0.5, img.cols * 0.5, img.type());
    cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);
    
    // �ʃE�B���h�E�̉���X�V
    cv::imshow("web camera", resized_img);
    
	//������������̂͂������ɖ߂��Ă��̂�����x���]�F�ϊ�����Ȃ��̂ł́H�H
    // RGB --> ARGB �ϊ�
 //   cv::Mat argb_img, flip_img;
 //   //cv::cvtColor(resized_img, argb_img, CV_RGB2BGRA);
 //   cv::cvtColor(img, argb_img, CV_RGB2BGRA);
 //   std::vector<cv::Mat> bgra;
 //   cv::split(argb_img, bgra);
 //   std::swap(bgra[0], bgra[3]);
 //   std::swap(bgra[1], bgra[2]);
	////x�����]
	//cv::flip(argb_img, flip_img, 0);
 //   std::memcpy(data, flip_img.data, flip_img.total() * flip_img.elemSize());

    std::memcpy(data, img.data, img.total() * img.elemSize());
}