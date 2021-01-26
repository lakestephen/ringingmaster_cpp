// OptionsGrabs.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsGrabs.h"
#include "UserManager.h"
#include "OptionsDlg.h"
#include "UserMessages.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsGrabs dialog


OptionsGrabs::OptionsGrabs()
	: CBCGPPropertyPage(OptionsGrabs::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsGrabs)
	//}}AFX_DATA_INIT
}


void OptionsGrabs::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsGrabs)
	DDX_Control(pDX, IDC_RESET, _reset);
	DDX_Control(pDX, IDC_LIST_RIGHT, _rightCtrl);
	DDX_Control(pDX, IDC_LIST_LEFT, _leftCtrl);
	DDX_Control(pDX, IDC_UP, _up);
	DDX_Control(pDX, IDC_RIGHT, _right);
	DDX_Control(pDX, IDC_LEFT, _left);
	DDX_Control(pDX, IDC_DOWN, _down);
	DDX_Control(pDX, IDC_ALL_RIGHT, _allRight);
	DDX_Control(pDX, IDC_ALL_LEFT, _allLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsGrabs, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsGrabs)
	ON_BN_CLICKED(IDC_ALL_LEFT, OnAllLeft)
	ON_BN_CLICKED(IDC_ALL_RIGHT, OnAllRight)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_DOUBLECLICKED(IDC_DOWN, OnDown)
	ON_BN_DOUBLECLICKED(IDC_UP, OnUp)
	ON_LBN_DBLCLK(IDC_LIST_LEFT, OnRight)
	ON_LBN_DBLCLK(IDC_LIST_RIGHT, OnLeft)
	ON_UPDATE_COMMAND_UI(IDC_ALL_LEFT, OnUpdateAllLeft)		   	
	ON_UPDATE_COMMAND_UI(IDC_ALL_RIGHT, OnUpdateAllRight)		   	
	ON_UPDATE_COMMAND_UI(IDC_LEFT, OnUpdateLeft)		   	
	ON_UPDATE_COMMAND_UI(IDC_RIGHT, OnUpdateRight)	
	ON_UPDATE_COMMAND_UI(IDC_UP, OnUpdateUp)		   	
	ON_UPDATE_COMMAND_UI(IDC_DOWN, OnUpdateDown)	
	ON_BN_CLICKED(IDC_RESET, onReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
					


/////////////////////////////////////////////////////////////////////////////
// OptionsGrabs message handlers

BOOL OptionsGrabs::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_allLeft.m_bDrawFocus = FALSE;
	_allLeft.SetTooltip("Deselect all items");
	_allLeft.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_allRight.m_bDrawFocus = FALSE;
	_allRight.SetTooltip("Select all items");
	_allRight.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_left.m_bDrawFocus = FALSE;
	_left.SetTooltip("Deselect hilighted items");
	_left.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_right.m_bDrawFocus = FALSE;
	_right.SetTooltip("Select hilighted items");
	_right.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_up.SetImage(IDB_UP);
	_up.m_bDrawFocus = FALSE;
	_up.SetTooltip("Move Item Up");
	_up.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_down.SetImage(IDB_DOWN);
	_down.m_bDrawFocus = FALSE;
	_down.SetTooltip("Move Item Down");
	_down.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_reset.SetImage(IDB_RESET);
	_reset.m_bDrawFocus = FALSE;
	_reset.SetTooltip("Reset The Field List");
	_reset.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}		   

void OptionsGrabs::update()
{
	/* todo
	const AvailableFields& available = recordManager()->getAvailableFields(vt_grab);

	_leftCtrl.ResetContentHS();

	for (int i=0;i<available.GetSize();i++)
	{				
		if (!isOnSelectedList(available.GetAt(i)))
		{
			int index = _leftCtrl.AddStringHS(recordManager()->getAvailableFieldsName(available.GetAt(i)));
			_leftCtrl.SetItemData(index, available.GetAt(i));
		}			
	}

	const AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	_rightCtrl.ResetContentHS();

	for (int i=0;i<selected.GetSize();i++)
	{				
		if (isOnAvailableList(selected.GetAt(i)))
		{
			int index = _rightCtrl.AddStringHS(recordManager()->getAvailableFieldsName(selected.GetAt(i)));
			_rightCtrl.SetItemData(index, selected.GetAt(i));
		}
	}
*/
}

