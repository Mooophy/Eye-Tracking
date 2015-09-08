#if !defined(AFX_USBCAM_H__58B07F01_A867_11D9_938B_D7604871106E__INCLUDED_)
#define AFX_USBCAM_H__58B07F01_A867_11D9_938B_D7604871106E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USBCam.h : header file
//

#include "cv.h"
#include "highgui.h"
#include "cvcam.h"

/////////////////////////////////////////////////////////////////////////////
// USBCam dialog

class USBCam : public CDialog
{
// Construction
public:
	int numOfCameras;
	USBCam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(USBCam)
	enum { IDD = IDD_USBCAM_DIALOG };
	CString	m_cameraName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(USBCam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(USBCam)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdjust();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBCAM_H__58B07F01_A867_11D9_938B_D7604871106E__INCLUDED_)
