#pragma once

class Row;


class Lead : public CArray<Row*, Row*>
{
public:
	Lead();
	virtual ~Lead();

	int getHuntCount() const;

};
