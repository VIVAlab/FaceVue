#ifndef PROCESS_H
#define PROCESS_H

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

class Process : public  QObject
{
	Q_OBJECT
	public:

		Process (FaceVuee* gui);
		~Process(void);
		Mat addImage(const Mat &image, CvRect rect);
		void findFace(IplImage* image);
		void AddImage(const Mat& image,const string& str);
		void DeleteImage(QString name);
		void setProcessingMode (FaceVuee *gui, Mode mode);
		void returnKeyPressed (bool isPressed);
		void drawOverlay (bool draw);
		const QImage& displayImage ();
		const ProcessingMode* processingMode ();

		//TODO: faces database needs to be separated 
		const vector<FaceSample>& getFaceSamples () const;


	private:
		Mat _displayImage; 	//used to ensure qimage has a buffer  
		QImage displayQImage;   //image to display
		QMutex mutex; 		//used to sync the processing mode
		ProcessingMode* mode;
		cv::VideoCapture cap;
		FaceVue *face_obj;

	public slots:
		void captureNextFrame ();

	signals:
		void OutImage(Mat, Mat);    //TODO: RENAME & MERGE WITH ImageAdded
		void ImageAdded (QString);  
		void newFrameIsProcessed();
		void unableToCapture ();
};

#endif 
