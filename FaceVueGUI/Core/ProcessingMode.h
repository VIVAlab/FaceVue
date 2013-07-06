#ifndef PROCESSINGMODE_H
#define PROCESSINGMODE_H

#include <facevue.h>
#include <opencv2/core/core.hpp>
#include <QImage>
#include <QLabel>

class FaceVuee; //foreward declaration

class ProcessingMode
{
public:
	ProcessingMode(FaceVuee *gui, FaceVue *facevue);
	virtual QImage* process (IplImage *img)=0;
	virtual QLabel* getProperLabel()=0;

protected:
	FaceVue *facevue;
	FaceVuee *gui;
};


#endif /* end of include guard: PROCESSINGMODE_H */
