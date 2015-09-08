// ColorSelector.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEye.h"
#include "ColorSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColorSelector dialog


ColorSelector::ColorSelector(CWnd* pParent /*=NULL*/)
	: CDialog(ColorSelector::IDD, pParent)
{
	//{{AFX_DATA_INIT(ColorSelector)
	m_drawFaceBox = TRUE;
	m_drawEye = TRUE;
	m_drawEyeCircle = TRUE;
	m_drawPupil = TRUE;
	m_drawSnake = TRUE;
	m_drawEllipse = TRUE;
	m_RGB1 = _T("");
	m_RGB2 = _T("");
	m_RGB3 = _T("");
	m_RGB4 = _T("");
	m_RGB5 = _T("");
	m_RGB6 = _T("");
	//}}AFX_DATA_INIT
	colors[0] = CV_RGB(255,0,0);
	colors[1] = CV_RGB(255,0,0);
	colors[2] = CV_RGB(0,0,255);
	colors[3] = CV_RGB(0,255,0);
	colors[4] = CV_RGB(0,0,255);
	colors[5] = CV_RGB(255,0,0);

	colorCodes[0][0] = 255; colorCodes[0][1] = 0; colorCodes[0][2] = 0;
	colorCodes[1][0] = 255; colorCodes[1][1] = 0; colorCodes[1][2] = 0;
	colorCodes[2][0] = 0; colorCodes[2][1] = 0; colorCodes[2][2] = 255;
	colorCodes[3][0] = 0; colorCodes[3][1] = 255; colorCodes[3][2] = 0;
	colorCodes[4][0] = 0; colorCodes[4][1] = 0; colorCodes[4][2] = 255;
	colorCodes[5][0] = 255; colorCodes[5][1] = 0; colorCodes[5][2] = 0;
}


void ColorSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColorSelector)
	DDX_Check(pDX, IDC_CHECK1, m_drawFaceBox);
	DDX_Check(pDX, IDC_CHECK2, m_drawEye);
	DDX_Check(pDX, IDC_CHECK3, m_drawEyeCircle);
	DDX_Check(pDX, IDC_CHECK4, m_drawPupil);
	DDX_Text(pDX, IDC_RGB1, m_RGB1);
	DDX_Text(pDX, IDC_RGB2, m_RGB2);
	DDX_Text(pDX, IDC_RGB3, m_RGB3);
	DDX_Text(pDX, IDC_RGB4, m_RGB4);
	DDX_Check(pDX, IDC_CHECK8, m_drawSnake);
	DDX_Check(pDX, IDC_CHECK9, m_drawEllipse);
	DDX_Text(pDX, IDC_RGB5, m_RGB5);
	DDX_Text(pDX, IDC_RGB6, m_RGB6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ColorSelector, CDialog)
	//{{AFX_MSG_MAP(ColorSelector)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDOK, OnCloseColors)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColorSelector message handlers
BOOL ColorSelector::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	initializeStatics();

	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON1);
	temp->EnableWindow(m_drawFaceBox);
	temp = (CButton*) GetDlgItem(IDC_BUTTON2);
	temp->EnableWindow(m_drawEye);
	temp = (CButton*) GetDlgItem(IDC_BUTTON3);
	temp->EnableWindow(m_drawEyeCircle);
	temp = (CButton*) GetDlgItem(IDC_BUTTON4);
	temp->EnableWindow(m_drawPupil);
	temp = (CButton*) GetDlgItem(IDC_BUTTON5);
	temp->EnableWindow(m_drawSnake);
	temp = (CButton*) GetDlgItem(IDC_BUTTON6);
	temp->EnableWindow(m_drawEllipse);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ColorSelector::SelectColor(int* colors)
{
	CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};
	COLORREF myColor = RGB(255, 0, 0);
	COLORREF myCustoms[16] = {0};
	
	cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
	cc.hwndOwner = this->m_hWnd;
	cc.rgbResult = myColor;
	cc.lpCustColors = myCustoms;
	
	//	COLORREF format : 0x00bbggrr 
	if(ChooseColor(&cc))
	{
		myColor = cc.rgbResult;
		colors[0] = myColor&0x0000FF;			// red
		colors[1] = (myColor&0x00FF00)/256;		// green
		colors[2] = (myColor&0xFF0000)/65536;	// blue
	}
}


void ColorSelector::OnCheck1() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK1))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON1);
	temp->EnableWindow(state);
	m_drawFaceBox = state;
}

void ColorSelector::OnCheck2() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK2))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON2);
	temp->EnableWindow(state);	
	m_drawEye = state;
}

