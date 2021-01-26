// SuperGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SuperGridCtrl.h"
#include "SGEditCtrl.h"
#include "SGRectangle.h"
					 //todo use BCG controls instead of this. 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
    SuperGrid, YALC (yet another listview control) 

	Written by me(Allan Nielsen), that is .. if the code works ;-)
		mailto:allan.nielsen.msfan@bluewin.ch

	Copyright (c) 1999.

	If you find bugs, please notify me and I'll contact the author who wrote the darn thing ;)
	
    You may use it, abuse it, redistribute it, in any way you desire, but
	if you choose to use this control in any commercial or non commercial application then please 
	send me an email letting me know, makes me ;) . 
	
    Also thanks for all the emails, I count around 200 in my emailbox on this one, all with
	bugfixes, improvements and suggestions ..thanks. If only I had more time....

	HISTORY 


	4 dec. 1998. 

				- MeasureItem now uses current font	
				- Combobox support in SGItemInfo				
				  see the ShowList in MySuperGrid.h/cpp, PreTranslateMessage functions
				  and the file ComboInListView.cpp/h
                - function to indicate a combobox, it draws a down arrow ..see DrawComboBox
				  this idea came from Robert Bouwens.;).
				- in file TestTreeDlg.cpp:  due to the MeasureItem message-reflection	
				  the listctrl is now created in the CDialog::OnCreate function and later on resized
				- LVIS_DROPHILITED support when dragging items
				- Support for LVS_EX_CHECKBOXES	  
				- Added some examples on how to print preview selected item(or at least to get some information from of the grid)
				  the sample is in the CMySuperGrid.h/cpp which is derived from SuperGridCtrl it shows you
				  how to initalize the grid, sort items, search for items and subitems, select and delete thems
				- added virtual GetIcon function to set your default icon handler for a listview index.
				- added virtual GetCellRGB() to set the color for current selected cell
				- renamed CTreeNode to SGTreeItem, sorry ;-(
				- the nested class SGTreeItem has been stripped down to nothing but simple (struct)data members
				  only servering as a linked list of lists now :-)
				- well.. moved all the importent stuff from SGTreeItem to SuperGridCtrl e.g
				  all operations on a SGTreeItem now resides in the SuperGridCtrl, much more OO-like ;-)
				- added Quicksort function
				- got rid of TheOpenCloseThing function.
				- added virtual function OnUpdateListViewItem..called when ever an Item is about to be updated
				- added virtual function OnControlLButtonDown...called when ever LButtondown in a cell
				- added virtual function CreateDragImageEx...

	11 jan 1999:
				- added SetFocus in OnDblclk and in OnLButtonDown, big thank you to Dieter Gollwitzer 
				- fixed a minor hittest error in the HitTestOnSign(), again thanks to Dieter Gollwitzer 


	16 jan 1999: 
				- added virtual OnItemExpanding(....
				- added virtual OnItemExpanded(...
				- added virtual OnCollapsing(....
				- added virtual OnItemCollapsed(...
				- added virtual OnDeleteItem(....
				- added virtual OnVKMultiply(...
				- added virtual OnVkSubTract(..
				- added virtual OnVKAdd(....
				- added SetChildrenFlag();

	1 feb 1999:
				- SGItemInfo now supports individual cell-color
				- fixed some bugs in regards to listdata associated with in each cell
				- added virtual BOOL OnVkReturn(........
				- added virtual BOOL OnItemLButtonDown(.....

	4 mai 1999  - moved to Switzerland to work as a systemdeveloper for 2-3 years :).
				- new email :)
				- added new class for encapsulating drawing + - buttons.

   22.june 1999 - Added GetCount()
				- removed support for VC++ 5.0.
				- added GetTreeItem(.....) 
				- GetSelectedItem(.....)
				- Howto disable Drag/drop in CMySuperGrid
				- level 4 compiled
				- Clean up some redudant code crap

	30.june 99  - fixed minor error in DrawItem, a drawing error occurred when 1.st column was hidden, 
				  it was previous fixed, but some how it was introduced again :).
	
	
	9. juli 99  - CRAIG SCHMIDT "the beachboy" ;-)

				  Craig did the basic implementation of the Multiple Root issue

				  he added the following functions :

				  - SGTreeItem*  InsertRootItem(.....
				  - void DeleteRootItem(....
				  - BOOL IsRoot(...
				  - SGTreeItem* GetRootItem(..
				  - int GetRootIndex(..
				  - int GetRootCount(...
				  - and a member variable, CPtrList m_RootItems.
				  see the file CMySuperGrid.cpp for usage of the InsertRootItem.

   	10.juli 99    added simple wrappers for the rootitems collection

				  -	Added GetRootHeadPosition(...)
				  -	Added GetRootTailPosition();
				  -	Added SGTreeItem* GetNextRoot(...
				  -	Added SGTreeItem* GetPrevRoot(...


	12. juli 99  - fixed minor error in CreateDragImageEx, an error occurred when 1st column was hidden and a drag operation was initiated

	13. juli 99	 - Added void DeleteAll()...deletes all items in the grid, uhh
				 - Removed GetNextSiblingItem(...), GetPrevSiblingItem(...) due to support for multiple roots. 
				 - Added SGTreeItem *GetNext(...) and SGTreeItem* GetPrev(..) due to support for multiple roots.

	14. juli 99	 - Fixed a memory leak in CMySuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized....
	
    --
	meantime     - working my ass of with other projects, still am .. 
	--

	4. dec. 99	 - Added SortEx, the previous Sort function did not sort RootItems.
				   My thanks goes to Thomas Brammer for pointing this out to me.
				   also he is the guy who added one pixel extra to the MakeShortString ;)
				   this should finally take care of the "header resizing problem". 

				 - changed the void CMySuperGrid::SortData() example.
				 *	
				 * Today it´s actually one year ago this code went public (good or bad you decide). 
                 * 

	8. dec. 99   - Added Arne Schild´s improvements, see his improvements in the comment section on the website.
				    - redesign of DeleteAll()..and them some.
				    - redesign of MakeShortString.
				    - HitTestOnSign....
				 - Bug fix in DrawItem, did not use the GetData function to get the Item string or substring for drawing.
				 - UpdateData now has a default parametre(BOOL bUpdateRow), which forces the listview item to be written.
				   this means you don´t have to call UpdateData and CListCtrl::SetItemText afterwards when doing dynamic updates.
				 - added InvalidateItemRect(int nItem);
				 - added InvalidateItemRectPtr(SGTreeItem *);	
				 - added InvalidateItem..does a basic SetItemText given a SGTreeItem ptr 
				 - added a test function: DynamicUpdateSomeItems(int nItem) in CMySuperGrid....
				 - Subitems now as support for an image call the
				   SetSubItemText(int iSubItem, const CString& strSubItem, int iImage) function to set the text and image of a subitem.
				   if you just want to set the image call SetSubItemImage(isubitem,iNewImage)
				 - added SGItemInfo::GetSubItemImage which returns an image index into an imagelist or -1 given an subitem as parametre
				   this function is called from DrawItem.	

				 - added SetSubItemImageList which only copies an imagelist, call this function
				   if you want subitems to support images.

				 - adjusted the DrawItem to account for subitems having a image.
				   this adjustment had impact on
				   - DrawFocusCell
				   - DrawComboBox
				   which now take an extra parametre as argument..of course this is a given subitems new images offet.

                 - added some examples in CMySuperGrid::InitializeGrid() and in CMySuperGrid::DynamicUpdateSomeItems
				   on how to associate an image with a particular subitem.
				 - added an example howto change the image of a subitem see the void CMySuperGrid::SetNewImage function
					
				
	WHAT'S UP :
				- NEW RELEASES can in future be found at www.codeproject.com, don´t forget to subscribe.
				- Peace on earth. 
				- Atl version is still on my list.
				- Better documentation ;)
				- Visual Developer Studio 7.0.
				- Windows 2001 ;)
*/

/////////////////////////////////////////////////////////////////////////////
// SuperGridCtrl
SuperGridCtrl::SuperGridCtrl()
{
	m_cxImage = m_cyImage = m_bIsDragging = m_CurSubItem = 0;
    m_nDragTarget=m_nDragItem = -1;
	m_psTreeLine.CreatePen(PS_SOLID, 1, RGB(192,192,192));
	m_psRectangle.CreatePen(PS_SOLID, 1, RGB(198,198,198));
	m_psPlusMinus.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	m_brushErase.CreateSolidBrush(RGB(255,255,255));
	m_himl = NULL;
}


SuperGridCtrl::~SuperGridCtrl()
{
	m_psPlusMinus.DeleteObject();
	m_psRectangle.DeleteObject();
	m_psTreeLine.DeleteObject();
	m_brushErase.DeleteObject();

	while(m_RootItems.GetCount())
	{
		SGTreeItem * root = (SGTreeItem*)m_RootItems.RemoveHead();
		if(root!=NULL && GetData(root) != NULL)
			delete GetData(root);
		delete root;
	}
	m_RootItems.RemoveAll();
}



BEGIN_MESSAGE_MAP(SuperGridCtrl, CCJListCtrl)
	//{{AFX_MSG_MAP(SuperGridCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SuperGridCtrl message handlers


BOOL SuperGridCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_SHAREIMAGELISTS | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS;	
	return CCJListCtrl::PreCreateWindow(cs);
}



int SuperGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCJListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}



#define OFFSET_FIRST	2 
#define OFFSET_OTHER	6
void SuperGridCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	if (lpDrawItemStruct->CtlType != ODT_LISTVIEW)
        return;

	if(lpDrawItemStruct->itemAction == ODA_DRAWENTIRE)
	{
		if(m_himl==NULL)
		{
			m_himl = (HIMAGELIST)::SendMessage(m_hWnd, LVM_GETIMAGELIST, (WPARAM)(int)(LVSIL_SMALL), 0L);
			if(m_himl==NULL)
				return;
		}

		LV_ITEM lvi;
		static _TCHAR szBuff[MAX_PATH];
		LPCTSTR pszText;
	
		int nItem = lpDrawItemStruct->itemID;
		CRect rcItem(lpDrawItemStruct->rcItem);


		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM; 
		lvi.iItem = nItem;
		lvi.iSubItem =0;
		lvi.pszText = szBuff;
		lvi.cchTextMax = sizeof(szBuff);
		lvi.stateMask = 0xFFFF;		
		GetItem(&lvi);
		
		SGTreeItem *pSelItem = (SGTreeItem*)lpDrawItemStruct->itemData;
		SGItemInfo *pItemInfo = GetData(pSelItem);

		CRect rcLabel;
		GetItemRect(nItem, rcLabel, LVIR_LABEL);
		
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		ASSERT(pDC);

		CRect rcClipBox;
		pDC->GetClipBox(rcClipBox);

		COLORREF crBackground, crText;
		
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// Set the text background and foreground colors
			crBackground = GetSysColor (COLOR_HIGHLIGHT);
			crText = GetSysColor (COLOR_HIGHLIGHTTEXT);
		}
		else
		{
			// Set the text background and foreground colors to the standard window
			// colors
			crBackground = GetSysColor (COLOR_WINDOW);
			if (pItemInfo->GetItemTextClr() != (COLORREF)-1)
				crText = pItemInfo->GetItemTextClr();
			else
				crText = GetSysColor (COLOR_WINDOWTEXT);
		}

		//Get current Icon, you have overridden this I hope :-)
		int iImage = GetIcon(pSelItem);
		if(iImage!=-1)
		{
			if(lvi.iImage!=iImage)
			{
				LV_ITEM lvItem;				
				lvItem.mask =  LVIF_IMAGE;
				lvItem.iImage = iImage;
				lvItem.iItem = nItem;
				lvItem.iSubItem = 0;
				SetItem(&lvItem);
				lvi.iImage = iImage; 
			}
		}

		CRect rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);

		CRect rcIcon;
		GetItemRect(nItem, rcIcon, LVIR_ICON);
		//Draw Current image
		int nOffset = (rcItem.Height() - m_cyImage)/2;
		int nY = rcItem.bottom - m_cyImage - nOffset;
		int nWidth = m_cxImage;//def icon size
		//do not draw icon out side 1.st column.
		int iTryIndent = GetIndent(pSelItem) * m_cxImage + m_cxImage;
		if(rc.left + iTryIndent  > GetColumnWidth(0)-2/*looks better -2*/)
			nWidth = (rc.left + iTryIndent) - GetColumnWidth(0);

		UINT uiFlags = ILD_TRANSPARENT;

		if( GetItemState(nItem, LVIF_STATE) & LVIS_DROPHILITED)//if dragging show a SelectDropTarget alike effect :)
			uiFlags |= ILD_BLEND50;
		
		if((nWidth=m_cxImage-nWidth) >-1)//calc width of icon
		{
			ImageList_DrawEx(m_himl, lvi.iImage, pDC->m_hDC,  
							 rc.left + (GetIndent(pSelItem) * m_cxImage), 
							 nY, 
							 nWidth,	
							 m_cyImage,
							 CLR_DEFAULT, 
							 CLR_DEFAULT, 
							 uiFlags);
			DrawTreeItem(pDC, pSelItem, nItem, rc);
		}

		//Draw selection bar (erase old selection too)
		pDC->SetBkColor(crBackground);

		CRect rcClip = lpDrawItemStruct->rcItem;
		rcClip.left += GetIndent(pSelItem) * m_cxImage + m_cxImage + 2;
		if(rcClip.left > GetColumnWidth(0))
			rcClip.left = GetColumnWidth(0);
		//fill background color
		ExtTextOut(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, rcClip, NULL, 0, NULL);
		
		//draw color in first col if any
		rcClip.right = rcLabel.right;
		
		COLORREF clf = pItemInfo->GetItemClr();
		if(clf!=-1)
		{	
			CBrush br(clf);
			pDC->FillRect(rcClip, &br);
		}
		//draw selection rect in 1.st col if selected
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (m_CurSubItem==0))
		{
			CBrush br(GetCellRGB());
			pDC->FillRect(rcClip,&br);
			pDC->DrawFocusRect(rcClip);
		}

		//if checkbox style
		UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
		if (nStateImageMask)
		{
			int nImage = (nStateImageMask>>12) - 1;
			CImageList *pImageList = GetImageList(LVSIL_STATE);
			if (pImageList)
			{
				int cxIcon,cyIcon=0;
				ImageList_GetIconSize(pImageList->m_hImageList, &cxIcon, &cyIcon);
				if(rc.left + (GetIndent(pSelItem) * m_cxImage) + m_cxImage + cxIcon < GetColumnWidth(0))
					pImageList->Draw(pDC, nImage,CPoint(rc.left + (GetIndent(pSelItem) * m_cxImage) + cxIcon, nY), ILD_TRANSPARENT);
			}
		}
		
		//that´s new
		CString strItemText = GetData(pSelItem)->GetItemText();
		//draw 1. item	
		GetItemRect(nItem, rcItem, LVIR_LABEL);
		pszText = MakeShortString(pDC, strItemText.GetBuffer(1)/*szBuff*/, rcItem.right - rcItem.left, OFFSET_FIRST);
		rcLabel = rcItem;
		rcLabel.left+=OFFSET_FIRST;
		rcLabel.right-=OFFSET_FIRST;
		
		pDC->SetBkColor (crBackground);
		pDC->SetTextColor (crText);
		pDC->DrawText(pszText,-1, rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

		//draw subitems..
		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;
		for(int nColumn=1; GetColumn(nColumn, &lvc); nColumn++)
		{
			int nSubItemWidth=0;
			rcItem.left=rcItem.right;
			rcItem.right+=lvc.cx;
			if (rcItem.left < rcClipBox.right && rcItem.right > rcClipBox.left && rcItem.right > rcItem.left)
			{
				//support for colors in each cell
				COLORREF clf = pItemInfo->GetBkColor(nColumn-1);
				if(clf!=-1)
				{	
					CBrush br(clf);
					pDC->FillRect(rcItem, &br);
				}
				
				//check for subitems who has an image associated 
				int iSubImage = pItemInfo->GetSubItemImage(nColumn-1); 
				if(iSubImage!=-1)
				{
					//m_cxImage is actually the width of the "tree" imagelist not your subitem imagelist..
					//remember that, when you create your bitmap, I was to lazy getting the icon size of the subitem imagelist
					//in a perfect world you would get the icon size of the subitem image list...
					nSubItemWidth = m_cxImage;
					CRect rect;
					GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
					HIMAGELIST himl = m_iSubItemImage.GetSafeHandle();
					ImageList_DrawEx(himl, iSubImage, pDC->m_hDC,  
									 rect.left, nY, nSubItemWidth, m_cyImage,
									 CLR_DEFAULT,CLR_DEFAULT, uiFlags);
					nSubItemWidth+=2;//+2 arhh you decide
				}

			    CString str = GetData(pSelItem)->GetSubItem(nColumn-1);

				pszText = MakeShortString(pDC,str.GetBuffer(1),(rcItem.right - rcItem.left) - nSubItemWidth, OFFSET_OTHER);

				UINT nJustify=DT_LEFT;

				if(pszText==str.GetBuffer(1))
				{
					switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
					{
					case LVCFMT_RIGHT:
						nJustify=DT_RIGHT;
						break;
					case LVCFMT_CENTER:
						nJustify=DT_CENTER;
						break;
					default:
						break;
					}
				}

				rcLabel=rcItem;
				rcLabel.left+=OFFSET_OTHER + nSubItemWidth;
				rcLabel.right-=OFFSET_OTHER;

				if (lpDrawItemStruct->itemState & ODS_SELECTED && !m_bIsDragging)
					DrawFocusCell(pDC, lpDrawItemStruct->itemID, nColumn, nSubItemWidth);
				if(pszText!=NULL)
					pDC->DrawText(pszText,-1,rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);
			}//if

			//draw down arrow if combobox regardless of rcClipBox rgn
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				DrawComboBox(pDC, pSelItem, nItem, nColumn, nSubItemWidth);
		}//for
	}//ODA_DRAWENTIRE
}



void SuperGridCtrl::DrawComboBox(CDC* pDC, SGTreeItem *pSelItem, int nItem, int nColumn,int /*iSubIconOffset*/)
{
	SGItemInfo* pInfo = GetData(pSelItem);
	SGItemInfo::CONTROLTYPE ctrlType;
	if(pInfo->GetControlType(nColumn-1, ctrlType))
	{
		if(ctrlType == SGItemInfo::combobox) 
		{
			CRect rect;
			GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
			rect.left=rect.right - GetSystemMetrics(SM_CYVSCROLL);
			pDC->DrawFrameControl(rect, DFC_SCROLL, DFCS_SCROLLDOWN);
		}
		//you could draw your own control here
	}
}


//this piece of code is borrowed from the wndproc.c file in the odlistvw.exe example from MSDN and was converted to mfc-style
void SuperGridCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    if (lpMeasureItemStruct->CtlType != ODT_LISTVIEW)
        return;

	TEXTMETRIC tm;
	CClientDC dc(this);	
	CFont* pFont = GetFont();
	CFont* pOldFont = dc.SelectObject(pFont);	
	dc.GetTextMetrics(&tm);
	int nItemHeight = tm.tmHeight + tm.tmExternalLeading;
	lpMeasureItemStruct->itemHeight = nItemHeight + 4; //or should I go for max(nItemheight+4, m_cxImage+2);
	dc.SelectObject(pOldFont);
}



//the basic rutine making the ... thing snatched it from some tedious code example from the MFC rowlist example
//Arne Schild´ had a comment on the static const _TCHAR thing. this is now changed
LPCTSTR SuperGridCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[]=_T("...");
	static CString csShort;

	int nStringLen=lstrlen(lpszLong);

	if(nStringLen==0 || pDC->GetTextExtent(lpszLong,nStringLen).cx + nOffset < nColumnLen+1)
		return(lpszLong);

	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	csShort = lpszLong;

	for(int i=nStringLen-1; i > 0; i--)
	{
		csShort = csShort.Left( csShort.GetLength() - 1 );
		if(pDC->GetTextExtent(csShort,i).cx + nOffset + nAddLen < nColumnLen)
			break;
	}
	csShort += szThreeDots;
	return csShort;
}




void SuperGridCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	switch(pLVKeyDow->wVKey)
	{
		case VK_SPACE: 
			{
				if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
				{	
					int nIndex = GetSelectedItem();
					if(nIndex !=-1)
					{
						SGTreeItem* pItem = GetTreeItem(nIndex);
						if(pItem!=NULL)
						{
							SGItemInfo *pInfo = GetData(pItem);
							pInfo->SetCheck(!pInfo->GetCheck());
						}
					}
				}
			}break;

		case VK_DELETE: 
			{
				int nItem = GetSelectedItem();
				if(nItem!=-1)
				{
					SGTreeItem* pSelItem = GetTreeItem(nItem);
					if(pSelItem != NULL)
					{
						if(OnDeleteItem(pSelItem, nItem))
							DeleteItemEx(pSelItem, nItem);
					}	
				}
			}	break;


		case VK_MULTIPLY:
			{  
				int nIndex = GetSelectedItem();
				if(nIndex != -1)
				{
					CWaitCursor wait;
					SetRedraw(0);
					SGTreeItem *pParent = GetTreeItem(nIndex);
					int nScroll=0;
					if(OnVKMultiply(pParent, nIndex))
					{	
						ExpandAll(pParent, nScroll);
					}		
					SetRedraw(1);
					RedrawItems(nIndex, nScroll);
					EnsureVisible(nScroll, TRUE);
				 }
			 }break;

		case VK_ADD:
			{
					int nIndex = GetSelectedItem();
					if(nIndex!=-1)
					{
						CWaitCursor wait;
						SGTreeItem *pSelItem = GetTreeItem(nIndex);
						int nScrollIndex = 0;
						if(OnVKAdd(pSelItem, nIndex))
						{
							 nScrollIndex = Expand(pSelItem, nIndex);
						}
						InvalidateItemRect(nIndex);
						EnsureVisible(nScrollIndex, 1);
					}

			}break;


		case VK_SUBTRACT:
			{
				int nIndex = GetSelectedItem();
				if(nIndex!=-1)
				{
					CWaitCursor wait;
					SGTreeItem *pSelItem = GetTreeItem(nIndex);
					if(OnVkSubTract(pSelItem, nIndex))
					{
						Collapse(pSelItem);
					}
					InvalidateItemRect(nIndex);
				}
			}break;
		default :break;
	}
	*pResult = 0;
}




BOOL SuperGridCtrl::HitTestOnSign(CPoint point, LVHITTESTINFO& ht)
{
	ht.pt = point;
	// Test which subitem was clicked.
	SubItemHitTest(&ht);
	if(ht.iItem!=-1)
	{
		//first hittest on checkbox
		BOOL bHit = FALSE;
		if(GetExtendedStyle() & LVS_EX_CHECKBOXES)
		{
			if (ht.flags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))//isn't this allways ownerdrawfixed :-)
			{
				CRect rcIcon,rcLabel;
				GetItemRect(ht.iItem, rcIcon, LVIR_ICON);//has to be between these two ....right :)
				GetItemRect(ht.iItem, rcLabel, LVIR_LABEL);
				// check if hit was on a state icon 
				if (point.x > rcIcon.left && point.x < rcLabel.left)
					bHit = TRUE;
			}
			else if (ht.flags & LVHT_ONITEMSTATEICON)
				bHit = TRUE;
		}

		SGTreeItem* pItem = GetTreeItem(ht.iItem);
		if(pItem!=NULL)
		{
			if(bHit)//if checkbox
			{
				//yes I know..have to maintain to sets of checkstates here...
				//one for listview statemask and one for SGTreeItem..but its located here so no harm done
				SetCheck(ht.iItem,!GetCheck(ht.iItem));
				SGItemInfo *pInfo = GetData(pItem);
				pInfo->SetCheck(!pInfo->GetCheck());
			}
			//if haschildren and clicked on + or - then expand/collapse
			if(ItemHasChildren(pItem))
			{
				//hittest on the plus/sign "button" 
				CRect rcBounds;
				GetItemRect(ht.iItem, rcBounds, LVIR_BOUNDS);
				SGRectangle rect(this, NULL, GetIndent(pItem), rcBounds);
				BOOL bRedraw=0;//if OnItemExpanding or OnCollapsing returns false, dont redraw
				if(rect.HitTest(point))
				{
					SetRedraw(0);

					//Arne Schild///////////////////////////////////////////////
					int nScrollIndex = GetCurIndex( pItem ); // AND NOT = 0 !!!
					/////////////////////////////////////////////////////////////

					if(IsCollapsed(pItem))
					{	
						if(OnItemExpanding(pItem, ht.iItem))
						{
							nScrollIndex = Expand(pItem, ht.iItem);
							OnItemExpanded(pItem, ht.iItem);
							bRedraw=1;
						}
					}	
					else {
					   if(OnCollapsing(pItem))
					   {
							Collapse(pItem);
							OnItemCollapsed(pItem);
							bRedraw=1;
					   }
					}
					SetRedraw(1);
					if(bRedraw)
					{
						InvalidateItemRect(ht.iItem);
						EnsureVisible(nScrollIndex, 1);
						return 0;
					}	
				}
			}//has kids
		}//pItem!=NULL
	}
	return 1;
}



void SuperGridCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( GetFocus() != this) 
		SetFocus();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO ht;
	ht.pt = pNMListView->ptAction;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht))
	{
		BOOL bSelect=1;
		bSelect = HitTestOnSign(pNMListView->ptAction, ht);
		//normal selection
		if(bSelect && ht.iItem !=-1)
		{		
			int nIndex = GetSelectedItem();
			if(nIndex!=-1)
			{
				SGTreeItem *pSelItem = GetTreeItem(nIndex);
				if (pSelItem != NULL)
				{
					BOOL bRedraw=0;
					if(ItemHasChildren(pSelItem))
					{
						SetRedraw(0);
						int nScrollIndex=0;
						if(IsCollapsed(pSelItem))
						{		
							if(OnItemExpanding(pSelItem, nIndex))
							{
								nScrollIndex = Expand(pSelItem, nIndex);
								OnItemExpanded(pSelItem, nIndex);
								bRedraw=1;
							}
						}	
					
						else 
						{
						   if(OnCollapsing(pSelItem))
						   {
								Collapse(pSelItem);
								OnItemCollapsed(pSelItem);
								bRedraw=1;
						   }
						}
						SetRedraw(1);

						if(bRedraw)
						{
							InvalidateItemRect(nIndex);
							EnsureVisible(nScrollIndex,1);
						}
					}//ItemHasChildren	
				}//!=NULL
			}
		}
	}
	*pResult = 0;
}



void SuperGridCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		m_nDragItem = pNMListView->iItem;
		CImageList* pDragImage=NULL;
		pDragImage = CreateDragImageEx(m_nDragItem);//override this if you want another dragimage or none at all.
		if(pDragImage)
		{
			pDragImage->BeginDrag(0, CPoint(0,0));
			pDragImage->DragEnter(this, pNMListView->ptAction);
			SetCapture();
			m_bIsDragging = TRUE;
		}
		delete pDragImage;
	}
	*pResult = 0;
}




//Create dragimage : Icon + the itemtext
CImageList *SuperGridCtrl::CreateDragImageEx(int nItem)
{
    CImageList *pList = new CImageList;          
	//get image index
	LV_ITEM lvItem;
	lvItem.mask =  LVIF_IMAGE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	GetItem(&lvItem);

	CRect rc;
	GetItemRect(nItem, &rc, LVIR_BOUNDS);         

	CString str;
	str = GetItemText(nItem, 0);
	CFont *pFont = GetFont();

	rc.OffsetRect(-rc.left, -rc.top);            
	rc.right = GetColumnWidth(0);                
	pList->Create(rc.Width(), rc.Height(),ILC_COLOR24| ILC_MASK , 1, 1);
	CDC *pDC = GetDC();                          
	if(pDC) 
	{
		CDC dc;	      
		dc.CreateCompatibleDC(pDC);      
		CBitmap bmpMap;
		bmpMap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());

		CBitmap *pOldBmp = dc.SelectObject(&bmpMap);
		CFont *pOldFont = dc.SelectObject(pFont);
		dc.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));
		CImageList *pImgList = GetImageList(LVSIL_SMALL);
		if(pImgList)
			pImgList->Draw(&dc, lvItem.iImage, CPoint(0,0), ILD_TRANSPARENT);
		dc.TextOut(m_cxImage + 4, 0, str);
		dc.SelectObject(pOldFont);
		dc.SelectObject(pOldBmp);                 
		//causes an error if the 1st column is hidden so must check the imagelist
		if(pList->m_hImageList != NULL)
			pList->Add(&bmpMap, RGB(255,255,255));
		else { 
			delete pList;
			pList=NULL;
		}
		ReleaseDC(pDC);   
	}   
	return pList;
}



void SuperGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(m_bIsDragging)
    {
		
		KillTimer(1);
		if (CWnd::GetCapture() != this)
			m_bIsDragging=0;
		
		if(nFlags==MK_RBUTTON || nFlags==MK_MBUTTON)
			m_bIsDragging=0;

		
		if(GetKeyState(VK_ESCAPE) < 0)		
			m_bIsDragging=0;
		
		if(!m_bIsDragging)//why not put this in a funtion :)
		{
			SetItemState (m_nDragTarget, 0, LVIS_DROPHILITED);
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			InvalidateRect(NULL);
			UpdateWindow();
		}
		else
		{
			CPoint ptList(point);
			MapWindowPoints(this, &ptList, 1);
			CImageList::DragMove(ptList);
			UINT uHitTest = LVHT_ONITEM;
			m_nDragTarget = HitTest(ptList, &uHitTest);
			// try turn off hilight for previous DROPHILITED state
			int nPrev = GetNextItem(-1,LVNI_DROPHILITED);
			if(nPrev != m_nDragTarget)//prevents flicker 
				SetItemState(nPrev, 0, LVIS_DROPHILITED);

			CRect rect;
			GetClientRect (rect);
			int cy = rect.Height();
			if(m_nDragTarget!=-1)
			{
				SetItemState(m_nDragTarget, LVIS_DROPHILITED, LVIS_DROPHILITED);
				SGTreeItem* pTarget = GetTreeItem(m_nDragTarget);
				if ((point.y >= 0 && point.y <= m_cyImage) || (point.y >= cy - m_cyImage && point.y <= cy) || 	
					( pTarget!=NULL && ItemHasChildren(pTarget) && IsCollapsed(pTarget)))
				{
					SetTimer(1, 300, NULL);
				}
			}
		}
    }
	CCJListCtrl::OnMouseMove(nFlags, point);
}




