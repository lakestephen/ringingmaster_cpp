// CallPosMethodEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallPosMethodEditDlg.h"
#include "Method.h"
#include "MethodDlg.h"
#include "MethodSelectRow.h"
#include "ProofManager.h"
#include "ProofInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodEditDlg dialog


CallPosMethodEditDlg::CallPosMethodEditDlg(CWnd* pParent, Notation &notation, 
										   CString name, int /*lead*/, int row)
: CDialog(CallPosMethodEditDlg::IDD, pParent),
_notation(notation),
_row(row)
{
	//{{AFX_DATA_INIT(CallPosMethodEditDlg)
	//}}AFX_DATA_INIT
	_name = name;

	//lead 
	ProofInput proofInput(_notation.getNumber());	
	proofInput.addNotation(_notation);

	Method met;
	proofManager()->requestProof(proofInput, met);
	/*	 todo
	_maxLeads = met.GetSize(); //this is implicitly +1

	if (lead == -1)
		_lead = 1;
	else
		_lead = lead + 1;	 */

}							  

void CallPosMethodEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CallPosMethodEditDlg)
	DDX_Control(pDX, IDC_CALL_POS_EDIT, _editButt);
	DDX_Control(pDX, IDC_POS, _pos);
	DDX_Control(pDX, IDC_LEAD_SPIN, _leadSpin);
	DDX_Text(pDX, IDC_NAME, _name);
	DDX_Text(pDX, IDC_LEAD, _lead);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _lead, 1, _maxLeads);
}


BEGIN_MESSAGE_MAP(CallPosMethodEditDlg, CDialog)
	//{{AFX_MSG_MAP(CallPosMethodEditDlg)
	ON_BN_CLICKED(IDC_CALL_POS_EDIT, OnCallPosEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodEditDlg message handlers

void CallPosMethodEditDlg::OnCallPosEdit() 
{
	UpdateData();
	CString str;

	_pos.GetLBText(_pos.GetCurSel(), str);

	Ints rows;
	for (int i=0;i<_notation.getCallPosLeadCount();i++)
		rows.Add(_notation.getCallPosLead(i));

	ProofInput proofInput(_notation.getNumber());	
	proofInput.addNotation(_notation);

	MethodSelectRow* met = new MethodSelectRow;
	proofManager()->requestProof(proofInput, *met);

	met->setCallPositions(&rows);
	met->_selectedPosition.setRow(met, _lead - 1, atoi(str) - (globalDataManager()->isStartAtRowZero()?0:1));

	MethodDlg dlg(this, met, "CallPosMethodEditDlg");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid()) 
	{
		_lead = met->_selectedPosition.getLead() + 1;
		str.Format("%d",met->_selectedPosition.getLeadRow() +(globalDataManager()->isStartAtRowZero()?0:1));
		_pos.SelectString(-1, str);
	}

	UpdateData(FALSE);	
}

BOOL CallPosMethodEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_editButt.SetImage(IDB_DOT_DOT_DOT);
	_editButt.m_bDrawFocus = FALSE;
	_editButt.SetTooltip("Visual Call Position Selector");
	_editButt.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_leadSpin.SetRange32(1, _maxLeads);
	
	EnableToolTips();
	
	//row
	CString pos;
	for (int i=0;i<_notation.getCallPosLeadCount();i++)
	{
		pos.Format("%d",_notation.getCallPosLead(i) + (globalDataManager()->isStartAtRowZero()?0:1));
		_pos.AddString(pos); 
	}

	if (_row != -1)
		pos.Format("%d",_row+(globalDataManager()->isStartAtRowZero()?0:1));
	else
	{
		int posCount = _notation.getCallPosLeadCount();
		if (posCount >0)
			pos.Format("%d",_notation.getCallPosLead(posCount-1)+1);
	}	 	

	if (_pos.SelectString(-1, pos) == CB_ERR)
	{
		//select the last string
		_pos.SetCurSel(_pos.GetCount()-1);//give up if this fails
	}

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CallPosMethodEditDlg::OnOK() 
{
	CString pos;		
	_pos.GetLBText(_pos.GetCurSel(), pos);
	_row = atoi(pos);

	CDialog::OnOK();
}
