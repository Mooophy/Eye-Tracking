// TrackEyeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "TrackEyeDlg.h"
#include "PCASettings.h"
#include "TrackingSettings.h"
#include "TrackingParameters.h"
#include "USBCam.h"
#include "Tracking_Library.h"
#include "TrajectoryExtractor2D.h"
#include "EyeDatabaseExplorer.h"

#include "Face.h"

#ifdef _CH_
#define WIN32
#error "The file needs cvaux, which is not wrapped yet. Sorry"
#endif

#ifndef _EiC
#include "cvaux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#endif

#ifdef _EiC
#define WIN32
#endif

#define ORIG_WIN_SIZE  24



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackEyeDlg dialog

CTrackEyeDlg::CTrackEyeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackEyeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrackEyeDlg)
	m_leyedist = _T("Distance from Eye Space :");
	m_reyedist = _T("Distance from Eye Space :");
	m_hWnd = _T("");
	m_LPR = 0;
	m_LPX = 0;
	m_LPY = 0;
	m_RPR = 0;
	m_RPX = 0;
	m_RPY = 0;
	m_LEX = 0;
	m_LEY = 0;
	m_REX = 0;
	m_REY = 0;
	m_counter = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	FPSTimer = 0.0;
	FPS = 0.0;
	record = false;
	
	// Haar Face Detection
	storage = 0;
	hid_cascade = 0;
	NumOfHaarFaces = 0;
	delay = 10;
}

void CTrackEyeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackEyeDlg)
	DDX_Control(pDX, IDC_WRITE2AVI, m_write2AVI);
	DDX_Control(pDX, IDC_STOPRECORD, m_StopRecord);
	DDX_Control(pDX, IDC_RECORD, m_Record);
	DDX_Control(pDX, IDC_BLINKSTATUS, m_blinkStatus);
	DDX_Control(pDX, IDC_UpdayeEyeDatabase, m_UpdateEyeDatabase);
	DDX_Control(pDX, IDC_ExtractTrajectory, m_ExtractTrajectory);
	DDX_Control(pDX, IDR_StartTracking2, m_StopTracking);
	DDX_Control(pDX, IDR_StartTracking, m_StartTracking);
	DDX_Control(pDX, IDC_EyeTrackerSettings, m_TrackerSettings);
	DDX_Control(pDX, IDR_EXITPROGRAM, m_ExitProgram);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_SLIDERYplane, m_sliderYplane);
	DDX_Control(pDX, IDC_SLIDERXplane, m_sliderXplane);
	DDX_Control(pDX, IDC_CHECK1, m_headTrajectory);
	DDX_Control(pDX, IDC_SLIDER_R, m_sliderRight);
	DDX_Control(pDX, IDC_SLIDER_L, m_sliderLeft);
	DDX_Text(pDX, IDC_LEYEDIST, m_leyedist);
	DDX_Text(pDX, IDC_REYEDIST, m_reyedist);
	DDX_Text(pDX, IDC_STATIC_OUTPUT, m_hWnd);
	DDX_Text(pDX, IDC_EDITLPR, m_LPR);
	DDX_Text(pDX, IDC_EDITLPX, m_LPX);
	DDX_Text(pDX, IDC_EDITLPY, m_LPY);
	DDX_Text(pDX, IDC_EDITRPR, m_RPR);
	DDX_Text(pDX, IDC_EDITRPX, m_RPX);
	DDX_Text(pDX, IDC_EDITRPY, m_RPY);
	DDX_Text(pDX, IDC_EDITLEX, m_LEX);
	DDX_Text(pDX, IDC_EDITLEY, m_LEY);
	DDX_Text(pDX, IDC_EDITREX, m_REX);
	DDX_Text(pDX, IDC_EDITREY, m_REY);
	DDX_Text(pDX, IDC_numOfRecorded, m_counter);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrackEyeDlg, CDialog)
	//{{AFX_MSG_MAP(CTrackEyeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUTTRACKEYE, OnHelpAbouttrackeye)
	ON_COMMAND(ID_TRACKEYEMENU_PCASETTINGS, OnTrackeyemenuPcasettings)
	ON_COMMAND(ID_TRACKEYEMENU_FACETRACKERSETTINGS, OnTrackeyemenuFacetrackersettings)
	ON_COMMAND(ID_TRACKEYEMENU_ADJUSTCAPTUREDEVICESETTINGS, OnTrackeyemenuAdjustcapturedevicesettings)
	ON_BN_CLICKED(IDR_StartTracking, OnStartTracking)
	ON_COMMAND(ID_TRACKEYEMENU_EXIT, OnTrackeyemenuExit)
	ON_BN_CLICKED(IDR_StartTracking2, OnStopTracking)
	ON_BN_CLICKED(IDR_EXITPROGRAM, OnExitprogram)
	ON_BN_CLICKED(IDC_EyeTrackerSettings, OnEyeTrackerSettings)
	ON_BN_CLICKED(IDC_ClearHT, OnClearHT)
	ON_BN_CLICKED(IDC_ExtractTrajectory, OnExtractTrajectory)
	ON_BN_CLICKED(IDC_UpdayeEyeDatabase, OnUpdateEyeDatabase)
	ON_COMMAND(ID_DATABASE_EXPLOREDATABASE, OnDatabaseExploredatabase)
	ON_COMMAND(ID_TRACKEYEMENU_STARTTRACKING, OnTrackeyemenuStarttracking)
	ON_COMMAND(ID_TRACKEYEMENU_STOPTRACKING, OnTrackeyemenuStoptracking)
	ON_COMMAND(ID_TRACKEYEMENU_SELECTPROGRAMCOLORS, OnTrackeyemenuSelectprogramcolors)
	ON_COMMAND(ID_HELP_SHOWTRACKEYEHELPFILE, OnHelpShowtrackeyehelpfile)
	ON_WM_TIMER()
	ON_COMMAND(ID_DATABASE_CLEARDATABASE, OnDatabaseCleardatabase)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	ON_BN_CLICKED(IDC_STOPRECORD, OnStoprecord)
	ON_BN_CLICKED(IDC_WRITE2AVI, OnWrite2avi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackEyeDlg message handlers

BOOL CTrackEyeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	initializeGUI();
	
	trackControl = 0;
	TrackSettings = new TrackingSettings;

	//	**	Initialize sliders
	m_sliderLeft.SetRange(0, 8, true);
	m_sliderLeft.SetTicFreq(1);
	m_sliderLeft.SetPos(4);
	m_sliderRight.SetRange(0, 8, true);
	m_sliderRight.SetTicFreq(1);
	m_sliderRight.SetPos(4);

	//	**	Init Trajectory Buffer & create window
	for (int k=0;k<3;k++)
	{
		trajectoryBuffer[k].center.x = -1;
		trajectoryBuffer[k].center.y = -1;
		trajectoryBuffer[k].size.width = -1;
		trajectoryBuffer[k].size.height = -1;
	}
	
	resizedheadTrajectory = cvCreateImage(cvSize(172, 146), IPL_DEPTH_8U, 1);
	resizedheadTrajectory->origin = 1;

	//	**	Initialize X&Y plane sliders & depth indicator progress bar
	m_sliderXplane.SetRange(0, 20, true);
	m_sliderXplane.SetTicFreq(1);
	m_sliderXplane.SetPos(0);
	m_sliderYplane.SetRange(0, 20, true);
	m_sliderYplane.SetTicFreq(1);
	m_sliderYplane.SetPos(0);

	m_progress.SetRange(0, 20);
	m_progress.SetStep(1);

	// Check whether the EyeDatabase folder exists or not?
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);
	Path += "EyeDatabase";

	if (::CreateDirectory(Path, NULL))
		MessageBox("Eye Database directory is empty.\n Do not forget to create a eye database!", "Error", MB_OK|MB_ICONEXCLAMATION);
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTrackEyeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//	If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTrackEyeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTrackEyeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CTrackEyeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	tips.RelayEvent(pMsg);	
	return CDialog::PreTranslateMessage(pMsg);
}


