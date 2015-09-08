// Calibration4Snake.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "Calibration4Snake.h"
#include "DefineEye4Calibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Calibration4Snake dialog


Calibration4Snake::Calibration4Snake(CWnd* pParent /*=NULL*/)
	: CDialog(Calibration4Snake::IDD, pParent)
{
	//{{AFX_DATA_INIT(Calibration4Snake)
	m_component = 0;
	m_thresholdValue = -12;
	m_cannyApertureSize = 15;
	m_cannyLower = 0;
	m_cannyUpper = 255;
	m_blockSize = 3;
	m_alpha = 0.2f;
	m_beta = 0.15f;
	m_gamma = 0.2f;
	m_iterations = 40;
	m_nPoints = 24;
	//}}AFX_DATA_INIT
	colorSpace = CV_RGB2HSV;
	thresholdOrNot = true;
	thresholdType = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
}


void Calibration4Snake::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Calibration4Snake)
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDSaveSnakeCalib, m_SaveSettings);
	DDX_Control(pDX, IDC_COMBO2, m_thresholdType);
	DDX_Control(pDX, IDC_COMBO1, m_ColorSpace);
	DDX_Radio(pDX, IDC_RADIO1, m_component);
	DDX_Text(pDX, IDC_thresholdValue, m_thresholdValue);
	DDV_MinMaxInt(pDX, m_thresholdValue, -100, 254);
	DDX_Text(pDX, IDC_Gamma, m_gamma);
	DDX_Text(pDX, IDC_CannyApertureSize, m_cannyApertureSize);
	DDX_Text(pDX, IDC_CannyLower, m_cannyLower);
	DDV_MinMaxInt(pDX, m_cannyLower, 0, 255);
	DDX_Text(pDX, IDC_CannyUpper, m_cannyUpper);
	DDV_MinMaxInt(pDX, m_cannyUpper, 0, 255);
	DDX_Text(pDX, IDC_blockSize, m_blockSize);
	DDX_Text(pDX, IDC_Beta, m_beta);
	DDX_Text(pDX, IDC_Alpha, m_alpha);
	DDX_Text(pDX, IDC_iterations, m_iterations);
	DDV_MinMaxInt(pDX, m_iterations, 1, 1001);
	DDX_Text(pDX, IDC_nPoints, m_nPoints);
	DDV_MinMaxInt(pDX, m_nPoints, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Calibration4Snake, CDialog)
	//{{AFX_MSG_MAP(Calibration4Snake)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDSaveSnakeCalib, OnSaveSnakeCalib)
	ON_BN_CLICKED(IDC_TESTSETTINGS, OnTestsettings)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Calibration4Snake message handlers

BOOL Calibration4Snake::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SaveSettings.SetIconID(IDI_SAVE);
	m_delete.SetIconID(IDI_DELETE);

	switch (colorSpace)
	{
	case -11:
		m_ColorSpace.SetCurSel(0);
		break;
	case CV_RGB2GRAY:
		m_ColorSpace.SetCurSel(1);
		break;
	case CV_RGB2XYZ:
		m_ColorSpace.SetCurSel(2);
		break;
	case CV_RGB2YCrCb:
		m_ColorSpace.SetCurSel(3);
		break;
	case CV_RGB2HSV:
		m_ColorSpace.SetCurSel(4);
		break;
	case CV_RGB2Lab:
		m_ColorSpace.SetCurSel(5);
		break;
	}

	CButton* temp = (CButton*)GetDlgItem(IDC_CHECK2); temp->SetCheck(thresholdOrNot);
	GetDlgItem(IDC_COMBO2)->EnableWindow(thresholdOrNot);
	GetDlgItem(IDC_thresholdValue)->EnableWindow(thresholdOrNot);
	GetDlgItem(IDC_blockSize)->EnableWindow(thresholdOrNot);

	switch (thresholdType)
	{
	case -11:
		m_thresholdType.SetCurSel(0);
		break;
	case CV_ADAPTIVE_THRESH_MEAN_C:
		m_thresholdType.SetCurSel(1);
		break;
	case CV_ADAPTIVE_THRESH_GAUSSIAN_C:
		m_thresholdType.SetCurSel(2);
		break;
	}	

	HWND hWnd;
	HWND hParent;

	cvNamedWindow("RGNL", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("RGNL");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_RGNL)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("CLRSPC", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("CLRSPC");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_CLRSPC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("THRSHLD", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("THRSHLD");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_THRSHLD)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("DGDTCTN", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("DGDTCTN");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_DGDTCTN)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	cvNamedWindow("FNL", CV_WINDOW_AUTOSIZE);
	hWnd = (HWND) cvGetWindowHandle("FNL");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_FNL)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	int width = 135; int height = 59;
	originalImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	colorSpaceImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	thresholdedImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	edgesImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	finalImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Calibration4Snake::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	cvDestroyWindow("RGNL");
	cvDestroyWindow("CLRSPC");
	cvDestroyWindow("THRSHLD");
	cvDestroyWindow("DGDTCTN");
	cvDestroyWindow("FNL");

	cvReleaseImage(&originalImg);
	cvReleaseImage(&colorSpaceImg);
	cvReleaseImage(&thresholdedImg);
	cvReleaseImage(&edgesImg);
	cvReleaseImage(&finalImg);
	
	this->EndDialog(0);
	
	CDialog::OnClose();
}

