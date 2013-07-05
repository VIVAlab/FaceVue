#include "facevue.h"
#include <QDebug>

//Constructor
FaceVue::FaceVue(): detection_threshold(130.f)
{
    bbox = (int*)malloc(4*sizeof(int));
    target_Face = new FaceContent();
    //    detection_Model=new LBPCascadeClassifier();
    is_aligned=false;
    is_Face_Found=false;
}

//Create Database
bool FaceVue::create_Database(vector<string> &filename)
{
    recognition->Face_database.clear();

    if (!(recognition->LoadImages(filename)))
    {
	    cout << "At least one file is missing: " << filename[0] << endl;
	    return false;
    }
    else
    {
	    Mat temp=Mat::zeros(128,128,DataType<uchar>::type );
	    recognition->ExtractKeypoints(temp, 4, 3 );

	    recognition->ReadClusters();
	    recognition->his_len=0;
	    for (unsigned int i=0;i<recognition->centers.size();i++)
		    recognition->his_len+=recognition->centers[i].rows;

	    for (unsigned int i=0; i< recognition->Face_database.size(); i++)
		    recognition->HistCreator(recognition->Face_database[i].image, recognition->Face_database[i].train_data_H);
	    return true;
    }
}

//Add image to Database
void FaceVue::add_to_Database(Mat image, string name)
{
    FaceSample face2;
    if(image.channels()==3)
        cvtColor(image,face2.image,CV_RGB2GRAY);
    else
        image.copyTo(face2.image);

    face2.file_address=name;
    face2.label_s=name.substr(name.find_last_of("//")+1,(name.find_last_of(".jpg")-name.find_last_of("//")-4));

    recognition->his_len=0;
    for (unsigned int i=0;i<recognition->centers.size();i++)
        recognition->his_len+=recognition->centers[i].rows;

    recognition->HistCreator(face2.image, face2.train_data_H);

    recognition->Face_database.push_back(face2);

}

//remove image from Databases
void FaceVue::remove_from_Database(const string &name)
{
    recognition->DeleteFace(name);
}

//remove image from Databases
void FaceVue::remove_from_Database(const vector<string> &name)
{
    recognition->DeleteFace(name);
}

//clear database
void FaceVue::clear_Database()
{
    recognition->Face_database.clear();
}

//Load cascade model
bool FaceVue::load_Detection_Model(const string &filename)
{
    // input should be like ->  "Models//lbpcascade_frontalface.xml"
    detection_Model = new LBPCascadeClassifier();
    if (detection_Model->load(filename))
        return true;
    else
    {
	    cout << "Unable to load face detector: " << filename[0] << endl;
	    return false;
    }
}

//Load flandmark model
bool FaceVue::load_Landmark_Model(const string &filename)
{

    // input should be like ->  "Models//flandmark_model.dat"
    landmark_Model = flandmark_init(filename.c_str());

    if (landmark_Model == 0)
    {
        printf("Structure model wasn't created. Corrupted file \n");
        return false;
    }
    else
    {
        landmarks = (double*)malloc(2*landmark_Model->data.options.M*sizeof(double));
        return true;
    }
}

