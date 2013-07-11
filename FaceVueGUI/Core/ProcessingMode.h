#ifndef PROCESSINGMODE_H
#define PROCESSINGMODE_H

#include <facevue.h>
#include <opencv2/core/core.hpp>
#include <QImage>
#include <QLabel>
#include <QPalette>

class FaceVuee; //foreward declaration

class ProcessingMode
{
public:
	ProcessingMode(FaceVuee *gui, FaceVue *facevue);
	virtual ~ProcessingMode();
	virtual Mat process (Mat &img)=0;
	virtual QLabel* getProperLabel() const = 0;
	QPalette getRecognitionLabelPalette() const;
	QPalette getDetectionLabelPalette() const;


protected:
	FaceVue *facevue;
	FaceVuee *gui;
	QPalette recognitionPalette;
	QPalette registrationPalette;
};


#endif /* end of include guard: PROCESSINGMODE_H */
