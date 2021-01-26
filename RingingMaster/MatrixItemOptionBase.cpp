// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixItemOptionBase.h"
#include "OptionSelectDlg.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemOptionBase::MatrixItemOptionBase() : 
_optionItem(TRUE), 
_imageList(NULL)
{
	_filterEditedAndReadyToUse = TRUE;
}

MatrixItemOptionBase::MatrixItemOptionBase(const MatrixItemOptionBase& matrixItemOptionBase):
MatrixItemBase(matrixItemOptionBase),
_optionItem(matrixItemOptionBase._optionItem), 
_imageList(NULL)
{
}



MatrixItemOptionBase::~MatrixItemOptionBase()
{
	delete _imageList;
}

bool MatrixItemOptionBase::editFilterCondition(CWnd* parent)
{
	OptionSelectDlg dlg(parent, getTrueString(), getFalseString(), _optionItem,  "Filter on " + getName());

	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}      

CString MatrixItemOptionBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse) 
	{
		if (_optionItem)
			return "flag is 'True'";
		else
			return "flag is 'False'";
	}

	return MatrixItemBase::getFilterDescriptionText();
}

int MatrixItemOptionBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	bool option1 = getOption(translatedEntity1);
	bool option2 = getOption(translatedEntity2);

	
	if ( option1 && !option2) return 1;
	if (!option1 &&  option2) return -1;
	
	return 0;
}

bool MatrixItemOptionBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	return getOption(translatedEntity) != (_optionItem == TRUE);
}

CString MatrixItemOptionBase::getText(SaveableObject* translatedEntity)
{
	return "";
}

MatrixGridImages MatrixItemOptionBase::getImage(SaveableObject* translatedEntity)
{
	return getOption(translatedEntity)?mgi_blue_tick:mgi_none;
}

void MatrixItemOptionBase::setOptionItem(bool optionItem)
{
	_optionItem = optionItem;
	_filterEditedAndReadyToUse = TRUE;
}

CString MatrixItemOptionBase::getTrueString()
{
	return getItemName();
}

CString MatrixItemOptionBase::getFalseString()
{
	return "Not " + getItemName();
}
			  
void MatrixItemOptionBase::Serialize(CArchive& ar)
{
	MatrixItemBase::Serialize(ar);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;		
		
		ar << _optionItem;
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		ar >>_optionItem;
	}
}

int MatrixItemOptionBase::getDefaultWidth()
{
	return 18;
}