// TrackingSettings.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "TrackingSettings.h"
#include "TrackingParameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CvPoint point11; CvPoint point22;
IplImage* videoFrame;

void on_mouseClick2(int event, int x, int y, int flags)
{
	CString dosya = "C:\\SampleHUE.jpg";
 	switch(event)
    {
 		case CV_EVENT_LBUTTONDOWN:
			point11.x = x;
			point11.y = y;
			break;
 		
		case CV_EVENT_LBUTTONUP:
			point22.x = x;
			point22.y = y;

			//cvRectangle(videoFrame, point11, point22, CV_RGB(255,0,0), 5);
			cvSetImageROI(videoFrame, cvRect (point11.x, point11.y, abs(point22.x-point11.x), abs(point22.y-point11.y)));
			cvSaveImage(dosya, videoFrame);
			cvResetImageROI(videoFrame);
			//cvShowImage("Define HUE", videoFrame);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// TrackingSettings dialog


TrackingSettings::TrackingSettings(CWnd* pParent /*=NULL*/)
	: CDialog(TrackingSettings::IDD, pParent)
{
	params			= new TrackingParameters;
	cameraControl	= 0;
	videoFile = "";
	//m_Check1.SetCheck(0);
	//{{AFX_DATA_INIT(TrackingSettings)
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);
	Path += "EyeDatabase";
	m_DBdirectory = Path;
	m_NeigenEyes = 0;
	m_NImages = 0;
	m_HUEw = 40;
	m_HUEh = 40;
	m_MaxError = 0;
	m_Ratio = 0.0f;
	m_Component = 3;
	m_variance = 30;
	m_eyeAlgorithm = 0;
	m_faceAlgorithm = 0;
	m_source = 1;
	//}}AFX_DATA_INIT
}


void TrackingSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TrackingSettings)
	DDX_Control(pDX, IDOK, m_saveAndClose);
	DDX_Control(pDX, IDC_SnakeSettings, m_SnakeSettings);
	DDX_Control(pDX, IDC_CHOOSEDIRECTORY, m_ChooseDirectory);
	DDX_Control(pDX, IDC_ThresholdSettings, m_ThresholdSettings);
	DDX_Control(pDX, IDC_COMBOColorspace, m_Colorspace);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_DBdirectory, m_DBdirectory);
	DDX_Text(pDX, IDC_NeigenEyes, m_NeigenEyes);
	DDV_MinMaxInt(pDX, m_NeigenEyes, 1, 15);
	DDX_Text(pDX, IDC_NImages, m_NImages);
	DDV_MinMaxInt(pDX, m_NImages, 1, 15);
	DDX_Text(pDX, IDC_EDIT2, m_HUEw);
	DDX_Text(pDX, IDC_EDIT3, m_HUEh);
	DDX_Text(pDX, IDC_MaxError, m_MaxError);
	DDX_Text(pDX, IDC_RatioEdit, m_Ratio);
	DDV_MinMaxFloat(pDX, m_Ratio, 0.f, 1.f);
	DDX_Text(pDX, IDC_ComponentEdit, m_Component);
	DDV_MinMaxInt(pDX, m_Component, 1, 3);
	DDX_Text(pDX, IDC_VARIANCEEDIT, m_variance);
	DDV_MinMaxInt(pDX, m_variance, 0, 1000);
	DDX_Radio(pDX, IDC_RADIO_EYE, m_eyeAlgorithm);
	DDX_Radio(pDX, IDC_RADIO_FACE, m_faceAlgorithm);
	DDX_Radio(pDX, IDC_RADIOSOURCE, m_source);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TrackingSettings, CDialog)
	//{{AFX_MSG_MAP(TrackingSettings)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDOK, OnSaveAndClose)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_BN_CLICKED(IDC_BUTTON1, OnDefineHUE)
	ON_BN_CLICKED(IDC_UpdateRectangle, OnUpdateRectangle)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SaveHUE, OnSaveHUE)
	ON_BN_CLICKED(IDC_ThresholdSettings, OnThresholdSettings)
	ON_BN_CLICKED(IDC_CHOOSEDIRECTORY, OnChoosedirectory)
	ON_BN_CLICKED(IDC_SnakeSettings, OnSnakeSettings)
	ON_BN_CLICKED(IDC_CHECK4, OnVarianceCheck)
	ON_BN_CLICKED(IDC_DefineHUE, OnDefineHUE)
	ON_BN_CLICKED(IDC_SELECTFILE, OnSelectfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrackingSettings message handlers

BOOL TrackingSettings::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	// TODO: Add extra initialization here
	//	GUI initializations first
	m_ThresholdSettings.SetIconID(IDI_EYE1);
	m_SnakeSettings.SetIconID(IDI_EYE2);
	m_ChooseDirectory.SetIconID(IDI_EXPLORE);
	m_saveAndClose.SetIconID(IDI_OK);

	CEdit* temp1 = (CEdit*) GetDlgItem(IDC_NImages);
	m_spin1.SetBuddy(temp1);
	m_spin1.SetRange(0, 15);
	
	CEdit* temp2 = (CEdit*) GetDlgItem(IDC_NeigenEyes);
	m_spin2.SetBuddy(temp2);
	m_spin2.SetRange(0, 15);
	
	//	**	Get Previous Settings
	CButton* temp;

	if (params->trackEyes == 1)
	{
		temp = (CButton*) GetDlgItem(IDC_CHECK1); temp->SetCheck(true);
		
		temp = (CButton*) GetDlgItem(IDC_CHECK4); temp->EnableWindow(true); 
		if (params->varianceCheck == 1)
		{
			temp->SetCheck(true);
			temp = (CButton*) GetDlgItem(IDC_VARIANCETEXT); temp->EnableWindow(true);
			temp = (CButton*) GetDlgItem(IDC_VARIANCEEDIT); temp->EnableWindow(true);
		}
		else
		{
			temp->SetCheck(false);
			temp = (CButton*) GetDlgItem(IDC_VARIANCETEXT); temp->EnableWindow(false);
			temp = (CButton*) GetDlgItem(IDC_VARIANCEEDIT); temp->EnableWindow(false);
		}
		
		temp = (CButton*) GetDlgItem(IDC_EyeTrackingFrame); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_NImagesText); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_NEigensText); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_EyeDatabaseText); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_NImages); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_NeigenEyes); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_DBdirectory); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_CHOOSEDIRECTORY); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_Error); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_MaxError); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_RatioText); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_RatioEdit); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_CHECK2); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_colorspacetext); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_COMBOColorspace); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_ComponentText); temp->EnableWindow(true);
		temp = (CButton*) GetDlgItem(IDC_ComponentEdit); temp->EnableWindow(true);
		
		if (params->detailtrack == 1) // ** i.e Track each eye
		{
			temp = (CButton*) GetDlgItem(IDC_CHECK2);
			temp->SetCheck(true);
			temp = (CButton*) GetDlgItem(IDC_EYEFRAME); temp->EnableWindow(true); 
			temp = (CButton*) GetDlgItem(IDC_CHECK3); temp->EnableWindow(true); 
			temp->SetCheck(params->snake);
			temp = (CButton*) GetDlgItem(IDC_CHECK5); temp->EnableWindow(true); 
			temp->SetCheck(params->fitellipse);
			temp = (CButton*) GetDlgItem(IDC_CHECK6); temp->EnableWindow(true); 
			temp->SetCheck(params->pupil);
			temp = (CButton*) GetDlgItem(IDC_CHECK7); temp->EnableWindow(true); 
			temp->SetCheck(params->blink);
			temp = (CButton*) GetDlgItem(IDC_ThresholdSettings); temp->EnableWindow(true); 	
			temp = (CButton*) GetDlgItem(IDC_SnakeSettings); temp->EnableWindow(true); 	
		}
	
		CSpinButtonCtrl* spin;
		spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1); spin->EnableWindow(true);
		spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN2); spin->EnableWindow(true);
	}	
	
	m_NImages = params->nImages;
	m_NeigenEyes = params->nEigens;
	m_DBdirectory = params->DBdirectory;
	m_MaxError = params->MaxError;
	m_Ratio = params->ratio;
	m_variance = params->variance;
	
	if (params->colorSpace == CV_RGB2GRAY)
		m_Colorspace.SetCurSel(0);
	else if (params->colorSpace == CV_RGB2XYZ)
		m_Colorspace.SetCurSel(1);
	else if (params->colorSpace == CV_RGB2YCrCb)
		m_Colorspace.SetCurSel(2);
	else if (params->colorSpace == CV_RGB2HSV)
		m_Colorspace.SetCurSel(3);
	else if (params->colorSpace == CV_RGB2Lab)
		m_Colorspace.SetCurSel(4);
	
	m_Component = params->component;


	CComboBox* tempCombo = (CComboBox*) GetDlgItem(IDC_TEMPMATCHING);
	if (params->tempMatch == CV_TM_SQDIFF)
		tempCombo->SetCurSel(0);
	else if (params->tempMatch == CV_TM_SQDIFF_NORMED)
		tempCombo->SetCurSel(1);
	else if (params->tempMatch == CV_TM_CCORR)
		tempCombo->SetCurSel(2);
	else if (params->tempMatch == CV_TM_CCORR_NORMED)
		tempCombo->SetCurSel(3);
	else if (params->tempMatch == CV_TM_CCOEFF)
		tempCombo->SetCurSel(4);
	else if (params->tempMatch == CV_TM_CCOEFF_NORMED)
		tempCombo->SetCurSel(5);
		
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void TrackingSettings::OnCheck1() 
{
	//	**	Get current status
	BOOL state = m_Check1.GetCheck();
	CButton* deneme = (CButton*) GetDlgItem(IDC_CHECK1);

	CButton* temp;
	temp = (CButton*) GetDlgItem(IDC_EyeTrackingFrame); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHECK4); temp->EnableWindow(state); temp->SetCheck(true);
	temp = (CButton*) GetDlgItem(IDC_VARIANCETEXT); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_VARIANCEEDIT); temp->EnableWindow(state);
	
	temp = (CButton*) GetDlgItem(IDC_NImagesText); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_NEigensText); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_EyeDatabaseText); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_NImages); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_NeigenEyes); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_DBdirectory); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHOOSEDIRECTORY); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_Error); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_MaxError); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_RatioText); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_RatioEdit); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_colorspacetext); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_COMBOColorspace); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_ComponentText); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_ComponentEdit); temp->EnableWindow(state);

	CSpinButtonCtrl* spin;
	spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1); spin->EnableWindow(state);
	spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN2); spin->EnableWindow(state);

	//	**	These are related with individual eyes
	temp = (CButton*) GetDlgItem(IDC_CHECK2); temp->EnableWindow(state);
	if (state == false)
	{
		temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_EYEFRAME); temp->EnableWindow(false); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK3); temp->EnableWindow(false); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK5); temp->EnableWindow(false); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK6); temp->EnableWindow(false); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK7); temp->EnableWindow(false); temp->SetCheck(false); 
		temp = (CButton*) GetDlgItem(IDC_ThresholdSettings); temp->EnableWindow(false);
		temp = (CButton*) GetDlgItem(IDC_SnakeSettings); temp->EnableWindow(false);
		spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN1); spin->EnableWindow(false);
		spin = (CSpinButtonCtrl*) GetDlgItem(IDC_SPIN2); spin->EnableWindow(false);
	}	
}

