// OptionsUser.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsUser.h"
#include "UserManager.h"
#include ".\optionsuser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsUser dialog


OptionsUser::OptionsUser()
	: CBCGPPropertyPage(OptionsUser::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsUser)
	_workingDir = _T("");
	//}}AFX_DATA_INIT
}


void OptionsUser::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsUser)
	DDX_Control(pDX, IDC_OPTIONS_USERS_DEL, _delBtn);
	DDX_Control(pDX, IDC_OPTIONS_USERS_ADD, _addBtn);
	DDX_Control(pDX, IDC_WORKING_DIR_BUTT, _workingDirBut);
	DDX_Control(pDX, IDC_OPTIONS_USER_COMBO, _comboUsers);
	DDX_Text(pDX, IDC_WORKING_DIR, _workingDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsUser, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsUser)
	ON_BN_CLICKED(IDC_OPTIONS_USERS_ADD, OnOptionsUsersAdd)
	ON_CBN_SELCHANGE(IDC_OPTIONS_USER_COMBO, OnSelchangeOptionsUserCombo)
	ON_BN_CLICKED(IDC_WORKING_DIR_BUTT, OnWorkingDirButt)
	ON_EN_CHANGE(IDC_WORKING_DIR, OnChangeWorkingDir)
	ON_BN_CLICKED(IDC_OPTIONS_USERS_DEL, OnOptionsUsersDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsUser message handlers

void OptionsUser::OnOptionsUsersAdd() 
{
	UpdateData();
	userManager()->setUserBasePath(_workingDir);
	userManager()->createUser();	

	updateUsersList();
}

void OptionsUser::OnOptionsUsersDel() 
{
	userManager()->deleteUser();

	updateUsersList();	
}


BOOL OptionsUser::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	//will load user details if not already.
	userManager()->isValid();

	_workingDir = userManager()->getUserBasePath();
	UpdateData(FALSE);

	updateUsersList();

	_workingDirBut.SetImage(IDB_DOT_DOT_DOT);
	_workingDirBut.m_bDrawFocus = FALSE;
	_workingDirBut.SetTooltip("Find Directory");
	_workingDirBut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addBtn.SetImage(IDB_NEW);
	_addBtn.m_bDrawFocus = FALSE;
	_addBtn.SetTooltip("Add User");
	_addBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_delBtn.SetImage(IDB_DELETE);
	_delBtn.m_bDrawFocus = FALSE;
	_delBtn.SetTooltip("Delete User");
	_delBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsUser::updateUsersList()
{

	_comboUsers.ResetContent();

	if((IsWindow(_comboUsers))&&(!_workingDir.IsEmpty()))
	{
		UpdateData();		
		userManager()->setUserBasePath(_workingDir);

		CStringArray users;
		userManager()->getUserList(users);

		for (int i=0;i<users.GetSize();i++)
		{
			_comboUsers.AddString(users.GetAt(i));
		}
		_comboUsers.SetCurSel(userManager()->getActiveUserIndex());
	}		
}

void OptionsUser::OnSelchangeOptionsUserCombo() 
{
	userManager()->setActiveUserIndex(_comboUsers.GetCurSel());		
}

int CALLBACK OptionsUserBrowseCallbackProc(HWND hwnd,
								UINT uMsg,
								LPARAM /*lParam*/,
								LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);

	return 0;
}


void OptionsUser::OnWorkingDirButt() 
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
	bi.lpszTitle = "Please choose a directory for the user data";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
	bi.lpfn = OptionsUserBrowseCallbackProc;
	bi.lParam = reinterpret_cast<LPARAM>(dir.GetBuffer(0));

	LPITEMIDLIST pIIL = ::SHBrowseForFolder(&bi);

	TCHAR szInitialDir[_MAX_PATH];
	
	BOOL bRet = ::SHGetPathFromIDList(pIIL, (char*)&szInitialDir);
	
	dir = szInitialDir;

	while((dir.GetLength() > 0)&&
		  (dir.GetAt(dir.GetLength()-1) == '\\'))
	{
		dir.Delete(dir.GetLength()-1,1);
	}

	if (!dir.IsEmpty())
		_workingDir = dir;
	UpdateData(FALSE);

	if (bRet)
	{
		LPMALLOC pMalloc;
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pIIL);
		pMalloc->Release();	 
	}
	
	updateUsersList();
}

void OptionsUser::OnChangeWorkingDir() 
{
	updateUsersList();	
}


BOOL OptionsUser::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		if (!_workingDir.IsEmpty())//does the UpdateData()
		{  			
			userManager()->setUserBasePath(_workingDir);
		}

		return TRUE;
	}

	return FALSE;
}
