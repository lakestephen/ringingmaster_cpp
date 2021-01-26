// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixItemBase.h"
#include "LogicDlg.h"
#include "DirectionDlg.h"
#include "ColumnWidthDlg.h"
#include "RingingMaster.h"
#include "MatrixManager.h"
#include "MatrixFilterSort.h"
#include "MatrixFilterDescriptionItem.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemBase::MatrixItemBase() :
_sortAscending(true),
_filterEditedAndReadyToUse(FALSE),
_columnWidth(-1),
_fnTranslatorItem(tfi_doNothing)
{
	setTranslatorFunction(tfi_doNothing);

	_columnWidth = getDefaultWidth();
}

MatrixItemBase::MatrixItemBase(const MatrixItemBase& matrixItemBase) :
_fnTranslatorItem(matrixItemBase._fnTranslatorItem),
_fnTranslator(matrixItemBase._fnTranslator),
_sortAscending(matrixItemBase._sortAscending),
_relationName(matrixItemBase._relationName),
_filterEditedAndReadyToUse(matrixItemBase._filterEditedAndReadyToUse), 
_columnWidth(matrixItemBase._columnWidth)
{
	for (int i=0;i<matrixItemBase._descriptionItems.GetSize();i++)
	{
		_descriptionItems.Add(
			new MatrixFilterDescriptionItem(*matrixItemBase._descriptionItems.GetAt(i)));
	}
}

MatrixItemBase::~MatrixItemBase()
{
	for (int i=0;i<_descriptionItems.GetSize();i++)
		delete _descriptionItems.GetAt(i);

	_descriptionItems.RemoveAll();
}

void MatrixItemBase::setTranslatorFunction(TranslatorFunctionItem translatorFunctionItem)
{
	_fnTranslatorItem = translatorFunctionItem;

	//get the translator function
	_fnTranslator = MatrixManager::GetTranslatorFunction(translatorFunctionItem);
	ASSERT(_fnTranslator);
}



CString MatrixItemBase::getText(SaveableObject* /*translatedEntity*/)
{
	return "";
}                                

COLORREF MatrixItemBase::getTextColor(SaveableObject* /*translatedEntity*/)
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void MatrixItemBase::setSortAscending(BOOL sortAscending)
{
	_sortAscending = sortAscending;
	_filterEditedAndReadyToUse = TRUE;
}

int MatrixItemBase::getColumnWidth()
{
	return _columnWidth;
}

void MatrixItemBase::setColumnWidth(int columnWidth)
{
	if (columnWidth == -1)
		_columnWidth = getDefaultWidth();
	else
		_columnWidth = columnWidth;

	_filterEditedAndReadyToUse = TRUE;
}

BOOL MatrixItemBase::isSortAscending()
{
	return _sortAscending;
}

void MatrixItemBase::setRelationName(CString relationName)
{
	_relationName = relationName;
}

CString MatrixItemBase::getName(bool forDescription)
{
	CString str;

	if (_relationName.IsEmpty())
	{
		if (forDescription)
			str.Format("<>  %s",  getItemName());
		else
			str = getItemName();
	}
	else
	{
		if (forDescription)
			str.Format("%s %s", _relationName, getItemName()); 
		else
			str.Format("%s (%s)", getItemName(), _relationName); 
	}
	return str;
}

int MatrixItemBase::doCompare(SaveableObject* originalEntity1, SaveableObject* originalEntity2)
{
	SaveableObject* translatedEntity1 = NULL;
	SaveableObject* translatedEntity2 = NULL;
		
	if (originalEntity1)
		translatedEntity1 = _fnTranslator(originalEntity1);
	if (originalEntity2)
		translatedEntity2 = _fnTranslator(originalEntity2);
	
	//first do a quick check that none of the entitys are NULL;
	if ((translatedEntity1 == NULL) &&(translatedEntity2 != NULL))
		return 1;
	else if ((translatedEntity1 != NULL) &&(translatedEntity2 == NULL))
		return -1;
	else if ((translatedEntity1 == NULL) &&(translatedEntity2 == NULL))
		return 0;
	else
		return doItemCompare(translatedEntity1, translatedEntity2);
}