void TrackingSettings::OnCheck2() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK2))->GetCheck();
	CButton* temp;

	temp = (CButton*) GetDlgItem(IDC_EYEFRAME); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHECK3); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHECK5); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHECK6); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_CHECK7); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_ThresholdSettings); temp->EnableWindow(state); 	
	temp = (CButton*) GetDlgItem(IDC_SnakeSettings); temp->EnableWindow(state); 	

	if (state == false)
	{
		temp = (CButton*) GetDlgItem(IDC_CHECK3); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK5); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK6); temp->SetCheck(false);
		temp = (CButton*) GetDlgItem(IDC_CHECK7); temp->SetCheck(false);
	}
}

void TrackingSettings::OnSaveAndClose() 
{
	UpdateData(true);
	int state = m_Check1.GetCheck();
	if (state == 0)
		params->trackEyes = 0;
	else
	{
		params->trackEyes = 1;
		
		CButton* temp = (CButton*) GetDlgItem(IDC_CHECK4);
		params->varianceCheck = temp->GetCheck();
		params->variance = m_variance;
		
		params->nImages = m_NImages;
		params->nEigens = m_NeigenEyes;
		params->DBdirectory.Empty(); 
		params->DBdirectory.Format("%s",m_DBdirectory);
		params->MaxError = m_MaxError;
		params->ratio = m_Ratio;

		int index = m_Colorspace.GetCurSel();
		
		if (index == 0)
			params->colorSpace = CV_RGB2GRAY;
		else if (index == 1)
			params->colorSpace = CV_RGB2XYZ;
		else if (index == 2)
			params->colorSpace = CV_RGB2YCrCb;
		else if (index == 3 )
			params->colorSpace = CV_RGB2HSV;
		else if (index == 4)
			params->colorSpace = CV_RGB2Lab;
		
		params->component = m_Component;
		
		temp = (CButton*) GetDlgItem(IDC_CHECK2);
		if (temp->GetCheck() == false)
		{
			params->detailtrack = 0; 
			params->pupil = 0;
			params->snake = 0; 
			params->fitellipse = 0;
			params->blink = 0;
		}
		else
		{
			params->detailtrack = 1;
			temp = (CButton*) GetDlgItem(IDC_CHECK3);
			params->snake = (temp->GetCheck() == 0 ? 0 : 1);
			temp = (CButton*) GetDlgItem(IDC_CHECK5);
			params->fitellipse = (temp->GetCheck() == 0 ? 0 : 1);
			temp = (CButton*) GetDlgItem(IDC_CHECK6);
			params->pupil = (temp->GetCheck() == 0 ? 0 : 1);
			temp = (CButton*) GetDlgItem(IDC_CHECK7);
			params->blink = (temp->GetCheck() == 0 ? 0 : 1);
		}

		CComboBox* tempCombo = (CComboBox*) GetDlgItem(IDC_TEMPMATCHING);
		int index2 = tempCombo->GetCurSel();

		if (index2 == 0)
			params->tempMatch = CV_TM_SQDIFF;
		else if (index2 == 1)
			params->tempMatch = CV_TM_SQDIFF_NORMED;
		else if (index2 == 2)
			params->tempMatch = CV_TM_CCORR;
		else if (index2 == 3 )
			params->tempMatch = CV_TM_CCORR_NORMED;
		else if (index2 == 4)
			params->tempMatch = CV_TM_CCOEFF;
		else if (index2 == 5)
			params->tempMatch = CV_TM_CCOEFF_NORMED;
	}

	if (m_source == 1 && videoFile == "")
		MessageBox("You have forgotten to select an AVI file!\nHint : Select an AVI file or change input to WebCam", "Input Source Error", MB_OK|MB_ICONSTOP);
	else
		CDialog::OnOK();
}

