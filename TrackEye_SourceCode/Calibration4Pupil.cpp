// Calibration4Pupil.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "Calibration4Pupil.h"
#include "DefineEye4Calibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Hough2.h"

/////////////////////////////////////////////////////////////////////////////
// Calibration4Pupil dialog


Calibration4Pupil::Calibration4Pupil(CWnd* pParent /*=NULL*/)
	: CDialog(Calibration4Pupil::IDD, pParent)
{
	//{{AFX_DATA_INIT(Calibration4Pupil)
	thresholdingType = CV_RGB2XYZ;
	m_thresholdComponent = 2;
	m_binaryThreshold = TRUE;
	m_thresholdValue = 100;
	m_cannyApertureSize = 3;
	m_cannyLower = 200.0;
	m_cannyUpper = 255.0;
	m_sampleFilename = _T("C:\\SampleEye.jpg");
	m_sumcolorspace = _T("");
	m_sumcomponent = _T("");
	m_sumcanny = _T("");
	m_sumbinthreshold = _T("");
	m_sumbinthresholdvalue = _T("");
	circleAlgorithm = 1;
	m_circleAlg = _T("");
	//}}AFX_DATA_INIT
}


void Calibration4Pupil::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Calibration4Pupil)
	DDX_Control(pDX, IDClosePupilCalib, m_closePupilCalib);
	DDX_Control(pDX, IDSavePupilCalib, m_SaveSettings);
	DDX_Control(pDX, IDC_COMBO2, m_circleAlgorithm);
	DDX_Control(pDX, IDC_COMBO1, m_thresholdType);
	DDX_Radio(pDX, IDC_RADIO1, m_thresholdComponent);
	DDX_Check(pDX, IDC_CHECK2, m_binaryThreshold);
	DDX_Text(pDX, IDC_thresholdValue, m_thresholdValue);
	DDX_Text(pDX, IDC_CannyApertureSize, m_cannyApertureSize);
	DDX_Text(pDX, IDC_CannyLower, m_cannyLower);
	DDX_Text(pDX, IDC_CannyUpper, m_cannyUpper);
	DDX_Text(pDX, IDC_sampleFilename, m_sampleFilename);
	DDX_Text(pDX, IDC_sumcolorspace, m_sumcolorspace);
	DDX_Text(pDX, IDC_sumcomponent, m_sumcomponent);
	DDX_Text(pDX, IDC_sumcanny, m_sumcanny);
	DDX_Text(pDX, IDC_sumbinthreshold, m_sumbinthreshold);
	DDX_Text(pDX, IDC_sumbinthresholdvalue, m_sumbinthresholdvalue);
	DDX_Text(pDX, IDC_circleAlgorithm, m_circleAlg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Calibration4Pupil, CDialog)
	//{{AFX_MSG_MAP(Calibration4Pupil)
	ON_BN_CLICKED(IDSavePupilCalib, OnSavePupilCalib)
	ON_BN_CLICKED(IDClosePupilCalib, OnClosePupilCalib)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDTEST, OnTest)
	ON_BN_CLICKED(IDC_DeletesampleEye, OnDeletesampleEye)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Calibration4Pupil message handlers

BOOL Calibration4Pupil::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SaveSettings.SetIconID(IDI_SAVE);
	m_closePupilCalib.SetIconID(IDI_DELETE);
	
	//	**	ColorSpace initialization
	if (thresholdingType == CV_RGB2XYZ)
		m_thresholdType.SetCurSel(0);
	else if (thresholdingType == CV_RGB2YCrCb)
		m_thresholdType.SetCurSel(1);
	else if (thresholdingType == CV_RGB2HSV)
		m_thresholdType.SetCurSel(2);
	else if (thresholdingType == CV_RGB2Lab)
		m_thresholdType.SetCurSel(3);
	//	**	Circle detection algortihm
	//	**	ColorSpace initialization
	if (circleAlgorithm == 1)
		m_circleAlgorithm.SetCurSel(1);
	else if (circleAlgorithm == 0)
		m_circleAlgorithm.SetCurSel(0);

	CEdit* temp = (CEdit*) GetDlgItem(IDC_thresholdValue);
	temp->EnableWindow(m_binaryThreshold);

	HWND hWnd;
	HWND hParent;

	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("original");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_originaltext)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("colorspace", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("colorspace");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_colorspacetext)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("thresholded", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("thresholded");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_thresholdedtext)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("edges", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("edges");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_edgestext)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("final", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("final");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_finaltext)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	int width = 177; int height = 93;
	original_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	colorSpace_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	thresholded_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	edges_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	final_img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Calibration4Pupil::OnCheck2() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK2))->GetCheck();
	CEdit* temp = (CEdit*) GetDlgItem(IDC_thresholdValue);
	temp->EnableWindow(state);
}

void Calibration4Pupil::OnSavePupilCalib() 
{
	if (m_thresholdType.GetCurSel() == 0)
		thresholdingType = CV_RGB2XYZ;
	else if (m_thresholdType.GetCurSel() == 1)
		thresholdingType = CV_RGB2YCrCb;
	else if (m_thresholdType.GetCurSel() == 2)
		thresholdingType = CV_RGB2HSV;
	else if (m_thresholdType.GetCurSel() == 3)
		thresholdingType = CV_RGB2Lab;

	circleAlgorithm = m_circleAlgorithm.GetCurSel();
	
	UpdateData(true);

	this->DisplaySummary();

	MessageBox("Settings are successfully saved!", "Save OK", MB_OK|MB_ICONINFORMATION);
}

