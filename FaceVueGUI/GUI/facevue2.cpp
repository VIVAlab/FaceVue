#include "GUI/facevue2.h"
#include <QDir>
#include <QMessageBox>
#include "GUI/addimage.h"
#include <QTableWidget>
#include <QSize>
#include <QDebug>
#include <QLabel>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>

#define GRAY_SCALE_POSTFIX   "_gry.jpg"
#define COLOR_POSTFIX	     "_rgb.jpg"


// TODO: for windows there are better alternatives 
// (the installation directory can be registered using QSetting and retreived)
// This can be investigated
// The current solution assumes the application is always executed from the
// working directory. (this is a strong assumption)
//
#if defined(Q_OS_UNIX)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QMessageBox>

const char*
FaceVuee::getFaceDir ()
{
	static string *face_dir = NULL;
	if (!face_dir)
	{
		face_dir = new string;
		stringstream ss;
		struct passwd *pw = getpwuid (getuid());
		ss << pw->pw_dir << "/.facevue/face/";
		*face_dir = ss.str();
	}

	return face_dir->c_str(); 
}
#elif defined(Q_OS_WIN32)
const char*
FaceVuee::getFaceDir ()
{
	static string *face_dir = NULL;
	if (!face_dir)
	{
		face_dir = new string;
		*face_dir = "face";
	}
	return face_dir->c_str();
}
#endif

FaceVuee::FaceVuee(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
	qRegisterMetaType <Mat>("Mat");
	keyPressed = false;
	flag=true;
	ui.setupUi(this);

	isImage_filled=false;

	ui.tableWidget->insertColumn(0);
	ui.tableWidget->setAutoScroll(true);

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

	process = new ProcessThread(this);

	connect(process, 
		SIGNAL(ImageAdded(QString)), 
		this, 
		SLOT(InsertIntoTable(QString)),
		Qt::QueuedConnection);
	connect(process,
		SIGNAL(Logging( char *,unsigned long)),
		this,
		SLOT(Logging(char *,unsigned long)));
	connect(process,
	  	SIGNAL(drawImage(QImage*, QWaitCondition*, QMutex*, QLabel*)),
		this,
		SLOT(drawImage(QImage*, QWaitCondition*, QMutex*, QLabel*)),
		Qt::QueuedConnection);
	connect (process,
		 SIGNAL(OutImage(Mat, Mat)),
		 this,
		 SLOT(OutImage(Mat, Mat)), 
		 Qt::QueuedConnection);
	connect (process,
		 SIGNAL(Beep()),
		 this,
		 SLOT(Beep()));
	connect (process,
		 SIGNAL(recognizedFace (QString)),
		 this,
		 SLOT(ApplyRecognizedFace (QString)),
		 Qt::QueuedConnection);

	process->start();
	LoadAllImages ();
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
		QString str = ui.lineEdit->text();
		if (SaveImage(str.toStdString(), image_gray, image_color))
			process->AddImage (image_gray, str.toStdString());
		else 
			QMessageBox::warning (this, 
					      tr("Warning"),
					      tr("Unable to store the color or gray-scale image.\n"),
					      QMessageBox::Ok);

		image_color.release();
		image_gray.release();
		ui.lineEdit->clear();
		isImage_filled = false;

		QImage img (":/FaceVue/Resources/unknown.jpg");
		ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));
		ui.lineEdit->setText("New face added");
	}
	else
	{
		ui.lineEdit->setText("No face detected");
	}
}

void FaceVuee::OutImage(Mat image, Mat image_color_140)
{    
	flag=true;

	//IplImage* img = cvCreateImage(cvSize(128, 128), 8, 3);
	//image_gray = cvCreateImage(cvSize(128, 128), 8, 3);
	Mat img (128, 128, CV_8UC3);
	image_gray = Mat (128, 128, CV_8UC3);
	cv::cvtColor(image_color_140, image_color_140, CV_RGB2BGR);
	cv::cvtColor(image, img, CV_GRAY2RGB);
	cv::cvtColor(image, image_gray, CV_GRAY2RGB);
	QImage image140 = QImage((uchar *)image_color_140.data,  
				 image_color_140.cols, 
				 image_color_140.rows, 
				 image_color_140.step,  
				 QImage::Format_RGB888);
	ui.Lbl_faceR->setPixmap(QPixmap::fromImage(image140));
	cv::cvtColor(image_color_140, image_color_140, CV_BGR2RGB);
	image_color_140.copyTo(image_color);
	image.release();
	img.release();
	image_color_140.release();
	isImage_filled=true;
}

void
FaceVuee::ApplyRecognizedFace (QString name)
{
	QImage image;
	Mat img;
	if (!name.compare("Unknown"))
	{
		image = QImage (":/FaceVue/Resources/unknown.jpg");
	} else {
		QString filename = QString (getFaceDir()) + name + QString (COLOR_POSTFIX);
		img = imread (filename.toStdString());
		cv::cvtColor (img, img, CV_RGB2BGR);
		image = QImage ((uchar*) img.data,
				img.cols,
				img.rows,
				img.step,
				QImage::Format_RGB888);
	}
	ui.Lbl_nameR->setText (name);
	ui.Lbl_faceR->setPixmap(QPixmap::fromImage(image));
}

