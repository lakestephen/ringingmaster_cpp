// NotationCallingPosMethod.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"
#include "NotationDlg.h"
#include "NotationCallingPosMethod.h"
#include "CallPosLeadEditDlg.h"
#include "CallPosMethodEditDlg.h"
#include "CallPosAgregateEditDlg.h"
#include "NotationCallPosAgregate.h"
#include ".\notationcallingposmethod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosMethod property page

NotationCallingPosMethod::NotationCallingPosMethod( Notation & notation) : 
CBCGPPropertyPage(NotationCallingPosMethod::IDD),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationCallingPosMethod)
	//}}AFX_DATA_INIT
}

NotationCallingPosMethod::~NotationCallingPosMethod()
{
}

void NotationCallingPosMethod::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationCallingPosMethod)
	DDX_Control(pDX, IDC_CALL_POS_ENUM, _addEnum);
	DDX_Control(pDX, IDC_CALL_POS_ADD_DEFAULT, _addDefault);
	DDX_Control(pDX, IDC_CALL_POS, _list);
	DDX_Control(pDX, IDC_CALL_POS_CHECK, _check);
	DDX_Control(pDX, IDC_CALL_POS_REMOVE, _remove);
	DDX_Control(pDX, IDC_CALL_POS_ADD, _add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationCallingPosMethod, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationCallingPosMethod)
	ON_BN_CLICKED(IDC_CALL_POS_REMOVE, OnCallPosRemove)
	ON_BN_CLICKED(IDC_CALL_POS_ADD, OnCallPosAdd)
	ON_BN_CLICKED(IDC_CALL_POS_CHECK, OnCallPosCheck)
	ON_BN_CLICKED(IDC_CALL_POS_ENUM, OnCallPosEnum)
	ON_BN_CLICKED(IDC_CALL_POS_ADD_DEFAULT, OnCallPosAddDefault)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ADD, OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_REMOVE, OnUpdateRemove)
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ADD_DEFAULT, OnUpdateDefault)		   	
	ON_UPDATE_COMMAND_UI(IDC_CALL_POS_ENUM, OnUpdateEnum)		   	
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify) 
	ON_MESSAGE(WM_PROP_BUTTON, OnEditCallingPos)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NotationCallingPosMethod message handlers

BOOL NotationCallingPosMethod::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	EnableToolTips();

	_add.SetImage(IDB_NEW);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add Method Calling Position");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_DELETE);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove Method Calling Position");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addEnum.SetImage(IDB_ADD_NUMBERED);
	_addEnum.m_bDrawFocus = FALSE;
	_addEnum.SetTooltip("Add a numbered position for each possible calling position");
	_addEnum.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addDefault.SetImage(IDB_ADD_DEFAULT);
	_addDefault.m_bDrawFocus = FALSE;
	_addDefault.SetTooltip("Add the default method calling positions to the user definable list");
	_addDefault.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	_list.setColumnHead(true);
	_list.AddPropItem(new PropItem(PT_STATIC, ""));
	_list.setColumnCount(3);
	_list.setColumnHeads("Name", "Lead (of tenor)", "After Row");

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void NotationCallingPosMethod::update()
{

	_check.SetCheck(_notation.getDefaultCallPosMethod());

	//empty the list;
	_list.deleteAllPropItems();

	BOOL editable = !_notation.getDefaultCallPosMethod();
	
	for (int i=0;i<_notation.getCallPosMethodCount();i++)
	{
		NotationCallPosMethod * callPosMet = _notation.getCallPosMethod(i);

		CString pos;
		pos.Format("%d", callPosMet->_pos+(globalDataManager()->isStartAtRowZero()?0:1));

		CString lead;
		lead.Format("%d", callPosMet->_lead+1);
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, callPosMet->_name, lead, pos);
		_list.AddPropItem(propItem);
	}

}



void NotationCallingPosMethod::OnCallPosAdd() 
{
	if (_notation.getDefaultCallPosMethod()) return;

	CallPosMethodEditDlg dlg(this,_notation);

	while (dlg.DoModal() == IDOK)
	{														   
		CallPosMethodError ret = _notation.addCallPosMethod(dlg._lead-1, dlg._row-(globalDataManager()->isStartAtRowZero()?0:1), dlg._name);
		if ((ret == CPME_OK)||(ret == CPME_OK_DEFAULT))
		{			
			update();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddCallPosMethodErrorText(ret));
		}
	}			  
}


