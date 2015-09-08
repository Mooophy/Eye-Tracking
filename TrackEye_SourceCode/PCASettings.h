#if !defined(AFX_PCASETTINGS_H__2BC3306E_A217_11D9_938B_C61810E7E065__INCLUDED_)
#define AFX_PCASETTINGS_H__2BC3306E_A217_11D9_938B_C61810E7E065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PCASettings.h : header file
//

#include "cv.h"
#include "highgui.h"
#include "IconButton.h"

/////////////////////////////////////////////////////////////////////////////
// PCASettings dialog

class PCASettings : public CDialog
{
// Construction
public:
	int DBaseType;
	bool ChooseFolder(char*);
	IplImage* tempFrame;
	CvPoint point2;
	CvPoint point1;
	int control;
	IplImage* cameraFrame;
	CvCapture* capture;
	int fileCounter;
	PCASettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PCASettings)
	enum { IDD = IDD_PCADIALOG };
	CIconButton	m_done;
	CIconButton	m_UpdateRectangle;
	CIconButton	m_Save2DB;
	CIconButton	m_CD2;
	CIconButton	m_CD1;
	CString	m_DBdirectory;
	int		m_rectangleHeight;
	int		m_rectangleWidth;
	CString	m_DBdirectory2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PCASettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PCASettings)
	afx_msg void OnSave2DB();
	afx_msg void OnUpdateRectangle();
	virtual BOOL OnInitDialog();
	afx_msg void OnDatabaseoperationsUpdateblinkdatabase();
	afx_msg void OnDatabaseoperationsUploadeyedatabase();
	afx_msg void OnExitdb();
	afx_msg void OnChoosedirectory1();
	afx_msg void OnChoosedirectory2();
	afx_msg void OnDone();
	afx_msg void OnDatabaseoperationsExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCASETTINGS_H__2BC3306E_A217_11D9_938B_C61810E7E065__INCLUDED_)
