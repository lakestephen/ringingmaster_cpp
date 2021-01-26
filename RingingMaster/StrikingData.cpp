#include "StdAfx.h"
#include "strikingdata.h"
#include "strikingitem.h"
#include "StrikingRow.h"
#include "Row.h"
#include "Method.h"
#include "UniqueIDGenerator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


StrikingData::StrikingData(void) : 
_number(0), 
_baseTimestamp(0), 
_uniqueId(UniqueIDGenerator::getNextId())
{	   
	reset(-1);
}

StrikingData::StrikingData(const StrikingData& strikingData)
{
	operator =(strikingData);
}

void StrikingData::createNewUniqueId()
{
	_uniqueId = UniqueIDGenerator::getNextId();
}

void StrikingData::operator =(const StrikingData& strikingData)
{
	CSingleLock lock(&_criticalSection, TRUE);

	reset(strikingData.getNumber());

	_baseTimestamp = strikingData._baseTimestamp;
	_uniqueId = strikingData._uniqueId;

	for (POSITION pos = strikingData.GetHeadPosition (); pos != NULL;)
	{
		const StrikingRow* strikingRow = strikingData.GetNext (pos);
		AddTail(new StrikingRow(*strikingRow));
	}                                          
}


StrikingData::~StrikingData(void)
{
	reset(-1);
}

void StrikingData::startNewRow(Stroke stroke, DWORD timestamp, int speed, const Row& row, int number)
{
	CSingleLock lock(&_criticalSection, TRUE);

	if (_number < number) 
		_number = number;
	
	// move everything down
	for (int i=LATEST_ROWS_SIZE-2;i>=0;i--)
	{
		StrikingRow* temp = _latestRow[i];
		_latestRow[i] = _latestRow[i+1];
		_latestRow[i+1] = temp;
	}
	                        
	//rebase the time if there are any early timestamps.	
	for (int i=0;i<_pendingComments.GetSize();i++)
	{
		if (timestamp >= _pendingComments.GetAt(i)->_timestamp)
			timestamp = _pendingComments.GetAt(i)->_timestamp-1;
	}

	_latestRow[0] = new StrikingRow(stroke, timestamp, speed, row);

	//add in the pending comments;
	for (int i=0;i<_pendingComments.GetSize();i++)
	{
		StrikingPendingComment* pendingComment = _pendingComments.GetAt(i);
		_latestRow[0]->addComment(pendingComment->_comment, pendingComment->_timestamp);
		delete pendingComment;
	}
	_pendingComments.RemoveAll();

	if (GetSize() == 0)
	{
		_baseTimestamp = timestamp;
		TRACE("Setting Striking Data base timestamp: %d\r\n", _baseTimestamp);
	}


	AddTail(_latestRow[0]);

}
 

void StrikingData::addGeneratedStrike(int place, int bell, Stroke stroke, DWORD perfectTimestamp, DWORD actualTimestamp)
{
	CSingleLock lock(&_criticalSection, TRUE);

	StrikingRow* correctStrokeRow = NULL;

	if ((_latestRow[0])&&(stroke == _latestRow[0]->getStroke()))
		correctStrokeRow = _latestRow[0];
	else
		correctStrokeRow = _latestRow[1];
	
	ASSERT(correctStrokeRow);

	if (correctStrokeRow)
		correctStrokeRow->addGeneratedStrike(place, bell, perfectTimestamp, actualTimestamp);	
}

void StrikingData::addSpeaking(CString comment, DWORD actualTimestamp)
{
	if (comment.IsEmpty())
	{
		return;
	}
 	CSingleLock lock(&_criticalSection, TRUE);

	StrikingRow* latestRow;

	if ((_latestRow[0])&&(actualTimestamp > _latestRow[0]->getBaseTimestamp() + (_latestRow[0]->getSpeed()/2)))
		latestRow = _latestRow[0];
	else
		latestRow = _latestRow[1];

//	ASSERT(latestRow);

	if (latestRow)
		latestRow->addComment(comment, actualTimestamp);	
	else
	{
		TRACE("_pendingComments.Add");
		StrikingPendingComment* pendingComment = new StrikingPendingComment;
		pendingComment->_timestamp = actualTimestamp;
		pendingComment->_comment = comment ;
		_pendingComments.Add(pendingComment);
	}

}

