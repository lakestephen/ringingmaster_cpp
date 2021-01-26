// SGRectangle.h: interface for the SGRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SGRECTANGLE_H__8282BBE3_F071_11D5_B53E_000255162CF0__INCLUDED_)
#define AFX_SGRECTANGLE_H__8282BBE3_F071_11D5_B53E_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SuperGridCtrl;


class SGRectangle
{
public:
	SGRectangle(SuperGridCtrl* pCtrl, CDC* pDC, int iIndent, const CRect& rcBounds);
	~SGRectangle();
	void DrawRectangle(SuperGridCtrl *pCtrl);
	BOOL HitTest(CPoint pt);
	void DrawPlus(void);
	void DrawMinus(void);
	int GetLeft(void){return m_left;}
	int GetTop(void){return m_top;}
	CRect GetHitTestRect(void);

private:
	CDC *m_pDC;
	SIZE m_right_bottom;
	int m_left;
	int m_top;
	POINT m_left_top;
	int m_topdown;
};


#endif // !defined(AFX_SGRECTANGLE_H__8282BBE3_F071_11D5_B53E_000255162CF0__INCLUDED_)
