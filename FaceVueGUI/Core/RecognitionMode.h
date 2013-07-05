#ifndef RECOGNITIONMODE_H
#define RECOGNITIONMODE_H

#include "ProcessingMode.h"

class RecognitionMode : public ProcessingMode
{
public:
	RecognitionMode(FaceVuee *gui, FaceVue *facevue);
	virtual void process (IplImage *img);
};

#endif /* end of include guard: RECOGNITIONMODE_H */
