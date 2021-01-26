#if !defined(AFX_MethodWndPrint_H__8B4D0B0F_0705_11D5_B587_009027BB3286__INCLUDED_)
#define AFX_MethodWndPrint_H__8B4D0B0F_0705_11D5_B587_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MethodWndPrint.h : header file
//

#include "ScrollWnd.h" 
#include "Style.h"	// Added by ClassView
#include "DataTip.h"
#include "Method.h"
#include "RingingMaster.h"


/////////////////////////////////////////////////////////////////////////////
// MethodWndPrint window



class MethodWndPrint 
{
// Construction

public:
	MethodWndPrint(CDC* pDC, Method* method, const Style& style, 
					float scaleX, float scaleY,
				   int pageWidth, int pageHeight,
				   int leftGap, int topGap, 
				   BOOL wholeColumns);

	virtual ~MethodWndPrint();


// Operations
public:
	int getNumberOfPages();
	virtual void OnDraw(CDC* pDC, int pageNumber);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodWndPrint)
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	void calculateHorizontalPageBreaks();
	virtual int getVertPageNumber();
	virtual int getNumberOfVertPages();
	virtual int getNumberOfHorzPages();
	bool validLead(int i);
	int getVertSize();
	int getHorzSize();
	void init(CDC* pDC);
	int getHorzOffset(int horzPageNumber);
	int getVertOffset(int vertPageNumber);

	int getHorzPageNumber();
	
	CSize getLeadOrigin(int leadNo);
	const CRect& getLeadBox(int leadNo);


	void drawNumbers(CDC *pDC);
	void drawLines(CDC *pDC);
	
	virtual void drawLeadLines(CDC *pDC);
	void drawBellLines(CDC *pDC);
	virtual void drawStarts(CDC *pDC, int max);
	virtual void drawFalseRows(CDC *pDC);
	void drawMusicRows(CDC *pDC);
	void drawNotation(CDC *pDC, const CStringArray* notationStrings, int lead = 0, bool doprint = true);
	void drawStrokes(CDC *pDC, bool doprint = true);
	void drawComments(CDC *pDC);
	int drawTitle(CDC *pDC, bool doprint = true);

	virtual void recalcLeadOrigins();
	void recalcLeadBoxes();



	CArray <CSize,CSize> _leadOrigins;
	CArray <CRect,CRect> _leadBoxs;	
	CArray <int, int> _pageColumns;
	Method * _method;
	Style _style;
	const float _scaleX;
	const float _scaleY;
	const float _scaleXY;
	int _offsetX;
	int _offsetY;
	const int _pageX;
	const int _pageY;
	BOOL _wholeColumns;
	const int _leftGap;
	const int _topGap;
	int _pageNumber;

	CRect translateRect(CRect rect);
	int translateX(int x);
	int translateY(int y);			 
	int translateXY(int xy);
	int scaleX(int x);
	int scaleY(int y);			 
	int scaleXY(int xy);
	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MethodWndPrint_H__8B4D0B0F_0705_11D5_B587_009027BB3286__INCLUDED_)
