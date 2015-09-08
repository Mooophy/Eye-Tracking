#if !defined(AFX_DEFINEEYE4CALIBRATION_H__1B08D4C2_C876_11D9_938B_F2237FB55552__INCLUDED_)
#define AFX_DEFINEEYE4CALIBRATION_H__1B08D4C2_C876_11D9_938B_F2237FB55552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineEye4Calibration.h : header file
//
#include "cv.h"
#include "highgui.h"
#include "IconButton.h"

/////////////////////////////////////////////////////////////////////////////
// DefineEye4Calibration dialog

class DefineEye4Calibration : public CDialog
{
// Construction
public:
	bool saveControl;
	int exitcode;
	DefineEye4Calibration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DefineEye4Calibration)
	enum { IDD = IDD_CameraFrame4Eye };
	CIconButton	m_exit;
	CString	m_saveStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DefineEye4Calibration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DefineEye4Calibration)
	afx_msg void OnStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEEYE4CALIBRATION_H__1B08D4C2_C876_11D9_938B_F2237FB55552__INCLUDED_)
