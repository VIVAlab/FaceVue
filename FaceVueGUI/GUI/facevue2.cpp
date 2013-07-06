#include "GUI/facevue2.h"
#include <QDir>
#include <QMessageBox>
#include "GUI/addimage.h"
#include <QTableWidget>
#include <QSize>
#include <QDebug>
#include <QLabel>
#include <stdio.h>

// TODO: for windows there are better alternatives 
// (the installation directory can be registered using QSetting and retreived)
// This can be investigated
// The current solution assumes the application is always executed from the
// working directory. (this is a strong assumption)
#if defined(Q_OS_UNIX)
	#define FACE_DIR "~/.facevue/face/"
#elif defined(Q_OS_WIN32)
	#define FACE_DIR "./face/"
#endif

FaceVuee::FaceVuee(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	keyPressed = false;
	flag=true;
	ui.setupUi(this);
	vector<string> images = FindImages(FACE_DIR);

	//    ui.imageslistLST->setSelectionBehavior(QAbstractItemView::SelectRows);
	//    for(int i = 0;i<images.size();i++)
	//    {
	//        ui.imageslistLST->insertItem(i,QString(images[i].substr(images[i].find_last_of("\\")+1,images[i].length() - images[i].find_last_of("\\") - 9).c_str()));
	//    }

	isImage_filled=false;
	QTableWidgetItem *tWidget;

	ui.tableWidget->insertColumn(0);
	ui.tableWidget->setAutoScroll(true);

	for(unsigned int i=0; i<images.size(); i++)
	{

		tWidget=new QTableWidgetItem();
		tWidget->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		ui.tableWidget->insertRow(i);

		Mat img_cv;
		QString str=QString((images[i].substr(0,images[i].length()-8)+"_rgb.jpg").c_str());
		img_cv=imread(str.toStdString(),1);
		cv::cvtColor(img_cv,img_cv,CV_BGR2RGB);
		QImage sidebar_project_icon_Image((uchar*)img_cv.data, 
				img_cv.cols, 
				img_cv.rows,
				img_cv.step, 
				QImage::Format_RGB888);
		QIcon sidebar_project_icon_Icon(QPixmap::fromImage(sidebar_project_icon_Image));
		tWidget->setText(QString(images[i].substr(images[i].find_last_of("\\")+1,
						images[i].length() - images[i].find_last_of("\\") - 11).c_str()));
		tWidget->setWhatsThis(QString(images[i].substr(images[i].find_last_of("//")+1,
						images[i].length() - images[i].find_last_of("//") - 9).c_str()));
		tWidget->setIcon(sidebar_project_icon_Icon);

		ui.tableWidget->setItem(i,0,tWidget);


	}   

	red_Palette = new QPalette();
	red_Palette->setColor(QPalette::Window, Qt::red);

	green_Palette = new QPalette();
	green_Palette->setColor(QPalette::Window, Qt::green);

	ui.FaceD->setAutoFillBackground(true);
	ui.FaceD->setPalette(*red_Palette);

	ui.FaceR->setAutoFillBackground(true);
	ui.FaceR->setPalette(*red_Palette);

	QImage img(":/FaceVue/Resources/unknown.jpg");
	ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));

	last_frame=0;   

	connect(ui.add_BTN,SIGNAL(clicked()),this,SLOT(addImg_to_database()));
	connect(ui.deleteBTN,SIGNAL(clicked()),this,SLOT(DeleteImage()));
	connect(ui.widgetTAB,SIGNAL(currentChanged(int)),this,SLOT(ChangeMode(int)));

	process = new ProcessThread(this, images);


	//connect(process,SIGNAL(DrawImage(FaceVue::FaceContent*)),this,SLOT(DrawImage(FaceVue::FaceContent*)));
	connect(process,SIGNAL(Logging( char *,unsigned long)),this,SLOT(Logging(char *,unsigned long)));
	connect(process,
	  	SIGNAL(drawImage(QImage*, QWaitCondition*, QMutex*, QLabel*)),
		this,
		SLOT(drawImage(QImage*, QWaitCondition*, QMutex*, QLabel*)),
		Qt::QueuedConnection);

	qRegisterMetaType< Mat >("Mat");

	connect(process,SIGNAL(OutImage(IplImage*,Mat)),this,SLOT(OutImage(IplImage*,Mat)));

	connect(process,SIGNAL(Beep()),this,SLOT(Beep()));
	process->start();
}

