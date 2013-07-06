#include "RecognitionMode.h"
#include <GUI/facevue2.h>

RecognitionMode::RecognitionMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
}

/* recognize a face in the image and then draw it */
Mat
RecognitionMode::process (IplImage *img)
{
	Mat m;
	return m;
	/*
	//recognize
	CvRect rect=face_obj->detect_FaceROI(image);
	IplImage* warp_dst = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	face_obj->align_Face(image,rect,warp_dst);
	if(face_obj->is_aligned)
	{
		string x=face_obj->recognize_Face(warp_dst);
		if(x.compare("unknown"))
			sprintf(name,"%s",x.c_str());
		else
			sprintf(name,"Unknown");
	}
	else
		sprintf(name,"Unknown");
	emit Logging(name,frame_cnt);
	cvReleaseImage(&warp_dst);

	//common operation for drawing
	Mat img;
	ProcessThread::mutex.lock();
	cvtColor(process->image,img,CV_BGR2RGB);
	ProcessThread::mutex.unlock();

	Point center = Point( img.cols/2, img.rows/2 );
	double angle = 90.0;
	double scale = 1;

	Mat rot_mat = getRotationMatrix2D( center, angle, scale );
	Mat rot_mat2 = getRotationMatrix2D( center, -1.f*angle, scale );


	//recognition-based drawing block
	ui.deleteBTN->setVisible(false);

	ui.lineEdit->setVisible(false);
	ui.add_BTN->setVisible(false);
	ui.label_name->setVisible(false);
	ui.Lbl_nameR->setVisible(true);
	if(ui.overlayCKB->isChecked() && f->index!=-1)
	{
		circle(img,Point(f->right_eye_x,f->right_eye_y),3,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(f->left_eye_x,f->left_eye_y),3,Scalar(0,0,255),CV_FILLED);
		circle(img,Point(f->mouth_x,f->mouth_y),3,cvScalar(0,255,255),CV_FILLED);
		rectangle(img,Point(f->p1_x,f->p1_y),Point(f->p2_x,f->p2_y),cvScalar(255,0,0),2);
	}
	//warpAffine( img, img, rot_mat, img.size());
	//flip(img,img,0);
	//warpAffine( img, img, rot_mat2, img.size());

	QImage image = QImage((uchar *)img.data, img.cols,img.rows, QImage::Format_RGB888);
	ui.recognitionDisplayLBL->setPixmap(QPixmap::fromImage(image));
	*/
}

QLabel*
RecognitionMode::getProperLabel()
{
	return gui->ui.recognitionDisplayLBL;
}