void TrackingSettings::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_NImages = m_spin1.GetPos() - 65535;

	UpdateData(false);
	
	*pResult = 0;
}

void TrackingSettings::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_NeigenEyes = m_spin2.GetPos() - 65535;
	UpdateData(false);

	*pResult = 0;
}


void TrackingSettings::OnDefineHUE() 
{
	UpdateData(true);

	if (m_source == 1)
	{		
		char* videoFile_char = new char[200];
		sprintf(videoFile_char, "%s", videoFile);
		Camera = cvCaptureFromAVI(videoFile_char);
		free(videoFile_char);

		cvNamedWindow("Define HUE", -1);
		cvSetMouseCallback("Define HUE", on_mouseClick2);
		CButton* temp = (CButton*) GetDlgItem(IDC_SaveHUE); 
		temp->EnableWindow(true);

		while (cameraControl != 2)
		{
			videoFrame = cvQueryFrame(Camera);
			if (videoFrame==NULL)
				break;
			cvShowImage("Define HUE", videoFrame);
			cvWaitKey(1000);

		}
		cvReleaseCapture(&Camera);
	}

	else
	{
			//	**	Get current status
		CButton* temp;
		temp = (CButton*) GetDlgItem(IDC_UpdateRectangle);
		BOOL state = temp->IsWindowEnabled();
		
		temp->EnableWindow(!state);

		temp = (CButton*) GetDlgItem(IDC_SaveHUE); temp->EnableWindow(!state);
		temp = (CButton*) GetDlgItem(IDC_HUEw); temp->EnableWindow(!state);
		temp = (CButton*) GetDlgItem(IDC_HUEh); temp->EnableWindow(!state);
		temp = (CButton*) GetDlgItem(IDC_EDIT2); temp->EnableWindow(!state);
		temp = (CButton*) GetDlgItem(IDC_EDIT3); temp->EnableWindow(!state);

		if (state == false)	//	**	User wants to sample HUE
		{
			temp = (CButton*) GetDlgItem(IDC_DefineHUE); temp->EnableWindow(false);
			Camera = cvCaptureFromCAM(-1); // Initialized
			if (Camera == NULL)
			{
				MessageBox("Can not initialize capture device", "Error", MB_OK|MB_ICONSTOP);
				temp = (CButton*) GetDlgItem(IDC_SaveHUE); temp->EnableWindow(false);
				temp = (CButton*) GetDlgItem(IDC_HUEw); temp->EnableWindow(false);
				temp = (CButton*) GetDlgItem(IDC_HUEh); temp->EnableWindow(false);
				temp = (CButton*) GetDlgItem(IDC_EDIT2); temp->EnableWindow(false);
				temp = (CButton*) GetDlgItem(IDC_EDIT3); temp->EnableWindow(false);
				temp = (CButton*) GetDlgItem(IDC_DefineHUE); temp->EnableWindow(true);
			}
			else 
			{
				cameraControl = 1;

				CvSize dimOfFrame;
				
				CameraFrame = cvQueryFrame(Camera);
				dimOfFrame = cvGetSize(CameraFrame);
				cvNamedWindow("Define HUE", -1);
				
				//	Camera is initialized and frames are ready to be shown
				while (cameraControl != 2)
				{
					//	**	Update point1 & point2
					dimOfFrame = cvGetSize(CameraFrame);
					point1.x = (dimOfFrame.width/2.0)-m_HUEw/2;
					point1.y = (dimOfFrame.height/2.0)-m_HUEh/2;
					point2.x = (dimOfFrame.width/2.0)+m_HUEw/2;
					point2.y = (dimOfFrame.height/2.0)+m_HUEh/2;
				
					cvRectangle( CameraFrame, point1, point2, CV_RGB(0,0,255), 2 );
					
					cvShowImage("Define HUE", CameraFrame);
					if (cvWaitKey(10) != -1) // -1 means NOT PRESSED
						cameraControl++;
					CameraFrame = cvQueryFrame(Camera);
				}
				cvReleaseCapture(&Camera);
			}
		}
	}
}

