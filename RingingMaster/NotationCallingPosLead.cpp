// NotationCallingPosLead.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"
#include "NotationDlg.h"
#include "NotationCallingPosLead.h"
#include "CallPosLeadEditDlg.h"
#include "CallPosMethodEditDlg.h"
#include "CallPosAgregateEditDlg.h"
#include ".\notationcallingposlead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosLead property page

NotationCallingPosLead::NotationCallingPosLead( Notation & notation) : 
CBCGPPropertyPage(NotationCallingPosLead::IDD),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationCallingPosLead)
	//}}AFX_DATA_INIT
}

NotationCallingPosLead::~NotationCallingPosLead()
{
}

void NotationCallingPosLead::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationCallingPosLead)
	DDX_Control(pDX, IDC_CALL_POS_ADD_DEFAULT, _addDefault);
	DDX_Control(pDX, IDC_CALL_POS, _list);
	DDX_Control(pDX, IDC_CALL_POS_CHECK, _check);
	DDX_Control(pDX, IDC_CALL_POS_REMOVE, _remove);
	DDX_Control(pDX, IDC_CALL_POS_ADD, _add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationCallingPosLead, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationCallingPosLead)
	ON_BN_CLICKED(IDC_CALL_POS_REMOVE, OnCallPosRemove)
	ON_BN_CLICKED(IDC_CALL_POS_ADD, OnCallPosAdd)
	ON_BN_CLICKED(IDC_CALL_POS_CHECK, OnCallPosCheck)
	ON_BN_CLICKED(IDC_CALL_POS_ADD_DEFAULT, OnCallPosAddDefault)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ADD, OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_REMOVE, OnUpdateRemove)
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ADD_DEFAULT, OnUpdateDefault)		   	
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify) 
	ON_MESSAGE(WM_PROP_BUTTON, OnEditCallingPos)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosLead message handlers

BOOL NotationCallingPosLead::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	EnableToolTips();


	_add.SetImage(IDB_NEW);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add Lead Calling Position");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_DELETE);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove Lead Calling Position");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addDefault.SetImage(IDB_ADD_DEFAULT);
	_addDefault.m_bDrawFocus = FALSE;
	_addDefault.SetTooltip("Add the default lead calling position to the user definable list");
	_addDefault.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	_list.setColumnHead(true);
	_list.AddPropItem(new PropItem(PT_STATIC, ""));
	_list.setColumnCount(1);
	_list.setColumnHeads("After Row");

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void NotationCallingPosLead::update()
{
	_check.SetCheck(_notation.getDefaultCallPosLead());
	
	//empty the list;
	_list.deleteAllPropItems();

	BOOL editable = !_notation.getDefaultCallPosLead();
	
	for (int i=0;i<_notation.getCallPosLeadCount();i++)
	{
		CString callPosLead;
		callPosLead.Format("%d", _notation.getCallPosLead(i) + (globalDataManager()->isStartAtRowZero()?0:1));
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, callPosLead);
		_list.AddPropItem(propItem);
	}

}



void NotationCallingPosLead::OnCallPosAdd() 
{
	if (_notation.getDefaultCallPosLead()) return;

	CallPosLeadEditDlg dlg(this, _notation);

	while (dlg.DoModal() == IDOK)
	{
		CallPosLeadError ret = _notation.addCallPosLead(dlg._callPosLead - (globalDataManager()->isStartAtRowZero()?0:1));
		if ((ret == CPLE_OK)||(ret == CPLE_OK_DEFAULT))
		{			
			update();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddCallPosLeadErrorText(ret));
		}
	} 
}


void NotationCallingPosLead::OnCallPosRemove() 
{
	//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if (pItem)
	{
		int pos = atoi(pItem->_col[0]) -1 ;
			
		if (pos >= 0)
		{
			_notation.deleteCallPosLead(_notation.findCallPosLead(pos));
		}
	}	
	update();
}


LRESULT NotationCallingPosLead::OnEditCallingPos(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
		//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if ((pItem)&&(!_notation.getDefaultCallPosLead()))
	{
		//find the call position from the prop item //lets be really safe here
		int pos = atoi(pItem->_col[0]) -(globalDataManager()->isStartAtRowZero()?0:1) ;
			
		if (pos >= 0)
		{
			int index = _notation.findCallPosLead(pos);
			if (index != -1) 
			{
			
				//edit the call
				CallPosLeadEditDlg dlg(this,_notation, pos);

				while (dlg.DoModal() == IDOK)
				{
					CallPosLeadError ret = _notation.changeCallPosLead(index, dlg._callPosLead - (globalDataManager()->isStartAtRowZero()?0:1));
					if ((ret == CPLE_OK)||(ret == CPLE_OK_DEFAULT))
					{			
						update();
						break;
					}
					else
					{
						AfxMessageBox(_notation.getAddCallPosLeadErrorText(ret));
					}
				}
			}
		}

	}	
	update();

	return 0;	
}


void NotationCallingPosLead::OnCallPosCheck() 
{
	_notation.setDefaultCallPosLead(_check.GetCheck()); 

	update();	
}

void NotationCallingPosLead::OnUpdateAdd(CCmdUI* pCmdUI) //add an item
{
	_notation.prepare();

	pCmdUI->Enable(!_notation.getDefaultCallPosLead()&&
				   (_notation.getChangesPerPlainLead() > 0));
}

//remove button
void NotationCallingPosLead::OnUpdateRemove(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	pCmdUI->Enable(!_notation.getDefaultCallPosLead()&&
					 pItem != NULL);
}


//adds the default call pos (lead / method) to the list
void NotationCallingPosLead::OnUpdateDefault(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_notation.getDefaultCallPosLead());
}



BOOL NotationCallingPosLead::OnToolTipNotify( UINT /*id*/, NMHDR * pNMHDR, LRESULT * /*pResult*/ )
{    
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;    
	
	UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)    
	{
		// idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);        

		if ((nID == IDC_CALL_POS_CHECK)&&
			(_check.IsWindowVisible())&&
			(!_check.IsWindowEnabled()))
		{
			pTTT->lpszText = "To enable the use of default lead calling positions, you must use default calls";		
		}

		return TRUE;            			
	}    		 
	return FALSE;
}

void NotationCallingPosLead::OnCallPosAddDefault() 
{
	if (_notation.getDefaultCallPosLead()) return;

	_notation.addCallPosLeadDefault();

	update();
}

BOOL NotationCallingPosLead::OnSetActive()
{
	update();	

	return CBCGPPropertyPage::OnSetActive();
}
