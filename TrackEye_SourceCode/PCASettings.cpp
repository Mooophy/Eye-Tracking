// PCASettings.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "PCASettings.h"
#include "cv.h"
#include "highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PCASettings dialog


PCASettings::PCASettings(CWnd* pParent /*=NULL*/)
	: CDialog(PCASettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(PCASettings)
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);
	Path += "EyeDatabase";
	m_DBdirectory = Path;
	m_rectangleHeight = 25;
	m_rectangleWidth = 40;
	m_DBdirectory2 = _T("C:\\EyeDatabase\\Blinkdatabase");
	//}}AFX_DATA_INIT

	//	**	My initializations
	control = 0;
	fileCounter = 0;
	cameraFrame = NULL;
	tempFrame = NULL;
	DBaseType = 0;
}


void PCASettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PCASettings)
	DDX_Control(pDX, IDC_DONE, m_done);
	DDX_Control(pDX, IDUpdateRectangle, m_UpdateRectangle);
	DDX_Control(pDX, IDSave2DB, m_Save2DB);
	DDX_Control(pDX, IDC_CHOOSEDIRECTORY2, m_CD2);
	DDX_Control(pDX, IDC_CHOOSEDIRECTORY1, m_CD1);
	DDX_Text(pDX, IDC_DBdirectory, m_DBdirectory);
	DDV_MaxChars(pDX, m_DBdirectory, 80);
	DDX_Text(pDX, IDC_imgHeight2, m_rectangleHeight);
	DDV_MinMaxInt(pDX, m_rectangleHeight, 5, 1000);
	DDX_Text(pDX, IDC_imgWidth2, m_rectangleWidth);
	DDV_MinMaxInt(pDX, m_rectangleWidth, 10, 1000);
	DDX_Text(pDX, IDC_DBdirectory2, m_DBdirectory2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PCASettings, CDialog)
	//{{AFX_MSG_MAP(PCASettings)
	ON_BN_CLICKED(IDSave2DB, OnSave2DB)
	ON_BN_CLICKED(IDUpdateRectangle, OnUpdateRectangle)
	ON_COMMAND(ID_DATABASEOPERATIONS_UPDATEBLINKDATABASE, OnDatabaseoperationsUpdateblinkdatabase)
	ON_COMMAND(ID_DATABASEOPERATIONS_UPLOADEYEDATABASE, OnDatabaseoperationsUploadeyedatabase)
	ON_BN_CLICKED(IDEXITDB, OnExitdb)
	ON_BN_CLICKED(IDC_CHOOSEDIRECTORY1, OnChoosedirectory1)
	ON_BN_CLICKED(IDC_CHOOSEDIRECTORY2, OnChoosedirectory2)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_COMMAND(ID_DATABASEOPERATIONS_EXIT, OnDatabaseoperationsExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PCASettings message handlers

BOOL PCASettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CD1.SetIconID(IDI_EXPLORE);
	m_CD2.SetIconID(IDI_EXPLORE);
	m_Save2DB.SetIconID(IDI_SAVE);
	m_UpdateRectangle.SetIconID(IDI_UR);
	m_done.SetIconID(IDI_OK);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PCASettings::OnDatabaseoperationsUploadeyedatabase() 
{
	// TODO: Add your command handler code here
	//	**	Operator wants to update the Eye Database
	capture =  cvCaptureFromCAM(-1);
	if (capture == NULL)
	{
		AfxMessageBox("Can not initialize capture device", MB_OK|MB_ICONSTOP);
		exit(1);
	}
	else 
	{
		//	**	Enable&Disable stuff
		GetDlgItem(IDC_imgWT)->EnableWindow(true);
		GetDlgItem(IDC_IMGHT)->EnableWindow(true);
		GetDlgItem(IDC_imgWidth)->EnableWindow(true);
		GetDlgItem(IDC_imgHeight)->EnableWindow(true);
		GetDlgItem(IDC_RECWT)->EnableWindow(true);
		GetDlgItem(IDC_RECHT)->EnableWindow(true);
		GetDlgItem(IDC_imgWidth2)->EnableWindow(true);
		GetDlgItem(IDC_imgHeight2)->EnableWindow(true);

		GetDlgItem(IDUpdateRectangle)->EnableWindow(true);
		GetDlgItem(IDSave2DB)->EnableWindow(true);
		GetDlgItem(IDC_DONE)->EnableWindow(true);
		GetDlgItem(IDEXITDB)->EnableWindow(false);

		DBaseType = 0;
		control = 0;
		cameraFrame = cvQueryFrame(capture); cvWaitKey(10);
		tempFrame = cvCloneImage(cameraFrame);
			
		CString tempStr1, tempStr2;
		tempStr1.Format("%d", cameraFrame->width);
		tempStr2.Format("%d", cameraFrame->height);
		GetDlgItem(IDC_imgWidth)->SetWindowText(tempStr1);
		GetDlgItem(IDC_imgHeight)->SetWindowText(tempStr2);
		
		UpdateData(true);		//	**	Display window size
		OnUpdateRectangle();	//	**	First Update
		
		HWND hWnd;
		HWND hParent;

		cvNamedWindow("Database", CV_WINDOW_AUTOSIZE);
		hWnd = (HWND) cvGetWindowHandle("Database");
		hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_DBwindow)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);

		while (control == 0)
		{
			cvRectangle( tempFrame, point1, point2, CV_RGB(255,0,0), 2);
			cvShowImage("Database", tempFrame);
			cameraFrame = cvQueryFrame(capture); cvWaitKey(10);
			cvCopy(cameraFrame, tempFrame);
		}

		//	**	Operator clicked "DONE!"
		cvReleaseCapture(&capture);
		cvReleaseImage(&tempFrame);
		cvDestroyWindow("Database");
		
		//	**	Enable&Disasble Stuff again
		GetDlgItem(IDC_imgWT)->EnableWindow(false);
		GetDlgItem(IDC_IMGHT)->EnableWindow(false);
		GetDlgItem(IDC_imgWidth)->EnableWindow(false);
		GetDlgItem(IDC_imgHeight)->EnableWindow(false);
		GetDlgItem(IDC_RECWT)->EnableWindow(false);
		GetDlgItem(IDC_RECHT)->EnableWindow(false);
		GetDlgItem(IDC_imgWidth2)->EnableWindow(false);
		GetDlgItem(IDC_imgHeight2)->EnableWindow(false);

		GetDlgItem(IDUpdateRectangle)->EnableWindow(false);
		GetDlgItem(IDSave2DB)->EnableWindow(false);
		GetDlgItem(IDC_DONE)->EnableWindow(false);
		GetDlgItem(IDEXITDB)->EnableWindow(true);
	}
}

