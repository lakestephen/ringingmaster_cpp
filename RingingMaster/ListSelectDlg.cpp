// ListSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ListSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ListSelectDlg dialog


ListSelectDlg::ListSelectDlg(CWnd* pParent,	CString title, ListSelectItems& available, ListSelectItems& selected, UINT ID) :
ResizeDialog(ID, pParent),
_originalSelected(selected),
_available(available),
_title(title)
{
	for (int i=0;i<_originalSelected.GetSize();i++)
	{
		_selected.Add(_originalSelected.GetAt(i));
	}

	//{{AFX_DATA_INIT(ListSelectDlg)
	//}}AFX_DATA_INIT
}


void ListSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ListSelectDlg)
	DDX_Control(pDX, IDC_LIST_LEFT, _listLeft);
	DDX_Control(pDX, IDC_RIGHT, _right);
	DDX_Control(pDX, IDC_LIST_RIGHT, _listRight);
	DDX_Control(pDX, IDC_LEFT, _left);
	DDX_Control(pDX, IDC_ALL_LEFT, _allLeft);
	DDX_Control(pDX, IDC_ALL_RIGHT, _allRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ListSelectDlg, ResizeDialog)
	//{{AFX_MSG_MAP(ListSelectDlg)
	ON_BN_CLICKED(IDC_ALL_LEFT, OnAllLeft)
	ON_BN_CLICKED(IDC_ALL_RIGHT, OnAllRight)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_UPDATE_COMMAND_UI(IDC_ALL_LEFT, OnUpdateAllLeft)		   	
	ON_UPDATE_COMMAND_UI(IDC_ALL_RIGHT, OnUpdateAllRight)		   	
	ON_UPDATE_COMMAND_UI(IDC_LEFT, OnUpdateLeft)		   	
	ON_UPDATE_COMMAND_UI(IDC_RIGHT, OnUpdateRight)		   	
	ON_LBN_DBLCLK(IDC_LIST_LEFT, OnRight)
	ON_LBN_DBLCLK(IDC_LIST_RIGHT, OnLeft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT ListSelectDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

/////////////////////////////////////////////////////////////////////////////
// ListSelectDlg message handlers

BOOL ListSelectDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();


	SetWindowText(_title);

	AddControl(IDC_LIST_LEFT, none, resize);
	AddControl(IDC_LIST_RIGHT, resize, resize);
	AddControl(IDOK, repos_center, repos);
	AddControl(IDCANCEL, repos_center, repos);	


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

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL ListSelectDlg::isOnAvailableList(ListSelectItem* item)
{
	for (int i=0;i<_available.GetSize();i++)
	{
		if (item == _available.GetAt(i))
			return TRUE;
	}
	return FALSE;
}

BOOL ListSelectDlg::isOnSelectedList(ListSelectItem* item)
{
	for (int i=0;i<_selected.GetSize();i++)
	{
		if (item == _selected.GetAt(i))
			return TRUE;
	}
	return FALSE;
}

void ListSelectDlg::update()
{
	//first add the selected ones to the right
	_listRight.ResetContentHS();
	for (int i=0;i<_selected.GetSize();i++)
	{
		if (isOnAvailableList(_selected.GetAt(i)))
		{
			int index = _listRight.AddStringHS(_selected.GetAt(i)->_text);
			_listRight.SetItemDataPtr(index, _selected.GetAt(i));
		}
	}

	//now add all unused 
	_listLeft.ResetContentHS();
	for (int i=0;i<_available.GetSize();i++)
	{
		if (!isOnSelectedList(_available.GetAt(i)))
		{
			int index = _listLeft.AddStringHS(_available.GetAt(i)->_text);
			_listLeft.SetItemDataPtr(index, _available.GetAt(i));
		}
	} 
}

void ListSelectDlg::OnAllLeft() 
{
	_selected.RemoveAll();
	update();
}

void ListSelectDlg::OnUpdateAllLeft(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_listRight.GetCount() != 0);
}

void ListSelectDlg::OnAllRight() 
{
	_selected.RemoveAll();
	for (int i=0;i<_available.GetSize();i++)
	{
		_selected.Add(_available.GetAt(i));
	}
	update();
}

void ListSelectDlg::OnUpdateAllRight(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_listLeft.GetCount() != 0);
}

void ListSelectDlg::OnLeft() 
{
	int count = _listRight.GetSelCount();
	int* buffer = new int[count];	
	_listRight.GetSelItems(count, buffer);

	for (int i=0;i<count;i++)
	{
		for (int j=0;j<_selected.GetSize();j++)
		{
			if (_listRight.GetItemDataPtr(buffer[i]) == _selected.GetAt(j))
				_selected.RemoveAt(j);
		}
	}

	delete[] buffer;
	
	update();
}

void ListSelectDlg::OnUpdateLeft(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_listRight.GetSelCount() != 0);
}

void ListSelectDlg::OnRight() 
{
	int count = _listLeft.GetSelCount();
	int* buffer = new int[count];	
	_listLeft.GetSelItems(count, buffer);

	for (int i=0;i<count;i++)
	{
		bool onList = false;
		for (int j=0;j<_selected.GetSize();j++)
		{
			if (_listLeft.GetItemDataPtr(buffer[i]) == _selected.GetAt(j))
				onList = true;
		}
		if (!onList)
		{
			ListSelectItem* item = static_cast<ListSelectItem*>(_listLeft.GetItemDataPtr(buffer[i]));
			if (item)
				_selected.Add(item);
		}

	}

	delete[] buffer;

	//now sort items using the available list.
	ListSelectItems tempList;
	for (int i=0;i<_available.GetSize();i++)
	{
		for (int j=0;j<_selected.GetSize();j++)
		{
			if (_available.GetAt(i) == _selected.GetAt(j))
			{
				tempList.Add(_available.GetAt(i));
				break;
			}
		}
	}
	_selected.RemoveAll();

	for (int i=0;i<tempList.GetSize();i++)
		_selected.Add(tempList.GetAt(i));
	
	update();
}

void ListSelectDlg::OnUpdateRight(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_listLeft.GetSelCount() != 0);
}

void ListSelectDlg::OnOK() 
{
	if (_selected.GetSize() == 0)
	{
		AfxMessageBox("Please select 1 or more items", MB_ICONASTERISK);
		return;
	}

	_originalSelected.RemoveAll();

	for (int i=0;i<_selected.GetSize();i++)
		_originalSelected.Add(_selected.GetAt(i));
	
	ResizeDialog::OnOK();
}

