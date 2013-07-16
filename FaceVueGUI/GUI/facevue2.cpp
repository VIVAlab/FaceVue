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
#include <QMessageBox>

#define GRAY_SCALE_POSTFIX   		"_gry.jpg"
#define COLOR_POSTFIX	     		"_rgb.jpg"
#define UNKNOWN_IMAGE_RESOURCE_NAME     ":/FaceVue/Resources/unknown.jpg"

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
	isImage_filled=false;
	ui.setupUi(this);

	QImage img(UNKNOWN_IMAGE_RESOURCE_NAME);
	ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));

	//User Interface signals 
	connect(ui.add_BTN, SIGNAL(clicked()), this, SLOT(addImg_to_database()));
	connect(ui.deleteBTN, SIGNAL(clicked()), this, SLOT(DeleteImage()));
	connect(ui.widgetTAB, SIGNAL(currentChanged(int)), this, SLOT(ChangeMode(int)));
	connect(ui.overlayCKB, SIGNAL(stateChanged(int)), this, SLOT(checkBoxStateChanged(int)));

	process = new Process(this);
	processThread = new QThread;
	process->moveToThread (processThread);

	connect (this,
	 	 SIGNAL(readyForNextImage()),
		 process,
		 SLOT (captureNextFrame()),
		 Qt::QueuedConnection);

	connect (process,
		 SIGNAL (newFrameIsProcessed()),
		 this,
		 SLOT(updateUserInterface()),
		 Qt::QueuedConnection);

	connect (process,
		 SIGNAL(unableToCapture()),
		 this,
		 SLOT(displayErrorUnableToCapture()),
		 Qt::QueuedConnection);

	//TODO: use QAbstractItemModel to remove this signal here !
	connect(process, 
		SIGNAL(ImageAdded(QString)), 
		this, 
		SLOT(InsertIntoTable(QString)),
		Qt::QueuedConnection);

	processThread->start();
	LoadAllImages ();
	ChangeMode (ui.widgetTAB->currentIndex ());
	emit readyForNextImage();
}


FaceVuee::~FaceVuee()
{
	processThread->exit (0);
	processThread->wait ();
	delete process;        
	delete processThread;
}

void 
FaceVuee::addImg_to_database()
{
	if(isImage_filled)
	{
		String str = ui.lineEdit->text().toStdString();
		if (SaveImage(str, image_gray, image_color)) {
			process->AddImage (image_gray, str);
		} else {
			stringstream warnMsg;
			warnMsg << "Unable to store color or gray-scale image." << endl;
			warnMsg << "Make sure " << getFaceDir() << " exists and" << endl;
			warnMsg << "you have permissions to read and write into this directory." << endl;
			QMessageBox::warning (this, 
					      tr("Warning"),
					      tr(warnMsg.str().c_str()),
					      QMessageBox::Ok);
		}

		image_color.release();
		image_gray.release();
		ui.lineEdit->clear();
		isImage_filled = false;

		QImage img (UNKNOWN_IMAGE_RESOURCE_NAME);
		ui.Lbl_faceR->setPixmap(QPixmap::fromImage(img));
		ui.lineEdit->setText("");
	} else {
		QMessageBox::warning (this,
				tr("Warning"),
				tr("You need to take a snapshot first"),
				QMessageBox::Ok);
	}
}

/**********************************
 *                                *
 * image => image_gray            *
 * image_color_140 => image_color *
 *                                *
 **********************************/
void FaceVuee::OutImage(Mat image, Mat image_color_140)
{
	image_gray = Mat (128, 128, CV_8UC3);
	cv::cvtColor(image_color_140, image_color_140, CV_RGB2BGR);
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
	image_color_140.release();
	isImage_filled = true;
}

