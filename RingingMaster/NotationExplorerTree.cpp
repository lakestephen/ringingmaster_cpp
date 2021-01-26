// NotationExplorerTree.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationExplorerTree.h"
#include "NotationExplorerDlg.h"
#include "ExplorerFolder.h"
#include "MethodManager.h"
#include "Notation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerTree

NotationExplorerTree::NotationExplorerTree()
{
	_imageList.Create(IDB_EXPLORER, 16, 1, RGB(255,0,255));	
}

NotationExplorerTree::~NotationExplorerTree()
{
}

void NotationExplorerTree::setParent(NotationExplorerDlg *parent)
{
	_parent = parent;
}


BEGIN_MESSAGE_MAP(NotationExplorerTree, CTreeCtrl)
	//{{AFX_MSG_MAP(NotationExplorerTree)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ME_NEW_FOLDER, OnMeNewFolder)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_ME_DELETE_FOLDER, OnMeDeleteFolder)
	ON_COMMAND(ID_ME_NEW_METHOD, OnMeNewMethod)
	ON_COMMAND(ID_ME_RENAME_FOLDER, OnMeRenameFolder)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerTree message handlers

int NotationExplorerTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	return 0;
}

void NotationExplorerTree::loadDirectory()
{
	const int levels = 100;
	HTREEITEM parent[levels];  //should be enough
	HTREEITEM hItem;

	for (int i=0;i<levels;i++)
		parent[i] = TVI_ROOT;

	ExplorerFolder* folder;

	//this will do the load is the initial load has not been done already, else nothing
	methodManager()->loadExplorerData();

	int size = methodManager()->getFolderCount();

	if (size >0)
	{
		for (int i=0;i<size;i++)
		{
			folder = methodManager()->getFolder(i);
			hItem = InsertItem(folder->getName(), 0,1, parent[folder->getLevel()-1]);
			if(folder->getLevel() > 2)
				SortChildren(parent[folder->getLevel()-1]);
			parent[folder->getLevel()] = hItem;
			SetItemData(hItem, folder->getID());
		}	 
	}
}



void NotationExplorerTree::saveDirectory()
{
	//clear out the folder;
	for (int i=0;i<methodManager()->getFolderCount();i++)
	{
		delete methodManager()->getFolder(i);
	}

	methodManager()->removeAllFolders();

	//save the new data
	HTREEITEM hItem = GetRootItem();

	while (hItem != NULL)
	{
		createFolderFromHTREEITEM(hItem);
		
		hItem = GetNextItem(hItem);
	}
}

unsigned long NotationExplorerTree::getCurFolderID()
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL)
		return 0;
	
	return GetItemData(hItem);
}

void NotationExplorerTree::getChildrenFolderID(UnsignedLongs& items, bool recurse)
{	 	
	items.RemoveAll();
	HTREEITEM hItem = GetSelectedItem();
	if (hItem != NULL)
	{
		int level = getLevel(hItem);
		int thisLevel = 100000; //silly high number

		while ((hItem != NULL)&&(thisLevel > level))
		{
			if ((recurse)||(thisLevel == level+1))
			items.Add(GetItemData(hItem));			
			hItem = GetNextItem(hItem);
			thisLevel = getLevel(hItem);
		}	
	}
}


HTREEITEM NotationExplorerTree::createNewFolder(HTREEITEM hItem, BOOL user, CString name)
{
	name = tidyFolderName(name);

	hItem = InsertItem(name, 0,1,hItem,hItem);
	
	if (hItem != NULL)
	{
		SetItemData(hItem,(user? ExplorerFolder::createUniqueUserID():ExplorerFolder::createUniqueCCID()));
		Select(hItem, TVGN_CARET);

		createFolderFromHTREEITEM(hItem);
	}
	return hItem;
}

ExplorerFolder* NotationExplorerTree::createFolderFromHTREEITEM(HTREEITEM hItem)
{
	ExplorerFolder* folder = new ExplorerFolder;
	methodManager()->addMethodFolder(folder);

	folder->setLevel(getLevel(hItem));
	folder->setName(GetItemText(hItem));
	folder->setID(GetItemData(hItem));
	
	return folder;
}

void NotationExplorerTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	_lastContextItem = pNMTreeView->itemNew.hItem;

	
	_parent->_list.showMethods();
	
	*pResult = 0;
}

void NotationExplorerTree::OnEndlabeledit(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
//	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	*pResult = TRUE;
}

void NotationExplorerTree::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	//return true to stop label edit

	HTREEITEM selectedItem = pTVDispInfo->item.hItem;

	if ((selectedItem == NULL)||
		(GetItemData(selectedItem) < ExplorerFolder::_pivotPoint)||
		(GetItemText(selectedItem) == "User Library"))
		*pResult = TRUE;
	else
		*pResult = FALSE; //allow the edit
}

void NotationExplorerTree::openFolder(unsigned long folderID)
{
	HTREEITEM hItem = GetRootItem();

	while (hItem != NULL)
	{
		if (folderID == GetItemData(hItem))
		{
			SelectItem(hItem);
			EnsureVisible(hItem);
			break;
		}
		hItem = GetNextItem(hItem);
	}
}

void NotationExplorerTree::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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

	if (_lastContextItem == NULL)
		return;

	if (GetItemData(_lastContextItem) < ExplorerFolder::_pivotPoint)
		return;
	
	CMenu menu;
	if (GetItemText(_lastContextItem) == "User Library")
	{
		VERIFY(menu.LoadMenu (IDR_EXPLORER_TREE_ROOT_USER));
	}
	else
	{
		VERIFY(menu.LoadMenu (IDR_EXPLORER_TREE));
	}	

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	
}

