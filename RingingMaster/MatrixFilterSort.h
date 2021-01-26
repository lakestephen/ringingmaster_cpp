#pragma once


#include "MatrixConstants.h"

enum LogicType;


class MatrixFilterSort  
{
public:

	void Serialize(CArchive& ar);

	MatrixFilterSort(MatrixEntityToken entityToken, MatrixFilterSectionToken matrixFilterSectionToken = mfst_user);
	MatrixFilterSort(const MatrixFilterSort& matrixFilterSort, bool newID = false, bool setMatrixFilterSectionToken = false, MatrixFilterSectionToken matrixFilterSectionToken = mfst_user);
	virtual ~MatrixFilterSort();

	MatrixFilterSort& operator = (const MatrixFilterSort& matrixFilterSort) ;

	MatrixFilterSectionToken getMatrixFilterSectionToken();
	MatrixEntityToken getMatrixEntityToken();

	void setFilter(const MatrixItemBase& filterItem, bool active = true);
	void setSort(const MatrixItemBase& filterItem, bool active = true, bool forceDirection = false);
	MatrixItemBase* setColumn(const MatrixItemBase& columnItem, bool active = true, bool forceDefaultWidth = false);

	void clearSorts();
	void clearColumns();

	int getFilterCount();
	int getSortCount();
	int getColumnCount();
		  
	MatrixItemBase* getFilter(int index);
	MatrixItemBase* getSort(int index);
	MatrixItemBase* getColumn(int index);

	CString getDescription();

	CString getEntityName();

	void editLogic(CWnd* parent);

	bool moveSortUp(int index);
	bool moveSortDown(int index);
	bool deleteSort(int index);

	bool moveColumnUp(int index);
	bool moveColumnDown(int index);
	bool deleteColumn(int index);
	bool moveColumn(int index, int newIndex);
	int getColumnIndex(MatrixItemToken token);

	bool deleteFilter(int index);

	CString getName(bool display = false);
	void setName(const CString& name);
	


	long getUniqueID();

	static long _lastUniqueID;

	LogicType getLogicType();

	BOOL isAllInvoked();

protected:
	MatrixEntityToken _entityToken;
	MatrixFilterSectionToken _matrixFilterSectionToken;
	CString _entityName;

	MatrixItemBases _sort;	
	MatrixItemBases _filter;	
	MatrixItemBases _column;	

	LogicType  _logicType;

	CString _name;

	long _uniqueID;

	void generateUniqueID();


};


