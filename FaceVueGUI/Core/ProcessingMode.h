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
		bool hasCornerImage() const;
		const Mat& cornerImage() const;

		//configuration setter functions
		void setReturnKeyFlag (bool set);
		void setDrawOverlayFlag (bool set);


	protected:
		FaceVue *facevue;
		FaceVuee *gui;

		void setDetectionFlag (bool set);
		void setRecognitionFlag (bool set);
		void setCornerImage (const Mat& image);
		void removeCornerImage ();

	private:
		/*
		 * Note:
		 *  corner image is the image at the top right corner of the GUI,
		 *  this image can represent different things. 
		 *  In registration mode, it will be the snapshot taken
		 *  In recognition mode, it will be the recognized face
		 */
		enum {
			PROCESSING_MODE_FLAG_DETECTED  		= 1 << 0,  //a face is detected
			PROCESSING_MODE_FLAG_RECOGNIZED		= 1 << 1,  //a face is recognized
			PROCESSING_MODE_FLAG_RETURN_KEY		= 1 << 2,  //the return key is pressed
			PROCESSING_MODE_FLAG_DRAW_OVERLAY       = 1 << 3,  //the overlay must be drawn
			PROCESSING_MODE_FLAG_CORNER_IMAGE	= 1 << 4   //corner image exits
		};
		unsigned int flags;
		Mat _cornerImage;
};


#endif /* end of include guard: PROCESSINGMODE_H */