void FaceVuee::DeleteImage()
{
	int value=ui.tableWidget->rowCount();

	for (int x=value-1;x>=0;x--)
	{
		if(ui.tableWidget->item(x,0)->isSelected())
		{
			QString name;
			name = ui.tableWidget->item(x,0)->whatsThis();
			QString name_gry = name + QString (GRAY_SCALE_POSTFIX);
			QString name_rgb = name + QString (COLOR_POSTFIX);
			process->DeleteImage (name);
			remove(name_gry.toStdString().c_str());
			remove(name_rgb.toStdString().c_str());
			ui.tableWidget->removeRow(x);
		}
	}
}

void
FaceVuee::InsertIntoTable (QString name)
{
	//TODO: use Qt Models
	QTableWidgetItem *tWidget = new QTableWidgetItem();
	tWidget->setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	int N = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(N);

	QString filename = QString (getFaceDir()) + name + QString (COLOR_POSTFIX);
	Mat image = imread (filename.toStdString());
	cv::cvtColor(image, image, CV_BGR2RGB);
	QImage sidebar_project_icon_Image ((uchar*)image.data,
					  image.cols,
					  image.rows,
					  image.step,
					  QImage::Format_RGB888);
	QIcon sidebar_project_icon_Icon(QPixmap::fromImage(sidebar_project_icon_Image));
	tWidget->setText(name);
	tWidget->setWhatsThis(name);
	tWidget->setIcon(sidebar_project_icon_Icon);
	ui.tableWidget->setItem(N, 0, tWidget);
}

bool
FaceVuee::SaveImage(string str, const Mat &img, const Mat &img_rgb)
{
	vector<FaceSample> faces = process->face_obj->recognition->Face_database;

	//if the name already exists, append it with an index value
	for (int a = faces.size() -1 ; a>-1 ; a--)
	{
		string tmplbl = faces[a].label_s;
		string pname = tmplbl.substr(0,tmplbl.length()-6);
		if (!str.compare(pname))
		{
			int index = atoi(tmplbl.substr(tmplbl.length()-6, 2).c_str())+1;

			//TODO: this assertion can be an exception ... 
			Q_ASSERT (index < 100);
			stringstream ss;
			ss << str << ((index < 10) ? "0" : "");
			str = ss.str();	
			break;
		}
	}

	stringstream address_gry;
	stringstream address_rgb;
	address_gry << getFaceDir() << str << GRAY_SCALE_POSTFIX;
	address_rgb << getFaceDir() << str << COLOR_POSTFIX;

	qDebug () << "storing color image into: " << address_rgb.str().c_str();
	qDebug () << "storing grey-scale image into: " << address_gry.str().c_str();

	return (imwrite (address_gry.str(), img) &&   //save the gray-scale image
	        imwrite (address_rgb.str(), img_rgb));     //save the color image
}

void
FaceVuee::LoadAllImages()
{
	QDir dir = QDir(QString(getFaceDir()));
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);
	QStringList list = dir.entryList();

	for (QStringList::iterator iter = list.begin(); iter != list.end(); iter++) 
	{
		QString filename = (*iter);
		QString gry = QString (GRAY_SCALE_POSTFIX);
		if (filename.endsWith (gry))
		{
			QString image_name = filename.left (filename.length() - gry.size());
			QString gry_filename = filename;
			QString rgb_filename = image_name + QString (COLOR_POSTFIX);
			if (list.contains (rgb_filename))
			{
				Mat gray_image = imread (gry_filename.toStdString());
				process->AddImage (gray_image, image_name.toStdString());
			}
		}
	}
}

void 
FaceVuee::ChangeMode(int a)
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
			ui.deleteBTN->setVisible(false);
			ui.lineEdit->setVisible(false);
			ui.add_BTN->setVisible(false);
			ui.label_name->setVisible(false);
			ui.Lbl_nameR->setVisible(true);
			process->setProcessingMode (this, RECOGNITION_MODE);
			break;
		default:
			qDebug() << "Unexpected mode";
	}
}

void 
FaceVuee::Logging(char* label,unsigned long frame)
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
		{//TODO: REDO this

			ui.FaceD->setPalette(*green_Palette);
			ui.FaceR->setPalette(*green_Palette);

			string label_(label);
			ui.Lbl_nameR->setText(QString((label_.substr(0,label_.length()-6)).c_str()));
			Mat img_cv;
			QString str=(getFaceDir() + QString((label_.substr(0,label_.length()-4)+"_rgb").c_str())+".jpg");
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


/**
 * TODO: get rid of cond, mutex by using the VideoCapturer framework
 **/
void 
FaceVuee::drawImage (QImage *img, QWaitCondition *cond, QMutex *mutex, QLabel *label)
{
	mutex->lock();
	label->setPixmap (QPixmap::fromImage (*img));
	cond->wakeAll();
	mutex->unlock();
}

