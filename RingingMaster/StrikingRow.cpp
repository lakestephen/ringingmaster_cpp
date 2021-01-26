#include "StdAfx.h"
#include "strikingrow.h"
#include "strikingitem.h"
#include "row.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

StrikingRow::StrikingRow(Stroke stroke, DWORD baseTimestamp, int speed, const Row& row) :
_stroke(stroke), 
_baseTimestamp(baseTimestamp), 
_speed(speed)
{
//	TRACE("starting row %s\r\n", row.getAsText());

	//first set the perfect time bell	
	for (int i=0;i<MAXBELLS;i++) 
	{
		_perfectTimeBell[i].setTimestamp(speed*(i+1));
		if (i < row.getNumber())
			_perfectTimeBell[i].setBell(row.getBellInPlace(i)-1);
		else
			_perfectTimeBell[i].setBell(i);
	}

//	TRACE("Setting StrikingRow base timestamp: %d\r\n", _baseTimestamp);
}
	
StrikingRow::StrikingRow(const StrikingRow& strikingRow):
_stroke(strikingRow._stroke), 
_baseTimestamp(strikingRow._baseTimestamp), 
_speed(strikingRow._speed)
{       
	for (POSITION pos = strikingRow.GetHeadPosition (); pos != NULL;)
	{
		const StrikingItem* strikingItem = strikingRow.GetNext (pos);
		AddHead(new StrikingItem(*strikingItem));
	} 	

	for (int i=0;i<MAXBELLS;i++)
		_perfectTimeBell[i]	= strikingRow._perfectTimeBell[i];
}
	
StrikingRow::StrikingRow():
_stroke(hand), 
_baseTimestamp(0), 
_speed(0)
{

}

StrikingRow::~StrikingRow(void)
{
	POSITION pos = GetHeadPosition();

	StrikingItem* item = NULL;
	while(pos) 
	{
		item  = (StrikingItem *) GetNext(pos);
		delete item;
	}

	RemoveAll();
}

bool StrikingRow::hasItems()
{
	return GetSize() > 0;
}

Stroke StrikingRow::getStroke() const
{
	return _stroke;
}

void StrikingRow::addGeneratedStrike(int place, int bell, DWORD perfectTimestamp, DWORD actualTimestamp)
{
	ASSERT(place >=0 && place <MAXBELLS);
	ASSERT(bell >=0 && bell <MAXBELLS);
	ASSERT(abs((int)(perfectTimestamp - actualTimestamp)) < _speed);
	
	//first set the perfect time bell	
	if (place >=0 && place <MAXBELLS)
	{
//		TRACE("Setting PLace %d, old Place %d, Bell %d\r\n", place, _perfectTimeBell[place].getBell(), bell);
		_perfectTimeBell[place].setTimestamp((int)perfectTimestamp - (int)_baseTimestamp);
		_perfectTimeBell[place].setBell(bell);
	}

	//now add a real strike to teh list of strikes.
	AddTail(new StrikingItem(bell, actualTimestamp - _baseTimestamp));	
}


void StrikingRow::addRealStrike(int bell, DWORD actualTimestamp)
{
	// add a real strike to teh list of strikes.
	AddTail(new StrikingItem(bell, (int)actualTimestamp - (int)_baseTimestamp));	
}

void StrikingRow::addComment(const CString& comment, DWORD actualTimestamp)
{
	// add a real voice command to teh list of strikes.
	AddTail(new StrikingItem(comment, actualTimestamp - _baseTimestamp));	
}

const StrikingItem& StrikingRow::getPerfectTimeBell(int place) const
{
	ASSERT(place >=0 && place <MAXBELLS);

	if (place >=0 && place <MAXBELLS)
	{
		return _perfectTimeBell[place];
	}

	//error
	return _perfectTimeBell[0];
}

int	StrikingRow::getPlaceFromBell(int bell) const 
{
	for (int place=0;place<MAXBELLS;place++)
	{
		if(_perfectTimeBell[place].getBell() == bell)
			return place;
	}

	ASSERT(FALSE);
	return -1;
}

double StrikingRow::getPercentOut(StrikingItem* strike, int place) const
{
	double percent = 0;

	ASSERT(strike && strike->getBell()>=0);
/*		//for debug
	int tempPlace = 0;
	for (int i=0;i<MAXBELLS;i++)
	{
		const StrikingItem& perfectPlace = getPerfectTimeBell(i);
		if (perfectPlace.getBell() == strike->getBell())
		{
			tempPlace = i;
			break;
		}
	}
	ASSERT(tempPlace == place);

  */
	if (strike && strike->getBell()>=0)
	{
		const StrikingItem& perfectPlace = getPerfectTimeBell(place);
		ASSERT(perfectPlace.getBell() == strike->getBell());
		if (perfectPlace.getBell() == strike->getBell())
		{
			int diff = (int)(strike->getTimestamp() - perfectPlace.getTimestamp());
			ASSERT(diff < 10000000);	  

			percent = (double)diff / (double)_speed;
		}
	} 

	return percent;
}

DWORD StrikingRow::getBaseTimestamp() const
{
	return _baseTimestamp;
}

int StrikingRow::getSpeed()
{
	return _speed;
}

void StrikingRow::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _stroke;
		ar << _baseTimestamp;
		ar << _speed;

		int size = MAXBELLS;
		ar << size;
		for (int i=0;i<size;i++)
			_perfectTimeBell[i].Serialize(ar);

		ar << GetSize();
		for (POSITION pos = GetHeadPosition (); pos != NULL;)
		{
			StrikingItem* strikingItem = GetNext (pos);
			strikingItem->Serialize(ar);
		}     

	}
	else
	{
		ar >> version;  
		int temp;
		ar >> temp;
		_stroke = (Stroke)temp;
		ar >> _baseTimestamp;
		ar >> _speed;

		int size;
		ar >> size;
		ASSERT(size == MAXBELLS);
		size = MAXBELLS;
		for (int i=0;i<size;i++)
			_perfectTimeBell[i].Serialize(ar);

		ar >> size;
		for (int i=0;i<size;i++)
		{
			StrikingItem* strikingItem = new StrikingItem();
			strikingItem->Serialize(ar);
			AddTail(strikingItem);
		}
	}
}