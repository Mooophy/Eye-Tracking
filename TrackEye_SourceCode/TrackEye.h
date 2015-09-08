// TrackEye.h : main header file for the TRACKEYE application
//

#if !defined(AFX_TRACKEYE_H__2BC33064_A217_11D9_938B_C61810E7E065__INCLUDED_)
#define AFX_TRACKEYE_H__2BC33064_A217_11D9_938B_C61810E7E065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrackEyeApp:
// See TrackEye.cpp for the implementation of this class
//

class CTrackEyeApp : public CWinApp
{
public:
	CTrackEyeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackEyeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTrackEyeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKEYE_H__2BC33064_A217_11D9_938B_C61810E7E065__INCLUDED_)
