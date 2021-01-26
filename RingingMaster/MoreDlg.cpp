// MoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "resizeDialog.h"
#include "MoreDlg.h"
#include "StyleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MoreDlg dialog


MoreDlg::MoreDlg(CWnd* pParent, CStringArray& strings, CString name)
	: ResizeDialog(MoreDlg::IDD, pParent),
	_strings(strings),
	_name(name)
{
	//{{AFX_DATA_INIT(MoreDlg)
	//}}AFX_DATA_INIT
	_staticName = "Select " + _name + ":";
}


void MoreDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MoreDlg)
	DDX_Control(pDX, IDC_LIST1, _list);
	DDX_Text(pDX, IDC_STATIC_NAME, _staticName);
	//}}AFX_DATA_MAP

	_index = _list.GetCurSel();
}


BEGIN_MESSAGE_MAP(MoreDlg, ResizeDialog)
	//{{AFX_MSG_MAP(MoreDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MoreDlg message handlers

BOOL MoreDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();

	//sort the resize controls
	AddControl(IDC_LIST1, resize, resize);
	AddControl(IDOK, repos, none);
	AddControl(IDCANCEL, repos, none);	 

	SetWindowText(_name);

	
	//fill the list.
	for (int i=0;i< _strings.GetSize();i++)
	{
		_list.AddString(_strings.GetAt(i));
	}						

	return TRUE;  // return TRUE unless you set the focus to a control

}

int MoreDlg::getIndex()
{
	return _index;
}

