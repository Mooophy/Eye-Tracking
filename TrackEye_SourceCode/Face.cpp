// Face.cpp: implementation of the Face class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrackEye.h"
#include "Face.h"
#include "TezLibrary_1.h"
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class eigenSize {
public:
	float width;
	float height;
	float conversion;
};


class eyeSearchSpace{
public:
	int xlimitLeft;
	int xlimitRight;
	int ylimitUp;
	int ylimitDown;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Face::Face(TrackingSettings* settings)
{
	numOfImages = settings->params->nImages;
	database = (IplImage**) malloc(sizeof(IplImage*) * numOfImages);
	
	//float vals[neigens];
	vals = (float*)malloc(sizeof(float)*numOfImages);
	
	//float weights[nEigens];
	weights = (float*)malloc(sizeof(float)*numOfImages);

	//	dynamic declaration of decomposition coefficients
	coeffs;	// n -> Number of images, nEigens -> number of eigenvectors
	coeffs = (float**)malloc(sizeof(float*)*numOfImages);
	for (int s=0; s<numOfImages; s++)
		coeffs[s] = (float*)malloc(sizeof(float)*settings->params->nEigens);
}


void Face::loadDatabase(TrackingSettings* settings)
{
	if (settings->params->trackEyes == 1)	//	then load eye database
	{
		CString fileName;
		fileName.Format("%s\\eye%d.jpg", settings->params->DBdirectory, 0);

		IplImage* tempImage	= cvLoadImage(fileName, -1);
		if (tempImage == NULL)
		{
			MessageBox(NULL, "Database is empty.\n Please check your eye database folder", "Error", MB_OK||MB_ICONSTOP);
			cvReleaseImage(&tempImage);
			exit(1);
		}
		else	//	Start loading database images
		{
			for (int i=0; i<numOfImages; i++)
			{
				fileName.Format("%s\\eye%d.jpg", settings->params->DBdirectory, i);
				tempImage = cvLoadImage(fileName, -1);

				if (tempImage == NULL)
				{
					MessageBox(NULL, "Database error.\n Please check your eye database folder", "Error", MB_OK||MB_ICONSTOP);
					cvReleaseImage(&tempImage);
					exit(1);
				}
				
				//	Change color space only once
				database[i] = cvCreateImage(cvGetSize(tempImage), IPL_DEPTH_8U, 1);
				changeColorSpace(settings, tempImage, database[i]);

				if (i == 1)
					if (settings->m_eyeAlgorithm == 0) // Template Matching
						break; // Only first two images are enough for template matching
			}
		}
		
		cvReleaseImage(&tempImage);
	}
}


void Face::unloadDatabase(TrackingSettings* settings)
{
	if (settings->params->trackEyes == 1)
	{
		for (int i=0; i<numOfImages; i++)
			cvReleaseImage(&database[i]);
	}
}


void Face::changeColorSpace(TrackingSettings* settings, IplImage* source, IplImage* dest)
{
	if (settings->params->colorSpace == CV_RGB2GRAY)
			cvCvtColor(source, dest, CV_RGB2GRAY);
	else
	{
		IplImage* copyOfSource = cvCreateImage(cvGetSize(source), IPL_DEPTH_8U, 3); 

		cvCvtColor(source, copyOfSource, settings->params->colorSpace);
		
		switch (settings->params->component)
		{
		case 1:
			cvCvtPixToPlane( copyOfSource, dest, NULL, NULL, NULL );
			break;
		case 2:
			cvCvtPixToPlane( copyOfSource, NULL, dest, NULL, NULL );
			break;
		case 3:
			cvCvtPixToPlane( copyOfSource, NULL, NULL, dest, NULL );
			break;
		}
		cvReleaseImage(&copyOfSource);
	}
}


void Face::findEyes(IplImage* faceImage, TrackingSettings* settings)
{
	IplImage** images = (IplImage**)malloc(sizeof(IplImage*)*numOfImages);
	IplImage** eigens = (IplImage**)malloc(sizeof(IplImage*)*numOfImages);

	IplImage* averageImage;
	IplImage* projection;
	
	CvSize faceSize; faceSize = cvGetSize(faceImage);
	eigenSize newEigenSize;

	newEigenSize.width = faceSize.width * settings->params->ratio;
	
	newEigenSize.conversion = ((float)newEigenSize.width) / ((float)database[0]->width); 
	newEigenSize.height = ((float)database[0]->height) * newEigenSize.conversion;
	
	CvSize newSize;
	newSize.width = (int)newEigenSize.width;
	newSize.height = (int)newEigenSize.height;
	
	IplImage* tempImg = cvCreateImage( newSize, IPL_DEPTH_8U, 1);
	//	**********Initializations**********
	for (int i=0; i<settings->params->nImages; i++)
	{
		/*cvResize(database[i], tempImg, CV_INTER_LINEAR); // was NN
		images[i] = cvCreateImage( cvGetSize(tempImg), IPL_DEPTH_8U, 1);
		
		//changeColorSpace(settings, tempImg, images[i]);
		images[i] = database[i];*/

		images[i] = cvCreateImage(newSize, IPL_DEPTH_8U, 1);
		cvResize(database[i], images[i], CV_INTER_LINEAR); // was NN
	}
	cvShowImage("Eigen", images[0]);
	cvReleaseImage(&tempImg); 
	
	//	Create space for EigenFaces
	//for (i=0; i<nEigens; i++)
	for (i=0; i<settings->params->nImages; i++)
		eigens[i] = cvCreateImage(cvGetSize(images[0]), IPL_DEPTH_32F, 1);
	
	averageImage = cvCreateImage(cvGetSize(images[0]), IPL_DEPTH_32F, 1);
	projection = cvCreateImage(cvGetSize(images[0]), IPL_DEPTH_8U, 1);
	
	//	*************************************************************
	//	************Calculate EigenVectors & EigenValues*************
	//	*************************************************************
	CvTermCriteria criteria;
	criteria.type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
    criteria.maxIter = 13;
    criteria.epsilon = 0.1;
	
	//	**	n was present instead of numOfImages
	cvCalcEigenObjects( settings->params->nImages, images, eigens, 0, 0, 0, &criteria, averageImage, vals );
	
	//////////////////////////////
	//	**	Blink Detection	**	//
	//////////////////////////////
	if (settings->params->blink == 1)	// detect blinks
	{			
		//	*************************************************************
		//	************Calculate Decomposition coefficients*************
		//	*************************************************************
		for( i=0; i<settings->params->nImages; i++ )
			cvEigenDecomposite( images[i], settings->params->nEigens, eigens, 0, 0, averageImage, coeffs[i] );

		//	*************************************************************
		//	*************Normalize Decomposition Coefficients************
		//	*************************************************************
		/*int max, val, j;
		for( i=0; i<settings->params->nEigens; i++ )
		{
			max = -100000.00;

			for( j=0; j<settings->params->nImages; j++ )
			{
				val = fabs( coeffs[j][i] );
				if( val > max )
					max = val;
			}
			for( j=0; j<settings->params->nImages; j++ )
				coeffs[j][i]=coeffs[j][i]/max;
		}*/
		
		//	Projection vector averaging
		int numOfBlinks = settings->params->nImages/2;
		float average = 0;
		int row, col;
		//	**	Average coefficients of blink eyes
		for (col=0; col<settings->params->nEigens; col++)
		{
			for (row=0; row<numOfBlinks; row++)
				average += coeffs[row][col];
			coeffs[0][col] = (float)(average / numOfBlinks);
			average = 0;
		}
		//	**	Average coefficients of non-blink eyes
		for (col=0; col<settings->params->nEigens; col++)
		{
			for (row=numOfBlinks; row<settings->params->nImages; row++)
				average += coeffs[row][col];
			coeffs[1][col] = (float)(average / numOfBlinks);
			average = 0;
		}
	}

	//	*************************************************************
	//	************Search faceImage for eyes************************
	//	*************************************************************
	IplImage* GRAYfaceImage = cvCreateImage(faceSize, IPL_DEPTH_8U, 1);
	changeColorSpace(settings,faceImage, GRAYfaceImage); 
	//cvCvtColor( faceImage, GRAYfaceImage, CV_RGB2GRAY);
	GRAYfaceImage->origin = 1;	//	**	Warning at this point image origin is bottom-left corner.

	int MARGIN = settings->params->MaxError; 
	double minimum = MARGIN; double distance = MARGIN; 
	
	//	**	Eye1 search Space
	settings->params->eye1.xlimitLeft = 0;
	settings->params->eye1.xlimitRight = faceSize.width/2.0 - images[0]->width - 1;
	settings->params->eye1.ylimitUp = (int)( ((float)faceSize.height)*0.75 - images[0]->height - 1); 
	settings->params->eye1.ylimitDown = faceSize.height/2;
	
	//	**	Eye2 search Space
	settings->params->eye2.xlimitLeft = faceSize.width/2.0;
	settings->params->eye2.xlimitRight = faceSize.width - images[0]->width - 1;
	settings->params->eye2.ylimitUp = (int)( ((float)faceSize.height)*0.75 - images[0]->height - 1);
	settings->params->eye2.ylimitDown = faceSize.height/2;
		
	settings->params->eye1.initializeEyeParameters();
	settings->params->eye2.initializeEyeParameters();
	settings->params->eye1.RectSize.width = images[0]->width;
	settings->params->eye1.RectSize.height = images[0]->height;
	settings->params->eye2.RectSize.width = images[0]->width;
	settings->params->eye2.RectSize.height = images[0]->height;

	IplImage* Image2Comp = cvCreateImage(cvGetSize(images[0]), IPL_DEPTH_8U, 1);
	int x,y;
	//	**	Search left eye i.e eye1
	for (y=settings->params->eye1.ylimitDown; y<settings->params->eye1.ylimitUp; y+=2)
	{
		for (x=settings->params->eye1.xlimitLeft; x<settings->params->eye1.xlimitRight; x+=2)
		{			
			cvSetImageROI(GRAYfaceImage, cvRect(x, y, images[0]->width, images[0]->height));
			if (settings->params->varianceCheck == 1 )
			{
				if (calculateSTD(GRAYfaceImage) <= (double)(settings->params->variance))
				{
					cvResetImageROI(GRAYfaceImage);
					continue;
				}
			}
			cvFlip( GRAYfaceImage, Image2Comp, 0);
			cvResetImageROI(GRAYfaceImage);
			//	**	Decide whether it is an eye or not
			//cvShowImage("ROI", Image2Comp); cvWaitKey(60);
			cvEigenDecomposite( Image2Comp, settings->params->nEigens, eigens, 0, 0, averageImage, weights );
			cvEigenProjection( eigens, settings->params->nEigens, CV_EIGOBJ_NO_CALLBACK, 0, weights, averageImage, projection );
			distance = cvNorm(Image2Comp, projection, CV_L2, 0);
			
			if (distance < minimum && distance > 0)
			{
				settings->params->eye1.eyefound = true;
				minimum = distance;
				settings->params->eye1.distance = distance;
				settings->params->eye1.coords.x = x;
				settings->params->eye1.coords.y = y;
				
				//	**	Eye blink or not?
				if (settings->params->blink == 1)
					settings->params->eye1.blinkStatus = ( RecognizeEyeState(coeffs[0], weights, settings) < RecognizeEyeState(coeffs[1], weights, settings) );
				//cvShowImage("ROI", projection);
			}	
		}
	}
	
	minimum = MARGIN; distance = MARGIN;
	
	//	**	Search right eye i.e eye2
	for (y=settings->params->eye2.ylimitDown; y<settings->params->eye2.ylimitUp; y+=2)
	{
		for (x=settings->params->eye2.xlimitLeft; x<settings->params->eye2.xlimitRight; x+=2)
		{			
			cvSetImageROI(GRAYfaceImage, cvRect(x, y, images[0]->width, images[0]->height));
			if (settings->params->varianceCheck == 1)
			{
				if (calculateSTD(GRAYfaceImage) <= (double)(settings->params->variance))
				{
					cvResetImageROI(GRAYfaceImage);
					continue;
				}
			}
			cvFlip( GRAYfaceImage, Image2Comp, 0);
			cvResetImageROI(GRAYfaceImage);
			//	**	Decide whether it is an eye or not
			//cvShowImage("ROI", Image2Comp); cvWaitKey(60);
			cvEigenDecomposite( Image2Comp, settings->params->nEigens, eigens, 0, 0, averageImage, weights );
			cvEigenProjection( eigens, settings->params->nEigens, 0, 0, weights, averageImage, projection );
			distance = cvNorm(Image2Comp, projection, CV_L2, 0);
			
			if (distance < minimum && distance > 0)
			{
				settings->params->eye2.eyefound = true;
				minimum = distance;
				settings->params->eye2.distance = distance;
				settings->params->eye2.coords.x = x;
				settings->params->eye2.coords.y = y;

				//	**	Eye blink or not?
				if (settings->params->blink == 1)
					settings->params->eye2.blinkStatus = ( RecognizeEyeState(coeffs[0], weights, settings) < RecognizeEyeState(coeffs[1], weights, settings) );
			}	
		}
	}
	cvReleaseImage(&Image2Comp);
	
	//	**	Cleanup
	cvReleaseImage(&GRAYfaceImage);
	for (i=0; i<settings->params->nImages; i++)
		cvReleaseImage(&images[i]);

	for (i=0; i<settings->params->nImages; i++)
		cvReleaseImage(&eigens[i]);
	
	cvReleaseImage(&averageImage);
	cvReleaseImage(&projection);

	free(images);
	free(eigens);
}

// Template Matching for Eye detection
void Face::findEyes_TM(IplImage* faceImage, TrackingSettings* settings)
{
	CvSize faceSize; faceSize = cvGetSize(faceImage);

	// Load Template from the eye database
	CString fileName;
	fileName.Format("%s\\eye%d.jpg", settings->params->DBdirectory, 0); // Name of the template for left eye
	IplImage* eyeImage_Left	= cvLoadImage(fileName, -1);
	fileName.Format("%s\\eye%d.jpg", settings->params->DBdirectory, 1); // Name of the template for left eye
	IplImage* eyeImage_Right = cvLoadImage(fileName, -1);
	
	IplImage* tempTemplateImg_Left; IplImage* tempTemplateImg_Right;
	IplImage* templateImg_Left; IplImage* templateImg_Right;
	
	if (eyeImage_Left == NULL || eyeImage_Right == NULL)
	{
		MessageBox(NULL, "Templates can not be loaded.\n Please check your eye database folder", "Error", MB_OK||MB_ICONSTOP);
		exit(1);
	}
	else
	{
		//	Change color space according to the settings entered by the user
		tempTemplateImg_Left = cvCreateImage(cvGetSize(eyeImage_Left), IPL_DEPTH_8U, 1);
		changeColorSpace(settings, eyeImage_Left, tempTemplateImg_Left);
		tempTemplateImg_Right = cvCreateImage(cvGetSize(eyeImage_Right), IPL_DEPTH_8U, 1);
		changeColorSpace(settings, eyeImage_Right, tempTemplateImg_Right);

		float idealWidth = faceSize.width * settings->params->ratio;
		float conversionRatio = idealWidth/(float)tempTemplateImg_Left->width;
		
		CvSize newSize;
		newSize.width = (int)idealWidth;
		newSize.height = (int)(tempTemplateImg_Left->height*conversionRatio);

		templateImg_Left = cvCreateImage(newSize, IPL_DEPTH_8U, 1);
		cvResize(tempTemplateImg_Left, templateImg_Left, CV_INTER_LINEAR); // was NN
		cvReleaseImage(&eyeImage_Left);
		cvReleaseImage(&tempTemplateImg_Left);

		templateImg_Right = cvCreateImage(newSize, IPL_DEPTH_8U, 1);
		cvResize(tempTemplateImg_Right, templateImg_Right, CV_INTER_LINEAR); // was NN
		cvReleaseImage(&eyeImage_Right);
		cvReleaseImage(&tempTemplateImg_Right);
	}
	//	*************************************************************
	//	************Search faceImage for eyes************************
	//	*************************************************************
	IplImage* GRAYfaceImage = cvCreateImage(faceSize, IPL_DEPTH_8U, 1);
	changeColorSpace(settings, faceImage, GRAYfaceImage); 
	//cvCvtColor( faceImage, GRAYfaceImage, CV_RGB2GRAY);
	//GRAYfaceImage->origin = 1;	//	**	Warning at this point image origin is bottom-left corner.
	
	//	**	Eye1 search area
	int x_left = 0;
	int y_left = 0;
	int width_left = (int)((float)(faceSize.width/2.0));
	int height_left = (int)((float)(faceSize.height));
	CvRect rect_Eye1 = cvRect(x_left, y_left, width_left, height_left);
	
	CvMat* Eye1Image = cvCreateMat(width_left, height_left, CV_8UC1);
	cvGetSubRect(GRAYfaceImage, Eye1Image, rect_Eye1 );
	cvFlip( Eye1Image, Eye1Image, 0);

	//	**	Eye2 search area
	int x_right= (int)((float)(faceSize.width/2.0));
	int y_right = 0;
	int width_right = (int)((float)(faceSize.width/2.0));
	int height_right = (int)((float)(faceSize.height));
	CvRect rect_Eye2 = cvRect(x_right, y_right, width_right, height_right);
	
	CvMat* Eye2Image = cvCreateMat(width_right, height_right, CV_8UC1);
	cvGetSubRect(GRAYfaceImage, Eye2Image, rect_Eye2 );
	cvFlip( Eye2Image, Eye2Image, 0);

	// OpenCV says that size of the result must be the following:
	CvSize size;
	size.height= Eye1Image->height - templateImg_Left->height + 1; 
	size.width = Eye1Image->width - templateImg_Left->width + 1;
	IplImage* result1 = cvCreateImage( size,IPL_DEPTH_32F,1);
	IplImage* result2 = cvCreateImage( size,IPL_DEPTH_32F,1);
 
	// Left Eye
	cvMatchTemplate( Eye1Image, templateImg_Left, result1, settings->params->tempMatch);
	// Right Eye
	cvMatchTemplate( Eye2Image, templateImg_Right, result2, settings->params->tempMatch);
	
	// find the best match location - LEFT EYE
    double minValue1, maxValue1;
    CvPoint minLoc1, maxLoc1;
    cvMinMaxLoc( result1, &minValue1, &maxValue1, &minLoc1, &maxLoc1 );
	cvCircle( result1, maxLoc1, 5, settings->programColors.colors[2], 1 ); 
	// transform point back to original image
    maxLoc1.x += templateImg_Left->width / 2;
    maxLoc1.y += templateImg_Left->height / 2;
	settings->params->eye1.coords.x = maxLoc1.x;
	settings->params->eye1.coords.y = maxLoc1.y;
	settings->params->eye1.RectSize.width = templateImg_Left->width;
	settings->params->eye1.RectSize.height = templateImg_Left->height;
	settings->params->eye1.eyefound = true;

	// find the best match location - RIGHT EYE
    double minValue2, maxValue2;
    CvPoint minLoc2, maxLoc2;
    cvMinMaxLoc( result2, &minValue2, &maxValue2, &minLoc2, &maxLoc2 );
	cvCircle( result2, maxLoc2, 5, settings->programColors.colors[2], 1 ); 
	// transform point back to original image
    maxLoc2.x += templateImg_Left->width / 2;
    maxLoc2.y += templateImg_Left->height / 2;
	settings->params->eye2.coords.x = maxLoc2.x+(int)faceSize.width/2;
	settings->params->eye2.coords.y = maxLoc2.y;
	settings->params->eye2.RectSize.width = templateImg_Left->width;
	settings->params->eye2.RectSize.height = templateImg_Left->height;
	settings->params->eye2.eyefound = true;
	
	cvCircle( Eye1Image, maxLoc1, 5, settings->programColors.colors[2], 1 );
	cvCircle( Eye2Image, maxLoc2, 5, settings->programColors.colors[2], 1 );
	
	cvNamedWindow("Eye1Image", -1 );
	cvShowImage("Eye1Image", Eye1Image);
	cvNamedWindow("Eye2Image", -1 );
	cvShowImage("Eye2Image", Eye2Image);
	cvNamedWindow("template_Left", -1 );
	cvShowImage("template_Left", templateImg_Left);
	cvNamedWindow("template_Right", -1 );
	cvShowImage("template_Right", templateImg_Right);
	cvNamedWindow("RESULT1", -1 );
	cvShowImage("RESULT1", result1);
	cvNamedWindow("RESULT2", -1 );
	cvShowImage("RESULT2", result2);

	cvReleaseImage(&GRAYfaceImage);
	cvReleaseImage(&templateImg_Left);
	cvReleaseImage(&templateImg_Right);
	cvReleaseMat(&Eye1Image);
	cvReleaseMat(&Eye2Image);
	cvReleaseImage(&result1);
	cvReleaseImage(&result2);
}


void Face::UpdateCoordinates(IplImage* frame, CvBox2D faceBox, TrackingSettings* settings)
{
	if (settings->m_eyeAlgorithm == 1) // PCA
	{
		if (settings->params->eye1.eyefound == true)
		{
			settings->params->eye1.coords.x = faceBox.center.x - (faceBox.size.width)/2 + settings->params->eye1.coords.x;
			settings->params->eye1.coords.y = faceBox.center.y - (faceBox.size.height)/2 + settings->params->eye1.coords.y;
		}
		if (settings->params->eye2.eyefound == true)
		{
			settings->params->eye2.coords.x = faceBox.center.x - (faceBox.size.width)/2 + settings->params->eye2.coords.x;
			settings->params->eye2.coords.y = faceBox.center.y - (faceBox.size.height)/2 + settings->params->eye2.coords.y;
		}
	}
	else if (settings->m_eyeAlgorithm == 0) // Haar
	{
		// Ana paneldeki görüntünün origini sol alt köþe
		// Template matching ile bulduðum noktalarýn orjini ise sol-üst köþedir
		settings->params->eye1.coords.x = faceBox.center.x-(faceBox.size.width)/2+settings->params->eye1.coords.x-settings->params->eye1.RectSize.width/2;
		settings->params->eye1.coords.y = faceBox.center.y+(faceBox.size.height)/2-settings->params->eye1.coords.y-settings->params->eye1.RectSize.height/2;
		settings->params->eye2.coords.x = faceBox.center.x-(faceBox.size.width)/2+settings->params->eye2.coords.x-settings->params->eye2.RectSize.width/2;
		settings->params->eye2.coords.y = faceBox.center.y+(faceBox.size.height)/2-settings->params->eye2.coords.y-settings->params->eye2.RectSize.height/2;
	}
}


void Face::detectEyePupil(IplImage* faceImage, TrackingSettings* settings)
{
	if (settings->params->eye1.eyefound == true)
	{
		IplImage* eyeRegion = cvCreateImage(settings->params->eye1.RectSize, IPL_DEPTH_8U, 3);
		cvSetImageROI(faceImage, cvRect(settings->params->eye1.coords.x, 
			settings->params->eye1.coords.y, 
			settings->params->eye1.RectSize.width, 
			settings->params->eye1.RectSize.height));
		eyeRegion = cvCloneImage(faceImage);
				
		//	Find Eye Pupil
		IplImage* work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 3);
		cvCvtColor(eyeRegion, work1, settings->params->pupilCalib->thresholdingType);
		
		IplImage* work2 = cvCreateImage(cvGetSize(work1), IPL_DEPTH_8U, 1);
		if (settings->params->pupilCalib->m_thresholdComponent == 2)
			cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
		else if (settings->params->pupilCalib->m_thresholdComponent == 1)
			cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
		else if (settings->params->pupilCalib->m_thresholdComponent == 0)
			cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
		
		if (settings->params->pupilCalib->m_binaryThreshold == true)
			cvThreshold(work2, work2, settings->params->pupilCalib->m_thresholdValue, 255, CV_THRESH_BINARY_INV);

		cvCanny(work2, work2, settings->params->pupilCalib->m_cannyLower, 
							  settings->params->pupilCalib->m_cannyUpper,
							  settings->params->pupilCalib->m_cannyApertureSize);
				
		settings->params->eye1.detectPupils(work2, settings->params->pupilCalib->circleAlgorithm);
		
		cvResetImageROI(faceImage);
		
		cvReleaseImage(&eyeRegion);
		cvReleaseImage(&work1);
		cvReleaseImage(&work2);
	}
	
