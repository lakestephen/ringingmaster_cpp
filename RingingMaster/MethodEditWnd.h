// MethodEditWnd.h: interface for the MethodEditWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MethodEditWND_H__C8A7C727_66D0_11D4_B4E6_009027BB3286__INCLUDED_)
#define AFX_MethodEditWND_H__C8A7C727_66D0_11D4_B4E6_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScrollWnd.h"
#include "Notation.h"	// Added by ClassView
#include "Style.h"	// Added by ClassView

class MethodEditDlg;

enum lineType
{
	none = 1000,
	left = -1,
	down =  0,
	right = 1 
};

struct MethodEditPos 
{
public:
	MethodEditPos() :
		_place(-1),
		_bell(-1),
		_row(-1)
		{;};
	~MethodEditPos() {;};

	int _row;
	int _place;
	int _bell;
};


class MethodEditWnd : public ScrollWnd  
{
	friend class MethodEditDlg;

public:
	void setBlankStartSize(int size);
	int _blankStartSize;
	MethodEditPos getMethodEditPos();
	bool createMethod(int number, CString notation = "");
	CSize getPos(CPoint &point);
	DECLARE_DYNAMIC(MethodEditWnd)

	MethodEditWnd();
	virtual ~MethodEditWnd();
  
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodEditWnd)
public:
	//}}AFX_VIRTUAL
	
	
protected:
	void drawNotation(CDC * pDC);
	void setStart(const CSize & start);
	MethodEditPos _methodEditPos; 
	CPoint _contextMouseDown;

	CString calcNotation();
	void checkClashes();
	CSize _addStart;
	void cascade();
	class Line
	{
	public:
		Line() 
		{
			_type = none;
			_number = NOBELL;
		}
		
		lineType _type;
		int _number;
	};

	struct Undo
	{
		Line _was;
		CSize _pos;
		bool _last;
	};

	void addLine(lineType type = none, bool last = false);
	void validateLine(const CPoint &point);
	MethodEditDlg * GetParent();
	int getLineCount();
	bool isValid();
	void removeRow();
	void addRow();
	int getHeight() {return _height;}
	int getNumber() { return _width;}
	CSize _drawStart;
	int _drawLength;
	lineType _drawType;
	
	void drawInvertLine(CDC &dc);
	CPoint _mouseDown;
	bool _drawing;
	int _width;
	int _height;
	Line * getLine(CSize &pos) { return getLine(pos.cx, pos.cy);}
	Line * getLine(int x, int y);
	Line * getLineIn(const CSize &pos); 
	Line * getLineIn(int x, int y) { return getLineIn(CSize(x,y));}
	void destroyLines();
	void centre();
	void createLines(int width, int height);
	void drawNumbers(CDC * pDC);
	void drawGrid(CDC * pDC);
	void drawLines(CDC * pDC);
	CArray<Line*, Line*> _lines;
	CArray<Undo, Undo> _undo;
	void updateDimentions();
	bool isOutOfBounds(const CSize & pos);
	bool isOutOfBounds(const CPoint & pos);
	CSize pointToPos(const CPoint &point);
	CPoint posToPoint(const CSize &pos, bool incScroll = true);
	CPoint snapToGrid(const CPoint &point);
	void initLines();


	int _left;
	int _top;
	Style _style;

	enum dimension
	{
		noBorder, 
		styleBorder, 
		actualBorder
	};								  
	CSize getDimensions(dimension dim) ;

	DECLARE_MESSAGE_MAP()
	
	void OnDraw(CDC* pDC);


	// Generated message map functions
	//{{AFX_MSG(MethodEditWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPopupAddworkSingledodge();
	afx_msg void OnPopupAddworkDoubleDodge();
	afx_msg void OnPopupAddworkPlacesdown();
	afx_msg void OnPopupAddworkPlacesup();
	afx_msg void OnPopupAddworkYorkshiredown();
	afx_msg void OnPopupAddworkYorkshireup();
	afx_msg void OnPopupAddworkCambridgedown();
	afx_msg void OnPopupAddworkCambridgeup();
	afx_msg void OnUpdatePopupAddwork(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPopupDelete();
	afx_msg void OnUpdatePopupDelete(CCmdUI* pCmdUI);
	afx_msg void OnMethodDrawClearAll();
	afx_msg void OnUpdateMethodDrawClearAll(CCmdUI* pCmdUI);
	afx_msg void OnMethodDrawStyle();
	afx_msg void OnMethodDrawUndo();
	afx_msg void OnUpdateMethodDrawUndo(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMethod(WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_MethodEditWND_H__C8A7C727_66D0_11D4_B4E6_009027BB3286__INCLUDED_)
