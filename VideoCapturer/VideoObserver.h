#ifndef VIDEOOBSERVER_H
#define VIDEOOBSERVER_H

#include <QObject>
#include <cv.h>

using namespace cv;

class VideoObserver : public QObject
{
Q_OBJECT
public:
	VideoObserver(QObject *owner, const char *imageSlot);
	~VideoObserver();
	void imageIsProcessed();
	Mat *getImage();

signals:
	void newImage ();
	void imageObserved(VideoObserver *);

public slots:
	void imageAvailable (Mat *);

private:
	Mat *myImage;
};


#endif /* end of include guard: VIDEOOBSERVER_H */