void Calibration4Pupil::OnTest() 
{
	int trick = 0;
	
	IplImage* sampleEye = cvLoadImage(m_sampleFilename, -1);
	if (sampleEye == NULL)
	{
		int res = MessageBox("File can not be loaded.\nDo you want to create a new eye image?", "Warning!", MB_YESNO|MB_ICONQUESTION);

		if (res == IDYES)
		{
			DefineEye4Calibration eyeDefinition;
			eyeDefinition.DoModal();
			sampleEye = cvLoadImage(m_sampleFilename, -1);
			trick = 1;			
		}
	}
	else 
		trick = 1;

	if (trick == 1)
	{
		//	**	Original image
		cvResize(sampleEye, original_img, CV_INTER_LINEAR);
		cvShowImage("original", original_img);

		//	**	Colorspace converted image
		IplImage* work1 = cvCreateImage(cvGetSize(sampleEye), IPL_DEPTH_8U, 3);
		cvCvtColor(sampleEye, work1, thresholdingType);
		
		IplImage* work2 = cvCreateImage(cvGetSize(sampleEye), IPL_DEPTH_8U, 1);
		if (m_thresholdComponent == 0)
			cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
		else if (m_thresholdComponent == 1)
			cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
		else 
			cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
		
		cvResize(work2, colorSpace_img);
		cvShowImage("colorspace", colorSpace_img);

		//	**	Binary thresholded image
		if (m_binaryThreshold == true)
		{
			cvThreshold(work2, work2, m_thresholdValue, 255, CV_THRESH_BINARY_INV);
			cvResize(work2, thresholded_img);
			cvShowImage("thresholded", thresholded_img);
		}

		//	**	Edges
		cvCanny(work2, work2, m_cannyLower, m_cannyUpper, m_cannyApertureSize);
		cvResize(work2, edges_img);
		cvShowImage("edges", edges_img);

		//	**	Detect circles
		int coords[3]; coords[0] = 0; coords[1] = 0; coords[2] = 0;
		
		if (circleAlgorithm == 1)
			findCircles2(work2, coords);
		else
			Hough2(work2, coords);
		
		if (coords[2]>0)
		{
			cvCircle( sampleEye, cvPoint(coords[0], coords[1]), coords[2], CV_RGB(0,0,255), 1 );
			cvResize(sampleEye, final_img);
		}
		
		cvShowImage("final", final_img);

		cvReleaseImage(&sampleEye);
		cvReleaseImage(&work1);
		cvReleaseImage(&work2);
	}
}

void Calibration4Pupil::OnClosePupilCalib() 
{
	cvDestroyWindow("original");
	cvDestroyWindow("colorspace");
	cvDestroyWindow("thresholded");
	cvDestroyWindow("edges");
	cvDestroyWindow("final");

	cvReleaseImage(&original_img);
	cvReleaseImage(&colorSpace_img);
	cvReleaseImage(&thresholded_img);
	cvReleaseImage(&edges_img);
	cvReleaseImage(&final_img);
	
	this->EndDialog(0);
}

void Calibration4Pupil::DisplaySummary()
{
	if ( thresholdingType == CV_RGB2XYZ )
		m_sumcolorspace.Format("Color space = CV_RGB2XYZ");
	else if (thresholdingType == CV_RGB2YCrCb)
		m_sumcolorspace.Format("Color space = CV_RGB2YCrCb");
	else if (thresholdingType == CV_RGB2HSV)
		m_sumcolorspace.Format("Color space = CV_RGB2HSV");
	else if (thresholdingType == CV_RGB2Lab)
		m_sumcolorspace.Format("Color space = CV_RGB2Lab");
	else 
		MessageBox("Internal error!", "Error", MB_OK|MB_ICONSTOP);

	m_sumcomponent.Format("Component of color space = %d", m_thresholdComponent+1);
	
	if (m_binaryThreshold == true)
	{
		m_sumbinthreshold.Format("Binary thresholding = YES");
		m_sumbinthresholdvalue.Format("Binary thresholding value = %d", m_thresholdValue);
	}
	else
	{
		m_sumbinthreshold.Format("Binary thresholding = NO");
		m_sumbinthresholdvalue.Format("Binary thresholding value = ***");
	}

	m_sumcanny.Format("Canny parameters = %.1f, %.1f, %d", m_cannyLower, m_cannyUpper, m_cannyApertureSize);
	
	circleAlgorithm == 1 ? m_circleAlg.Format("Circle algorithm = cvFitEllipse") : m_circleAlg.Format("Circle algorithm = Zafer's Algorithm");

	UpdateData(false);
}

void Calibration4Pupil::OnDeletesampleEye() 
{
	CFile::Remove(m_sampleFilename);
	MessageBox("File deleted", "info", MB_OK|MB_ICONINFORMATION);
}
