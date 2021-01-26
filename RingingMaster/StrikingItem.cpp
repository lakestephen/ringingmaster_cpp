#include "StdAfx.h"
#include ".\strikingitem.h"

StrikingItem::StrikingItem() :
_bell(-1), 
_timestamp(-1)
{
 	
}

StrikingItem::StrikingItem(int bell, int timestamp) :
_bell(bell), 
_timestamp(timestamp)
{
	ASSERT(timestamp < 100000);
//	TRACE("StrikingItem bell %d, %d", bell, timestamp);
}

StrikingItem::StrikingItem(const CString& comment, int timestamp) :
_bell(-1), 
_comment(comment),
_timestamp(timestamp)
{
	ASSERT(timestamp < 100000);
//	TRACE("StrikingItem Comment %s, %d", comment, timestamp);
}


StrikingItem::~StrikingItem(void)
{
}

void StrikingItem::setBell(int bell)
{
	_bell = bell;
}

int StrikingItem::getBell() const
{
	return _bell;
}	

void StrikingItem::setTimestamp(int timestamp)
{
	ASSERT(timestamp < 100000);

	_timestamp = timestamp;
}

int StrikingItem::getTimestamp() const
{
	return _timestamp;
}	

const CString& StrikingItem::getComment() const
{
	return _comment;
}

void StrikingItem::setComment(CString comment)
{
	_comment = comment;
}

void StrikingItem::Serialize(CArchive& ar)
{
	short version = 1;
	if (ar.IsStoring())
	{
		ar << version; 
		short bell = (short)_bell;
		ar << bell;
		ar << _timestamp;
		ar << _comment;
	}
	else
	{
		ar >> version;  
		short bell;
		ar >> bell;
		_bell = bell;
		ar >> _timestamp;
		ar >> _comment;
	}
}

bool StrikingItem::isComment()
{
	return _bell == -1;
}