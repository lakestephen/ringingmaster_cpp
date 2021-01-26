// MatrixFilterDescriptionItem.cpp: implementation of the MatrixFilterDescriptionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixFilterDescriptionItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixFilterDescriptionItem::MatrixFilterDescriptionItem(CString text, MatrixFilterDescriptionItemType type) :
_text(text),
_type(type)
{

}


MatrixFilterDescriptionItem::~MatrixFilterDescriptionItem()
{

}

CString MatrixFilterDescriptionItem::getText()
{
	return _text;
}

bool MatrixFilterDescriptionItem::isFilterCondition()
{
	return _type == mfdit_filterCondition;
}

bool MatrixFilterDescriptionItem::isSortCondition()
{
	return _type == mfdit_sortCondition;
}

bool MatrixFilterDescriptionItem::isColumnCondition()
{
	return _type == mfdit_columnCondition;
}

bool MatrixFilterDescriptionItem::isLink()
{
	return ((_type == mfdit_filterCondition)||
		    (_type == mfdit_sortCondition)||
		    (_type == mfdit_columnCondition)||
			( _type == mfdit_logic)); //logic is link also
}

bool MatrixFilterDescriptionItem::isLogic()
{
	return _type == mfdit_logic;
}

bool MatrixFilterDescriptionItem::isPost()
{
	return _type == mfdit_post;
}

void MatrixFilterDescriptionItem::setPositions(int left, int right)
{
	_left = left;
	_right = right;
}

int MatrixFilterDescriptionItem::getLeft()
{
	return _left;
}

int MatrixFilterDescriptionItem::getRight()
{
	return _right;
}

void MatrixFilterDescriptionItem::setText(CString text)
{
	_text = text;
}