	if (settings->params->eye2.eyefound == true)
	{
		IplImage* eyeRegion = cvCreateImage(settings->params->eye2.RectSize, IPL_DEPTH_8U, 3);
		cvSetImageROI(faceImage, cvRect(settings->params->eye2.coords.x, 
			settings->params->eye2.coords.y, 
			settings->params->eye2.RectSize.width, 
			settings->params->eye2.RectSize.height));
		eyeRegion = cvCloneImage(faceImage);
		
		//	Find Eye Pupil
		IplImage* work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 3);
		cvCvtColor(eyeRegion, work1, settings->params->pupilCalib->thresholdingType);
		
		IplImage* work2 = cvCreateImage(cvGetSize(work1), IPL_DEPTH_8U, 1);
		if (settings->params->pupilCalib->m_thresholdComponent == 2)
			cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
		else if (settings->params->pupilCalib->m_thresholdComponent == 1)
			cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
		else if (settings->params->pupilCalib->m_thresholdComponent == 0)
			cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
		
		if (settings->params->pupilCalib->m_binaryThreshold == true)
			cvThreshold(work2, work2, settings->params->pupilCalib->m_thresholdValue, 255, CV_THRESH_BINARY_INV);

		cvCanny(work2, work2, settings->params->pupilCalib->m_cannyLower, 
							  settings->params->pupilCalib->m_cannyUpper,
							  settings->params->pupilCalib->m_cannyApertureSize);
				
