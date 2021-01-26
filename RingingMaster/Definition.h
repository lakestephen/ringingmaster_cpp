// Definitions.h: interface for the Definitions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFINITIONS_H__66609E17_7AA8_11D5_B60A_009027BB3286__INCLUDED_)
#define AFX_DEFINITIONS_H__66609E17_7AA8_11D5_B60A_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CellData.h"

class TouchDoc;


enum DefinitionType
{
	dt_call,
	dt_splice,
};

class Definition  
{
public:
	CellData* getCellData();
	void create();
	void setDefinition(const CString definition);
	void setName(const CString& name);
	CString getDefinition();
	CString getName(bool includeType = false);
	Definition(TouchDoc* touchDoc);
	virtual ~Definition();
	void Serialize(CArchive &ar);
	void setType(DefinitionType type);
	DefinitionType getType();

protected:

	CString _name;
	CString _definition;
	DefinitionType _type;
	CellData _cellData;
	TouchDoc* _touchDoc;
};

typedef CArray<Definition*, Definition*> Definitions;


#endif // !defined(AFX_DEFINITIONS_H__66609E17_7AA8_11D5_B60A_009027BB3286__INCLUDED_)
