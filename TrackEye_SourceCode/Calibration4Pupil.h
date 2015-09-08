#if !defined(AFX_CALIBRATION4PUPIL_H__58B64560_C866_11D9_938B_88596D6BE953__INCLUDED_)
#define AFX_CALIBRATION4PUPIL_H__58B64560_C866_11D9_938B_88596D6BE953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calibration4Pupil.h : header file
//
#include "cv.h"
#include "highgui.h"
#include "IconButton.h"


/////////////////////////////////////////////////////////////////////////////
// Calibration4Pupil dialog

class Calibration4Pupil : public CDialog
{
// Construction
public:
	void DisplaySummary(void);
	int colorSpace;
	int circleAlgorithm;

	IplImage* original_img;
	IplImage* colorSpace_img;
	IplImage* thresholded_img;
	IplImage* edges_img;
	IplImage* final_img;
	Calibration4Pupil(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Calibration4Pupil)
	enum { IDD = IDD_CALIBRATION4PUPIL };
	CIconButton	m_closePupilCalib;
	CIconButton	m_SaveSettings;
	CComboBox	m_circleAlgorithm;
	CComboBox	m_thresholdType;
	int thresholdingType;
	int	m_thresholdComponent;
	BOOL	m_binaryThreshold;
	int		m_thresholdValue;
	int		m_cannyApertureSize;
	double	m_cannyLower;
	double	m_cannyUpper;
	CString	m_sampleFilename;
	CString	m_sumcolorspace;
	CString	m_sumcomponent;
	CString	m_sumcanny;
	CString	m_sumbinthreshold;
	CString	m_sumbinthresholdvalue;
	CString	m_circleAlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Calibration4Pupil)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Calibration4Pupil)
	afx_msg void OnSavePupilCalib();
	virtual BOOL OnInitDialog();
	afx_msg void OnClosePupilCalib();
	afx_msg void OnCheck2();
	afx_msg void OnTest();
	afx_msg void OnDeletesampleEye();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATION4PUPIL_H__58B64560_C866_11D9_938B_88596D6BE953__INCLUDED_)
