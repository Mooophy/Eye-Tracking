#include "cv.h"
#include "highgui.h"


//	Function prototypes
void extractHUE(IplImage* source, IplImage* dest);
void drawFaceBox(IplImage* img, CvBox2D faceBox);


void extractHUE(IplImage* source, IplImage* hue)
{
	IplImage* HSV = cvCreateImage(cvGetSize(source), IPL_DEPTH_8U, 3);

	// convert HSV color space
	cvCvtColor(source, HSV, CV_RGB2HSV);
	cvCvtPixToPlane(HSV, hue, NULL, NULL, NULL);
	
	cvReleaseImage(&HSV);
}

void drawFaceBox(IplImage* img, CvBox2D faceBox, double color)
{
	// top-left corner
	CvPoint pt1; 
	pt1.x = (int)(faceBox.center.x - (faceBox.size.width/2.0)); 
	pt1.y = (int)(faceBox.center.y - (faceBox.size.height/2.0));
	
	// bottom-right corner
	CvPoint pt2; 
	pt2.x = (int)(faceBox.center.x + (faceBox.size.width/2.0)); 
	pt2.y = (int)(faceBox.center.y + (faceBox.size.height/2.0));

	cvRectangle( img, pt1, pt2, color, 2 );
}

