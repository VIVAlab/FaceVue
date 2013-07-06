#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

#include "ProcessingMode.h"

class RegistrationMode : public ProcessingMode
{
public:
	RegistrationMode(FaceVuee *gui, FaceVue *facevue);
	virtual Mat process (IplImage *img);
	virtual QLabel* getProperLabel();

};


#endif /* end of include guard: REGISTRATIONMODE_H */
