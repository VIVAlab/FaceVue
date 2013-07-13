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
		virtual Mat process (const Mat &img)=0;

		//configuration getter functions
		bool isRecognized() const;
		bool isDetected() const;
		bool isReturnKeyPressed() const;
		bool drawsOverlay() const;

		//configuration setter functions
		void setReturnKeyFlag (bool set);
		void setDrawOverlayFlag (bool set);


	protected:
		FaceVue *facevue;
		FaceVuee *gui;

		void setDetectionFlag (bool set);
		void setRecognitionFlag (bool set);

	private:
		enum {
			PROCESSING_MODE_FLAG_DETECTED  		= 1 << 0,
			PROCESSING_MODE_FLAG_RECOGNIZED		= 1 << 1,
			PROCESSING_MODE_FLAG_RETURN_KEY		= 1 << 2,
			PROCESSING_MODE_FLAG_DRAW_OVERLAY       = 1 << 3
		};
		unsigned int flags;
};


#endif /* end of include guard: PROCESSINGMODE_H */
