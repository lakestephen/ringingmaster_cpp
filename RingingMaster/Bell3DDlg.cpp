// Bell3DDlg.cpp +: implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "Bell3DDlg.h"
#include "WinPos.h"
#include "UserMessages.h"  
#include "BellCircleDlg.h"
#include "TowerSoundsDlg.h"
#include "PlayManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Bell3DDlg dialog


Bell3DDlg::Bell3DDlg(TowerSoundsDlg* pParent, Tower& tower)
: ResizeDialog(Bell3DDlg::IDD, pParent),
coll(IDB_3D_EDIT,10),
_origTower(tower), //keep a reference
_zoomLevel(2),
_parent(pParent)
{			
	setup();

	ASSERT(pParent);

	//{{AFX_DATA_INIT(Bell3DDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Bell3DDlg::~Bell3DDlg()
{
}


void Bell3DDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Bell3DDlg)
	DDX_Control(pDX, IDC_STOP, _stop);
	DDX_Control(pDX, ID_APPLY, _apply);
	DDX_Control(pDX, ID_CENTER_OF_GRAVITY, _centerOfGravity);
	DDX_Control(pDX, ID_CENTER, _center);
	DDX_Control(pDX, ID_ZOOM_OUT, _zoomOut);
	DDX_Control(pDX, ID_ZOOM_IN, _zoomIn);
	DDX_Control(pDX, ID_MAKE_CIRCLE, _makeCircle);
	DDX_Control(pDX, IDC_STATUS, _status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Bell3DDlg, ResizeDialog)
	//{{AFX_MSG_MAP(Bell3DDlg)
	ON_BN_CLICKED(ID_MAKE_CIRCLE, OnMakeCircle)
	ON_BN_CLICKED(ID_ZOOM_OUT, OnZoomOut)
	ON_BN_CLICKED(ID_ZOOM_IN, OnZoomIn)
	ON_BN_CLICKED(ID_CENTER, OnCenter)
	ON_BN_CLICKED(ID_CENTER_OF_GRAVITY, OnCenterOfGravity)
	ON_MESSAGE(THREE_D_EDIT_UPDATE_STATUS, OnUpdateStatus)
	ON_MESSAGE(THREE_D_EDIT_ITEM_MOVED, OnItemMoved)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_STOP, OnUpdateStop)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, OnUpdateZoomOut)
	ON_UPDATE_COMMAND_UI(ID_CENTER, OnUpdateCenter)
	ON_UPDATE_COMMAND_UI(ID_CENTER_OF_GRAVITY, OnUpdateCenterOfGravity)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Bell3DDlg message handlers

LRESULT Bell3DDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}


BOOL Bell3DDlg::OnInitDialog() 
{

	ResizeDialog::OnInitDialog();

	//get the position of the place holder static item
	CRect drawRect;
	GetDlgItem(IDC_3D_DUMMY)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	//create the method window
	_3DWnd.Create(this, drawRect, &coll);


	AddControl(IDC_3D_WND, resize, resize);
	AddControl(IDC_STATUS, none, repos);

	WinPos::Load(this, "Bell3DDlg", true);

	GetDlgItem(IDC_3D_WND)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	coll.setupPosition(CPoint(drawRect.Width(), drawRect.Height()),0,0);


	_makeCircle.SetImage(IDB_BELL_CIRCLE);
	_makeCircle.m_bDrawFocus = FALSE;
	_makeCircle.SetTooltip("Arrange the bells to a circle.");
	_makeCircle.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_zoomIn.SetImage(IDB_ZOOM_IN);
	_zoomIn.m_bDrawFocus = FALSE;
	_zoomIn.SetTooltip("Zoom In.");
	_zoomIn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_zoomOut.SetImage(IDB_ZOOM_OUT);
	_zoomOut.m_bDrawFocus = FALSE;
	_zoomOut.SetTooltip("Zoom Out.");
	_zoomOut.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_center.SetImage(IDB_CENTER);
	_center.m_bDrawFocus = FALSE;
	_center.SetTooltip("Center window on user.");
	_center.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_centerOfGravity.SetImage(IDB_BELL_AND_DOTS);
	_centerOfGravity.m_bDrawFocus = FALSE;
	_centerOfGravity.SetTooltip("Place the user at the center of gravity of the bell circle.");
	_centerOfGravity.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_apply.SetImage(IDB_BELL);
	_apply.m_bDrawFocus = FALSE;
	_apply.SetTooltip("Test the profile sounds.");
	_apply.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_stop.SetImage(IDB_STOP);
	_stop.m_bDrawFocus = FALSE;
	_stop.SetTooltip("Stop the playing method.");
	_stop.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;  
	

	return TRUE;  // return TRUE unless you set the focus to a control

}



void Bell3DDlg::OnStop() 
{
	playManager()->OnPlayStop();
}

void Bell3DDlg::OnUpdateStop(CCmdUI* pCmdUI) 
{
	playManager()->OnUpdateStopMethod(pCmdUI);
}


void Bell3DDlg::OnOK() 
{
	ResizeDialog::OnOK();
}

void Bell3DDlg::OnCancel() 
{
	writePositions();

	ResizeDialog::OnCancel();
}