void ColorSelector::OnCheck3() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK3))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON3);
	temp->EnableWindow(state);	
	m_drawEyeCircle = state;
}

void ColorSelector::OnCheck4() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK4))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON4);
	temp->EnableWindow(state);
	m_drawPupil = state;
}


void ColorSelector::OnCheck8() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK8))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON5);
	temp->EnableWindow(state);
	m_drawSnake = state;
}

void ColorSelector::OnCheck9() 
{
	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK9))->GetCheck();
	CButton* temp = (CButton*) GetDlgItem(IDC_BUTTON6);
	temp->EnableWindow(state);
	m_drawEllipse = state;
}


void ColorSelector::OnButton1() 
{
	SelectColor(myRGB);
	colors[0] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[0][0] = myRGB[0]; colorCodes[0][1] = myRGB[1]; colorCodes[0][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::OnButton2() 
{
	SelectColor(myRGB);
	colors[1] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[1][0] = myRGB[0]; colorCodes[1][1] = myRGB[1]; colorCodes[1][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::OnButton3() 
{
	SelectColor(myRGB);
	colors[2] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[2][0] = myRGB[0]; colorCodes[2][1] = myRGB[1]; colorCodes[2][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::OnButton4() 
{
	SelectColor(myRGB);
	colors[3] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[3][0] = myRGB[0]; colorCodes[3][1] = myRGB[1]; colorCodes[3][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::OnButton5() 
{
	SelectColor(myRGB);
	colors[4] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[4][0] = myRGB[0]; colorCodes[4][1] = myRGB[1]; colorCodes[4][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::OnButton6() 
{
	SelectColor(myRGB);
	colors[5] = CV_RGB(myRGB[0], myRGB[1], myRGB[2]);
	colorCodes[5][0] = myRGB[0]; colorCodes[5][1] = myRGB[1]; colorCodes[5][2] = myRGB[2];
	initializeStatics();
}

void ColorSelector::initializeStatics()
{
	m_RGB1.Format("CV_RGB(%d, %d, %d)", colorCodes[0][0], colorCodes[0][1], colorCodes[0][2]);
	m_RGB2.Format("CV_RGB(%d, %d, %d)", colorCodes[1][0], colorCodes[1][1], colorCodes[1][2]);
	m_RGB3.Format("CV_RGB(%d, %d, %d)", colorCodes[2][0], colorCodes[2][1], colorCodes[2][2]);
	m_RGB4.Format("CV_RGB(%d, %d, %d)", colorCodes[3][0], colorCodes[3][1], colorCodes[3][2]);
	m_RGB5.Format("CV_RGB(%d, %d, %d)", colorCodes[4][0], colorCodes[4][1], colorCodes[4][2]);
	m_RGB6.Format("CV_RGB(%d, %d, %d)", colorCodes[5][0], colorCodes[5][1], colorCodes[5][2]);
	UpdateData(false);
}

void ColorSelector::OnCloseColors() 
{
	UpdateData(true);

	BOOL state = ((CButton*) GetDlgItem(IDC_CHECK5))->GetCheck();
	if (state == true)
	{
		colors[0] = CV_RGB(255,0,0);
		colors[1] = CV_RGB(255,0,0);
		colors[2] = CV_RGB(0,0,255);
		colors[3] = CV_RGB(0,255,0);
		colors[4] = CV_RGB(0,0,255);
		colors[5] = CV_RGB(255,0,0);

		colorCodes[0][0] = 255; colorCodes[0][1] = 0; colorCodes[0][2] = 0;
		colorCodes[1][0] = 255; colorCodes[1][1] = 0; colorCodes[1][2] = 0;
		colorCodes[2][0] = 0; colorCodes[2][1] = 0; colorCodes[2][2] = 255;
		colorCodes[3][0] = 0; colorCodes[3][1] = 255; colorCodes[3][2] = 0;
		colorCodes[4][0] = 0; colorCodes[4][1] = 0; colorCodes[4][2] = 255;
		colorCodes[5][0] = 255; colorCodes[5][1] = 0; colorCodes[5][2] = 0;

		m_drawFaceBox = true;
		m_drawEye = true;
		m_drawEyeCircle = true;
		m_drawPupil = true;
		m_drawSnake = true;
		m_drawEllipse = true;
	}
	
	if (m_drawFaceBox == false)
		colors[0] = -1;
	if (m_drawEye == false)
			colors[1] = -1;
	if (m_drawEyeCircle == false)
			colors[2] = -1;
	if (m_drawPupil == false)
			colors[3] = -1;
	if (m_drawSnake == false)
			colors[4] = -1;
	if (m_drawEllipse == false)
			colors[5] = -1;

	EndDialog(1);
}
