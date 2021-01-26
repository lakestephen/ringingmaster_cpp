// CallEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallEditDlg.h"
#include "Notation.h"
#include "MethodEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CallEditDlg dialog


CallEditDlg::CallEditDlg(CWnd* pParent, int number, CString nameShorthand, CString nameLonghand, CString notation)
	: CDialog(CallEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CallEditDlg)
	_notationStatic = _T("");
	//}}AFX_DATA_INIT
	_notation = _T(notation);
	_nameShorthand = _T(nameShorthand);
	_nameLonghand = _T(nameLonghand);
	_number = number;
}


void CallEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CallEditDlg)
	DDX_Control(pDX, IDC_CALL_EDIT, _visualEdit);
	DDX_Text(pDX, IDC_CALL_NOTATION, _notation);
	DDX_Text(pDX, IDC_CALL_NAME_SHORTHAND, _nameShorthand);
	DDX_Text(pDX, IDC_CALL_NAME_LONGHAND, _nameLonghand);
	DDX_Text(pDX, IDC_CALL_NOTATION_STATIC, _notationStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CallEditDlg, CDialog)
	//{{AFX_MSG_MAP(CallEditDlg)
	ON_BN_CLICKED(IDC_CALL_EDIT, OnCallEdit)
	ON_EN_CHANGE(IDC_CALL_NOTATION, OnChangeCallNotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CallEditDlg message handlers

void CallEditDlg::OnCallEdit() 
{
	UpdateData();

	//create the temp notation
	Notation not;
	not.create(true, "",_number, false, _notation);
	CString notStr = not.getNotation();

	int size = 0 ;
	if (notStr.IsEmpty())
	{
		size = 1;
	}
		
	MethodEditDlg dlg(this, _number, notStr, size);
	if (dlg.DoModal() == IDOK)
	{
		not.create(true, "",_number, false, dlg._notation);
		_notation = not.getNotation();
		UpdateData(FALSE);
	}	
}

BOOL CallEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips();

	_visualEdit.SetImage(IDB_NOTATION);
	_visualEdit.m_bDrawFocus = FALSE;
	_visualEdit.SetTooltip("Visual Notation Editor");
	_visualEdit.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CallEditDlg::OnChangeCallNotation() 
{
	UpdateData();

	//create the temp notation
	Notation not;
	not.create(true, "",_number, false, _notation);
	_notationStatic = not.getNotation();

	UpdateData(FALSE);		
}