void Bell3DDlg::writePositions() 		 
{

	for (int i=0;i<_origTower.getNumber();i++)
	{
		ThreeDimEditItem* item = coll.getItem(i); //last item is the user
		if (item)
		{
			TowerBellPersist& bell = _origTower.getTowerBellPersist(i);
			bell.setPosX(item->getXMeters());
			bell.setPosZ(-item->getYMeters());
			
		}
	}
	
}

BOOL Bell3DDlg::DestroyWindow() 		 
{
	WinPos::Save(this, "Bell3DDlg");
	
	return ResizeDialog::DestroyWindow();
}

LRESULT Bell3DDlg::OnItemMoved(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//get the position of the user.
	ThreeDimEditItem* item = coll.getItemUsingID(-1);

	if ((item)&&((item->getXMeters() != 0)||(item->getYMeters() != 0)))
	{
		//get the data into the Tower collection
		writePositions();		

		//do the modifications to the Tower
		_origTower.moveUser(-item->getXMeters(), 0 ,item->getYMeters());
		
		//bring back in again
		setup();
		Invalidate();
	}

	return 0;
}

LRESULT Bell3DDlg::OnUpdateStatus(WPARAM /*wParam*/, LPARAM lParam)
{
	CString* strStatus = (CString*)lParam;

	_status.SetWindowText(*strStatus);
	
	delete strStatus;
	return 0;
}

void Bell3DDlg::OnMakeCircle() 
{
	BellCircleDlg dlg(this, _origTower.getNumber());

	if (dlg.DoModal() == IDOK)
	{
		_origTower.arrangerBellsInCircle(dlg._number-1, dlg._meters, (dlg._clockwise == TRUE), (dlg._userInCenter == TRUE));
		setup();
		Invalidate();
	}
}

void Bell3DDlg::setup()
{
	coll.removeAllItems();

	//add the bells
	for (int i=0;i<_origTower.getNumber();i++)
	{
		CString tooltip;
		tooltip.Format("Bell %d", i+1);
		const TowerBellPersist& bell = _origTower.getTowerBellPersist(i);
		CString text;
		text.Format("%d", i+1);
		coll.addItem(new ThreeDimEditItem(coll, i, bell.getPosX(), -bell.getPosZ(),1,true, tooltip, text));		
	}

	//add the user last so the other selectable items get selected first
	coll.addItem(new ThreeDimEditItem(coll, -1,0,0,0,true, "User listen point", ""));

	switch (_zoomLevel)
	{
	case 0:
		coll.setZoom(6,1);
		break;
	case 1:
		coll.setZoom(12,1);
		break;
	case 2:
		coll.setZoom(25,1);
		break;
	case 3:
		coll.setZoom(50,1);
		break;
	case 4:
		coll.setZoom(100,1);
		break;
	default:
		coll.setZoom(25,1);
		break;
	}

}

void Bell3DDlg::OnZoomOut() 
{
	if (_zoomLevel > 0)
	{
		_zoomLevel-- ;

		writePositions();
		setup();

		CRect drawRect;
		GetDlgItem(IDC_3D_WND)->GetWindowRect(&drawRect);
		ScreenToClient(&drawRect);

		coll.setupPosition(CPoint(drawRect.Width(), drawRect.Height()),0,0);

		Invalidate();
	}
	
}

void Bell3DDlg::OnZoomIn() 
{
	if (_zoomLevel < 4)
	{
		_zoomLevel++;
		
		writePositions();
		setup();

		CRect drawRect;
		GetDlgItem(IDC_3D_WND)->GetWindowRect(&drawRect);
		ScreenToClient(&drawRect);

		coll.setupPosition(CPoint(drawRect.Width(), drawRect.Height()),0,0);

		Invalidate();
	}
}

void Bell3DDlg::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_zoomLevel < 4);
}

void Bell3DDlg::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_zoomLevel > 0);
}

void Bell3DDlg::OnCenter() 
{

	CRect drawRect;
	GetDlgItem(IDC_3D_WND)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);
	
	coll.setupPosition(CPoint(drawRect.Width(), drawRect.Height()),0,0);

	Invalidate();
	
}

void Bell3DDlg::OnUpdateCenter(CCmdUI* pCmdUI) 
{
	CRect drawRect;
	GetDlgItem(IDC_3D_WND)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	BOOL val = coll.setupPositionWouldMove(CPoint(drawRect.Width(), drawRect.Height()),0,0);

	pCmdUI->Enable(val);
}

void Bell3DDlg::OnUpdateCenterOfGravity(CCmdUI* pCmdUI) 
{
	//get the data into the Tower collection
	writePositions();		

	//do the modifications to the Tower
	BOOL val = _origTower.placeUserAtCenterOfGravityWouldChange();

	pCmdUI->Enable(val);
}

void Bell3DDlg::OnCenterOfGravity() 
{
	//get the data into the Tower collection
	writePositions();		

	//do the modifications to the Tower
	_origTower.placeUserAtCenterOfGravity();
	
	//bring back in again
	setup();
	Invalidate();
}

void Bell3DDlg::OnApply() 
{
	writePositions();

	if (_parent->apply())
		playManager()->test(false);	 
}
