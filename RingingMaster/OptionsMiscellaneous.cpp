// OptionsMiscellaneous.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsMiscellaneous.h"
#include ".\optionsmiscellaneous.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsMiscellaneous dialog


OptionsMiscellaneous::OptionsMiscellaneous()
	: CBCGPPropertyPage(OptionsMiscellaneous::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsMiscellaneous)
	_autoOpen = TRUE;
	_workingDir = _T("");
	_propertyHint = FALSE;
	_useWorkingDir = TRUE;
	//}}AFX_DATA_INIT
}

void OptionsMiscellaneous::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsMiscellaneous)
	DDX_Control(pDX, IDC_WORKING_DIR_BUTT, _workingDirBut);
	DDX_Check(pDX, IDC_OPTIONS_MICL_AUTO_OPEN, _autoOpen);
	DDX_Text(pDX, IDC_WORKING_DIR, _workingDir);
	DDX_Check(pDX, IDC_PROPERTY_HINT, _propertyHint);
	DDX_Check(pDX, IDC_USE_WORKING_DIR, _useWorkingDir);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WORKING_DIR, _workingDirCtrl);
}


BEGIN_MESSAGE_MAP(OptionsMiscellaneous, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsMiscellaneous)
	ON_BN_CLICKED(IDC_WORKING_DIR_BUTT, OnWorkingDirButt)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USE_WORKING_DIR, OnBnClickedUseWorkingDir)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsMiscellaneous message handlers

void OptionsMiscellaneous::OnOK() 
{
	UpdateData();
	//check a valid file	   

	CString dir = _workingDir;
	while((dir.GetLength() > 0)&&
		  (dir.GetAt(dir.GetLength()-1) != '\\'))
	{
		dir += "\\";
	}

	CFileFind fileFind;
	if (!fileFind.FindFile(dir + "."))
	{
		AfxMessageBox("The working directory is not valid");
		return;
	}
	
	app()->WriteProfileString("MainFrame", 
								"Working Directory", dir);	

	app()->WriteProfileInt("MainFrame", 
								"Auto Open Last Project", _autoOpen);	

	app()->WriteProfileInt("MainFrame", 
								"Property Window Hint", _propertyHint);	

	app()->WriteProfileInt("MainFrame", 
								"Use Working Directory", _useWorkingDir);	

	CBCGPPropertyPage::OnOK();
}

BOOL OptionsMiscellaneous::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_autoOpen = app()->GetProfileInt("MainFrame", 
									"Auto Open Last Project", 1);

	_propertyHint= app()->GetProfileInt("MainFrame", 
									"Property Window Hint", TRUE);

	_useWorkingDir = app()->GetProfileInt("MainFrame", 
									"Use Working Directory", TRUE);

	_workingDir = app()->getDocumentDirectory(true);
	
	UpdateData(FALSE);
	
	OnBnClickedUseWorkingDir();

	_workingDirBut.SetImage(IDB_DOT_DOT_DOT);
	_workingDirBut.m_bDrawFocus = FALSE;
	_workingDirBut.SetTooltip("Find Directory");
	_workingDirBut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	return TRUE;  // return TRUE unless you set the focus to a control

}

int CALLBACK OptionsMiscellaneousBrowseCallbackProc(HWND hwnd,
								UINT uMsg,
								LPARAM /*lParam*/,
								LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);

	return 0;
}

void OptionsMiscellaneous::OnWorkingDirButt() 
{
	UpdateData();
				  
	CString dir = _workingDir;
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
	bi.lpfn = OptionsMiscellaneousBrowseCallbackProc;
	bi.lParam = reinterpret_cast<LPARAM>(dir.GetBuffer(0));

	LPITEMIDLIST pIIL = ::SHBrowseForFolder(&bi);

	TCHAR szInitialDir[_MAX_PATH];
	
	BOOL bRet = ::SHGetPathFromIDList(pIIL, (char*)&szInitialDir);
	
	dir = szInitialDir;
	if (!dir.IsEmpty())
	{
		_workingDir = dir + "\\";
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


void OptionsMiscellaneous::OnBnClickedUseWorkingDir()
{
	UpdateData();
	_workingDirBut.EnableWindow(_useWorkingDir);
	_workingDirCtrl.EnableWindow(_useWorkingDir);
}
