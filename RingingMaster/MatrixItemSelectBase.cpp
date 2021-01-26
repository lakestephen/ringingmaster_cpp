// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MatrixItemSelectBase.h"
#include "ListSelectDlg.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemSelectBase::MatrixItemSelectBase()
{
}

MatrixItemSelectBase::MatrixItemSelectBase(const MatrixItemSelectBase& matrixItemSelectBase):
MatrixItemBase(matrixItemSelectBase)
{
	for (int i=0;i<matrixItemSelectBase._selectedItems.GetSize();i++)
		_selectedItems.Add(new ListSelectItem(*matrixItemSelectBase._selectedItems.GetAt(i)));
}



MatrixItemSelectBase::~MatrixItemSelectBase()
{
	destroySelectedItems();
}


void MatrixItemSelectBase::destroySelectedItems()
{
	for (int i=0;i<_selectedItems.GetSize();i++)
		delete _selectedItems.GetAt(i);

	_selectedItems.RemoveAll();
}


bool MatrixItemSelectBase::editFilterCondition(CWnd* parent)
{
	//create available list	
	ListSelectItems available;
	getAllItems(available);

	//get selected items
	ListSelectItems selected;
	getSelectedItems(available, selected);

	ListSelectDlg dlg(parent, "Filter on " + getName(), available, selected);
	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		setSelectedItems(selected);
		_filterEditedAndReadyToUse = selected.GetSize() > 0;
	}

	//delete 
	for (int i=0;i<available.GetSize();i++)
		delete available.GetAt(i);
					  
	return ret;
}

void MatrixItemSelectBase::getSelectedItems(const ListSelectItems &available, ListSelectItems &selected)
{
	for (int i=0;i<_selectedItems.GetSize();i++)
	{
		ListSelectItem* item = _selectedItems.GetAt(i);
		for (int j=0;j<available.GetSize();j++)
		{
			//are we using ref or string as primary key?
			if (item->_ref != -1)
			{
				//using the ref
				if (item->_ref == available.GetAt(j)->_ref)
					selected.Add(available.GetAt(j));
			}
			else
			{
				//using the strings 
				if (item->_text.Compare(available.GetAt(j)->_text) == 0)
					selected.Add(available.GetAt(j));
			}
		}
	}
}

void MatrixItemSelectBase::setSelectedItems(ListSelectItems &items)
{
	destroySelectedItems();

	for (int i=0;i<items.GetSize();i++)
	{
		_selectedItems.Add(new ListSelectItem(*items.GetAt(i)));
	}
}

CString MatrixItemSelectBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse)
	{
		CString str("is '");

		for (int i=0;i<_selectedItems.GetSize();i++)
		{
			str += _selectedItems.GetAt(i)->_text;
			if (i < _selectedItems.GetUpperBound()-1)
				str += ", ";
			else if (i == _selectedItems.GetUpperBound()-1)
				str += " or ";
		}

		str += "'";

		return str;
	}

	return MatrixItemBase::getFilterDescriptionText();
}

int MatrixItemSelectBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	CString str1 = getText(translatedEntity1);
	CString str2 = getText(translatedEntity2);

	return str1.Compare(str2);
}

bool MatrixItemSelectBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	CString str = getTextForFiltering(translatedEntity);

	//look through all the selected items to see if this item is here
	for (int i=0;i<_selectedItems.GetSize();i++)
	{
		if (_selectedItems.GetAt(i)->_text == str)
			return FALSE;
	}

	return TRUE;
}

CString MatrixItemSelectBase::getTextForFiltering(SaveableObject* translatedEntity)
{
	return getText(translatedEntity);
}

void MatrixItemSelectBase::Serialize(CArchive& ar)
{
	MatrixItemBase::Serialize(ar);

	int version = 2;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _selectedItems.GetSize();
		for (int i=0;i<_selectedItems.GetSize();i++)
		{
			_selectedItems.GetAt(i)->Serialize(ar);
		}

	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		_selectedItems.RemoveAll();
		if (thisVersion == 1)
		{
			// This is a bug, but to allow the reuse of the filters, 
			// we handle the problem by doing the desrialisation of 
			// the array, then delete it.
			// We were serialising the memory address of the items, rather than the items themselves.
			_selectedItems.Serialize(ar);
			_selectedItems.RemoveAll();
		}
		else if (thisVersion == 2)
		{
			// correct de seralisation
			_selectedItems.RemoveAll();
			int count;
			ar >> count;
			for (int i=0;i<count;i++)
			{
				ListSelectItem* item = new ListSelectItem;
				item->Serialize(ar);
				_selectedItems.Add(item);
			}

			ASSERT(count == _selectedItems.GetSize());
		}
		else
		{
			ASSERT(thisVersion == version);
			AfxThrowArchiveException(CArchiveException::badSchema);	
		}
	}
}
