#if !defined(AFX_TRACKINGSETTINGS_H__82272CA1_A6E4_11D9_938B_F075F93C6E68__INCLUDED_)
#define AFX_TRACKINGSETTINGS_H__82272CA1_A6E4_11D9_938B_F075F93C6E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackingSettings.h : header file
//

#include "cv.h"
#include "highgui.h"
#include "TrackingParameters.h"
#include "ColorSelector.h"
#include "IconButton.h"


/////////////////////////////////////////////////////////////////////////////
// TrackingSettings dialog

class TrackingSettings : public CDialog
{
// Construction
public:
	bool ChooseFolder(char* folderName);
	ColorSelector programColors;
	CvPoint point2;
	CvPoint point1;
	CvCapture* Camera;
	IplImage* CameraFrame;
	int cameraControl;
	TrackingParameters* params;
	TrackingSettings(CWnd* pParent = NULL);   // standard constructor
	CString videoFile;

// Dialog Data
	//{{AFX_DATA(TrackingSettings)
	enum { IDD = IDD_TRACKINGSETTINGS_DIALOG };
	CIconButton	m_saveAndClose;
	CIconButton	m_SnakeSettings;
	CIconButton	m_ChooseDirectory;
	CIconButton	m_ThresholdSettings;
	CComboBox	m_Colorspace;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	CButton	m_Check1;
	CString	m_DBdirectory;
	CString recordFolder;
	int		m_NeigenEyes;
	int		m_NImages;
	int		m_HUEw;
	int		m_HUEh;
	int		m_MaxError;
	float	m_Ratio;
	int		m_Component;
	int		m_variance;
	int		m_eyeAlgorithm;
	int		m_faceAlgorithm;
	int		m_source;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TrackingSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TrackingSettings)
	afx_msg void OnCheck1();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck2();
	afx_msg void OnSaveAndClose();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefineHUE();
	afx_msg void OnUpdateRectangle();
	afx_msg void OnClose();
	afx_msg void OnSaveHUE();
	afx_msg void OnThresholdSettings();
	afx_msg void OnChoosedirectory();
	afx_msg void OnSnakeSettings();
	afx_msg void OnVarianceCheck();
	afx_msg void OnSelectfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKINGSETTINGS_H__82272CA1_A6E4_11D9_938B_F075F93C6E68__INCLUDED_)



