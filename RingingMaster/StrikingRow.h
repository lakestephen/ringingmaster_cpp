#pragma once


#include "Stroke.h"
#include "StrikingItem.h"

class Row;



class StrikingRow : public CList<StrikingItem*, StrikingItem*> 
{
public:
	StrikingRow(Stroke stroke, DWORD baseTimestamp, int speed, const Row& row);
	StrikingRow(const StrikingRow& strikingRow);
	StrikingRow();

	~StrikingRow(void);

	void addGeneratedStrike(int place, int bell, DWORD perfectTimestamp, DWORD actualTimestamp);
	void addRealStrike(int bell, DWORD actualTimestamp);
	void addComment(const CString& comment, DWORD actualTimestamp);
	bool hasItems();
	Stroke getStroke() const;

	const StrikingItem& getPerfectTimeBell(int place) const;
	double getPercentOut(StrikingItem* strike, int place) const;
	DWORD getBaseTimestamp() const;
	int getSpeed();
	int	getPlaceFromBell(int bell) const;
	
	void Serialize(CArchive& ar);
protected:	
	Stroke _stroke;
	DWORD _baseTimestamp;
	int _speed;

	StrikingItem _perfectTimeBell[MAXBELLS]; //this holds the perfect time, and the bell in each place 
};