		settings->params->eye2.detectPupils(work2, settings->params->pupilCalib->circleAlgorithm);
		
		cvResetImageROI(faceImage);
		
		cvReleaseImage(&eyeRegion);
		cvReleaseImage(&work1);
		cvReleaseImage(&work2);
	}
}


void Face::drawEyeRectangles(IplImage* CameraFrame, TrackingSettings* settings)
{	
	CvPoint pt1, pt2;
	if (settings->params->eye1.eyefound == true)
	{	
			pt1.x = settings->params->eye1.coords.x;
			pt1.y = settings->params->eye1.coords.y;
			pt2.x = pt1.x + settings->params->eye1.RectSize.width; 
			pt2.y = pt1.y + settings->params->eye1.RectSize.height;

			//	Draw Eye Frame
			if (settings->programColors.m_drawEye == true)
				cvRectangle( CameraFrame, pt1, pt2, settings->programColors.colors[1], 2);
			if (settings->programColors.m_drawEyeCircle == true)
			{
				cvCircle( CameraFrame, pt1, 5, settings->programColors.colors[2], 1 ); 
				cvCircle( CameraFrame, pt2, 5, settings->programColors.colors[2], 1 );
			}
	}
	if (settings->params->eye2.eyefound == true)
	{	
			pt1.x = settings->params->eye2.coords.x;
			pt1.y = settings->params->eye2.coords.y;
			pt2.x = pt1.x + settings->params->eye2.RectSize.width; 
			pt2.y = pt1.y + settings->params->eye2.RectSize.height;

			//	Draw Eye Frame
			if (settings->programColors.m_drawEye == true)
				cvRectangle( CameraFrame, pt1, pt2, settings->programColors.colors[1], 2);
			if (settings->programColors.m_drawEyeCircle == true)
			{
				cvCircle( CameraFrame, pt1, 5, settings->programColors.colors[2], 1 ); 
				cvCircle( CameraFrame, pt2, 5, settings->programColors.colors[2], 1 );
			}
	}
	//cvLine(faceImage, cvPoint(0, 0), cvPoint(40, 10), CV_RGB(0,255,0), 4, 8);
}


