// *************************** //
//	General Purpose functions  //
//	By Zafer SAVAS 25.02.2005  //	
// *************************** //

#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>

float dist(float*, float*, int);
void ShowImage(IplImage* resim, char* title);
void ShowPixel(IplImage* img, int n, char* str);
void writeToFile(IplImage* imgs[], IplImage* average);
void calculateAverageImage(IplImage* imgs[], IplImage* awg);
void writeImageProperties(IplImage* image);
void getDataOf32U (IplImage* img32U, IplImage* img8U);
void drawEyeRectangle(IplImage* face);
void drawFaceRectangle(IplImage* face, IplImage* eigen, int x1, int y1);
//
void drawLine(IplImage* eye, CvPoint* points, int numOfPts);
//
void delay(int seconds);

//	Finds the Euclidian Distance between two points in multi-D space
float dist(float* a, float* b, int boyut)
{
	float my_dist = 0;
	
	for (int k=0; k<boyut;k++)
		my_dist += pow( (a[k] - b[k]) , 2 ); // This formula is given wrong in TURK & Pentlands paper
	
	return(sqrt(my_dist));
}

//	Displays an Image
void ShowImage(IplImage* resim, char* title)
{
	cvNamedWindow(title, -1);
	cvShowImage(title, resim);
	cvWaitKey(0);
	cvDestroyWindow(title);
}

void ShowPixel(IplImage* img, int n, char* str)
{
	char* data = img->imageData;
	n =  data[n];
	CString tempStr;
	tempStr.Format("%s = %d", str, n);
	AfxMessageBox(tempStr);
}


void writeToFile(IplImage* imgs[4], IplImage* averageImage)
{
	FILE* averagefile;
	averagefile = fopen("c:\\average.txt", "w+");
	

	char* data[5];
	for (int i=0; i<4; i++)
		data[i] = imgs[i]->imageData;
	data[4] = averageImage->imageData;

	int stop;
	stop = (imgs[0]->width) * (imgs[0]->height);
	for (i=0; i<stop; i++)
		fprintf(averagefile, "IMG1=%d\tIMG2=%d\tIMG3=%d\tIMG4=%d\tAVE=%d\n", data[0][i], data[1][i], data[2][i], data[3][i], data[4][i]);
	fclose(averagefile);
}

void calculateAverageImage(IplImage* imgs[4], IplImage* awg)
{
	char* data[5];
	for (int i=0; i<4; i++)
		data[i] = imgs[i]->imageData;
	data[4] = awg->imageData;

	int stop;
	stop = (imgs[0]->width) * (imgs[0]->height);
	
	int AWG_temp;
	for (i = 0; i<stop;i++)
	{
		AWG_temp = 0;
		for (int n=0; n<4; n++)
			AWG_temp += data[n][i];
		data[4][i] = AWG_temp/4;
	}
}

void writeImageProperties(IplImage* image)
{
	FILE* fptr;
	fptr = fopen("C:\\Img_Properties.txt","w+");

	fprintf(fptr, "nSize = %d\n", image->nSize);
	fprintf(fptr, "ID = %d\n", image->ID);
	fprintf(fptr, "nChannels = %d\n", image->nChannels);
	fprintf(fptr, "depth = %d\n", image->depth);
	fprintf(fptr, "dataOrder = %d\n", image->dataOrder);
	fprintf(fptr, "origin = %d\n", image->origin);
	fprintf(fptr, "width = %d\n", image->width);
	fprintf(fptr, "height = %d\n", image->height);
	fprintf(fptr, "imageSize = %d\n", image->imageSize);
	fprintf(fptr, "widthStep = %d\n", image->widthStep);
	fclose(fptr);
}

void getDataOf32U (IplImage* img32U, IplImage* img8U)
{
	char* data1 = img32U->imageData;
	char* data2 = img8U->imageData;
	int stop;
	stop = (img32U->width) * (img32U->height);

	for (int i=0; i<stop; i++)
		data2[i] = data1[i];
}

void drawEyeRectangle(IplImage* face)
{
	CvSize faceSize;
	faceSize.width = face->width;
	faceSize.height = face->height;
	
	//	Template face is 18x24
	CvPoint pt1; pt1.x = (faceSize.width/18)*3; pt1.y = (faceSize.height/24)*11;
	CvPoint pt2; pt2.x = (faceSize.width/18)*7; pt2.y = (faceSize.height/24)*13;
	CvPoint pt3; pt3.x = (faceSize.width/18)*11; pt3.y = (faceSize.height/24)*11;
	CvPoint pt4; pt4.x = (faceSize.width/18)*15; pt4.y = (faceSize.height/24)*13;
	
	cvRectangle( face, pt1, pt2, CV_RGB(255,0,0), 2);
	cvRectangle( face, pt3, pt4, CV_RGB(255,0,0), 2);
}

void drawFaceRectangle(IplImage* face, IplImage* eigen, int x1, int y1)
{
	CvSize faceSize;
	faceSize.width = eigen->width;
	faceSize.height = eigen->height;
	
	CvPoint pt1; pt1.x = x1; pt1.y = y1;
	CvPoint pt2; pt2.x = x1+faceSize.width; pt2.y=y1+faceSize.height;

	
	cvRectangle( face, pt1, pt2, CV_RGB(255,0,0), 2);
}

void drawLine(IplImage* eye, CvPoint* points, int numOfPts)
{
	for (int k=0; k<numOfPts-1; k++)
	{
		cvLine(eye, points[k], points[k+1], CV_RGB(0, 0, 255), 1, 8);
	}
	cvLine(eye, points[k], points[0], CV_RGB(0, 0, 255), 1, 8);
}

void delay(int seconds)
{
	clock_t finish = clock() + (clock_t)(CLOCKS_PER_SEC*seconds);
	
	while (finish > clock())
		;
}