void Calibration4Snake::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	CButton* temp = (CButton*) GetDlgItem(IDC_CHECK2);
	thresholdOrNot = (bool)temp->GetCheck();
	GetDlgItem(IDC_COMBO2)->EnableWindow(thresholdOrNot);
	GetDlgItem(IDC_thresholdValue)->EnableWindow(thresholdOrNot);
	GetDlgItem(IDC_blockSize)->EnableWindow(thresholdOrNot);
}

void Calibration4Snake::OnSaveSnakeCalib() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	switch (m_ColorSpace.GetCurSel())
	{
	case 0:
		colorSpace = -11;
		break;
	case 1:
		colorSpace = CV_RGB2GRAY;
		break;
	case 2:
		colorSpace = CV_RGB2XYZ;
		break;
	case 3:
		colorSpace = CV_RGB2YCrCb;
		break;
	case 4:
		colorSpace = CV_RGB2HSV;
		break;
	case 5:
		colorSpace = CV_RGB2Lab;
		break;
	}
	
	CButton* temp = (CButton*)GetDlgItem(IDC_CHECK2);
	(temp->GetCheck() ? (thresholdOrNot = true) : (thresholdOrNot = false));

	switch (m_thresholdType.GetCurSel())
	{
	case 0:
		thresholdType = -11;
		break;
	case 1:
		thresholdType = CV_ADAPTIVE_THRESH_MEAN_C;
		break;
	case 2:
		thresholdType = CV_ADAPTIVE_THRESH_GAUSSIAN_C;
		break;
	}
	
	if (m_nPoints<8)
	{
		m_nPoints = 8;
		UpdateData(false);
	}

	MessageBox("Settings are succesfully saved...", "Settings saved", MB_OK|MB_ICONINFORMATION);
}

