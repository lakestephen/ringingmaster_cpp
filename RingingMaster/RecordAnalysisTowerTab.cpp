// RecordAnalysisTowerTab.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordAnalysisTowerTab.h"
#include "RecordAnalysisTowerTower.h"
#include "RecordAnalysisTowerGrab.h"
#include "RecordManager.h"
#include "MainFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerTab

RecordAnalysisTowerTab::RecordAnalysisTowerTab(RecordAnalysisTowerType type) :
_type(type),
_tower(NULL),
_grab(NULL)
{
}

RecordAnalysisTowerTab::~RecordAnalysisTowerTab()
{
	delete _tower;
	_tower = NULL;

	delete _grab;
	_grab = NULL;
}


BEGIN_MESSAGE_MAP(RecordAnalysisTowerTab, CBCGPTabWnd)
	//{{AFX_MSG_MAP(RecordAnalysisTowerTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerTab message handlers

BOOL RecordAnalysisTowerTab::Create(CWnd *pParentWnd)
{
	BOOL ret = CBCGPTabWnd::Create (CBCGPTabWnd::STYLE_FLAT, CRect(0,0,0,0), pParentWnd, AAA_ANALYSIS_TOWER_TABS);

	mainFrame()->addEventListener(this);
	recordManager()->addEventListener(this);

	SetFont(&app()->_fontStd);
	SetFlatFrame(TRUE);		  

	//create the tower tab
	_tower = new RecordAnalysisTowerTower();
	_tower->Create(this);
	_tower->SetOwner (this);
	
	//create the tower tab
	_grab = new RecordAnalysisTowerGrab();
	_grab->Create(this);
	_grab->SetOwner (this);	
	
	AddTab (_tower,  _T("Tower"));
	AddTab (_grab ,  _T("Grabs"));

	return ret;
}

RecordTower* RecordAnalysisTowerTab::getTower()
{
	switch (_type)
	{
	case ratt_home:
		return recordManager()->getHomeTower();
	case ratt_selected:
//todo		return recordManager()->recordManager_notifyGetSelectedTower();
	default:
		return NULL;
	}				
}

BOOL RecordAnalysisTowerTab::DestroyWindow()
{
	mainFrame()->removeEventListener(this);
	recordManager()->removeEventListener(this);

	return __super::DestroyWindow();
}

void RecordAnalysisTowerTab::mainFrame_notifyDocViewChanged(ViewType /*oldViewType*/, ViewType newViewType, CDocument* /*oldDoc*/, CDocument* /*newDoc*/)
{
	RecordTower* recordTower = NULL;

	if (newViewType == vt_tower ||
		newViewType == vt_grab ||
		newViewType == vt_map)
	{
		recordTower = getTower();
	}

	update(recordTower);
}

void RecordAnalysisTowerTab::update(RecordTower* recordTower)
{
	_grab->doUpdate(recordTower);
	_tower->doUpdate(recordTower);
}

void RecordAnalysisTowerTab::recordManager_notifyHomeTowerChange(RecordTower* homeTower)
{
	if (_type == ratt_home)
		update(homeTower);
}

