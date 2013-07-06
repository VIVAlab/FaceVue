#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QMutex>
#include <QThread>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "flandmark_detector.h"
//#include "cascadedetect.hpp"
//#include "faceRecognition.h"
#include <facevue.h>
#include "RegistrationMode.h"
#include "RecognitionMode.h"
#include <QMutex>
#include <QLabel>
#include <QWaitCondition>

using namespace cv;
using namespace std;

enum Mode {
	REGISTRATION_MODE,
	RECOGNITION_MODE
};

class FaceVuee;  //foreward declaration

class ProcessThread : public  QThread
{
	Q_OBJECT
	public:

		//Process thread Constructor. We Load all the models and images to the database here.
		ProcessThread(FaceVuee* gui, vector<string>);

		//Destructor
		~ProcessThread(void);

		Mat image;
		bool isStopped;

		//"Run" part of the process thread.
		void run();

		//Detect the face in frame image and Add it to database
		IplImage* addImage(IplImage* image, CvRect rect);

		//Find the face region in a frame
		void findFace(IplImage* image);

		//Add image to database and compute the descriptor for that
		void AddImage(const Mat& image,const string& str);

		//Delete image from database
		void DeleteImage2(vector<string> name);
		FaceVue *face_obj;
		void setProcessingMode (FaceVuee *gui, Mode mode);

	private:
		QMutex mutex; //used to sync the processing mode
		ProcessingMode *mode;
		char* name;
		unsigned int frame_cnt;

	signals:
		void Logging(char*,unsigned long);
		void OutImage(IplImage*,Mat);
		void Beep();
		void drawImage(QImage*, QWaitCondition*, QMutex*, QLabel*);
};

#endif 
