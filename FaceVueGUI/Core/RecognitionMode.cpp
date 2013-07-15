#include "RecognitionMode.h"
#include <GUI/facevue2.h>
#include <QDebug>

RecognitionMode*
RecognitionMode::getInstance(FaceVuee *gui, FaceVue *facevue)
{
	static RecognitionMode* instance = NULL;
	if (!instance)
		instance = new RecognitionMode (gui, facevue);
	return instance;
}


RecognitionMode::RecognitionMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
}

/* recognize a face in the image and then draw it */
Mat
RecognitionMode::process (const Mat &image)
{
	//recognize
	CvRect rect = facevue->detect_FaceROI(image);
	Mat warp_dst = facevue->align_Face(image, rect);

	QString detected_face = QString (facevue->is_aligned ? (facevue->recognize_Face(warp_dst)).c_str() : "Unknown");
	//TODO: set the detected_face somewhere and write a getter for it
	//gui->process->faceRecognized (detected_face);

	FaceVue::FaceContent *f = facevue->target_Face;
	setDetectionFlag (f->index != -1);
	setRecognitionFlag (detected_face.compare("Unknown"));
	
	//draw onto a new image
	Mat img;
	cvtColor(image, img, CV_BGR2RGB);

	Point center = Point( img.cols/2, img.rows/2 );
	double angle = 90.0;
	double scale = 1;

	Mat rot_mat = getRotationMatrix2D (center, angle, scale);
	Mat rot_mat2 = getRotationMatrix2D (center, -1.f*angle, scale);
	
	if(drawsOverlay() && f->index != -1)
	{
		circle(img, Point(f->right_eye_x, f->right_eye_y), 3, Scalar(0, 0, 255), CV_FILLED);
		circle(img, Point(f->left_eye_x, f->left_eye_y), 3, Scalar(0, 0, 255), CV_FILLED);
		circle(img, Point(f->mouth_x, f->mouth_y), 3, cvScalar(0, 255, 255), CV_FILLED);
		rectangle(img, Point(f->p1_x, f->p1_y), Point(f->p2_x, f->p2_y), cvScalar(255, 0, 0), 2);
	}
	//warpAffine does not operate in-place
	Mat img2;
	warpAffine(img, img2, rot_mat, img.size());
	flip(img2,img2,0);
	warpAffine(img2, img, rot_mat2, img.size());

	return img;
}

