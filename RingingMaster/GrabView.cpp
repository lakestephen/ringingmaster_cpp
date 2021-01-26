// GrabView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GrabView.h"
#include "RecordGrab.h"
#include "RecordManager.h"
#include "MapManager.h"
#include "MapDoc.h"
#include "RecordChooseGrabDlg.h"
#include "GrabDocumentInfo.h"

#include "MatrixManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GrabView


IMPLEMENT_DYNCREATE(GrabView, MatrixView)

GrabView::GrabView()
{
}

GrabView::~GrabView()
{
}


BEGIN_MESSAGE_MAP(GrabView, MatrixView)
	//{{AFX_MSG_MAP(GrabView)
	ON_COMMAND(ID_POPUP_EDITGRAB, OnPopupEditgrab)
	ON_COMMAND(ID_POPUP_SETHOMETOWER, OnPopupSethometower)
	ON_COMMAND(ID_POPUP_SHOWMAP, OnPopupShowmap)
	ON_COMMAND(ID_POPUP_GRABTOWER, OnPopupGrabtower)
	ON_COMMAND(ID_POPUP_EDITDAYORDER, OnPopupEditDayOrder)
	ON_COMMAND(ID_POPUP_DELETEGRAB, OnPopupDeletegrab)
	ON_COMMAND(ID_LAUNCH_WEB_PAGE, OnLaunchWebPage)
	ON_UPDATE_COMMAND_UI(ID_LAUNCH_WEB_PAGE, OnUpdateLaunchWebPage)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWMAP, OnUpdatePopupShowmap)

/*	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_MENU, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_TOOLBAR, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrint)
*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GrabView drawing

void GrabView::OnDraw(CDC* /*pDC*/)
{
}

MatrixEntityToken GrabView::getMatrixEntityToken() const
{
	return met_grab;
}	

/////////////////////////////////////////////////////////////////////////////
// GrabView diagnostics

#ifdef _DEBUG
void GrabView::AssertValid() const
{
	MatrixView::AssertValid();
}

void GrabView::Dump(CDumpContext& dc) const
{
	MatrixView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GrabView message handlers

void GrabView::OnPopupEditgrab() 
{
/*	todoif (_list.getLastContextItem() != -1)
	{
		RecordGrab* grab = recordManager()->getFilteredGrab(_list.getLastContextItem());
		if (grab)
			recordManager()->editGrab(grab);
	}	*/	
}					   

void GrabView::OnPopupSethometower() 
{
/*	todo	if (_list.getLastContextItem() != -1)
	{
		RecordGrab* grab = recordManager()->getFilteredGrab(_list.getLastContextItem());
		if (grab)
			recordManager()->setHomeTower(grab->getTowerIndex());
	}	*/									
}

void GrabView::OnPopupShowmap() 
{
/*	todo	CWaitCursor wait;

	RecordTower* recordTower = _list.recordManager_notifyGetSelectedTower(vt_grab);
	if (recordTower)
	{
		mapManager()->showTowerMapDoc(recordTower);
	}*/
}

void GrabView::OnPopupGrabtower() 
{
/*	todo	if (_list.getLastContextItem() != -1)
	{
		RecordGrab* grab = recordManager()->getFilteredGrab(_list.getLastContextItem());
		if (grab)
			recordManager()->createGrab(grab->getTowerIndex());
	}	*/		
}	

void GrabView::OnPopupEditDayOrder() 
{
/*	todo	if (_list.getLastContextItem() != -1)
	{
		RecordGrab* grab = recordManager()->getFilteredGrab(_list.getLastContextItem());
		if (grab)
			recordManager()->editGrabDayOrder(grab);
	}		*/	
}

void GrabView::OnPopupDeletegrab() 
{
/*	todo	if (_list.getLastContextItem() != -1)
	{
		RecordGrab* grab = recordManager()->getFilteredGrab(_list.getLastContextItem());
		if (grab)
			recordManager()->deleteGrab(grab);
	}*/			
}

void GrabView::OnLaunchWebPage() 
{
/*	todo	RecordTower* tower = _list.recordManager_notifyGetSelectedTower(vt_grab);
	
	if ((tower)&&(!tower->getWebAddress().IsEmpty()))
		app()->launchBrowser(tower->getWebAddress());*/
}

void GrabView::OnUpdateLaunchWebPage(CCmdUI* pCmdUI) 
{
/*	todo	RecordTower* tower = _list.recordManager_notifyGetSelectedTower(vt_grab);
	pCmdUI->Enable((tower)&&(!tower->getWebAddress().IsEmpty()));*/
}

void GrabView::OnUpdatePopupShowmap(CCmdUI* pCmdUI) 
{
/*	todo	pCmdUI->Enable(mapManager()->isValid() && 
		           _list.recordManager_notifyGetSelectedTower(vt_grab)&&
				   _list.recordManager_notifyGetSelectedTower(vt_grab)->getMapRef().isValidOSGridRef());*/
}


GrabDocumentInfo* GrabView::getGrabDocumentInfo()
{
/*	todo	RecordGrab* grab = _list.recordManager_notifyGetSelectedGrab(vt_grab);

	if (grab)
		return new GrabDocumentInfo(grab->getGrabIndex());
*/
	return NULL;
}

void GrabView::setGrabDocInfo(GrabDocumentInfo* grabDocInfo)
{
/*	todo	if (grabDocInfo)
	{
		_list.selectGrabIndex(grabDocInfo->getSelectedGrabIndex());
	}*/
}


/*
void GrabView::OnPopupPrintAnalysis() 
{
	//do not set the grab here, it is set in the showing of the popup
	recordManager()->OnFilePrint();	
	MatrixView::OnFilePrint();
}

void GrabView::OnFilePrint() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedGrab(vt_grab)->getTower());
	recordManager()->OnFilePrint();	
	MatrixView::OnFilePrint();
}

void GrabView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_list.recordManager_notifyGetSelectedGrab(vt_grab) != NULL);	
}

void GrabView::OnFilePrintMenu() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedGrab(vt_grab)->getTower());
	recordManager()->OnFilePrintMenu();	
	MatrixView::OnFilePrint();
}

void GrabView::OnFilePrintToolbar() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedGrab(vt_grab)->getTower());
	recordManager()->OnFilePrintToolbar();	
	MatrixView::OnFilePrint();
}

void GrabView::OnFilePrintPreview() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedGrab(vt_grab)->getTower());
	BCGPPrintPreview (this);
}


BOOL GrabView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return recordManager()->OnPreparePrinting(pInfo, this);
}

void GrabView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnBeginPrinting(pDC, pInfo);
		
	MatrixView::OnBeginPrinting(pDC, pInfo);
}

void GrabView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	MatrixView::OnPrepareDC(pDC, pInfo);

	recordManager()->OnPrepareDC(pDC, pInfo);
}

void GrabView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnEndPrinting(pDC, pInfo);
	
	MatrixView::OnEndPrinting(pDC, pInfo);
}

void GrabView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnPrint(pDC, pInfo);
}
*/

/*

//if we have more than one item on a day
	RecordGrab* grab = recordManager()->getFilteredGrab(_lastContextItem);
	if (recordManager()->hasMultipleGrabsOnDay(grab))
	{
		pPopup->InsertMenu(4, MF_BYPOSITION | MF_SEPARATOR, NULL, "");
		pPopup->InsertMenu(4, MF_BYPOSITION , ID_POPUP_EDITDAYORDER, "Edit &Day Order");
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, FALSE);


	*/ //todo this was in the grabListCtrl context menu code.