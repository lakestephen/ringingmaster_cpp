// TouchAnalysisMusic.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchAnalysisMusic.h"
#include "TouchDoc.h"
#include "Method.h"
#include "MethodAnalysis.h"
#include "TouchHoldView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisMusic

TouchAnalysisMusic::TouchAnalysisMusic() 
{
}

TouchAnalysisMusic::~TouchAnalysisMusic()
{

}


BEGIN_MESSAGE_MAP(TouchAnalysisMusic, SuperGridCtrl)
//{{AFX_MSG_MAP(TouchAnalysisMusic)
ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
		ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ANALYSIS, OnUpdatePrintAnalysis)
	ON_COMMAND(ID_PRINT_ANALYSIS, OnPrintAnalysis)
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnPopupEditCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisMusic message handlers

BOOL TouchAnalysisMusic::Create(CWnd * pParentWnd)
{
	BOOL ret = SuperGridCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(0,0,100,100),		
		pParentWnd, AAA_MUSIC_LIST);
	
	mainFrame()->addEventListener(this);

	return  ret;
}


BOOL TouchAnalysisMusic::DestroyWindow()
{
	mainFrame()->removeEventListener(this);

	return __super::DestroyWindow();
}


void TouchAnalysisMusic::mainFrame_notifyUpdate(ViewType viewType)
{
	if ((viewType != vt_touch_main)&&(viewType != vt_touch_blue_line)) 
		return;

	DeleteAll(false);
	
	SetRedraw(false); //has to be after the DeleteAll, else flag is reset
	
	TouchDoc* pDoc = mainFrame()->getActiveTouchDoc();	
		
	//we retrive the method names.
	//we retrive the method names.
	if ((pDoc)&&(pDoc->getMethod())&&(pDoc->getMethod()->hasAnalysis()))
	{
		Method * method = pDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 
		const MusicResults& musicResults = analysis->getMusicResults();
			

		int musicalChangesCount = 0;
		int CRUCount = 0;
		//get the count
		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			if (result->getType() == MRT_MUSICAL_CHANGES)
			{
				musicalChangesCount += result->GetSize();
			}
			if (result->getType() == MRT_CRU)
			{
				CRUCount  += result->GetSize();
			}
		}

		//musical changes
		SGTreeItem* musicalChanges = NULL;
		SGItemInfo* musicalChangesInfo = NULL;
		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			if (result->getType() == MRT_MUSICAL_CHANGES)
			{
				if (musicalChanges == NULL)
				{
					//create the root node
					musicalChangesInfo = new SGItemInfo();

					musicalChangesInfo->SetItemText("Musical Changes", ::GetSysColor(COLOR_GRAYTEXT), (COLORREF)-1);
					musicalChangesInfo->AddSubItemText("various"); 

					CString number;
					number.Format("%d", musicalChangesCount);

					musicalChangesInfo->AddSubItemText(number); 
					musicalChangesInfo->SetImage((musicalChangesCount == 0)?3:0);
					musicalChanges = InsertRootItem(musicalChangesInfo);
// 					musicalChangesInfo->SetControlType(SGItemInfo::CONTROLTYPE::edit); //for testing
				}
				addMusicNode(musicalChanges, result, false); 
			}
		}
		
		//CRU changes
		SGTreeItem* CRUChanges = NULL;
		SGItemInfo* CRUChangesInfo = NULL;
		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			if (result->getType() == MRT_CRU)
			{
				if (CRUChanges == NULL)
				{
					//create the root node
					CRUChangesInfo = new SGItemInfo();

					CRUChangesInfo->SetItemText("Combination Roll-Ups", ::GetSysColor(COLOR_GRAYTEXT), (COLORREF)-1);
		
					CString str = "? ? ? ? ( 4 5 6 ) ( 4 5 6 ) ";
					while(str.Replace(" ", ""));
					///the spaces, and the replace get over som strange character replacement of the question marks

					for (int i=6;i<method->getNumber();i++)
					{
						str += GlobalFunctions::bellNumbersToChar(i+1);
					}

					CRUChangesInfo->AddSubItemText(str); 

					CString number;
					number.Format("%d", CRUCount);

					CRUChangesInfo->AddSubItemText(number); 
					CRUChangesInfo->SetImage((CRUCount  == 0)?3:0);
					CRUChanges = InsertRootItem(CRUChangesInfo);
				}
				addMusicNode(CRUChanges, result, false); 
			}
		}
		 
		//user music
		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			
			if (result->getType() == MRT_USER)
				addMusicNode(NULL, result);		
		}  
	} 
		  
	SetRedraw(true); 
	InvalidateRect(NULL);
	UpdateWindow();
}