void NotationCallingPosMethod::OnCallPosRemove() 
{
	//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if (pItem)
	{
		_notation.deleteCallPosMethod(_notation.findCallPosMethod(atoi(pItem->_col[1])-1, 
																  atoi(pItem->_col[2])-(globalDataManager()->isStartAtRowZero()?0:1)));
	}	
	update();
}


LRESULT NotationCallingPosMethod::OnEditCallingPos(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
		//find the item
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	if ((pItem)&&(!_notation.getDefaultCallPosMethod()))
	{
		//find the lead and row from the prop item

		int lead = atoi(pItem->_col[1]) - 1;
		int row  = atoi(pItem->_col[2]) - (globalDataManager()->isStartAtRowZero()?0:1);

		if ((lead >= 0)&&(row >= 0))
		{
			int index = _notation.findCallPosMethod(lead,row);
			if (index != -1) 
			{

				CallPosMethodEditDlg dlg(this,_notation, pItem->_col[0], lead, row);

				while (dlg.DoModal() == IDOK)
				{														   
					CallPosMethodError ret = _notation.changeCallPosMethod(index, dlg._lead-1, dlg._row-(globalDataManager()->isStartAtRowZero()?0:1), dlg._name);
					if ((ret == CPME_OK)||(ret == CPME_OK_DEFAULT))
					{			
						update();
						break;
					}
					else
					{
						AfxMessageBox(_notation.getAddCallPosMethodErrorText(ret));
					}
				}	
			}
		}
	}	
	update();

	return 0;	
}

void NotationCallingPosMethod::OnCallPosCheck() 
{
	_notation.setDefaultCallPosMethod(_check.GetCheck()); 

	update();	
}

void NotationCallingPosMethod::OnUpdateAdd(CCmdUI* pCmdUI) //add an item
{
	_notation.prepare();

	pCmdUI->Enable(!_notation.getDefaultCallPosMethod()&&
				   (_notation.getChangesPerPlainLead() > 0)&&
				   (_notation.getCallPosLeadCount() > 0));
}

//remove button
void NotationCallingPosMethod::OnUpdateRemove(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());

	pCmdUI->Enable(!_notation.getDefaultCallPosMethod()&&
				   pItem != NULL);
}


//adds the default call pos (lead / method) to the list
void NotationCallingPosMethod::OnUpdateDefault(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_notation.getDefaultCallPosMethod());
}

//enumerate all the calling positions
void NotationCallingPosMethod::OnUpdateEnum(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(!_notation.getDefaultCallPosMethod()&&
				   (_notation.getChangesPerPlainLead() > 0)&&
				   (_notation.getCallPosLeadCount() > 0));
}


BOOL NotationCallingPosMethod::OnToolTipNotify( UINT /*id*/, NMHDR * pNMHDR, LRESULT * /*pResult*/ )
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
			pTTT->lpszText = "To enable the use of default method calling positions, you must use default calls and default lead calling positions";		
		}

		return TRUE;            			
	}    		 
	return FALSE;
}

void NotationCallingPosMethod::OnCallPosEnum() 
{
	if (_notation.getDefaultCallPosMethod()) return;

	bool overwrite = false;
	if (_notation.getCallPosMethodCount() >0)
	{
		if(AfxMessageBox("Do you want to overwrite your existing call positions?", MB_YESNO|MB_ICONQUESTION) == IDYES)
			overwrite = true;
	}

	_notation.addCallPosMethodEnum(overwrite);

	update();
}

void NotationCallingPosMethod::OnCallPosAddDefault() 
{
	if (_notation.getDefaultCallPosMethod()) return;

	bool overwrite = false;
	if (_notation.getCallPosMethodCount() >0)
	{
		if(AfxMessageBox("Do you want to overwrite existsting call positions?", MB_YESNO|MB_ICONQUESTION) == IDYES)
			overwrite = true;
	}

	_notation.addCallPosMethodDefault(overwrite);

	update();
}

BOOL NotationCallingPosMethod::OnSetActive()
{
	update();	

	return CBCGPPropertyPage::OnSetActive();
}
