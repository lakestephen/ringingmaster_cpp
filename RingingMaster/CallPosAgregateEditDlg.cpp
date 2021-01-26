// CallPosAgregateEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CallPosAgregateEditDlg.h"
#include "CallPosMethodSelDlg.h"
#include "PropItem.h"
#include "UserMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CallPosAgregateEditDlg dialog


CallPosAgregateEditDlg::CallPosAgregateEditDlg(NotationCallPosAgregate * agregateCallingPos, Notation& notation)
	: CDialog(CallPosAgregateEditDlg::IDD, NULL),
	_originalAgregateCallingPos(agregateCallingPos),
	_agregateCallingPos(*agregateCallingPos),
	_notation(notation)
{
	//{{AFX_DATA_INIT(CallPosAgregateEditDlg)
	_name = _T("");
	//}}AFX_DATA_INIT
}


void CallPosAgregateEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CallPosAgregateEditDlg)
	DDX_Control(pDX, IDC_MOVE_UP, _up);
	DDX_Control(pDX, IDC_MOVE_DOWN, _down);
	DDX_Control(pDX, IDC_AGREGATE, _agregate);
	DDX_Control(pDX, IDC_REMOVE_AGREGATE, _remove);
	DDX_Control(pDX, IDC_ADD_AGREGATE, _add);
	DDX_Text(pDX, IDC_AGREGATE_NAME, _name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CallPosAgregateEditDlg, CDialog)
	//{{AFX_MSG_MAP(CallPosAgregateEditDlg)
	ON_BN_CLICKED(IDC_ADD_AGREGATE, OnAddAgregate)
	ON_BN_CLICKED(IDC_REMOVE_AGREGATE, OnRemoveAgregate)
	ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_AGREGATE, OnUpdateRemove)		   	
	ON_UPDATE_COMMAND_UI(IDC_MOVE_UP, OnUpdateUp)		   	
	ON_UPDATE_COMMAND_UI(IDC_MOVE_DOWN, OnUpdateDown)	
	ON_MESSAGE(WM_PROP_BUTTON, OnEditCallingPos)
END_MESSAGE_MAP()

LRESULT CallPosAgregateEditDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

void CallPosAgregateEditDlg::OnUpdateRemove(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());
	pCmdUI->Enable(pItem != NULL);
}

void CallPosAgregateEditDlg::OnUpdateUp(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());
	pCmdUI->Enable(pItem != NULL && _agregate.GetCurSel() != 1);
}

void CallPosAgregateEditDlg::OnUpdateDown(CCmdUI* pCmdUI) 
{
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());
	pCmdUI->Enable(pItem != NULL && _agregate.GetCurSel() != _agregate.GetCount()-1);
}

/////////////////////////////////////////////////////////////////////////////
// CallPosAgregateEditDlg message handlers

void CallPosAgregateEditDlg::OnAddAgregate() 
{
	//create a list of the names of the call pos methods that have not been used yet.
	CStringArray callPosMethods;
	for (int i=0;i<_notation.getCallPosMethodCount();i++)
	{
		bool used = false;
		for (int j=0;j< _agregateCallingPos._callPosMethods.GetSize();j++)
		{
			if (_agregateCallingPos._callPosMethods.GetAt(j) ==
				_notation.getCallPosMethod(i)->_name)
				used = true;
		}

		if (!used) 
			callPosMethods.Add(_notation.getCallPosMethod(i)->_name);
	}
	
	if (callPosMethods.GetSize() <1)
	{
		AfxMessageBox("There are no more to add.");
		return;
	}

	if (callPosMethods.GetSize() == 1)
	{
		_agregateCallingPos._callPosMethods.Add(callPosMethods.GetAt(0));
	}
	else
	{
		CallPosMethodSelDlg dlg(&callPosMethods);
		if (dlg.DoModal() == IDOK)
		{
			_agregateCallingPos._callPosMethods.Add(dlg._selection);	
		}		
	}

	update();	
}

void CallPosAgregateEditDlg::OnRemoveAgregate() 
{
	//find the item
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());

	if (pItem)
	{
		for (int i=0;i<_agregateCallingPos._callPosMethods.GetSize();i++)
		{
			if (_agregateCallingPos._callPosMethods.GetAt(i) == pItem->_col[0])
				_agregateCallingPos._callPosMethods.RemoveAt(i);
		}
	}

	update();
	
	
}

