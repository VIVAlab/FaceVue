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
#include <QString>

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
    void DeleteFace(const QString &name);

    //Set the Description model parameteres
    void set_Description_Model(const descriptionModel &model);

    //Set the Recognition model parameteres
    void set_recognition_Model(const recognitionModel &model);

    //Computing the descriptor for each face
    void HistCreator(Mat image, Mat &hist_imgs);

    //Computing Keypoints
    void ExtractKeypoints(Mat& image, int x_step, int y_step);

    //Loading Images
    bool LoadImages(QVector<QString> file_path);

    //Loading cluster files
    void ReadClusters();

    void BoWInit(QVector<QString> name_file);

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