void TrackingSettings::OnUpdateRectangle() 
{
	UpdateData(true);	//	**	m_HUEw & m_HUEh variables are updated
}

void TrackingSettings::OnClose() 
{
	if (cameraControl != 0)
	{
		cameraControl = 2;
		cvReleaseCapture(&Camera);	
	}
	CDialog::OnClose();
}

void TrackingSettings::OnSaveHUE() 
{
	CString dosya = "C:\\SampleHUE.jpg";
	
	cameraControl = 2;
	cvDestroyWindow("Define HUE");
	if (m_source == 0) // Webcam
	{
		CameraFrame = cvQueryFrame(Camera);
		cvSetImageROI(CameraFrame, cvRect (point1.x, point1.y, m_HUEw, m_HUEh));
		cvSaveImage(dosya, CameraFrame);
		cvResetImageROI(CameraFrame);
		cvReleaseCapture(&Camera);
	}

	CButton* temp;
	temp = (CButton*) GetDlgItem(IDC_DefineHUE); temp->EnableWindow(true);
	temp = (CButton*) GetDlgItem(IDC_SaveHUE); temp->EnableWindow(false);
	temp = (CButton*) GetDlgItem(IDC_HUEw); temp->EnableWindow(false);
	temp = (CButton*) GetDlgItem(IDC_HUEh); temp->EnableWindow(false);
	temp = (CButton*) GetDlgItem(IDC_EDIT2); temp->EnableWindow(false);
	temp = (CButton*) GetDlgItem(IDC_EDIT3); temp->EnableWindow(false);
	temp = (CButton*) GetDlgItem(IDC_UpdateRectangle); temp->EnableWindow(false);
}

