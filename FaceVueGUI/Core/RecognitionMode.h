#ifndef RECOGNITIONMODE_H
#define RECOGNITIONMODE_H

#include "ProcessingMode.h"

class RecognitionMode : public ProcessingMode
{
public:
	RecognitionMode(FaceVuee *gui, FaceVue *facevue);
	virtual Mat process (Mat &img);
	virtual QLabel * getProperLabel();
};

#endif /* end of include guard: RECOGNITIONMODE_H */
