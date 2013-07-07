#include "RecognitionMode.h"
#include <GUI/facevue2.h>

RecognitionMode::RecognitionMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
}

/* recognize a face in the image and then draw it */
Mat
RecognitionMode::process (IplImage *image)
{
	//recognize
	CvRect rect = facevue->detect_FaceROI(image);
	IplImage* warp_dst = cvCreateImage(cvSize(128,128), IPL_DEPTH_8U, 1);
	facevue->align_Face(image, rect, warp_dst);

	//get the name
	//gui->process->setName (facevue->is_aligned ? (facevue->recognize_Face(warp_dst)) : "Unknown");
	std::cout << (facevue->is_aligned ? (facevue->recognize_Face(warp_dst)) : "Unknown") << std::endl;

	cvReleaseImage(&warp_dst);
	FaceVue::FaceContent *f = facevue->target_Face;

	//common operation for drawing
	Mat img;
	cvtColor(Mat(image),img,CV_BGR2RGB);

	Point center = Point( img.cols/2, img.rows/2 );
	double angle = 90.0;
	double scale = 1;

	Mat rot_mat = getRotationMatrix2D (center, angle, scale);
	Mat rot_mat2 = getRotationMatrix2D (center, -1.f*angle, scale);
	
	if(gui->ui.overlayCKB->isChecked() && f->index != -1)
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

QLabel*
RecognitionMode::getProperLabel()
{
	return gui->ui.recognitionDisplayLBL;
}

