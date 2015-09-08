// TrajectoryExtractor2D.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "TrajectoryExtractor2D.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CvPoint point1; CvPoint point2;
IplImage* AVIFrame;
IplImage* trajectoryImg;

void on_mouseClick(int event, int x, int y, int flags)
{
	CString mes;
 	switch(event)
    {
 		case CV_EVENT_LBUTTONDOWN:
			point1.x = x;
			point1.y = AVIFrame->height - y;
			break;
 		
		case CV_EVENT_LBUTTONUP:
			point2.x = x;
			point2.y = AVIFrame->height - y;

			cvRectangle(AVIFrame, point1, point2, CV_RGB(255,0,0), 2);
			cvShowImage("AVIwindow", AVIFrame);
			break;
	}
}


void getHUEcomponent(IplImage* source, IplImage* destination)
{
	IplImage* HSV = cvCreateImage(cvGetSize(source), IPL_DEPTH_8U, 3);
	
	// convert HSV color space
	cvCvtColor(source, HSV, CV_RGB2HSV);
	cvCvtPixToPlane(HSV, destination, NULL, NULL, NULL);
	
	cvReleaseImage(&HSV);
}


void drawObjectBox(IplImage* img, CvBox2D objectBox)
{
	// top-left corner
	CvPoint pt1; 
	pt1.x = (int)(objectBox.center.x - (objectBox.size.width/2.0)); 
	pt1.y = (int)(objectBox.center.y - (objectBox.size.height/2.0));
	
	// bottom-right corner
	CvPoint pt2; 
	pt2.x = (int)(objectBox.center.x + (objectBox.size.width/2.0)); 
	pt2.y = (int)(objectBox.center.y + (objectBox.size.height/2.0));

	cvRectangle( img, pt1, pt2, CV_RGB(0,0,255), 2 );
}

/////////////////////////////////////////////////////////////////////////////
// TrajectoryExtractor2D dialog


TrajectoryExtractor2D::TrajectoryExtractor2D(CWnd* pParent /*=NULL*/)
	: CDialog(TrajectoryExtractor2D::IDD, pParent)
{
	//{{AFX_DATA_INIT(TrajectoryExtractor2D)
		// NOTE: the ClassWizard will add member initialization here
	fileAlreadySelected = false;
	AVItrackControl = 0;
	//}}AFX_DATA_INIT
}


void TrajectoryExtractor2D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrajectoryExtractor2D)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrajectoryExtractor2D, CDialog)
	//{{AFX_MSG_MAP(TrajectoryExtractor2D)
	ON_COMMAND(ID_SETTINGS_SELECTFILE, OnSettingsSelectfile)
	ON_BN_CLICKED(IDC_NextFrame, OnNextFrame)
	ON_BN_CLICKED(IDC_FirstFrame, OnFirstFrame)
	ON_BN_CLICKED(IDC_SaveObjectColor, OnSaveObjectColor)
	ON_BN_CLICKED(IDC_TrackObject, OnTrackObject)
	ON_BN_CLICKED(IDEXITRAJECTORY2D, OnExitrajectory2d)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrajectoryExtractor2D message handlers


BOOL TrajectoryExtractor2D::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	cvNamedWindow("TrajectoryWindow", 0);
	HWND hWnd = (HWND) cvGetWindowHandle("TrajectoryWindow");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_AVITRAJECTORY)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void TrajectoryExtractor2D::OnSettingsSelectfile() 
{
	if (fileAlreadySelected == true)
	{
		cvReleaseCapture(&AVICapture);
		cvDestroyWindow("AVIwindow");
	}
	
	CFileDialog dlg(TRUE, _T("*.avi"), NULL, 
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
		"AVI files (*.avi)|*.avi||", // Bunu anlayamadým
		NULL);
	dlg.m_ofn.lpstrTitle = "Open Avi file";
	dlg.m_ofn.lpstrInitialDir = "C:\\";
	dlg.DoModal();

	AVIfilename = dlg.GetFileName();

	AVICapture = cvCaptureFromAVI(AVIfilename);
	CButton* temp;
	if (AVICapture != NULL)
	{
		//	**	Create AVIFrame window
		cvNamedWindow("AVIwindow", 0);
		HWND hWnd = (HWND) cvGetWindowHandle("AVIwindow");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_AVI)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);

		cvSetMouseCallback( "AVIwindow", on_mouseClick);

		temp = (CButton*) GetDlgItem(IDC_NextFrame); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_FirstFrame); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_SaveObjectColor); temp->EnableWindow(true);
		
		fileAlreadySelected = true;

		AVIFrame = cvQueryFrame(AVICapture); cvWaitKey(10);
		cvShowImage("AVIwindow", AVIFrame);				
	}
	else 
	{
		MessageBox("cvCaptureFromFile can not initialize.\nInvalid file format", "Error", MB_OK|MB_ICONSTOP);
		fileAlreadySelected = false;
		
		temp = (CButton*) GetDlgItem(IDC_NextFrame); temp->EnableWindow(false);
		temp = (CButton*) GetDlgItem(IDC_FirstFrame); temp->EnableWindow(false);
		temp = (CButton*) GetDlgItem(IDC_SaveObjectColor); temp->EnableWindow(false);
	}
	temp = (CButton*) GetDlgItem(IDC_TrackObject); temp->EnableWindow(false);
}