void Face::drawEyePupilCircles(IplImage* CameraFrame, TrackingSettings* settings)
{
	if (settings->params->pupil == 1)
	{
		//	Draw Pupils
		if (settings->params->eye1.eyefound == true && settings->programColors.m_drawPupil == true)
		{
			cvCircle(CameraFrame, cvPoint(settings->params->eye1.coords.x + settings->params->eye1.eyePupil.x,
					 settings->params->eye1.coords.y + settings->params->eye1.eyePupil.y), 
					 settings->params->eye1.eyePupil.radius, settings->programColors.colors[3], CV_FILLED);
		}
		if (settings->params->eye2.eyefound == true && true && settings->programColors.m_drawPupil == true)
		{
			cvCircle(CameraFrame, cvPoint(settings->params->eye2.coords.x + settings->params->eye2.eyePupil.x,
					 settings->params->eye2.coords.y + settings->params->eye2.eyePupil.y), 
					 settings->params->eye2.eyePupil.radius, settings->programColors.colors[3], CV_FILLED);
		}
	}
}

//	**	Blink Detection
float Face::RecognizeEyeState(float* eyeClass, float* image, TrackingSettings* settings)
{
	double sum = 0;
	for (int i=0; i<settings->params->nEigens; i++)
		sum += pow( abs(eyeClass[i] - image[i]) , 2 );
	return((float)sqrt(sum));
}