void
FaceVuee::ApplyRecognizedFace (QString name)
{
	QImage image;
	Mat img;
	if (!name.compare("Unknown"))
	{
		image = QImage (UNKNOWN_IMAGE_RESOURCE_NAME);
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
FaceVuee::nameAlreadyExists (const string &name) const
{
	//TODO: database faces should have a separate object 
	const vector<FaceSample> &faces = process->getFaceSamples();
	for (vector <FaceSample>::const_iterator iter = faces.begin(); iter != faces.end(); iter++)
		if (!name.compare ((*iter).label_s))
			return true;
	return false;
}

bool
FaceVuee::SaveImage(string &str, const Mat &img, const Mat &img_rgb)
{
	//find a unique label
	stringstream ss;
	unsigned int index = 0;
	do
	{
		ss.str(std::string()); //clear out the stringstream
		ss << str << index;
		index ++;
	} while (nameAlreadyExists (ss.str()));
	str = ss.str();

	stringstream address_gry;
	stringstream address_rgb;
	address_gry << getFaceDir() << str << GRAY_SCALE_POSTFIX;
	address_rgb << getFaceDir() << str << COLOR_POSTFIX;	

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
			QString gry_filename = getFaceDir() + filename;
			QString rgb_filename = image_name + QString (COLOR_POSTFIX);
			if (list.contains (rgb_filename))
			{
				Mat gray_image = imread (gry_filename.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
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
			ui.Lbl_faceR->setPixmap(QPixmap::fromImage(QImage(UNKNOWN_IMAGE_RESOURCE_NAME)));
			ui.deleteBTN->setVisible(true);
			ui.lineEdit->setVisible(true);
			ui.add_BTN->setVisible(true);
			ui.label_name->setVisible(true);
			ui.Lbl_nameR->setVisible(false);
			process->setProcessingMode (this, REGISTRATION_MODE);
			process->drawOverlay (true); //draw overlay in registration mode!
			break;
		case 1:
			ui.deleteBTN->setVisible(false);
			ui.lineEdit->setVisible(false);
			ui.add_BTN->setVisible(false);
			ui.label_name->setVisible(false);
			ui.Lbl_nameR->setVisible(true);
			process->setProcessingMode (this, RECOGNITION_MODE);
			process->drawOverlay (ui.overlayCKB->isChecked());
			break;
		default:
			qDebug() << "Unexpected mode";
	}
}

void FaceVuee::keyPressEvent(QKeyEvent *event)
{
	if (event->key()==Qt::Key_Return)
		process->returnKeyPressed (true);
	QMainWindow::keyPressEvent (event);
}

void
FaceVuee::keyReleaseEvent (QKeyEvent *event)
{	if (event->key()==Qt::Key_Return)
		process->returnKeyPressed (false);
	QMainWindow::keyReleaseEvent (event);
}

/*****************************************************
 * draws image onto the label by keeping the aspect
 * by keeping the aspect ratio of the image relative
 * to the label
 *****************************************************/
void
FaceVuee::updateLabelKeepAspectRatio (QLabel *label, const QImage &image)
{
	QPixmap pixmap = QPixmap::fromImage(image).scaled(label->size(), Qt::KeepAspectRatio);
	label->setPixmap (pixmap);
}

/******************************************
 * this method is invoked only if the 
 * recognition tab is the current tab 
 ******************************************/
void
FaceVuee::updateUserInterfaceInRecognitionMode ()
{
	//draw the image
	updateLabelKeepAspectRatio (ui.recognitionDisplayLBL, process->displayImage ());
	
	//set the recognition/detection label palettes
	QPalette greenPalette (QPalette::Window, Qt::green);
	QPalette redPalette (QPalette::Window, Qt::red);
	ui.FaceR->setPalette (process->processingMode()->isRecognized() ? greenPalette : redPalette);
	ui.FaceD->setPalette (process->processingMode()->isDetected() ? greenPalette : redPalette);

}


/******************************************
 * this method is invoked only if the 
 * registration tab is the current tab 
 ******************************************/
void
FaceVuee::updateUserInterfaceInRegistrationMode ()
{
	//draw the image
	updateLabelKeepAspectRatio (ui.registrationDisplayLBL, process->displayImage ());
	
}

void
FaceVuee::updateUserInterface ()
{
	//update based on active tab-widget 
	switch (ui.widgetTAB->currentIndex ())
	{
		case 0:
			updateUserInterfaceInRegistrationMode ();
			break;
		case 1:
			updateUserInterfaceInRecognitionMode ();
			break;
		default:
			qDebug () << "unrecognized mode !";
			break;
	}
	//draw the corner image
	QImage cornerImage;
	const Mat &cornerImageInMatFormat = process->processingMode()->cornerImage();
	if (process->processingMode()->hasCornerImage()){
		cornerImage = QImage((uchar *)cornerImageInMatFormat.data,  
				cornerImageInMatFormat.cols, 
				cornerImageInMatFormat.rows, 
				cornerImageInMatFormat.step,  
				QImage::Format_RGB888);
	} else {
		cornerImage = QImage (UNKNOWN_IMAGE_RESOURCE_NAME);
	}
	ui.Lbl_faceR->setPixmap(QPixmap::fromImage(cornerImage));

	emit readyForNextImage();
}

void 
FaceVuee::displayErrorUnableToCapture ()
{
	QMessageBox::warning (this, 
			tr("Error"),
			tr("Unable to capture images from a usb camera.\n"
			   "Another application may be using the camera or\n"
			   "the usb camera driver may be malfunctioning"),
			QMessageBox::Ok);
	this->close();
}

void 
FaceVuee::checkBoxStateChanged (int state)
{
	switch (state)
	{
		case Qt::Unchecked:
			process->drawOverlay (false);
			break;
		case Qt::Checked:
			process->drawOverlay (true);
			break;
		default:
			qDebug () << "checkbox state unexpected!";
			break;
	}
}

