// NotationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationDlg.h"
#include "TouchDoc.h"
#include "InfoDlg.h"
#include "MethodPreview.h"
#include "MethodDlg.h"

#include "NotationPlain.h"
#include "NotationCalls.h"
#include "NotationCallingPosLead.h"
#include "NotationCallingPosMethod.h"
#include "NotationCallingPosAgregate.h"
#include "NotationSplicePos.h"
#include "NotationLeadLines.h"
#include "NotationCourseHead.h"
#include "NotationInfo.h"
#include "NotationNotes.h"
#include "WinPos.h"
#include "ProofManager.h"
#include "ProofInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationDlg dialog


NotationDlg::NotationDlg(CWnd* pParent, Notation &notation, BOOL fixNumber, TouchDoc* touchDoc, bool editing, bool readOnly, bool noCrib)
: PropertySheetBase("Notation Editor", pParent),
_notation(notation),		   //create a copy
_originalNotation(notation), //keep ref to original
_touchDoc(touchDoc),
_editing(editing),
_readOnly(readOnly),
_originalName(notation.getName(true)),
_originalShorthand(notation.getSpliceLetter()),
_noCrib(noCrib)	 									   
{
	//{{AFX_DATA_INIT(NotationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//set up the dialog stuff	
	m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_PROPTITLE;
	m_psh.dwFlags &= ~PSH_HASHELP;


	_plain = new NotationPlain(_notation, fixNumber);                             
	_calls = new NotationCalls(_notation);
	_callPosLead = new NotationCallingPosLead(_notation);
	_callPosMethod = new NotationCallingPosMethod(_notation);
	_callPosAgregate = new NotationCallingPosAgregate(_notation);
	_splicePos = new NotationSplicePos(_notation);
	_leadLines = new NotationLeadLines(_notation);
	_courseHead = new NotationCourseHead(_notation);
	_info = new NotationInfo(_notation);
	_notes = new NotationNotes(_notation);
	
	CBCGPPropSheetCategory* pBasic = AddTreeCategory (_T("Basic"), 0, 1);
	AddPageToTree (pBasic, _plain, -1, 2);
	AddPageToTree (pBasic, _calls, -1, 2);
	CBCGPPropSheetCategory* pCallPositionRoot = AddTreeCategory (_T("Callings"), 0, 1);
	AddPageToTree (pCallPositionRoot, _callPosLead, -1, 2);
	AddPageToTree (pCallPositionRoot, _callPosMethod, -1, 2);
	AddPageToTree (pCallPositionRoot, _callPosAgregate, -1, 2);
	CBCGPPropSheetCategory* pPositionRoot = AddTreeCategory (_T("Positions"), 0, 1);
	AddPageToTree (pPositionRoot, _splicePos, -1, 2);
	AddPageToTree (pPositionRoot, _leadLines, -1, 2);
	AddPageToTree (pPositionRoot, _courseHead, -1, 2);
	CBCGPPropSheetCategory* pReferenceRoot = AddTreeCategory (_T("Reference"), 0, 1);
	AddPageToTree (pReferenceRoot, _info, -1, 2);
	AddPageToTree (pReferenceRoot, _notes, -1, 2);
	
	SetActivePage(_plain);

}

void NotationDlg::DoDataExchange(CDataExchange* pDX)
{
	PropertySheetBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationDlg)
	DDX_Control(pDX, AAA_INFO_BUT, _infoBut);
	DDX_Control(pDX, AAA_CRIB_BUT, _cribBut);
	DDX_Control(pDX, AAA_PREV_BUT, _prevBut);
	//}}AFX_DATA_MAP
}							 


