// GrabOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GrabOrderDlg.h"
#include "RecordManager.h"
#include "RecordGrab.h"
#include "RecordTower.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GrabOrderDlg dialog


GrabOrderDlg::GrabOrderDlg(RecordGrabs& recordGrabs)
	: ResizeDialog(GrabOrderDlg::IDD, NULL),
	_recordGrabs(recordGrabs)
{
	//{{AFX_DATA_INIT(GrabOrderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GrabOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GrabOrderDlg)
	DDX_Control(pDX, IDC_LIST, _list);
	DDX_Control(pDX, IDC_UP, _up);
	DDX_Control(pDX, IDC_DOWN, _down);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GrabOrderDlg, ResizeDialog)
	//{{AFX_MSG_MAP(GrabOrderDlg)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_DOUBLECLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_DOUBLECLICKED(IDC_UP, OnUp)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_DOWN, OnUpdateDown)		   	
	ON_UPDATE_COMMAND_UI(IDC_UP, OnUpdateUp)		   	

END_MESSAGE_MAP()
			  

LRESULT GrabOrderDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

/////////////////////////////////////////////////////////////////////////////
// GrabOrderDlg message handlers


BOOL GrabOrderDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	//sort to the in day order
	sortGrabs();

	update();

	_up.SetImage(IDB_UP);
	_up.m_bDrawFocus = FALSE;
	_up.SetTooltip("Move grab up in the order");
	_up.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_down.SetImage(IDB_DOWN);
	_down.m_bDrawFocus = FALSE;
	_down.SetTooltip("Move grab down in the order");
	_down.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void GrabOrderDlg::OnOK() 
{
	//set the order
	for (int i=0;i<_recordGrabs.GetSize();i++)
		_recordGrabs.GetAt(i)->setDayOrder(i+1);
	
	ResizeDialog::OnOK();
}


int GrabOrderDlg::compareGrabs( const void * arg1, const void * arg2 )
{
	RecordGrab* grab1 = (*(RecordGrab**)arg1);
	RecordGrab* grab2 = (*(RecordGrab**)arg2);

	if (grab1->getDayOrder() == grab2->getDayOrder())
		return 0;
	else 
		return (grab1->getDayOrder() > grab2->getDayOrder())?1:(-1);
}

void GrabOrderDlg::sortGrabs()
{
	int itemCount = _recordGrabs.GetSize();
	RecordGrab** items = new RecordGrab*[itemCount]; 

	for (int i=0;i<itemCount;i++)
		items[i] = _recordGrabs.GetAt(i);

	qsort((void*)items, (size_t)itemCount, sizeof(RecordGrab *), compareGrabs);

	_recordGrabs.RemoveAll();

	for (int i=0;i<itemCount;i++)
	{
		_recordGrabs.Add(items[i]);
		items[i]->setGrabIndex(i+1);
	}

	delete items;  
}

void GrabOrderDlg::OnDown() 
{
	if ((_list.GetCurSel() != LB_ERR)&&(_list.GetCurSel() < _list.GetCount()-1))
	{  
		const int index = _list.GetCurSel();
		RecordGrab* temp = _recordGrabs.GetAt(index);
		_recordGrabs.SetAt(index, _recordGrabs.GetAt(index+1));
		_recordGrabs.SetAt(index+1, temp);

		update();
		_list.SetCurSel(index+1);
	}	
}

void GrabOrderDlg::OnUp() 
{
	if ((_list.GetCurSel() != LB_ERR)&&(_list.GetCurSel() >0))
	{  
		const int index = _list.GetCurSel();
		RecordGrab* temp = _recordGrabs.GetAt(index);
		_recordGrabs.SetAt(index, _recordGrabs.GetAt(index-1));
		_recordGrabs.SetAt(index-1, temp);

		update();

		_list.SetCurSel(index-1);
	}	
}

void GrabOrderDlg::OnUpdateUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_list.GetCurSel() != LB_ERR)&&(_list.GetCurSel() >0));
}

void GrabOrderDlg::OnUpdateDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_list.GetCurSel() != LB_ERR)&&(_list.GetCurSel() < _list.GetCount()-1));
}

void GrabOrderDlg::update()
{
	_list.ResetContentHS();

	for (int i=0;i<_recordGrabs.GetSize();i++)
	{
		RecordGrab* grab = _recordGrabs.GetAt(i);
		
		RecordTower* tower = grab->getTower();
		CString str;
		//debug only str.Format("%d, ", grab->getDayOrder());
		int index = _list.AddStringHS(str + tower->getDisplayName());
		_list.SetItemDataPtr(index, grab);
	}
}
