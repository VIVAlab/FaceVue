#ifndef FaceVuee_H
#define FaceVuee_H
#include <vector>
#include "Core/ProcessThread.h"
#include <QEvent>
#include <QKeyEvent>
#include <QFile>
#include <facevue.h>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <QWaitCondition>
#include <QLabel>
#include <QMainWindow>
#include "ui_facevue.h"

class RegistrationMode;
using namespace std;
class FaceVuee : public QMainWindow
{
	Q_OBJECT
	friend class RegistrationMode;
	friend class RecognitionMode;

	public:
		FaceVuee(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~FaceVuee();

	protected:
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);

	private:
		Ui::FaceVueClass ui;
		Process* process;
		QThread* processThread;
		void LoadAllImages ();
		bool nameAlreadyExists (const string &name) const;
		bool SaveImage(string &str, const Mat &img, const Mat &img_rgb);
		bool isImage_filled;
		bool flag;
		Mat image_gray;
		Mat image_color;
		const char* getFaceDir();

		//functions for updating the UI 
		void updateUserInterfaceInRecognitionMode ();
		void updateUserInterfaceInRegistrationMode ();
		void updateLabelKeepAspectRatio (QLabel *label, const QImage &image);

	private slots:
		//TabWidget changemode
		void ChangeMode(int);

		//display image functions
		void OutImage(Mat,Mat);
		void ApplyRecognizedFace (QString name);

		//Table modification functions
		void InsertIntoTable (QString name);
		void addImg_to_database();
		void DeleteImage();

		//UI update functions
		void updateUserInterface ();
		
		//UI error diplayers
		void displayErrorUnableToCapture ();

	signals:
		//emitted when the UI is ready 
		//for the processing thread
		//to submit its next image
		void readyForNextImage();
};

#endif // FaceVuee_H
