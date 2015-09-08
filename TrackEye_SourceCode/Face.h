// Face.h: interface for the Face class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__19673440_ADFF_11D9_938B_F0B94510EF6A__INCLUDED_)
#define AFX_FACE_H__19673440_ADFF_11D9_938B_F0B94510EF6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "highgui.h"
#include "TrackingSettings.h"

class Face  
{
public:	
	double calculateSTD(IplImage*);
	void CCA(IplImage*, TrackingSettings*);
	void snake(IplImage*, TrackingSettings*);
	Face(TrackingSettings*);
	
	int numOfImages;
	IplImage** database;
	
	float* vals;
	float* weights;
	float** coeffs;

	//	**	Member Functions
	void loadDatabase(TrackingSettings*);
	void unloadDatabase(TrackingSettings*);
	void changeColorSpace(TrackingSettings*, IplImage*, IplImage*);
	void findEyes(IplImage*, TrackingSettings*);
	void findEyes_TM(IplImage*, TrackingSettings*);
	void drawEyeRectangles(IplImage*, TrackingSettings*);
	void UpdateCoordinates(IplImage*, CvBox2D, TrackingSettings*);
	void drawEyePupilCircles(IplImage* , TrackingSettings*);
	void detectEyePupil(IplImage*, TrackingSettings*);
	float RecognizeEyeState(float*, float*, TrackingSettings*);

	virtual ~Face();
};

#endif // !defined(AFX_FACE_H__19673440_ADFF_11D9_938B_F0B94510EF6A__INCLUDED_)
