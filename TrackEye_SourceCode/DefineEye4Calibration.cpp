// DefineEye4Calibration.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "DefineEye4Calibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DefineEye4Calibration dialog


DefineEye4Calibration::DefineEye4Calibration(CWnd* pParent /*=NULL*/)
	: CDialog(DefineEye4Calibration::IDD, pParent)
{
	//{{AFX_DATA_INIT(DefineEye4Calibration)
	saveControl = false;
	exitcode = 0;
	m_saveStatus = _T("");
	//}}AFX_DATA_INIT
}


void DefineEye4Calibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DefineEye4Calibration)
	DDX_Control(pDX, IDEXIT, m_exit);
	DDX_Text(pDX, IDC_STATUS, m_saveStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DefineEye4Calibration, CDialog)
	//{{AFX_MSG_MAP(DefineEye4Calibration)
	ON_BN_CLICKED(IDSTART, OnStart)
	ON_BN_CLICKED(IDEXIT, OnExit)
	ON_BN_CLICKED(IDSAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DefineEye4Calibration message handlers

BOOL DefineEye4Calibration::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_exit.SetIconID(IDI_DELETE);
	
	// TODO: Add extra initialization here
	cvNamedWindow("EyeFrame", CV_WINDOW_AUTOSIZE );
	HWND hWnd = (HWND) cvGetWindowHandle("EyeFrame");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_CameraFrame4Eye)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DefineEye4Calibration::OnStart() 
{
	CvCapture* myCam = cvCaptureFromCAM(-1);
	
	if (myCam == NULL )
		MessageBox("Can not initialize capture device!\nPlease check the connection", "Error",  MB_OK|MB_ICONSTOP);
	else
	{
		GetDlgItem(IDSTART)->EnableWindow(false);
		GetDlgItem(IDSAVE)->EnableWindow(true);
		
		IplImage* myFrame = cvQueryFrame(myCam);
		cvWaitKey(10);

		//	**	RectangleSize is 50x20
		CvSize frameSize = cvGetSize(myFrame);
		CvPoint point1;
		point1.x = frameSize.width/2 - 30;
		point1.y = frameSize.height/2 - 20;
	
		CvPoint point2;
		point2.x = frameSize.width/2 + 30;
		point2.y = frameSize.height/2 + 20;

		//	Camera is initialized and frames are ready to be shown
		exitcode = 0;
		while (exitcode == 0)
		{
			cvRectangle( myFrame, point1, point2, CV_RGB(255,200,0), 1 );	
			cvShowImage("EyeFrame", myFrame);
			
			myFrame = cvQueryFrame(myCam);
			cvWaitKey(10);
			
			if (saveControl == true)	// Save is clicked
			{
				cvSetImageROI(myFrame, cvRect(point1.x, point1.y, point2.x-point1.x, point2.y-point1.y));
				cvSaveImage("C:\\SampleEye.jpg", myFrame);
				cvResetImageROI(myFrame);
				saveControl = false;
				m_saveStatus.Format("Image has been successfully saved...");
				UpdateData(false);
			}
		}//	**	time to exit
		
		cvReleaseCapture(&myCam);
		cvDestroyWindow("EyeFrame");
		OnOK();
	}
}

void DefineEye4Calibration::OnExit() 
{
	exitcode = 1;
}

void DefineEye4Calibration::OnSave() 
{
	saveControl = true; //	**	save Image	
}
