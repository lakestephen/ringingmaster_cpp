// MatrixFilterDescriptionItem.h: interface for the MatrixFilterDescriptionItem class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum MatrixFilterDescriptionItemType
{
	mfdit_pre,
	mfdit_filterCondition,
	mfdit_sortCondition,
	mfdit_columnCondition,
	mfdit_post, 
	mfdit_logic,
};

class MatrixFilterDescriptionItem  
{
public:
	void setText(CString text);
	int getRight();
	int getLeft();
	void setPositions(int left, int right);
	bool isLink();
	bool isSortCondition();
	bool isFilterCondition();
	bool isColumnCondition();
	bool isLogic();
	bool isPost();
	CString getText();
	MatrixFilterDescriptionItem(CString text, MatrixFilterDescriptionItemType type);
	virtual ~MatrixFilterDescriptionItem();

protected:
	int _left;
	int _right;
	CString _text;
	MatrixFilterDescriptionItemType _type;
};



