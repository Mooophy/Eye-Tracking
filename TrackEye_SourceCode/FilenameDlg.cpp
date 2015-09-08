// FilenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "FilenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FilenameDlg dialog


FilenameDlg::FilenameDlg(CWnd* pParent /*=NULL*/, CString* file /*NULL*/)
	: CDialog(FilenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FilenameDlg)
	m_filename = _T("sample1");
	//}}AFX_DATA_INIT

	tempPtr = file;
}


void FilenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FilenameDlg)
	DDX_Text(pDX, IDC_EDIT1, m_filename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FilenameDlg, CDialog)
	//{{AFX_MSG_MAP(FilenameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FilenameDlg message handlers

void FilenameDlg::OnOK() 
{
	
	UpdateData(true);
	*tempPtr = this->m_filename + ".avi";

	CDialog::OnOK();
}
