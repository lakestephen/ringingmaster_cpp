#if !defined(AFX_TouchView_H__34F12E25_115F_11D5_B595_009027BB3286__INCLUDED_)
#define AFX_TouchView_H__34F12E25_115F_11D5_B595_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TouchView.h : header file
//

#include "TouchDoc.h"
#include "DataTip.h"
#include "ColorListBox.h"
#include "CPage.h"
#include "ScrollView32Bit.h"


/////////////////////////////////////////////////////////////////////////////
// TouchView view

class BlockPos
{
public:
	enum bpType
	{
		BP_NONE,
		BP_NODE,
		BP_CHAR,
		BP_INBETWEEN,
		BP_WHOLE_CELL,
		BP_BRACKET
	} _type;

	BlockPos(bpType type, int line) :
	_type(type),
	_line(line)
	{ }
	

	int _line;
};


class TouchView : public CScrollView32Bit
{
	enum ScreenPos
	{
		SP_NONE,
		SP_GRID,
		SP_BLOCKS,
		SP_DEFINITIONS
	};

	struct TouchClipInfo
	{
		GridPos _start;		
		GridPos _end;
		

		int _width;
		int _height;
	};					

protected:
	TouchView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TouchView)

// Attributes
public:

	TouchDoc* GetDocument();

