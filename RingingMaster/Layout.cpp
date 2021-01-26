// Layout.cpp: implementation of the Layout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Layout.h"
#include "TouchDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Layout::Layout(TouchDoc* touchDoc) :
_touchDoc(touchDoc),
_notesSize(0,0)
{
	_left = 10;
	_top = 13;
	_title = 0;
	_height = 28;
	_caretHeight = 15;

	_gridColor = RGB(210,210,210);
	_gridWidth = 1;

	_CallingPosColor = RGB(0,255,0);
	
	_courseHeadTextWidth = 0;
	_blockSpace = 3;
}

Layout::~Layout()
{

}

int Layout::getTop(bool rawTop)
{
	if ((rawTop)||(_title <= 0))
		return _top;
	else 
		return  (2*_top) + _title;
}

void Layout::setTitleheight(int titleHeight)
{
	_title = titleHeight;
}

int Layout::getGridLeft() 
{
	if (_touchDoc->getShowCourseHeads())
		return (_left * 2)+ _courseHeadTextWidth;
	else
		return _left;
}


void Layout::applyScale(int x, int y)
{
	_left *= x;
	_top *= y;
	_gridWidth *= x;
}

void Layout::setNotesSize(CSize notesSize)
{
	_notesSize = notesSize;
}

CSize Layout::getNotesSize()
{
	return _notesSize;

}