void SuperGridCtrl::OnTimer(UINT nIDEvent) 
{
	CCJListCtrl::OnTimer(nIDEvent);
	if(nIDEvent==1)
	{
		if(CWnd::GetCapture()!=this)
		{
			SetItemState(m_nDragTarget, 0, LVIS_DROPHILITED);
			m_bIsDragging=0;
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();
			InvalidateRect(NULL);
			UpdateWindow();
			KillTimer(1);
			return;
		}

		SetTimer(1,300,NULL);//reset timer
		DWORD dwPos = ::GetMessagePos();
		CPoint ptList(LOWORD(dwPos),HIWORD(dwPos));
		ScreenToClient(&ptList);

		CRect rect;
		GetClientRect(rect);
		int cy = rect.Height();
		//
		// perform autoscroll if the cursor is near the top or bottom.
		//
		if (ptList.y >= 0 && ptList.y <= m_cyImage) 
		{
			int n = GetTopIndex(); 
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer (1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		else if (ptList.y >= cy - m_cyImage && ptList.y <= cy) 
		{
			int n = GetTopIndex(); 
			CImageList::DragShowNolock(0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			CImageList::DragShowNolock(1);
			if (GetTopIndex()== n)
				KillTimer (1);
			else {
				CImageList::DragShowNolock(0);
				CImageList::DragMove(ptList);
				CImageList::DragShowNolock(1);
				return;
			}
		}
		//Hover test 
		CImageList::DragMove(ptList);
		UINT uHitTest = LVHT_ONITEM;
		m_nDragTarget = HitTest(ptList, &uHitTest);
	
		if(m_nDragTarget!=-1)
		{
			//if the target has children
			//expand them
			SGTreeItem* pTarget=GetTreeItem(m_nDragTarget);
			if(pTarget != NULL && ItemHasChildren(pTarget) && IsCollapsed(pTarget) && (m_nDragItem!=-1))
			{
				CImageList::DragShowNolock(0);
				SGTreeItem* pSource = GetTreeItem(m_nDragItem);
	
				SetRedraw(0);
				int nScrollIndex=0;
				if(ItemHasChildren(pTarget) && IsCollapsed(pTarget))
				{	
					if(OnItemExpanding(pTarget, m_nDragTarget))
					{	
						nScrollIndex = Expand(pTarget, m_nDragTarget);
						OnItemExpanded(pTarget, m_nDragTarget);
					}
				}		
				m_nDragItem = NodeToIndex(pSource);
				SetRedraw(1);
				EnsureVisible(nScrollIndex, 1);
				InvalidateRect(NULL);
				UpdateWindow();
				CImageList::DragShowNolock(1);
				KillTimer(1);
				return;
			}	
		}
		KillTimer(1);
	}
}



void SuperGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bIsDragging == TRUE)
    {
		KillTimer(1);
        CImageList::DragLeave(this);
        CImageList::EndDrag();
        ReleaseCapture();
        m_bIsDragging = FALSE;
		SetItemState(m_nDragTarget, 0, LVIS_DROPHILITED);
        if((m_nDragTarget != -1) && (m_nDragTarget != m_nDragItem) && (m_nDragItem!=-1))//no drop on me self
        {
		
			SGTreeItem* pSource = GetTreeItem(m_nDragItem);
			SGTreeItem* pTarget = GetTreeItem(m_nDragTarget);
			if(IsRoot(pSource))
				return;
			SGTreeItem* pParent = GetParentItem(pSource);
			if(pParent==pTarget) //can't drag child to parent
				return;

			if(!IsChildOf(pSource, pTarget))//can't drag parent to child
			{
				CWaitCursor wait;
				SetRedraw(0);
				if(DoDragDrop(pTarget, pSource))
				{
					UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
					SetItemState(m_nDragTarget, uflag, uflag);
					m_nDragItem=-1;
					//delete source
					int nIndex = NodeToIndex(pSource);			
					DeleteItem(nIndex);
					HideChildren(pSource, TRUE, nIndex);
					Delete(pSource);
					InternaleUpdateTree();
					SetRedraw(1);
					InvalidateRect(NULL);
					UpdateWindow();
				}else
					SetRedraw(1);
			}
	    }
    }
    else
		CCJListCtrl::OnLButtonUp(nFlags, point);
}



//used with the drag/drop operation
void SuperGridCtrl::CopyChildren(SGTreeItem* pDest, SGTreeItem* pSrc)
{
	if (ItemHasChildren(pSrc))
	{
		POSITION pos = pSrc->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			SGTreeItem* pItem = (SGTreeItem *)pSrc->m_listChild.GetNext(pos);
			SGItemInfo* lp = CopyData(GetData(pItem));
			SGTreeItem* pNewItem = InsertItem(pDest, lp);
			CopyChildren(pNewItem, pItem);
		}
	}
}


//hmmm 
BOOL SuperGridCtrl::DoDragDrop(SGTreeItem* pTarget, SGTreeItem* pSource)
{
	if(pTarget==NULL)
		return 0;

	BOOL bUpdate=FALSE;
	if(!IsCollapsed(pTarget))
		bUpdate=TRUE; //children are expanded, want to see update right away
	
	//make a copy of the source data
	SGItemInfo* lp = CopyData(GetData(pSource));
	//create new node with the source data and make pTarget the parent
	
	SGTreeItem* pNewParent = InsertItem(pTarget, lp, bUpdate);
	//if the source has children copy all source data and make the newly create item the parent
	if(ItemHasChildren(pSource))
		CopyChildren(pNewParent, pSource);

	return 1;
}


void SuperGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//its not meself
	if( GetFocus() != this) 
		SetFocus();
	
	CCJListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}




void SuperGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( GetFocus() != this) 
		SetFocus();
	CCJListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}




BOOL SuperGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus()==this)
		{
			switch( pMsg->wParam )
			{
				case VK_LEFT:
					{
						// Decrement the order number.
						m_CurSubItem--;
						if(m_CurSubItem < 0) 
							m_CurSubItem = 0;
						else{
							
							CHeaderCtrl* pHeader = GetHeaderCtrl();
							// Make the column visible.
							// We have to take into account that the header may be reordered.
							MakeColumnVisible( Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
							// Invalidate the item.
							int iItem = GetSelectedItem();
							if( iItem != -1 )
								InvalidateItemRect(iItem);
							
						}
					}
					return TRUE;

				case VK_RIGHT:
					{
						// Increment the order number.
						m_CurSubItem++;
						CHeaderCtrl* pHeader = GetHeaderCtrl();
						int nColumnCount = pHeader->GetItemCount();
						// Don't go beyond the last column.
						if( m_CurSubItem > nColumnCount -1 ) 
							m_CurSubItem = nColumnCount-1;
						else
						{
							MakeColumnVisible(Header_OrderToIndex( pHeader->m_hWnd, m_CurSubItem));
							 
							int iItem = GetSelectedItem();
							// Invalidate the item.
							if( iItem != -1 )
								InvalidateItemRect(iItem);
							
						}
					}
					return TRUE;

				case VK_RETURN://edit itemdata
					{
						BOOL bResult = OnVkReturn();
						if(!bResult)
						{								
							LVHITTESTINFO ht;
							ht.iItem = GetSelectedItem();
							if( m_CurSubItem != -1 && ht.iItem != -1)
							{
								CHeaderCtrl* pHeader = GetHeaderCtrl();
								ht.iSubItem = Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem);
								
								//OnControlLButtonDown only uses the item and subitem of ht
								OnControlLButtonDown(0,0, ht); // sjl
							
								//original code
								/*	if(iSubItem==0)//that's just me saying all nodes in col 0 are edit-controls, you may modify this
								{
									CRect rcItem;
									GetItemRect(iItem, rcItem, LVIR_LABEL);
									DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL|ES_LEFT;
									CEdit *pEdit = new SGEditCtrl(iItem, iSubItem, GetItemText(iItem, iSubItem));
									pEdit->Create(dwStyle, rcItem, this, 0x1233);	
								}
								else
									EditLabelEx(iItem, iSubItem);	*/
								return 1;
							}
						}
					}
					break;
				default:
					break;
			}
		}
	}
	return CCJListCtrl::PreTranslateMessage(pMsg);
}




