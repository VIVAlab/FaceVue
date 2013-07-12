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
		bool isRecognized() const;
		bool isDetected() const;
		bool isReturnKeyPressed() const;

		void setReturnKeyFlag (bool set);

	protected:
		FaceVue *facevue;
		FaceVuee *gui;

		void setDetectionFlag (bool set);
		void setRecognitionFlag (bool set);

	private:
		enum {
			PROCESSING_MODE_FLAG_DETECTED  	= 1 << 0,
			PROCESSING_MODE_FLAG_RECOGNIZED	= 1 << 1,
			PROCESSING_MODE_RETURN_KEY	= 1 << 2
		};
		unsigned int flags;
};


#endif /* end of include guard: PROCESSINGMODE_H */