BOOL OptionsGrabs::isOnSelectedList(AvailableField availableField)
{  
	/* todo 	const AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	for (int i=0;i<selected.GetSize();i++)
	{
		if (availableField == selected.GetAt(i))
			return TRUE;
	}*/
	return FALSE;
}


BOOL OptionsGrabs::isOnAvailableList(AvailableField selectedField)
{  
/*	todo const AvailableFields& available = recordManager()->getAvailableFields(vt_grab);

	for (int i=0;i<available.GetSize();i++)
	{
		if (selectedField == available.GetAt(i))
			return TRUE;
	}*/
	return FALSE;
}

void OptionsGrabs::OnAllLeft() 
{
	/* todo AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);
	selected.RemoveAll();	
	recordManager()->resetAllGrabListColumnWidths();
	update();	*/
}

void OptionsGrabs::OnAllRight() 
{
	/* todo const AvailableFields& available = recordManager()->getAvailableFields(vt_grab);
	AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);
	AvailableFields tempList;

	for (int i=0;i<available.GetSize();i++)
	{				
		if (!isOnSelectedList(available.GetAt(i)))
		{
			tempList.Add(available.GetAt(i));
		}			
	}

	if (tempList.GetSize() != 0)
	{
		//sort
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (int i=0;i<tempList.GetUpperBound();i++)
			{
				if (recordManager()->getAvailableFieldsName(tempList.GetAt(i)) >  recordManager()->getAvailableFieldsName(tempList.GetAt(i+1)))
				{
					changed = true;
					AvailableField temp    = tempList.GetAt(i);
					tempList.SetAt(i,tempList.GetAt(i+1));
					tempList.SetAt(i+1, temp);
				}
			}
		}
		//now add to selected in order
		for(int i=0;i<tempList.GetSize();i++)
			selected.Add(tempList.GetAt(i));
	}

	recordManager()->resetAllGrabListColumnWidths();
	update();	*/
	
}

void OptionsGrabs::OnLeft() 
{
/*	todo AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	int count = _rightCtrl.GetSelCount();
	int* buffer = new int[count];	
	_rightCtrl.GetSelItems(count, buffer);

	for (int i=0;i<count;i++)
	{
		for (int j=0;j<selected.GetSize();j++)
		{
			if (_rightCtrl.GetItemData(buffer[i]) == (unsigned long)selected.GetAt(j))
				selected.RemoveAt(j);
		}
	}

	delete[] buffer;   

	recordManager()->resetAllGrabListColumnWidths();
	update();	*/
	
}

void OptionsGrabs::OnRight() 
{			  
	/* todo 	AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	int count = _leftCtrl.GetSelCount();
	int* buffer = new int[count];	
	_leftCtrl.GetSelItems(count, buffer);

	for (int i=0;i<count;i++)
	{
		bool onList = false;
		for (int j=0;j<selected.GetSize();j++)
		{
			if (_leftCtrl.GetItemData(buffer[i]) == (unsigned long)selected.GetAt(j))
				onList = true;
		}
		if (!onList)
		{
			selected.Add((AvailableField)_leftCtrl.GetItemData(buffer[i]));
		}

	}

	delete[] buffer;
	
	recordManager()->resetAllGrabListColumnWidths();
	update();	*/

}

void OptionsGrabs::OnUp() 
{	/* todo 
	AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	int count = _rightCtrl.GetSelCount();
	ASSERT (count == 1);
	if (count != 1)
		return;

	int* buffer = new int[count];	
	_rightCtrl.GetSelItems(count, buffer);

	ASSERT(buffer[0] >0);
	if (buffer[0] <= 0)
		return;

	//get the ref of the single selected item.
	AvailableField field = (AvailableField)_rightCtrl.GetItemData(buffer[0]);

	for (int i=0;i<selected.GetSize();i++)
	{
		if (selected.GetAt(i) == field)
		{
			AvailableField temp = selected.GetAt(i-1);
			selected.SetAt(i-1, field);
			selected.SetAt(i, temp);
			break;
		}
	}	
	
	delete[] buffer;

	recordManager()->resetAllGrabListColumnWidths();
	update();	
	
	//now reselect the item.
	selectAvailableFields(field);*/
}

