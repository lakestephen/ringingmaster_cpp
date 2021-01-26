// NotationCalls.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationCalls.h"
#include "NotationDlg.h"
#include "CallEditDlg.h"
#include "PropItem.h"
#include "LeadHeadDlg.h"
#include "Notation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationCalls property page

NotationCalls::NotationCalls( Notation &notation) : 
CBCGPPropertyPage( NotationCalls::IDD),
_notation(notation)	
{
	//{{AFX_DATA_INIT(NotationCalls)
	//}}AFX_DATA_INIT
}

NotationCalls::~NotationCalls()
{
}

void NotationCalls::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationCalls)
	DDX_Control(pDX, IDC_ADD_NEAR_PLACE_CALL, _defNear);
	DDX_Control(pDX, IDC_ADD_EXTREAM_PLACE_CALL, _defExtream);
	DDX_Control(pDX, IDC_LEAD_HEAD, _leadHead);
	DDX_Control(pDX, IDC_SET_STD_CALL, _stdCall);
	DDX_Control(pDX, IDC_VIEW_LEADHEADS, _viewLeadHeads);
	DDX_Control(pDX, IDC_USE_DEFAULT_CALLS, _useDefault);
	DDX_Control(pDX, IDC_CALLS, _calls);
	DDX_Control(pDX, IDC_REMOVE_CALL, _removeCall);
	DDX_Control(pDX, IDC_ADD_CALL, _addCall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationCalls, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationCalls)
	ON_BN_CLICKED(IDC_ADD_CALL, OnAddCall)
	ON_BN_CLICKED(IDC_REMOVE_CALL, OnRemoveCall)
	ON_BN_CLICKED(IDC_USE_DEFAULT_CALLS, OnUseDefaultCalls)
	ON_BN_CLICKED(IDC_VIEW_LEADHEADS, OnViewLeadheads)
	ON_BN_CLICKED(IDC_SET_STD_CALL, OnSetStdCall)
	ON_BN_DOUBLECLICKED(IDC_USE_DEFAULT_CALLS, OnUseDefaultCalls)
	ON_BN_CLICKED(IDC_ADD_NEAR_PLACE_CALL, OnAddNearPlaceCall)
	ON_BN_CLICKED(IDC_ADD_EXTREAM_PLACE_CALL, OnAddExtreamPlaceCall)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ADD_CALL, OnUpdateAddCall)
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_CALL, OnUpdateRemoveCall)
	ON_UPDATE_COMMAND_UI(IDC_SET_STD_CALL, OnUpdateDefaultCall)
	ON_UPDATE_COMMAND_UI(IDC_ADD_NEAR_PLACE_CALL, OnUpdateAddNear)
	ON_UPDATE_COMMAND_UI(IDC_ADD_EXTREAM_PLACE_CALL, OnUpdateAddExtreme)
	ON_MESSAGE(WM_PROP_BUTTON, OnEditCall)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationCalls message handlers

BOOL NotationCalls::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	EnableToolTips();
	
	_addCall.SetImage(IDB_NEW);
	_addCall.m_bDrawFocus = FALSE;
	_addCall.SetTooltip("Add Call");
	_addCall.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_removeCall.SetImage(IDB_DELETE);
	_removeCall.m_bDrawFocus = FALSE;
	_removeCall.SetTooltip("Remove Call");
	_removeCall.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_stdCall.SetImage(IDB_FLAG);
	_stdCall.m_bDrawFocus = FALSE;
	_stdCall.SetTooltip("Flag Default Call");
	_stdCall.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_defNear.SetImage(IDB_NEW_NEAR);
	_defNear.m_bDrawFocus = FALSE;
	_defNear.SetTooltip("Add the default near place calls to the user definable list");
	_defNear.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	_defExtream.SetImage(IDB_NEW_EXTREME);
	_defExtream.m_bDrawFocus = FALSE;
	_defExtream.SetTooltip("Add the default extreme place calls to the user definable list");
	_defExtream.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	_viewLeadHeads.SetImage(IDB_LEADHEADTABLE);
	_viewLeadHeads.m_bDrawFocus = FALSE;
	_viewLeadHeads.SetTooltip("View Standard Lead Head Table");
	_viewLeadHeads.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	PropItem * propItem;

	//method referanced
	_calls.setColumnCount(3);
	_calls.setColumnHead(true);
	
	propItem = new PropItem(PT_BUTTON, "Call Name", "Notation", "Default");
	_calls.AddPropItem(propItem);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationCalls::OnAddCall() 
{
	if (_notation.getDefaultCalls()) return;

	CallEditDlg dlg(this,_notation.getNumber(), "","","");

	while (dlg.DoModal() == IDOK)
	{
		CallError ret = _notation.addCall(dlg._nameShorthand, dlg._nameLonghand, dlg._notation);
		if ((ret == CE_OK)||(ret == CE_OK_DEFAULT))
		{			
			createList();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddCallErrorText(ret));
		}
	}
}

