// GenericPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GenericPropDlg.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GenericPropDlg dialog


GenericPropDlg::GenericPropDlg(CWnd* pParent, CString name, CString regName, int devider)
: ResizeDialog(GenericPropDlg::IDD, pParent),
_name(name),
_devider(devider),
_regName(regName)
{
	//{{AFX_DATA_INIT(GenericPropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GenericPropDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GenericPropDlg)
	DDX_Control(pDX, IDC_LIST, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GenericPropDlg, ResizeDialog)
	//{{AFX_MSG_MAP(GenericPropDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GenericPropDlg message handlers

BOOL GenericPropDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
		


	AddControl(IDC_LIST, resize, resize);

	_list.setColumnCount(2);
	_list.setColumnHead(false);
	_list._devider[0] = _devider;

	SetWindowText(_name);

	//populate defered items
	for (int i=0;i<_defPropItems.GetSize();i++)
	{
		_list.AddPropItem(_defPropItems.GetAt(i));
	}

	WinPos::Load(this, _regName, true)	;


	return TRUE;  // return TRUE unless you set the focus to a control

}

void GenericPropDlg::addProp(CString name, CString value)
{
	PropItem * propItem;
	propItem = new PropItem(PT_STATIC, name, value);

	if (_list.GetSafeHwnd() == NULL)
		_defPropItems.Add(propItem);
	else
		_list.AddPropItem(propItem);
}



BOOL GenericPropDlg::DestroyWindow() 
{
	WinPos::Save(this, _regName);
		
	return ResizeDialog::DestroyWindow();
}