void OptionsGrabs::OnDown() 
{
		/* todo AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

	int count = _rightCtrl.GetSelCount();
	ASSERT (count == 1);
	if (count != 1)
		return;

	int* buffer = new int[count];	
	_rightCtrl.GetSelItems(count, buffer);

	ASSERT(buffer[0] != _rightCtrl.GetCount()-1);
	if (buffer[0] == _rightCtrl.GetCount()-1)
		return;

	//get the ref of the single selected item.
	AvailableField field = (AvailableField)_rightCtrl.GetItemData(buffer[0]);

	for (int i=0;i<selected.GetSize();i++)
	{
		if (selected.GetAt(i) == field)
		{
			AvailableField temp = selected.GetAt(i+1);
			selected.SetAt(i+1, field);
			selected.SetAt(i, temp);
			break;
		}
	}	
	
	delete[] buffer;

	recordManager()->resetAllGrabListColumnWidths();
	update();	
	
	//now reselect the item.
	selectAvailableFields(field);
*/
}

void OptionsGrabs::OnUpdateAllLeft(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_rightCtrl.GetCount() != 0);
}

void OptionsGrabs::OnUpdateAllRight(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_leftCtrl.GetCount() != 0);
}

void OptionsGrabs::OnUpdateLeft(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_rightCtrl.GetSelCount() != 0);
}

void OptionsGrabs::OnUpdateRight(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_leftCtrl.GetSelCount() != 0);
}

void OptionsGrabs::OnUpdateUp(CCmdUI* pCmdUI)  
{
	int count = _rightCtrl.GetSelCount();
	BOOL enable = FALSE;

	if (count == 1)
	{
		int* buffer = new int[count];	
		_rightCtrl.GetSelItems(count, buffer);

		enable = (buffer[0] != 0);

		delete[] buffer;

	}

	pCmdUI->Enable(enable);
}

void OptionsGrabs::OnUpdateDown(CCmdUI* pCmdUI)  
{
	int count = _rightCtrl.GetSelCount();
	BOOL enable = FALSE;

	if (count == 1)
	{
		int* buffer = new int[count];	
		_rightCtrl.GetSelItems(count, buffer);

		enable = (buffer[0] != _rightCtrl.GetCount() -1);

		delete[] buffer;

	}

	pCmdUI->Enable(enable);
}


void OptionsGrabs::selectAvailableFields(AvailableField field)
{
	for (int i=0;i<_rightCtrl.GetCount();i++)
	{
		if (_rightCtrl.GetItemData(i) == (unsigned long)field)
			_rightCtrl.SetSel(i);
		else
			_rightCtrl.SetSel(i, FALSE);
	}
}

void OptionsGrabs::onReset() 
{	/* todo
	if(AfxMessageBox("Are you sure you want to reset the fields list?", MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		recordManager()->setupSelectedGrabFieldsLists();
		recordManager()->resetAllGrabListColumnWidths();
		update();
	}		*/
}

BOOL OptionsGrabs::OnKillActive()
{	/* todo 
	if (CBCGPPropertyPage::OnKillActive())
	{
		const AvailableFields& selected = recordManager()->getSelectedFields(vt_grab);

		if (selected.GetSize() < 1)
		{
			AfxMessageBox("Please select at least one field.");
			return FALSE;
		}

		return TRUE;
	}
*/
	return FALSE;
}

BOOL OptionsGrabs::OnSetActive()
{
	if (!userManager()->isValid())
	{
		AfxMessageBox("Please add or select a user before editing the grabs options.", MB_ICONHAND);
		
		GetParent()->PostMessage( OPTIONS_SELECT_USER_PAGE);

		return FALSE;
	}
	
	return CBCGPPropertyPage::OnSetActive(); 
}