void CTrackEyeDlg::initializeGUI()
{
	// Icons
	m_TrackerSettings.SetIconID(IDI_SETTINGS);
	m_UpdateEyeDatabase.SetIconID(IDI_UPDATE);
	m_StartTracking.SetIconID(IDI_START);
	m_StopTracking.SetIconID(IDI_STOP);
	m_ExtractTrajectory.SetIconID(IDI_CAM1);
	m_ExitProgram.SetIconID(IDI_EXIT);
	m_Record.SetIconID(IDI_RECORD);
	m_StopRecord.SetIconID(IDI_STOPRECORD);
	m_write2AVI.SetIconID(IDI_AVI);
	// Tips
	tips.Create(this);
	tips.AddTool(GetDlgItem(IDC_EyeTrackerSettings), "Adjust Tracker Settings");
	tips.AddTool(GetDlgItem(IDC_UpdayeEyeDatabase), "Update Eye Database");
	tips.AddTool(GetDlgItem(IDR_StartTracking), "Start Tracking");
	tips.AddTool(GetDlgItem(IDR_StartTracking2), "Stop Tracking");
	tips.AddTool(GetDlgItem(IDC_ExtractTrajectory), "2D Trajectory Extractor");
	tips.AddTool(GetDlgItem(IDR_EXITPROGRAM), "Exit Program");
	tips.AddTool(GetDlgItem(IDC_RECORD), "Record Output Window");
	tips.AddTool(GetDlgItem(IDC_STOPRECORD), "Stop Recording");
	tips.Activate(true);
}

void CTrackEyeDlg::OnHelpAbouttrackeye() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CTrackEyeDlg::OnTrackeyemenuPcasettings() 
{
	PCASettings PCADlg;
	PCADlg.DoModal();
}


void CTrackEyeDlg::OnUpdateEyeDatabase() 
{
	OnTrackeyemenuPcasettings();
}


void CTrackEyeDlg::OnTrackeyemenuFacetrackersettings() 
{
	TrackSettings->DoModal();
	
	GetDlgItem(IDC_BLINKTEXT)->ShowWindow(TrackSettings->params->blink ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BLINKSTATUS)->ShowWindow(TrackSettings->params->blink ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_LoggingStatus)->ShowWindow(TrackSettings->params->blink ? SW_SHOW : SW_HIDE);
}

void CTrackEyeDlg::OnEyeTrackerSettings() 
{
	OnTrackeyemenuFacetrackersettings();	
}

void CTrackEyeDlg::OnTrackeyemenuAdjustcapturedevicesettings() 
{
	USBCam usbcamera;
	usbcamera.DoModal();
}

void CTrackEyeDlg::OnTrackeyemenuStarttracking() 
{
	OnStartTracking();
}

void CTrackEyeDlg::OnTrackeyemenuStoptracking() 
{
	OnStopTracking();	
}

void CTrackEyeDlg::OnTrackeyemenuExit() 
{
	EndDialog(1);	
}

void CTrackEyeDlg::OnExitprogram() 
{
	EndDialog(1);
}

void CTrackEyeDlg::OnHelpShowtrackeyehelpfile() 
{
	CWinApp* theApp;
	theApp = AfxGetApp();
	//theApp->WinHelp(NULL);
	::WinExec("winhelp trackEye.hlp", 1);
	//::WinHelp(m_hWnd, theApp->m_pszHelpFilePath, HELP_CONTENTS, NULL);
}

//	************************************************
//	***	 Here comes the real tracking algorithm  ***
//	************************************************

