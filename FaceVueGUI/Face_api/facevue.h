#ifndef FACEVUE_H
#define FACEVUE_H

#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "flandmark_detector.h"
#include "cascadedetect.hpp"
#include "faceRecognition.h"

using namespace cv;
using namespace std;

//Struct for Loading the model
typedef struct model {
    double * W;

} Model;



//FaceVue class for recording information of each face which has been deteceted
class FaceVue {
public:

    struct FaceContent
    {
        int left_eye_x;
        int left_eye_y;

        int right_eye_x;
        int right_eye_y;

        int mouth_x;
        int mouth_y;

        int p1_x;
        int p1_y;
        int p2_x;
        int p2_y;

        int index;
    };

    //Constructor
    FaceVue();

    //Destructors
    ~FaceVue();

    //aligned checking
    bool is_aligned;
    bool is_Face_Found;

    //threshhold for face detection
    double detection_threshold;

    //Create Database
    bool create_Database(vector<string> &filename);

    //Add image to Database
    void add_to_Database(Mat image, string name);

    //remove image from Database
    void remove_from_Database(const string &name);

    //clear database
    void clear_Database();

    //Load cascade model
    bool load_Detection_Model(const string &filename);

    //Load flandmark model
    bool load_Landmark_Model(const string &filename);

    //Load description model
    bool load_Description_Model(const string &filename_params,const string &filename_clusters);

    //Load rocognition model
    bool load_Recognition_Model(const string &filename);

    //Initializing recognition module with models and parameters
    void init_Recognition_Module(descriptionModel description_Model,recognitionModel recognition_Model);

    //Setting the threshhold value for face detection
    void set_Detection_Threshold(double value);

    //Camera Initialization
    bool init_Camera(int device);
    bool init_Camera(const string& filename);

    //Capture frame from  inited device or video file
    Mat capture_Frame(int channels=0);

    //Return regions of detected faces
    vector<Rect> detect_ALLFacesROI(const IplImage *frame,IplImage* input);

    //Return region of detected face and target_Face is filled
    CvRect detect_FaceROI(const IplImage* frame);

    //Return aligned face
    IplImage* align_Face(const IplImage* frame,const CvRect &faceROI, IplImage* warp_dst);

    //Compute the image descriptor
    void describe_Face(const Mat &face, Mat &face_descriptor);

    //Recognize face
    string recognize_Face(const Mat &face);

    //Description parameters
    descriptionModel description_Model;

    //Recognition parameters
    recognitionModel recognition_Model;

    //Compute Landmark
    void my_Landmark(IplImage* input,double* value,int *bbox);

    //Object of FaceContent class for recording and processing of each face which has been detected
    FaceContent *target_Face;

    //Object of FaceRecog class for recognition
    FaceRecog *recognition;

private:

    VideoCapture cap;
    int *bbox;
    double *landmarks;

    //Detection model
    LBPCascadeClassifier *detection_Model;

    //Alignment model
    FLANDMARK_Model *landmark_Model;

    //Function for removing space in Loading information
    static void stripSpace(string &str);

};

#endif //FACEVUE_H
