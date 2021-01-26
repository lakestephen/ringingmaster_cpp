// PrintAnalysisSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PrintAnalysisSetupDlg.h"
#include "TouchDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PrintAnalysisSetupDlg dialog


PrintAnalysisSetupDlg::PrintAnalysisSetupDlg(CWnd* pParent )
	: CDialog(PrintAnalysisSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PrintAnalysisSetupDlg)
	_bAllWork = FALSE;
	_bCourse = FALSE;
	_bMultiBlock = FALSE;
	_bMusic = FALSE;
	_bSplice = FALSE;
	_bStatus = FALSE;
	_bFalseness = FALSE;
	_bLeadEnds = FALSE;
	_bCallsMade = FALSE;
	_bSpliceTransition = FALSE;
	//}}AFX_DATA_INIT
}


void PrintAnalysisSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PrintAnalysisSetupDlg)
	DDX_Control(pDX, IDC_PRT_SPLICE_TRANSITION, _ctrlSpliceTransition);
	DDX_Control(pDX, ID_PRINT, _print);
	DDX_Control(pDX, ID_PREVIEW, _preview);
	DDX_Control(pDX, IDC_PRT_MULTI_BLOCK, _ctrlMultiBlock);
	DDX_Control(pDX, IDC_PRT_SPLICE, _ctrlSplice);
	DDX_Check(pDX, IDC_PRT_ALL_WORK, _bAllWork);
	DDX_Check(pDX, IDC_PRT_COURSE_HEADS, _bCourse);
	DDX_Check(pDX, IDC_PRT_MULTI_BLOCK, _bMultiBlock);
	DDX_Check(pDX, IDC_PRT_MUSIC, _bMusic);
	DDX_Check(pDX, IDC_PRT_SPLICE, _bSplice);
	DDX_Check(pDX, IDC_PRT_STATUS, _bStatus);
	DDX_Check(pDX, IDC_PRT_FALSENESS, _bFalseness);
	DDX_Check(pDX, IDC_PRT_LEAD_ENDS, _bLeadEnds);
	DDX_Check(pDX, IDC_PRT_CALLS_MADE, _bCallsMade);
	DDX_Check(pDX, IDC_PRT_SPLICE_TRANSITION, _bSpliceTransition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PrintAnalysisSetupDlg, CDialog)
	//{{AFX_MSG_MAP(PrintAnalysisSetupDlg)
	ON_BN_CLICKED(ID_PREVIEW, OnPreview)
	ON_BN_CLICKED(ID_PRINT, OnPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PrintAnalysisSetupDlg message handlers

BOOL PrintAnalysisSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_print.SetImage(IDB_PRINT);
	_print.m_bDrawFocus = FALSE;
	_print.SetTooltip("Print");
	_print.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_3D;

	_preview.SetImage(IDB_PREVIEW);
	_preview.m_bDrawFocus = FALSE;
	_preview.SetTooltip("Print Preview");
	_preview.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_3D;


	_bFalseness= 			app()->GetProfileInt("Print", "Falseness", FALSE);	
	_bStatus =				app()->GetProfileInt("Print", "Status", TRUE);	
	_bSplice =				app()->GetProfileInt("Print", "Splice", TRUE);	
	_bSpliceTransition =	app()->GetProfileInt("Print", "Splice Transition", TRUE);	
	_bMusic =				app()->GetProfileInt("Print", "Music", TRUE);	
	_bMultiBlock =			app()->GetProfileInt("Print", "MultiBlock", TRUE);	
	_bCourse =				app()->GetProfileInt("Print", "Course", TRUE);	
	_bLeadEnds =			app()->GetProfileInt("Print", "LeadEnds", TRUE);	
	_bAllWork =				app()->GetProfileInt("Print", "AllWork", TRUE);	
	_bCallsMade =			app()->GetProfileInt("Print", "CallsMade", TRUE);	
	

	//disable non wanted analysis
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	if ((pTouchDoc)&&(!pTouchDoc->isMultiBlock()))
	{
		_ctrlMultiBlock.EnableWindow(FALSE);
		_bMultiBlock = FALSE;
	}
	if ((pTouchDoc)&&(!pTouchDoc->isSpliced()))
	{
		_ctrlSplice.EnableWindow(FALSE);
		_ctrlSpliceTransition.EnableWindow(FALSE);
		_bSplice = FALSE;
		_bSpliceTransition = FALSE;
	}
		
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void PrintAnalysisSetupDlg::OnPreview() 
{
	update();
	EndDialog(ID_PREVIEW);	
}

void PrintAnalysisSetupDlg::OnPrint() 
{
	update();
	EndDialog(ID_PRINT);
}

void PrintAnalysisSetupDlg::update()
{
	UpdateData(TRUE);
	
	//Dont save the state of the non showing analysis.
	//as the value if forced in init dialog

	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	if ((pTouchDoc)&&(pTouchDoc->isMultiBlock()))
	{
		app()->WriteProfileInt("Print", "MultiBlock", _bMultiBlock);	
	}
	if ((pTouchDoc)&&(pTouchDoc->isSpliced()))
	{
		app()->WriteProfileInt("Print", "Splice", _bSplice);	
		app()->WriteProfileInt("Print", "Splice Transition", _bSpliceTransition);	
	}


	app()->WriteProfileInt("Print", "Falseness", _bFalseness);	
	app()->WriteProfileInt("Print", "Status", _bStatus);	
	app()->WriteProfileInt("Print", "Music", _bMusic);	
	app()->WriteProfileInt("Print", "Course", _bCourse);	
	app()->WriteProfileInt("Print", "LeadEnds", _bLeadEnds);	
	app()->WriteProfileInt("Print", "AllWork", _bAllWork);	
	app()->WriteProfileInt("Print", "CallsMade", _bCallsMade);	

}
