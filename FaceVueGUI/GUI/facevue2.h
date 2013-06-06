#ifndef FaceVuee_H
#define FaceVuee_H
#include <QtGui/QMainWindow>
#include "ui_FaceVue.h"
#include <vector>
#include "Core/ProcessThread.h"
#include <QEvent>
#include <QKeyEvent>
#include <QFile>
#include <FaceVue.h>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

using namespace std;
class FaceVuee : public QMainWindow
{
	Q_OBJECT

public:
        FaceVuee(QWidget *parent = 0, Qt::WFlags flags = 0);
        ~FaceVuee();

protected:
        void keyPressEvent(QKeyEvent *);
private:
        Ui::FaceVueClass ui;
	ProcessThread* process;
	vector<string> FindImages(string in);
        void SaveImage(string str,IplImage* img,Mat &img_rgb);
	QPalette *red_Palette;
	QPalette *green_Palette;
        int last_frame;
        char* last_label;
        bool isImage_filled;
        bool flag;
        IplImage* image_gray;
        Mat image_color;
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
