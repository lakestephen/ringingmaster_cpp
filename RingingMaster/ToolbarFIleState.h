
#pragma once

#include "ToolbarTypes.h"


enum StateType
{
	st_none = 0,
	st_touch_main, 
	st_touch_simulator,
	st_records,
	st_internet,
	st_image,
	st_crib,
	st_strike,

	st_null, //leve this at the end

};

enum ToolbarFileStateType
{
	tfs_SHOW, 
	tfs_HIDE, 
	tfs_DONTCARE, 
};



class ToolbarFileState  
{
public:
	ToolbarFileState(ToolbarFileStateType initialState = tfs_HIDE);
	virtual ~ToolbarFileState();

	void  setState(ToolbarTypes item, ToolbarFileStateType state);
	ToolbarFileStateType getState(ToolbarTypes type) const;

	void Serialize(CArchive& ar);

private:	
	ToolbarFileStateType _toolbarFileStateType[LAST_CONTROLBAR+1];
};