void PCASettings::OnDatabaseoperationsUpdateblinkdatabase() 
{
	// TODO: Add your command handler code here
	//	**	Operator wants to update the Eye Database
	capture =  cvCaptureFromCAM(-1);
	if (capture == NULL)
	{
		AfxMessageBox("Can not initialize capture device", MB_OK|MB_ICONSTOP);
		exit(1);
	}
	else 
	{
		//	**	Enable&Disable stuff
		GetDlgItem(IDC_imgWT)->EnableWindow(true);
		GetDlgItem(IDC_IMGHT)->EnableWindow(true);
		GetDlgItem(IDC_imgWidth)->EnableWindow(true);
		GetDlgItem(IDC_imgHeight)->EnableWindow(true);
		GetDlgItem(IDC_RECWT)->EnableWindow(true);
		GetDlgItem(IDC_RECHT)->EnableWindow(true);
		GetDlgItem(IDC_imgWidth2)->EnableWindow(true);
		GetDlgItem(IDC_imgHeight2)->EnableWindow(true);

		GetDlgItem(IDUpdateRectangle)->EnableWindow(true);
		GetDlgItem(IDSave2DB)->EnableWindow(true);
		GetDlgItem(IDC_DONE)->EnableWindow(true);
		GetDlgItem(IDEXITDB)->EnableWindow(false);

		DBaseType = 1;
		control = 0;
		cameraFrame = cvQueryFrame(capture); cvWaitKey(10);
		tempFrame = cvCloneImage(cameraFrame);
			
		CString tempStr1, tempStr2;
		tempStr1.Format("%d", cameraFrame->width);
		tempStr2.Format("%d", cameraFrame->height);
		GetDlgItem(IDC_imgWidth)->SetWindowText(tempStr1);
		GetDlgItem(IDC_imgHeight)->SetWindowText(tempStr2);
		
		UpdateData(true);		//	**	Display window size
		OnUpdateRectangle();	//	**	First Update
		
		HWND hWnd;
		HWND hParent;

		cvNamedWindow("Database", CV_WINDOW_AUTOSIZE);
		hWnd = (HWND) cvGetWindowHandle("Database");
		hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_DBwindow)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);

		while (control == 0)
		{
			cvRectangle( tempFrame, point1, point2, CV_RGB(255,0,0), 2);
			cvShowImage("Database", tempFrame);
			cameraFrame = cvQueryFrame(capture); cvWaitKey(10);
			cvCopy(cameraFrame, tempFrame);
		}

		//	**	Operator clicked "DONE!"
		cvReleaseCapture(&capture);
		cvReleaseImage(&tempFrame);
		cvDestroyWindow("Database");
		
		//	**	Enable&Disasble Stuff again
		GetDlgItem(IDC_imgWT)->EnableWindow(false);
		GetDlgItem(IDC_IMGHT)->EnableWindow(false);
		GetDlgItem(IDC_imgWidth)->EnableWindow(false);
		GetDlgItem(IDC_imgHeight)->EnableWindow(false);
		GetDlgItem(IDC_RECWT)->EnableWindow(false);
		GetDlgItem(IDC_RECHT)->EnableWindow(false);
		GetDlgItem(IDC_imgWidth2)->EnableWindow(false);
		GetDlgItem(IDC_imgHeight2)->EnableWindow(false);

		GetDlgItem(IDUpdateRectangle)->EnableWindow(false);
		GetDlgItem(IDSave2DB)->EnableWindow(false);
		GetDlgItem(IDC_DONE)->EnableWindow(false);
		GetDlgItem(IDEXITDB)->EnableWindow(true);
	}
}

