#if !defined(AFX_EYEDATABASEEXPLORER_H__A7D7DF40_D21C_11D9_938B_B66D85B6F056__INCLUDED_)
#define AFX_EYEDATABASEEXPLORER_H__A7D7DF40_D21C_11D9_938B_B66D85B6F056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EyeDatabaseExplorer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EyeDatabaseExplorer dialog

class EyeDatabaseExplorer : public CDialog
{
// Construction
public:
	CString pathName;
	EyeDatabaseExplorer(CWnd* pParent = NULL, CString path = "C:\\Eyedatabase\\"); // standard constructor
	
// Dialog Data
	//{{AFX_DATA(EyeDatabaseExplorer)
	enum { IDD = IDD_EYEDATABASE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EyeDatabaseExplorer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EyeDatabaseExplorer)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseeyedatabase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EYEDATABASEEXPLORER_H__A7D7DF40_D21C_11D9_938B_B66D85B6F056__INCLUDED_)