void CTrackEyeDlg::OnStartTracking()
{
	cvNamedWindow("HT", CV_WINDOW_AUTOSIZE);
	HWND hWndHT = (HWND) cvGetWindowHandle("HT");
	HWND hParentHT = ::GetParent(hWndHT);
	::SetParent(hWndHT, GetDlgItem(IDC_Trajectory)->m_hWnd);
	::ShowWindow(hParentHT, SW_HIDE);
	
	cvNamedWindow("Tracking", 0);
	HWND hWnd = (HWND) cvGetWindowHandle("Tracking");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_OUTPUT)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	GetDlgItem(IDC_EyeTrackerSettings)->EnableWindow(false);
	GetDlgItem(IDC_UpdayeEyeDatabase)->EnableWindow(false);
	GetDlgItem(IDC_ExtractTrajectory)->EnableWindow(false);
	GetDlgItem(IDR_EXITPROGRAM)->EnableWindow(false);
	GetDlgItem(IDR_StartTracking)->EnableWindow(false);
	GetDlgItem(IDR_StartTracking2)->EnableWindow(true);
	GetDlgItem(IDC_RECORD)->EnableWindow(true);
	
	if (TrackSettings->m_source == 0) // WEBCAM
	{
		Camera = cvCaptureFromCAM(-1);
		delay = 10;
	}
	else
	{
		char* videoFile = new char[200];
		sprintf(videoFile, "%s", TrackSettings->videoFile);
		Camera = cvCaptureFromAVI(videoFile);
		free(videoFile);
		delay = 200;
	}

	if (Camera == NULL )
	{
		MessageBox("Can not initialize capture!", "Error",  MB_OK|MB_ICONSTOP);
		exit(1);
	}
	
	CameraFrame = cvQueryFrame(Camera); cvWaitKey(10);
	
	// Preparations before tracking
	CvBox2D faceBox;
	
	CvHistogram* hist;
	CvRect searchWin;
	CvConnectedComp comp;
	Face face(TrackSettings);
	
	if (TrackSettings->m_faceAlgorithm != 2)
		face.loadDatabase(TrackSettings);
	
	//cvNamedWindow("Tester1", -1);
	
	if (TrackSettings->m_faceAlgorithm == 0) // Haar Face Tracking
	{
		CvHaarClassifierCascade* cascade = cvLoadHaarClassifierCascade( "<default_face_cascade>",	cvSize( ORIG_WIN_SIZE, ORIG_WIN_SIZE ));
		hid_cascade = cvCreateHidHaarClassifierCascade( cascade, 0, 0, 0, 1 );
		cvReleaseHaarClassifierCascade( &cascade );
		
		storage = cvCreateMemStorage(0);
	}
	else if (TrackSettings->m_faceAlgorithm == 1)// CamShift algorithm
	{
		cvNamedWindow("Eigen", -1);
		//cvNamedWindow("ROI", -1);
		cvNamedWindow("threshold", -1);
		//cvNamedWindow("Tester1", -1);
		//cvNamedWindow("Tester2", -1);
	
		IplImage* SampleForHUE = cvLoadImage("C:\\SampleHUE.jpg", -1);
		if (SampleForHUE == NULL)	
		{
			MessageBox("Camshift can not be initialized\nDefine HUE first", "Error", MB_OK|MB_ICONSTOP);
			exit(1);
		}
		else
		{
			//	**	Inputs for CamShift algorithm		
			IplImage* HUE = cvCreateImage(cvGetSize(SampleForHUE), IPL_DEPTH_8U, 1);	
			extractHUE(SampleForHUE, HUE);	//	**	Extract HUE information
		
			int hist_size	  = 20;
			float ranges[]	  = { 0, 180 };
			float* pranges[]  = {ranges};
			hist = cvCreateHist( 1, &hist_size, CV_HIST_ARRAY, pranges, 1 );
			cvCalcHist(&HUE, hist);	//	**	Calculate histogram of HUE part
			
			headTrajectory = cvCreateImage(cvSize(CameraFrame->width, CameraFrame->height), IPL_DEPTH_8U, 1);
			
			hueFrame = cvCreateImage(cvGetSize(CameraFrame), IPL_DEPTH_8U, 1);
			backProject = cvCreateImage(cvGetSize(CameraFrame), IPL_DEPTH_8U, 1);
			extractHUE(CameraFrame, hueFrame);
					
			// whole image
			searchWin.x = 0;
			searchWin.y = 0;
			searchWin.width = hueFrame->width;
			searchWin.height = hueFrame->height;

			//AfxMessageBox("Camshift initialization OK!");
		}
	}
			
	FPSTimer = 0.0;
	FPS = 0.0;
	SetTimer(0x1010, 1000, NULL);

	trackControl = 1;
	while (trackControl != 0)
	{
		if (TrackSettings->m_faceAlgorithm == 0) // Haar Face Detection
			HaarFaceDetect( CameraFrame, &faceBox);
		
		else if (TrackSettings->m_faceAlgorithm == 1)	// CamShift algorithm
		{
			extractHUE( CameraFrame, hueFrame );
			cvCalcBackProject( &hueFrame, backProject, hist ); // Probability is formed
			//cvShowImage("Tester2", backProject);
			cvCamShift( backProject, searchWin, cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 15, 0.1 ), &comp, &faceBox );
			searchWin = comp.rect;
		}
						
		
		// Detecting eyes
		if (TrackSettings->params->trackEyes == 1)
		{
			if ( (TrackSettings->m_faceAlgorithm == 1 && (faceBox.size.width>CameraFrame->width/6)) || 
				 (TrackSettings->m_faceAlgorithm == 0 && (this->NumOfHaarFaces>0)) )
			{
				cvSetImageROI(CameraFrame, cvRect ((faceBox.center.x-faceBox.size.width/2), (faceBox.center.y-faceBox.size.height/2), faceBox.size.width, faceBox.size.height));
				if (TrackSettings->m_eyeAlgorithm == 0)
				{
					face.findEyes_TM(CameraFrame, TrackSettings);
					TrackSettings->params->eye1.eyefound = true;
				}
				else
					face.findEyes(CameraFrame, TrackSettings);
				
				cvResetImageROI(CameraFrame);
				
				face.UpdateCoordinates(CameraFrame, faceBox, TrackSettings);
				
				//	Track pupils ?
				if (TrackSettings->params->pupil == 1)
					face.detectEyePupil(CameraFrame, TrackSettings);
				//	Snake?
				if (TrackSettings->params->snake == 1)
					face.snake(CameraFrame, TrackSettings);
				else if (TrackSettings->params->fitellipse == 1)
					face.CCA(CameraFrame, TrackSettings);

				//	Here are the results
				face.drawEyePupilCircles(CameraFrame, TrackSettings);
				face.drawEyeRectangles(CameraFrame, TrackSettings);
				DisplayEyeStatus(TrackSettings);
			}

			if ( TrackSettings->m_faceAlgorithm == 0 && this->NumOfHaarFaces==0)
			{
				TrackSettings->params->eye1.eyefound = false;
				TrackSettings->params->eye2.eyefound = false;
				face.drawEyeRectangles(CameraFrame, TrackSettings);
			}
		}
		
		// Draw face box
		if (TrackSettings->programColors.m_drawFaceBox == true)
			if (TrackSettings->m_faceAlgorithm == 1 || (TrackSettings->m_faceAlgorithm == 0 && this->NumOfHaarFaces>0) )
				drawFaceBox(CameraFrame, faceBox, TrackSettings->programColors.colors[0]);
		
		// Show the final image
		cvShowImage("Tracking", CameraFrame);

		//	**	Draw head trajectory
		drawHeadTrajectory(CameraFrame, &faceBox);
		
		//	Record to jpg files
		if (record == true)
		{
			recordFileName.Format("%s\\record%d.jpg", TrackSettings->recordFolder, m_counter); 
			cvSaveImage(recordFileName, CameraFrame);
			m_counter++;		
			CString temp; temp.Format("%d", m_counter);
			GetDlgItem(IDC_numOfRecorded)->SetWindowText(temp);
		}
		
		CameraFrame = cvQueryFrame(Camera);	
		
		cvWaitKey(delay);

		if (CameraFrame == NULL)
			trackControl = 0; // EnfOf AVI file.
		
		//cvShowImage("Tester1", hueFrame);
		
		FPS++;
	}
			
	FPSTimer = 0.0;
	FPS = 0.0;
	KillTimer(0x1010);

	// Cleanup
	if (TrackSettings->m_faceAlgorithm == 1)
	{
		face.unloadDatabase(TrackSettings);
		cvReleaseImage(&hueFrame);
	}
	cvReleaseCapture(&Camera);
}

