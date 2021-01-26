#pragma once



#include "MatrixConstants.h"
#include "SaveableObjects.h"

class MatrixFilterSort;

enum LogicType;

class MatrixItemBase;

class MatrixItemBase  
{
public:
	MatrixItemBase();
	MatrixItemBase(const MatrixItemBase& matrixItemBase);
	virtual ~MatrixItemBase();

	virtual void Serialize(CArchive& ar);

	BOOL isSortAscending();
	void setSortAscending(BOOL sortAscending);
	
	void setColumnWidth(int columnWidth = -1);
	int getColumnWidth();

	CString getName(bool forDescription = false);

	int doCompare(SaveableObject* originalEntity1, SaveableObject* originalEntity2);

	void setTranslatorFunction(TranslatorFunctionItem translatorFunctionItem);
	void setRelationName(CString relationName);

	SaveableObject* translateEntity(SaveableObject* originalEntity);

	virtual COLORREF getTextColor(SaveableObject* translatedEntity);
	virtual CString getText(SaveableObject* translatedEntity);
	virtual MatrixGridImages getImage(SaveableObject* translatedEntity);
	virtual MatrixGridImages getHeaderImageIndex();

	virtual MatrixItemBase* clone() const = 0 ;

	virtual	MatrixItemToken getToken() const = 0;

	virtual MatrixFilterDescriptionItems& getFilterDescriptionItems(LogicType logicType);
	virtual MatrixFilterDescriptionItems& getSortDescriptionItems();
	virtual MatrixFilterDescriptionItems& getColumnDescriptionItems();
	virtual CString getSortDescriptionText();
	virtual CString getFilterDescriptionText();
	virtual CString getColumnDescriptionText();

	void clickLink(CWnd* parent, int xPos, MatrixFilterSort* matrixFilterSort);
	bool isLink(int xPos) const;

	virtual int getDefaultWidth();
	int isInvoked();

	virtual bool editFilterCondition(CWnd* parent) = 0;
	virtual bool editSortCondition(CWnd* parent) ;
	virtual bool editColumnCondition(CWnd* parent);

	void doAndFilter(SaveableObjects& originalEntitys) ;
	void doOrFilter(SaveableObjects& originalEntitys, CMap<SaveableObject*,SaveableObject*,int,int>& wantedOriginalItems);


	virtual bool doFilterShouldRemove(SaveableObject* translatedEntity) = 0 ;
protected:
	BOOL _filterEditedAndReadyToUse;	
	virtual CString getItemName() const = 0;


private:

	virtual int doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2) = 0;

	int getDescriptionIndex(int xPos) const;
	MatrixFilterDescriptionItem*getDescriptionItem(int xPos) const;


	BOOL _sortAscending;
	int _columnWidth;    
	MatrixFilterDescriptionItems _descriptionItems;

	//bits for the relationships
	MATRIX_ENTITY_TRANSLATOR* _fnTranslator;
	TranslatorFunctionItem _fnTranslatorItem;
    CString _relationName;
};


