// MatrixManager.cpp: implementation of the MatrixManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "UserMessages.h"

#include "MatrixManager.h"
#include "MatrixGridCtrl.h"

#include "matrixmanager.h"
#include "MatrixFilterDlg.h"

#include "MatrixView.h"
#include "MatrixDoc.h"
#include "MatrixEntityTower.h" //todo have factory for these
#include "MatrixEntityGrab.h"//todo have factory for these
#include "MatrixItembase.h"//todo have factory for these
#include "MatrixManagerColumnFactory.h"

#include "RecordManager.h"
#include "OptionsManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//todo need to serialise stock filters

#define MAX_USER_FILTERS_SHOWN 10
#define MAX_STOCK_FILTERS_SHOWN 50


MatrixManager::MatrixManager() 
{
}

MatrixManager::~MatrixManager()
{
	POSITION pos = _filters.GetStartPosition();
	MatrixFilterHash  nKey;
	MatrixFilterSorts* nValue;
	while (pos != NULL)
	{
		_filters.GetNextAssoc( pos, nKey, nValue );

		if (nValue)
		{
			for (int i=0;i<nValue->GetSize();i++)
				delete nValue->GetAt(i);

			nValue->RemoveAll();
			
			delete nValue;
			nValue = NULL;
		}
		
		VERIFY(_filters.RemoveKey( nKey ));
	}   
	
	MatrixManagerColumnFactory::Destroy();
}


BEGIN_MESSAGE_MAP(MatrixManager, CCmdTarget)
	ON_COMMAND_RANGE(ID_USERFILTERDUMMY, (ID_USERFILTERDUMMY+MAX_USER_FILTERS_SHOWN), OnUserFilter)
	ON_COMMAND_RANGE(ID_STOCKFILTERDUMMY, (ID_STOCKFILTERDUMMY+MAX_STOCK_FILTERS_SHOWN), OnStockFilter) 
	ON_CBN_SELCHANGE(ID_FILTER_COMBO, OnSelchangeFilterComboChange)
END_MESSAGE_MAP()


void MatrixManager::save()
{	 

}

void MatrixManager::load()
{
	setupStockFilters();	
}

void MatrixManager::openUp()
{
	addEventListener(this);
	optionsManager()->addEventListener(this);
	recordManager()->addEventListener(this);
}

void MatrixManager::closeDown()
{
	removeEventListener(this);
	optionsManager()->removeEventListener(this);
	recordManager()->removeEventListener(this);
}


void MatrixManager::addEventListener(MatrixManagerEventListener *matrixManagerEventListener)
{
	ASSERT(matrixManagerEventListener != NULL);

	_listenerList.Add(matrixManagerEventListener);

}

void MatrixManager::removeEventListener(MatrixManagerEventListener *matrixManagerEventListener)
{
	ASSERT(matrixManagerEventListener!= NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == matrixManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

#ifdef _DEBUG
	if (hasRemoved != 1)
		TRACE("[MatrixManager::removeEventListener] window already removed.");
#endif
}

void MatrixManager::fireUpdateFilterList(  )
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->matrixManager_updateFilterList(); 
	}  
}

void MatrixManager::fireEntityAdded(MatrixEntityToken token, long id)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->matrixManager_entityAdded(token, id); 
	}  
}

void MatrixManager::fireEntityRemoved(MatrixEntityToken token, long id)
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->matrixManager_entityRemoved(token, id); 
	}  
}

void MatrixManager::fireRefresh()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->matrixManager_refresh(); 
	}  
}

//creates the entity without initialising it
MatrixEntityBase* MatrixManager::createEntity(MatrixEntityToken token)
{
	switch (token) 
	{
	case met_tower:
		return new MatrixEntityTower;
	case met_grab:
		return new MatrixEntityGrab;
	default:
		ASSERT(FALSE);   //add your entity constructor here.
		return NULL;
	}

}

MatrixGridCtrl* MatrixManager::createEntityGridCtrl(MatrixEntityToken token, MatrixFilterSectionToken matrixFilterSectionToken)
{
	//create the data structure for the entity
	MatrixEntityBase* matrixEntityBase = createEntity(token);

	//create the entity window
	if (matrixEntityBase != NULL)
	{	
		//initiate the entity
		matrixEntityBase->init();

		MatrixGridCtrl* matrixGrid = new MatrixGridCtrl(matrixEntityBase);

		return matrixGrid;
	}

	ASSERT(FALSE);
	return NULL;
}