void Face::snake(IplImage* eyeRegion, TrackingSettings* settings)
{
	bool algorithmControl = false;
	CvSize ROISize;

	for (int c=0; c<=1; c++)
	{
		if (c==0 && settings->params->eye1.eyefound == true && (settings->params->eye1.RectSize.width >= eyeRegion->width/10))
		{
			ROISize = cvSize(settings->params->eye1.RectSize.width, settings->params->eye1.RectSize.height);
			cvSetImageROI(eyeRegion, cvRect(settings->params->eye1.coords.x, 
				settings->params->eye1.coords.y, 
				settings->params->eye1.RectSize.width, 
				settings->params->eye1.RectSize.height));
			algorithmControl = true;
		}
		else if (c==1 && settings->params->eye2.eyefound == true && (settings->params->eye2.RectSize.width >= eyeRegion->width/10))
		{
			ROISize = cvSize(settings->params->eye2.RectSize.width, settings->params->eye2.RectSize.height);
			cvSetImageROI(eyeRegion, cvRect(settings->params->eye2.coords.x, 
				settings->params->eye2.coords.y, 
				settings->params->eye2.RectSize.width, 
				settings->params->eye2.RectSize.height));
			algorithmControl = true;
		}

		if (algorithmControl == true)
		{
			//	**	Colorspace converted image
			IplImage* work1 = NULL;
			if (settings->params->snakeCalib->colorSpace == CV_RGB2GRAY)
			{
				work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 1);
				cvCvtColor(eyeRegion, work1, settings->params->snakeCalib->colorSpace);
			}
			else
			{
				work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 3);
				if (settings->params->snakeCalib->colorSpace != -11)
					cvCvtColor(eyeRegion, work1, settings->params->snakeCalib->colorSpace);
				else
					work1 = cvCloneImage(eyeRegion); //	**	None is selected
			}
				
			//	**	Get Component
			IplImage* work2 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 1);
			if (settings->params->snakeCalib->colorSpace != CV_RGB2GRAY)
			{
				if (settings->params->snakeCalib->m_component == 0)
					cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
				else if (settings->params->snakeCalib->m_component == 1)
					cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
				else 
					cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
			}
			else
				work2 = cvCloneImage(work1);

			//	**	Binary thresholded image
			if (settings->params->snakeCalib->thresholdOrNot == true)
			{
				if (settings->params->snakeCalib->thresholdType == -11)
					cvThreshold(work2, work2, settings->params->snakeCalib->m_thresholdValue, 255, CV_THRESH_BINARY_INV);
				else
					cvAdaptiveThreshold(work2, work2, (float)255, settings->params->snakeCalib->thresholdType, CV_THRESH_BINARY_INV, settings->params->snakeCalib->m_blockSize, settings->params->snakeCalib->m_thresholdValue);
			}
			cvShowImage("threshold", work2);
			
			//	**	Edges
			//cvCanny(work2, work2, settings->params->snakeCalib->m_cannyLower, settings->params->snakeCalib->m_cannyUpper, settings->params->snakeCalib->m_cannyApertureSize);

			//	**	Snake minimization
			CvPoint* snk = (CvPoint*)malloc(sizeof(CvPoint)*settings->params->snakeCalib->m_nPoints);
			CvSize win = {3,3}; 
			CvTermCriteria criteria; 
			criteria.type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
			criteria.maxIter = settings->params->snakeCalib->m_iterations;
			criteria.epsilon = 1;
			
			//	Best parameters are 0, 0.1, 0.2
			float alpha= settings->params->snakeCalib->m_alpha;
			float beta = settings->params->snakeCalib->m_beta;
			float gamma = settings->params->snakeCalib->m_gamma;
			
			//	Initial positions of the active contour points
			int k;
			for (k=0; k<(settings->params->snakeCalib->m_nPoints/4); k++)
			{
				snk[k].x = k*ROISize.width/(settings->params->snakeCalib->m_nPoints/4);
				snk[k].y = 0;
			}
			for (k=(settings->params->snakeCalib->m_nPoints/4); k<(settings->params->snakeCalib->m_nPoints/2); k++)
			{
				snk[k].x = ROISize.width;
				snk[k].y = (k-(settings->params->snakeCalib->m_nPoints/4))*ROISize.height/(settings->params->snakeCalib->m_nPoints/4);;
			}
			for (k=(settings->params->snakeCalib->m_nPoints/2); k<(3*settings->params->snakeCalib->m_nPoints/4); k++)
			{
				snk[k].x = ROISize.width - (k-(settings->params->snakeCalib->m_nPoints/2))*ROISize.width/(settings->params->snakeCalib->m_nPoints/4);
				snk[k].y = ROISize.height;
			}
			for (k=(3*settings->params->snakeCalib->m_nPoints/4); k<settings->params->snakeCalib->m_nPoints; k++)
			{
				snk[k].x = 0;
				snk[k].y = ROISize.height - (k-(3*settings->params->snakeCalib->m_nPoints/4))*ROISize.height/(settings->params->snakeCalib->m_nPoints/4);;
			}
			
			//	Active Contour is minimizing its energy
			cvSnakeImage( work2, snk, settings->params->snakeCalib->m_nPoints, &alpha, &beta, &gamma, CV_VALUE, win, criteria, 1);
			
			//	**	Draw the result
			if (settings->programColors.m_drawSnake == true)
			{
				for (int n=0; n<settings->params->snakeCalib->m_nPoints; n++)
					cvCircle(eyeRegion, cvPoint(snk[n].x, snk[n].y) , 3, settings->programColors.colors[4], 1);
			}
			
			if (settings->params->fitellipse == 1)
			{
				// Fits ellipse to current contour.
				CvBox2D32f* box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
				CvPoint2D32f* snk2D32f = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f)*settings->params->snakeCalib->m_nPoints);
				for(int i=0; i<settings->params->snakeCalib->m_nPoints; i++)
				{
					snk2D32f[i].x = (float)snk[i].x;
					snk2D32f[i].y = (float)snk[i].y;
				}
    			cvFitEllipse(snk2D32f, settings->params->snakeCalib->m_nPoints, box);
			
				// Convert ellipse data from float to integer representation.
				CvPoint center; CvSize size;
				center.x = cvRound(box->center.x);
				center.y = cvRound(box->center.y);
				size.width = cvRound(box->size.width*0.5);
				size.height = cvRound(box->size.height*0.5);
				box->angle = -box->angle; 
				// Draw ellipse.
				if (settings->programColors.m_drawEllipse == true)
					cvEllipse(eyeRegion, center, size, box->angle, 0, 360, settings->programColors.colors[5], 1);

				free(snk2D32f);
				free(box);
			}

			free(snk);

			cvReleaseImage(&work1);
			cvReleaseImage(&work2);
		
			cvResetImageROI(eyeRegion);
			algorithmControl = false;
		}
	}
}


