// TowerView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerView.h"

#include "RecordManager.h"
#include "MapManager.h"
#include "TowerDocumentInfo.h"
#include "MapDoc.h"

#include "RecordChooseTowerDlg.h" //todo do we need this

#include "MatrixGridCtrl.h"
#include "MatrixManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerView  

IMPLEMENT_DYNCREATE(TowerView, MatrixView)

TowerView::TowerView()
{
}

TowerView::~TowerView()
{
}


BEGIN_MESSAGE_MAP(TowerView, MatrixView)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_SHOWMAP, OnPopupShowmap)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWMAP, OnUpdatePopupShowmap)
	ON_COMMAND(ID_POPUP_SETHOMETOWER, OnPopupSethometower)
	ON_COMMAND(ID_POPUP_GRABTOWER, OnPopupGrabtower)
	ON_COMMAND(ID_POPUP_DELETETOWER, OnPopupDeletetower)
	ON_COMMAND(ID_POPUP_EDITTOWER, OnPopupEdittower)
	ON_COMMAND(ID_LAUNCH_WEB_PAGE, OnLaunchWebPage)
	ON_UPDATE_COMMAND_UI(ID_LAUNCH_WEB_PAGE, OnUpdateLaunchWebPage)

	/*todo
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_COMMAND(ID_FILE_PRINT_TOOLBAR, OnFilePrintToolbar)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_MENU, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_TOOLBAR, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrint)*/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerView drawing

void TowerView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// TowerView diagnostics

#ifdef _DEBUG
void TowerView::AssertValid() const
{
	MatrixView::AssertValid();
}

void TowerView::Dump(CDumpContext& dc) const
{
	MatrixView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TowerView message handlers

int TowerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MatrixView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

MatrixEntityToken TowerView::getMatrixEntityToken() const
{
	return met_tower;
}	

void TowerView::OnPopupShowmap() 
{
	CWaitCursor wait;

	const SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	const RecordTower* recordTower= dynamic_cast<const RecordTower*>(activeItem);
		
	if (recordTower)
		mapManager()->showTowerMapDoc(recordTower);
}
		
void TowerView::OnUpdatePopupShowmap(CCmdUI* pCmdUI) 
{
	const SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	const RecordTower* recordTower= dynamic_cast<const RecordTower*>(activeItem);


	pCmdUI->Enable(mapManager()->isValid() && 
		           recordTower &&
				   recordTower->getMapRef().isValidOSGridRef());												   
}

void TowerView::OnPopupDeletetower() 
{
	SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	RecordTower* recordTower= dynamic_cast<RecordTower*>(activeItem);
		
	if (recordTower)
		recordManager()->deleteTower(recordTower);

}

void TowerView::OnPopupEdittower() 
{
	SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	RecordTower* recordTower= dynamic_cast<RecordTower*>(activeItem);
		
	if (recordTower)
		recordManager()->editTower(recordTower);
}


void TowerView::OnPopupSethometower() 
{  
	const SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	const RecordTower* recordTower= dynamic_cast<const RecordTower*>(activeItem);
		
	if (recordTower)
		recordManager()->setHomeTower(recordTower->getIndex());
}

void TowerView::OnPopupGrabtower() 
{
	const SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	const RecordTower* recordTower= dynamic_cast<const RecordTower*>(activeItem);
		
	if (recordTower)
		recordManager()->createGrab(recordTower->getIndex());
}	

void TowerView::OnLaunchWebPage() 
{
	SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	RecordTower* recordTower= dynamic_cast<RecordTower*>(activeItem);

	if ((recordTower)&&(!recordTower->getWebAddress().IsEmpty()))
		app()->launchBrowser(recordTower->getWebAddress());
}

void TowerView::OnUpdateLaunchWebPage(CCmdUI* pCmdUI) 
{
	SaveableObject* activeItem = getMatrixGridCtrl()->getActiveItem();
	RecordTower* recordTower= dynamic_cast<RecordTower*>(activeItem);

	pCmdUI->Enable((recordTower)&&(!recordTower->getWebAddress().IsEmpty()));
}

TowerDocumentInfo* TowerView::getTowerDocumentInfo()
{
	MatrixGridCtrl* grid = getMatrixGridCtrl();
	if (grid)
	{
		RecordTower* tower = dynamic_cast<RecordTower*>(grid->getActiveItem());

		if (tower)
			return new TowerDocumentInfo(tower->getIndex());
	}

	return NULL;
}

void TowerView::setTowerDocInfo(TowerDocumentInfo* towerDocInfo)
{
	if (towerDocInfo)
	{
		MatrixGridCtrl* grid = getMatrixGridCtrl();
		if (grid)
		{		
			grid->selectEntityFromId(towerDocInfo->getSelectedTowerIndex());
		}
	}
}

/* //todo
void TowerView::OnPopupPrintAnalysis() 
{
	//do not set the tower here, it is set in the showing of the popup
	recordManager()->OnFilePrint();	
	MatrixView::OnFilePrint();
}

void TowerView::OnFilePrint() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedTower(vt_tower));
	recordManager()->OnFilePrint();	
	MatrixView::OnFilePrint();
}

void TowerView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_list.recordManager_notifyGetSelectedTower(vt_tower) != NULL);	
}

void TowerView::OnFilePrintMenu() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedTower(vt_tower));
	recordManager()->OnFilePrintMenu();	
	MatrixView::OnFilePrint();
}

void TowerView::OnFilePrintToolbar() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedTower(vt_tower));
	recordManager()->OnFilePrintToolbar();	
	MatrixView::OnFilePrint();
}

void TowerView::OnFilePrintPreview() 
{
	recordManager()->setPrintTowerRef(_list.recordManager_notifyGetSelectedTower(vt_tower));
	BCGPPrintPreview (this);
}


BOOL TowerView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return recordManager()->OnPreparePrinting(pInfo, this);
}

void TowerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnBeginPrinting(pDC, pInfo);
		
	MatrixView::OnBeginPrinting(pDC, pInfo);
}

void TowerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	MatrixView::OnPrepareDC(pDC, pInfo);

	recordManager()->OnPrepareDC(pDC, pInfo);
}

void TowerView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnEndPrinting(pDC, pInfo);
	
	MatrixView::OnEndPrinting(pDC, pInfo);
}

void TowerView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	recordManager()->OnPrint(pDC, pInfo);
}

*/
