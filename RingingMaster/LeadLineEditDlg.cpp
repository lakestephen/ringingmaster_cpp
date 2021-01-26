// LeadLineEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "LeadLineEditDlg.h"
#include "Method.h"
#include "MethodDlg.h"
#include "Lead.h"
#include "MethodSelectLeadLine.h"
#include "ProofManager.h"
#include "ProofInput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LeadLineEditDlg dialog


LeadLineEditDlg::LeadLineEditDlg(CWnd* pParent, Notation & notation, int pos)
: CDialog(LeadLineEditDlg::IDD, pParent),
_notation(notation)

{			
	notation.prepare();
	_maxRows = notation.getChangesPerPlainLead() - 1 +(globalDataManager()->isStartAtRowZero()?0:1) ;
	if (pos == -1) 
	{

		ProofInput proofInput(_notation.getNumber());
		proofInput.addNotation(_notation);
		proofInput.setLeadLimit(1);

		Method met;
		proofManager()->requestProof(proofInput, met);

		if ((met.getLeadCount() > 0)&&(met.getLead(0).getHuntCount() == 1))
			_leadLine = met.getLead(0).GetSize() - 2 + (globalDataManager()->isStartAtRowZero()?0:1);
		else 
			_leadLine = (globalDataManager()->isStartAtRowZero()?0:1);
			
	}
	else
		_leadLine = pos + (globalDataManager()->isStartAtRowZero()?0:1);
		
}


void LeadLineEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LeadLineEditDlg)
	DDX_Control(pDX, IDC_LEAD_LINE_EDIT, _leadLineEdit);
	DDX_Control(pDX, IDC_LEAD_SPIN, _leadSpin);
	DDX_Text(pDX, IDC_LEAD_LINE, _leadLine);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _leadLine, (globalDataManager()->isStartAtRowZero()?0:1), _maxRows);
}


BEGIN_MESSAGE_MAP(LeadLineEditDlg, CDialog)
	//{{AFX_MSG_MAP(LeadLineEditDlg)
	ON_BN_CLICKED(IDC_LEAD_LINE_EDIT, OnLeadLineEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LeadLineEditDlg message handlers

BOOL LeadLineEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_leadLineEdit.SetImage(IDB_DOT_DOT_DOT);
	_leadLineEdit.m_bDrawFocus = FALSE;
	_leadLineEdit.SetTooltip("Visual Lead Line Selector");
	_leadLineEdit.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_leadSpin.SetRange32((globalDataManager()->isStartAtRowZero()?0:1),_maxRows);
 	
	EnableToolTips();
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void LeadLineEditDlg::OnLeadLineEdit() 
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

	MethodSelectLeadLine* met = new MethodSelectLeadLine;
	proofManager()->requestProof(proofInput, *met);

	met->_selectedPosition.setRow(met, 0, 0);//just needed to give a valid methodPosition
	met->_selectedPosition.setLeadLine(_leadLine - (globalDataManager()->isStartAtRowZero()?0:1));

	MethodDlg dlg(this, met, "LeadLineEditDlg");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid())  
		_leadLine = met->_selectedPosition.getLeadLine() + (globalDataManager()->isStartAtRowZero()?0:1);
	
	UpdateData(FALSE);		
}