void Face::CCA(IplImage* eyeRegion, TrackingSettings* settings)
{
	bool algorithmControl = false;
	CvSize ROISize;

	for (int c=0; c<=1; c++)
	{
		if (c==0 && settings->params->eye1.eyefound == true && (settings->params->eye1.RectSize.width >= eyeRegion->width/10))
		{
			ROISize = cvSize(settings->params->eye1.RectSize.width, settings->params->eye1.RectSize.height);
			cvSetImageROI(eyeRegion, cvRect(settings->params->eye1.coords.x, 
				settings->params->eye1.coords.y, 
				settings->params->eye1.RectSize.width, 
				settings->params->eye1.RectSize.height));
			algorithmControl = true;
		}
		else if (c==1 && settings->params->eye2.eyefound == true && (settings->params->eye2.RectSize.width >= eyeRegion->width/10))
		{
			ROISize = cvSize(settings->params->eye2.RectSize.width, settings->params->eye2.RectSize.height);
			cvSetImageROI(eyeRegion, cvRect(settings->params->eye2.coords.x, 
				settings->params->eye2.coords.y, 
				settings->params->eye2.RectSize.width, 
				settings->params->eye2.RectSize.height));
			algorithmControl = true;
		}

		if (algorithmControl == true)
		{
			//	**	Colorspace converted image
			IplImage* work1 = NULL;
			if (settings->params->snakeCalib->colorSpace == CV_RGB2GRAY)
			{
				work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 1);
				cvCvtColor(eyeRegion, work1, settings->params->snakeCalib->colorSpace);
			}
			else
			{
				work1 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 3);
				if (settings->params->snakeCalib->colorSpace != -11)
					cvCvtColor(eyeRegion, work1, settings->params->snakeCalib->colorSpace);
				else
					work1 = cvCloneImage(eyeRegion); //	**	None is selected
			}
				
			//	**	Get Component
			IplImage* work2 = cvCreateImage(cvGetSize(eyeRegion), IPL_DEPTH_8U, 1);
			if (settings->params->snakeCalib->colorSpace != CV_RGB2GRAY)
			{
				if (settings->params->snakeCalib->m_component == 0)
					cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
				else if (settings->params->snakeCalib->m_component == 1)
					cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
				else 
					cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
			}
			else
				work2 = cvCloneImage(work1);

			//	**	Binary thresholded image
			if (settings->params->snakeCalib->thresholdOrNot == true)
			{
				if (settings->params->snakeCalib->thresholdType == -11)
					cvThreshold(work2, work2, settings->params->snakeCalib->m_thresholdValue, 255, CV_THRESH_BINARY_INV);
				else
					cvAdaptiveThreshold(work2, work2, (float)255, settings->params->snakeCalib->thresholdType, CV_THRESH_BINARY_INV, settings->params->snakeCalib->m_blockSize, settings->params->snakeCalib->m_thresholdValue);
			}
			cvShowImage("threshold", work2);
			
			//	**	Edges
			//cvCanny(work2, work2, settings->params->snakeCalib->m_cannyLower, settings->params->snakeCalib->m_cannyUpper, settings->params->snakeCalib->m_cannyApertureSize);

			//	**	Perform Connected Component Analysis (CCA) and fit elllipse
			CvMemStorage* storage = cvCreateMemStorage(1000);
			CvSeq* firstContour;
			int headerSize = sizeof(CvContour);

			int count=0;
			CvPoint* pointArray;
			CvPoint2D32f* pointArray32f;
			int i;

			CvBox2D* myBox = (CvBox2D*)malloc(sizeof(CvBox2D));
			CvPoint myCenter;
			int height, width;
			
			//	**	Contours are found
			cvFindContours(work2, storage, &firstContour, headerSize, 
						   CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

			//	**	Search for valid contours
			while (firstContour!=NULL) //&& count<6)
			{
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

					if (count>=7)
					{
			
						//	**	Fit Ellipse to the points
						cvFitEllipse(pointArray32f, count, myBox);

						myCenter.x = (int)myBox->center.x;
						myCenter.y = (int)myBox->center.y;
						height	= (int)myBox->size.height;
						width	= (int)myBox->size.width;
						
						if (settings->programColors.m_drawEllipse == true)
							cvEllipse(eyeRegion, myCenter, cvSize ((int)width/2,(int)height/2), -myBox->angle, 0, 360, settings->programColors.colors[5],1);
						break;
					}
					
					free(pointArray);
					free(pointArray32f);
				}
				firstContour = firstContour->h_next;	
			}

			free(myBox);
			cvReleaseMemStorage(&storage);
			
			cvReleaseImage(&work1);
			cvReleaseImage(&work2);
		
			cvResetImageROI(eyeRegion);
			algorithmControl = false;
		}
	}
}


Face::~Face()
{
	free(database);
	free(vals);
	free(weights);
	
	for(int i=0; i<numOfImages;i++)
		free(coeffs[i]);
	free(coeffs);
}

double Face::calculateSTD(IplImage* eyeImage)
{
	CvScalar avg, std;

	cvAvgSdv(eyeImage, &avg, &std);

	return(std.val[0]);
}
