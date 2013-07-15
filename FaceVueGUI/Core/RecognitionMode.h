#ifndef RECOGNITIONMODE_H
#define RECOGNITIONMODE_H

#include "ProcessingMode.h"

class RecognitionMode : public ProcessingMode
{
	public:
		static RecognitionMode* getInstance(FaceVuee *gui, FaceVue *facevue);
		virtual Mat process (const Mat &img);
	private:
		RecognitionMode(FaceVuee *gui, FaceVue *facevue);
};

#endif /* end of include guard: RECOGNITIONMODE_H */
