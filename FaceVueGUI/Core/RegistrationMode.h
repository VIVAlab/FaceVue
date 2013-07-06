#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

#include "ProcessingMode.h"

//minimum number of frames required between face registrations
#define SNAPSHOT_FRAME_GAP 30  

class RegistrationMode : public ProcessingMode
{
public:
	RegistrationMode(FaceVuee *gui, FaceVue *facevue);
	virtual Mat process (IplImage *img);
	virtual QLabel* getProperLabel();

private:
	unsigned int countDown;
};


#endif /* end of include guard: REGISTRATIONMODE_H */
