// Layout.h: interface for the Layout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYOUT_H__C33A8B40_3FFC_11D5_BB7F_A503A2EAB70A__INCLUDED_)
#define AFX_LAYOUT_H__C33A8B40_3FFC_11D5_BB7F_A503A2EAB70A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TouchDoc;

class Layout  
{
public:
	CSize getNotesSize();
	void setNotesSize(CSize notesSize);
	void applyScale(int x, int y);
	void setTitleheight(int titleHeight = 0);
	int getTop(bool rawTop = false);
	Layout(TouchDoc* touchDoc);
	~Layout();

	int getGridLeft();
	int getCourseHeadLeft() {return _left;}

	int _height;
	int _caretHeight;
	int _courseHeadTextWidth;
	int _blockSpace;

	//grid
	COLORREF _gridColor;
	int _gridWidth;

	//text
	COLORREF _CallingPosColor;

protected:
	int _left;
	int _top;
	int _title;
	CSize _notesSize;

	TouchDoc* _touchDoc;

};

#endif // !defined(AFX_LAYOUT_H__C33A8B40_3FFC_11D5_BB7F_A503A2EAB70A__INCLUDED_)
