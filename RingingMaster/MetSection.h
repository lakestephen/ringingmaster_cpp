// MetSection.h: interface for the MetSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METSECTION_H__69A43567_B357_11D5_B4F3_000255162CF0__INCLUDED_)
#define AFX_METSECTION_H__69A43567_B357_11D5_B4F3_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Met.h"

class MetSection  
{
public:
	void setStr(int index, CString section, CString value);
	CString _strSectionType;
	CString _strSuffex;
	CString getSectionType();
	CString _fileName;
	bool init();
	int getNumber();
	CString getStr(int index, CString token);
	MetSection();
	virtual ~MetSection();

	CString _type;
	Ints _columnNum;
	CStringArray _columnNames;
	Mets _mets; //array of method objects

private:
	int _number;

};


typedef CArray<MetSection*, MetSection*> MetSections;

#endif // !defined(AFX_METSECTION_H__69A43567_B357_11D5_B4F3_000255162CF0__INCLUDED_)