//todo only call this method when no filters to be deserialised. or deserialise into these ones.
void MatrixManager::setupStockFilters()
{
	for (int i=0; i<= MatrixEntityTokenMax; i++)
	{
		MatrixEntityToken entityToken = (MatrixEntityToken) i;

		MatrixEntityBase* entity = createEntity(entityToken);
		entity->init();

		ASSERT(entity);

		if (entity)
		{
			MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, mfst_stock);

			for (int j=0;j<entity->getColumnCount();j++)
			{
				MatrixItemBase* item = entity->getColumn(j);
				if (item)
				{
					MatrixFilterSort* matrixFilterSort = new MatrixFilterSort(entityToken, mfst_stock);
					matrixFilterSort->setName(item->getName());
					matrixFilterSort->setFilter(*item);
					matrixFilterSorts.Add(matrixFilterSort);
				}
			}

			sortFilters(entityToken, mfst_stock);
		}
	}

	fireUpdateFilterList();
}

long MatrixManager::addFilter(CWnd* pParent, MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken)
{
	//create a new filter object
	MatrixFilterSort* matrixFilterSort = new MatrixFilterSort(entityToken, matrixFilterSectionToken);

	//launch the matrix dialog with the 
	MatrixFilterDlg dlg(*matrixFilterSort, false, pParent);
	if (dlg.DoModal() == IDOK)
	{
		//if the user pressed OK, then add the filter
		MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, matrixFilterSectionToken);

		matrixFilterSorts.Add(matrixFilterSort);

		sortFilters(entityToken, matrixFilterSectionToken);
	
		fireUpdateFilterList();

		if (AfxMessageBox("Would you like to apply filter '" + matrixFilterSort->getName() + "'", 
			MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			applyFilterToCurrentGridView(matrixFilterSort);	
		}

		return	matrixFilterSort->getUniqueID();
	}
	else
	{
		delete matrixFilterSort;
	}

	return -1;
}		 

bool MatrixManager::isFilterNameUnique(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken, CString name)
{
	//get the filter
	MatrixFilterSorts& filterSorts = getFilters(entityToken, matrixFilterSectionToken);

	for (int i=0;i<filterSorts.GetSize();i++)
	{
		if (name.CompareNoCase(filterSorts.GetAt(i)->getName()) == 0)
			return false;
	}

	return true;
}