SaveableObject* MatrixItemBase::translateEntity(SaveableObject* originalEntity)
{
	if ((originalEntity)&&(_fnTranslator))
		return _fnTranslator(originalEntity);

	return NULL;
}


MatrixGridImages MatrixItemBase::getHeaderImageIndex()
{
	return mgi_none;
}

MatrixFilterDescriptionItems& MatrixItemBase::getFilterDescriptionItems(LogicType logicType)
{
	if (_descriptionItems.GetSize() == 0)
	{
		CString name = getName();
		name.MakeLower();

		_descriptionItems.Add(new MatrixFilterDescriptionItem("where the " + name + " " , mfdit_pre));
		_descriptionItems.Add(new MatrixFilterDescriptionItem("", mfdit_filterCondition));
		_descriptionItems.Add(new MatrixFilterDescriptionItem(" ", mfdit_post));
		_descriptionItems.Add(new MatrixFilterDescriptionItem("", mfdit_logic));
	}  

	for (int i=0;i<_descriptionItems.GetSize();i++)
	{
		MatrixFilterDescriptionItem* item = _descriptionItems.GetAt(i);
		if (item->isFilterCondition())
			item->setText(getFilterDescriptionText());

		if (item->isLogic())
		{
			switch (logicType)
			{
			case lt_or:
				item->setText("or");
				break;
			case lt_and:
				item->setText("and");
				break;
			}
		}

	}

	return _descriptionItems;
}

MatrixFilterDescriptionItems& MatrixItemBase::getSortDescriptionItems()
{
	if (_descriptionItems.GetSize() == 0)
	{
		CString name = getName();
		name.MakeLower();

		_descriptionItems.Add(new MatrixFilterDescriptionItem(name + " ", mfdit_pre));
		_descriptionItems.Add(new MatrixFilterDescriptionItem("", mfdit_sortCondition));
		_descriptionItems.Add(new MatrixFilterDescriptionItem(" then by", mfdit_post));
	}  

	for (int i=0;i<_descriptionItems.GetSize();i++)
	{
		MatrixFilterDescriptionItem* item = _descriptionItems.GetAt(i);
		if (item->isSortCondition())
			item->setText(getSortDescriptionText());
	}

	return _descriptionItems;
}

MatrixFilterDescriptionItems& MatrixItemBase::getColumnDescriptionItems()
{
	if (_descriptionItems.GetSize() == 0)
	{
		CString name = getName();
		name.MakeLower();

		_descriptionItems.Add(new MatrixFilterDescriptionItem(name + " ", mfdit_pre));
		_descriptionItems.Add(new MatrixFilterDescriptionItem("", mfdit_columnCondition));
		_descriptionItems.Add(new MatrixFilterDescriptionItem(", ", mfdit_post));
	}  
				  
	for (int i=0;i<_descriptionItems.GetSize();i++)
	{
		MatrixFilterDescriptionItem* item = _descriptionItems.GetAt(i);
		if (item->isColumnCondition())
			item->setText(getColumnDescriptionText());
	}				

	return _descriptionItems;
}

CString MatrixItemBase::getSortDescriptionText()
{
	if (!_filterEditedAndReadyToUse)
		return "choose direction"; //should always be invoked
	
	return  _sortAscending?"ascending":"descending";
}

CString MatrixItemBase::getFilterDescriptionText()
{
	return "is specified";				 
}

CString MatrixItemBase::getColumnDescriptionText()
{
	CString str;
	str.Format("(%d)", _columnWidth);
	return str;			 
}

bool MatrixItemBase::isLink(int xPos) const
{
	MatrixFilterDescriptionItem* item = getDescriptionItem(xPos);

	if (item)
		return item->isLink();

	return false;
}

