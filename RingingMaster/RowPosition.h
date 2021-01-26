// RowPosition.h: interface for the RowPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWPOSITION_H__64D50AB2_C722_4BB6_8D8A_412E48C5C929__INCLUDED_)
#define AFX_ROWPOSITION_H__64D50AB2_C722_4BB6_8D8A_412E48C5C929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalDataManager.h"
class Method;

enum RowPositionFormatType
{
	rpft_short,
	rpft_full,
	rpft_lead,
	rpft_rowDisplay,
	//used in derived types
	rpft_mp_place,
	rpft_mp_bell,
	rpft_mp_stroke,
};

enum RowPositionValidityState
{
	rpvs_invalid,
	rpvs_valid,
	rpvs_leadRowSet,
	rpvs_methodRowSet
};

class RowPosition  
{
public:
	static int compare( const void * arg1, const void * arg2 );

	virtual bool decrementRow(Method* method);
	virtual bool incrementRow(Method* method);
	virtual bool verify(Method* method, bool& didRollOver);
	bool isValid(bool relaxedValidity = false) const;
	int getRowDisplay(bool relaxedValidity = false) const;
	int getLeadRow(bool relaxedValidity = false) const;
	int getMethodRow(bool relaxedValidity = false) const;
	int getLead(bool relaxedValidity = false) const;
	virtual void setInvalid();
	RowPosition();
	virtual bool setRow(const Method* method, int methodRow);
	void setRow(int lead, int leadRow);
	virtual bool setRow(const Method* method, int lead, int leadRow, bool rollOver = true);
	virtual ~RowPosition();

	virtual CString format(RowPositionFormatType type);

	bool operator !=(const RowPosition & rowPosition);
	bool operator ==(const RowPosition & rowPosition);

protected:

	int _methodRow;	//numbered from 0 to the number of rows in method.
	int _leadRow;		//numbered from 0 to the number of rows in _lead.
	int _lead;		//numbered from 0 to the number of leads in method.
	RowPositionValidityState _valid;
};

typedef CArray<RowPosition*, RowPosition*> RowPositions;

#endif // !defined(AFX_ROWPOSITION_H__64D50AB2_C722_4BB6_8D8A_412E48C5C929__INCLUDED_)
