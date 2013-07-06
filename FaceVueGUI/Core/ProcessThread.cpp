#include "Core/ProcessThread.h"
#include <QDebug>
#include <facevue.h>
#include "opencv2/highgui/highgui.hpp"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

//Process thread Constructor. We Load all the models and images to the database here.
ProcessThread::ProcessThread(FaceVuee *gui, vector<string> imagess)
{
	frame_cnt = 0;
	setProcessingMode (gui, REGISTRATION_MODE);
        face_obj=new FaceVue();
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
        face_obj->create_Database(imagess);


        isStopped = false;
        name = new char[100];
}

//Add image to database and compute the descriptor for that
void ProcessThread::AddImage(const Mat &image,const string &str)
{
        face_obj->add_to_Database(image,str);
}


//Delete image from database
void ProcessThread::DeleteImage2(vector<string> name)
{    
	mutex.lock();
	face_obj->remove_from_Database(name);
	mutex.unlock();
}

//Destructor
ProcessThread::~ProcessThread(void)
{
        image.release();
        delete[] name;
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
		mode->process(img);
		mutex.unlock();
	}
};

//Find the face region in a frame
void ProcessThread::findFace(IplImage* image)
{
    int *bbox2=(int*)malloc(4*sizeof(int));
    IplImage* input = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);
    std::vector<Rect> faces=face_obj->detect_ALLFacesROI(image,input);
    double value;

    for (unsigned int iface = 0; iface < faces.size(); ++iface)
    {
        bbox2[0] = faces[iface].x;
        bbox2[1] = faces[iface].y;
        bbox2[2] = faces[iface].x + faces[iface].width;
        bbox2[3] = faces[iface].y + faces[iface].height;
        face_obj->my_Landmark(input,&value,bbox2);
    }

    cvReleaseImage(&input);
    delete[] bbox2;
}

//Detect the face in frame image and Add it to database
IplImage* ProcessThread::addImage(IplImage* image,Mat& image_color_140, CvRect rect)
{
	//CvRect rect=face_obj->detect_FaceROI(image);
	IplImage* warp_dst = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	face_obj->align_Face(image,rect, warp_dst);
	IplImage* out = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
	//colored image
	bool captured=false;

	if(face_obj->is_aligned){
		captured = true;
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
		Mat temp(destination);
		image_color_140=temp.clone();
		cvReleaseImage(&destination);
	}

	if(!captured)
		cvReleaseImage(&out);

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
