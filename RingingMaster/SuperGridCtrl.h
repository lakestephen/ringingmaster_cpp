#if !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
#define AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuperGridCtrl.h : header file
//
#include "CJListCtrl.h"
#include "SGItemInfo.h"


/////////////////////////////////////////////////////////////////////////////
//
// SuperGridCtrl 
//
/////////////////////////////////////////////////////////////////////////////
class SuperGridCtrl : public CCJListCtrl
{
// Construction
public:
	SuperGridCtrl();
protected:
	//nested class forward decl.
	class SGTreeItem;
public:
	// Overrides
	//MUST override this to make a copy of SGItemInfo..see the CMySuperGrid.cpp for implementation
	//used in drag/drop operations
	virtual SGItemInfo* CopyData(SGItemInfo* lpSrc);
	//sets the icon state...called from within DrawItem returns valid image index
	//You MUST override this function to set your current icon, must of course be a valid CImageList index
	virtual int GetIcon(const SGTreeItem* pItem);
	//override this to set the color for current cell
	virtual COLORREF GetCellRGB(void);
	//override this to update listview items, called from within OnEndlabeledit.
	virtual void OnUpdateListViewItem(SGTreeItem* lpItem, LV_ITEM *plvItem);
	//override this to activate any control when lButtonDown in a cell, called from within OnLButtonDown
	virtual void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);
	//override this to provide your dragimage, default: creates an image + text
	virtual CImageList *CreateDragImageEx(int nItem);
    //called before item is about to explode, return TRUE to continue, FALSE to prevent expanding
	virtual BOOL OnItemExpanding(SGTreeItem *pItem, int iItem);
	//called after item has expanded
	virtual BOOL OnItemExpanded(SGTreeItem* pItem, int iItem);
	//called before item are collapsed,return TRUE to continue, FALSE to prevent collapse
	virtual BOOL OnCollapsing(SGTreeItem *pItem);
	//called after item has collapsed
	virtual BOOL OnItemCollapsed(SGTreeItem *pItem);
	//called before item is about to be deleted,return TRUE to continue, FALSE to prevent delete item
	virtual BOOL OnDeleteItem(SGTreeItem* pItem, int nIndex);
	//called before VK_MULTIPLY keydown, return TRUE to continue, FALSE to prevent expandall
	virtual BOOL OnVKMultiply(SGTreeItem *pItem, int nIndex);
	//called before VK_SUBTRACT keydown, return TRUE to continue, FALSE to prevent collapse item
	virtual BOOL OnVkSubTract(SGTreeItem *pItem, int nIndex);
	//called before VK_ADD keydown, return TRUE to continue, FALSE to prevent expanding item
	virtual BOOL OnVKAdd(SGTreeItem *pItem, int nIndex);
	//called from PreTranslateMessage, override this to handle other controls than editctrl's
	virtual BOOL OnVkReturn(void);
	//called before from within OnlButtonDown and OnDblclk, but before anything happens, return TRUE to continue, FALSE to say not selecting the item
	virtual BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
