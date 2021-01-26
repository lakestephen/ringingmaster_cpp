// ToolbarFIleState.cpp: implementation of the ToolbarFIleState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ToolbarFileState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ToolbarFileState::ToolbarFileState(ToolbarFileStateType initialState)
{
	for (int i = FIRST_TOOLBAR; i <= LAST_CONTROLBAR;i++)
		setState((ToolbarTypes)i, initialState);

}

ToolbarFileState::~ToolbarFileState()
{

}

void  ToolbarFileState::setState(ToolbarTypes item, ToolbarFileStateType state)
{
	_toolbarFileStateType[item] = state;
}

ToolbarFileStateType ToolbarFileState::getState(ToolbarTypes item) const 
{
	return _toolbarFileStateType[item];
}

void ToolbarFileState::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		for (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
			ar << _toolbarFileStateType[i];
	}
	else
	{
		ar >> version;
		int temp;

		for (int i=FIRST_TOOLBAR;i<=LAST_CONTROLBAR;i++)
		{
			ar >> temp;
			_toolbarFileStateType[i] = (ToolbarFileStateType)temp;
		}
	}
}
