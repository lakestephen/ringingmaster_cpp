// RecordGrab.cpp: implementation of the RecordGrab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordGrab.h"
#include "RecordTower.h"
#include "RecordManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RecordGrab::RecordGrab() :
SaveableObject(0), //todo
_index(0),
_towerIndex(0),
_dateOle(COleDateTime::GetCurrentTime()),
_comments(""),
_bellsGo(bgt_none),
_bellsSound(bst_none),
_dayOrder(0),
_secondGrabIndex(-1)
{

}

RecordGrab::~RecordGrab()
{

}

void RecordGrab::Serialize(CArchive& ar)
{
	int version = 2;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _towerIndex;
		ar << _dateOle;
		ar << _bellsGo;
		ar << _bellsSound;
		ar << _dayOrder;
		ar << _comments;
	}
	else
	{
		int thisVersion;

		ar >> thisVersion;

		if (thisVersion == 1) //upto and including version 2.01 Build RC4
			deserialize_v1(ar);
		else if (thisVersion == 2) //version 2.01 Build RC5
			deserialize_v2(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
	}	
}

	
void RecordGrab::deserialize_v1(CArchive& ar)
{ //todo ditch these versions.
	TRACE("Loading a version 1 RecordGrab \r\n");

	ar >> _towerIndex;
	
	
	CTime date;	//version 1 was a CTime
	ar >> date;	   
	_dateOle = COleDateTime(date.GetYear(), date.GetMonth(), date.GetDay(),0,0,0);

	int value;
	ar >> value ;
		_bellsGo = (BellsGo)value;
	ar >> value ;
		_bellsSound = (BellsSound)value;
	ar >> _dayOrder;
	ar >> _comments;
}


void RecordGrab::deserialize_v2(CArchive& ar)
{
	TRACE("Loading a version 2 RecordGrab \r\n");

	ar >> _towerIndex;
	ar >> _dateOle;
	int value;
	ar >> value ;
		_bellsGo = (BellsGo)value;
	ar >> value ;
		_bellsSound = (BellsSound)value;
	ar >> _dayOrder;
	ar >> _comments;
}



void RecordGrab::setComments(CString comments)
{
	_comments = comments;
}
void RecordGrab::setBellsGo(BellsGo bellsGo)
{
	_bellsGo = bellsGo;
}

void RecordGrab::setBellsSound(BellsSound bellsSound)
{
	_bellsSound = bellsSound;
}

void RecordGrab::setDateOle(COleDateTime dateOle)
{
	_dateOle = dateOle;
}

int RecordGrab::getTowerIndex()
{
	return _towerIndex;
}

void RecordGrab::setTowerIndex(int towerIndex)
{
	_towerIndex = towerIndex;
}

CString RecordGrab::getDateAsString(bool shortDate)
{
	if (shortDate)
		return _dateOle.Format("%x"); 
	else
		return _dateOle.Format("%#x"); 
}

CString RecordGrab::getCommentAsSingleLine()
{
	CString comments = _comments;

	comments.Replace("\r", "");
	comments.Replace("\n", "");

	return comments;
}

CString RecordGrab::getComments()
{
	return _comments;
}

COleDateTime RecordGrab::getDateOle()
{
	return _dateOle;
}

BellsGo RecordGrab::getBellsGo()
{
	return _bellsGo;
}

CString RecordGrab::getBellsGoAsString()
{
	return getBellsGoName(getBellsGo());
}

BellsSound RecordGrab::getBellsSound()
{
	return _bellsSound;
}

CString RecordGrab::getBellsSoundAsString()
{
	return getBellsSoundName(getBellsSound());
}

int RecordGrab::getGrabIndex()
{
	return _index;
}

CString RecordGrab::getGrabIndexAsString()
{
	if (_index == 0)
	{
		ASSERT(FALSE);
		return "";
	}
	
	CString str;
	if (_secondGrabIndex != -1)
		str.Format("%d (%d)", _index, _secondGrabIndex);
	else
		str.Format("%d", _index);
	return str;
}

void RecordGrab::setGrabIndex(int index)
{
	_index = index;
}

CString RecordGrab::getBellsGoName(BellsGo type)
{
	switch (type)
	{
	case bgt_wow:
		return "Wow";
	case bgt_excellent:
		return "Excellent";
	case bgt_good:
		return "Good";
	case bgt_average:
		return "Average";
	case bgt_poor:
		return "Poor";
	case bgt_veryPoor:
		return "Very Poor";
	case bgt_unringable:
		return "Unringable";
	case bst_none:
		return "";
	default:
		ASSERT(FALSE);
		return "";
	}
}

CString RecordGrab::getBellsSoundName(BellsSound type)
{
	switch (type)
	{
	case bst_sweet:
		return "Sweet";
	case bst_excellent:
		return "Excellent";
	case bst_good:
		return "Good";
	case bst_average:
		return "Average";
	case bst_poor:
		return "Poor";
	case bst_veryPoor:
		return "Very Poor";
	case bst_tinCans:
		return "Tin Cans";
	case bst_none:
		return "";

	default:
		ASSERT(FALSE);
		return "";
	}
}

void RecordGrab::bellGoFillComboBox(CComboBox &comboBox)
{
	comboBox.ResetContent();

	for (int i=0;i<=bgt_unringable;i++)
	{
		CString str = getBellsGoName((BellsGo)i);
		if (str.IsEmpty())
			str = "[none]";
		int index = comboBox.AddString(str);
		comboBox.SetItemData(i, i);
		if (i == _bellsGo)
			comboBox.SetCurSel(index);
	}
}

void RecordGrab::bellSoundFillComboBox(CComboBox &comboBox)
{
	comboBox.ResetContent();

	for (int i=0;i<=bst_tinCans;i++)
	{
		CString str = getBellsSoundName((BellsSound)i);
		if (str.IsEmpty())
			str = "[none]";
		int index = comboBox.AddString(str);
		comboBox.SetItemData(i, i);
		if (i == _bellsSound)
			comboBox.SetCurSel(index);
	}
}

int RecordGrab::getDayOrder()
{
	return _dayOrder;
}

void RecordGrab::setDayOrder(int dayOrder)
{
	_dayOrder = dayOrder;
}

CString RecordGrab::getDisplayName()
{
	RecordTower* tower = getTower();

	CString displayName;

	if (tower)
	{
		displayName = tower->getDisplayName();

		if (tower->getGrabCount() > 1)
		{
			displayName += " (";
			displayName += getDateAsString(false);
			displayName += ")";
		}			
	}

	return displayName;
}

void RecordGrab::setSecondGrabIndex(int secondGrabIndex)
{
	_secondGrabIndex = secondGrabIndex;
}

int RecordGrab::getSecondGrabIndex()
{
	return _secondGrabIndex;
}

RecordTower* RecordGrab::getTower()
{
	return recordManager()->getTower(getTowerIndex());
}
