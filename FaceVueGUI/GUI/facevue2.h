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
		bool isReturnKeyPressed ();

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
		bool keyPressed;
		const char* getFaceDir();

	private slots:
		void ChangeMode(int);
		void addImg_to_database();
		void OutImage(Mat,Mat);
		void DeleteImage();
		void drawImage (QImage *img, QWaitCondition *cond, QMutex *mutex, QLabel *label);
		void InsertIntoTable (QString name);
		void ApplyRecognizedFace (QString name);
		void updateUserInterface ();
		void displayErrorUnableToCapture ();

	signals:
		void readyForNextImage();
};

#endif // FaceVuee_H
