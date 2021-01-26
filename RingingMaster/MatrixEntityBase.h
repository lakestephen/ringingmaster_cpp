
#pragma once

#include "MatrixConstants.h"


class MatrixRelation;
class MatrixItemBase;
class MatrixFilterSort;


class SaveableObject;
class CPage ;
struct DragDropTargetInfo;



											   
class MatrixEntityBase  
{
public:
	MatrixEntityBase();
	virtual ~MatrixEntityBase();


protected:

	virtual UINT getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity);

   	BOOL contextClick(const SaveableObjects& contextSelectedObjects, SaveableObject* contextClickObject, CWnd* parent);
	virtual void doubleClick(SaveableObject* originalEntity, CWnd* parent);
	virtual void mouseOver(SaveableObject* originalEntity);

	virtual void showTooltips(const CPoint& cursorPos, SaveableObject* originalEntity) {};

	bool retrieveEntityArrayFromIDs(SaveableObjects& array);
	bool retrieveIDsFromEntityArray(SaveableObjects& array);
	

	// overridden in the implementation classes to provide a complete list of all entitys
	virtual void resetIdList(CArray<long, long>& ids) = 0;

	long getIDFromRow(int row);

	double printMatrix(CPage * page, double nextRow, MatrixFilterSort* filterSort, int startRow, int endRow, const unsigned int curHorzPage);

	static void CreateColumnHelper(MatrixItemBases& columns, MatrixItemToken matrixItemToken);
	
public:


 	SaveableObject* getEntityFromRow(int row);

	void applyFilterSort(MatrixFilterSort* filterSort);

	void print(MatrixFilterSort& filterSort, CArray<int, int>* selectionRows = NULL);
	
	int getRowFromID(long id);

	virtual bool canStartDrag();
	virtual bool canAcceptDrop(DragDropTargetInfo* info);   
	virtual bool performDrop(DragDropTargetInfo* info, long iRow);


	void showTooltips(const CPoint& cursorPos, int row);


	void getEntitysFromRows(SaveableObjects& selectedEntitys, const CArray<long, long>& selectedRows);
	void getIDsFromRows(CArray<long, long>& selectedIDs, const CArray<long, long>& selectedRows);

	void addItemToFilter(long id, bool memory = false);
	void removeItemFromFilter(long id, bool memory = false);

	void resetIdList();
	int getIdCount() const;
	virtual int getUnfilteredIdCount() const = 0;
	
	void invertSelection();
	void memoryAssign();
	void mainAssign();
	void memorySwap();
	void memoryOr();
	void memoryMinus();
	void memoryAnd();

	virtual void getStatusBarText(CString& pane1, CString& pane2, CString& pane3);
	virtual void getStatusBarText(CString& pane1) = 0;

	virtual void createColumns(MatrixItemBases& columns) = 0;
	virtual void createRelations(MatrixRelations& relations) = 0;

	void applyDefaultSort();
	virtual MatrixItemToken getDefaultSortToken() = 0;
	void filterListSimple(const MatrixItemBase& item);

	void sortListCol(int column, bool bAscending);

	MatrixItemBase* getColumn(int col);
	int getColumnCount();
	
	COLORREF getTextColor(int row, int column);
	CString getText(int row, int column);
	MatrixGridImages getImage(int row, int column);
	
	virtual MatrixEntityToken getMatrixEntityToken() = 0;
	
	int getRowCount();
	void init(void);
	virtual CString getName() = 0;
	virtual CString getShortName() {return getName();}

	void mouseOver(int row);
	void doubleClick(int row, CWnd* parent);
	BOOL contextClick(const CArray<long, long>& selectedRows, int row, CWnd* parent);


private:
	MatrixItemBases _allColumns; //holds the instance of columns
	MatrixRelations _relations; //holds the relations instances of the columns
	MatrixItemBases _primaryColumns; //holds the colums for the primary type
	CArray<long, long> _ids;
	CArray<long, long> _memoryIds;

	static MatrixFilterSort* _filterSort;//used for static access in the sorting and filtering routines
	static int sortCompare( const void * arg1, const void * arg2 );

	virtual bool getEntityFromId(long id, SaveableObject*& entity) = 0;
	virtual long getIdFromEntity(SaveableObject* entity, long& id) = 0;

	void sortListSimple(const MatrixItemBase& item);


	void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	SaveableObjects _contextSelectedObjects; 
	SaveableObject* _contextClickObject;


	friend class MatrixRelation;


};

