#if !defined(AFX_CALIBRATION4SNAKE_H__76F4F780_D935_11D9_938B_B44E53A41F54__INCLUDED_)
#define AFX_CALIBRATION4SNAKE_H__76F4F780_D935_11D9_938B_B44E53A41F54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Calibration4Snake.h : header file
//
#include "cv.h"
#include "highgui.h"
#include "IconButton.h"

/////////////////////////////////////////////////////////////////////////////
// Calibration4Snake dialog

class Calibration4Snake : public CDialog
{
// Construction
public:
	int colorSpace;
	bool thresholdOrNot;
	int thresholdType;

	IplImage* finalImg;
	IplImage* edgesImg;
	IplImage* thresholdedImg;
	IplImage* colorSpaceImg;
	IplImage* originalImg;
	Calibration4Snake(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Calibration4Snake)
	enum { IDD = IDD_CALIBRATION4SNAKE };
	CIconButton	m_delete;
	CIconButton	m_SaveSettings;
	CComboBox	m_thresholdType;
	CComboBox	m_ColorSpace;
	int		m_component;
	int		m_thresholdValue;
	float	m_gamma;
	int		m_cannyApertureSize;
	int		m_cannyLower;
	int		m_cannyUpper;
	int		m_blockSize;
	float	m_beta;
	float	m_alpha;
	int		m_iterations;
	int		m_nPoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Calibration4Snake)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Calibration4Snake)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCheck2();
	afx_msg void OnSaveSnakeCalib();
	afx_msg void OnTestsettings();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATION4SNAKE_H__76F4F780_D935_11D9_938B_B44E53A41F54__INCLUDED_)
