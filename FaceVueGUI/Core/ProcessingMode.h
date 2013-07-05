#ifndef PROCESSINGMODE_H
#define PROCESSINGMODE_H

#include "../GUI/facevue2.h"
#include <facevue.h>
#include <opencv2/core/core.hpp>

class ProcessingMode
{
public:
	ProcessingMode(FaceVuee *gui, FaceVue *facevue);
	virtual void process (IplImage *img)=0;

protected:
	FaceVue *facevue;
	FaceVuee *gui;
};


#endif /* end of include guard: PROCESSINGMODE_H */
