// EyeDatabaseExplorer.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "EyeDatabaseExplorer.h"
#include "cv.h"
#include "highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EyeDatabaseExplorer dialog


EyeDatabaseExplorer::EyeDatabaseExplorer(CWnd* pParent /*=NULL*/, CString path)
	: CDialog(EyeDatabaseExplorer::IDD, pParent)
{
	pathName = path;
	//{{AFX_DATA_INIT(EyeDatabaseExplorer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void EyeDatabaseExplorer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EyeDatabaseExplorer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EyeDatabaseExplorer, CDialog)
	//{{AFX_MSG_MAP(EyeDatabaseExplorer)
	ON_BN_CLICKED(IDC_CLOSEEYEDATABASE, OnCloseeyedatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EyeDatabaseExplorer message handlers

BOOL EyeDatabaseExplorer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_Directory)->SetWindowText(pathName);
	HWND hWnd;
	HWND hParent;
	IplImage* img = NULL;
	IplImage* reSizedImg = cvCreateImage(cvSize(113, 58), IPL_DEPTH_8U, 3);
	CString filename;

	int windowID = IDC_EYE1;
	for (int k=0; k<20; k++)
	{
		filename.Format("%s\\Eye%d.jpg", pathName, k);
		img = cvLoadImage(filename, -1);
		
		if ( img != NULL )
		{
			filename.Format("Eye%d", k);
			
			cvNamedWindow(filename, CV_WINDOW_AUTOSIZE);
			hWnd = (HWND) cvGetWindowHandle(filename);
			hParent = ::GetParent(hWnd);
			::SetParent(hWnd, GetDlgItem(windowID+k)->m_hWnd);
			::ShowWindow(hParent, SW_HIDE);

			cvResize(img, reSizedImg);
			cvShowImage(filename, reSizedImg);
		}
	}

	cvReleaseImage(&img);
	cvReleaseImage(&reSizedImg);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EyeDatabaseExplorer::OnCloseeyedatabase() 
{
	CString filename;
	
	for (int k=0; k<20; k++)
	{
		filename.Format("Eye%d", k);
		cvDestroyWindow(filename);
	}
	
	EndDialog(1);	
}