LRESULT CallPosAgregateEditDlg::OnEditCallingPos(WPARAM /*wParam*/, LPARAM lParam)	
{
	PropItem * pItem = (PropItem*)lParam;
	if (!pItem) return 0;

	//create a list of the names of the call pos methods that have not been used yet.
	CStringArray callPosMethods;
	int index = 0;

	//remove the one that we are interested in from the array
	for (int i=0;i<_agregateCallingPos._callPosMethods.GetSize();i++)
	{
		if (_agregateCallingPos._callPosMethods.GetAt(i) == pItem->_col[0])
		{
			_agregateCallingPos._callPosMethods.RemoveAt(i);
			index = i;
		}
	}

	for (int i=0;i<_notation.getCallPosMethodCount();i++)
	{
		bool used = false;
		for (int j=0;j< _agregateCallingPos._callPosMethods.GetSize();j++)
		{
			if (_agregateCallingPos._callPosMethods.GetAt(j) ==
				_notation.getCallPosMethod(i)->_name)
				used = true;
		}

		if (!used) 
			callPosMethods.Add(_notation.getCallPosMethod(i)->_name);
	}
	
	//make sure that the one we are interested in is passed as an index
	int passedIndex = -1;
	for (int i=0;i<callPosMethods.GetSize();i++) 
	{
		if (callPosMethods.GetAt(i) == pItem->_col[0])
		{
			passedIndex = i;
			break;
		}
	}
	
	if (callPosMethods.GetSize() < 1)
	{
		OnRemoveAgregate();
		AfxMessageBox("The selection is being removed as there are no valid selections to make.");
	}		
	else if (callPosMethods.GetSize() == 1)
	{
		AfxMessageBox("No valid selections other than the one already chosen.");
		_agregateCallingPos._callPosMethods.InsertAt(index,pItem->_col[0]);	
	}
	else
	{
		CallPosMethodSelDlg dlg(&callPosMethods, passedIndex);
		if (dlg.DoModal() == IDOK)
		{
			//now add the new one at the same index -  may be the same item as we have just removed
			_agregateCallingPos._callPosMethods.InsertAt(index,dlg._selection);	
		}		
		else  //if we cancle, we must re add at the same index
		{
			_agregateCallingPos._callPosMethods.InsertAt(index,pItem->_col[0]);	
		}													  
	}

	update();
			 
	return 0;
}

BOOL CallPosAgregateEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips();
	
	_add.SetImage(IDB_NEW);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add to Agregate");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_DELETE);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove from Agregate");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_up.SetImage(IDB_UP);
	_up.m_bDrawFocus = FALSE;
	_up.SetTooltip("Move Item Up");
	_up.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_down.SetImage(IDB_DOWN);
	_down.m_bDrawFocus = FALSE;
	_down.SetTooltip("Move Item Down");
	_down.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	PropItem * propItem;

	//method referanced
	_agregate.setColumnCount(1);
	_agregate.setColumnHead(true);
	
	propItem = new PropItem(PT_STATIC, "Call Pos Method");
	_agregate.AddPropItem(propItem);	

	_name = _agregateCallingPos._name;
	UpdateData(FALSE);

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CallPosAgregateEditDlg::OnOK() 
{
	UpdateData();

	if (_name.IsEmpty())
	{
		AfxMessageBox("Please choose a name.");
		return;
	}

	if (_agregateCallingPos._callPosMethods.GetSize() < 2)
	{
		AfxMessageBox("Please choose at lease two calling positions.");
		return;
	}

	UpdateData();

	_agregateCallingPos._name = _name;

	*_originalAgregateCallingPos = _agregateCallingPos;
	
	CDialog::OnOK();
}


void CallPosAgregateEditDlg::update()
{
	_agregate.deleteAllPropItems();

	for (int i=0;i<_agregateCallingPos._callPosMethods.GetSize();i++)
	{
		PropItem * propItem = new PropItem(PT_BUTTON, _agregateCallingPos._callPosMethods.GetAt(i));
		_agregate.AddPropItem(propItem);	
	}
}

void CallPosAgregateEditDlg::OnMoveDown() 
{
	//get the string
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());
	if (pItem)
	{
		CString str = pItem->_col[0];
		for (int i=0;i<_agregateCallingPos._callPosMethods.GetUpperBound();i++)
		{
			if (str == _agregateCallingPos._callPosMethods.GetAt(i)) 
			{
				_agregateCallingPos._callPosMethods.SetAt(i,
					_agregateCallingPos._callPosMethods.GetAt(i+1));

				_agregateCallingPos._callPosMethods.SetAt(i+1, str);
			}
		}
	}

	update();	
}

void CallPosAgregateEditDlg::OnMoveUp() 
{
//get the string
	PropItem * pItem = _agregate.getPropItem(_agregate.GetCurSel());
	if (pItem)
	{
		CString str = pItem->_col[0];
		for (int i=1;i<_agregateCallingPos._callPosMethods.GetSize();i++)
		{
			if (str == _agregateCallingPos._callPosMethods.GetAt(i)) 
			{
				_agregateCallingPos._callPosMethods.SetAt(i,
					_agregateCallingPos._callPosMethods.GetAt(i-1));

				_agregateCallingPos._callPosMethods.SetAt(i-1, str);
			}
		}
	}

	update();	
}
