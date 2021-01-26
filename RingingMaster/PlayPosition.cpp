// PlayPosition.cpp: implementation of the PlayPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PlayPosition.h"
#include "MemDC.h"
#include "TouchBlueLineView.h"
#include "PlayManager.h"
#include "BCGPNoSizeMiniFrameWnd.h"
#include "TowerManager.h"
#include "Tower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const CRect rectforInvalidation(35,7,90,52);

PlayPosition::PlayPosition() :
_checkState(TRUE), 
_pealTime("TODO"), 
_lastNumberInclCover(0)
{  

}

PlayPosition::~PlayPosition()
{

}


IMPLEMENT_DYNAMIC(PlayPosition, CBCGPDockingControlBar)

BEGIN_MESSAGE_MAP(PlayPosition, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(PlayPosition)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(AAA_TRACK, OnTrack)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(AAA_TRACK, OnUpdateTrack)
END_MESSAGE_MAP()



void PlayPosition::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	MemDC memdc(&dc);

	CRect rectClient;
	GetClientRect (rectClient);

	memdc.FillSolidRect (rectClient, ::GetSysColor (COLOR_BTNFACE));

	CFont* pOldFont = (CFont*) memdc.SelectObject(&app()->_fontStd);

	memdc.SetBkMode (TRANSPARENT);

	memdc.TextOut (7, 5, "Lead:");
	memdc.TextOut (7, 21, "Row:");
	memdc.TextOut (7, 37, "Speed:");

	const int xPos = 44;
	if (_rowPosition.isValid(true))
	{
		CString str;
		str.Format("%d", _rowPosition.getLead(true)+1);
		memdc.TextOut (xPos, 5, str);
		str.Format("%d", _rowPosition.getRowDisplay(true)+1);
		memdc.TextOut (xPos, 21, str);
		memdc.TextOut (xPos, 37, _pealTime);
	}

	memdc.SelectObject (pOldFont);
}




void PlayPosition::playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage)
{
	_rowPosition = rowPositionChangedPackage._rowPosition;

	int numberInclCover = playManager()->getNumberInclCover();
	if (_lastNumberInclCover != numberInclCover)
	{
		towerManager_notifyUpdateTowerList();
		_lastNumberInclCover = numberInclCover;
	}

	InvalidateRect(rectforInvalidation);
}

int PlayPosition::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	
	SetMiniFrameRTC(RUNTIME_CLASS(PlayPositionNoSizeMiniFrameWnd));

	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_trackCheck.Create("Track",BS_CHECKBOX | WS_VISIBLE | WS_CHILD, CRect (0,0,0,0),this,AAA_TRACK);
	_trackCheck.SetFont(&app()->_fontStd);

	playManager()->addEventListener(this);
	towerManager()->addEventListener(this);
	
	return 0;
}

void PlayPosition::OnDestroy() 
{
	CBCGPDockingControlBar::OnDestroy();
	
	playManager()->removeEventListener(this);	
	towerManager()->removeEventListener(this);
}

void PlayPosition::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	
	if (_trackCheck)	_trackCheck.MoveWindow(10 ,56,70,22);	 
}

void PlayPosition::OnTrack() 
{
	playManager()->toggleTracking();
}

void PlayPosition::OnUpdateTrack(CCmdUI* pCmdUI) 
{			
	pCmdUI->SetCheck(playManager()->isTracking());
}  		

BOOL PlayPosition::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
	
	//return CBCGPDockingControlBar::OnEraseBkgnd(pDC);
}

void PlayPosition::towerManager_notifyUpdateTowerList()
{
	Tower* tower = towerManager()->getTower();
	if (tower)
	{
		int numberInclCover = playManager()->getNumberInclCover();
		_pealTime = GlobalFunctions::getPealTimeText(numberInclCover, 
													tower->getSpeed(),
													tower->getOpenHandstrokeGap());
	}
	else 
	{
		_pealTime.Empty();
	}

	InvalidateRect(rectforInvalidation);
}
