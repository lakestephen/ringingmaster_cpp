// PrintScaleObject.h: interface for the PrintScaleObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTSCALEOBJECT_H__4C44CBE4_9CC3_11D7_B6D3_000255162CF0__INCLUDED_)
#define AFX_PRINTSCALEOBJECT_H__4C44CBE4_9CC3_11D7_B6D3_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPage;

class PrintScaleObject  
{
public:
	PrintScaleObject(CDC* scrnDC, CDC* printDC, CPage* page, CPrintInfo* pInfo, CSize objectSize);
	virtual ~PrintScaleObject();

	float _scaleX;
	float _scaleY;
	float _scaleXY;
	int _offsetX;
	int _offsetY;
	int _pageX;
	int _pageY;
	BOOL _wholeColumns;
	int _leftGap;
	int _topGap;
	int _pageNumber;
	CSize _objectSize; //in screen units


	CRect translateRect(CRect rect);
	int translateX(int x);
	int translateY(int y);			 
	int translateXY(int xy);
	int scaleX(int x);
	int scaleY(int y);			 
	int scaleXY(int xy);
						   
	int getHorzOffset(int horzPageNumber);
	int getVertOffset(int vertPageNumber);

	int getVertPageNumber();
	int getHorzPageNumber();

	int getNumberOfVertPages();
	int getNumberOfHorzPages();

	int getNumberOfPages();

	int getVertSize();
	int getHorzSize();


};

#endif // !defined(AFX_PRINTSCALEOBJECT_H__4C44CBE4_9CC3_11D7_B6D3_000255162CF0__INCLUDED_)
