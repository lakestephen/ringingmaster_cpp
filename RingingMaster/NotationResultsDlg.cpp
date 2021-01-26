// NotationResultsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationResultsDlg.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationResultsDlg dialog


NotationResultsDlg::NotationResultsDlg(NotationLites& lites, CWnd* pParent, bool noSelect)
: ResizeDialog(NotationResultsDlg::IDD, pParent),
_lites(lites), 
_noSelect(noSelect)
{
	//{{AFX_DATA_INIT(NotationResultsDlg)
	_status = _T("");
	//}}AFX_DATA_INIT
}


void NotationResultsDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationResultsDlg)
	DDX_Text(pDX, IDC_EXPLORE_STATIS, _status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationResultsDlg, ResizeDialog)
	//{{AFX_MSG_MAP(NotationResultsDlg)
		ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT NotationResultsDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{

	updateStatus();
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

/////////////////////////////////////////////////////////////////////////////
// NotationResultsDlg message handlers

BOOL NotationResultsDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();

	_list.Create(LVS_REPORT | 
                    LVS_SHOWSELALWAYS | LVS_EDITLABELS | LVS_OWNERDATA | LVS_SINGLESEL |
                    WS_VISIBLE | WS_TABSTOP, CRect(0,0,245,125), this, IDC_LIST);
	_list.ModifyStyleEx (0,WS_EX_CLIENTEDGE );

	
	AddControl(IDC_LIST, resize, resize);
	AddControl(IDC_EXPLORE_STATIS, resize, repos);
	AddControl(IDOK, repos_center, repos);
	AddControl(IDCANCEL, repos_center, repos);

	WinPos::Load(this, "NotationResultsDlg", true);

	
	_list._liteList = new NotationLite*[_lites.GetSize()];
	_list._liteListCount = _lites.GetSize();

	for (int i=0;i<_lites.GetSize();i++)
	{
		_list._liteList[i] = _lites.GetAt(i);

	}

	_list.setParent(this);
	_list.init();
	_list.showMethods();
	
	//so that we dont delete proper methods (from MainFrame) in the calling function
	_lites.RemoveAll();

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationResultsDlg::OnOK() 
{
	makeSelection();
}

void NotationResultsDlg::makeSelection()
{
	_lites.RemoveAll();

	POSITION pos = _list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
//		TRACE0("No items were selected!\n");
	}
	else
	{
		while (pos)
		{
			int nItem = _list.GetNextSelectedItem(pos);
			
			NotationLite* not = _list._liteList[nItem];
			if (not != NULL)
			{
				NotationLite* sel = new NotationLite;
				*sel = *not;
				_lites.Add(sel);					
			}
		}
	}
	EndDialog(IDOK);

}

BOOL NotationResultsDlg::DestroyWindow() 
{
	WinPos::Save(this, "NotationResultsDlg");
	
	return ResizeDialog::DestroyWindow();
}



void NotationResultsDlg::updateStatus()
{
		_status.Format("%d method%s in folder, %d method%s selected",
		_list.GetItemCount(),
		(_list.GetItemCount() == 1)?"":"s",
		_list.GetSelectedCount(),
		(_list.GetSelectedCount() == 1)?"":"s");


	UpdateData(FALSE);
}