void MatrixManager::editFilter(CWnd* pParent, long uniqueId)
{
	MatrixFilterSort* matrixFilterSort = getFilter(uniqueId);


	MatrixFilterDlg dlg(*matrixFilterSort, true, pParent);
	dlg.DoModal();
	
	sortFilters(matrixFilterSort->getMatrixEntityToken(), matrixFilterSort->getMatrixFilterSectionToken());

	fireUpdateFilterList();	
	
	if (AfxMessageBox("Would you like to apply filter '" + matrixFilterSort->getName() + "'", 
		MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		applyFilterToCurrentGridView(matrixFilterSort);	
	}
}

void MatrixManager::deleteFilter(int uniqueId)
{		
	MatrixFilterSort* matrixFilterSort = getFilter(uniqueId);

	if (matrixFilterSort)
	{

		MatrixFilterSorts& matrixFilterSorts = getFilters(matrixFilterSort->getMatrixEntityToken(), matrixFilterSort->getMatrixFilterSectionToken());
		for (int i=0;i<matrixFilterSorts.GetSize();i++)
		{
			if (matrixFilterSorts.GetAt(i)->getUniqueID() == uniqueId)
			{
				matrixFilterSorts.RemoveAt(i--);
			}
		} 

		//update
		fireUpdateFilterList();

		//delete the removed filter
		delete matrixFilterSort;
		matrixFilterSort = NULL;
	}
}

void MatrixManager::applyFilterToCurrentGridView(long uniqueId)
{
	//get the filter object
	MatrixFilterSort* filterSort = getFilter(uniqueId);
	
	if (filterSort)
		applyFilterToCurrentGridView(filterSort);
}

void MatrixManager::applyFilterToCurrentGridView(MatrixFilterSort* matrixFilterSort)
{
	//find the current document
	MatrixGridCtrl* matrixGridCtrl = getMatrixGridCtrl();
	if (matrixGridCtrl)
		matrixGridCtrl->applyFilterSort(matrixFilterSort);
}

MatrixGridCtrl* MatrixManager::getMatrixGridCtrl()
{	
	MatrixDoc* doc = mainFrame()->getActiveMatrixDoc();

	if (doc)
		return doc->getMatrixGridCtrl();
	
	return NULL;
}

/*

void MatrixManager::Serialize(CArchive& ar,  MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken)
{
	//write to the archive 
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << entityToken;
		ar << matrixFilterSectionToken;

		//serialise the filter list filter list.
		MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, matrixFilterSectionToken);

		//sort out what needs serialising
		MatrixFilterSorts filterSortsForSerialisation;
		for (int i=0;i<matrixFilterSorts.GetSize();i++)
		{
			MatrixFilterSort* matrixFilterSort = matrixFilterSorts.GetAt(i);

				if (matrixFilterSort->isEditable() &&
					!matrixFilterSort->isUsingSpecificIds())
					filterSortsForSerialisation.Add(matrixFilterSort);
		}

		//now serialise
		ar << filterSortsForSerialisation.GetSize();
		for (int i=0;i<filterSortsForSerialisation.GetSize();i++)
		{
			MatrixFilterSort* matrixFilterSort = filterSortsForSerialisation.GetAt(i);
				matrixFilterSort->Serialize(ar);
		}  
	}
	else
	{
		//deserialize
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		int temp;
		ar >> temp;
		ASSERT(entityToken == static_cast<MatrixEntityToken>(temp));
		if (entityToken != static_cast<MatrixEntityToken>(temp))
			AfxThrowArchiveException(CArchiveException::badSchema);	

		ar >> temp;
		ASSERT(matrixFilterSectionToken == static_cast<MatrixFilterSectionToken>(temp));
		if (matrixFilterSectionToken != static_cast<MatrixFilterSectionToken>(temp))
			AfxThrowArchiveException(CArchiveException::badSchema);	

		MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, matrixFilterSectionToken);
		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			MatrixFilterSort* matrixFilterSort = new MatrixFilterSort(entityToken, matrixFilterSectionToken);
			matrixFilterSort->Serialize(ar);

			matrixFilterSorts.Add(matrixFilterSort);
		}
		sortFilters(entityToken, matrixFilterSectionToken);
	}
}
*/
MatrixFilterSort* MatrixManager::getFilter(long filterID)
{
	//iterate through the map. 
	POSITION pos = _filters.GetStartPosition();
	MatrixFilterHash  nKey;
	MatrixFilterSorts* nValue;
	while (pos != NULL)
	{
		_filters.GetNextAssoc( pos, nKey, nValue );

		if (nValue)
		{
			for (int i=0;i<nValue->GetSize();i++)
			{
				if (nValue->GetAt(i)->getUniqueID() == filterID)
				{
					return nValue->GetAt(i);
				}
			}
		}
	}   

	return NULL;
}

MatrixFilterSorts& MatrixManager::getFilters(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken )
{
	MatrixFilterHash hash(entityToken, matrixFilterSectionToken);
	MatrixFilterSorts* matrixFilterSorts = 0;

	//do we already have one?
	if (!_filters.Lookup(hash, matrixFilterSorts))
	{
		//if we do not, then make one.
		matrixFilterSorts = new MatrixFilterSorts;
		_filters.SetAt(hash, matrixFilterSorts);
		
		fireUpdateFilterList();	
	}		
  
	return *matrixFilterSorts;
}

void MatrixManager::sortFilters(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken )
{              
	MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, matrixFilterSectionToken);

	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<matrixFilterSorts.GetUpperBound();i++)
		{
			if (matrixFilterSorts[i]->getName() > matrixFilterSorts[i+1]->getName())
			{
				changed = true;
				MatrixFilterSort* temp = matrixFilterSorts.GetAt(i);
				matrixFilterSorts.SetAt(i,matrixFilterSorts.GetAt(i+1));
				matrixFilterSorts.SetAt(i+1, temp);
			}
		}
	}  
}


MATRIX_ENTITY_TRANSLATOR* MatrixManager::GetTranslatorFunction(TranslatorFunctionItem translatorFunctionItem)
{
	switch (translatorFunctionItem)
	{
	default: 
		ASSERT(FALSE);
	case tfi_doNothing:
		return MatrixEntityTranslatorDoNothing;

	}
}

SaveableObject* MatrixManager::MatrixEntityTranslatorDoNothing(SaveableObject* originalEntity)
{
	return originalEntity;
}


MatrixPropListExpanded& MatrixManager::getMatrixPropListExpanded()
{
	return _matrixPropListExpanded;
}
		
void MatrixManager::getStatusBarText(CString& pane1, CString& pane2, CString& pane3)
{
	MatrixGridCtrl* matrixGridCtrl = getMatrixGridCtrl();
	if (matrixGridCtrl)
		matrixGridCtrl->getEntitity()->getStatusBarText(pane1, pane2, pane3);
	else
	{
		pane1.Empty();
		pane2.Empty();
		pane3.Empty();
	}
}

