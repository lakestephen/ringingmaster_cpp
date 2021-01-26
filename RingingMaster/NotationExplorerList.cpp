// NotationExplorerList.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationExplorerList.h"
#include "NotationExplorerDlg.h"
#include "NotationLite.h"
#include "NotationDlg.h"

#include "MethodPosition.h"
#include "MethodManager.h"
#include "Lead.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerList



NotationExplorerList::NotationExplorerList() 
{

}

NotationExplorerList::~NotationExplorerList()
{

}

void NotationExplorerList::setParent(NotationExplorerDlg *parent)
{
	_parent = parent;
}

BEGIN_MESSAGE_MAP(NotationExplorerList, NotationListCtrl)
	//{{AFX_MSG_MAP(NotationExplorerList)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ME_LIST_EDIT, OnExplorerListEdit)
	ON_COMMAND(ID_ME_NEW_METHOD, OnMeNewMethod)
	ON_COMMAND(ID_ME_LIST_DELETE, OnMeListDelete)
	ON_COMMAND(ID_ME_NEW_FOLDER, OnMeNewFolder)
	ON_COMMAND(ID_ME_RENAME_METHOD, OnMeRenameMethod)
	ON_COMMAND(ID_ME_LIST_VIEW, OnMeListView)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_PASTE, OnUpdateEditPaste)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerList message handlers

//this adds the methods to the virtual list
void NotationExplorerList::showMethods()
{
	CWaitCursor wait;

	//GET THE CURRENT FOLDER
	unsigned long folderID = _parent->_tree.getCurFolderID();
	CMap<unsigned long, unsigned long, unsigned long, unsigned long> itemMap;
	itemMap.SetAt(folderID,0);


	//search all the notations
	DeleteAllItems();

	//suppress drawing
	SetRedraw(FALSE);
	
	delete _liteList;
	_liteList = NULL;
	
	_liteList = new NotationLite*[methodManager()->getMethodCount()]; 
	_liteListCount = 0;

	_childFolders.RemoveAll();

	//get a map of IDs of all sub folders if we have a flattened hirachy.
	if (_parent->isFlattened())
	{
		UnsignedLongs items;
		_parent->_tree.getChildrenFolderID(items, true);
		for (int i=0;i<items.GetSize();i++)
			itemMap.SetAt(items.GetAt(i),0);
	}
	else
	{
		//add in the immediate children as folders
		_parent->_tree.getChildrenFolderID(_childFolders, false);
	}



	//see what we need to add
	unsigned long temp;
	for (int i=0;i<methodManager()->getMethodCount();i++)
	{
		NotationLite* not = methodManager()->getMethod(i);
		
		if ((itemMap.Lookup(not->getFolderID(), temp))&&
			((!_parent->_filterState)||(not->getNumber() == _parent->_filterNumber)))
		{
			_liteList[_liteListCount++] = not;
		}
	}

	NotationListCtrl::showMethods();	
}


void NotationExplorerList::init()
{
	NotationListCtrl::init();
	
	AutoSaveColumns(NULL, "Notation Explorer Columns", "148,52,173,38,168");

	m_bAscending = (app()->GetProfileInt("Explorer", "Last Explorer Sort Direction", TRUE) == TRUE);
	m_nSortedCol = app()->GetProfileInt("Explorer", "Last Explorer Sort Column", 0);


	showMethods();
}


 
void NotationExplorerList::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
 	CPoint hitPoint;
 	GetCursorPos(&hitPoint);
 	ScreenToClient(&hitPoint);
	_lastContextItem = HitTest(hitPoint);

	if ((_lastContextItem >= 0)&&(_lastContextItem < _childFolders.GetSize()))
	{
		_parent->_tree.selectItem(_childFolders.GetAt(_lastContextItem));
	}
	else if (_parent->_databaseMaintainance)
 	{
 		if (_lastContextItem != -1)
 			OnMePreview();
 	}
 	else
		_parent->makeSelection();	
	
	*pResult = 0;
}



void NotationExplorerList::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pLvdi = (LV_DISPINFO*)pNMHDR;

	NotationLite* lite = _liteList[pLvdi->item.iItem];

	if ((pLvdi->item.pszText != NULL)&&(lite))
	{
		CString str(pLvdi->item.pszText);
		if (!str.IsEmpty())
		{
			lite->setName(str);
			*pResult = TRUE; //to accept the new label
			return;
		}
	}

	*pResult = FALSE;
}

void NotationExplorerList::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pLvdi = (LV_DISPINFO*)pNMHDR;

	if (pLvdi->item.iItem < getChildFoldersCount())
	{
		*pResult = TRUE; //stop the edit
		return;
	}

	NotationLite* lite = _liteList[pLvdi->item.iItem - getChildFoldersCount()];

	if (lite->getFolderID() < ExplorerFolder::_pivotPoint)
	{
		*pResult = TRUE; //stop the edit
		return;
	}

	*pResult = FALSE;
}

void NotationExplorerList::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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
	
	CPoint hitPoint = point;
	ScreenToClient(&hitPoint);

	_lastContextItem = HitTest(hitPoint);

	//if we are a folder, then make it look like we are off the list
	if (_lastContextItem <  _childFolders.GetSize())
		 _lastContextItem = -1;
		
	unsigned long folder = _parent->_tree.getCurFolderID();

	CMenu menu;
	if (folder > ExplorerFolder::_pivotPoint) //user
	{
		if (_lastContextItem == -1)
			VERIFY(menu.LoadMenu (IDR_EXPLORER_LIST_OFF));
		else
			VERIFY(menu.LoadMenu (IDR_EXPLORER_LIST));
	}
	else //CC stuff
	{
		if (_lastContextItem == -1)
			return;
		else
#ifdef _DEBUG
			VERIFY(menu.LoadMenu (IDR_EXPLORER_LIST)); //this is to allow the alteration for me
#else
			VERIFY(menu.LoadMenu (IDR_EXPLORER_LIST_RO));
#endif
	}												  

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
}