#define IDC_EDITCTRL 0x1234
CEdit* SuperGridCtrl::EditLabelEx(int nItem, int nCol)
{
	CRect rect;
	int offset = 0;
	if(!EnsureVisible(nItem, TRUE)) 
		return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size(offset + rect.left,0);		
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;

	// Get Column alignment	
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);

	DWORD dwStyle;
	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;	

	//YOU MAY WANNA COMMENT THIS OUT,YOU DECIDE ..
	//now you could take into account here, that an subitem might have an icon
	SGTreeItem *p = GetTreeItem(nItem);
	if(p!=NULL)
	{
		SGItemInfo *pInfo = GetData(p);
		if(pInfo!=NULL)
		{
			int iSubImage = pInfo->GetSubItemImage(nCol-1); 
			if(iSubImage!=-1)
			{
				//m_cxImage is actually the width of the "tree" imagelist not your subitem imagelist..
				//remember that, when you create your bitmap, I was to lazy getting the icon size of the subitem imagelist
				rect.left+=m_cxImage;
			}
		}
	}
	//////////////////////////////////
	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CEdit *pEdit = new SGEditCtrl(nItem, nCol, GetItemText(nItem, nCol));
	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);	
	//pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE); //funny thing happend here, uncomment this, 
												//and then edit an item, 
												//enter a long text so that the ES_AUTOHSCROLL comes to rescue
												//yes that's look funny, ???.
	return pEdit;
}




void SuperGridCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
 	LV_ITEM *plvItem = &plvDispInfo->item;
	if (plvItem->pszText != NULL)//valid text
	{
		if(plvItem->iItem != -1) //valid item
		{
			SGTreeItem*pSelItem = GetTreeItem(plvItem->iItem);
			if(pSelItem != NULL)
				OnUpdateListViewItem(pSelItem, plvItem);
			
		}
	}
	*pResult = 0;
}



int SuperGridCtrl::GetNumCol()
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	return pHeader ? pHeader->GetItemCount() : 0;
}



//Think Rex Myer is spooking here
void SuperGridCtrl::MakeColumnVisible(int nCol)
{
	if(nCol < 0)
		return;
	// Get the order array to total the column offset.
	CHeaderCtrl* pHeader = GetHeaderCtrl();

	int nColCount = pHeader->GetItemCount();
	ASSERT( nCol < nColCount);
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	// Get the column offset
	int offset = 0;
	for(int i = 0; pOrderarray[i] != nCol; i++)
		offset += GetColumnWidth(pOrderarray[i]);

	int colwidth = GetColumnWidth(nCol);
	delete[] pOrderarray;

	CRect rect;
	GetItemRect(0, &rect, LVIR_BOUNDS);
	// Now scroll if we need to show the column
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset + rect.left < 0 || offset + colwidth + rect.left > rcClient.right)
	{
		CSize size(offset + rect.left,0);
		Scroll(size);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}



//Think Rex Myer is spooking here
int SuperGridCtrl::IndexToOrder( int iIndex )
{
	// This translates a column index value to a column order value.
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColCount = pHeader->GetItemCount();
	int *pOrderarray = new int[nColCount];
	Header_GetOrderArray(pHeader->m_hWnd, nColCount, pOrderarray);
	for(int i=0; i<nColCount; i++)
	{
		if(pOrderarray[i] == iIndex )
		{
			delete[] pOrderarray;
			return i;
		}
	}
	delete[] pOrderarray;
	return -1;
}



void SuperGridCtrl::DrawFocusCell(CDC *pDC, int nItem, int iSubItem,int nIconOffset)
{
	if(iSubItem==m_CurSubItem)
	{
		CRect rect;
		GetSubItemRect(nItem, iSubItem, LVIR_BOUNDS, rect);
		CBrush br(GetCellRGB());
		if(iSubItem==0)
			GetItemRect(iSubItem, rect, LVIR_LABEL);
		rect.left+=nIconOffset;
		pDC->FillRect(rect, &br);
		pDC->DrawFocusRect(rect);
	}
}




//insert item and return new parent pointer.
SuperGridCtrl::SGTreeItem* SuperGridCtrl::InsertItem(SGTreeItem *pParent, SGItemInfo* lpInfo,  BOOL bUpdate)
{
	if(pParent==NULL)
		return NULL;

	SGTreeItem *pItem = NULL;
	pItem =  new SGTreeItem();

	if(lpInfo==NULL)
		lpInfo = new SGItemInfo;

	UpdateData(pItem, lpInfo);
	SetIndent(pItem, GetIndent(pParent)+1);
	SetParentItem(pItem, pParent);
	//add as the last child 
	pParent->m_listChild.AddTail(pItem);

	if(!bUpdate)
		 Hide(pParent, TRUE);	
	else
	{
		//calc listview index for the new node
		int nIndex = NodeToIndex(pItem);			
		CString str = GetData(pItem)->GetItemText();
		LV_ITEM     lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
		lvItem.pszText = str.GetBuffer(1); 
		//insert item
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam = (LPARAM)pItem;
		lvItem.iIndent = GetIndent(pItem);
		CCJListCtrl::InsertItem(&lvItem);
		if(lpInfo->GetCheck())
			SetCheck(nIndex);
		//Get subitems
		int nSize = GetData(pItem)->GetItemCount();
		for(int i=0; i < nSize;i++)
		{
		   CString str = GetData(pItem)->GetSubItem(i);
		   lvItem.mask = LVIF_TEXT;
		   lvItem.iSubItem = i+1;
		   lvItem.pszText = str.GetBuffer(1);
		   SetItem(&lvItem);
		}
		InternaleUpdateTree();//better do this
	}
	return pItem;
}	



void SuperGridCtrl::InternaleUpdateTree()
{
	int nItems = GetItemCount();
	for(int nItem=0; nItem < nItems; nItem++)
	{
		SGTreeItem* pItem = GetTreeItem(nItem);
		SetCurIndex(pItem, nItem);
	}
}


int SuperGridCtrl::NodeToIndex(SGTreeItem *pNode)
{
	int nStartIndex=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		SGTreeItem * root = (SGTreeItem*)m_RootItems.GetNext(pos);
		int ret = _NodeToIndex(root, pNode, nStartIndex);
		if(ret != -1)
			return ret;
	}
	return -1;
}




SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetRootItem(int nIndex)
{
	POSITION pos = m_RootItems.FindIndex(nIndex);
	if(pos==NULL)
		return NULL;
	return (SGTreeItem*)m_RootItems.GetAt(pos);
}



int SuperGridCtrl::GetRootIndex(SGTreeItem * root)
{
	int nIndex = 0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos != NULL)
	{
		SGTreeItem * pItem = (SGTreeItem*)m_RootItems.GetNext(pos);
		if(pItem== root)
			return nIndex;
		nIndex++;
	}
	return -1;
}



BOOL SuperGridCtrl::IsRoot(SGTreeItem * lpItem)
{
	return m_RootItems.Find(lpItem) != NULL;
}


void SuperGridCtrl::DeleteRootItem(SGTreeItem * root)
{
	POSITION pos = m_RootItems.Find(root);
	if(pos!=NULL)
	{
		SGTreeItem* pRoot=(SGTreeItem*)m_RootItems.GetAt(pos);
		if(pRoot->m_lpNodeInfo!=NULL)
				delete pRoot->m_lpNodeInfo;
		delete pRoot;
		m_RootItems.RemoveAt(pos);
	}
}



SuperGridCtrl::SGTreeItem*  SuperGridCtrl::InsertRootItem(SGItemInfo * lpInfo)
{
	if(lpInfo==NULL)
		lpInfo = new SGItemInfo;

	SGTreeItem* pRoot = NULL;
	
	pRoot =  new SGTreeItem();

	CleanMe(pRoot);
	UpdateData(pRoot, lpInfo);
	SetIndent(pRoot, 1);
	SetCurIndex(pRoot, GetItemCount());
	SetParentItem(pRoot, NULL);

	SGItemInfo* lp = GetData(pRoot);
	LV_ITEM lvItem;		
	lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
	CString strItem = lp->GetItemText();
	lvItem.pszText = strItem.GetBuffer(1); 
	lvItem.iItem = GetItemCount();
	lvItem.lParam = (LPARAM)pRoot;
	lvItem.iIndent = 1;
	lvItem.iSubItem = 0;
	CCJListCtrl::InsertItem(&lvItem);
	int nSize = lp->GetItemCount();
	for(int i=0; i < nSize;i++)
	{
	   CString str = lp->GetSubItem(i);
	   lvItem.mask = LVIF_TEXT;
	   lvItem.iSubItem = i+1;
	   lvItem.pszText = str.GetBuffer(1);
	   SetItem(&lvItem);
	}
	m_RootItems.AddTail(pRoot);
	return pRoot;
}



