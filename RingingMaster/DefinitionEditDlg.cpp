// DefinitionEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "DefinitionEditDlg.h"
#include "Definition.h"
#include "TouchDoc.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DefinitionEditDlg dialog


DefinitionEditDlg::DefinitionEditDlg(Definition* definition, bool editing, TouchDoc* touchDoc)
	: CDialog(DefinitionEditDlg::IDD, NULL),
	_origDefinition(definition),	
	_editing(editing),
	_touchDoc(touchDoc)
{
	//{{AFX_DATA_INIT(DefinitionEditDlg)
	_definition = _T("");
	_name = _T("");
	_type = -1;
	//}}AFX_DATA_INIT
}


void DefinitionEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DefinitionEditDlg)
	DDX_Text(pDX, IDC_DEFINITION, _definition);
	DDX_Text(pDX, IDC_NAME, _name);
	DDX_Radio(pDX, IDC_DEF_CALL, _type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DefinitionEditDlg, CDialog)
	//{{AFX_MSG_MAP(DefinitionEditDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DefinitionEditDlg message handlers

BOOL DefinitionEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_name = _origDefinition->getName();
	_definition = _origDefinition->getDefinition();	
	_type = _origDefinition->getType();

	UpdateData(FALSE);
	
	WinPos::Load(this, "DefinitionEditDlg", true); 
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void DefinitionEditDlg::OnDestroy() 
{
	WinPos::Save(this, "DefinitionEditDlg");

	CDialog::OnDestroy(); 
}

void DefinitionEditDlg::OnOK() 
{
	UpdateData();
	if (_name.IsEmpty())
	{
		AfxMessageBox("Please enter a name");
		return;
	}

	CString msg;
	if (GlobalFunctions::isIligalChar(_name,msg))
	{
		msg = "Name " + msg;
		AfxMessageBox(msg);
		return;
	}
	
	if (_definition.IsEmpty())
	{
		AfxMessageBox("Please enter a definition");
		return;
	}

	CString iligalMsg;
	if (GlobalFunctions::isIligalChar(_name, iligalMsg))
	{
		CString str;
		str.Format("Definition name %s", iligalMsg);
		AfxMessageBox(str);
		return;
	}

	if (GlobalFunctions::isIligalChar(_definition, iligalMsg))
	{
		CString str;
		str.Format("Definition %s", iligalMsg);
		AfxMessageBox(str);
		return;
	}

	if ((_touchDoc->isDefinitionUnique(_name))||
		((_editing)&&(_name == _origDefinition->getName())))
	{
		_origDefinition->setName(_name);
		_origDefinition->setDefinition(_definition);	
		_origDefinition->setType(static_cast<DefinitionType>(_type));
		CDialog::OnOK();
	}
	else
		AfxMessageBox("Please choose a unique definition name");  	
}