void MatrixManager::matrixManager_updateFilterList()
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	MatrixGridCtrl* grid = getMatrixGridCtrl();

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_FILTER_COMBO, listButtons) > 0 &&
		grid)
	{
		MatrixEntityToken matrixEntityToken;
		if (grid->getEntitityToken(matrixEntityToken))
		{	 
			for (POSITION posCombo = listButtons.GetHeadPosition (); 
				pSrcCombo == NULL && posCombo != NULL;)
			{
				CBCGPToolbarComboBoxButton* pCombo = 
					DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

				if (pCombo != NULL)
				{
					pCombo->RemoveAllItems();
					if (CBCGPToolBar::IsCustomizeMode())
					{
						pCombo->AddItem("<select filter>");
						pCombo->SelectItem(0);
					}
					else
					{
						CombinedFilterItems combinedFilterItems;
						getCombinedFilterList(matrixEntityToken, combinedFilterItems);
						pCombo->AddItem("<select filter>");

						for (int i=0;i<combinedFilterItems.GetSize();i++)
						{
							pCombo->AddItem(combinedFilterItems.GetAt(i)->_name, 
								combinedFilterItems.GetAt(i)->_filterType == mfst_user?1:0);
							delete combinedFilterItems.GetAt(i);
						}
						pCombo->SelectItem(0);
					}
				}
			}
		}
	}  
}

//caller is responsable for deleting returned items.
void MatrixManager::getCombinedFilterList(MatrixEntityToken matrixEntityToken, CombinedFilterItems& combinedFilterItems)
{
	//user filters
	MatrixFilterSorts& userFilters = getFilters(met_tower,mfst_user);
	for (int i=0;i<userFilters.GetSize();i++)
	{
		CombinedFilterItem* item = new CombinedFilterItem;

		item->_name = userFilters.GetAt(i)->getName();
		item->_filterType = mfst_user;
		item->_filterId = userFilters.GetAt(i)->getUniqueID();
		combinedFilterItems.Add(item);
	}

	//user filters
	MatrixFilterSorts& stockFilters = getFilters(met_tower,mfst_stock);
	for (int i=0;i<stockFilters.GetSize();i++)
	{
		CombinedFilterItem* item = new CombinedFilterItem;

		item->_name = stockFilters.GetAt(i)->getName();
		item->_filterType = mfst_stock;
		item->_filterId = stockFilters.GetAt(i)->getUniqueID();
		combinedFilterItems.Add(item);
	}

	//sort
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<combinedFilterItems.GetUpperBound();i++)
		{
			if (combinedFilterItems[i]->_name > combinedFilterItems[i+1]->_name)
			{
				changed = true;
				CombinedFilterItem* temp    = combinedFilterItems.GetAt(i);
				combinedFilterItems.SetAt(i,combinedFilterItems.GetAt(i+1));
				combinedFilterItems.SetAt(i+1, temp);
			}
		}
	}			
}


int MatrixManager::getCombinedFilterCount(ViewType viewType)//todo remove 
{			 
	MatrixFilterSorts& filterSorts = getFilters(met_tower,mfst_user);

	return	filterSorts.GetSize() + 		
			1;//the 1 is so we can have a <select filter>
}

void MatrixManager::applyCombinedFilter(int index)
{
	if (index == 0)
		return;

	//for the select filter line
	index--;
	
	MatrixGridCtrl* grid = getMatrixGridCtrl();

	if (grid == 0)
		return;

	MatrixEntityToken matrixEntityToken;
	if (grid->getEntitityToken(matrixEntityToken))
	{
		CombinedFilterItems combinedFilterItems;

		getCombinedFilterList(matrixEntityToken, combinedFilterItems);

		if (index < combinedFilterItems.GetSize())
		{
			bool okToApplyFilter = true;

			CombinedFilterItem* item = combinedFilterItems.GetAt(index);
			MatrixFilterSort* filter = getFilter(item->_filterId);
			if (item->_filterType == mfst_stock)
			{
				ASSERT(filter->getFilterCount() == 1);
				if (filter->getFilterCount() == 1)
				{
					if (!filter->getFilter(0)->editFilterCondition(mainFrame()))
						okToApplyFilter = false;
				}
			}

			if (okToApplyFilter)
				applyFilterToCurrentGridView(filter);
		}

		//clean up
		for (int i=0;i<combinedFilterItems.GetSize();i++)
			delete combinedFilterItems.GetAt(i);		 
	}

}

