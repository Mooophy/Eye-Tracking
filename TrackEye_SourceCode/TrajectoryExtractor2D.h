#if !defined(AFX_TRAJECTORYEXTRACTOR2D_H__5B989961_CBCA_11D9_938B_CC13525DB855__INCLUDED_)
#define AFX_TRAJECTORYEXTRACTOR2D_H__5B989961_CBCA_11D9_938B_CC13525DB855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrajectoryExtractor2D.h : header file
//
#include "cv.h"
#include "highgui.h"

/////////////////////////////////////////////////////////////////////////////
// TrajectoryExtractor2D dialog

class TrajectoryExtractor2D : public CDialog
{
// Construction
public:
	void drawTrajectoryImg(CvBox2D*);
	int AVItrackControl;
	bool fileAlreadySelected;
	CString AVIfilename;
	
	CvCapture* AVICapture;
	TrajectoryExtractor2D(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TrajectoryExtractor2D)
	enum { IDD = IDD_2DTRAJECTORYEXTRACTOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrajectoryExtractor2D)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrajectoryExtractor2D)
	afx_msg void OnSettingsSelectfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnNextFrame();
	afx_msg void OnFirstFrame();
	afx_msg void OnSaveObjectColor();
	afx_msg void OnTrackObject();
	afx_msg void OnExitrajectory2d();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAJECTORYEXTRACTOR2D_H__5B989961_CBCA_11D9_938B_CC13525DB855__INCLUDED_)
