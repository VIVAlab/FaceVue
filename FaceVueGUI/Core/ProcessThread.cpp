#include "Core/ProcessThread.h"
#include <QDebug>
#include <facevue.h>
#include "opencv2/highgui/highgui.hpp"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

void
ProcessThread::setName (const string &name)
{
	this->name = name;
}

//Process thread Constructor. 
ProcessThread::ProcessThread(FaceVuee *gui)
{
	frame_cnt = 0;
        face_obj = new FaceVue();
	mode = new RegistrationMode (gui, face_obj);
#if defined(Q_OS_WIN32)
        face_obj->load_Detection_Model("Models//lbpcascade_frontalface.xml");
        face_obj->load_Landmark_Model("Models//flandmark_model.dat");
        face_obj->load_Description_Model("Models//Description_Model.txt","Models//clusters.bin");
        face_obj->load_Recognition_Model("Models//Recognition_Model.txt");
#elif defined(Q_OS_UNIX)
	//FACE_ETC_DIR must be defined in the .pro file
        face_obj->load_Detection_Model(STR(FACEVUE_ETC_DIR) "Models/lbpcascade_frontalface.xml");
        face_obj->load_Landmark_Model(STR(FACEVUE_ETC_DIR) "Models/flandmark_model.dat");
        face_obj->load_Description_Model(STR(FACEVUE_ETC_DIR) "Models/Description_Model.txt", 
			                 STR(FACEVUE_ETC_DIR) "Models/clusters.bin");
        face_obj->load_Recognition_Model(STR(FACEVUE_ETC_DIR) "Models/Recognition_Model.txt");
#endif         

        face_obj->init_Recognition_Module(face_obj->description_Model,face_obj->recognition_Model);
	face_obj->create_Database();

        isStopped = false;
}

//Add image to database and compute the descriptor for that
void ProcessThread::AddImage(const Mat &image,const string &str)
{
        face_obj->add_to_Database (image, str);
	emit ImageAdded (QString (str.c_str()), image.clone());
}


//Delete image from database
void ProcessThread::DeleteImage(QString name)
{    
	mutex.lock();
	face_obj->remove_from_Database(name);
	mutex.unlock();
}

//Destructor
ProcessThread::~ProcessThread(void)
{
        image.release();
}

//"Run" part of the process thread.
void ProcessThread::run()
{
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		qDebug () << "Video capture (cap) was unable to start... ";
		return;
	}
	frame_cnt=0;
	Mat tmpImage;
	while(!(this->isStopped))
	{
		frame_cnt++;
		cap >> tmpImage;
		resize(tmpImage,image,Size(800,600));

		mutex.lock();
		IplImage *img = &(IplImage)image;
		Mat matimg = mode->process(img);
		QImage qimg((uchar *)matimg.data, matimg.cols, matimg.rows, QImage::Format_RGB888);
		QLabel *label = mode->getProperLabel();
		mutex.unlock();

		QWaitCondition cond;
		QMutex drawMutex;
		drawMutex.lock();
		emit drawImage (&qimg, &cond, &drawMutex, label);
		cond.wait (&drawMutex);
		drawMutex.unlock();
	}
};

//Detect the face in frame image and Add it to database
IplImage* ProcessThread::addImage(IplImage* image, CvRect rect)
{
	Mat image_color_140;
	IplImage* warp_dst = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	face_obj->align_Face(image,rect, warp_dst);
	IplImage* out = NULL;
	//colored image

	if(face_obj->is_aligned){
		out = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
		cvCopyImage(warp_dst,out);
		int val_x=abs(face_obj->target_Face->right_eye_x-face_obj->target_Face->left_eye_x);
		int val_y=abs(face_obj->target_Face->mouth_y -face_obj->target_Face->left_eye_y);
		CvRect rect2 = cvRect(abs(face_obj->target_Face->right_eye_x-2*val_x),
					  abs(face_obj->target_Face->right_eye_y-2*val_y),
					  rect.width*1.1f,
					  rect.height*1.1f);
		cvSetImageROI(image, rect2);
		IplImage *destination = cvCreateImage( cvSize(140,140),image->depth, image->nChannels );
		cvResize(image, destination);
		image_color_140 = Mat(destination).clone();
		cvReleaseImage(&destination);
		emit OutImage (out, image_color_140);
	}

	cvReleaseImage(&warp_dst);
	return out;
}


void 
ProcessThread::setProcessingMode (FaceVuee *gui, Mode mode)
{
	mutex.lock();
	delete this->mode;
	switch (mode)
	{
		case REGISTRATION_MODE:
			this->mode = new RegistrationMode(gui, face_obj);
			break;
		case RECOGNITION_MODE:
			this->mode = new RecognitionMode(gui, face_obj);
			break;
	}
	mutex.unlock();
}
