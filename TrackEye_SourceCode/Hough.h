//	Voting based Hough Transform for circle detection
//	Algorithm implementation by Zafer SAVAS
//	zsavas@yahoo.com

#pragma once

#include "cv.h"
#include "highgui.h"
#include "math.h"
#include "list"

using namespace std;

class voteNode {
public:
	int x;
	int y;
};

void Hough(IplImage* img, int* circle)
{		
	uchar* pxl_ptr;
	CvPixelPosition8u pos_src;
	//	Now we are pointing to the first pixel
	CV_INIT_PIXEL_POS(pos_src, (unsigned char *) img->imageData, img->widthStep, cvGetSize(img),0,0,img->origin);

	//	Define search space for radius
	const int search_xmin = img->width/4;
	const int search_ymin = img->height/4;
	const int search_xmax = img->width*3/4;//(img->width*7)/8;
	const int search_ymax = img->height*3/4;//(img->height*3)/4;
		
	const int x_max = img->width;
	const int y_max = img->height;
	const int r_max = min(img->width, img->height)/3;
		
	
	//	Our list to store radius information for each img pixel
	list<voteNode>*** P = new list<voteNode>** [x_max];


	int i,j;
	for (i = 0; i < x_max; i++)
	{
		P[i] = new list<voteNode>*[y_max];
		for (j = 0; j < y_max; j++)
			P[i][j] = new list<voteNode>[r_max];
	}

	//	Start searching img for 255 pixels
	int r;
	voteNode temp;
	int x, y;

	for (i = search_xmin; i < search_xmax; i++)
	{
		for (j = search_ymin; j < search_ymax; j++)
		{
			for (x = x_max/6; x < x_max*5/6; x++)
			{
				for (y = y_max/6; y < y_max*5/6; y++)
				{
					pxl_ptr = CV_MOVE_TO(pos_src, x, y, 1); // Now we point to the ixjth pixel
					
					if (*pxl_ptr == 255)	// whites are edges
					{
						r = (int) cvRound( sqrt( pow((i-x), 2.0) + pow((j-y), 2.0) ) );
						temp.x = x; temp.y = y;
						if (r<r_max)
							P[i][j][r].push_front(temp);					
					}
				}
			}
		}
	}
	
	int k;
	int cx, cy;

	int max_r = 0;
	int max_radius_vote = 0;
	list<voteNode>::iterator votes;
	bool flag;
	
	for (i = search_xmin; i < search_xmax; i++)
	{
		for ( j = search_xmin; j < search_ymax; j++)
		{
			// find highest voted radius
			for (k = 0; k < r_max; k++)
			{
				if (P[i][j][k].size() > max_radius_vote)
				{
					max_radius_vote = P[i][j][k].size();
					max_r = k;
					cx = i; cy = j;
				}
			}
		}
	}

	//	Get the coordinates and radius information
	circle[0] = cx; circle[1] = cy; circle[2] = max_r; 
		
	delete []P;
}

void findCircles(IplImage* inputImg, int* circleInfo)
{
	CvMemStorage* storage = cvCreateMemStorage(1000);
	CvSeq* firstContour;
	int headerSize = sizeof(CvContour);

	int count;
	CvPoint* pointArray;
	CvPoint2D32f* pointArray32f;
	int i;

	CvBox2D* myBox = (CvBox2D*)malloc(sizeof(CvBox2D));
	CvPoint myCenter;
	int height, width;
	
	//	**	Contours are found
	cvFindContours(inputImg, storage, &firstContour, headerSize, 
				   CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1 );

	//cvNamedWindow("draw", 0);
	//	**	Search for valid contours
	int max = 0;
	while (firstContour != NULL)
	{
		//cvDrawContours(inputImg, firstContour,RGB(0,0,255),CV_FILLED,0);
		//cvShowImage("draw", inputImg);
		//cvWaitKey(0);
		
		//	not a point?
		if ( CV_IS_SEQ_CURVE(firstContour) )
		{
			count = firstContour->total;

			pointArray = (CvPoint*) malloc(count*sizeof(CvPoint));
			pointArray32f = (CvPoint2D32f*) malloc((count+1)*sizeof(CvPoint2D32f));

			//	**	Get contour points
			cvCvtSeqToArray(firstContour, pointArray, CV_WHOLE_SEQ);
			//	**	Convert to 32f points
			for (i=0; i<count; i++)
			{
				pointArray32f[i].x = (float)pointArray[i].x;
				pointArray32f[i].y = (float)pointArray[i].y;
			}
			pointArray32f[i].x = pointArray[0].x;
			pointArray32f[i].y = pointArray[0].y;

			if (count>=6)
			{
	
				//	**	Fit Ellipse to the points
				cvFitEllipse(pointArray32f, count, myBox);

				myCenter.x = (int)myBox->center.x;
				myCenter.y = (int)myBox->center.y;
				height	= (int)myBox->size.height;
				width	= (int)myBox->size.width;
				
				//cvCircle(inputImg,myCenter, (int)length/2 ,RGB(0,0,255));
				//float myAngle= myBox->angle;
				//cvEllipse(inputImg, myCenter, cvSize ((int)width/2,(int)height/2), -myBox->angle, 0,360,RGB(0,255,0),1);
				
				//	**	Check whether it is a valid connected component or not?
				if ( (myCenter.x > 0) && (myCenter.y > 0) && 
					 ((height+width) > max) && ((height-width) <= width) && 
					 ((width-height) <= height) && (width <= (int)inputImg->width/2) && (height <= (int)inputImg->height/2))
				{
					max = height + width;
					circleInfo[0] = myCenter.x;
					circleInfo[1] = myCenter.y;
					height>width ? circleInfo[2]=(int)height/2 : circleInfo[2]=(int)width/2;
				}
			}
			
			free(pointArray);
			free(pointArray32f);
		}
		//cvShowImage("draw", inputImg); cvWaitKey(0);
		firstContour = firstContour->h_next;	
	}

	free(myBox);
	cvReleaseMemStorage(&storage);
	
	//cvDestroyWindow("draw");
}
	