BEGIN_MESSAGE_MAP(NotationDlg, PropertySheetBase)
	//{{AFX_MSG_MAP(NotationDlg)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(AAA_PREV_BUT, OnUpdatePreviewButton)
	ON_UPDATE_COMMAND_UI(AAA_INFO_BUT, OnUpdatePreviewButton)
	ON_UPDATE_COMMAND_UI(AAA_CRIB_BUT, OnUpdatePreviewButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(AAA_PREV_BUT, OnPreviewButton)
	ON_BN_CLICKED(AAA_INFO_BUT, OnInfoButton)
	ON_BN_CLICKED(AAA_CRIB_BUT, OnCribButton)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationDlg message handlers


NotationDlg::~NotationDlg()
{
 	delete _plain;
 	delete _calls;
 	delete _callPosLead;
 	delete _callPosMethod;
 	delete _callPosAgregate;
 	delete _splicePos;
 	delete _leadLines;
 	delete _courseHead;
 	delete _info;
	delete _notes;
}


void NotationDlg::OnOK() 
{						   
	if (_readOnly)
	{
		PressButton(PSBTN_CANCEL);
		return;
	}
	
	CString msg;
	PressButton(PSBTN_APPLYNOW);
	
	CWaitCursor wait;

	if (_notation.verify(msg))
	{
		if (_touchDoc == NULL) //when not in a touch doc
		{
			Apply();
			PressButton(PSBTN_OK);
		}
		else if (!((_touchDoc->isNotationNameUnique(_notation.getName(true)))||
			((_editing)&&(_originalName == _notation.getName(true)))))
		{
			AfxMessageBox("Please choose a unique name.");
		}
		else if (!((_touchDoc->isSpliceUnique(_notation.getSpliceLetter()))||
				  ((_editing)&&(_originalShorthand == _notation.getSpliceLetter()))))
		{
			AfxMessageBox("Please choose a unique shorthand.");
		}
		else
		{			
			if ((_notation.verifyNonEssential(msg))||
				(AfxMessageBox(msg, MB_OKCANCEL) == IDOK))
			{
				Apply();
				PressButton(PSBTN_OK);
			}
		}
	}
	else
	{
		AfxMessageBox(msg);
	}					   
}	  


BOOL NotationDlg::OnInitDialog() 
{
	BOOL ret = PropertySheetBase::OnInitDialog();

	setConstantText(_notation.getName(true));

	WinPos::Load(this, "NotationDlg", false) ;

	
	if (_readOnly)
	{
		if (_ok)
		{
			//ok butt
			_ok.ShowWindow(SW_HIDE);
		}
	}
		

	CWnd* okBtn = GetDlgItem(IDOK);
	if (okBtn && ::IsWindow(okBtn->m_hWnd))
	{
		//get the rect of the OK btn
  		CRect okBtnRect;	
		okBtn->GetWindowRect (okBtnRect);
		ScreenToClient (okBtnRect);

		//add in the extra buttons
		CRect infoBtnRect = (okBtnRect - CPoint(220,0));
		infoBtnRect.right = infoBtnRect.left + 24;
		_infoBut.Create("", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, infoBtnRect , this, AAA_INFO_BUT);
		_infoBut.SetImage(IDB_INFO);
		_infoBut.m_bDrawFocus = FALSE;
		_infoBut.SetTooltip("Method Statistics");
		_infoBut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		CRect prevBtnRect = (okBtnRect - CPoint(250,0));
		prevBtnRect.right = prevBtnRect.left + 24;
		_prevBut.Create("", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, prevBtnRect , this, AAA_PREV_BUT);
		_prevBut.SetImage(IDB_PREVIEW);
		_prevBut.m_bDrawFocus = FALSE;
		_prevBut.SetTooltip("Preview Method");
		_prevBut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


		if (!_noCrib)
		{
			CRect cribBtnRect = (okBtnRect - CPoint(280,0));
			cribBtnRect.right = cribBtnRect.left + 24;
			_cribBut.Create("", WS_CHILD|BS_CHECKBOX|WS_VISIBLE, cribBtnRect , this, AAA_CRIB_BUT);
			_cribBut.SetImage(IDB_CRIB);
			_cribBut.m_bDrawFocus = FALSE;
			_cribBut.SetTooltip("Crib Method");
			_cribBut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
		}
	}

	return ret;
}

BOOL NotationDlg::DestroyWindow() 
{
	//this code is causing problems with stedman
	/* 
	if ((_touchDoc != NULL)&&	 	
		(_originalNotation.checkForSymmetry(false)))
	{
		CString msg = _originalNotation.getName();
		msg += " can be converted to a symmetrical representation. \n";
		msg += "Do you want to make this conversion?"; 
			
			if (AfxMessageBox(msg, MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			_originalNotation.checkForSymmetry(true);
		}
	}*/
	
	return PropertySheetBase::DestroyWindow();
}

void NotationDlg::OnDestroy() 
{
	WinPos::Save(this, "NotationDlg");

	PropertySheetBase::OnDestroy();	
}


void NotationDlg::Apply()
{
	if (!_readOnly)
		_originalNotation = _notation;
}  	 




void NotationDlg::OnPreviewButton() 
{
	PressButton(PSBTN_APPLYNOW);

	CString msg;

	CWaitCursor wait;

	if (_notation.verify(msg))
	{
		ProofInput proofInput(_notation.getNumber());
		proofInput.addNotation(_notation);
		proofInput.setPerformAnalysis(); //todo do we want to do analysis here?
		MethodPreview* mPrev = new MethodPreview;			
		proofManager()->requestProof(proofInput, *mPrev);

		MethodDlg dlg(this, mPrev, "NotationDlg");
		dlg.DoModal();	
	}
	else
	{
		AfxMessageBox(msg);
	}
}




void NotationDlg::OnInfoButton() 
{
	PressButton(PSBTN_APPLYNOW);

	if (!_notation.getNotation().IsEmpty())
	{
		InfoDlg dlg(this, _notation);
		dlg.DoModal();
	}
}	   

void NotationDlg::OnCribButton() 
{
	_notation.crib();
}



void NotationDlg::OnUpdatePreviewButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_notation.getNotation().IsEmpty());
}