void MatrixManager::OnSelchangeFilterComboChange()
{
	CBCGPToolbarComboBoxButton* pSrcCombo = NULL;

	CString filterName("");

	CObList listButtons;
	if (CBCGPToolBar::GetCommandButtons (ID_FILTER_COMBO, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
			pSrcCombo == NULL && posCombo != NULL;)
		{
			CBCGPToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CBCGPToolbarComboBoxButton, listButtons.GetNext (posCombo));

			if (pCombo != NULL)
			{
				mainFrame()->PostMessage(WM_APPLY_COMBINED_FILTER, pCombo->GetCurSel(),0);
			}
		}
	}
}

void MatrixManager::setupFilterMenu(CMenu &menu, MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken, int todo, UINT menuBaseId )
{

	int maxNoFilters = 0;
	switch (matrixFilterSectionToken)
	{
	case mfst_user:
		maxNoFilters = MAX_USER_FILTERS_SHOWN;
		break;
	case mfst_stock:
		maxNoFilters = MAX_STOCK_FILTERS_SHOWN;
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	MatrixFilterSorts& matrixFilterSorts = getFilters(entityToken, matrixFilterSectionToken);
	menu.CreateMenu();
	int i;

	for (i=0;i<min(matrixFilterSorts.GetSize(), maxNoFilters);i++)
		menu.AppendMenu(MF_STRING, 
			(UINT)menuBaseId + i, 
			"&" + matrixFilterSorts.GetAt(i)->getName());
									   
	if (matrixFilterSorts.GetSize() > maxNoFilters)
	{
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_USERFILTERMORE + entityToken, "More Filters..."); //todo test
	}			 
}


void MatrixManager::recordManager_notifyHomeTowerChange(RecordTower* /*homeTower*/)
{
	fireRefresh();
}

void MatrixManager::optionsManager_optionsDlgClosed()
{
	fireRefresh();
}
					 		
void MatrixManager::OnUserFilter(UINT nID) 
{
	onFilter((nID - ID_USERFILTERDUMMY), mfst_user);
}

void MatrixManager::OnStockFilter(UINT nID) 
{
	onFilter((nID - ID_STOCKFILTERDUMMY), mfst_stock);
}

void MatrixManager::onFilter(UINT index, MatrixFilterSectionToken matrixFilterSectionToken) 
{

	MatrixGridCtrl* grid = getMatrixGridCtrl();
	if (grid)
	{
		MatrixEntityToken matrixEntityToken;
		if (grid->getEntitityToken(matrixEntityToken))
		{
			MatrixFilterSorts& matrixFilterSorts = getFilters(matrixEntityToken, matrixFilterSectionToken);

			if (index < matrixFilterSorts.GetSize())
			{
				bool okToApplyFilter = true;

				MatrixFilterSort* filter = matrixFilterSorts.GetAt(index);

				if (matrixFilterSectionToken == mfst_stock)
				{
					ASSERT(filter->getFilterCount() == 1);
					if (filter->getFilterCount() == 1)
					{
						if (!filter->getFilter(0)->editFilterCondition(mainFrame()))
							okToApplyFilter = false;
					}
				}

				if (okToApplyFilter)
					applyFilterToCurrentGridView(filter);
			}
		}
	}
}


/* //todo this is the original dfault list of fields for towers and grabs.

void RecordManager::setupSelectedTowerFieldsLists()
{
	//this is the default set up that is usually overwritten by the serialisation process.
	_towerSelectedFields.RemoveAll();

	_towerSelectedFields.Add(af_tower_name);		
	_towerSelectedFields.Add(af_tower_county);		
	_towerSelectedFields.Add(af_tower_dedication);	
	_towerSelectedFields.Add(af_tower_number);		
	_towerSelectedFields.Add(af_tower_weight);		
	_towerSelectedFields.Add(af_tower_note);		
	_towerSelectedFields.Add(af_tower_night);	
}

void RecordManager::setupSelectedGrabFieldsLists()
{
	//this is the default set up that is usually overwritten by the serialisation process.
	_grabSelectedFields.RemoveAll();

	_grabSelectedFields.Add(af_grab_number);	
	_grabSelectedFields.Add(af_grab_date);	
	_grabSelectedFields.Add(af_tower_name);		
	_grabSelectedFields.Add(af_tower_county);		
	_grabSelectedFields.Add(af_tower_dedication);	
	_grabSelectedFields.Add(af_grab_bellsGo);	
	_grabSelectedFields.Add(af_grab_bellsSound);	
	_grabSelectedFields.Add(af_grab_comments);	
}   */