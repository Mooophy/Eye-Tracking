// TrackingParameters.h: interface for the TrackingParameters class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKINGPARAMETERS_H__FA2F88A2_A7A2_11D9_938B_E2B11978486E__INCLUDED_)
#define AFX_TRACKINGPARAMETERS_H__FA2F88A2_A7A2_11D9_938B_E2B11978486E__INCLUDED_

#if _MSC_VER > 1000
//	**	This class is kinda summary for Eye Tracking Settings
//	**	TrackingSettings class does all the job and just before Save&close is checked
//	**	Summary for the settings are assigned to the params object

#pragma once
#endif // _MSC_VER > 1000

#include "Eye.h"
#include "Calibration4Pupil.h"
#include "Calibration4Snake.h"

class TrackingParameters  
{
public:
	TrackingParameters();
	
	int trackEyes;
	
	int varianceCheck;
	int variance;

	int nImages;
	int nEigens;
	CString DBdirectory;
	int MaxError;
	float ratio;
	int colorSpace;
	int component;
	
	int detailtrack;
	int pupil;
	int snake;
	int fitellipse;
	int blink;

	//	**	These variables are for thresholding for pupil & Snake
	Calibration4Pupil* pupilCalib;
	Calibration4Snake* snakeCalib;

	//	**	These variables are for results
	Eye eye1;
	Eye eye2;

	//	**	Template Matching Method
	int tempMatch;
	
	virtual ~TrackingParameters();
};

#endif // !defined(AFX_TRACKINGPARAMETERS_H__FA2F88A2_A7A2_11D9_938B_E2B11978486E__INCLUDED_)
