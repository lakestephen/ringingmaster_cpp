#pragma once

#include "ListTS.h"

class StrikingItem;
class StrikingRow;
class Row;

#include "Stroke.h"


#define STATS_GRAPH_RANGE 100
#define LATE_EARLY_PERCENT 5

struct AllBellsStrikingDataStatsStroke
{
	double _allBellsError;
	int _rowCount;
};

struct AllBellsStrikingDataStats
{
	AllBellsStrikingDataStatsStroke _hand;
	AllBellsStrikingDataStatsStroke _back;
	AllBellsStrikingDataStatsStroke _handback;
};

struct StrikingDataStatsStroke
{
	int _graphVals[STATS_GRAPH_RANGE];

	double _meanPercentError;
	double _standardDeviationPercent;
	int _strikeCount;
	int _lateStrikesCount;
	int _earlyStrikesCount;

};

struct StrikingDataStats
{
	StrikingDataStatsStroke _hand;
	StrikingDataStatsStroke _back;
	StrikingDataStatsStroke _handback;
};

struct StrikingPendingComment
{
	DWORD _timestamp;
	CString _comment;
};


#define LATEST_ROWS_SIZE 3

class StrikingData : protected CListTS<StrikingRow*, StrikingRow*>
{
public:
	StrikingData(void);
	StrikingData(const StrikingData& strikingData);
	void operator =(const StrikingData& strikingData);
	virtual ~StrikingData(void);

	void startNewRow(Stroke stroke, DWORD timestamp, int speed, const Row& row, int number);
	void addGeneratedStrike(int place, int bell, Stroke stroke, DWORD perfectTimestamp, DWORD actualTimestamp);
	void addRealStrike(int bell, DWORD actualTimestamp);
	void addSpeaking(CString comment, DWORD actualTimestamp);
	void reset(int number);

	bool canCreateReport();
	
	void getLatestRow(StrikingRow*& latestRow, StrikingRow*& latestButOneRow) const;
	
	CCriticalSection& getCriticalSection() const;
	
	void Serialize(CArchive& ar);

	//overrides to allow some access to the list, without opening it up compleatly
	POSITION GetHeadPosition() const
	{
		return CListTS<StrikingRow*, StrikingRow*>::GetHeadPosition();
	}
	StrikingRow* const& GetNext(POSITION& rPosition) const 
	{
		return CListTS<StrikingRow*, StrikingRow*>::GetNext(rPosition);
	}
	INT_PTR GetSize() const
	{
		return CListTS<StrikingRow*, StrikingRow*>::GetSize();
	}

	int getNumber() const;

	DWORD getBaseTimestamp() const;
	
	int getUniqueId() const;
	void createNewUniqueId();

	void calculateStats();
	const StrikingDataStats& getStats(int bell) const;
	const AllBellsStrikingDataStats& getAllBellsStats() const;



protected:
	void calculateStats(int bell, StrikingDataStats& stats) ;
	CArray<StrikingPendingComment*, StrikingPendingComment*> _pendingComments;
	StrikingRow* _latestRow[LATEST_ROWS_SIZE];
	AllBellsStrikingDataStats _allBellsStrikingDataStats;
	StrikingDataStats _stats[MAXBELLS];
	int _number;
	DWORD _baseTimestamp;
	int _uniqueId;
};