void TouchAnalysisMusic::addMusicNode(SGTreeItem *parent, MusicResult *result, bool enable)
{
	COLORREF textColor;
	if (enable)
		textColor = GetSysColor(COLOR_WINDOWTEXT);
	else
		textColor = GetSysColor(COLOR_GRAYTEXT);

	CString name = result->getName();
	CString change = result->getDisplayString(false);
	CString number;
	number.Format("%d", result->GetSize());
	
	SGItemInfo* lp = new SGItemInfo();
	
	lp->SetItemText(name, textColor, (COLORREF)-1);
	lp->AddSubItemText(change); 
	lp->AddSubItemText(number); 
	lp->SetImage(((result->GetSize()==0)?3:0));
	
	
	SGTreeItem * musicParent = NULL;
	
	if (parent == NULL)
		musicParent = InsertRootItem(lp);
	else
		musicParent = InsertItem(parent, lp);
	
	if (musicParent)
	{
		for (int j=0;j<result->GetSize();j++)
		{
			lp = new SGItemInfo();
			lp->SetItemText(result->GetAt(j)->format(rpft_full), textColor, (COLORREF)-1);
			lp->AddSubItemText(result->GetAt(j)->getChange()); 
			lp->SetImage(2);
			
			lp->_rowPosition = result->GetAt(j);
			
			InsertItem(musicParent, lp);
		}
	}	 

}



int TouchAnalysisMusic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this	
	///////////////////

	//associate imagelist with listviewctrl, 
	//this imagelist is only for the first column re: the tree column	
	if(!_image.Create(IDB_MUSIC_CTRL,16,1,RGB(255, 0, 255)))
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

	AddColumn("Name / Row");
	AddColumn("Change / Condition");
	AddColumn("Count");

	AutoSaveColumns(NULL, "Music Columns", "150,150,75");

	
	return 0;
}

//this is my override of GetIcon, override this to set what ever icon suits you
int TouchAnalysisMusic::GetIcon(const SGTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int iImage = GetData(pItem)->GetImage();

		if((iImage == 0)&&(ItemHasChildren(pItem)))
		{
			IsCollapsed(pItem) ? iImage = 0/*close icon*/:iImage = 1;/*open icon*/
		}
		return iImage;
	}
	return 0;
}

BOOL TouchAnalysisMusic::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();	
	
	if ((pTouchDoc)&&(pTouchDoc->getMethod())&&(ht.iItem != -1))
	{
		SGTreeItem *pSelItem = GetTreeItem(ht.iItem);
		if (pSelItem)
		{
			SGItemInfo *pItemInfo = GetData(pSelItem);
			if ((pItemInfo)&&(pItemInfo->_rowPosition))
			{
				pTouchDoc->doGoto(*pItemInfo->_rowPosition, false, true, true);
			}
		}
	}

	return 1;
}

CString TouchAnalysisMusic::getEmptyListText()
{
	return "";
}


CImageList *TouchAnalysisMusic::CreateDragImageEx(int /*nItem*/)
{
	return NULL; //this effectivly disables the drag drop
}

void TouchAnalysisMusic::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
	if (pTouchDoc)		
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
		VERIFY(menu.LoadMenu (IDR_TOUCH_ANALYSIS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		//set what we want to print
		pTouchDoc->getPrintWhatAnalysis().unSetAll();
		pTouchDoc->getPrintWhatAnalysis()._bMusic = TRUE;

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
	
}

void TouchAnalysisMusic::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisMusic::OnPopupPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	

	}	
}

void TouchAnalysisMusic::OnUpdatePrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mainFrame()->getActiveTouchDoc() != NULL);
	
}

void TouchAnalysisMusic::OnPrintAnalysis() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (pTouchDoc)
	{
		CView* view = pTouchDoc->getTouchHoldView()->GetActiveView();

		if (view != NULL)
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_PRINT_ANALYSIS,0);	

	}	
}

