#include "Core\ProcessThread.h"
#include <QDebug>
#include <facevue.h>
#include "opencv2/highgui/highgui.hpp"

//Process thread Constructor. We Load all the models and images to the database here.
ProcessThread::ProcessThread(vector<string> imagess)
{
        mode = Registration;
        takePicture = false;
        face_obj=new FaceVue();
        face_obj->load_Detection_Model("Models//lbpcascade_frontalface.xml");
        face_obj->load_Landmark_Model("Models//flandmark_model.dat");
        face_obj->load_Description_Model("Models//Description_Model.txt","Models//clusters.bin");
        face_obj->load_Recognition_Model("Models//Recognition_Model.txt");
        face_obj->init_Recognition_Module(face_obj->description_Model,face_obj->recognition_Model);
        face_obj->create_Database(imagess);


        isStopped = false;
        name = new char[100];
        dialogShowed = false;
        key_pressed=false;
        take_picture_cnt;
        key_flag=true;
}

//Add image to database and compute the descriptor for that
void ProcessThread::AddImage(const Mat &image,const string &str)
{
        face_obj->add_to_Database(image,str);
}


//Delete image from database
void ProcessThread::DeleteImage2(vector<string> name)
{
    face_obj->remove_from_Database(name);
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
            qDebug () << "Video capture (cap) was unable to start. No usb-camera is mounted or the driver of the device is missing ... ";
            return;
        }
        int cnt = 0;
        frame_cnt=0;
        Mat tmpImage;
        while(!(this->isStopped))
        {
                frame_cnt++;
                mutex.lock();
                cap >> tmpImage;
                resize(tmpImage,image,Size(800,600));
                if(mode==Recognition)
                        detectFaceInImage( &(IplImage)image,face_obj->detection_threshold );
                else if(mode==Registration)
                {
                    IplImage z=image;
                    CvRect rect=face_obj->detect_FaceROI(&z);
                    if(dialogShowed)
                        mutex.unlock();
                    else
                    {
                        if(takePicture )
                        {

                            cnt++;

                            if(cnt<10)
                            {
                                mutex.unlock();
                            }
                            else
                            {
                                IplImage* out = 0;
                                Mat out_color_140;
                                out = addImage(&(IplImage)image,out_color_140 ,face_obj->detection_threshold,rect);
                                emit OutImage(out,out_color_140);
                                cnt = 0;
//                                key_pressed=false;
                                take_picture_cnt=0;
                            }
                        }
                        else
                        {
                            findFace(&(IplImage)image ,face_obj->detection_threshold);
                        }
                    }
                }
                emit DrawImage(face_obj->target_Face);
        }
};

//Find the face region in a frame
void ProcessThread::findFace(IplImage* image ,double thresh)
{
    mutex.unlock();
    int *bbox2=(int*)malloc(4*sizeof(int));
    IplImage* input = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,1);
    std::vector<Rect> faces=face_obj->detect_ALLFacesROI(image,input);
    takePicture = false;
    double value;
    if(key_pressed){
        take_picture_cnt++;
    }
    if(take_picture_cnt>=50){
        key_flag=true;
    }
    for (int iface = 0; iface < faces.size(); ++iface)
    {
        bbox2[0] = faces[iface].x;
        bbox2[1] = faces[iface].y;
        bbox2[2] = faces[iface].x + faces[iface].width;
        bbox2[3] = faces[iface].y + faces[iface].height;
        face_obj->my_Landmark(input,&value,bbox2);
        if(value > face_obj->detection_threshold)
        {
                key_flag=true;
                if (key_pressed)
                {
                    if(take_picture_cnt<50)
                        takePicture = true;

                    key_pressed=false;
                    take_picture_cnt=0;

                    break;
                }

        }
//        key_pressed=false;
    }
    cvReleaseImage(&input);
    delete[] bbox2;

}

//Detect the face in frame image and Add it to database
IplImage* ProcessThread::addImage(IplImage* image,Mat& image_color_140 ,double thresh, CvRect rect)
{
    dialogShowed = true;
    mutex.unlock();
//    CvRect rect=face_obj->detect_FaceROI(image);
    IplImage* warp_dst = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
    face_obj->align_Face(image,rect, warp_dst);
    IplImage* out = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
    // colored image
    bool captured=false;

    if(face_obj->is_aligned){
        captured = true;
        cvCopyImage(warp_dst,out);
        int val_x=abs(face_obj->target_Face->right_eye_x-face_obj->target_Face->left_eye_x);
        int val_y=abs(face_obj->target_Face->mouth_y -face_obj->target_Face->left_eye_y);
        CvRect rect2 =cvRect(abs(face_obj->target_Face->right_eye_x-2*val_x),abs(face_obj->target_Face->right_eye_y-2*val_y),rect.width*1.1f,rect.height*1.1f);
        cvSetImageROI(image, rect2);
        IplImage *destination = cvCreateImage( cvSize(140,140),image->depth, image->nChannels );
        cvResize(image, destination);
        Mat temp(destination);
        image_color_140=temp.clone();
        cvReleaseImage(&destination);
    }

    if(!captured)
    {
        cvReleaseImage(&out);
    }
    cvReleaseImage(&warp_dst);

    return out;
}


//Detect the face in image and recognize it
void ProcessThread::detectFaceInImage(IplImage* image ,double thresh)
{
        mutex.unlock();
        CvRect rect=face_obj->detect_FaceROI(image);
        IplImage* warp_dst = cvCreateImage(cvSize(128,128),IPL_DEPTH_8U,1);
        face_obj->align_Face(image,rect,warp_dst);
        if(face_obj->is_aligned)
        {
            string x=face_obj->recognize_Face(warp_dst);
            if(x.compare("unknown"))
                sprintf(name,"%s",x.c_str());
            else
                sprintf(name,"Unknown");
        }
        else
            sprintf(name,"Unknown");
        emit Logging(name,frame_cnt);
        cvReleaseImage(&warp_dst);
}