void SuperGridCtrl::DrawTreeItem(CDC* pDC, SGTreeItem* pSelItem, int nListItem, const CRect& rcBounds)
{
	int nColWidth = GetColumnWidth(0);
	int yDown = rcBounds.top;
    CPen* pPenTreeLine = pDC->SelectObject(&m_psTreeLine);
	int iIndent = GetIndent(pSelItem);
	int nHalfImage = (m_cxImage >> 1);
	int nBottomDown = yDown + nHalfImage + ((rcBounds.Height() - m_cyImage) >> 1);
	//
	BOOL bChild = ItemHasChildren(pSelItem);
	BOOL bCollapsed = IsCollapsed(pSelItem);
	//draw outline	
	while(1)
	{
		SGTreeItem* pParent = GetParentItem(pSelItem);
		if(pParent==NULL)//no more parents, stop
			break;

		POSITION pos = pParent->m_listChild.GetTailPosition();
		while(pos!=NULL)
		{
			SGTreeItem *pLastChild = (SGTreeItem*)pParent->m_listChild.GetPrev(pos);
			int nIndex = GetCurIndex(pLastChild);
			int nCurIndent = GetIndent(pLastChild);
			if(nListItem > nIndex && iIndent > nCurIndent)//no need to go further in this loop
				break;

			//no drawing outside the 1st columns right
			int xLine =  rcBounds.left + nCurIndent * m_cxImage - nHalfImage;
			if(nIndex == nListItem && nCurIndent==iIndent)
			{
				//draw '-
				int x;
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, nBottomDown);
				// -
				xLine + nHalfImage > nColWidth ? x = nColWidth: x = xLine + nHalfImage;
				
				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent==iIndent)
			{
				//draw |-
				int x;
				xLine + nHalfImage > nColWidth ? x = nColWidth : x = xLine + nHalfImage;
				pDC->MoveTo(xLine, nBottomDown);
				pDC->LineTo(x, nBottomDown);
				//-
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
			else
			if(nIndex > nListItem && nCurIndent < iIndent)
			{
				//draw |
				pDC->MoveTo(xLine, yDown);
				pDC->LineTo(xLine, rcBounds.bottom);
				break;
			}
		}			
		pSelItem = pParent;//next
	}

	//draw plus/minus sign
	if(bChild)
	{
		SGRectangle rect(this, pDC, iIndent, rcBounds);

		rect.DrawRectangle(this);

		CPen* pPenPlusMinus = pDC->SelectObject(&m_psPlusMinus);
		if(bCollapsed)
			rect.DrawPlus();
		else {
			rect.DrawMinus();
			//draw line up to parent folder
			CPen* pLine = pDC->SelectObject(&m_psTreeLine);
			int nOffset = (rcBounds.Height() - m_cyImage)/2;
			pDC->MoveTo(rect.GetLeft()+ m_cxImage, rcBounds.top + m_cyImage+nOffset);
			pDC->LineTo(rect.GetLeft() + m_cxImage, rcBounds.bottom);
			pDC->SelectObject(pLine);		
		}
		pDC->SelectObject(pPenPlusMinus);		
	}
	pDC->SelectObject(pPenTreeLine);
}



//walk all over the place setting the hide/show flag of the nodes.
//it also deletes items from the listviewctrl.
void SuperGridCtrl::HideChildren(SGTreeItem *pItem, BOOL bHide,int nItem)
{
	if(!IsCollapsed(pItem))
	if(ItemHasChildren(pItem))
	{
		Hide(pItem, bHide);
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			HideChildren((SGTreeItem *)pItem->m_listChild.GetNext(pos),bHide,nItem+1);
			DeleteItem(nItem);

		}
	}
}




void SuperGridCtrl::Collapse(SGTreeItem *pItem)
{
	if(pItem != NULL && ItemHasChildren(pItem))
	{
		SetRedraw(0);
		int nIndex = NodeToIndex(pItem);			
		HideChildren(pItem, TRUE, nIndex+1);
		InternaleUpdateTree();
		SetRedraw(1);
	}
}


void SuperGridCtrl::ExpandAll(SGTreeItem *pItem, int& nScroll)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		int nIndex = NodeToIndex(pItem);
		nScroll = Expand(pItem, nIndex);
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		SGTreeItem *pChild = (SGTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandAll(pChild, nScroll);
	}
	
}



int SuperGridCtrl::Expand(SGTreeItem* pSelItem, int nIndex)
{
	if(ItemHasChildren(pSelItem) && IsCollapsed(pSelItem))
	{

		LV_ITEM lvItem;
		lvItem.mask = LVIF_INDENT;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.lParam=(LPARAM)pSelItem;
		lvItem.iIndent = GetIndent(pSelItem);
		SetItem(&lvItem);
		
		Hide(pSelItem, FALSE);
		//expand children
		POSITION pos = pSelItem->m_listChild.GetHeadPosition();
		while(pos != NULL)
		{
			SGTreeItem* pNextNode = (SGTreeItem*)pSelItem->m_listChild.GetNext(pos);
			CString str = GetData(pNextNode)->GetItemText();
			LV_ITEM lvItem;
			lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
			lvItem.pszText =str.GetBuffer(1); 
			lvItem.iItem = nIndex + 1;
			lvItem.iSubItem = 0;
			lvItem.lParam=(LPARAM)pNextNode;
			lvItem.iIndent = GetIndent(pSelItem)+1;
			CCJListCtrl::InsertItem(&lvItem);
			if(GetData(pNextNode)->GetCheck())
				SetCheck(nIndex + 1);
			//get subitems
			int nSize = GetData(pNextNode)->GetItemCount();
			for(int i=0; i< nSize;i++)
			{
			   CString str=GetData(pNextNode)->GetSubItem(i);
			   lvItem.mask = LVIF_TEXT;
			   lvItem.iSubItem = i+1;
			   lvItem.pszText=str.GetBuffer(1);
			   SetItem(&lvItem);
			}
			nIndex++;
		}
	}
	InternaleUpdateTree();
	return nIndex;
}




int SuperGridCtrl::SelectNode(SGTreeItem *pLocateNode)
{
	if(IsRoot(pLocateNode))
	{
		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		SetItemState(0, uflag, uflag);
		return 0;
	}
	int nSelectedItem=-1;
	SGTreeItem* pNode = pLocateNode;
	SGTreeItem* pTopLevelParent=NULL;
	//Get top parent
	while(1)
	{
		SGTreeItem *pParent = GetParentItem(pLocateNode);
		if(IsRoot(pParent))
			break;
		pLocateNode = pParent;
	}
	pTopLevelParent = pLocateNode;//on top of all
	//Expand the folder
	if(pTopLevelParent != NULL)
	{
		SetRedraw(0);
		CWaitCursor wait;
		SGTreeItem *pRoot = GetParentItem(pTopLevelParent);

		if(IsCollapsed(pRoot))
			Expand(pRoot,0);

		ExpandUntil(pTopLevelParent, pNode);

		UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
		nSelectedItem = NodeToIndex(pNode);

		SetItemState(nSelectedItem, uflag, uflag);

		SetRedraw(1);
		EnsureVisible(nSelectedItem, TRUE);
	}
	return nSelectedItem;
}




void SuperGridCtrl::ExpandUntil(SGTreeItem *pItem, SGTreeItem* pStopAt)
{
	const int nChildren = pItem->m_listChild.GetCount();
	if (nChildren > 0)
	{
		POSITION pos = pItem->m_listChild.GetHeadPosition();
		while (pos)
		{
			SGTreeItem *pChild = (SGTreeItem*)pItem->m_listChild.GetNext(pos);
			if(pChild == pStopAt)	
			{
				int nSize = GetIndent(pChild);
				SGTreeItem** ppParentArray = new SGTreeItem*[nSize];
				int i=0;
				while(1)
				{
					SGTreeItem *pParent = GetParentItem(pChild);
					
					if(IsRoot(pParent))
						break;
					pChild = pParent;
					ppParentArray[i] = pChild;
					i++;
				}

				for(int x=i; x > 0; x--)
				{
					SGTreeItem *pParent = ppParentArray[x-1];
					Expand(pParent, NodeToIndex(pParent));
				}
				delete [] ppParentArray;
				return;
			}
		}
	}

	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		SGTreeItem *pChild = (SGTreeItem*)pItem->m_listChild.GetNext(pos);
		ExpandUntil(pChild, pStopAt);
	}
	
}



void SuperGridCtrl::DeleteItemEx(SGTreeItem *pSelItem, int nItem)
{
	SetRedraw(0);
	DeleteItem(nItem);//delete cur item in listview
	//delete/hide all children in pSelItem
	HideChildren(pSelItem, TRUE, nItem);
	//delete all internal nodes
	// If root, must delete from m_rootData
	if(GetParentItem(pSelItem) == NULL )
	{
		DeleteRootItem(pSelItem);
	}
	else
		Delete(pSelItem);

	InternaleUpdateTree();
	if(nItem-1<0)//no more items in list
	{
		SetRedraw(1); 
		InvalidateRect(NULL);
		UpdateWindow();
		return;
	}

	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	CRect rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem;//just to get the documention right :)
	GetItemRect(nItem, rcTestIfItemIsValidToSelectOtherWiseSubtrackOneFromItem ,LVIR_LABEL) ? SetItemState(nItem, uflag, uflag) : SetItemState(nItem-1, uflag, uflag);
	
	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();
}



void SuperGridCtrl::CleanMe(SGTreeItem *pItem)
{
	// delete child nodes
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		SGTreeItem* pItemData = (SGTreeItem*)pItem->m_listChild.GetNext(pos);
		if(pItemData!=NULL)
		{
			if(pItemData->m_lpNodeInfo!=NULL)
				delete pItemData->m_lpNodeInfo;

			pItemData->m_listChild.RemoveAll();
			delete pItemData;
		}
	}
	pItem->m_listChild.RemoveAll();
}




SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetNext(SGTreeItem* pStartAt, SGTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static BOOL bFound;
	if (bInit)
		bFound = FALSE;
		
	if (pNode == pStartAt)
		bFound = TRUE;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				SGTreeItem* pChild = (SGTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE, TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				SGTreeItem* pChild = (SGTreeItem*)pStartAt->m_listChild.GetNext(pos);
				if (bFound)
					return pChild;
				pChild = GetNext(pChild, pNode, FALSE,FALSE);
				if (pChild != NULL)
					return pChild;
			}
	}
	// if reached top and last level return original
	if (bInit)
		return pNode;
	else
		return NULL;
}



SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetPrev(SGTreeItem* pStartAt, SGTreeItem* pNode, BOOL bInit, BOOL bDontIncludeHidden)
{
	static SGTreeItem* pPrev;
	if (bInit)
		pPrev = pStartAt;

	if (pNode == pStartAt)
		return pPrev;

	pPrev = pStartAt;

	if(bDontIncludeHidden)
	{
		if (!IsCollapsed(pStartAt))
		{
			POSITION pos = pStartAt->m_listChild.GetHeadPosition();
			while (pos != NULL)
			{
				SGTreeItem* pCur = (SGTreeItem*)pStartAt->m_listChild.GetNext(pos);
				SGTreeItem* pChild = GetPrev(pCur,pNode, FALSE,TRUE);
				if (pChild != NULL)
					return pChild;
			}
		}
	}
	else {
		POSITION pos = pStartAt->m_listChild.GetHeadPosition();
		while (pos != NULL)
		{
			SGTreeItem* pCur = (SGTreeItem*)pStartAt->m_listChild.GetNext(pos);
			SGTreeItem* pChild = GetPrev(pCur,pNode, FALSE,FALSE);
			if (pChild != NULL)
				return pChild;
		}
	}

	if (bInit)
		return pPrev;
	else
		return NULL;
}


int SuperGridCtrl::_NodeToIndex(SGTreeItem *pStartpos, SGTreeItem* pNode, int& nIndex, BOOL binit)
{
	static BOOL bFound;	
	// Account for other root nodes
	if(GetParentItem(pStartpos) == NULL && GetRootIndex(pStartpos) !=0)
		nIndex++;

	if(binit)
		bFound=FALSE;

	if(pStartpos==pNode)
		bFound=TRUE;

	if(!IsCollapsed(pStartpos))
	{
		POSITION pos = GetHeadPosition(pStartpos);
		while (pos)
		{
			SGTreeItem *pChild = GetNextChild(pStartpos, pos);
			if(bFound)
				return nIndex;

//	Craig Schmidt: Cannot set nIndex as return value.  Worked find with single root but
//				   the calling function get confused since the return value may indicate
//				   that the next root needs to be searched.  Didn'd spend much time on
//				   this so there is probably a better way of doing this.
//			nIndex = _NodeToIndex(pChild, pNode, nIndex, binit);
			_NodeToIndex(pChild, pNode, nIndex, binit);
			nIndex++;
		}
	}
	if(binit && bFound)
		return nIndex;
	else
		return -1;
}


BOOL SuperGridCtrl::Delete(SGTreeItem* pNode, BOOL bClean)
{
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		SGTreeItem * pRoot = (SGTreeItem*)m_RootItems.GetNext(pos);
		if(_Delete(pRoot, pNode, bClean))
			return TRUE;
	}
	return FALSE;
}



BOOL SuperGridCtrl::_Delete(SGTreeItem* pStartAt, SGTreeItem* pNode, BOOL bClean)
{
	POSITION pos = pStartAt->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posPrev = pos;
		SGTreeItem *pChild = (SGTreeItem*)pStartAt->m_listChild.GetNext(pos);
		if (pChild == pNode)
		{
			pStartAt->m_listChild.RemoveAt(posPrev);
			if(bClean)
			{
				if(GetData(pNode)!=NULL)
					delete GetData(pNode);
				delete pNode;
			}
			return TRUE;
		}
		if (_Delete(pChild, pNode) == TRUE)
			return TRUE;
	}
	return FALSE;
}




BOOL SuperGridCtrl::IsChildOf(const SGTreeItem* pParent, const SGTreeItem* pChild) const
{
	if (pChild == pParent)
		return TRUE;
	POSITION pos = pParent->m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		SGTreeItem* pNode = (SGTreeItem*)pParent->m_listChild.GetNext(pos);
		if (IsChildOf(pNode, pChild))
			return TRUE;
	}
	return FALSE;
}



void SuperGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( GetFocus() != this) 
		SetFocus();

	LVHITTESTINFO ht;
	ht.pt = point;
	SubItemHitTest(&ht);
	if(OnItemLButtonDown(ht))
	{
		BOOL bSelect=1;
		bSelect = HitTestOnSign(point, ht);
		if(bSelect && ht.iItem!=-1)
		{
			m_CurSubItem = IndexToOrder(ht.iSubItem);
			CHeaderCtrl* pHeader = GetHeaderCtrl();
			// Make the column fully visible.
			MakeColumnVisible(Header_OrderToIndex(pHeader->m_hWnd, m_CurSubItem));
			CCJListCtrl::OnLButtonDown(nFlags, point);
			OnControlLButtonDown(nFlags, point, ht);
			//update row anyway for selection bar
			InvalidateItemRect(ht.iItem);
		}
	}
}


void SuperGridCtrl::OnUpdateListViewItem(SGTreeItem* lpItem, LV_ITEM *plvItem)
{
	//default implementation you would go for this 9 out of 10 times
	SGItemInfo *lp = GetData(lpItem);
	CString str = (CString)plvItem->pszText;
	if(lp!=NULL)
	{
		if(plvItem->iSubItem==0)
			lp->SetItemText(str);
		else //subitem data 
			lp->SetSubItemText(plvItem->iSubItem-1, str);
	   UpdateData(lpItem, lp); // do not use bUpdateRow here, hence we only update a specific item or subitem..not all of them
	}
	SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);

}


//////////////// Arne Schild ///////////////////////////////////////////////
void SuperGridCtrl::DeleteAll(bool update)
{
	SetRedraw(false);
	DeleteAllItems();
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		SGTreeItem * pRoot = (SGTreeItem*)m_RootItems.GetNext(pos);
		if(pRoot!=NULL)
		{
			//DeleteItem(0);//delete cur item in listview: no Sorry Arne you don´t need this
			//HideChildren(pRoot, TRUE, 0);  no Sorry Arne you don´t need this

			// If root, must delete from m_rootData
			if(GetParentItem(pRoot) == NULL )
			{
				DeleteRootItem(pRoot);
			}
			else
				Delete(pRoot);
		//	InternaleUpdateTree(); no Sorry Arne you don´t need too
		//	other than that, thanks.
		}
	}
	SetRedraw(true);
	if(update)
	{
		InvalidateRect(NULL);
		UpdateWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////// 
/*
previous code for DeleteAll 

void SuperGridCtrl::DeleteAll()
{
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		SGTreeItem * pRoot = (SGTreeItem*)m_RootItems.GetNext(pos);
		if(pRoot!=NULL)
			DeleteItemEx(pRoot, 0);
	}
}
*/

POSITION SuperGridCtrl::GetRootHeadPosition() const
{
	return m_RootItems.GetHeadPosition();
}


POSITION SuperGridCtrl::GetRootTailPosition() const
{
	return m_RootItems.GetTailPosition();
}


SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetNextRoot(POSITION& pos) const
{
	return (SGTreeItem*)m_RootItems.GetNext(pos);
}


SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetPrevRoot(POSITION& pos) const
{
	return (SGTreeItem*)m_RootItems.GetNext(pos);
}


POSITION SuperGridCtrl::GetHeadPosition(SGTreeItem* pItem) const
{
	return pItem->m_listChild.GetHeadPosition();
}



SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetNextChild(SGTreeItem *pItem, POSITION& pos) const
{
	return (SGTreeItem*)pItem->m_listChild.GetNext(pos);
}



SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetPrevChild(SGTreeItem *pItem, POSITION& pos) const
{
	return (SGTreeItem*)pItem->m_listChild.GetPrev(pos);
}



POSITION SuperGridCtrl::GetTailPosition(SGTreeItem *pItem) const
{
	return pItem->m_listChild.GetTailPosition();
}



void SuperGridCtrl::AddTail(SGTreeItem *pParent, SGTreeItem *pChild)
{
	pParent->m_listChild.AddTail(pChild);
}


inline int StrComp(const CString* pElement1, const CString* pElement2)
{
	return pElement1->Compare(*pElement2);
}



int SuperGridCtrl::CompareChildren(const void* p1, const void* p2)
{
	SGTreeItem * pChild1 = *(SGTreeItem**)p1;
	SGTreeItem * pChild2 = *((SGTreeItem**)p2);
	SGItemInfo *pItem1=(*pChild1).m_lpNodeInfo;
	SGItemInfo *pItem2=(*pChild2).m_lpNodeInfo;
	return StrComp(&(pItem1->GetItemText()), &(pItem2->GetItemText()));
}


void SuperGridCtrl::Sort(SGTreeItem* pParent, BOOL bSortChildren)
{
	const int nChildren = NumChildren(pParent);
	if (nChildren > 1)
	{
		SGTreeItem** ppSortArray = new SGTreeItem*[nChildren];
		// Fill in array with pointers to our children.
		POSITION pos = pParent->m_listChild.GetHeadPosition();
		for (int i=0; pos; i++)
		{
			ASSERT(i < nChildren);
			ppSortArray[i] = (SGTreeItem*)pParent->m_listChild.GetAt(pos);
			pParent->m_listChild.GetNext(pos);
		}

		qsort(ppSortArray, nChildren, sizeof(SGTreeItem*), CompareChildren);
		// reorg children with new sorted list
		pos = pParent->m_listChild.GetHeadPosition();
		for (int i=0; pos; i++)
		{
			ASSERT(i < nChildren);
			pParent->m_listChild.SetAt(pos, ppSortArray[i]);
			pParent->m_listChild.GetNext(pos);
		}

		delete [] ppSortArray;
	}

	if(bSortChildren)
	{
		POSITION pos = pParent->m_listChild.GetHeadPosition();
		while (pos)
		{
			SGTreeItem *pChild = (SGTreeItem*)pParent->m_listChild.GetNext(pos);
			Sort(pChild, TRUE);
		}
	}
}

void SuperGridCtrl::SortEx(BOOL bSortChildren)
{
	int nItems = m_RootItems.GetCount();
	if (nItems > 0)
	{
		if(bSortChildren)
		{
			POSITION posSortChildren = GetRootHeadPosition();
			while(posSortChildren != NULL)
			{
					SGTreeItem *pParent =(SGTreeItem*)GetNextRoot(posSortChildren); 
					Sort(pParent, TRUE);//sort children			
			}	
		}		
		//set hideflag for rootitems
		POSITION posHide = GetRootHeadPosition();
		while(posHide != NULL)
		{
				SGTreeItem *pParent =(SGTreeItem*)GetNextRoot(posHide); 
				Collapse(pParent);
		}	

		DeleteAllItems();//this is quite okay, I don´t delete the internal state
		//sort rootitems
		SGTreeItem** ppSortArray = new SGTreeItem*[nItems];
		// Fill in array with pointers to our children.
		POSITION posCur = m_RootItems.GetHeadPosition();
		for (int i=0; posCur; i++)
		{
			ppSortArray[i] = (SGTreeItem*)m_RootItems.GetAt(posCur);
			m_RootItems.GetNext(posCur);
		}

		qsort(ppSortArray, nItems, sizeof(SGTreeItem*), CompareChildren);
		// reorg rootitems with new sorted list
		posCur = m_RootItems.GetHeadPosition();
		for (int i=0; posCur; i++)
		{
			m_RootItems.SetAt(posCur, ppSortArray[i]);
			m_RootItems.GetNext(posCur);
		}
		delete [] ppSortArray;
		//do a "repaint" of only the rootitems...you could "refresh" the children with a expand 
		int nIndex=0;//no suprise here
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			SGTreeItem *pParent = (SGTreeItem*)GetNextRoot(pos); 
			if(pParent!=NULL)
			{
				LV_ITEM lvItem;
				lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
				SGItemInfo* lp = GetData(pParent); 
				CString str = lp->GetItemText();
				lvItem.pszText = str.GetBuffer(1); 
				lvItem.iItem = nIndex;
				lvItem.iSubItem = 0;
				lvItem.lParam = (LPARAM)pParent;//associate the root and all its children with this listviewitem
				lvItem.iIndent = GetIndent(pParent);
				CCJListCtrl::InsertItem(&lvItem);
				int nSize = lp->GetItemCount();
				for(int i=0; i < nSize; i++)
				{
				   CString strSubItems = lp->GetSubItem(i);
				   lvItem.mask = LVIF_TEXT;
				   lvItem.iSubItem = i+1;
				   lvItem.pszText = strSubItems.GetBuffer(1);
				   SetItem(&lvItem);
				}
				nIndex++;
			}
		}//while
	}//nItems
}