void StrikingData::addRealStrike(int bell, DWORD actualTimestamp)
{
	CSingleLock lock(&_criticalSection, TRUE);

	StrikingRow* latestRow;

	if ((_latestRow[0])&&(actualTimestamp > _latestRow[0]->getBaseTimestamp() + (_latestRow[0]->getSpeed()/2)))
		latestRow = _latestRow[0];
	else
		latestRow = _latestRow[1];

//	ASSERT(latestRow);

	if (latestRow)
		latestRow->addRealStrike(bell, actualTimestamp);	
}

void StrikingData::reset(int number)
{	
	CSingleLock lock(&_criticalSection, TRUE);

	//iterate through and remove all items
	while (GetSize() >0)
	{
		StrikingRow* row = RemoveHead();
		delete row;   		
	}

	for (int i=0;i<LATEST_ROWS_SIZE;i++)
		_latestRow[i] = NULL;

	for (int i=0;i<_pendingComments.GetSize();i++)
	{
		delete _pendingComments.GetAt(i);
	}
	_pendingComments.RemoveAll();


	_number = number;
}

void StrikingData::getLatestRow(StrikingRow*& latestRow, StrikingRow*& latestButOneRow) const
{
	CSingleLock lock(&_criticalSection, TRUE);

	if (_latestRow[0] && _latestRow[0]->hasItems())
	{
		latestRow = _latestRow[0];	 
		latestButOneRow = _latestRow[1];	 
	}
	else
	{
		latestRow = _latestRow[1];	 
		latestButOneRow = _latestRow[2];	 
	}
}

CCriticalSection& StrikingData::getCriticalSection()	const
{
   return _criticalSection;
}

bool StrikingData::canCreateReport()
{
	return GetSize() > 0;
}

void StrikingData::Serialize(CArchive& ar)
{
	CSingleLock lock(&_criticalSection, TRUE);

	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _number;
		ar << _baseTimestamp;
		ar << GetSize();
		for (POSITION pos = GetHeadPosition (); pos != NULL;)
		{
			StrikingRow* strikingRow = GetNext (pos);
			strikingRow->Serialize(ar);
		}     
	}
	else
	{
		ar >> version;  
		ar >> _number;
		ar >> _baseTimestamp;
		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			StrikingRow* strikingRow = new StrikingRow();
			strikingRow->Serialize(ar);
			AddTail(strikingRow);
		}
	}
}


int StrikingData::getNumber() const
{
	CSingleLock lock(&_criticalSection, TRUE);

	return _number;
}

DWORD StrikingData::getBaseTimestamp() const
{
	return _baseTimestamp;
}

int StrikingData::getUniqueId() const 
{
	return _uniqueId;	
}

void StrikingData::calculateStats() 
{
	memset(&_allBellsStrikingDataStats, 0, sizeof(_allBellsStrikingDataStats));

	for ( POSITION pos = GetHeadPosition (); pos != NULL; )
	{
		const StrikingRow* strikingRow = GetNext (pos);

		_allBellsStrikingDataStats._handback._rowCount++;
		if (strikingRow->getStroke() == hand)
		{
			_allBellsStrikingDataStats._hand._rowCount++;
		}
		else
		{
			_allBellsStrikingDataStats._back._rowCount++;
		}
	}

	for (int i=0;i<getNumber();i++)
	{
		calculateStats(i, _stats[i]);
		_allBellsStrikingDataStats._hand._allBellsError+= _stats[i]._hand._meanPercentError;
		_allBellsStrikingDataStats._back._allBellsError+= _stats[i]._back._meanPercentError;
		_allBellsStrikingDataStats._handback._allBellsError += _stats[i]._handback._meanPercentError;
	}

	_allBellsStrikingDataStats._hand._allBellsError /= getNumber();
	_allBellsStrikingDataStats._back._allBellsError /= getNumber();
	_allBellsStrikingDataStats._handback._allBellsError /= getNumber();

}