void TrackingSettings::OnThresholdSettings() 
{
	this->params->pupilCalib->DoModal();	
}

bool TrackingSettings::ChooseFolder(char *folderName)
{
	BROWSEINFO bi;
	ITEMIDLIST* pItemIDList;
	
	char folder[_MAX_PATH];
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = folder;
	bi.lpszTitle = "Choose Eyedatabase folder";
	
	if((pItemIDList = SHBrowseForFolder(&bi))!=NULL) 
	{
		SHGetPathFromIDList(pItemIDList, folderName);
		return true;
	}
	else
		return false;
}

void TrackingSettings::OnChoosedirectory() 
{
	char folder[_MAX_PATH]	;
	if (ChooseFolder(folder) == true)
	{
		m_DBdirectory.Format("%s", folder);
		UpdateData(false);
	}
}

void TrackingSettings::OnSnakeSettings() 
{
	// TODO: Add your control notification handler code here
	this->params->snakeCalib->DoModal();
}

void TrackingSettings::OnVarianceCheck() 
{
	//	**	Get current status
	CButton* temp = (CButton*) GetDlgItem(IDC_CHECK4);
	BOOL state = temp->GetCheck();

	temp = (CButton*) GetDlgItem(IDC_VARIANCETEXT); temp->EnableWindow(state);
	temp = (CButton*) GetDlgItem(IDC_VARIANCEEDIT); temp->EnableWindow(state);
}

void TrackingSettings::OnSelectfile() 
{		
	CFileDialog SelectFile(TRUE, _T("*.avi"), NULL, 
		OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
		"AVI files (*.avi)|*.avi||", // Bunu anlayamadým
		NULL);
	SelectFile.m_ofn.lpstrTitle = "Open Avi file";
	
	CString Path(*__argv);
	int i = Path.ReverseFind('\\')+1;
	if(i) 
		Path=Path.Left(i);

	SelectFile.m_ofn.lpstrInitialDir = Path;

	if (SelectFile.DoModal() == IDOK)
		videoFile = SelectFile.GetPathName();
}