FaceVuee::~FaceVuee()
{
    delete red_Palette;
    delete green_Palette;
    process->isStopped = true;
    process->wait();
    delete process;        
}
void FaceVuee::Beep()
{
    QApplication::beep();
}
void FaceVuee::addImg_to_database()
{
    if(isImage_filled)
    {
        QString str;
        str = ui.lineEdit->text();
        SaveImage(str.toStdString(),image_gray,image_color);

        cvReleaseImage(&image_gray);
        image_color.release();
        ui.lineEdit->clear();
        isImage_filled=false;

	QImage img (":/FaceVue/Resources/unknown.jpg");
        ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));
        ui.lineEdit->setText("New face added");
    }
    else
    {
        ui.lineEdit->setText("No face detected");
    }
}

void FaceVuee::OutImage(IplImage* image,Mat image_color_140)
{    
	flag=true;

	if(image)
	{
		IplImage* img = cvCreateImage(cvSize(128,128),8,3);
		image_gray = cvCreateImage(cvSize(128,128),8,3);
		cv::cvtColor(image_color_140,image_color_140,CV_RGB2BGR);
		cvCvtColor(image,img,CV_GRAY2RGB);
		cvCvtColor(image,image_gray,CV_GRAY2RGB);
		QImage image140 = QImage((uchar *)image_color_140.data, 
					 image_color_140.cols,
					 image_color_140.rows,
					 image_color_140.step, 
					 QImage::Format_RGB888);
		ui.Lbl_faceR->setPixmap(QPixmap::fromImage(image140));
		cv::cvtColor(image_color_140,image_color_140,CV_BGR2RGB);
		image_color_140.copyTo(image_color);
		cvReleaseImage(&image);
		cvReleaseImage(&img);
		image_color_140.release();
		isImage_filled=true;
	}
}

void FaceVuee::DeleteImage()
{
	//QList <QTableWidgetItem *> items = ui.tableWidget->selectedItems();
	int value=ui.tableWidget->rowCount();

	for (int x=value-1;x>=0;x--)
	{
		if(ui.tableWidget->item(x,0)->isSelected())
		{
			remove((ui.tableWidget->item(x,0)->whatsThis()+QString("_gry.jpg")).toStdString().c_str());
			remove((ui.tableWidget->item(x,0)->whatsThis()+QString("_rgb.jpg")).toStdString().c_str());

			ui.tableWidget->removeRow(x);
		}
	}
	vector<string> images = FindImages(FACE_DIR);
	process->DeleteImage2(images);

	//code:
	//    QFile::remove(QString(FACE_DIR)+ui.imageslistLST->currentItem()->text()+QString(".jpg"));
	//    ProcessThread::mutex.lock();
	//    process->DeleteImage(ui.imageslistLST->currentRow());
	//    ProcessThread::mutex.unlock();
	//    delete ui.imageslistLST->currentItem();
	//    ui.imagePreviewLBL->setPixmap(0);
}
void FaceVuee::SaveImage(string str,IplImage* img,Mat &img_rgb)
{
//    vector<FaceSample>* faces = process->returnFaces();
    vector<FaceSample> faces = process->face_obj->recognition->Face_database;
    int number = 0;
    for (int a = faces.size() -1 ; a>-1 ; a--)
    {
        string tmplbl = faces[a].label_s;
        string pname = tmplbl.substr(0,tmplbl.length()-6);
        if (str.compare(pname)==0)
        {
            number = atoi(tmplbl.substr(tmplbl.length()-6,2).c_str())+1;
            break;
        }
    }
    char address[100];
    char address_rgb[100];
    if(number<10)
    {
        sprintf(address,FACE_DIR"%s0%d_gry.jpg",str.c_str(),number);
        sprintf(address_rgb,FACE_DIR"%s0%d_rgb.jpg",str.c_str(),number);
//        ui.imageslistLST->insertItem(faces->size(),QString(str.c_str())+QString("0")+QString::number(number));
    }
    else
    {
        sprintf(address,FACE_DIR"%s%d_gry.jpg",str.c_str(),number);
        sprintf(address_rgb,FACE_DIR"%s%d_rgb.jpg",str.c_str(),number);
//        ui.imageslistLST->insertItem(faces->size(),QString(str.c_str())+QString(number));
    }
    cvSaveImage(address,img);
    imwrite(address_rgb,img_rgb);

    string add(address);
    process->AddImage(Mat(img),address);
    QTableWidgetItem *tWidget=new QTableWidgetItem();
    tWidget->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    int N=ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(N);

    Mat img_cv;
    QString str1=QString((add.substr(0,add.length()-8)+"_rgb.jpg").c_str());
    img_cv=imread(str1.toStdString(),1);
    cv::cvtColor(img_cv,img_cv,CV_BGR2RGB);
    QImage sidebar_project_icon_Image((uchar*)img_cv.data, img_cv.cols, img_cv.rows,img_cv.step, QImage::Format_RGB888);
    QIcon sidebar_project_icon_Icon(QPixmap::fromImage(sidebar_project_icon_Image));
    tWidget->setText(QString(add.substr(add.find_last_of("//")+1,add.length() - add.find_last_of("//") - 11).c_str()));
    tWidget->setWhatsThis(QString(add.substr(add.find_last_of("//")+1,add.length() - add.find_last_of("//") - 9).c_str()));
    tWidget->setIcon(sidebar_project_icon_Icon);

    ui.tableWidget->setItem(N,0,tWidget);


}