void PCASettings::OnUpdateRectangle() 
{
	CvSize dimOfFrame = cvGetSize(cameraFrame);
	
	UpdateData(true);
	point1.x = dimOfFrame.width/2 - m_rectangleWidth/2;
	point1.y = dimOfFrame.height/2 - m_rectangleHeight/2;	
	point2.x = point1.x + m_rectangleWidth;
	point2.y = point1.y + m_rectangleHeight;
	
	CString consoleText = "";
	consoleText.Format("(X,Y) of Point1 = (%d,%d) & (X,Y) of Point2 = (%d,%d)\r\n",point1.x, point1.y, point2.x, point2.y);
	CEdit* console = (CEdit*)GetDlgItem(IDC_STATUS);
	console->SetSel(-1, false);
	console->ReplaceSel(LPCTSTR(consoleText));
}

void PCASettings::OnSave2DB() 
{
	CEdit* console = (CEdit*)GetDlgItem(IDC_STATUS);
	console->SetSel(-1, false);
		
	UpdateData(true);
	IplImage* ROI = cvCloneImage(cameraFrame);
	cvSetImageROI(ROI, cvRect(point1.x, point1.y, point2.x-point1.x, point2.y-point1.y));
	
	CString filename;
	if (DBaseType == 0)
		filename.Format("%s\\eye%d.jpg", m_DBdirectory, fileCounter); 
	else
		filename.Format("%s\\eye%d.jpg", m_DBdirectory2, fileCounter); 
	cvSaveImage(filename, ROI);

	//	**	Inform user
	filename.Empty(); filename.Format("File saved as %s\\eye%d.jpg\r\n", m_DBdirectory, fileCounter);
	console->ReplaceSel((LPCTSTR)filename);
	fileCounter++;

	cvResetImageROI(ROI);
	cvReleaseImage(&ROI);	
}

void PCASettings::OnDone() 
{
	control = 1;
	fileCounter = 0;
}

void PCASettings::OnExitdb() 
{
	EndDialog(1);
}

void PCASettings::OnDatabaseoperationsExit() 
{
	control = 1;
	EndDialog(1);
}

bool PCASettings::ChooseFolder(char* folderName)
{
	BROWSEINFO bi;
	ITEMIDLIST* pItemIDList;
	
	char folder[_MAX_PATH];
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = folder;
	bi.lpszTitle = "Choose Database Folder";
	
	if((pItemIDList = SHBrowseForFolder(&bi))!=NULL) 
	{
		SHGetPathFromIDList(pItemIDList, folderName);
		return true;
	}
	else
		return false;

}

void PCASettings::OnChoosedirectory1() 
{
	char folder[_MAX_PATH]	;
	if (ChooseFolder(folder) == true)
	{
		m_DBdirectory.Format("%s", folder);
		UpdateData(false);
	}
	
	CEdit* console = (CEdit*)GetDlgItem(IDC_STATUS);
	console->SetSel(-1, false);
	console->ReplaceSel("EyeDatabase directory change.....OK\r\n");
}

void PCASettings::OnChoosedirectory2() 
{
	char folder[_MAX_PATH]	;
	if (ChooseFolder(folder) == true)
	{
		m_DBdirectory2.Format("%s", folder);
		UpdateData(false);
	}
	CEdit* console = (CEdit*)GetDlgItem(IDC_STATUS);
	console->SetSel(-1, false);
	console->ReplaceSel("BlinkDatabase directory change.....OK\r\n");
}

