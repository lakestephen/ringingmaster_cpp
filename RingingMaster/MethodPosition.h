// MethodPosition.h: interface for the MethodPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METHODPOSITION_H__987D2CD4_4DB5_11D5_B5D8_009027BB3286__INCLUDED_)
#define AFX_METHODPOSITION_H__987D2CD4_4DB5_11D5_B5D8_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
#include "RingingMaster.h"

#include "RowPosition.h"

class MethodPosition : public RowPosition
{
public:
	int getLeadLine();
	void setStroke(Stroke stroke);
	void setBell(int bell);
	void setPlace(int place);
	void setLeadLine(int leadLine);
	Stroke getStroke();
	int getBell();
	int getPlace();
	MethodPosition();
	~MethodPosition() {}
	
	void setInvalid();
	
	bool setRow(Method* method, int lead, int leadRow, bool rollOver = true);
	
	CString format(RowPositionFormatType type);

protected:

	int _place;
	int _bell;
	int _leadLine;
	Stroke _stroke;

};


#endif // !defined(AFX_METHODPOSITION_H__987D2CD4_4DB5_11D5_B5D8_009027BB3286__INCLUDED_)
