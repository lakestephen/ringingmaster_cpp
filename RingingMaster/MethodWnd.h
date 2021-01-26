#pragma once

#include "ScrollWnd.h" 
#include "Style.h"	// Added by ClassView
#include "DataTip.h"
#include "Method.h"
#include "RingingMaster.h"

class CxImage;
class MethodPosition;
class RowPosition;
class Row;
class FalseRow;



/////////////////////////////////////////////////////////////////////////////
// MethodWnd window

enum ShouldDrawType
{
	sdt_no, 
	sdt_grey, 
	sdt_yes,
};

enum MethodWndResizeHitTest
{
	MWRHT_LEFT = 100, //higher than HTHELP
	MWRHT_RIGHT,
	MWRHT_TOP,
	MWRHT_BOTTOM,
	MWRHT_NONE,	
};


class MethodWnd : public ScrollWnd
{
// Construction
friend class TouchBlueLineView;	//todo remove this   and all other friend class's

public:
	MethodWnd(defaultStyle type = ds_fromStyleManager);
	virtual ~MethodWnd();
	virtual bool rowHitTest(const CPoint &point, MethodPosition * pos, bool forStatus = false);

	DECLARE_DYNCREATE(MethodWnd)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, Method * method) ;
	//}}AFX_VIRTUAL

// Implementation
public:
	CxImage* getImage();
	virtual int getStyleMenuIndex();
	void setResizePos(CPoint point);
	void doResizeLineInvert();
	CBitmap* getCopyDataBitmap() ;
	int getNumber();
	void scrollToRow(RowPosition rowPosition, bool force = false);
	Row* getHilightRow();
	void hilightRow(const RowPosition& rowPosition);
	void hilightRow(Row* hilightRow);
	void drawOverload(CDC* pDC);

	virtual void recalcLeadOrigins();
	void recalcLeadBoxes();
	void setMethod(Method * method);
	
	void SetScrolls();
	CPoint getContectMouseDown() { return _contextMouseDown;}

	virtual CSize getSize();
	Style _style;

protected:
	virtual ShouldDrawType shouldDrawRow(int rowIndex, int place);
	BOOL isLeadVisible(int lead, CDC* pDC);

	MethodWndResizeHitTest _hitType;
	MethodWndResizeHitTest _resizeType;
	int _resizePos;
	bool _resizing;

	virtual UINT getContectMenuId() 
	{ 
		//should be overridden
		ASSERT(FALSE); 
		return IDR_METHOD_STYLE_DEFAULT;
	}
	void notifyStyleChange(bool flagChange = true);

	
	virtual CSize getLeadOrigin(int leadNo); //todo make const ref
	virtual const CRect& getLeadBox(int leadNo);
	int leadHitTest(const CPoint &point);

	virtual void OnDraw(CDC* pDC);

	void drawNumbers(CDC *pDC);
	void drawLines(CDC *pDC);
	virtual void drawHilightRow(CDC *pDC);
	
	virtual void drawLeadLines(CDC *pDC);
	void drawBellLines(CDC *pDC);
	virtual void drawStarts(CDC *pDC, int max);
	virtual void drawFalseRows(CDC *pDC);
	virtual void drawMusicRows(CDC *pDC);
	void drawNotation(CDC *pDC, const CStringArray* notationStrings, int lead = 0);
	void drawStrokes(CDC *pDC);
	virtual void drawComments(CDC *pDC);
	int drawTitle(CDC *pDC);

	bool _16BitOverload;
	CPoint _contextMouseDown;

	DataTip _tip;



	CArray <CSize,CSize> _leadOrigins;
	CArray <CRect,CRect> _leadBoxs;	
	Row* _highlightRow;
	Method * _method;
	Row* _contextRow;
	FalseRow * _falseRow;

	//{{AFX_MSG(MethodWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnToolsStyle();
	afx_msg void OnLinewidth1();
	afx_msg void OnUpdateLinewidth1(CCmdUI* pCmdUI);
	afx_msg void OnLinewidth2();
	afx_msg void OnUpdateLinewidth2(CCmdUI* pCmdUI);
	afx_msg void OnLinewidth3();
	afx_msg void OnUpdateLinewidth3(CCmdUI* pCmdUI);
	afx_msg void OnLinecolor();
	afx_msg void OnTextcolor();
	afx_msg void OnStyleMenuMore();
	afx_msg void OnStyleMenuSaveAs();
	afx_msg void OnUpdateStyleMenuSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsStyle(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSetEndAtRow();
	afx_msg void OnUpdateSetEndAtRow(CCmdUI* pCmdUI);
	afx_msg void OnSetStartAtRow();
	afx_msg void OnUpdateSetStartAtRow(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFilePrint();
	afx_msg void OnEditFind();
	afx_msg void OnEditGoto();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnMethodOptions();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	virtual void doToolsStyle();


	virtual CString getCopyDataString();

	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateLinecolor(CCmdUI* pCmdUI);
	afx_msg void OnApplyStyle(UINT nID);
	afx_msg LRESULT OnUpdateMethod(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

