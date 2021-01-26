// SpliceEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpliceEditDlg.h"
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
// SpliceEditDlg dialog


SpliceEditDlg::SpliceEditDlg(CWnd* pParent, Notation &notation, int pos)
	: CDialog(SpliceEditDlg::IDD, pParent),
	_notation(notation)
{
	notation.prepare();
	_maxRows = notation.getChangesPerPlainLead() + (globalDataManager()->isStartAtRowZero()?0:1);
	if (pos == -1) 
		_splice = notation.getChangesPerPlainLead() + (globalDataManager()->isStartAtRowZero()?0:1);
	else
		_splice = pos + (globalDataManager()->isStartAtRowZero()?0:1);

	//{{AFX_DATA_INIT(SpliceEditDlg)
	
	//}}AFX_DATA_INIT
}


void SpliceEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SpliceEditDlg)
	DDX_Control(pDX, IDC_SPLICE_EDIT, _spliceEdit);
	DDX_Control(pDX, IDC_SPLICE_SPIN, _spliceSpin);
	DDX_Text(pDX, IDC_SPLICE, _splice);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _splice, (globalDataManager()->isStartAtRowZero()?0:1), _maxRows);
}


BEGIN_MESSAGE_MAP(SpliceEditDlg, CDialog)
	//{{AFX_MSG_MAP(SpliceEditDlg)
	ON_BN_CLICKED(IDC_SPLICE_EDIT, OnSpliceEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SpliceEditDlg message handlers

BOOL SpliceEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_spliceEdit.SetImage(IDB_DOT_DOT_DOT);
	_spliceEdit.m_bDrawFocus = FALSE;
	_spliceEdit.SetTooltip("Visual Selector of Possible Splice Position");
	_spliceEdit.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_spliceSpin.SetRange32((globalDataManager()->isStartAtRowZero()?0:1), _maxRows);
 	
	EnableToolTips();	

	return TRUE;  // return TRUE unless you set the focus to a control

}

void SpliceEditDlg::OnSpliceEdit() 
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

	met->_selectedPosition.setRow(met, 0, _splice - (globalDataManager()->isStartAtRowZero()?0:1));

	MethodDlg dlg(this, met, "SpliceEditDlg");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid())
		_splice = met->_selectedPosition.getLeadRow() + (globalDataManager()->isStartAtRowZero()?0:1);

	UpdateData(FALSE);	
}
