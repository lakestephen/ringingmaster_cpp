#pragma once

#include "Stroke.h"


class StrikingItem
{
public:
	StrikingItem(int bell, int timestamp);
	StrikingItem(const CString& comment, int timestamp);
	StrikingItem();
	virtual ~StrikingItem(void);

	int getBell() const;
	void setBell(int bell);

	const CString& getComment() const;
	void setComment(CString comment);

	int getTimestamp() const;
	void setTimestamp(int timestamp);

	void Serialize(CArchive& ar);

	bool isComment();

protected:
	int _timestamp;

	int _bell;
	CString _comment;
};


