// USBCam.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "USBCam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// USBCam dialog


USBCam::USBCam(CWnd* pParent /*=NULL*/)
	: CDialog(USBCam::IDD, pParent)
{
	numOfCameras = 0;
	//{{AFX_DATA_INIT(USBCam)
	m_cameraName = _T("");
	//}}AFX_DATA_INIT
}


void USBCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(USBCam)
	DDX_Text(pDX, IDC_EDIT1, m_cameraName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(USBCam, CDialog)
	//{{AFX_MSG_MAP(USBCam)
	ON_BN_CLICKED(IDC_BUTTON1, OnAdjust)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// USBCam message handlers

BOOL USBCam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//	**	Select Camera
	numOfCameras = cvcamGetCamerasCount( );
	
	if (numOfCameras != 0)
	{
		//	**	Display USB Camera Name
		CameraDescription ptr;
		cvcamGetProperty(0, CVCAM_DESCRIPTION, &ptr);
		m_cameraName = ptr.DeviceDescription;
	}
	else 
		m_cameraName = "No capture device available";
		
	UpdateData(false);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void USBCam::OnAdjust() 
{	
	cvcamSelectCamera(NULL);
	if (numOfCameras != 0)
	{
		cvcamInit();
		cvcamGetProperty(0, CVCAM_CAMERAPROPS, NULL);
	}
}

void USBCam::OnClose() 
{
	cvcamExit();	
	CDialog::OnClose();
}