int MatrixItemBase::getDescriptionIndex(int xPos) const
{
	for (int i=0;i<_descriptionItems.GetSize();i++)
	{
		if ((xPos > _descriptionItems.GetAt(i)->getLeft())&&
			(xPos <= _descriptionItems.GetAt(i)->getRight()))
		{
			return i;
		}														  
	}
	return -1;
}

MatrixFilterDescriptionItem* MatrixItemBase::getDescriptionItem(int xPos) const
{
	int index = getDescriptionIndex(xPos);
	if ((index >=0)&&(index < _descriptionItems.GetSize()))
		return _descriptionItems.GetAt(index);

	return NULL;
}

void MatrixItemBase::clickLink(CWnd* parent, int xPos, MatrixFilterSort* matrixFilterSort)
{	 
	MatrixFilterDescriptionItem* item = getDescriptionItem(xPos);
	

	if ((item)&&(item->isLink()))
	{
		if (item->isLogic())
			matrixFilterSort->editLogic(parent);	
		if (item->isFilterCondition())
			editFilterCondition(parent);
		if (item->isSortCondition())
			editSortCondition(parent);
		if (item->isColumnCondition())
			editColumnCondition(parent);
	}	
}

int MatrixItemBase::getDefaultWidth()
{
	return 65;
}

int MatrixItemBase::isInvoked()
{
	return _filterEditedAndReadyToUse;
}

bool MatrixItemBase::editSortCondition(CWnd* parent)
{
	DirectionDlg dlg(_sortAscending, parent);
	if (dlg.DoModal() == IDOK)
	{
		_filterEditedAndReadyToUse = TRUE;
		return true;
	}
	return false;
}

bool MatrixItemBase::editColumnCondition(CWnd* parent)
{
	ColumnWidthDlg dlg(_columnWidth, parent, getName());
	if (dlg.DoModal() == IDOK)
	{
		_filterEditedAndReadyToUse = TRUE;
		return true;
	}
	return false;
}

void MatrixItemBase::doAndFilter(SaveableObjects& originalEntitys)
{
	for (int i=0;i<originalEntitys.GetSize();i++)
	{
		SaveableObject* originalEntity  =originalEntitys.GetAt(i) ;
		SaveableObject* translatedEntity  = NULL;
			
		if (originalEntity)
			translatedEntity = _fnTranslator(originalEntity);
		
		//ASSERT(translatedEntity);

		if ((!translatedEntity)||(doFilterShouldRemove(translatedEntity)))
		{
			//NEVER delete items here. Just remove them
			originalEntitys.RemoveAt(i--);
		}
	}	
}

void MatrixItemBase::doOrFilter(SaveableObjects& originalEntitys, CMap<SaveableObject*,SaveableObject*,int,int>& wantedOriginalItems)
{
	for (int i=0;i<originalEntitys.GetSize();i++)
	{
		SaveableObject* originalEntity  =originalEntitys.GetAt(i) ;
		SaveableObject* translatedEntity  = NULL;
			
		if (originalEntity)
			translatedEntity = _fnTranslator(originalEntity);
		
		ASSERT(translatedEntity);

		if (translatedEntity)
		{
			if (!doFilterShouldRemove(translatedEntity))
			{
				wantedOriginalItems.SetAt(originalEntity,0);
			}
		}
	}	
}

void MatrixItemBase::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _columnWidth;
		ar << _filterEditedAndReadyToUse;
		ar << _relationName;
		ar << _sortAscending;
		ar << _fnTranslatorItem;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		ar >> _columnWidth;
		ar >> _filterEditedAndReadyToUse;
		ar >> _relationName;
		ar >> _sortAscending;

		int fnTranslatorItem;
		ar >> fnTranslatorItem;
		setTranslatorFunction(static_cast<TranslatorFunctionItem>(fnTranslatorItem));
	}
}

MatrixGridImages MatrixItemBase::getImage(SaveableObject* translatedEntity)
{
	return mgi_none;
}