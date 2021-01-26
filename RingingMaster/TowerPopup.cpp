// TowerPopup.cpp: implementation of the TowerPopup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerPopup.h"
#include "Tower.h"
#include "TowerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(TowerPopup, AutoPopup)
	//{{AFX_MSG_MAP(TowerPopup)
	ON_CBN_SELCHANGE(AAA_POPUP_TOWER_COMBO, OnSelchangeOptionsTowerCombo)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



TowerPopup::TowerPopup()
{

}

TowerPopup::~TowerPopup()
{

}

BOOL TowerPopup::Create(CWnd* pParentWnd)
{
	BOOL res = AutoPopup::Create(pParentWnd);

	if (res)
	{
		_comboTower.Create(this);
		SetTimer(AAA_KILL_FOCUS_TIMER, 100, NULL);
	}

	towerManager()->addEventListener(this);

    return res;
}

void TowerPopup::Set(CPoint point)
{
   	towerManager_notifyUpdateTowerList();	

	// determine the window size and position
	// displayed above and centered on the origin
	CRect wndRect(point.x, 
			point.y, 
			point.x + 125,
			point.y + 27); 
			
	//ckeck it can be shown.
	CRect screen;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &screen, SPIF_UPDATEINIFILE);
	
	CPoint scrOffset;
	scrOffset.x = screen.right - wndRect.right - 8;
	scrOffset.y = screen.bottom - wndRect.bottom - 8;
	
	if (scrOffset.x > 0) scrOffset.x = 0;
	if (scrOffset.y > 0) scrOffset.y = 0;
	
	wndRect.OffsetRect(scrOffset);
	
	// update window position and display
	SetWindowPos(&wndTop, 
		wndRect.left,
		wndRect.top,
		wndRect.Width( ), 
		wndRect.Height( ), 
		SWP_SHOWWINDOW | SWP_NOACTIVATE);

//	_comboTower.SetFocus(); 
}

void TowerPopup::OnSelchangeOptionsTowerCombo() 
{
	towerManager()->setActiveTowerIndex(_comboTower.GetCurSel());	

	PostMessage(WM_KILL_POPUP); 
}


void TowerPopup::OnTimer(UINT nIDEvent) 
{
	if (!IsChild(GetFocus()))
 		PostMessage(WM_KILL_POPUP); 
	
	AutoPopup::OnTimer(nIDEvent);
}

void TowerPopup::towerManager_notifyUpdateTowerList()
{
	//TRACE("Listener TowerPopup::towerManager_notifyUpdateTowerList() \r\n");

	towerManager()->populateComboBox(_comboTower);

}

void TowerPopup::OnDestroy() 
{
	towerManager()->removeEventListener(this);

	AutoPopup::OnDestroy();	
}