CString TouchAnalysisMusic::getCopyData(const Method& method, const MethodAnalysis& analysis)
{
	CString str;
	CString tempStr;

	const MusicResults& musicResults = analysis.getMusicResults();

	int musicalChangesCount = 0;
	int CRUCount = 0;
	int userMusicCount = 0;
	//get the count
	for (int i=0;i<musicResults.GetSize();i++)
	{
		MusicResult* result = musicResults.GetAt(i);
		if (result->getType() == MRT_MUSICAL_CHANGES)
		{
			musicalChangesCount += result->GetSize();
		}
		if (result->getType() == MRT_CRU)
		{
			CRUCount  += result->GetSize();
		}
		if (result->getType() == MRT_USER)
		{
			userMusicCount  += result->GetSize();
		}											 
		
	}

	//musical changes
	if (musicalChangesCount >0)
	{
		tempStr.Format("Musical Changes \t\tvarious \t%d\r\n", musicalChangesCount);
		str += tempStr;

		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			if (result->getType() == MRT_MUSICAL_CHANGES)
				str += copyMusicNode(result); 
		}

		str += "\r\n";
	}
	
	//CRU changes
	if (CRUCount > 0)
	{
		tempStr = "? ? ? ? ( 4 5 6 ) ( 4 5 6 ) ";
		while(tempStr.Replace(" ", ""));
		for (int i=6;i<method.getNumber();i++)
			tempStr += GlobalFunctions::bellNumbersToChar(i+1);
		CString tempStr2;
		tempStr2.Format("Combination Roll-Ups \t\t%s \t%d\r\n", tempStr, CRUCount);
		str += tempStr2;		 

		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			if (result->getType() == MRT_CRU)
				str += copyMusicNode(result); 
		}
		str += "\r\n";
	}
	 
	//user music
	if (userMusicCount >0)
	{
		tempStr.Format("User Music \t\tvarious \t%d\r\n", userMusicCount);
		str += tempStr;

		for (int i=0;i<musicResults.GetSize();i++)
		{
			MusicResult* result = musicResults.GetAt(i);
			
			if (result->getType() == MRT_USER)
				str += copyMusicNode(result);		
		}  	
	}   
  
	return str;
}


CString	TouchAnalysisMusic::copyMusicNode(MusicResult *result)
{
	CString str;

	str.Format("\t%s\t%s\t%d\r\n", result->getName(), result->getDisplayString(false), result->GetSize());
							   
	for (int j=0;j<result->GetSize();j++)
	{
		str += "\t\t";
		str += result->GetAt(j)->format(rpft_full);
		str += "\t";
		str += result->GetAt(j)->getChange();
		str += "\r\n";
	}   

	return str;
}


void TouchAnalysisMusic::OnPopupEditCopy() 
{
	TouchAnalysisBase::OnPopupEditCopy(this);	
}


double TouchAnalysisMusic::print(CPage *page, double nextRow)
{
	/* todo
	nextRow = PrintUtils::printTitle(page, "Music", nextRow, false);
	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0)+ PrintUtils::_AnalysisIndent;	

	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	//we retrive the method names.
	if (_method)
	{	
	//get the width of the labels
		double width = 0.0;
		//musical changes
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			double temp= page->getTextWidth(createMusicPrintLabel(result));
			if (temp > width) width = temp;
		}
		//CRU changes
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			double temp= page->getTextWidth(createMusicPrintLabel(result));
			if (temp > width) width = temp;
		}
		//user music
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			double temp= page->getTextWidth(createMusicPrintLabel(result));
			if (temp > width) width = temp;
		}
		width *= .8;

	//do the printing
		//musical changes
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			if (result->getType() == MRT_MUSICAL_CHANGES)
				nextRow = printMusicLine(page, nextRow, column, result, false, width); 
		}
		
		//CRU changes
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			if (result->getType() == MRT_CRU)
				nextRow = printMusicLine(page, nextRow, column, result, false, width); 
		}
		 
		//user music
		for (int i=0;i<_method->getMusicResults().GetSize();i++)
		{
			MusicResult* result = _method->getMusicResults().GetAt(i);
			if (result->getType() == MRT_USER)
				nextRow = printMusicLine(page, nextRow, column, result, true, width); 
		}  
	} 

	page->SetFont(oldFont);
	page->SetColor(oldColor);
		*/
	return nextRow + .15;
}



double TouchAnalysisMusic::printMusicLine(CPage *page, double nextRow, double column, MusicResult *result, bool enable, double labelOffset)
{
/* todo	UINT flag;
	CString str;

	if (!enable)
		flag = TEXT_LEFT | TEXT_ITALIC;
	else
		flag = TEXT_LEFT;

	str = createMusicPrintLabel(result);

	double columnHold = column;

	page->Print(nextRow, column, flag, 8, str.GetBuffer(str.GetLength()));
	str.ReleaseBuffer();

	str.Empty();
	for (int j=0;j<result->GetSize();j++)
	{
		str += result->GetAt(j)->format(rpft_short);
		str += " : ";
		str += result->GetAt(j)->getChange();
		if (j < result->GetSize()-1)
			str += ", ";
	}

	nextRow = page->Print(nextRow, columnHold + labelOffset , 
							flag, 8, str.GetBuffer(str.GetLength()));
	str.ReleaseBuffer();
	   */
	return nextRow;
}

CString TouchAnalysisMusic::createMusicPrintLabel(MusicResult *result)
{
	CString str;
/* todo 	str.Format("%s (%s) on %d rows", result->getName(), result->getDisplayString(false), result->GetSize());
	TRACE(str + "\r\n");*/
	return str;
}

			