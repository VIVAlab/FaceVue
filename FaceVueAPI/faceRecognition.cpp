#include "faceRecognition.h"
#include <QDebug>

FaceRecog::FaceRecog(){

}
FaceRecog::~FaceRecog()
{
    delete[] clusters;
}

void FaceRecog::BoWInit(QVector<QString> name_file)
{
    Face_database.clear();
    Mat temp=Mat::zeros(128,128,DataType<uchar>::type );
    ExtractKeypoints(temp, 4, 3 );
    his_len=0;
    for(unsigned int i=0;i<centers.size();i++)
        his_len+=centers[i].rows;
    for (unsigned int i=0; i< Face_database.size(); i++)
        HistCreator(Face_database[i].image, Face_database[i].train_data_H);
}

void FaceRecog::AddNewFace(Mat img,Mat face_descriptor, string name){

    FaceSample face2;
    if(img.channels()==3)
        cvtColor(img,face2.image,CV_RGB2GRAY);
    else
        img.copyTo(face2.image);

    face2.file_address = name;
    face2.label_s = name;
    face_descriptor.copyTo(face2.train_data_H);
    Face_database.push_back(face2);
}

void FaceRecog::DeleteFace(const QString &name)
{

	vector<FaceSample>::iterator it = Face_database.begin();
	unsigned int index=0;
	for(unsigned int i=0;i<Face_database.size();i++)
		if(Face_database[i].label_s.compare(name.toStdString())==0)
			index=i;
	for (unsigned int i=0;i<index;i++)
		it++;
	Face_database.erase(it);
}

void FaceRecog::set_Description_Model(const descriptionModel &model)
{
    m_patch = model.m_patch;
    n_patch = model.n_patch;
    window = model.window;
    K_cluster = model.K_cluster;

    clusters = new uchar[m_patch*n_patch*K_cluster*59];

    for(int i=0; i < m_patch*n_patch*K_cluster*59 ; i++)
        clusters[i] = model.clusters[i];

}

void FaceRecog::set_recognition_Model(const recognitionModel &model)
{
    globalThreshold = model.globalThreshold;
    binThreshold = model.binThreshold;
}

void FaceRecog::LBPFilter(Mat& input,Mat& dest)
{
    dest = Mat::zeros(input.rows,input.cols,CV_8U);
    for(int i=1;i<input.rows-1;i++)
    {
        for(int j=1;j<input.cols-1;j++)
        {
            uchar b = input.at<uchar>(i,j);
            uchar out = 0;
            out += b < input.at<uchar>(i-1,j-1) ? 0:1;
            out += b < input.at<uchar>(i-1,j) ? 0:2;
            out += b < input.at<uchar>(i-1,j+1) ? 0:4;
            out += b < input.at<uchar>(i,j-1) ? 0:8;
            out += b < input.at<uchar>(i,j+1) ? 0:16;
            out += b < input.at<uchar>(i+1,j-1) ? 0:32;
            out += b < input.at<uchar>(i+1,j) ? 0:64;
            out += b < input.at<uchar>(i+1,j+1) ? 0:128;


            dest.at<uchar>(i,j) = 0;
            for(int k = 0; k < 58; k++){
                if(out == code[k]){
                    dest.at<uchar>(i,j) = k+1;
                    break;
                }
            }
        }
    }
}

void FaceRecog::norm_(Mat x, Mat *H_data, int n){
    uchar sum=0;
    Mat z(x.rows,x.cols,x.type());
    for(int i=0;i<x.cols;i++){
        sum+=(x.at<uchar>(0,i));
    }
    for(int i=0;i<x.cols;i++){
        H_data->at<uchar>(n,i)=x.at<uchar>(0,i);///(sum);
        //z.at<float>(0,i)=x.at<float>(0,i)/sqrt(sum);
    }
}

void FaceRecog::LBP(Mat img, Mat *H_data){

    int start_p_x;
    int start_p_y;
    Mat LBPed_img;
    LBPFilter(img,LBPed_img);


    for (unsigned int i=0; i<keyps.size();i++){
        start_p_x=keyps[i].x-window/2;
        start_p_y=keyps[i].y-window/2;
        Mat hist_=Mat::zeros(1,59,DataType<uchar>::type);

        for(int x=0;x<window;x++){
            for(int y=0;y<window;y++){
                //H_data->at<float>(i,LBPed_img.at<float>(start_p_x+x,start_p_y+y))+=1;
                if((start_p_x+x)<0||(start_p_y+y)<0 || (start_p_x+x)>=img.rows || (start_p_y+y)>=img.cols)
                    continue;
                hist_.at<uchar>(0,LBPed_img.at<uchar>(start_p_x+x,start_p_y+y))+=1;
            }
        }
        norm_(hist_,H_data,i);
    }


}	

