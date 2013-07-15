#include "Core/Process.h"
#include <QDebug>
#include <facevue.h>
#include "opencv2/highgui/highgui.hpp"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

//Process thread Constructor. 
Process::Process(FaceVuee *gui):cap (0)
{
        face_obj = new FaceVue();
	setProcessingMode (gui, REGISTRATION_MODE);
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
}

//TODO: faces database needs to be separated 
const vector<FaceSample>& 
Process::getFaceSamples () const
{
	return face_obj->recognition->Face_database;
}


//Add image to database and compute the descriptor for that
void 
Process::AddImage(const Mat &image, const string &str)
{
        face_obj->add_to_Database (image, str);
	emit ImageAdded (QString (str.c_str()));
}


//Delete image from database
void 
Process::DeleteImage(QString name)
{    
	mutex.lock();
	face_obj->remove_from_Database(name);
	mutex.unlock();
}

//Destructor
Process::~Process(void)
{
	delete mode;
        _displayImage.release();
}


void
Process::drawOverlay (bool draw)
{
	mutex.lock ();
	mode->setDrawOverlayFlag (draw);
	mutex.unlock ();
}


const QImage&
Process::displayImage ()
{
	return displayQImage;
}

const ProcessingMode*
Process::processingMode ()
{
	return mode;
}

/**************************************************
 * this slot is only called when the previous
 * image was fully loaded by an observer (eg. GUI)
 **************************************************/
void 
Process::captureNextFrame()
{
	if(!cap.isOpened())
	{
		emit unableToCapture();
		return;
	}

	Mat imageFromCamera;
	cap >> imageFromCamera;
	//TODO: try removing resize ...
	cv::resize (imageFromCamera, imageFromCamera, Size(800, 600));

	mutex.lock();
	_displayImage = mode->process(imageFromCamera);
	displayQImage = QImage ((uchar *)_displayImage.data, 
			        _displayImage.cols, 
				_displayImage.rows, 
				QImage::Format_RGB888);
	mutex.unlock();

	emit newFrameIsProcessed ();
};

//Detect the face in frame image and ?? Add it to database ??
Mat 
Process::addImage(const Mat &image, CvRect rect)
{
	Mat image_color_140;
	Mat warp_dst = face_obj->align_Face(image, rect);
	Mat out;
	//colored image

	if(face_obj->is_aligned){
		out = warp_dst;
		int val_x=abs(face_obj->target_Face->right_eye_x-face_obj->target_Face->left_eye_x);
		int val_y=abs(face_obj->target_Face->mouth_y -face_obj->target_Face->left_eye_y);
		CvRect rect2 = cvRect(abs(face_obj->target_Face->right_eye_x-2*val_x),
					  abs(face_obj->target_Face->right_eye_y-2*val_y),
					  rect.width*1.1f,
					  rect.height*1.1f);
		if (rect2.x + rect2.width > image.size().width)
			rect2.width = image.size().width - rect2.x;
		if (rect2.y + rect2.height > image.size().height)
			rect2.height = image.size().height - rect2.y;
		Mat image_color_140 = image(rect2);
		cv::resize (image_color_140, image_color_140, cvSize (140, 140)); 
		emit OutImage (out, image_color_140);
	}

	return out;
}


void 
Process::setProcessingMode (FaceVuee *gui, Mode mode)
{
	mutex.lock();
	switch (mode)
	{
		case REGISTRATION_MODE:
			this->mode = RegistrationMode::getInstance(gui, face_obj);
			break;
		case RECOGNITION_MODE:
			this->mode = RecognitionMode::getInstance(gui, face_obj);
			break;
	}
	mutex.unlock();
}

void 
Process::returnKeyPressed (bool isPressed)
{
	mutex.lock();
	mode->setReturnKeyFlag (isPressed);
	mutex.unlock();
}