void NotationExplorerTree::OnMeNewFolder() 
{
	if (mainFrame()->verifyKey() == FALSE)
	{
		AfxMessageBox("This Action is not allowed in the unregistered version.\nPlease register");
	}
	else
	{	//must be a user request
		createNewFolder(_lastContextItem, true);
	}
}

void NotationExplorerTree::OnMeDeleteFolder() 
{
	CString msg;
	msg.Format("Are you sure that you want to delete %s, all its sub folders and methods?",
		GetItemText(_lastContextItem));
	if (AfxMessageBox(msg, MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		HTREEITEM hItem = _lastContextItem;
		int level = getLevel(_lastContextItem);
		int thisLevel = 100000; //silly high number

		while ((hItem != NULL)&&(thisLevel > level))
		{
			_parent->deleteMethodsInFolder(GetItemData(hItem));		
			
			hItem = GetNextItem(hItem);
			thisLevel = getLevel(hItem);
		}
		
		DeleteItem(_lastContextItem);
		_lastContextItem = TVI_ROOT;
	}	
}

		 
void NotationExplorerTree::createBlankCCDir()
{
	HTREEITEM hItem; 

	//create the root dir
	HTREEITEM rootItem = InsertItem("CC Library", 0,1);				
	SetItemData(rootItem, ExplorerFolder::createUniqueCCID());	

	for (int i=3;i<=12;i++)
	{
		hItem = InsertItem(GlobalFunctions::numberName(i), 0,1, rootItem);				
		SetItemData(hItem, ExplorerFolder::createUniqueCCID());	
	}	 	

	hItem = InsertItem("Higher Numbers", 0,1, rootItem);				
	SetItemData(hItem, ExplorerFolder::createUniqueCCID());	

}

void NotationExplorerTree::init()
{
	SetImageList(&_imageList, TVSIL_NORMAL);
}

 

void NotationExplorerTree::OnRclick(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	
	DWORD dwPos = GetMessagePos();

	CPoint point(((int)(short)LOWORD(dwPos)),((int)(short)HIWORD(dwPos)));

	OnContextMenu(this, point);
	
	*pResult = 0;
}

void NotationExplorerTree::OnMeNewMethod() 
{
	_parent->_list.OnMeNewMethod();	
}

void NotationExplorerTree::OnMeRenameFolder() 
{
	EditLabel(_lastContextItem);	
}


//this function should only be used when there is ONLY CC data in the system
unsigned long NotationExplorerTree::getSectionFolderID(MetSection *section)
{
	//find the number folder
	CString folderToFind;
	if (section->getNumber() <= 12)
		folderToFind = GlobalFunctions::numberName(section->getNumber());
	else
		folderToFind = "higher numbers";

	HTREEITEM hNumberItem = FindItem(folderToFind, FALSE, TRUE, TRUE, GetRootItem()); 
	
	if (hNumberItem == NULL)
	{
		ASSERT(FALSE); //why cant we find the root item??
		hNumberItem = GetRootItem();
	}

	//get the name of the wanted section
	CString strSection = section->getSectionType();
	strSection = tidyFolderName(strSection);
	strSection.MakeLower();
	
	//now index through all the children to find the section wanted (alliance, delight Etc)
	HTREEITEM sectionsIter = NULL;		
	
	if (ItemHasChildren(hNumberItem))
		sectionsIter = GetChildItem(hNumberItem);
	
	while (sectionsIter != NULL) 
	{
		CString itemText = GetItemText(sectionsIter);
		itemText.MakeLower();
		if (itemText == strSection)
		{
			break;
		}
		sectionsIter = GetNextSiblingItem(sectionsIter); 
	}

	if (sectionsIter == NULL)
	{
		//definitly a CC folder
		sectionsIter = createNewFolder(hNumberItem, false, strSection);
	}

	//now index through all the section children to find the folder wanted (treble to 12 place Etc)
	//get the name of the wanted section
	CString strFolder = section->_type;
	strFolder = tidyFolderName(strFolder);
	strFolder.MakeLower();

	if ((strFolder.IsEmpty())||(strFolder == " "))
		return GetItemData(sectionsIter);

	HTREEITEM folderIter = NULL;		
	
	if (ItemHasChildren(sectionsIter))
		folderIter = GetChildItem(sectionsIter);
	
	while (folderIter != NULL) 
	{
		CString itemText = GetItemText(folderIter);
		itemText.MakeLower();
		if (itemText == strFolder)
		{
			break;
		}
		folderIter = GetNextSiblingItem(folderIter); 
	}

	if (folderIter == NULL)
	{
		//definitly a CC folder
		folderIter = createNewFolder(sectionsIter, false, strFolder);
	}

	ASSERT (folderIter != NULL);
	return GetItemData(folderIter);
	
}

CString NotationExplorerTree::tidyFolderName(CString folderName)
{
	folderName.TrimLeft(' ');
	folderName.TrimRight(' ');
	return GlobalFunctions::titleString(folderName);
}


void NotationExplorerTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_parent->OnLButtonUp(nFlags, point);
	
	CTreeCtrl::OnLButtonUp(nFlags, point);
}


CString NotationExplorerTree::getItemName(unsigned long item)
{
	HTREEITEM hItem = GetRootItem();

	while (hItem != NULL)
	{
		if (GetItemData(hItem) == item)
			return GetItemText(hItem);
		
		hItem = GetNextItem(hItem);
	}
	
	return "";
}

void NotationExplorerTree::selectItem(unsigned long item)
{
	HTREEITEM hItem = GetRootItem();

	while (hItem != NULL)
	{
		if (GetItemData(hItem) == item)
			SelectItem(hItem);
		
		hItem = GetNextItem(hItem);
	}
	
}