void StrikingData::calculateStats(int bell, StrikingDataStats& stats) 
{
	memset(&stats, 0, sizeof(stats));

	//to2do the stroke should cone form the strike, rather than thge row.

	for ( POSITION pos = GetHeadPosition (); pos != NULL; )
	{
		const StrikingRow* strikingRow = GetNext (pos);

		POSITION rowPos = strikingRow->GetHeadPosition();
		StrikingItem* realStrikeItem = NULL;

		while(rowPos) 
		{
			realStrikeItem  = (StrikingItem *) strikingRow->GetNext(rowPos);
			if (!realStrikeItem->isComment() && 
				realStrikeItem->getBell() == bell)
			{
				int place = strikingRow->getPlaceFromBell(realStrikeItem->getBell());

				if (place >= 0)
				{
					double percent = strikingRow->getPercentOut(realStrikeItem, place);

					if (percent*100 > LATE_EARLY_PERCENT)
						stats._handback._lateStrikesCount++;
					if (percent*100 < - LATE_EARLY_PERCENT)
						stats._handback._earlyStrikesCount++;

					//graph calculation
					int iPercent = (int)floor(percent*100) + (STATS_GRAPH_RANGE/2);

					stats._handback._strikeCount++;
					stats._handback._meanPercentError += percent;
					if (iPercent >=0 && iPercent <= STATS_GRAPH_RANGE)
						stats._handback._graphVals[iPercent] ++;

					if (strikingRow->getStroke() == hand)
					{
						stats._hand._meanPercentError += percent;
						stats._hand._strikeCount++;
						if (iPercent >=0 && iPercent <= STATS_GRAPH_RANGE)
							stats._hand._graphVals[iPercent] ++;
						
						if (percent*100 > LATE_EARLY_PERCENT)
							stats._hand._lateStrikesCount++;
						if (percent*100 < - LATE_EARLY_PERCENT)
							stats._hand._earlyStrikesCount++;
					}
					else
					{
						stats._back._meanPercentError += percent;
						stats._back._strikeCount++;
						if (iPercent >=0 && iPercent <= STATS_GRAPH_RANGE)
							stats._back._graphVals[iPercent] ++;

						if (percent*100 > LATE_EARLY_PERCENT)
							stats._back._lateStrikesCount++;
						if (percent*100 < - LATE_EARLY_PERCENT)
							stats._back._earlyStrikesCount++;
					}
				}                      
			}
		}
	}	

	//mean percent error
	stats._hand._meanPercentError		/= stats._hand._strikeCount;
	stats._back._meanPercentError		/= stats._back._strikeCount;;
	stats._handback._meanPercentError	/= stats._handback._strikeCount;

	//now calculate stddev
	double sumHand = 0.0;
	double sumBack = 0.0;
	double sumHandBack = 0.0;
	for ( POSITION pos = GetHeadPosition (); pos != NULL; )
	{
		const StrikingRow* strikingRow = GetNext (pos);

		POSITION rowPos = strikingRow->GetHeadPosition();
		StrikingItem* realStrikeItem = NULL;

		while(rowPos) 
		{
			realStrikeItem  = (StrikingItem *) strikingRow->GetNext(rowPos);
			if (realStrikeItem->getBell() == bell)
			{
				int place = strikingRow->getPlaceFromBell(realStrikeItem->getBell());

				if (place >= 0)
				{
					double percent = strikingRow->getPercentOut(realStrikeItem, place);
					double val = ((stats._handback._meanPercentError - percent) * (stats._handback._meanPercentError - percent));
					
					sumHandBack += val;
					if (strikingRow->getStroke() == hand)
					{
						sumHand += val;
					}
					else if (strikingRow->getStroke() == back)
					{
						sumBack += val;
					}
				}
			}
		}
	}

	stats._hand._standardDeviationPercent = sqrt(sumHand/(double)(stats._hand._strikeCount-1));
	stats._back._standardDeviationPercent = sqrt(sumBack/(double)(stats._back._strikeCount-1));
	stats._handback._standardDeviationPercent = sqrt(sumHandBack/(double)(stats._handback._strikeCount-1));

}

const StrikingDataStats& StrikingData::getStats(int bell) const
{
	ASSERT(bell >= 0 && bell < MAXBELLS);

	if (bell >= 0 && bell < MAXBELLS)
	{
		return _stats[bell];
	}

	return _stats[0];
}

const AllBellsStrikingDataStats& StrikingData::getAllBellsStats() const
{
	return _allBellsStrikingDataStats;
}
