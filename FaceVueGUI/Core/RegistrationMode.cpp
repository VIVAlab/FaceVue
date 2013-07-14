#include "RegistrationMode.h"
#include <QImage>
#include <GUI/facevue2.h>

RegistrationMode::RegistrationMode(FaceVuee *gui, FaceVue *facevue)
	:ProcessingMode (gui, facevue)
{
	countDown = 0;
}

Mat
RegistrationMode::process (const Mat &image)
{
	//Find a face
	CvRect rect = facevue->detect_FaceROI(image);
	FaceVue::FaceContent *f = facevue->target_Face;

	//count down
	countDown = (countDown==0) ? 0 : (countDown-1);

	if(isReturnKeyPressed()     //the return key is pressed
	   && f->index != -1 	    //a face is detected
	   && countDown == 0)       //last snapshot was taken at least SNAPSHOT_FRAME_GAP ago
	{
		countDown = SNAPSHOT_FRAME_GAP;  //reset the counter
		Mat warp_dst = facevue->align_Face(image, rect);
		//TODO: Set the corner image & store the gray-scale image and others
		// look into FaceVuee::OutImage for more info
		// you need to remove image_gray & other intermediate
		// processed images from FaceVuee 
		// ...
		if(facevue->is_aligned) {
			int val_x=abs(f->right_eye_x - f->left_eye_x);
			int val_y=abs(f->mwarp_dsth_y - f->left_eye_y);
			CvRect rect2 = cvRect(abs(f->right_eye_x-2*val_x),
					      abs(f->right_eye_y-2*val_y),
					      rect.width*1.1f,
					      rect.height*1.1f);
			if (rect2.x + rect2.width > image.size().width)
				rect2.width = image.size().width - rect2.x;
			if (rect2.y + rect2.height > image.size().height)
				rect2.height = image.size().height - rect2.y;
			Mat image_color_140 = image(rect2);
			cv::resize (image_color_140, image_color_140, cvSize (140, 140)); 
			//
			//	emit OutImage (warp_dst, image_color_140);
		}
	} 

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