void NotationCalls::OnRemoveCall() 
{
	//find the item
	PropItem * pItem = _calls.getPropItem(_calls.GetCurSel());

	if (pItem)
	{
		_notation.deleteCall(_notation.findCall(pItem->_col[0]));
	}

	createList();
}

LRESULT NotationCalls::OnEditCall(WPARAM /*wParam*/, LPARAM lParam)	
{
	if (_notation.getDefaultCalls()) return 0;

	PropItem * pItem = (PropItem*)lParam;
	if (!pItem) return 0;

	//find the call from the prop item //lets be really safe here
	int index = _notation.findCall(pItem->_col[0]);
	if (index == -1) return 0;
	
	NotationCall * pCall = _notation.getCall(index);
	if (!pCall) return 0;

	//are we the standard call?
	bool weAreStandard = (pCall == _notation.getStandardCall());

	
	//edit the call
	CallEditDlg dlg(this,_notation.getNumber(), 
						 pCall->_nameShorthand,
						 pCall->_nameLonghand,
						 pCall->_notation);

	while (dlg.DoModal() == IDOK)
	{
		CallError ret = _notation.changeCall(index, dlg._nameShorthand, dlg._nameLonghand, dlg._notation);
		if ((ret == CE_OK)||(ret == CE_OK_DEFAULT))
		{			
			if (weAreStandard) _notation.setStandardCall(_notation.getCall(index)->_nameShorthand);
			createList();
			return 0;
		}
		else
		{
			AfxMessageBox(_notation.getAddCallErrorText(ret));
		}
	}																   
	return 0;	
}

void NotationCalls::OnUseDefaultCalls() 
{
	_notation.setDefaultCalls(_useDefault.GetCheck());

	update();
}

void NotationCalls::OnUpdateAddCall(CCmdUI* pCmdUI) 
{
	//not default calls
	pCmdUI->Enable(!_notation.getDefaultCalls());
}  

void NotationCalls::OnUpdateRemoveCall(CCmdUI* pCmdUI) 
{
	//not default calls and on an item
	pCmdUI->Enable(!_notation.getDefaultCalls() &&				
					_calls.getPropItem(_calls.GetCurSel()));
}  

void NotationCalls::OnUpdateDefaultCall(CCmdUI* pCmdUI) 
{
	//not default calls and on an item
	pCmdUI->Enable(!_notation.getDefaultCalls() &&
					_calls.getPropItem(_calls.GetCurSel()));
}  

void NotationCalls::OnUpdateAddNear(CCmdUI* pCmdUI) 
{
	//not default calls 
	pCmdUI->Enable(!_notation.getDefaultCalls());
}  

void NotationCalls::OnUpdateAddExtreme(CCmdUI* pCmdUI) 
{
	//not default calls 
	pCmdUI->Enable(!_notation.getDefaultCalls());
					
}  


void NotationCalls::update()
{
	_notation.calculateLeadHead();
	_leadHead.SetWindowText(_notation.getLeadHead());
	_useDefault.SetCheck(_notation.getDefaultCalls());

	createList();
}

void NotationCalls::createList()
{
	//empty the list;
	_calls.deleteAllPropItems();

	BOOL editable = !_notation.getDefaultCalls();

	NotationCall * stdCall = _notation.getStandardCall() ;
	
	for (int i=0;i<_notation.getCallCount();i++)
	{
		NotationCall * call = _notation.getCall(i);
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, call->getDisplayName() , 
			call->_notation, (call == stdCall)?"Default":"");
		_calls.AddPropItem(propItem);
	}
}


void NotationCalls::OnSetStdCall() 
{
		//find the item
	PropItem * pItem = _calls.getPropItem(_calls.GetCurSel());

	if (pItem)
	{
		NotationCall* call = _notation.getCall(_notation.findCall(pItem->_col[0]));
		if (call)
			_notation.setStandardCall(call->_nameShorthand);
	}
	else 
	{
		AfxMessageBox("Select a call before flagging a default call");
	}

	createList();	
}


void NotationCalls::OnViewLeadheads() 
{
	mainFrame()->doShowLeadHead(this);
}

void NotationCalls::OnAddNearPlaceCall() 
{
	if (_notation.getDefaultCalls()) return;
	
	_notation.addCallDefault(TRUE);

	update();	
}

void NotationCalls::OnAddExtreamPlaceCall() 
{
	if (_notation.getDefaultCalls()) return;
	
	_notation.addCallDefault(FALSE);

	update();	
}

BOOL NotationCalls::OnKillActive() 
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		update();
		return TRUE;
	}

	return FALSE;   
}

BOOL NotationCalls::OnSetActive() 
{
	if (CBCGPPropertyPage::OnSetActive())
	{
		update();
		return TRUE;
	}

	return FALSE;   
}
