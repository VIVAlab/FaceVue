#include "RegistrationMode.h"
#include <QImage>
#include <GUI/facevue2.h>

RegistrationMode::RegistrationMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
	countDown = 0;
}

Mat
RegistrationMode::process (IplImage *image)
{
	//Find a face
	CvRect rect = facevue->detect_FaceROI(image);
	FaceVue::FaceContent *f = facevue->target_Face;

	//count down
	countDown = (countDown==0) ? 0 : (countDown-1);

	if(gui->isReturnKeyPressed()     //the return key is pressed
	   && f->index != -1 		 //a face is detected
	   && countDown == 0) 		 //last snapshot was taken at least SNAPSHOT_FRAME_GAP ago
	{
		countDown = SNAPSHOT_FRAME_GAP;  //reset the counter
		gui->process->addImage(image, rect);
	} 

	//common stuff for drawing
	Mat img;
	cvtColor(Mat(image),img,CV_BGR2RGB);

	Point center = Point( img.cols/2, img.rows/2 );
	double angle = 90.0;
	double scale = 1;

	Mat rot_mat = getRotationMatrix2D (center, angle, scale);
	Mat rot_mat2 = getRotationMatrix2D (center, -1.f*angle, scale);

	if(f->index != -1)
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
RegistrationMode::getProperLabel()
{
	return gui->ui.registrationDisplayLBL;
}
