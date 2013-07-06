#ifndef RECOGNITIONMODE_H
#define RECOGNITIONMODE_H

#include "ProcessingMode.h"

class RecognitionMode : public ProcessingMode
{
public:
	RecognitionMode(FaceVuee *gui, FaceVue *facevue);
	virtual QImage * process (IplImage *img);
	virtual QLabel * getProperLabel();
};

#endif /* end of include guard: RECOGNITIONMODE_H */