vector<string> FaceVuee::FindImages(string in)
{
    vector<string> out;
    QDir dir(QString(in.c_str()));
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        string tmp = in + fileInfo.fileName().toStdString();
        if(tmp.substr(tmp.length()-8,8).compare("_gry.jpg")==0){
            out.push_back(tmp);
        }
    }
    return out;
}

void FaceVuee::ChangeMode(int a)
{
	switch (a)
	{
		case 0:
			ui.Lbl_faceR->setPixmap(QPixmap::fromImage(QImage(":/FaceVue/Resources/unknown.jpg")));
			ui.deleteBTN->setVisible(true);
			ui.lineEdit->setVisible(true);
			ui.add_BTN->setVisible(true);
			ui.label_name->setVisible(true);
			ui.Lbl_nameR->setVisible(false);
			process->setProcessingMode (this, REGISTRATION_MODE);
			break;
		case 1:
			process->setProcessingMode (this, RECOGNITION_MODE);
			break;
		default:
			qDebug() << "Unexpected mode";
	}
}

void FaceVuee::Logging(char* label,unsigned long frame)
{    

	if(!std::string(label).compare("Unknown"))
	{
		if(frame-last_frame>5)
		{
			//ui.deleteBTN->setDisabled(false);
			ui.FaceD->setPalette(*green_Palette);
			ui.FaceR->setPalette(*red_Palette);
			//            ui.loggingNameLBL->setText(QString(""));
			ui.Lbl_nameR->setText(QString("Unknown"));

			QImage img (":/FaceVue/Resources/unknown.jpg");
			ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));
		}


	}
	else if(!std::string(label).compare("NoFace"))
	{
		if(frame-last_frame>5)
		{
			//ui.deleteBTN->setDisabled(false);
			ui.FaceD->setPalette(*red_Palette);
			ui.FaceR->setPalette(*red_Palette);
			//            ui.loggingNameLBL->setText(QString(""));
			ui.Lbl_nameR->setText(QString("No Face"));
			QImage img (":/FaceVue/Resources/unknown.jpg");
			ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));
		}



	}
	else
	{
		//ui.deleteBTN->setDisabled(true);
		if(frame-last_frame>5)
		{

			ui.FaceD->setPalette(*green_Palette);
			ui.FaceR->setPalette(*green_Palette);
			//                ui.loggingNameLBL->setText(QString(label));
			string label_(label);
			ui.Lbl_nameR->setText(QString((label_.substr(0,label_.length()-6)).c_str()));
			Mat img_cv;
			QString str=(FACE_DIR+QString((label_.substr(0,label_.length()-4)+"_rgb").c_str())+".jpg");
			img_cv=imread(str.toStdString(),1);
			cv::cvtColor(img_cv,img_cv,CV_BGR2RGB);
			QImage img((uchar*)img_cv.data, img_cv.cols, img_cv.rows,img_cv.step, QImage::Format_RGB888);
			ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));

		}
		last_frame= frame;

	}
	string temp=string(label);
}

void FaceVuee::keyPressEvent(QKeyEvent *event)
{
	if (event->key()==Qt::Key_Return)
		keyPressed = true;
	QMainWindow::keyPressEvent (event);
}

void
FaceVuee::keyReleaseEvent (QKeyEvent *event)
{	if (event->key()==Qt::Key_Return)
		keyPressed = false;
	QMainWindow::keyReleaseEvent (event);
}

bool
FaceVuee::isReturnKeyPressed ()
{
	return keyPressed;
}


void 
FaceVuee::drawImage (QImage *img, QWaitCondition *cond, QMutex *mutex, QLabel *label)
{
	mutex->lock();
	label->setPixmap (QPixmap::fromImage (*img));
	cond->wakeAll();
}

