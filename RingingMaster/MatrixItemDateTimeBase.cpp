// MatrixItemBase.cpp: implementation of the MatrixItemBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixItemDateTimeBase.h"
#include "DateTimeSelectDlg.h"
#include "ringingmaster.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixItemDateTimeBase::MatrixItemDateTimeBase() 
{

}

MatrixItemDateTimeBase::MatrixItemDateTimeBase(const MatrixItemDateTimeBase& matrixItemDateTimeBase):
MatrixItemBase(matrixItemDateTimeBase),
_dateTimeSelectItem(matrixItemDateTimeBase._dateTimeSelectItem)
{

}


MatrixItemDateTimeBase::~MatrixItemDateTimeBase()
{
}

bool MatrixItemDateTimeBase::editFilterCondition(CWnd* parent)
{
	//NOTE: if this is done in the constructor, then it will never call the subclass version.
	_dateTimeSelectItem._format = getDateTimeSelectFormatType();

	DateTimeSelectDlg dlg(parent, _dateTimeSelectItem,  "Filter on " + getName());

	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}      

int MatrixItemDateTimeBase::doItemCompare(SaveableObject* translatedEntity1, SaveableObject* translatedEntity2)
{
	ASSERT(translatedEntity1);
	ASSERT(translatedEntity2);

	COleDateTime dateTime1 = getDateTime(translatedEntity1);
	COleDateTime dateTime2 = getDateTime(translatedEntity2);

	setDateTimeToFormat(dateTime1);
	setDateTimeToFormat(dateTime2);

	if (!dateTime1.GetStatus() == COleDateTime::valid && !dateTime2.GetStatus() == COleDateTime::valid)
		return 0;
	else if (dateTime1.GetStatus() == COleDateTime::valid && !dateTime2.GetStatus() == COleDateTime::valid)
		return 1;
	else if (!dateTime1.GetStatus() == COleDateTime::valid && dateTime2.GetStatus() == COleDateTime::valid)
		return -1;
	else
		return 	(int)(dateTime1.m_dt - dateTime2.m_dt);
}

bool MatrixItemDateTimeBase::doFilterShouldRemove(SaveableObject* translatedEntity)
{
	COleDateTime dateTime = getDateTime(translatedEntity);
	
	setDateTimeToFormat(dateTime);
	setDateTimeToFormat(_dateTimeSelectItem._dateTime1);
	setDateTimeToFormat(_dateTimeSelectItem._dateTime2);

	if (dateTime.GetStatus() == COleDateTime::valid)
	{
		switch (_dateTimeSelectItem._type)
		{
		case dtst_greater:
			return dateTime <= _dateTimeSelectItem._dateTime1;
		case dtst_less:
			return dateTime >= _dateTimeSelectItem._dateTime1;
		case dtst_between:
			return dateTime <= _dateTimeSelectItem._dateTime1 || dateTime >= _dateTimeSelectItem._dateTime2;
		case dtst_exact:
			return dateTime != _dateTimeSelectItem._dateTime1;
		}
	}

	return true;
}

CString MatrixItemDateTimeBase::getText(SaveableObject* translatedEntity)
{
	COleDateTime dateTime = getDateTime(translatedEntity);

	return dateTime.Format(getDateTimeSelectFormatTypeString());       
}

CString MatrixItemDateTimeBase::getFilterDescriptionText()
{
	if (_filterEditedAndReadyToUse)
	{
		CString str("is '");
		
		switch (_dateTimeSelectItem._type)
		{
		case dtst_greater:
			str += "greater than ";
			break;
		case dtst_less:
			str += "less than ";
			break;
		case dtst_between:
			str += "between ";
			break;
		case dtst_exact:
			str += "exact ";
			break;
		}
	
		str += _dateTimeSelectItem._dateTime1.Format(getDateTimeSelectFormatTypeString());

		if (_dateTimeSelectItem._type == dtst_between)
		{
			str += " and ";
			str += _dateTimeSelectItem._dateTime2.Format(getDateTimeSelectFormatTypeString());
		}

		str += "'";

		return str;
	}

	return MatrixItemBase::getFilterDescriptionText();}

int MatrixItemDateTimeBase::getDefaultWidth()
{
	return 102;
}

void MatrixItemDateTimeBase::setDateTimeSelectItem(const DateTimeSelectItem& dateTimeSelectItem)
{
	_dateTimeSelectItem = dateTimeSelectItem;

	_filterEditedAndReadyToUse = TRUE;
}

DateTimeSelectFormatType MatrixItemDateTimeBase::getDateTimeSelectFormatType()
{
	return dtsft_date_time;
}  

CString MatrixItemDateTimeBase::getDateTimeSelectFormatTypeString()
{
	switch (getDateTimeSelectFormatType())
	{
	case dtsft_date:
		return "%x";
	case dtsft_time:
		return "%H:%M";
	case dtsft_date_time:
	default:
		return "%x %H:%M";
	}
}

void MatrixItemDateTimeBase::setDateTimeToFormat(COleDateTime& dateTime)
{
	switch (getDateTimeSelectFormatType())
	{
	case dtsft_date:
		//set to midnight 
		dateTime = COleDateTime(dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(),0,0,0);
		break;
	case dtsft_time:
		//set to arbatory date
		dateTime = COleDateTime(2000,01,01,dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
		break;
	case dtsft_date_time:
	default:
		break;
	}
}

void MatrixItemDateTimeBase::Serialize(CArchive& ar)
{//todo test
	MatrixItemBase::Serialize(ar);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		_dateTimeSelectItem.Serialize(ar);
	}
	else
	{
		int thisVersion;
		ar >> thisVersion;
		ASSERT(thisVersion == version);
		if (thisVersion != version)
			AfxThrowArchiveException(CArchiveException::badSchema);	

		_dateTimeSelectItem.Serialize(ar);
	}
}




