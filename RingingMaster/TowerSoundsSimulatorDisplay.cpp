// TowerSoundsSimulatorDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsSimulatorDisplay.h"
#include "TowerTTSItemDlg.h"
#include "Tower.h"
#include "PropItem.h"
#include "SpeechManager.h"
#include "SimulatorImagesPreviewDlg.h"
#include ".\towersoundssimulatordisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsSimulatorDisplay dialog


TowerSoundsSimulatorDisplay::TowerSoundsSimulatorDisplay( Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsSimulatorDisplay::IDD),
	_internalTower(internalTower)
	, _baseDir(_T(""))
	, _clockwise(FALSE)
	, _firstSimulatedBell(1)
{

	_baseDir = _internalTower.getPictureBaseDir();
	_clockwise = _internalTower.getClockwise();
	_firstSimulatedBell = _internalTower.getFirstSimulatedBell() +1;

	//{{AFX_DATA_INIT(TowerSoundsSimulatorDisplay)
	//}}AFX_DATA_INIT
}


void TowerSoundsSimulatorDisplay::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIRECTORY, _directoryButt);
	DDX_Control(pDX, IDC_BELL_NO_FONT, _bellNoFontButt);
	DDX_Control(pDX, IDC_PREVIEW, _previewButt);
	DDX_Text(pDX, IDC_FILE_NAME, _baseDir);
	DDX_Check(pDX, IDC_CLOCKWISE, _clockwise);
	DDX_Text(pDX, IDC_FIRST_SIMULATED_BELL, _firstSimulatedBell);
	DDV_MinMaxInt(pDX, _firstSimulatedBell, 1, _internalTower.getNumber());
	DDX_Control(pDX, IDC_FIRST_SIMULATED_BELL_SPIN, _firstSimulatedBellSpin);
}


BEGIN_MESSAGE_MAP(TowerSoundsSimulatorDisplay, CBCGPPropertyPage)

	ON_BN_CLICKED(IDC_BELL_NO_FONT, OnBnClickedBellNoFont)
	ON_BN_CLICKED(IDC_DIRECTORY, OnBnClickedDirectory)
	ON_BN_CLICKED(IDC_PREVIEW, OnBnClickedPreview)
	ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsSimulatorDisplay message handlers

BOOL TowerSoundsSimulatorDisplay::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_bellNoFontButt.SetImage(IDB_FONT);
	_bellNoFontButt.m_bDrawFocus = FALSE;
	_bellNoFontButt.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_previewButt.SetImage(IDB_PICTURE);
	_previewButt.m_bDrawFocus = FALSE;
	_previewButt.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_previewButt.SetTooltip("Preview selected image.");

	_directoryButt.SetImage(IDB_DOT_DOT_DOT);
	_directoryButt.m_bDrawFocus = FALSE;
	_directoryButt.SetTooltip("Edit the image root directory path.");
	_directoryButt.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void TowerSoundsSimulatorDisplay::update()
{
	_bellNoFontButt.SetTooltip("Edit Bell Numbers Font (" + 
		_internalTower.getBellFontInfo().getDisplayName() + ")");

	
	_firstSimulatedBellSpin.SetRange(1, (short)_internalTower.getNumber());   
}

void TowerSoundsSimulatorDisplay::OnBnClickedBellNoFont()
{
	_internalTower.editBellFont(this);
	update();
}

int CALLBACK OptionsWorkingDirCallbackProc(HWND hwnd,
								UINT uMsg,
								LPARAM /*lParam*/,
								LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);

	return 0;
}

void TowerSoundsSimulatorDisplay::OnBnClickedDirectory()
{
	UpdateData();
				  
	CString dir = _baseDir;
	while((dir.GetLength() > 0)&&
		  (dir.GetAt(dir.GetLength()-1) == '\\'))
	{
		dir.Delete(dir.GetLength()-1,1);
	}

	BROWSEINFO bi;
	memset((LPVOID)&bi, 0, sizeof(bi));
	TCHAR szDisplayName[_MAX_PATH];
	szDisplayName[0] = '\0';
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = "Please choose a default directory.";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
	bi.lpfn = OptionsWorkingDirCallbackProc;
	bi.lParam = reinterpret_cast<LPARAM>(dir.GetBuffer(0));

	LPITEMIDLIST pIIL = ::SHBrowseForFolder(&bi);

	TCHAR szInitialDir[_MAX_PATH];
	
	BOOL bRet = ::SHGetPathFromIDList(pIIL, (char*)&szInitialDir);
	
	dir = szInitialDir;
	if (!dir.IsEmpty())
	{
		_baseDir = dir + "\\";
		UpdateData(FALSE);
	}

	if (bRet)
	{
		LPMALLOC pMalloc;
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pIIL);
		pMalloc->Release();	 
	}				
}



void TowerSoundsSimulatorDisplay::OnBnClickedPreview()
{
	SimulatorImagesPreviewDlg dlg(this, _internalTower);

	dlg.DoModal();
}

void TowerSoundsSimulatorDisplay::OnOK()
{
	UpdateData();

	CString dir = _baseDir;
	while((dir.GetLength() > 0)&&
		  (dir.GetAt(dir.GetLength()-1) != '\\'))
	{
		dir += "\\";
	}

	_internalTower.setPictureBaseDir(dir);
	_internalTower.setClockwise(_clockwise);
	_internalTower.setFirstSimulatedBell(_firstSimulatedBell-1);


	CBCGPPropertyPage::OnOK();
}

LRESULT TowerSoundsSimulatorDisplay::OnQuerySiblings( WPARAM wParam, LPARAM /*lParam */)
{
	if (wParam == TOWER_SOUNDS_UPDATE)
		update();

	return 0;
}

BOOL TowerSoundsSimulatorDisplay::OnSetActive()
{
	update();

	return CBCGPPropertyPage::OnSetActive();
}