void CTrackEyeDlg::OnStopTracking() 
{
	trackControl = 0;

	GetDlgItem(IDC_EyeTrackerSettings)->EnableWindow(true);
	GetDlgItem(IDC_UpdayeEyeDatabase)->EnableWindow(true);
	GetDlgItem(IDC_ExtractTrajectory)->EnableWindow(true);
	GetDlgItem(IDR_EXITPROGRAM)->EnableWindow(true);
	GetDlgItem(IDR_StartTracking)->EnableWindow(true);
	GetDlgItem(IDR_StartTracking2)->EnableWindow(false);
	
	cvDestroyWindow("Tracking");
	cvDestroyWindow("HT");
	cvDestroyWindow("Eigen");
	//cvDestroyWindow("ROI");
	cvDestroyWindow("threshold");
	
	//cvDestroyWindow("Tester1");
	//cvDestroyWindow("Tester2");
}

void CTrackEyeDlg::DisplayEyeStatus(TrackingSettings* settings)
{
	
	//	**	Display eye distance & eye coordinates
	m_leyedist.Format("Distance from Eye Space : %.1f", settings->params->eye1.distance);
	m_reyedist.Format("Distance from Eye Space : %.1f", settings->params->eye2.distance);
	m_LEX = settings->params->eye1.coords.x;
	m_LEY = settings->params->eye1.coords.y;
	m_REX = settings->params->eye2.coords.x;
	m_REY = settings->params->eye2.coords.y;

	int tickSize = settings->params->eye1.RectSize.width / 8;
	//	**	Display pupil coordinates
	if (settings->params->pupil == 1)
	{
		m_LPX = settings->params->eye1.coords.x + settings->params->eye1.eyePupil.x;
		m_LPY = settings->params->eye1.coords.x + settings->params->eye1.eyePupil.y;
		m_LPR = settings->params->eye1.eyePupil.radius;
		//	**	Update slider position
		m_sliderLeft.SetPos(settings->params->eye1.eyePupil.x / tickSize);
	
		m_RPX = settings->params->eye2.coords.x + settings->params->eye2.eyePupil.x;
		m_RPY = settings->params->eye2.coords.x + settings->params->eye2.eyePupil.y;
		m_RPR = settings->params->eye2.eyePupil.radius;
		//	**	Update slider position
		m_sliderRight.SetPos(settings->params->eye2.eyePupil.x / tickSize);
	}

	displayBlinkStatus();
	
	UpdateData(false);
}