void Calibration4Snake::OnTestsettings() 
{
	int trick = 0;
	
	IplImage* sampleEye = cvLoadImage("C:\\SampleEye.jpg", -1);
	if (sampleEye == NULL)
	{
		int res = MessageBox("Test file doesn't exist.\nDo you want to create a new eye image?", "Warning!", MB_YESNO|MB_ICONQUESTION);

		if (res == IDYES)
		{
			DefineEye4Calibration eyeDefinition;
			eyeDefinition.DoModal();
			sampleEye = cvLoadImage("C:\\SampleEye.jpg", -1);
			trick = 1;			
		}
	}
	else 
		trick = 1;

	if (trick == 1)
	{
		//	**	Original image
		cvResize(sampleEye, originalImg, CV_INTER_LINEAR);
		cvShowImage("RGNL", originalImg);

		
		//	**	Colorspace converted image
		IplImage* work1 = NULL;
		if (colorSpace == CV_RGB2GRAY)
		{
			work1 = cvCreateImage(cvGetSize(sampleEye), IPL_DEPTH_8U, 1);
			cvCvtColor(sampleEye, work1, colorSpace);
		}
		else
		{
			work1 = cvCreateImage(cvGetSize(sampleEye), IPL_DEPTH_8U, 3);
			if (colorSpace != -11)
				cvCvtColor(sampleEye, work1, colorSpace);
			else
				work1 = cvCloneImage(sampleEye); //	**	None is selected
		}
		
		//	**	Get Component
		IplImage* work2 = cvCreateImage(cvGetSize(sampleEye), IPL_DEPTH_8U, 1);
		if (colorSpace != CV_RGB2GRAY)
		{
			if (m_component == 0)
				cvCvtPixToPlane( work1, work2, NULL, NULL, NULL );
			else if (m_component == 1)
				cvCvtPixToPlane( work1, NULL, work2, NULL, NULL );
			else 
				cvCvtPixToPlane( work1, NULL, NULL, work2, NULL );
		}
		else
			work2 = cvCloneImage(work1);

		cvResize(work2, colorSpaceImg);
		cvShowImage("CLRSPC", colorSpaceImg);

		//	**	Binary thresholded image
		if (thresholdOrNot == true)
		{
			if (thresholdType == -11)
				cvThreshold(work2, work2, m_thresholdValue, 255, CV_THRESH_BINARY_INV);
			else
			{
				if (m_blockSize%2 != 1)
					m_blockSize = 3;
				cvAdaptiveThreshold(work2, work2, (float)255, thresholdType, CV_THRESH_BINARY_INV, m_blockSize, m_thresholdValue);
			}
			
			cvResize(work2, thresholdedImg);
			cvShowImage("THRSHLD", thresholdedImg);
		}

		//	**	Edges
		//cvCanny(work2, work2, m_cannyLower, m_cannyUpper, m_cannyApertureSize);
		cvResize(work2, edgesImg);
		cvShowImage("DGDTCTN", edgesImg);

		//	**	Snake minimization
		CvPoint* snk = (CvPoint*)malloc(sizeof(CvPoint)*m_nPoints);
		CvSize win = {3,3}; 
		CvTermCriteria criteria; 
		criteria.type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
		criteria.maxIter = m_iterations;
		criteria.epsilon = 1;
		//	Best parameters are 0, 0.1, 0.2
		float alpha= m_alpha, beta = m_beta, gamma = m_gamma;
		//	Initial positions of the active contour points
		int k;
		for (k=0; k<(m_nPoints/4); k++)
		{
			snk[k].x = k*sampleEye->width/(m_nPoints/4);
			snk[k].y = 0;
		}
		for (k=(m_nPoints/4); k<(m_nPoints/2); k++)
		{
			snk[k].x = sampleEye->width;
			snk[k].y = (k-(m_nPoints/4))*sampleEye->height/(m_nPoints/4);;
		}
		for (k=(m_nPoints/2); k<(3*m_nPoints/4); k++)
		{
			snk[k].x = sampleEye->width - (k-(m_nPoints/2))*sampleEye->width/(m_nPoints/4);
			snk[k].y = sampleEye->height;
		}
		for (k=(3*m_nPoints/4); k<m_nPoints; k++)
		{
			snk[k].x = 0;
			snk[k].y = sampleEye->height - (k-(3*m_nPoints/4))*sampleEye->height/(m_nPoints/4);;
		}

		//	Active Contour is minimizing its energy
		cvSnakeImage( work2, snk, m_nPoints, &alpha, &beta, &gamma, CV_VALUE, win, criteria, 1);
		
		for (int n=0; n<m_nPoints; n++)
			cvCircle(sampleEye, cvPoint(snk[n].x, snk[n].y) , 3, CV_RGB(0,0,255), 1);
		
		// Fits ellipse to current contour.
		CvBox2D32f* box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
		CvPoint2D32f* snk2D32f = (CvPoint2D32f*)malloc(sizeof(CvPoint2D32f)*m_nPoints);
		for(int i=0; i<m_nPoints; i++)
        {
            snk2D32f[i].x = (float)snk[i].x;
            snk2D32f[i].y = (float)snk[i].y;
        }
                
        cvFitEllipse(snk2D32f, m_nPoints, box);
		// Convert ellipse data from float to integer representation.
        CvPoint center; CvSize size;
		center.x = cvRound(box->center.x);
        center.y = cvRound(box->center.y);
        size.width = cvRound(box->size.width*0.5);
        size.height = cvRound(box->size.height*0.5);
        box->angle = -box->angle; 
        // Draw ellipse.
       	cvEllipse(sampleEye, center, size, box->angle, 0, 360, CV_RGB(255,0,0), 1);
				
		cvResize(sampleEye, finalImg);
		cvShowImage("FNL", finalImg);

		free(snk);
		free(snk2D32f);
		free(box);

		cvReleaseImage(&sampleEye);
		cvReleaseImage(&work1);
		cvReleaseImage(&work2);
	}	
}

void Calibration4Snake::OnDelete() 
{
	if (MessageBox("Do you really want to delete the test file?", "Warning!", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		CFile::Remove("c:\\SampleEye.jpg");
		MessageBox("Test file deleted", "info", MB_OK|MB_ICONINFORMATION);	
	}
}
