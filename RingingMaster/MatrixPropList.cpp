#include "StdAfx.h"
#include "RingingMaster.h"
#include "MatrixPropList.h"
#include "ButtonProp.h"
#include "UserMessages.h"
#include "UserManager.h"
#include "MatrixPropListExpanded.h"
#include "MatrixFilterSort.h"
#include "MatrixManager.h"
#include "MatrixDoc.h"

MatrixPropList::MatrixPropList()
{
	matrixManager()->addEventListener(this);
	userManager()->addEventListener(this);

}

MatrixPropList::~MatrixPropList(void)
{
	matrixManager()->removeEventListener(this);
	userManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(MatrixPropList, PropListBase)
	//{{AFX_MSG_MAP(MatrixPropList)
	ON_MESSAGE(BCG_BTN_PRESS, OnBCGBtnPress)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_MATRIX_DELETEUSERFILTER, OnDeleteUserFilter)
	ON_UPDATE_COMMAND_UI(ID_MATRIX_DELETEUSERFILTER, OnUpdateDeleteUserFilter)
	ON_COMMAND(ID_MATRIX_ADDUSERFILTER, OnAddUserFilter)
	ON_UPDATE_COMMAND_UI(ID_MATRIX_ADDUSERFILTER, OnUpdateAddUserFilter)
	ON_COMMAND(ID_MATRIX_APPLYUSERFILTER, OnApplyUserFilter)
	ON_UPDATE_COMMAND_UI(ID_MATRIX_APPLYUSERFILTER, OnUpdateApplyUserFilter)
	ON_COMMAND(ID_USER_OPTIONS, OnUserOptions)
	ON_UPDATE_COMMAND_UI(ID_USER_OPTIONS, OnUpdateUserOptions)
END_MESSAGE_MAP()

void MatrixPropList::initPropList()
{
	__super::initPropList();
	
	//-----------------------------------------
	//general
	//-----------------------------------------
	CBCGPProp* generalRoot = new CBCGPProp (_T("General"), ptm_userRoot);
	AddProperty (generalRoot);
	generalRoot->SetDescription("General settings. Press button above to edit users.");
	generalRoot->AddSubItem(new CBCGPProp (_T("Current User"),	(_variant_t) "",	_T("The current user."), ptm_userCurrent));
	generalRoot->Expand(FALSE);

	//-----------------------------------------
	//Filters 
	//-----------------------------------------
	CBCGPProp* filterRoot = new CBCGPProp (_T("User Filters"), ptm_filterRoot);
	AddProperty (filterRoot);
	filterRoot->SetDescription("Manage the filters.");
	filterRoot->Expand(FALSE);
	
	userManager_notifyUserListChange();
	matrixManager_updateFilterList();
}

void MatrixPropList::userManager_notifyUserListChange()
{
	CBCGPProp* currentUser = FindItemByData(ptm_userCurrent);
	if (currentUser) 	{
		currentUser->RemoveAllOptions();
		CStringArray strings;
		userManager()->getUserList(strings);

		for (int i=0;i<strings.GetSize();i++)
		{
			currentUser->AddOption(strings.GetAt(i));
		}
		currentUser->AllowEdit(FALSE);
		currentUser->SetValue((_variant_t)userManager()->getActiveUserName());	
	}
}

void MatrixPropList::matrixManager_updateFilterList()
{     
 	//first check if we have the correct number of items.
	CBCGPProp* filterRoot = FindItemByData(ptm_filterRoot);

	ASSERT(filterRoot);

	if (filterRoot)
	{
		//get the filters

		MatrixFilterSorts& filters = matrixManager()->getFilters(met_tower); //todo get from child frame 

		//do we have too many?
		while ( filters.GetSize() < filterRoot->GetSubItemsCount())
		{
			CBCGPProp* itemForRemoval = filterRoot->GetSubItem(0);
			filterRoot->RemoveSubItem(itemForRemoval);
		}

		//do we not have enough?
		while ( filters.GetSize() > filterRoot->GetSubItemsCount())
		{
			filterRoot->AddSubItem (new ButtonProp( "<dummy>", (_variant_t) "", "<dummy>", ptm_filterItem, IDB_FILTER_EDIT, FALSE));
		}

		//now update the items
		for (int i=0;i<	filterRoot->GetSubItemsCount();i++)
		{											
			
			CBCGPProp* item = filterRoot->GetSubItem(i);
			ASSERT(item);

			if (item)
			{
				MatrixFilterSort* filter = filters.GetAt(i);

				//name
				item->SetName(filter->getName());
				item->AllowEdit(FALSE);
				item->SetDescription(filter->getDescription());
				item->SetData(ptm_filterItem + i);
				_filterMap.SetAt(ptm_filterItem + i, filter->getUniqueID()); 
			}   
		}  

		//rebuild
		AdjustLayout();
	}   
}

void MatrixPropList::mainFrame_notifyDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc)
{
	__super::mainFrame_notifyDocViewChanged(oldViewType, newViewType, oldDoc, newDoc);

	matrixManager_updateFilterList();

	//load the expand status
	loadExpandStatus();
}

