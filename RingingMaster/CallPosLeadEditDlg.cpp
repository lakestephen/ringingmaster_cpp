// CallPosLeadEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallPosLeadEditDlg.h"
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
// CallPosLeadEditDlg dialog


CallPosLeadEditDlg::CallPosLeadEditDlg(CWnd* pParent, Notation & notation, int pos)
: CDialog(CallPosLeadEditDlg::IDD, pParent),
_notation(notation)
{
	notation.prepare();
	_maxRows = notation.getChangesPerPlainLead() + (globalDataManager()->isStartAtRowZero()?0:1);
	if (pos == -1) 
		_callPosLead = notation.getChangesPerPlainLead() + (globalDataManager()->isStartAtRowZero()?-1:0);
	else
		_callPosLead = pos + (globalDataManager()->isStartAtRowZero()?0:1);
	//{{AFX_DATA_INIT(CallPosLeadEditDlg)
	//}}AFX_DATA_INIT

}


void CallPosLeadEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CallPosLeadEditDlg)
	DDX_Control(pDX, IDC_LEAD_SPIN, _leadSpin);
	DDX_Control(pDX, IDC_CALL_POS_EDIT, _callPosEdit);
	DDX_Text(pDX, IDC_CALL_POS_LEAD, _callPosLead);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _callPosLead, (globalDataManager()->isStartAtRowZero()?0:1), _maxRows);

}


BEGIN_MESSAGE_MAP(CallPosLeadEditDlg, CDialog)
	//{{AFX_MSG_MAP(CallPosLeadEditDlg)
	ON_BN_CLICKED(IDC_CALL_POS_EDIT, OnCallPosEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CallPosLeadEditDlg message handlers

void CallPosLeadEditDlg::OnCallPosEdit() 
{
	UpdateData();

	Notation notCopy(_notation);

	//remove all the call positions from the copy
	for (int i=0;i<notCopy.getCallPosMethodCount();i++)
	{
		notCopy.deleteCallPosMethod(0);
	}

	ProofInput proofInput(notCopy.getNumber());
	proofInput.addNotation(notCopy);
	proofInput.setLeadLimit(1);

	MethodSelectRow* met = new MethodSelectRow;
	proofManager()->requestProof(proofInput, *met);

	met->_selectedPosition.setRow(met, 0, _callPosLead - (globalDataManager()->isStartAtRowZero()?0:1));

	MethodDlg dlg(this, met, "CallPosLeadEditDlg");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid())
		_callPosLead = met->_selectedPosition.getLeadRow() + (globalDataManager()->isStartAtRowZero()?0:1);

	UpdateData(FALSE);	
}

BOOL CallPosLeadEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_callPosEdit.SetImage(IDB_DOT_DOT_DOT);
	_callPosEdit.m_bDrawFocus = FALSE;
	_callPosEdit.SetTooltip("Visual Selector of Possible Call Position");
	_callPosEdit.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_leadSpin.SetRange32((globalDataManager()->isStartAtRowZero()?0:1), _maxRows);
 	
	EnableToolTips();

	return TRUE;  // return TRUE unless you set the focus to a control

}