void CTrackEyeDlg::drawHeadTrajectory(IplImage* CameraFrame, CvBox2D* faceBox)
{
	//	**	There is a global variable named "HeadTrajectory" of type IplImage*
	if (m_headTrajectory.GetCheck() == 1)
	{	
		//	**	Update trajectoryBuffer
		for (int k=0; k<=1; k++)
		{
			trajectoryBuffer[k].center.x = trajectoryBuffer[k+1].center.x;
			trajectoryBuffer[k].center.y = trajectoryBuffer[k+1].center.y;
			trajectoryBuffer[k].size.width = trajectoryBuffer[k+1].size.width;
			trajectoryBuffer[k].size.height = trajectoryBuffer[k+1].size.height;
		}

		trajectoryBuffer[2].center.x = faceBox->center.x;
		trajectoryBuffer[2].center.y = faceBox->center.y;
		trajectoryBuffer[2].size.width = faceBox->size.width;
		trajectoryBuffer[2].size.height = faceBox->size.height;
		
		CvPoint traPts[2];
		if (trajectoryBuffer[1].size.width > 0 && 
			(trajectoryBuffer[1].center.x != trajectoryBuffer[2].center.x ||
			trajectoryBuffer[1].center.y != trajectoryBuffer[2].center.y))
		{
			for (int n=0; n<=1; n++)
			{
				traPts[n].x = (int)trajectoryBuffer[n+1].center.x;
				traPts[n].y = (int)trajectoryBuffer[n+1].center.y;
			}
			cvLine(headTrajectory, traPts[0], traPts[1], CV_RGB(255,255,255), 2, 8);

			//	**	Update Slider positions
			int tickSizeX = CameraFrame->width/20;
			int tickSizeY = CameraFrame->height/20;
			
			m_sliderXplane.SetPos(trajectoryBuffer[2].center.x / tickSizeX);
			m_sliderYplane.SetPos(20-trajectoryBuffer[2].center.y / tickSizeY);

			//	**	Update depth
			int frameArea = CameraFrame->width*CameraFrame->height/20;
			m_progress.SetPos(trajectoryBuffer[2].size.width*trajectoryBuffer[2].size.height/frameArea);
		}

		cvResize(headTrajectory, resizedheadTrajectory, CV_INTER_LINEAR);
		cvShowImage("HT", resizedheadTrajectory);
	}
}