void NotationExplorerList::OnExplorerListEdit() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{

		//get the item
		NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

		if (!lite) return;
							  
		Notation notation;
		notation = *lite;

		if ((mainFrame()->verifyKey() == FALSE)&&(notation.getNumber() > 6))
		{
			AfxMessageBox("Unregistered version limited to Minor (6): Please Register");
			return;
		}

		NotationDlg dlg(this, notation, FALSE, NULL, true);

		if (dlg.DoModal() != IDCANCEL)
		{
			*lite = notation;
			showMethods();
		}
	}
#ifdef _DEBUG
	if (AfxMessageBox("Would you like to save your changes to the CCLib.dat file?", MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		methodManager()->saveExplorerData(false, false);
	}
#endif
}

void NotationExplorerList::OnMeNewMethod() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{
		long folderID = _parent->_tree.getCurFolderID();

		ASSERT(folderID != 0);
		if (folderID == 0) 
			return;

   		Notation * not = new Notation();
		not->create(false, "",8,true,"");
		
		NotationDlg dlg(this, *not, FALSE, NULL, false);

		if (dlg.DoModal() != IDCANCEL)
		{
 			//convert to notation lite
			NotationLite* lite = new NotationLite();
			*lite = *not;
			lite->setFolderID(folderID);
			delete not;
			methodManager()->addMethod(lite);
			showMethods();
		} 	
		else
		{
			delete not;
			not = NULL;
		}		
	}
}

void NotationExplorerList::OnMeListDelete() 
{
	NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

	for (int i=0;i<methodManager()->getMethodCount();i++)
	{
		if (lite == methodManager()->getMethod(i))
		{
			delete methodManager()->getMethod(i);
			methodManager()->removeMethod(i);
		}
	}	

	showMethods();
}

void NotationExplorerList::OnMeNewFolder() 
{
	_parent->_tree.OnMeNewFolder();
}

void NotationExplorerList::OnMeRenameMethod() 
{
	EditLabel(_lastContextItem);
}


bool NotationExplorerList::SortList(int nCol, bool bAscending)
{		
	bool val = NotationListCtrl::SortList(nCol, bAscending);

	CWaitCursor wait;
	
	app()->WriteProfileInt("Explorer", "Last Explorer Sort Direction", bAscending);
	app()->WriteProfileInt("Explorer", "Last Explorer Sort Column", nCol);


	Invalidate(); 

	return val;
}

void NotationExplorerList::OnMeListView() 
{
	//get the item
	NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

	if (!lite) return;

	Notation notation;
	notation = *lite;

	if ((mainFrame()->verifyKey() == FALSE)&&(notation.getNumber() > 6))
	{
		AfxMessageBox("Unregistered version limited to Minor (6): Please Register");
		return;
	}

	NotationDlg dlg(this, notation, FALSE, NULL, true, true);//edit read only

	dlg.DoModal();
}

CString NotationExplorerList::getEmptyListText()
{
	if (_parent->_filterState)
	{
		return "No " + GlobalFunctions::numberName(_parent->_filterNumber) + " methods in this folder. \nSwitch off filter (top left) to see all methods";
	}
	else
	{
		return "No methods in this folder";
	}
}

 


void NotationExplorerList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_parent->OnLButtonUp(nFlags, point);
	
	NotationListCtrl::OnLButtonUp(nFlags, point);
}

void NotationExplorerList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	_lastContextItem = HitTest(point); 

	NotationListCtrl::OnLButtonDown(nFlags, point);
}

CString NotationExplorerList::getFolderName(unsigned long item)
{
	return _parent->_tree.getItemName(item);
}

int NotationExplorerList::getChildFoldersCount()
{
   return _childFolders.GetSize();
}

int NotationExplorerList::getSelectedCount()
{
	int count = 0;
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
//		TRACE0("No items were selected!\n");
	}
	else
	{
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			
			if (nItem >= _childFolders.GetSize())
				count++;
		}
	}

	
	return count;
}

void NotationExplorerList::OnEditCut() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{
		NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

		if (lite) 
		{
			NotationLite::CopyToClipboard(this, lite);

			//now remove the method
			OnMeListDelete();
		}
	}
}

void NotationExplorerList::OnEditCopy() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{
		NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

		if (lite) 
			NotationLite::CopyToClipboard(this, lite);
	}
}

void NotationExplorerList::OnEditPaste() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{
		if (NotationLite::IsClipboardFormatAvailable())
		{
			NotationLite* lite = NotationLite::PasteFromClipboard(this);
			
			long folderID = _parent->_tree.getCurFolderID();

			ASSERT(folderID != 0);
			if (folderID == 0) 
				return;

			lite->setFolderID(folderID);
			methodManager()->addMethod(lite);
			showMethods();
		}
	}
}

void NotationExplorerList::OnUpdateEditCut(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_lastContextItem >=0 && getSelectedCount() == 1
#ifndef _DEBUG
		&& _parent->_tree.getCurFolderID() > ExplorerFolder::_pivotPoint
#endif
		);
}

void NotationExplorerList::OnUpdateEditCopy(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(_lastContextItem >=0 && getSelectedCount() == 1);
}

void NotationExplorerList::OnUpdateEditPaste(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(NotationLite::IsClipboardFormatAvailable() 
#ifndef _DEBUG
		&& _parent->_tree.getCurFolderID() > ExplorerFolder::_pivotPoint
#endif
		);
}



