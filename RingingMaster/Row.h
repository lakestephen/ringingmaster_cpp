// Row.h: interface for the Row class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "stroke.h"

#include "Notation.h"
#include "TwoDArray.h"

class MusicResult;


class Row
{
public:
	bool isRounds();
	void swap(int firstIndex);
	bool getIsInCourse();
	void calculateInCourse();
	MusicResult* getMusicResult();
	virtual bool setAsText(CString str, CString& msg, bool forceTenor = false);
	CString getAsTextDiff(Row* prevRow);
	int getRowIndex() {return _rowIndex;}  //todo remove row index - use Method instead now held in whole row.
	void setRowIndex(int rowIndex) {_rowIndex = rowIndex;}
	CString getAsText() const;
	~Row();
	Row(int number, Stroke stroke = back, CString rowText = CString(""));
	Row(const Row & row);
	Row(Row & row, Ints * intArr);

	short & getBellInPlace(int place) const ;
	int getPositionOfBell(int bell) const ;
	Row& Row::operator =(const Row & row)	;
	bool operator ==(const Row & row);
	bool operator !=(const Row & row);
	void setRounds();

	bool isLeadLine() {return _leadLine;}
	bool setLeadLine();
	bool isFalse() {return _false;}
	bool setFalse();
	bool isMusic() {return _musicResult != NULL;}
	bool setMusic(MusicResult* musicResult);
	bool isCommon() {return _common;}
	bool setCommon();
	int getNumber() const {return _number;}

	void setStroke(Stroke stroke) {_stroke = stroke;}
	Stroke getStroke() const {return _stroke;}

	int _number;
	short * _data;

protected:
	bool contains (Ints * intArr, int place);
	bool _false;
	bool _common; //only used in multi block
	bool _inCourse; //for +ve and -ve rows
	MusicResult* _musicResult;
	bool _leadLine;

	Stroke _stroke;
	int _rowIndex;	//todo do we need this?
};



typedef TwoDArray<Row*> RowArray;
typedef CArray<Row*, Row*> Rows;