void CTrackEyeDlg::OnClearHT() 
{
	cvZero(headTrajectory);
}


void CTrackEyeDlg::OnExtractTrajectory() 
{
	TrajectoryExtractor2D AVIdlg;
	AVIdlg.DoModal();
}

void CTrackEyeDlg::OnDatabaseExploredatabase() 
{
	EyeDatabaseExplorer explorer(NULL, TrackSettings->m_DBdirectory);
	explorer.DoModal();
}

void CTrackEyeDlg::OnDatabaseCleardatabase() 
{
	if (MessageBox("Delete EyeDatabase?", "Warning", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		CString filename;
		filename.Format("%s\\Eye0.jpg", TrackSettings->m_DBdirectory);
		
		CFile myFile;
		
		int k=1;
		while (myFile.Open(filename, CFile::modeRead, NULL) != false)
		{
			myFile.Close();
			CFile::Remove(filename);	//	Delete file
			filename.Format("%s\\Eye%d.jpg", TrackSettings->m_DBdirectory, k);
			k++;
		}
		
		MessageBox("Database has been cleared", "info", MB_OK|MB_ICONEXCLAMATION);
	}
}

void CTrackEyeDlg::OnTrackeyemenuSelectprogramcolors() 
{
	TrackSettings->programColors.DoModal();	
}

void CTrackEyeDlg::displayBlinkStatus()
{
	if (TrackSettings->params->blink == 1)
	{
		if (TrackSettings->params->eye1.eyefound == true)
		{
			if (TrackSettings->params->eye1.blinkStatus == 1)
			{
				m_blinkStatus.SetSel(-1, false);
				m_blinkStatus.ReplaceSel("Left Eye Blink\r\n");
			}
		}
		if (TrackSettings->params->eye2.eyefound == true)
		{
			if (TrackSettings->params->eye2.blinkStatus == 1)
			{
				m_blinkStatus.SetSel(-1, false);
				m_blinkStatus.ReplaceSel("Right Eye Blink\r\n");
			}
		}
	}
}


void CTrackEyeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	FPSTimer += 2.0;
	CString temp; temp.Format("%.1f", FPS/FPSTimer);
	GetDlgItem(IDC_FPS)->SetWindowText(temp);

	CDialog::OnTimer(nIDEvent);
}

bool CTrackEyeDlg::ChooseDirectory(char *folderName)
{
	BROWSEINFO bi;
	ITEMIDLIST* pItemIDList;
	
	char folder[_MAX_PATH];
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = folder;
	bi.lpszTitle = "Choose folder to store records";
	
	if((pItemIDList = SHBrowseForFolder(&bi))!=NULL) 
	{
		SHGetPathFromIDList(pItemIDList, folderName);
		return true;
	}
	else
		return false;
}

