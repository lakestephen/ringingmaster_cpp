// NotationCourseHead.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"
#include "NotationDlg.h"
#include "NotationCourseHead.h"
#include "LeadLineEditDlg.h"
#include "MethodPosition.h"
#include "MethodDlg.h"
#include "MethodSelectRow.h"
#include ".\notationcoursehead.h"
#include "ProofManager.h"
#include "ProofInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationCourseHead property page


NotationCourseHead::NotationCourseHead(Notation & notation) : 
CBCGPPropertyPage(NotationCourseHead::IDD),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationCourseHead)
	_courseHead = 1;
	//}}AFX_DATA_INIT

}

NotationCourseHead::~NotationCourseHead()
{
}

void NotationCourseHead::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationCourseHead)
	DDX_Control(pDX, IDC_USE_DEFAULT_COURSE_HEAD, _useDefaultCourseHead);
	DDX_Control(pDX, IDC_COURSE_HEAD_SPIN, _courseHeadSpin);
	DDX_Control(pDX, IDC_COURSE_HEAD_EDIT, _courseHeadButton);
	DDX_Text(pDX, IDC_COURSE_HEAD, _courseHead);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _courseHead, (globalDataManager()->isStartAtRowZero()?0:1), _notation.getChangesPerPlainLead()+(globalDataManager()->isStartAtRowZero()?0:1));
}


BEGIN_MESSAGE_MAP(NotationCourseHead, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationCourseHead)
	ON_BN_CLICKED(IDC_COURSE_HEAD_EDIT, OnCourseHeadButton)
	ON_BN_CLICKED(IDC_USE_DEFAULT_COURSE_HEAD, OnUseDefaultCourseHead)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_COURSE_HEAD, OnUpdateDefaultCourseHeads)	
	ON_UPDATE_COMMAND_UI(IDC_COURSE_HEAD_SPIN, OnUpdateDefaultCourseHeads)	
	ON_UPDATE_COMMAND_UI(IDC_COURSE_HEAD_EDIT, OnUpdateDefaultCourseHeads)	
	

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NotationCourseHead message handlers

BOOL NotationCourseHead::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_courseHeadButton.SetImage(IDB_DOT_DOT_DOT);
	_courseHeadButton.m_bDrawFocus = FALSE;
	_courseHeadButton.SetTooltip("Visual Course Head Position Selector");
	_courseHeadButton.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	EnableToolTips();

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationCourseHead::OnUpdateDefaultCourseHeads(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultCourseHead.GetCheck());
}

void NotationCourseHead::OnCourseHeadButton() 
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
	ASSERT(met);//todo test this also test for memory leak

	met->_selectedPosition.setRow(met, 0,_courseHead - (globalDataManager()->isStartAtRowZero()?0:1));

	MethodDlg dlg(this, met, "NotationCourseHead");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid()) 
		_notation.setCourseHeadPos(met->_selectedPosition.getLeadRow());
	
	createCourseHeadPos();		
}

void NotationCourseHead::createCourseHeadPos()
{	
	_useDefaultCourseHead.SetCheck(_notation.getDefaultCourseHeadPos());
	_courseHead = _notation.getCourseHeadPos() + (globalDataManager()->isStartAtRowZero()?0:1);
	_courseHeadSpin.SetRange32((globalDataManager()->isStartAtRowZero()?0:1), _notation.getChangesPerPlainLead()+(globalDataManager()->isStartAtRowZero()?0:1));
	UpdateData(FALSE);
}



void NotationCourseHead::OnUseDefaultCourseHead() 
{
	_notation.setDefaultCourseHeadPos(_useDefaultCourseHead.GetCheck());
	createCourseHeadPos();		
}

BOOL NotationCourseHead::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		if (!_useDefaultCourseHead.GetCheck())
			_notation.setCourseHeadPos(_courseHead-(globalDataManager()->isStartAtRowZero()?0:1));

		return TRUE;
	}

	return FALSE;
}

BOOL NotationCourseHead::OnSetActive()
{
	createCourseHeadPos();	

	return CBCGPPropertyPage::OnSetActive();
}
