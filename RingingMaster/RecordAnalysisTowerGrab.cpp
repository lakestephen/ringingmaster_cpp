// RecordAnalysisTowerGrab.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordAnalysisTowerGrab.h"
#include "MapManager.h"
#include "RecordManager.h"
#include "RecordTower.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerGrab

RecordAnalysisTowerGrab::RecordAnalysisTowerGrab() 
{

}


RecordAnalysisTowerGrab::~RecordAnalysisTowerGrab()
{

}


BEGIN_MESSAGE_MAP(RecordAnalysisTowerGrab, SuperGridCtrl)
	//{{AFX_MSG_MAP(RecordAnalysisTowerGrab)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_LAUNCH_WEB_PAGE, OnLaunchWebPage)
	ON_UPDATE_COMMAND_UI(ID_LAUNCH_WEB_PAGE, OnUpdateLaunchWebPage)
	ON_COMMAND(ID_POPUP_SHOWMAP, OnShowMap)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWMAP, OnUpdateShowMap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerGrab message handlers


BOOL RecordAnalysisTowerGrab::Create(CWnd* parent)
{
	BOOL ret = SuperGridCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0),		
		parent, AAA_GRAB_LIST);

	if (ret)
		SetFont(&app()->_fontStd);
	
	return  ret;
}

CImageList *RecordAnalysisTowerGrab::CreateDragImageEx(int /*nItem*/)
{
	return NULL; //this effectivly disables the drag drop
}

CString RecordAnalysisTowerGrab::getEmptyListText()
{
	return "";
}

void RecordAnalysisTowerGrab::doUpdate(RecordTower* tower)
{	
/*	if (!::IsWindow(m_hWnd))
		return;
	_lastTower = tower;

	DeleteAll(false);

	if ((viewType != vt_tower)&&
		(viewType != vt_grab)&&
		(viewType != vt_map))
	{
		ASSERT(FALSE);
		return;
	}
	
	if (tower == NULL)
		return;

	
	SetRedraw(false); //has to be after the DeleteAll, else flag is reset

	
	
	//add the tower name
	SGItemInfo*  lp = new SGItemInfo();
	lp->SetItemText(tower->getName(), GetSysColor(COLOR_WINDOWTEXT), (COLORREF)-1);
	lp->SetImage(0);
	SGTreeItem* towerName = InsertRootItem(lp);

	// add each grab
	for (int i=0;i<tower->getGrabCount();i++)
	{
		RecordGrab* grab = tower->getGrab(i);

		//add the date
		lp = new SGItemInfo;
		lp->SetItemText(grab->getDateAsString(false));
		lp->AddSubItemText(grab->getBellsGoAsString());
		lp->AddSubItemText(grab->getBellsSoundAsString());
		lp->AddSubItemText(grab->getCommentAsSingleLine());

		lp->SetImage(1);
		InsertItem(towerName, lp);
	}	
	
	int nScroll = 0;
	ExpandAll(towerName, nScroll);


	
	SetRedraw(true); 
	InvalidateRect(NULL);
	UpdateWindow();		 */ //todo
}


int RecordAnalysisTowerGrab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/////////////////////
	//remember this	
	///////////////////

	//associate imagelist with listviewctrl, 
	//this imagelist is only for the first column re: the tree column	
	if(!_image.Create(IDB_GRAB_CTRL,16,1,RGB(255, 0, 255)))
		return -1;

	SetImageList(&_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	//If you want to assiciate images to subitems, create an imagelist
	//and call SetSubItemImageList
	//now the next time you want to map an image from this imagelist to a
	//particular subitem call SetSubItemText(col,text,image-index);

	CImageList imageSubItem;
	if(!imageSubItem.Create(IDB_SUBITEMS,16,1,RGB(255, 0, 255)))
		return -1;

    if(!SetSubItemImageList(&imageSubItem))
		return -1;
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/

//	SetExtendedStyle(LVS_EX_GRIDLINES);

/*	AddColumn("Tower\\" + recordManager()->getAvailableFieldsName(af_grab_date));
	AddColumn(recordManager()->getAvailableFieldsName(af_grab_bellsGo));
	AddColumn(recordManager()->getAvailableFieldsName(af_grab_bellsSound));
	AddColumn(recordManager()->getAvailableFieldsName(af_grab_comments));
  */ //todo
	AutoSaveColumns(NULL, "Grab Columns", "140,70,70,200,");

	
	return 0;

}


//this is my override of GetIcon, override this to set what ever icon suits you
int RecordAnalysisTowerGrab::GetIcon(const SGTreeItem* pItem)
{
	if(pItem!=NULL)
		return GetData(pItem)->GetImage();

	return 0;
}

void RecordAnalysisTowerGrab::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	ViewType viewType = app()->getViewType();
	if (viewType == vt_grab || viewType == vt_tower|| viewType == vt_map)		
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
		
		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_TOWER_ANALYSIS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}

}

void RecordAnalysisTowerGrab::OnEditCopy() 
{

	if(_tower && OpenClipboard())
	{
		EmptyClipboard();

		CString strSource; // todo migrate to matrix = recordManager()->getCopyData(_tower);
		
		HGLOBAL strBuff;
		char * buffer;
		
		//str
		strBuff = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+1);
		buffer = (char*)GlobalLock(strBuff);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(strBuff);
		SetClipboardData(CF_TEXT,strBuff);

		CloseClipboard();
	}

}

void RecordAnalysisTowerGrab::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_tower != NULL);		
}

void RecordAnalysisTowerGrab::OnPopupPrintAnalysis() 
{
	CDocument* pDoc = (CDocument*)recordManager()->getTowerDoc();

	if (!pDoc) 
		pDoc = (CDocument*)recordManager()->getGrabDoc();

	if ((pDoc)&&(_tower))
	{
		POSITION pos = pDoc->GetFirstViewPosition();

		CView* view = pDoc->GetNextView(pos);

		if (view)
		{
			recordManager()->setPrintTowerRef(_tower);
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	
		}  
	}	   	
}

void RecordAnalysisTowerGrab::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_tower != NULL);	
}

void RecordAnalysisTowerGrab::OnLaunchWebPage() 
{
	if ((_tower)&&(!_tower->getWebAddress().IsEmpty()))
		app()->launchBrowser(_tower->getWebAddress());
}

void RecordAnalysisTowerGrab::OnUpdateLaunchWebPage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_tower)&&(!_tower->getWebAddress().IsEmpty()));
}

void RecordAnalysisTowerGrab::OnShowMap() 
{
	CWaitCursor wait;

	if (_tower)
		mapManager()->showTowerMapDoc(_tower);
}

void RecordAnalysisTowerGrab::OnUpdateShowMap(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mapManager()->isValid()&&
				   _tower&&
				   _tower->getMapRef().isValidOSGridRef());		
}


