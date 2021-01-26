#pragma once


#include "MatrixConstants.h"
#include "ManagerBase.h"
#include "MatrixPropListExpanded.h"
#include "MatrixManagerEventListener.h"
#include "RecordManagerEventListener.h"
#include "MainFrameEventListener.h"
#include "OptionsManagerEventListener.h"
#include "CombinedFilterItem.h"

class MatrixGridCtrl;
class MatrixEntityBase;
class MatrixItemBase;
class SaveableObject;
class MatrixFilterSort;
class MatrixFrame;

		
class MatrixFilterHash
{
public:
	MatrixFilterHash() {};
	MatrixFilterHash(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken) :
	_matrixEntityToken(entityToken), 
	_matrixFilterSectionToken(matrixFilterSectionToken)
	{
		assert(entityToken < 1000);
	}

	bool operator == (const MatrixFilterHash& matrixFilterHash) const
	{
		return ((matrixFilterHash._matrixEntityToken == _matrixEntityToken)&&
			(matrixFilterHash._matrixFilterSectionToken == _matrixFilterSectionToken));
	}        

	MatrixEntityToken _matrixEntityToken;
	MatrixFilterSectionToken _matrixFilterSectionToken;
};	 


template<>
	inline UINT AFXAPI HashKey(MatrixFilterHash& key)
{
	//create a hash of the values
	return (key._matrixEntityToken *1000) + key._matrixFilterSectionToken;
}


class MatrixManager : public CCmdTarget, 
					  public ManagerBase, 					  					  					  
					  public MatrixManagerEventListener, 
					  public RecordManagerEventListener,
					  public OptionsManagerEventListener
{
public:

	void save();
	void load();
	void openUp();
	void closeDown();

	MatrixManager();
	virtual ~MatrixManager();


	void removeEventListener(MatrixManagerEventListener *matrixManagerEventListener);
	void addEventListener(MatrixManagerEventListener *matrixManagerEventListener);

	void fireUpdateFilterList();
	void fireEntityAdded(MatrixEntityToken token, long id);
	void fireEntityRemoved(MatrixEntityToken token, long id);
	void fireRefresh();

	MatrixGridCtrl* createEntityGridCtrl(MatrixEntityToken token, MatrixFilterSectionToken matrixFilterSectionToken);

	MatrixEntityBase* createEntity(MatrixEntityToken token);

	long addFilter(CWnd* pParent, MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken = mfst_user);
	void editFilter(CWnd* pParent, long uniqueId);
	void deleteFilter(int index);
	void applyFilterToCurrentGridView(MatrixFilterSort* matrixFilterSort);
	void applyFilterToCurrentGridView(long uniqueId);

	MatrixGridCtrl* getMatrixGridCtrl();

/*	

	void Serialize(CArchive& ar,  MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken);
*/
	bool isFilterNameUnique(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken, CString name);

	MatrixFilterSorts& getFilters(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken = mfst_user);
	void sortFilters(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken = mfst_user);

	static MATRIX_ENTITY_TRANSLATOR* GetTranslatorFunction(TranslatorFunctionItem translatorFunctionItem);

	void getCombinedFilterList(MatrixEntityToken matrixEntityToken, CombinedFilterItems& combinedFilterItems);
	int getCombinedFilterCount(ViewType viewType);
	void applyCombinedFilter(int index);

	void addIdToFilters(MatrixEntityToken token, long id);

	MatrixFilterSort* getFilter(long filterID);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatrixManager)
	public:

	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()

protected:

	static SaveableObject* MatrixEntityTranslatorDoNothing(SaveableObject* originalEntity);

	CMap<MatrixFilterHash, MatrixFilterHash&, MatrixFilterSorts*, MatrixFilterSorts*> _filters;

	MatrixManagerEventListeners _listenerList;

	void setupStockFilters();
	void onFilter(UINT index, MatrixFilterSectionToken matrixFilterSectionToken);

	//prop list stuff
	MatrixPropListExpanded _matrixPropListExpanded;
	
public:
	MatrixPropListExpanded& getMatrixPropListExpanded();
	void getStatusBarText(CString& pane1, CString& pane2, CString& pane3);

	afx_msg void OnStockFilter(UINT nID);
	afx_msg void OnUserFilter(UINT nID);
	afx_msg void OnSelchangeFilterComboChange();
	void setupFilterMenu(CMenu &menu, MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken, int maxNoFilters, UINT menuBaseId);

	void recordManager_notifyHomeTowerChange(RecordTower* /*homeTower*/);
	void optionsManager_optionsDlgClosed();
	void matrixManager_updateFilterList();

};