int FaceRecog::MaxFinder(vector<uchar> data){
    uchar max=data[0];
    int max_num=0;
    for (unsigned int i=0; i<data.size(); i++){
        if(data[i]>max){
            max=data[i];
            max_num=i;
        }
    }
    return max_num;
}

int FaceRecog::NearestCenter(Mat data, Mat centers){

    vector<uchar> dis;
    Mat g(1,centers.cols,DataType<uchar>::type);
    for(int j=0;j<centers.rows;j++){
        centers.row(j).copyTo(g);
        dis.push_back(CompareHistInterSect(data,g));
    }
    return MaxFinder(dis);
}

void FaceRecog::HistCreator(Mat image, Mat &hist_imgs){

    Mat t1=Mat::zeros(keyps.size(),59,DataType<uchar>::type);
    LBP(image,&t1);

    hist_imgs = Mat::zeros(1,(his_len),DataType<uchar>::type);

    int stack_num=0;
    for(int j=0;j<t1.rows;j++){

        Mat g =Mat::zeros( 1, t1.cols,DataType<uchar>::type);
        t1.row(j).copyTo(g);

        int a = NearestCenter( g, centers [ key_labels[j] - 1]);
        hist_imgs.at<uchar> (0, ( a + stack_num)) += 1;
        if(j!=(t1.rows-1)){
            if(key_labels[j]!=key_labels[j+1])
                stack_num+=centers[key_labels[j] - 1].rows;
        }
    }
}


void FaceRecog::ExtractKeypoints(Mat& image, int x_step, int y_step)
{
	int size_m_patch=image.rows/m_patch;
	int size_n_patch=image.cols/n_patch;
	key_labels.clear();
	keyps.clear();

	for(int j=1;j<=n_patch;j++){

		for (int i=1;i<=m_patch;i++){
			int l;

			for(int y=(j-1)*size_n_patch;y<(j*size_n_patch);y+=y_step){

				for (int x=(i-1)*size_m_patch;x<(i*size_m_patch);x+=x_step){

					Point k(x,y);
					keyps.push_back(k);
					l=i+(j-1)*m_patch;
					key_labels.push_back(l);
				}
			}

		}
	}
}

void FaceRecog::ReadClusters(){

	int mn_num = m_patch*n_patch;

	for(int i=0;i<mn_num;i++){
		Mat temp(K_cluster,59,DataType<uchar>::type, clusters+K_cluster*59*i);
		centers.push_back(temp);
	}
}


int FaceRecog::CompareHist(Mat a, Mat b){

    int dis=0;

    for(int i=0;i<n_patch*m_patch;i++){
	    float his=0;
	    for(int j=0; j< K_cluster; j++){
		    his+=min(a.at<uchar>(0,j+i*K_cluster),b.at<uchar>(0,j+i*K_cluster));
	    }
	    if(his>binThreshold)
		    dis++;
    }
    return dis;
}

int FaceRecog::CompareHistInterSect(Mat a, Mat b){

    int dis=0;

    for(int i=0;i<a.cols;i++){
        dis+=min(a.at<uchar>(0,i),b.at<uchar>(0,i));
    }
    return dis;
}
string FaceRecog::Predictor( Mat test){

    vector<uchar> N;
    for(unsigned int j=0;j<Face_database.size();j++){

        //N.push_back(compareHist(test,train[j], CV_COMP_INTERSECT));
        int p=CompareHist(test,Face_database[j].train_data_H);
        N.push_back(p);
    }
    int t=MaxFinder(N);
    string l="";

    if(N[t]>=globalThreshold)
        l=Face_database[t].label_s.substr(0,Face_database[t].label_s.length());//-2);
    else
        l = "Unknown";
    return l;
}

string FaceRecog::Detector(Mat test_data){
    string l="";
    if(!Face_database.empty()){
        Mat test_data_H;
        HistCreator(test_data,test_data_H);
        l=Predictor(test_data_H);
    }else
        l = "Unknown";

    return l;
}
