#if !defined(AFX_COLORSELECTOR_H__9528D5E1_D2E7_11D9_938B_8D1642533C56__INCLUDED_)
#define AFX_COLORSELECTOR_H__9528D5E1_D2E7_11D9_938B_8D1642533C56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSelector.h : header file
//
#include "cv.h"

/////////////////////////////////////////////////////////////////////////////
// ColorSelector dialog

class ColorSelector : public CDialog
{
// Construction
public:
	int myRGB[3];
	double colors[6];
	int colorCodes[6][3];
	void SelectColor(int*);
	void initializeStatics(void);
	ColorSelector(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ColorSelector)
	enum { IDD = IDD_PROGRAM_COLORS };
	BOOL	m_drawFaceBox;
	BOOL	m_drawEye;
	BOOL	m_drawEyeCircle;
	BOOL	m_drawPupil;
	CString	m_RGB1;
	CString	m_RGB2;
	CString	m_RGB3;
	CString	m_RGB4;
	BOOL	m_drawSnake;
	BOOL	m_drawEllipse;
	CString	m_RGB5;
	CString	m_RGB6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColorSelector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ColorSelector)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnCloseColors();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSELECTOR_H__9528D5E1_D2E7_11D9_938B_8D1642533C56__INCLUDED_)
