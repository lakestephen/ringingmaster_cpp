// NotationCallingPosAgregate.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"
#include "NotationDlg.h"
#include "NotationCallingPosAgregate.h"
#include "CallPosLeadEditDlg.h"
#include "CallPosMethodEditDlg.h"
#include "CallPosAgregateEditDlg.h"
#include "NotationCallPosAgregate.h"
#include ".\notationcallingposagregate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosAgregate property page

NotationCallingPosAgregate::NotationCallingPosAgregate( Notation & notation) : 
CBCGPPropertyPage(NotationCallingPosAgregate::IDD),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationCallingPosAgregate)
	//}}AFX_DATA_INIT
}

NotationCallingPosAgregate::~NotationCallingPosAgregate()
{
}

void NotationCallingPosAgregate::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationCallingPosAgregate)
	DDX_Control(pDX, IDC_CALL_POS, _list);
	DDX_Control(pDX, IDC_CALL_POS_REMOVE, _remove);
	DDX_Control(pDX, IDC_CALL_POS_ADD, _add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationCallingPosAgregate, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationCallingPosAgregate)
	ON_BN_CLICKED(IDC_CALL_POS_REMOVE, OnCallPosRemove)
	ON_BN_CLICKED(IDC_CALL_POS_ADD, OnCallPosAdd)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ADD, OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_REMOVE, OnUpdateRemove)
	ON_MESSAGE(WM_PROP_BUTTON, OnEditCallingPos)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosAgregate message handlers

BOOL NotationCallingPosAgregate::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	EnableToolTips();


	_add.SetImage(IDB_NEW);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add Agregate Calling Position");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_DELETE);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove Agregate Calling Position");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_list.setColumnHead(true);
	_list.AddPropItem(new PropItem(PT_STATIC, ""));
	_list.setColumnCount(2);
	_list.setColumnHeads("Agregate Name", "Method Positions");
	_list._devider[0] = 80;

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void NotationCallingPosAgregate::update()
{

	//empty the list;
	_list.deleteAllPropItems();
	
	BOOL editable = TRUE;	

	for (int i=0;i<_notation.getCallPosAgregateCount();i++)
	{
		NotationCallPosAgregate* callPosAg = _notation.getCallPosAgregate(i);
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, callPosAg->_name, callPosAg->getDisplay());
		_list.AddPropItem(propItem);
	}

}



void NotationCallingPosAgregate::OnCallPosAdd() 
{
	NotationCallPosAgregate NACP;

	CallPosAgregateEditDlg dlg(&NACP,_notation);
	while (dlg.DoModal() == IDOK)
	{														   
		CallPosAgregateError ret = _notation.addCallPosAgregate(NACP);
		if (ret == CPAE_OK)
		{			
			update();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddCallPosAgregateErrorText(ret));
		}
	}			  
}


void NotationCallingPosAgregate::OnCallPosRemove() 
{
	//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if (pItem)
	{
		_notation.deleteCallPosAgregate(_notation.findCallPosAgregate(pItem->_col[0]));
	}	
	update();
}


LRESULT NotationCallingPosAgregate::OnEditCallingPos(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
		//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if (pItem)
	{
		int index = _notation.findCallPosAgregate(pItem->_col[0]);
		if (index != -1) 
		{
			//create a new agregate;
			NotationCallPosAgregate* existing = _notation.getCallPosAgregate(index);
			if (existing) 
			{

				NotationCallPosAgregate modifyed(*existing);

				CallPosAgregateEditDlg dlg(&modifyed, _notation);

				while (dlg.DoModal() == IDOK)
				{														   
					CallPosAgregateError ret = _notation.changeCallPosAgregate(index, modifyed);
					if (ret == CPAE_OK)
					{			
						update();
						break;
					}
					else
					{
						AfxMessageBox(_notation.getAddCallPosAgregateErrorText(ret));
					}
				}	
			}
		}

	}	
	update();

	return 0;	
}



void NotationCallingPosAgregate::OnUpdateAdd(CCmdUI* pCmdUI) //add an item
{
	_notation.prepare();

	pCmdUI->Enable(_notation.getCallPosMethodCount() >1);
}

//remove button
void NotationCallingPosAgregate::OnUpdateRemove(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	pCmdUI->Enable(pItem != NULL);
}

BOOL NotationCallingPosAgregate::OnSetActive()
{
	update();	

	return CBCGPPropertyPage::OnSetActive();
}
