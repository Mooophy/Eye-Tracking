// Eye.h: interface for the Eye class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EYE_H__35CA77A0_B994_11D9_938B_93AA212C446A__INCLUDED_)
#define AFX_EYE_H__35CA77A0_B994_11D9_938B_93AA212C446A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"

class pupil {
public:
	int x;
	int y;
	int radius;
};

class Eye  
{
public:
	
	void detectPupils(IplImage*, int);
	void initializeEyeParameters(void);
	Eye();
	
	//	**	Member Variables
	int xlimitLeft;
	int xlimitRight;
	int ylimitDown;
	int ylimitUp;

	double distance;
	bool eyefound;
	CvPoint coords;
	CvSize RectSize;
	pupil eyePupil;
	int blinkStatus;

	virtual ~Eye();

};

#endif // !defined(AFX_EYE_H__35CA77A0_B994_11D9_938B_93AA212C446A__INCLUDED_)
