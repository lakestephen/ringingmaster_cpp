// SimulatorImagesPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorImagesPreviewDlg.h"
#include "Tower.h"
#include ".\simulatorimagespreviewdlg.h"


// SimulatorImagesPreviewDlg dialog

IMPLEMENT_DYNAMIC(SimulatorImagesPreviewDlg, CDialog)
SimulatorImagesPreviewDlg::SimulatorImagesPreviewDlg(CWnd* pParent, const Tower& tower)
	: CDialog(SimulatorImagesPreviewDlg::IDD, pParent), 
	_internalTower(tower)
{
}

SimulatorImagesPreviewDlg::~SimulatorImagesPreviewDlg()
{
}

void SimulatorImagesPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BACKLEFT, _backLeft);
	DDX_Control(pDX, IDC_HANDLEFT, _handLeft);
	DDX_Control(pDX, IDC_BACKRIGHT, _backRight);
	DDX_Control(pDX, IDC_HANDRIGHT, _handRight);

}


BEGIN_MESSAGE_MAP(SimulatorImagesPreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BACKLEFT, OnBnClickedBackleft)
	ON_BN_CLICKED(IDC_HANDLEFT, OnBnClickedHandleft)
	ON_BN_CLICKED(IDC_BACKRIGHT, OnBnClickedBackright)
	ON_BN_CLICKED(IDC_HANDRIGHT, OnBnClickedHandright)
END_MESSAGE_MAP()


// SimulatorImagesPreviewDlg message handlers

BOOL SimulatorImagesPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_path = _internalTower.getPicturePath(spt_back_left);
	loadImage();
	_backLeft.SetCheck(TRUE)  ;

	_backLeft.m_bDrawFocus = FALSE;
	_backLeft.SetTooltip("Show the backstroke left position.");
	_backLeft.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_backLeft.SetImage(IDB_TOPLEFT);

	_handLeft.m_bDrawFocus = FALSE;
	_handLeft.SetTooltip("Show the handstroke left position.");
	_handLeft.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_handLeft.SetImage(IDB_BOTTOMLEFT);

	_backRight.m_bDrawFocus = FALSE;
	_backRight.SetTooltip("Show the backstroke right position.");
	_backRight.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_backRight.SetImage(IDB_TOPRIGHT);

	_handRight.m_bDrawFocus = FALSE;
	_handRight.SetTooltip("Show the handstroke right position.");
	_handRight.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_handRight.SetImage(IDB_BOTTOMRIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SimulatorImagesPreviewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(5,5);
	rect.top += 40;
	
	CRect rectPath(rect);

	rect.bottom -= 20;
	rectPath.top = rect.bottom + 5;

	_image.Draw(dc.m_hDC, rect);

	dc.Draw3dRect (rect, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DSHADOW));
	CFont* oldFont = dc.SelectObject(&app()->_fontStd);
	dc.SetBkMode (TRANSPARENT);
	dc.DrawText(_path,rectPath, DT_LEFT|DT_VCENTER| DT_NOPREFIX | DT_PATH_ELLIPSIS );
	dc.SelectObject(oldFont);
}

void SimulatorImagesPreviewDlg::OnBnClickedBackleft()
{
	_path = _internalTower.getPicturePath(spt_back_left);
	loadImage();
}

void SimulatorImagesPreviewDlg::OnBnClickedHandleft()
{
	_path = _internalTower.getPicturePath(spt_hand_left);
	loadImage();
}

void SimulatorImagesPreviewDlg::OnBnClickedBackright()
{
	_path = _internalTower.getPicturePath(spt_back_right);
	loadImage();
}

void SimulatorImagesPreviewDlg::OnBnClickedHandright()
{
	_path = _internalTower.getPicturePath(spt_hand_right);
	loadImage();
}

void SimulatorImagesPreviewDlg::loadImage()
{
	_image.Clear();
	_image.Load(_path);
		CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(5,5);
	rect.top += 40;

	InvalidateRect(rect);
}
