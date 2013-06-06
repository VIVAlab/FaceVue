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

using namespace cv;
using namespace std;

enum Mode{Registration,Recognition};

class ProcessThread :public  QThread
{
    Q_OBJECT
public:

    //Process thread Constructor. We Load all the models and images to the database here.
    ProcessThread(vector<string>);

    //Destructor
    ~ProcessThread(void);

    static QMutex mutex;
    Mat image;
    Mode mode;
    bool isStopped;

    //"Run" part of the process thread.
    void run();

    //Detect the face in frame image and Add it to database
    IplImage* addImage(IplImage* image,Mat& image_color_140,double thresh,CvRect rect);

    //Find the face region in a frame
    void findFace(IplImage* image ,double thresh);
    bool takePicture;
    bool dialogShowed;
    bool key_pressed;
    bool key_flag;
    int capture_cnt;
    unsigned long frame_cnt;
    int take_picture_cnt;
    //Add image to database and compute the descriptor for that
    void AddImage(const Mat& image,const string& str);

    //Delete image from database
    void DeleteImage2(vector<string> name);
    FaceVue *face_obj;

private:

    char* name;

    //Detect the face in image and recognize it
    void detectFaceInImage(IplImage* image ,double thresh);

signals:
    void DrawImage(FaceVue::FaceContent*);
    void Logging(char*,unsigned long);
    void OutImage(IplImage*,Mat);
    void FaceInElipse();
    void Beep();
};

#endif 
