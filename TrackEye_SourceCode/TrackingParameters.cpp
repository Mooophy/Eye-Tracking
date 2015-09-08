// TrackingParameters.cpp: implementation of the TrackingParameters class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrackEye.h"
#include "TrackingParameters.h"
#include "TrackingSettings.h"
#include "cv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TrackingParameters::TrackingParameters()
{
	trackEyes = 0;
	
	varianceCheck == 1;
	variance = 30;

	nImages = 9; nEigens = 7; 
	
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);
	Path += "EyeDatabase";

	DBdirectory.Format("%s", Path); 
	MaxError = 800; ratio = 0.32; colorSpace = CV_RGB2XYZ; component = 3;
	detailtrack = 0; pupil = 0; snake = 0; fitellipse = 0; blink = 0;

	pupilCalib = new Calibration4Pupil;
	snakeCalib = new Calibration4Snake;
	
	//	**	Initialize results also
	eye1.eyefound = false;	eye1.coords.x = 0;	eye1.coords.y = 0;
	eye2.eyefound = false;	eye2.coords.x = 0;	eye2.coords.y = 0;

	tempMatch = CV_TM_CCOEFF_NORMED;
	
}


TrackingParameters::~TrackingParameters()
{

}
