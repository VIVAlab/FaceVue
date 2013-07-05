#include "VideoObserver.h"
#include <opencv/cv.h>
#include <QThread>
#include <QDebug>

VideoObserver::VideoObserver(QObject *owner, const char *imageSlot):QObject(owner)
{
	myImage = new Mat;
	QObject::connect (this, SIGNAL(newImage()), owner, imageSlot, Qt::QueuedConnection);
}

VideoObserver::~VideoObserver()
{
	delete myImage;
}

void
VideoObserver::imageIsProcessed()
{
	emit imageObserved (this);
}

Mat*
VideoObserver::getImage ()
{
	return myImage;
}

void
VideoObserver::imageAvailable (Mat *img)
{
	delete myImage;
	myImage = new Mat (*img);
	emit newImage();
}

