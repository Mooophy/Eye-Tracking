// Eye.cpp: implementation of the Eye class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrackEye.h"
#include "Eye.h"
#include "hough.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Eye::Eye()
{

}

Eye::~Eye()
{

}

void Eye::initializeEyeParameters()
{
	coords.x = 0; coords.y = 0;
	distance = 0.0;
	eyefound = false;
}


void Eye::detectPupils(IplImage* eyeImage, int circleAlgorithm)
{
	int coords[3];
	for (int i=0; i<=2; i++)
		coords[i] = 0;

	//	Find circle
	if (circleAlgorithm == 1)
		findCircles(eyeImage, coords);
	else if (circleAlgorithm == 0)
		Hough(eyeImage, coords);
	
	if (coords[2] > 0)
	{
		eyePupil.x = coords[0]; 
		eyePupil.y = coords[1]; 
		eyePupil.radius = coords[2];
	}
}