void MatrixPropList::saveExpandStatus()
{
	MatrixPropListExpanded& matrixPropListExpanded = matrixManager()->getMatrixPropListExpanded();

	if (matrixPropListExpanded._loaded == TRUE)
	{
	
		CBCGPProp* root = FindItemByData(ptm_userRoot);
		if (root)
			matrixPropListExpanded._users = root->IsExpanded();

		root = FindItemByData(ptm_filterRoot);
		if (root)
			matrixPropListExpanded._filters = root->IsExpanded();

		matrixPropListExpanded._scrollPos = m_nVertScrollOffset;
		matrixPropListExpanded._columnPos = m_nLeftColumnWidth;

		CBCGPProp* focusItem = GetCurSel();
		if (focusItem)
			matrixPropListExpanded._focusType = static_cast<PropertyTypeMatrix>(focusItem->GetData());
	}
}

void MatrixPropList::loadExpandStatus()
{
	MatrixPropListExpanded& matrixPropListExpanded = matrixManager()->getMatrixPropListExpanded();
		
	CBCGPProp* root = FindItemByData(ptm_userRoot);
	if (root)
		root->Expand(matrixPropListExpanded._users);

	root = FindItemByData(ptm_filterRoot);
	if (root)
		root->Expand(matrixPropListExpanded._filters );


	__super::OnVScroll(SB_THUMBTRACK, matrixPropListExpanded._scrollPos, GetScrollBarCtrl(SB_VERT));

	CBCGPProp* focusItem = FindItemByData(matrixPropListExpanded._focusType);
	if (focusItem)
		SetCurSel(focusItem);

	matrixPropListExpanded._loaded = TRUE;

}

void MatrixPropList::OnPropertyChanged (CBCGPProp* pProp) const
{
	if (pProp)
	{
		if (pProp->GetData() >= ptm_filterItem && pProp->GetData() < (ptm_filterItem+ TypeGapSize))
		{
			//do nothing - handled in the button click			
		}
		else
		{    
			switch (pProp->GetData())
			{
			case ptm_userCurrent:
				userManager()->setActiveUserName(pProp->GetValue());
				break;

			default:
				ASSERT(FALSE);
				break;
			}
		}
	}
}

										 
LRESULT MatrixPropList::OnBCGBtnPress(WPARAM /*wParam*/, LPARAM lParam)
{
	CBCGPProp* pProp = reinterpret_cast<CBCGPProp*>(lParam);

	if ((pProp)&&
		(pProp->GetData() >= ptm_filterItem) && 
		(pProp->GetData() < (ptm_filterItem+ TypeGapSize)))
	{
		//try and get out of the map.
		int filterUniqueId = 0;
		if (_filterMap.Lookup(pProp->GetData(), filterUniqueId))
			matrixManager()->editFilter(mainFrame(), filterUniqueId); 
	}

	return 0;
}



void MatrixPropList::OnDeleteUserFilter()
{
	CBCGPProp* pProp = GetCurSel();

	if ((pProp)&&
		(pProp->GetData() >= ptm_filterItem) && 
		(pProp->GetData() < (ptm_filterItem+ TypeGapSize)))
	{
		//try and get out of the map.
		int filterUniqueId = 0;
		if (_filterMap.Lookup(pProp->GetData(), filterUniqueId))
		{
			matrixManager()->deleteFilter(filterUniqueId);
			matrixManager()->getMatrixPropListExpanded()._filters = TRUE;
		}
	}   		
}

void MatrixPropList::OnUpdateDeleteUserFilter(CCmdUI *pCmdUI)
{
	BOOL enable = FALSE;
	CBCGPProp* pProp = GetCurSel();

	if (pProp)
	{
        BOOL filterItem = (pProp->GetData() >= ptm_filterItem && pProp->GetData() < (ptm_filterItem+ TypeGapSize));

		enable = (filterItem);
	}

	pCmdUI->Enable(enable);	  
}

void MatrixPropList::OnAddUserFilter()
{
	matrixManager()->addFilter(mainFrame(),met_tower); //todo get from curent grid
	matrixManager()->getMatrixPropListExpanded()._filters = TRUE;
}

void MatrixPropList::OnUpdateAddUserFilter(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);	  
}

void MatrixPropList::OnApplyUserFilter()
{
	CBCGPProp* pProp = GetCurSel();

	if ((pProp)&&
		(pProp->GetData() >= ptm_filterItem) && 
		(pProp->GetData() < (ptm_filterItem+ TypeGapSize)))
	{
		//try and get out of the map.
		int filterUniqueId = 0;
		if (_filterMap.Lookup(pProp->GetData(), filterUniqueId))
		{
			matrixManager()->applyFilterToCurrentGridView(filterUniqueId); 
			matrixManager()->getMatrixPropListExpanded()._filters = TRUE;
		}
	}
}

void MatrixPropList::OnUpdateApplyUserFilter(CCmdUI *pCmdUI)
{
	BOOL enable = FALSE;
	CBCGPProp* pProp = GetCurSel();

	if (pProp)
	{
        BOOL filterItem = (pProp->GetData() >= ptm_filterItem && pProp->GetData() < (ptm_filterItem+ TypeGapSize));

		enable = (filterItem);
	}

	pCmdUI->Enable(enable);	  
}

void MatrixPropList::OnUserOptions()
{
	CBCGPProp* root = FindItemByData(ptm_userRoot);
	if (root)
		root->Expand();

	matrixManager()->getMatrixPropListExpanded()._users = TRUE;
	::PostMessage(mainFrame()->m_hWnd,WM_COMMAND, ID_USER_OPTIONS,0);;	
}

void MatrixPropList::OnUpdateUserOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