//Load description model
bool FaceVue::load_Description_Model(const string &filename_params,const string &filename_clusters)
{

    // Reading parameters

    ifstream input;
    input.open(filename_params.c_str());
    if(!input.is_open())
    {
        printf("Parametrs' path is not correct \n");
        return false;
    }
    else
    {
        string str;
        while (getline( input, str))
        {
            if(str.find("m_patch")!=string::npos){
                stripSpace(str);
                description_Model.m_patch=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
            if(str.find("n_patch")!=string::npos){
                stripSpace(str);
                description_Model.n_patch=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
            if(str.find("window")!=string::npos){
                stripSpace(str);
                description_Model.window=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
            if(str.find("K_cluster")!=string::npos){
                stripSpace(str);
                description_Model.K_cluster=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
        }

        input.close();
    }

    // Reading clusters
    description_Model.clusters = new uchar[description_Model.m_patch*description_Model.n_patch*description_Model.K_cluster*59];
    FILE* clu = fopen(filename_clusters.c_str(),"rb");
    if(clu==NULL)
    {
        printf("Clusters' path is not correct \n");
        return false;
    }
    else
    {
        uchar a;
        for(int i=0;i<description_Model.m_patch*description_Model.n_patch*description_Model.K_cluster*59;i++)
        {
            fread(&a, sizeof(uchar), 1, clu);
            description_Model.clusters[i] = (uchar)a;
        }
        fclose(clu);
    }
    return true;
}

//Load rocognition model
bool FaceVue::load_Recognition_Model(const string &filename)
{
    ifstream input;
    input.open (filename.c_str());

    if(!input.is_open())
    {
        printf("Recognition params' path is not correct \n");
        return false;
    }
    else
    {
        string str;
        while (getline( input, str))
        {
            if(str.find("binThreshold")!=string::npos){
                stripSpace(str);
                recognition_Model.binThreshold=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
            if(str.find("globalThreshold")!=string::npos){
                stripSpace(str);
                recognition_Model.globalThreshold=atoi(str.substr(str.find_last_of("=")+1,str.length()).c_str());
            }
        }
        input.close();
        return true;
    }
}

//Initializing recognition module with models and parameters
void FaceVue::init_Recognition_Module(descriptionModel description_Model,
                                      recognitionModel recognition_Model)
{
    recognition=new FaceRecog();
    recognition->set_Description_Model(description_Model);
    recognition->set_recognition_Model(recognition_Model);
}

//Setting the threshhold value for face detection
void FaceVue::set_Detection_Threshold(double value)
{
    detection_threshold=value;
}

//Return regions of detected faces
vector<Rect> FaceVue::detect_ALLFacesROI(const IplImage *frame,IplImage* input)
{
    cvCvtColor(frame,input,CV_RGB2GRAY);
    std::vector<Rect> faces;
    cvEqualizeHist(input,input);

    /// TO DO What is 1.25,2, Size(30,30)
    detection_Model->detectMultiScale( input, faces, 1.25, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    target_Face->index = -1;

    return faces;
}

//Return region of detected face and target_Face is filled
CvRect FaceVue::detect_FaceROI(const IplImage *frame)
{
    is_Face_Found=false;
    IplImage* input = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
    cvCvtColor(frame,input,CV_RGB2GRAY);
    std::vector<Rect> faces;
    cvEqualizeHist(input,input);

    /// TO DO What is 1.25,2, Size(30,30)
    detection_Model->detectMultiScale( input, faces, 1.25, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    double value;
    int maxArea = 0;
    target_Face->index = -1;

    // stores detected face ROI
    CvRect rect;

    for (unsigned int iface = 0; iface < faces.size(); ++iface)
    {
        bbox[0] = faces[iface].x;
        bbox[1] = faces[iface].y;
        bbox[2] = faces[iface].x + faces[iface].width;
        bbox[3] = faces[iface].y + faces[iface].height;

        flandmark_detect(input, &value ,bbox, landmark_Model, landmarks);

        if(value > detection_threshold)
        {
            if(maxArea <=  faces[iface].width*faces[iface].height)
            {
                is_Face_Found=true;
                maxArea = faces[iface].width*faces[iface].height;
                target_Face->index = iface;
                target_Face->left_eye_x = (landmarks[4]+landmarks[12])/2;
                target_Face->left_eye_y = (landmarks[5]+landmarks[13])/2;

                target_Face->right_eye_x = (landmarks[2]+landmarks[10])/2;
                target_Face->right_eye_y = (landmarks[3]+landmarks[11])/2;

                target_Face->mouth_x = (landmarks[6]+landmarks[8])/2;
                target_Face->mouth_y = (landmarks[7]+landmarks[9])/2;

                rect =  cvRect(faces[iface].x - faces[iface].width/4,faces[iface].y -faces[iface].height/4,3*faces[iface].width/2,3*faces[iface].height/2);
                if(rect.x + rect.width > input->width)
                {
                    rect.width -= 2*(rect.width + rect.x - input->width);
                    rect.x += rect.width + rect.x -input->width;
                }
                if(rect.y + rect.height > input->height)
                {
                    rect.height -= 2*(rect.height + rect.y - input->height);
                    rect.y += rect.height + rect.y - input->height;
                }
                if(rect.x < 0)
                {
                    rect.width += 2*rect.x;
                    rect.x=0;
                }
                if(rect.y < 0)
                {
                    rect.height += 2*rect.y;
                    rect.y=0;
                }

            }
        }
    }

    if(target_Face->index!=-1)
    {

        target_Face->p1_x = faces[target_Face->index].x;
        target_Face->p1_y = faces[target_Face->index].y;
        target_Face->p2_x = faces[target_Face->index].x + faces[target_Face->index].width;
        target_Face->p2_y = faces[target_Face->index].y +faces[target_Face->index].height;

    }

    cvReleaseImage(&input);

    return rect;

}

//Return aligned face
IplImage * FaceVue::align_Face(const IplImage *frame, const CvRect &faceROI, IplImage* warp_dst)
{
    is_aligned=false;

    IplImage* input = cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
    cvCvtColor(frame,input,CV_RGB2GRAY);

    CvPoint2D32f* pt1 = new CvPoint2D32f[3];
    CvPoint2D32f* pt2 = new CvPoint2D32f[3];
    pt2[0] = cvPoint2D32f(27,46);
    pt2[1] = cvPoint2D32f(101,46);
    pt2[2] = cvPoint2D32f(64,101);

    cvZero(warp_dst);

    pt1[0] = cvPoint2D32f(target_Face->right_eye_x - faceROI.x,target_Face->right_eye_y - faceROI.y);
    pt1[1] = cvPoint2D32f(target_Face->left_eye_x - faceROI.x,target_Face->left_eye_y - faceROI.y);
    pt1[2] = cvPoint2D32f(target_Face->mouth_x - faceROI.x,target_Face->mouth_y - faceROI.y);

    if(pt1[0].x >= 0 && pt1[0].y>=0 &&
       pt1[1].x >= 0 && pt1[1].y>=0 &&
       pt1[2].x >= 0 && pt1[2].y>=0 &&
       pt1[0].x < frame->width && pt1[0].y < frame->height &&
       pt1[1].x < frame->width && pt1[1].y < frame->height&&
       pt1[2].x < frame->width && pt1[2].y < frame->height)
    {
        is_aligned=true;
        CvMat* warp_mat = cvCreateMat(2, 3, CV_64FC1);
        cvGetAffineTransform(pt1, pt2 ,warp_mat);
        IplImage* face = cvCreateImage(cvSize(faceROI.width,faceROI.height),IPL_DEPTH_8U,1);
        cvSetImageROI(input, faceROI);
        cvCopy(input, face, NULL);

        cvWarpAffine(face, warp_dst, warp_mat);

        cvReleaseImage(&face);
    }

    delete[] pt1;
    delete[] pt2;
    cvReleaseImage(&input);
    return warp_dst;

}

//Compute the image descriptor
void FaceVue::describe_Face(const Mat &face, Mat &face_descriptor)
{
    recognition->HistCreator(face,face_descriptor);
}

//Recognize face
string FaceVue::recognize_Face(const Mat &face)
{
    return recognition->Detector(face);
}

//Function for removing space in Loading information
void FaceVue::stripSpace(string &str)
{
    for (unsigned int i=0;i<str.length();i++)
    {
        if (str[i]==' ') {
            str.erase(i,1);
            i--;
        }
    }
}

//Compute Landmark
void FaceVue::my_Landmark(IplImage* input,double* value,int *bbox2)
{
    flandmark_detect(input, value ,bbox2, landmark_Model, landmarks);
}