public:
	//creates a root
	SGTreeItem*  InsertRootItem(SGItemInfo * lpRoot);
	//from the looks of it,...it deletes a rootitem
	void DeleteRootItem(SGTreeItem * lpRoot);
	//hmm
	BOOL IsRoot(SGTreeItem * lpItem);
	//given the rootindex it returns the rootptr
	SGTreeItem* GetRootItem(int nIndex);
	int GetRootCount() { return m_RootItems.GetCount();}
	//call this to delete all items in grid
	void DeleteAll(bool update = true);
	//return previous node from pItem, given a RootItem
	SGTreeItem* GetPrev(SGTreeItem* pRoot, SGTreeItem *pItem, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//return next node from pItem, given a RootItem
	SGTreeItem* GetNext(SGTreeItem* pRoot, SGTreeItem* pNode, BOOL bInit = TRUE, BOOL bDontIncludeHidden=TRUE);
	//returns the selected item :)
	int GetSelectedItem(void) const;
	//returns the itemdata associated with the supergrid
	SGTreeItem* GetTreeItem(int nIndex);
	// Retrieves the number of items associated with the SuperGrid control.
	UINT GetCount(void);
	//returns number of children given the pItem node ptr.
	int NumChildren(const SGTreeItem *pItem) const;
	//Determines if this tree item is a child of the specified parent
	BOOL IsChildOf(const SGTreeItem* pParent, const SGTreeItem* pChild) const;
	//hmm
	BOOL ItemHasChildren(const SGTreeItem* pItem) const;
	// Use this to indicate that pItem has children, but has not been inserted yet.
	void SetChildrenFlag(SGTreeItem *pItem,int nFlag) const;
	//are children collapsed
	BOOL IsCollapsed(const SGTreeItem* pItem) const;
	//return the Indent Level of pItem
	int GetIndent(const SGTreeItem* pItem) const;
	//Sets the Indentlevel of pItem
	void SetIndent(SGTreeItem *pItem, int iIndent);
	//get the pItems current listview index, 
	int GetCurIndex(const SGTreeItem *pItem) const;
	//set pItems current listview index
	void SetCurIndex(SGTreeItem* pItem, int nIndex);
	//sets the pItem' parent
	void SetParentItem(SGTreeItem*pItem, SGTreeItem* pParent);
	//gets pItems parent
	SGTreeItem* GetParentItem(const SGTreeItem* pItem); 
	//return ptr to SGItemInfo daaa
	SGItemInfo* GetData(const SGTreeItem* pItem); 
	//sets the SGItemInfo ptr of pItem, if bUpdateRow true, it uses the InvalidateItem(SGTreeItem pItem) 
	void UpdateData(SGTreeItem* pItem, SGItemInfo* lpInfo, BOOL bUpdateRow=FALSE);
	//Insert item and return new parent node.
	//the bUpdate is here for performance reasons, when you insert say 100 node each having 10 children(1000 listview items)
	//the bUpdate should be set to FALSE(default) but when you want to insert an item, and you want to user to see it right away
	//set bUpdate to TRUE.(see the use of bUpdate in the HowToInsertItemsAfterTheGridHasBeenInitialized function in the CMySuperGridCtrl)
	SGTreeItem* InsertItem(SGTreeItem *pParent, SGItemInfo* lpInfo, BOOL bUpdate=FALSE);
	//collapse all children from pItem
	void Collapse(SGTreeItem *pItem);
	//expand one folder and return the last index of the expanded folder
	int Expand(SGTreeItem* pSelItem, int nIndex);
	//expand all items from pItem and return last index of the expanded folder
	void ExpandAll(SGTreeItem *pItem, int& nScroll);
	//expand all node in pItem and stop at pStopAt, used in SelectNode function
	void ExpandUntil(SGTreeItem *pItem, SGTreeItem* pStopAt);
	//use this if you want to select a node 
	//if the node is collapsed all items with in the node are expanded and the node is selected
	//it returns the listview index for the selected node
	int SelectNode(SGTreeItem *pLocateNode);
	//Delete an item in the listview 
	//takes the node to be delete and its listview item index as arg.
	//note the item you delete must be visible, hence the nItem as arg. 
	void DeleteItemEx(SGTreeItem *pSelItem, int nItem);
	//returns the number of columns in the listview
	int GetNumCol(void);
	//does a Quicksort of the rootitems and all children if bSortChildren set TRUE
	void SortEx(BOOL bSortChildren);
	//does a Quicksort of the pParents children and if bSortChildren set, 
	//all items from pParent are sorted. 
	void Sort(SGTreeItem* pParent, BOOL bSortChildren);
	// simpel wrapper for the CObList in SGTreeItem, same usage as in the COblist
	POSITION GetHeadPosition(SGTreeItem* pItem) const;
	POSITION GetTailPosition(SGTreeItem *pItem) const;
	SGTreeItem* GetNextChild(SGTreeItem *pItem, POSITION& pos) const;
	SGTreeItem* GetPrevChild(SGTreeItem *pItem, POSITION& pos) const;
	void AddTail(SGTreeItem *pParent, SGTreeItem *pChild);
	//simpel wrapper for the CPtrList collection of rootitems
	//feel free to add more of these simple wrappers.
	POSITION GetRootHeadPosition(void) const;
	POSITION GetRootTailPosition(void) const;
	SGTreeItem* GetNextRoot(POSITION& pos) const;
	SGTreeItem* GetPrevRoot(POSITION& pos) const;
	//invalidates the nItems rectbound.
	void InvalidateItemRect(int nItem);
	//invalidates the pItem rectbound.
	void InvalidateItemRectPtr(SGTreeItem *pItem);	
	//use this when you do dynamic updates, 
	//it writes the content of pItem to the listview
	void InvalidateItem(SGTreeItem *pItem);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SuperGridCtrl)
	public:
	//handle VK_xxxx
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~SuperGridCtrl();
protected:
	//given the rootptr it returns the rootindex.
	int GetRootIndex(SGTreeItem * lpRoot);
	//delete pItem and all of its children
	BOOL Delete(SGTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	//used in drag/drop operation..
	void CopyChildren(SGTreeItem* pDest, SGTreeItem* pSrc);
	//drag/drop thing....clipboard not supported!
	BOOL DoDragDrop(SGTreeItem* pTarget, SGTreeItem* pSelItem);
	//updates internal nodes, called when ever insert,delete on listview
	void InternaleUpdateTree(void);
	//Get ListView Index from pNode
	int NodeToIndex(SGTreeItem *pNode);
	//see if user clicked the [+] [-] sign, also handles LVS_EX_CHECKBOXES.
	BOOL HitTestOnSign(CPoint point, LVHITTESTINFO& ht);
	//positions an edit-control and creates it
	CEdit* EditLabelEx(int nItem, int nCol);
	int m_cxImage, m_cyImage;//icon size
	//translates column index value to a column order value.
	int IndexToOrder(int iIndex);
	//set hideflag for pItem
	void Hide(SGTreeItem* pItem, BOOL bFlag);
	//hmm
	int GetCurSubItem(void){return m_CurSubItem;}
	int GetDragItem(void) const {return m_nDragItem; }
	int GetDropTargetItem(void) const {return m_nDragTarget; }
	BOOL SetSubItemImageList(CImageList *pImageList=NULL)
	{
		if(pImageList!=NULL)
		{
			m_iSubItemImage.Create(pImageList);
			int nCount=pImageList->GetImageCount(); 
			for (int i=0; i < nCount; i++)
				 m_iSubItemImage.Copy(i,pImageList,i);

			return TRUE;
		}
		else return FALSE;
	}
private:
	//imagelist for subitems
	CImageList m_iSubItemImage;
	//list of rootItems
	CPtrList m_RootItems;
	//internal helpers
	BOOL _Delete(SGTreeItem* pStartAt, SGTreeItem *pItem, BOOL bClean=TRUE/*delete itemdata*/);
	UINT _GetCount(SGTreeItem* pItem, UINT& nCount);
	//hmm
	void DrawTreeItem(CDC* pDC, SGTreeItem* pSelItem, int nListItem, const CRect& rcBounds);
	//makes the dot ... thing
	LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
	//set the hideflag from pItem and all its children
	void HideChildren(SGTreeItem *pItem, BOOL bHide, int iIndent);
	//checks whether a column is visible, if not scrolls to it
	void MakeColumnVisible(int nCol);
	//hmm
	void DrawFocusCell(CDC *pDC, int nItem, int iSubItem,int nIconOffset);
	//draw the down arrow if any combobox precent in the listview item
	void DrawComboBox(CDC *pDC, SGTreeItem *pSelItem, int nItem, int nColumn,int iSubIconOffset);
	int m_CurSubItem;//keyboard handling..it is what it says
	//hmm these represents the state of my expresso machine
    int m_nDragItem, m_nDragTarget;
    BOOL m_bIsDragging;
	//helper compare fn used with Quicksort
	static int CompareChildren(const void* p1, const void* p2);
	void CleanMe(SGTreeItem *pItem);
	int _NodeToIndex(SGTreeItem *pStartpos, SGTreeItem* pNode, int& nIndex, BOOL binit = TRUE);
	CPen m_psTreeLine;
	CPen m_psRectangle;
	CPen m_psPlusMinus;
	CBrush m_brushErase;
	friend class SGRectangle;
	HIMAGELIST m_himl;
	// Generated message map functions
protected:
	//{{AFX_MSG(SuperGridCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);	
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//the nested class 
class SuperGridCtrl::SGTreeItem : public CObject
{
	SGTreeItem(): m_pParent(NULL), m_bHideChildren(0), m_nIndex(-1), m_nIndent(-1),m_bSetChildFlag(-1){};
	~SGTreeItem();
	CObList m_listChild;
	SGTreeItem* m_pParent;
	SGItemInfo* m_lpNodeInfo;
	BOOL m_bHideChildren;  
	int m_nIndex; //CListCtrl index
	int m_nIndent; 
	int m_bSetChildFlag;
	friend class SuperGridCtrl;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERGRIDCTRL_H__C6DF1701_806D_11D2_9A94_002018026B76__INCLUDED_)