// Operations
public:
	CxImage* getImage();
	void resetExtSelValid();
	void newVarianceOnChar();
	void editVarianceOnChar();
	void doFind(CString findText, bool searchDown);
	void doDraw(CDC* pDC);

	void cleanPopupInfo();
	BlockPos blocksHitTest(const CPoint& point, bool inBetween = false, bool wholeCell = false);
	CRect getGridRect();
	CPoint drawBracket(CDC *pDC, CPoint start, int width, int height);
	void drawTouchBlocks(CDC* pDC);
	void drawDefinitions(CDC* pDC);
	CSize drawNotes(CDC *pDC);

	ScreenPos screenHitTest(const CPoint& point);
	void doPaste(const TouchClipInfo& clipInfo, const CString& clipStr);
	bool isInSelection(const GridPos& gridPos);
	CString getCopyData(bool forData, TouchClipInfo& clipInfo);
	CBitmap* getCopyDataBitmap();
	bool doEditCopy();
	void updateExtSelToCursor();
	void startExtSel(const GridPos& startGrid);
	void terminateExtSel(bool caretToStart);
	void normaliseGridPair(GridPos& start, GridPos& end);
	int evaluateGridPos(const GridPos& first, const GridPos& last);
	void scrollToPos(GridPos gridPos = GridPos(-1,-1));
	CSize getSize(bool includeNotes = true);
	void calculateFontSizes();
	void drawGridText(CDC *pDC);
	void drawCourseHeads(CDC *pDC);
	void drawGridLines(CDC *pDC);
	void drawExtSelection(CDC* pDC);
	void updateCaret(bool bScrollToPos = true);

	void doPrintTouchView(CDC* pDC, CPrintInfo* pInfo);
	double printTitle(CPage* page, double nextRow);
	double printTable(CPage* page, double nextLine);
	double printDefinitions(CPage* page, double nextline);
	double printNotes(CPage* page, double nextline);

	//my version of this 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	static CString _ClipboardTokenData;
	static CString _ClipboardTokenInfo;

	int getBlockDefMin(TouchBlock* tb);
	int getBlockDefMax(TouchBlock* tb);
	static const UINT _findDialogMessage;
	CFindReplaceDialog *_pFindDialog;

	int _titleWidth;
	int drawTitle(CDC *pDC);

	COLORREF getTextColor(ParseType pt);
	int _gridFontAscent;
	int _averageCharWidth;
	void drawUnderLine(CDC * pDC);
	void underline(CDC *pDC, CPoint start, int length);
	CellData* getCellData(const GridPos& gridPos);
	void setScrolls();
	CPoint gridToClient(const GridPos& gridPos, bool extSel = false);
	GridPos gridHitTest(const CPoint& pos, bool noRounding = false);
	CString _searchStr;

	enum PopupAction
	{
		PA_NONE,
		PA_NEW_VAR_LEFT,
		PA_EDIT_VAR
	} _popupAction;

	StringColors _popupStrings;
	CellElement* _popupElement;
	
	GridPos _extSelStart;
	GridPos _extSelEnd;
	bool   _extSelValid;
	bool   _extSelInMouse;
	bool   _dragging;
	bool   _resizeBlock;
	bool _moveBlock;
	Layout* _layout;
	TouchBlock* _startTouchBlock;
	TouchBlock* _endTouchBlock;
	int _blockOffset;
	CPoint _contextMouseDown;
	DataTip _tip;
	GridPos _contextGridPos;
	CSize _statSize;

	virtual ~TouchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(TouchView)
	afx_msg void OnEditWordLeft();
	afx_msg void OnEditWordRight();
	afx_msg void OnEditLineDown();
	afx_msg void OnEditLineUp();
	afx_msg void OnEditCharLeft();
	afx_msg void OnEditCharRight();
	afx_msg void OnEditTab();
	afx_msg void OnEditUntab();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditDelete();
	afx_msg void OnEditDeleteBack();
	afx_msg void OnEditLineHome();
	afx_msg void OnEditLineEnd();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEditDeleteWord();
	afx_msg void OnEditDeleteWordBack();
	afx_msg void OnEditTextHome();
	afx_msg void OnEditTextEnd();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnEditScrollDown();
	afx_msg void OnEditScrollUp();
	afx_msg void OnEditPageDown();
	afx_msg void OnEditPageUp();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditExtCharLeft();
	afx_msg void OnEditExtCharRight();
	afx_msg void OnEditExtLineDown();
	afx_msg void OnEditExtLineUp();
	afx_msg void OnEditExtPageDown();
	afx_msg void OnEditExtPageUp();
	afx_msg void OnEditExtWordLeft();
	afx_msg void OnEditExtWordRight();
	afx_msg void OnEditExtLineEnd();
	afx_msg void OnEditExtLineHome();
	afx_msg void OnEditExtTextEnd();
	afx_msg void OnEditExtTextHome();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditSwitchOvrmode();
	afx_msg void OnUpdateShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnShowGrid();
	afx_msg void OnDeleteColumn();
	afx_msg void OnUpdateDeleteColumn(CCmdUI* pCmdUI);
	afx_msg void OnDeleteRow();
	afx_msg void OnUpdateDeleteRow(CCmdUI* pCmdUI);
	afx_msg void OnInsertColumn();
	afx_msg void OnInsertRow();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnAddblockdefinition();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDeleteblock();
	afx_msg void OnUpdateDeleteblock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddblockdefinition(CCmdUI* pCmdUI);
	afx_msg void OnAddblockdefinitionMenu();
	afx_msg void OnUpdateAddblockdefinitionMenu(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditblockletter();
	afx_msg void OnUpdateEditblockletter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditVariance(CCmdUI* pCmdUI);
	afx_msg void OnEditVariance();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowCourseHeads();
	afx_msg void OnUpdateShowCourseHeads(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrevious();
	afx_msg void OnTouchAddvariance();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnPrintAnalysis();
	afx_msg void OnPopupPrintAnalysis();

	//}}AFX_MSG
	afx_msg LRESULT OnPopupListTerm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindDialogMessage(WPARAM wParam, LPARAM lParam);		

	DECLARE_MESSAGE_MAP()


private:

	bool _initedScrols;
	
};


#ifndef _DEBUG  // debug version in TouchView.cpp
inline TouchDoc * TouchView::GetDocument()
   { return (TouchDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TouchView_H__34F12E25_115F_11D5_B595_009027BB3286__INCLUDED_)
