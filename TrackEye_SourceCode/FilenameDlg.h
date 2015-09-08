#if !defined(AFX_FILENAMEDLG_H__E46D2720_75FB_45AE_A1FB_2E3DC432BFB2__INCLUDED_)
#define AFX_FILENAMEDLG_H__E46D2720_75FB_45AE_A1FB_2E3DC432BFB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilenameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FilenameDlg dialog

class FilenameDlg : public CDialog
{
// Construction
public:
	FilenameDlg(CWnd* pParent = NULL, CString* file = NULL);   // standard constructor
	CString* tempPtr;

// Dialog Data
	//{{AFX_DATA(FilenameDlg)
	enum { IDD = IDD_FILENAMEDLG };
	CString	m_filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FilenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FilenameDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMEDLG_H__E46D2720_75FB_45AE_A1FB_2E3DC432BFB2__INCLUDED_)
