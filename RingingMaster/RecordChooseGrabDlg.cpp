// RecordChooseGrabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordChooseGrabDlg.h"
#include "RecordManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RecordChooseGrabDlg::RecordChooseGrabDlg(CWnd* pParent, CPoint requestPosition, RecordGrab* initialGrab, RecordGrabs* recordGrabs)
	: CDialog(RecordChooseGrabDlg::IDD, pParent),
	_requestPosition(requestPosition), 
	_recordGrabs(recordGrabs),
	_recordGrab(NULL),
	_initialGrab(initialGrab)
{
	//{{AFX_DATA_INIT(RecordChooseGrabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void RecordChooseGrabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RecordChooseGrabDlg)
	DDX_Control(pDX, IDC_TOWER_COMBO, _grabs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RecordChooseGrabDlg, CDialog)
	//{{AFX_MSG_MAP(RecordChooseGrabDlg)
	ON_CBN_SELCHANGE(IDC_TOWER_COMBO, OnSelchangeGrabCombo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecordChooseGrabDlg message handlers

BOOL RecordChooseGrabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/*	todo
	if (_initialGrab == NULL)
		_initialGrab = recordManager()->recordManager_notifyGetSelectedGrab();

	recordManager()->populateAllGrabs(_grabs, _initialGrab, _recordGrabs);

	if ((_requestPosition.x != -1)&&(_requestPosition.y != -1))
		GlobalFunctions::negotiateSpace(this, _requestPosition); 
	  */
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RecordChooseGrabDlg::OnSelchangeGrabCombo() 
{
	if (_grabs.GetCurSel() != CB_ERR)
	{
		_recordGrab = static_cast<RecordGrab*>(_grabs.GetItemDataPtr(_grabs.GetCurSel()));
		EndDialog(IDOK);
	}	
}

RecordGrab* RecordChooseGrabDlg::getRecordGrab()
{		 
	return _recordGrab;
}

void RecordChooseGrabDlg::OnClose() 
{
	if (_grabs.GetCurSel() != CB_ERR)
	{
		_recordGrab = static_cast<RecordGrab*>(_grabs.GetItemDataPtr(_grabs.GetCurSel()));
	}
	
	CDialog::OnClose();
}