void CTrackEyeDlg::OnRecord() 
{
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);

	TrackSettings->recordFolder = Path+"Records\\";

	if (::CreateDirectory(TrackSettings->recordFolder, NULL) == 0) // If already exists then clear the folder contents
	{
		CString filename;
		filename.Format("%srecord0.jpg", TrackSettings->recordFolder);
		
		CFile myFile;
		
		int k=1;
		while (myFile.Open(filename, CFile::modeRead, NULL) != false)
		{
			myFile.Close();
			CFile::Remove(filename);	//	Delete file
			filename.Format("%srecord%d.jpg", TrackSettings->recordFolder, k);
			k++;
		}
	}

	m_counter = 0;
	record = true;
	
	GetDlgItem(IDC_RECORD)->EnableWindow(false);
	GetDlgItem(IDC_STOPRECORD)->EnableWindow(true);
	GetDlgItem(IDC_RECORDTEXT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_numOfRecorded)->ShowWindow(SW_SHOW);

	start = clock();
}

void CTrackEyeDlg::OnStoprecord() 
{
	record = false;

	GetDlgItem(IDC_RECORD)->EnableWindow(true);
	GetDlgItem(IDC_STOPRECORD)->EnableWindow(false);
	GetDlgItem(IDC_RECORDTEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_numOfRecorded)->ShowWindow(SW_HIDE);
	
	finish = clock();
	double fark = (double)(finish-start)/CLOCKS_PER_SEC;
	CString a; 
	a.Format("start = %2.2f, stop = %2.2f, Fark = %2.2f", start, finish, fark);
	AfxMessageBox(a);

	((CButton*)GetDlgItem(IDC_WRITE2AVI))->EnableWindow(true);
}


void CTrackEyeDlg::HaarFaceDetect( IplImage* img, CvBox2D* faceBox)
{
    int scale = 2;
    IplImage* temp = cvCreateImage( cvSize(img->width/2,img->height/2), 8, 3 );
    CvPoint pt1, pt2;
    int i;

    cvPyrDown( img, temp, CV_GAUSSIAN_5x5 );
	#ifdef WIN32
		cvFlip( temp, temp, 0 );
	#endif    
    cvClearMemStorage( storage );

    if( hid_cascade )
    {
        CvSeq* faces = cvHaarDetectObjects( temp, hid_cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING );
        
		NumOfHaarFaces = faces->total;

		if (NumOfHaarFaces > 0)
		{
            CvRect* r = (CvRect*)cvGetSeqElem( faces, 0, 0 );
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
			#ifdef WIN32            
				pt1.y = img->height - r->y*scale;
				pt2.y = img->height - (r->y+r->height)*scale;
			#else
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;
			#endif            
			
			faceBox->center.x = (float)(pt1.x+pt2.x)/2.0;
			faceBox->center.y = (float)(pt1.y+pt2.y)/2;
			faceBox->size.width = (float)(pt2.x - pt1.x);
			faceBox->size.height = (float)(pt1.y - pt2.y);
        }
    }

    cvShowImage( "Tracking", img );
    cvReleaseImage( &temp );
}

void CTrackEyeDlg::OnWrite2avi() 
{
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);

	CString* AVIFilename = new CString;
	FilenameDlg fileDlg(NULL, AVIFilename);
	fileDlg.DoModal();

	*AVIFilename = Path + *AVIFilename;

	CString tempStr;
	tempStr.Format("%srecord0.jpg", TrackSettings->recordFolder);
	IplImage* frame = cvLoadImage(tempStr, -1);


	char* AVIFilename_Char = new char[200];
	sprintf(AVIFilename_Char, "%s", *AVIFilename);
	
	CvAVIWriter* AVIWriter = cvCreateAVIWriter(AVIFilename_Char, -1, 5, cvGetSize(frame));
	
	int n=0;
	for (;;)
	{
		tempStr.Format("%srecord%d.jpg", TrackSettings->recordFolder, n);
		frame = cvLoadImage(tempStr, -1);
		if (frame == NULL)
			break;
	
		cvWriteToAVI(AVIWriter, frame);
		cvReleaseImage(&frame);
		n++;

	}

	MessageBox("AVI write succesfull\n"+*AVIFilename, "AVI Write", MB_OK|MB_ICONINFORMATION);

	cvReleaseAVIWriter(&AVIWriter);
	free(AVIFilename);
}
