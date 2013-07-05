#include "RegistrationMode.h"

RegistrationMode::RegistrationMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
}

void 
RegistrationMode::process (IplImage *img)
{
	//process image for registration
	IplImage z = *img;
	CvRect rect=facevue->detect_FaceROI(&z);
	if(dialogShowed)
		mutex.unlock();
	else
	{
		if(takePicture )
		{

			cnt++;

			if(cnt<10)
			{
				mutex.unlock();
			}
			else
			{
				IplImage* out = 0;
				Mat out_color_140;
				out = addImage(&(IplImage)image,out_color_140 ,facevue->detection_threshold,rect);
				emit OutImage(out,out_color_140);
				cnt = 0;
				//                                key_pressed=false;
				take_picture_cnt=0;
			}
		}
		else
		{
			findFace(&(IplImage)image ,facevue->detection_threshold);
		}
	}

	//common stuff for drawing
	Mat img;
	ProcessThread::mutex.lock();
	cvtColor(process->image,img,CV_BGR2RGB);
	ProcessThread::mutex.unlock();

	Point center = Point( img.cols/2, img.rows/2 );
	double angle = 90.0;
	double scale = 1;

	Mat rot_mat = getRotationMatrix2D( center, angle, scale );
	Mat rot_mat2 = getRotationMatrix2D( center, -1.f*angle, scale );

	//registration-based stuff for drawing
	//f is FaceContent*
	ui.deleteBTN->setVisible(true);
	ui.lineEdit->setVisible(true);
	ui.add_BTN->setVisible(true);
	ui.label_name->setVisible(true);
	ui.Lbl_nameR->setVisible(false);
	if(f->index!=-1)
	{
		circle(img,Point(f->right_eye_x,f->right_eye_y),3,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(f->left_eye_x,f->left_eye_y),3,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(f->mouth_x,f->mouth_y),3,cvScalar(0,255,255),CV_FILLED);
		rectangle(img,Point(f->p1_x,f->p1_y),Point(f->p2_x,f->p2_y),cvScalar(255,0,0),2);
	}
	//  warpAffine( img, img, rot_mat, img.size());
	//flip(img,img,0);
	//  warpAffine( img, img, rot_mat2, img.size());

	//        ellipse(img,Point(img.cols/2,img.rows/2),Size(126,90),90,0,360,Scalar(0,255,0),2);
	//        circle(img,Point(img.cols/2 - 50 , img.rows/2 - 30),5,Scalar(255,0,0),2);
	//        circle(img,Point(img.cols/2 + 50 , img.rows/2 - 30),5,Scalar(255,0,0),2);
	QImage image = QImage((uchar *)img.data, img.cols,img.rows, QImage::Format_RGB888);
	ui.registrationDisplayLBL->setPixmap(QPixmap::fromImage(image));
}

