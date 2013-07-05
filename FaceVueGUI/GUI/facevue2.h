#ifndef FaceVuee_H
#define FaceVuee_H
#include <QtGui/QMainWindow>
#include "ui_facevue.h"
#include <vector>
#include "Core/ProcessThread.h"
#include <QEvent>
#include <QKeyEvent>
#include <QFile>
#include <facevue.h>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

using namespace std;
class FaceVuee : public QMainWindow
{
	Q_OBJECT

public:
        FaceVuee(QWidget *parent = 0, Qt::WFlags flags = 0);
        ~FaceVuee();
	bool isReturnPressed ();

protected:
        void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
private:
        Ui::FaceVueClass ui;
	ProcessThread* process;
	vector<string> FindImages(string in);
        void SaveImage(string str,IplImage* img,Mat &img_rgb);
	QPalette *red_Palette;
	QPalette *green_Palette;
        int last_frame;
        bool isImage_filled;
        bool flag;
        IplImage* image_gray;
        Mat image_color;
	bool keyPressed;
private slots:
        void DrawImage(FaceVue::FaceContent*);
	void ChangeMode(int);
        void Logging(char *,unsigned long);
        void addImg_to_database();
//        void selectImage(QListWidgetItem *);
	//void takePicture();
        void OutImage(IplImage*,Mat);


	void DeleteImage();
	void FaceInElipse();
	void Beep();
};

#endif // FaceVuee_H
