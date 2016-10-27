#include "Source.h"
#include "DebugLogWrapper.h"

DLLExport void* getCamera(int device)
{
    // �E�B���h�E���J��
    cv::namedWindow("web camera");


	//-1��������Đ����[�h
	if(device != -1)
	{
		//���ʂ�WebCamera�������Ă���Ƃ�
		return static_cast<void*>(new cv::VideoCapture(device));
	}
	else
	{
		//�^��Đ��p	
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
		//�^��p
		videoWriter = cv::VideoWriter("capture.avi", CV_FOURCC('X','V','I','D'), fps, cv::Size(width, height));
	}
}

DLLExport void releaseCamera(void* camera, int device)
{
    // �E�B���h�E�����
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
		//�^��p
		videoWriter = cv::VideoWriter("capture.avi", CV_FOURCC('X','V','I','D'), vc->get(CV_CAP_PROP_FPS), cv::Size((int)(vc->get(CV_CAP_PROP_FRAME_WIDTH)), (int)(vc->get(CV_CAP_PROP_FRAME_WIDTH))));

	}


		// �J������̎擾
		cv::Mat img;
		*vc >> img;

		if(!img.empty())
		{
			//�^��t���O�������Ă�����^�������
			if(isRecord) videoWriter << img;

			//std::cout << "img size:" << img.rows << " * " << img.cols << std::endl;
    
			if(isShowWin)
			{
				// ���T�C�Y
				cv::Mat resized_img(img.rows * 0.5, img.cols * 0.5, img.type());
				cv::resize(img, resized_img, resized_img.size(), cv::INTER_CUBIC);
    
				// �ʃE�B���h�E�̉���X�V
				cv::imshow("web camera", resized_img);
			}
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
}