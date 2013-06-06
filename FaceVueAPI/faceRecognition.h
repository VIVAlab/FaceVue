#ifndef __FACE_RECOGNITION__
#define __FACE_RECOGNITION__

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <time.h>
#include "faceRecognition.h"

using namespace cv;
using namespace std;

//Description model for computing the descriptor
struct descriptionModel
{
    int m_patch;
    int n_patch;
    int window;
    int K_cluster;

    uchar* clusters;

    descriptionModel():m_patch(8),n_patch(8),window(8),K_cluster(20){}
};

//recognition Model for recognize a face
struct recognitionModel
{
    int binThreshold;
    int globalThreshold;
    recognitionModel():binThreshold(12),globalThreshold(40){}
};

//Code array for computing the LBP U2
static int code[] = {0,1,2,3,4,6,7,8,12,14,15,16,24,28,30,31,32,48,56,60,62,63,64,96,112,120,124,126,127,128,129,131,135,143,159
                     ,191,192,193,195,199,207,223,224,225,227,231,239,240,241,243,247,248,249,251,252,253,254,255};

//Structure for recording each face information
struct FaceSample
{
    Mat image;
    Mat train_data_H;
    string label_s;
    string file_address;
};


class FaceRecog {


public:
    vector<FaceSample> Face_database;

    FaceRecog();
    ~FaceRecog();
    string Detector(Mat test_data);

    //Add new face to database
    void AddNewFace(Mat img,Mat face_descriptor, string name);

    //Remove face from database
    void DeleteFace(const string &name);
    void DeleteFace(const vector<string> &name);

    //Set the Description model parameteres
    void set_Description_Model(const descriptionModel &model);

    //Set the Recognition model parameteres
    void set_recognition_Model(const recognitionModel &model);

    //Computing the descriptor for each face
    void HistCreator(Mat image, Mat &hist_imgs);

    //Computing Keypoints
    void ExtractKeypoints(Mat& image, int x_step, int y_step);

    //Loading Images
    bool LoadImages(vector<string> file_path);

    //Loading cluster files
    void ReadClusters();

    void BoWInit(vector<string> name_file);

    vector<Mat> centers;
    vector<int> key_labels;
    vector<Point> keyps;
    int his_len;

    int m_patch;
    int n_patch;
    int window;
    int K_cluster;

    uchar* clusters;

    int binThreshold;
    int globalThreshold;

    //Compute LBP U2 for face image
    void LBPFilter(Mat& input,Mat& dest);

    //Normlize historgram for computing face descriptor
    void norm_(Mat x, Mat *H_data, int n);

    //Compute LBP U2 for face image
    void LBP(Mat img, Mat *H_data);

    //Computing Maximum value
    int MaxFinder(vector<uchar> data);

    //Find the nearest cluster to our data in each subwindow of face
    int NearestCenter(Mat data, Mat centers);

    //Compare two histogram
    int CompareHistInterSect(Mat a, Mat b);

    //Compare two histogram
    int CompareHist(Mat a, Mat b);

    //Recognize the face
    string Predictor( Mat test);
};


#endif
