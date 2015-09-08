// TrackEyeDlg.h : header file
//

#if !defined(AFX_TRACKEYEDLG_H__2BC33066_A217_11D9_938B_C61810E7E065__INCLUDED_)
#define AFX_TRACKEYEDLG_H__2BC33066_A217_11D9_938B_C61810E7E065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrackingParameters.h"
#include "TrackingSettings.h"	// Added by ClassView
#include "IconButton.h"
#include "FilenameDlg.h"
//
#ifdef _CH_
#define WIN32
#error "The file needs cvaux, which is not wrapped yet. Sorry"
#endif

#ifndef _EiC
#include "cvaux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#endif

#ifdef _EiC
#define WIN32
#endif

#define ORIG_WIN_SIZE  24

#include <time.h>
//

/////////////////////////////////////////////////////////////////////////////
// CTrackEyeDlg dialog

class CTrackEyeDlg : public CDialog
{
// Construction
public:
	bool ChooseDirectory(char*);
	CString recordFileName;
	bool record;
	
	float FPSTimer;
	float FPS;
	CToolTipCtrl tips;
	IplImage* headTrajectory;
	IplImage* resizedheadTrajectory;
	CvBox2D trajectoryBuffer[3];
	
	void initializeGUI(void);
	void drawHeadTrajectory(IplImage*, CvBox2D*);
	void DisplayEyeStatus(TrackingSettings*);
	void displayBlinkStatus();
	
	// Haar Face Detection
	CvMemStorage* storage;
	CvHidHaarClassifierCascade* hid_cascade;
	void HaarFaceDetect(IplImage*, CvBox2D*);
	int NumOfHaarFaces;
			
	IplImage* hueFrame;
	IplImage* backProject;
	int trackControl;
	IplImage* CameraFrame;
	CvCapture* Camera;
	TrackingSettings* TrackSettings;
	CTrackEyeDlg(CWnd* pParent = NULL);	// standard constructor

	int delay;

	clock_t start;
	clock_t finish;


// Dialog Data
	//{{AFX_DATA(CTrackEyeDlg)
	enum { IDD = IDD_TRACKEYE_DIALOG };
	CIconButton	m_write2AVI;
	CIconButton	m_StopRecord;
	CIconButton	m_Record;
	CEdit	m_blinkStatus;
	CIconButton	m_UpdateEyeDatabase;
	CIconButton	m_ExtractTrajectory;
	CIconButton	m_StopTracking;
	CIconButton	m_StartTracking;
	CIconButton	m_TrackerSettings;
	CIconButton	m_ExitProgram;
	CProgressCtrl	m_progress;
	CSliderCtrl	m_sliderYplane;
	CSliderCtrl	m_sliderXplane;
	CButton	m_headTrajectory;
	CSliderCtrl	m_sliderRight;
	CSliderCtrl	m_sliderLeft;
	CString	m_leyedist;
	CString	m_reyedist;
	CString	m_hWnd;
	int		m_LPR;
	int		m_LPX;
	int		m_LPY;
	int		m_RPR;
	int		m_RPX;
	int		m_RPY;
	int		m_LEX;
	int		m_LEY;
	int		m_REX;
	int		m_REY;
	int		m_counter;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackEyeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTrackEyeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHelpAbouttrackeye();
	afx_msg void OnTrackeyemenuPcasettings();
	afx_msg void OnTrackeyemenuFacetrackersettings();
	afx_msg void OnTrackeyemenuAdjustcapturedevicesettings();
	afx_msg void OnStartTracking();
	afx_msg void OnTrackeyemenuExit();
	afx_msg void OnStopTracking();
	afx_msg void OnExitprogram();
	afx_msg void OnEyeTrackerSettings();
	afx_msg void OnClearHT();
	afx_msg void OnExtractTrajectory();
	afx_msg void OnUpdateEyeDatabase();
	afx_msg void OnDatabaseExploredatabase();
	afx_msg void OnTrackeyemenuStarttracking();
	afx_msg void OnTrackeyemenuStoptracking();
	afx_msg void OnTrackeyemenuSelectprogramcolors();
	afx_msg void OnHelpShowtrackeyehelpfile();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDatabaseCleardatabase();
	afx_msg void OnRecord();
	afx_msg void OnStoprecord();
	afx_msg void OnWrite2avi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKEYEDLG_H__2BC33066_A217_11D9_938B_C61810E7E065__INCLUDED_)
