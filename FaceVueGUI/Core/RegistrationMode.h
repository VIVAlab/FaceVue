#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

#include "ProcessingMode.h"

//minimum number of frames required between face registrations
#define SNAPSHOT_FRAME_GAP 20

class RegistrationMode : public ProcessingMode
{
	public:
		static RegistrationMode* getInstance(FaceVuee *gui, FaceVue *facevue);
		virtual Mat process (const Mat &img);
		void registerCornerImage (std::string name);

	private:
		unsigned int countDown;
		Mat grayImage;
		RegistrationMode(FaceVuee *gui, FaceVue *facevue);

};


#endif /* end of include guard: REGISTRATIONMODE_H */