void TrajectoryExtractor2D::OnNextFrame() 
{
	AVIFrame = cvQueryFrame(AVICapture); cvWaitKey(10);
	if (AVIFrame != NULL)
		cvShowImage("AVIwindow", AVIFrame);	
	else
	{
		MessageBox("End of AVI file is reached", "WARNING", MB_OK|MB_ICONINFORMATION);
		OnFirstFrame();
	}
	cvShowImage("AVIwindow", AVIFrame);	
}

void TrajectoryExtractor2D::OnFirstFrame() 
{
	cvReleaseCapture(&AVICapture);
	AVICapture = cvCaptureFromAVI(AVIfilename);

	AVIFrame = cvQueryFrame(AVICapture); cvWaitKey(10);
	cvShowImage("AVIwindow", AVIFrame);	
}

void TrajectoryExtractor2D::OnSaveObjectColor() 
{
	cvSetImageROI(AVIFrame, cvRect(point1.x, point2.y, abs(point2.x-point1.x), abs(point1.y-point2.y)));
	cvSaveImage("c:\\Trajectoryfile.jpg", AVIFrame);
	cvResetImageROI(AVIFrame);

	CButton* temp = (CButton*) GetDlgItem(IDC_TrackObject);
	temp->EnableWindow(true);
}

void TrajectoryExtractor2D::OnTrackObject() 
{
	CButton* temp = (CButton*) GetDlgItem(IDC_TrackObject);
	temp->EnableWindow(false);

	CEdit* StatusConsole = (CEdit*)GetDlgItem(IDC_STATUSCONSOLE);
	StatusConsole->SetSel(-1, false);
	
	CString status; 
		
	cvDestroyWindow("AVIwindow");
	
	cvNamedWindow("TrackingWindow", 0);
	HWND hWnd = (HWND) cvGetWindowHandle("TrackingWindow");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_AVI)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	IplImage* objectColor = cvLoadImage("c:\\Trajectoryfile.jpg", -1);
	IplImage* objectColorHUE = cvCreateImage(cvGetSize(objectColor), IPL_DEPTH_8U, 1);
	
	getHUEcomponent(objectColor, objectColorHUE);
	cvReleaseImage(&objectColor);
	//AfxMessageBox("getHUEcomponent OK");
	
	int hist_size	  = 30;
	float ranges[]	  = { 0, 180 };
	float* pranges[]  = {ranges};
	CvHistogram* hist = cvCreateHist( 1, &hist_size, CV_HIST_ARRAY, pranges, 1 );
	cvCalcHist(&objectColorHUE, hist);	//	**	Calculate histogram of HUE part

	AVICapture = cvCaptureFromAVI(AVIfilename);
	AVIFrame = cvQueryFrame(AVICapture); cvWaitKey(10);
	trajectoryImg = cvCreateImage(cvGetSize(AVIFrame), IPL_DEPTH_8U, 1);
	cvZero(trajectoryImg);
	
	IplImage* hueFrame = cvCreateImage(cvGetSize(AVIFrame), IPL_DEPTH_8U, 1);
	IplImage* backProject = cvCreateImage(cvGetSize(AVIFrame), IPL_DEPTH_8U, 1);
	getHUEcomponent(AVIFrame, hueFrame); 

	CvRect searchWin = { 0, 0, hueFrame->width, hueFrame->height }; // whole image
	CvConnectedComp comp;
	CvBox2D objectBox;
	
	AVItrackControl = 1;
	while (AVItrackControl != 0 && AVIFrame != NULL)
	{
		cvCalcBackProject( &hueFrame, backProject, hist ); // Probability is formed
		cvCamShift( backProject, searchWin,	
					cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), 
					&comp, &objectBox );
		searchWin = comp.rect;
		
		drawObjectBox(AVIFrame, objectBox);
		cvShowImage("TrackingWindow", AVIFrame);

		//	**	Draw trajectory
		drawTrajectoryImg(&objectBox);
		
		//	**	Inform user
		status.Empty(); 
		status.Format("Object detected at X=%.1f Y=%.1f\r\n", objectBox.center.x, objectBox.center.y);
		StatusConsole->ReplaceSel((LPCTSTR)status);
		
		AVIFrame = cvQueryFrame(AVICapture); cvWaitKey(250);
		if (AVIFrame != NULL)
			getHUEcomponent(AVIFrame, hueFrame);
	}

	status.Empty();
	status.Format("End of AVI file is reached\r\n");
	StatusConsole->ReplaceSel((LPCTSTR)status);
	
	temp->EnableWindow(true);
	
	cvDestroyWindow("TrackingWindow");
}

void TrajectoryExtractor2D::OnExitrajectory2d() 
{
	AVItrackControl = 0;
	
	cvDestroyWindow("TrajectoryWindow");
	
	cvReleaseImage(&trajectoryImg);
	cvReleaseCapture(&AVICapture);
	OnOK();	
}

void TrajectoryExtractor2D::drawTrajectoryImg(CvBox2D* objectBox)
{
	CvPoint pt;
	pt.x = (int)objectBox->center.x;
	pt.y = (int)objectBox->center.y;
	cvCircle(trajectoryImg, pt, 1, CV_RGB(255,255,255), 2);
	trajectoryImg->origin = 1;

	cvShowImage("TrajectoryWindow", trajectoryImg);
}