int SuperGridCtrl::NumChildren(const SGTreeItem *pItem) const
{
	return pItem->m_listChild.GetCount();
}


BOOL SuperGridCtrl::ItemHasChildren(const SGTreeItem* pItem) const
{ 
	BOOL bChildren = pItem->m_listChild.GetCount() != 0;
	//see if we have a flag
	int nFlag = pItem->m_bSetChildFlag;
	if(nFlag!=-1)
		return 1;
	else
		return bChildren;
}


void SuperGridCtrl::SetChildrenFlag(SGTreeItem *pItem, int nFlag) const
{
	pItem->m_bSetChildFlag = nFlag;
}


BOOL SuperGridCtrl::IsCollapsed(const SGTreeItem* pItem) const
{
	return pItem->m_bHideChildren;//e.g not visible
}


void SuperGridCtrl::Hide(SGTreeItem* pItem, BOOL bFlag)
{
	pItem->m_bHideChildren=bFlag;
}


int SuperGridCtrl::GetIndent(const SGTreeItem* pItem) const
{
	return pItem->m_nIndent;
}


void SuperGridCtrl::SetIndent(SGTreeItem *pItem, int iIndent)
{
	pItem->m_nIndent = iIndent;
}


int SuperGridCtrl::GetCurIndex(const SGTreeItem *pItem) const
{
	return pItem->m_nIndex;
}


void SuperGridCtrl::SetCurIndex(SGTreeItem* pItem, int nIndex) 
{
	pItem->m_nIndex = nIndex;
}


void SuperGridCtrl::SetParentItem(SGTreeItem*pItem, SGTreeItem* pParent)
{
	pItem->m_pParent=pParent;

}


SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetParentItem(const SGTreeItem* pItem) 
{
	return pItem->m_pParent;
};



SGItemInfo* SuperGridCtrl::GetData(const SGTreeItem* pItem) 
{
	return pItem->m_lpNodeInfo;
}




void SuperGridCtrl::UpdateData(SGTreeItem* pItem, SGItemInfo* lpInfo, BOOL bUpdateRow)
{
	pItem->m_lpNodeInfo = lpInfo;
	if(bUpdateRow)//update listview item and subitems
		InvalidateItem(pItem);
}


//overrides
SGItemInfo* SuperGridCtrl::CopyData(SGItemInfo* /*lpSrc*/)
{
	ASSERT(FALSE);//debug
	return NULL;  //release
}

//default implementation for setting icons
int SuperGridCtrl::GetIcon(const SGTreeItem* /*pItem*/)
{
	
	return 0;//just take the first item in CImageList ..what ever that is
}


void SuperGridCtrl::OnControlLButtonDown(UINT /*nFlags*/, CPoint /*point*/, LVHITTESTINFO& ht)
{
	SGTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		SGItemInfo* pInfo = GetData(pSelItem);
		SGItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
			if(ctrlType == SGItemInfo::static_ctrl) 
			{
				//do nothing - sjl
				return ;
			}
			else if(ctrlType== SGItemInfo::edit) 
			{
				if(ht.iSubItem==0)
				{
					CRect rcItem;
					GetItemRect(ht.iItem, rcItem, LVIR_LABEL);
					DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL|ES_LEFT;
					CEdit *pEdit = new SGEditCtrl(ht.iItem, ht.iSubItem, GetItemText(ht.iItem, ht.iSubItem));
					pEdit->Create(dwStyle, rcItem, this, 0x1233);	
				}
				else
					EditLabelEx(ht.iItem, ht.iSubItem);	
			}
		}	
	}	
}
	

COLORREF SuperGridCtrl::GetCellRGB()
{
	return GetSysColor(COLOR_HIGHLIGHT);
}

	
BOOL SuperGridCtrl::OnVKMultiply(SGTreeItem * /*pItem*/, int /*nIndex*/)
{
	return 1;
}



BOOL SuperGridCtrl::OnVkSubTract(SGTreeItem * /*pItem*/, int /*nIndex*/)
{
	return 1;
}



BOOL SuperGridCtrl::OnVKAdd(SGTreeItem * /*pItem*/, int /*nIndex*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnDeleteItem(SGTreeItem* /*pItem*/, int /*nIndex*/)
{
	return 1;
}



BOOL SuperGridCtrl::OnItemExpanding(SGTreeItem * /*pItem*/, int /*iItem*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnItemExpanded(SGTreeItem* /*pItem*/, int /*iItem*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnCollapsing(SGTreeItem * /*pItem*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnItemCollapsed(SGTreeItem * /*pItem*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnItemLButtonDown(LVHITTESTINFO& /*ht*/)
{
	return 1;
}


BOOL SuperGridCtrl::OnVkReturn(void)
{
	return 0;
}


void SuperGridCtrl::OnSysColorChange() 
{
	CCJListCtrl::OnSysColorChange();
	//nop nothing yet
}



UINT SuperGridCtrl::_GetCount(SGTreeItem* pItem, UINT& nCount)
{
	POSITION pos = pItem->m_listChild.GetHeadPosition();
	while (pos)
	{
		SGTreeItem *pChild = (SGTreeItem*)pItem->m_listChild.GetNext(pos);
		nCount = _GetCount(pChild, nCount);
		nCount++;				
	}
	return nCount;
}



UINT SuperGridCtrl::GetCount(void) 
{
	UINT nCount=0;
	UINT _nCount=0;
	POSITION pos = m_RootItems.GetHeadPosition();
	while(pos!=NULL)
	{
		SGTreeItem * pRoot = (SGTreeItem*)m_RootItems.GetNext(pos);
		nCount += _GetCount(pRoot, _nCount) + 1;
	}
	return nCount;
}



SuperGridCtrl::SGTreeItem* SuperGridCtrl::GetTreeItem(int nIndex /*nIndex must be valid of course*/ ) 
{
	return reinterpret_cast<SGTreeItem*>(GetItemData(nIndex));
}



int SuperGridCtrl::GetSelectedItem(void) const
{
	return GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
}


void SuperGridCtrl::InvalidateItem(SGTreeItem *pItem)
{
	if(pItem!=NULL)
	{
		int nItem = GetCurIndex(pItem);//just has to be visible
		if(nItem != -1)
		{
			SGItemInfo *lp = GetData(pItem);
			int nSize = lp->GetItemCount();
			SetRedraw(false);
			SetItemText(nItem, 0,lp->GetItemText());
			for(int i=0; i < nSize;i++)
			{
				CString str = lp->GetSubItem(i);
				SetItemText(nItem, i+1, str.GetBuffer(1));
			}	
			SetRedraw(true);
		}
	}
}

void SuperGridCtrl::InvalidateItemRect(int nItem/*better be valid item*/)
{
	if(nItem != -1)
	{
		CRect rc;
		GetItemRect(nItem, rc, LVIR_BOUNDS);
		InvalidateRect(rc);
		UpdateWindow();
	}
}


void SuperGridCtrl::InvalidateItemRectPtr(SGTreeItem *pItem)	
{
	if(pItem!=NULL)
	{
		int nItem = GetCurIndex(pItem);
		if(nItem!=-1)
			InvalidateItemRect(nItem);
	}
}

//////////////////////////////////////////////////////////////////////////
//
// not much but ... 
//
//////////////////////////////////////////////////////////////////////////

SuperGridCtrl::SGTreeItem::~SGTreeItem()
{
	// delete child nodes
	POSITION pos = m_listChild.GetHeadPosition();
	while (pos != NULL)
	{
		SGTreeItem* pItem = (SGTreeItem*)m_listChild.GetNext(pos);
		if(pItem!=NULL)
		{
			if(pItem->m_lpNodeInfo!=NULL)
				delete pItem->m_lpNodeInfo;
			delete pItem;
		}
	}
	m_listChild.RemoveAll();
}


