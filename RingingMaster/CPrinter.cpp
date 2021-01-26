#include "stdafx.h"
#include "CPrinter.h"
//	This file contains the printer primitives. This is a very low level implemtation
//	and should never be called directly. A member of this class is included in the 
//	CPage class and is used to print the page. Information in the PRTTYPE struct is
//	modified here and is reflected in the structure.
CPrinter::CPrinter()
{
	RotationAngle=0;
}

CPrinter::~CPrinter()
{
}

void CPrinter::FillRect(PRTTYPE *ps)
{
	doOffset(ps, true);    
	HBRUSH hOldBrush, hBrush;
	RECT  lpRect;

	hBrush = (HBRUSH)::GetStockObject((ps->uFillFlags & FILL_GRAY)   ? GRAY_BRUSH   :
				(ps->uFillFlags & FILL_LTGRAY) ? LTGRAY_BRUSH :
				(ps->uFillFlags & FILL_DKGRAY) ? DKGRAY_BRUSH :
				(ps->uFillFlags & FILL_WHITE)  ? WHITE_BRUSH :
				(ps->uFillFlags & FILL_BLACK)  ? BLACK_BRUSH  : HOLLOW_BRUSH);

	hOldBrush = (HBRUSH)::SelectObject(ps->pDC->GetSafeHdc(), hBrush);


	lpRect.left= ps->rc.left+ps->MarginOffset;
	lpRect.top = ps->rc.top;
	lpRect.right= ps->rc.right+ps->MarginOffset;
	lpRect.bottom= ps->rc.bottom;
	
	::FillRect(ps->pDC->GetSafeHdc(),&lpRect,hBrush);
	::DeleteObject(SelectObject(ps->pDC->GetSafeHdc(), hOldBrush));
	doOffset(ps, false);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//	Desc:
//	params:
//	Returns:
///////////////////////////////////////////////////////////////////////////
void CPrinter::DrawRect(PRTTYPE *ps,int LineSize)
{
			doOffset(ps, true);
	    HPEN   hOldPen, hPen;
	    HBRUSH hOldBrush, hBrush;
	    int   nLineWidth;
	    POINT  lpPT[4];

	    hBrush = (HBRUSH)::GetStockObject((ps->uFillFlags & FILL_GRAY)   ? GRAY_BRUSH   :
				    (ps->uFillFlags & FILL_LTGRAY) ? LTGRAY_BRUSH :
				    (ps->uFillFlags & FILL_DKGRAY) ? DKGRAY_BRUSH :
					(ps->uFillFlags & FILL_WHITE)  ? WHITE_BRUSH :
				    (ps->uFillFlags & FILL_BLACK)  ? BLACK_BRUSH  : HOLLOW_BRUSH);

	    hOldBrush = (HBRUSH)::SelectObject(ps->pDC->GetSafeHdc(), hBrush);
        
        nLineWidth = LineSize;
        
	    hPen = ::CreatePen((ps->uPenFlags & PEN_DASH) ? PS_DASH  :
			     (ps->uPenFlags & PEN_DOT)	? PS_DOT   : PS_SOLID,
			     nLineWidth, ps->pDC->GetTextColor());

	    hOldPen = (HPEN)::SelectObject(ps->pDC->GetSafeHdc(), hPen);

	    lpPT[0].x = ps->rc.left+ps->MarginOffset;
	    lpPT[0].y = ps->rc.top;
	    lpPT[1].x = ps->rc.right+ps->MarginOffset;
	    lpPT[1].y = ps->rc.top;
	    lpPT[2].x = ps->rc.right+ps->MarginOffset;
	    lpPT[2].y = ps->rc.bottom;
	    lpPT[3].x = ps->rc.left+ps->MarginOffset;
	    lpPT[3].y = ps->rc.bottom;

	    Polygon(ps->pDC->GetSafeHdc(), lpPT, sizeof(lpPT) / sizeof(POINT));

	    ::DeleteObject(SelectObject(ps->pDC->GetSafeHdc(), hOldPen));
	    ::DeleteObject(SelectObject(ps->pDC->GetSafeHdc(), hOldBrush));

		doOffset(ps, false);
}

////////////////////////////////////////////////////////////////////////////
//	Desc:
//	params:
//	Returns:
///////////////////////////////////////////////////////////////////////////
void CPrinter::DrawLine(PRTTYPE *ps,int LineSize)                    
{
		doOffset(ps, true);
		
		HPEN  hOldPen, hPen;
	    POINT pt[2];
	    int   nPenStyle;
	    int   nLineWidth;

	    nPenStyle = (ps->uPenFlags & PEN_DASH)	 ? PS_DASH	 :
			(ps->uPenFlags & PEN_DOT)	 ? PS_DOT	 :
			(ps->uPenFlags & PEN_DASHDOT)	 ? PS_DASHDOT	 :
			(ps->uPenFlags & PEN_DASHDOTDOT) ? PS_DASHDOTDOT :
			 PS_SOLID;


	    nLineWidth = LineSize;

		hPen = ::CreatePen(nPenStyle, nLineWidth, ps->pDC->GetTextColor( ));
	    
        pt[0].x = ps->rc.left+ps->MarginOffset;
        pt[0].y = ps->rc.top;
        pt[1].x = ps->rc.right+ps->MarginOffset;
        pt[1].y = ps->rc.bottom;
        
	    hOldPen = (HPEN)::SelectObject(ps->pDC->GetSafeHdc(), hPen);                   
	    
        ::Polyline(ps->pDC->GetSafeHdc(), pt, sizeof(pt) / sizeof(POINT));
        
	    ::DeleteObject(SelectObject(ps->pDC->GetSafeHdc(), hOldPen));

		doOffset(ps, false);
}


//returns the point of the bracket
CPoint CPrinter::drawBracket(PRTTYPE *ps,int LineSize)
{
	//get these tow before the offset is applied
	int height = ps->rc.bottom;
	int width = ps->rc.right;

	doOffset(ps, true);

	CPoint start(ps->rc.left, ps->rc.top);
	
	HPEN  hOldPen, hPen;
	POINT pt[2];
	int   nPenStyle;
	int   nLineWidth;
	
	nPenStyle = (ps->uPenFlags & PEN_DASH)	 ? PS_DASH	 :
	(ps->uPenFlags & PEN_DOT)	 ? PS_DOT	 :
	(ps->uPenFlags & PEN_DASHDOT)	 ? PS_DASHDOT	 :
	(ps->uPenFlags & PEN_DASHDOTDOT) ? PS_DASHDOTDOT :
			 PS_SOLID;
			 
 	 nLineWidth = LineSize;
 
	 hPen = ::CreatePen(nPenStyle, nLineWidth, ps->pDC->GetTextColor( ));
 
	 hOldPen = (HPEN)::SelectObject(ps->pDC->GetSafeHdc(), hPen);                   
 
 
 
	 int dev = height/6;
	 int half = start.y + height/2;
	 int whole = start.y + height;
 
	 ::Arc(ps->pDC->GetSafeHdc(),
		 start.x-width,		start.y, 
		 start.x+width,		start.y+dev+dev,
		 start.x+width,		start.y+dev, 
		 start.x,			start.y);
 
	 pt[0].x = start.x+width;
	 pt[0].y = start.y+dev;
	 pt[1].x = start.x+width;
	 pt[1].y = half-dev;
 
	 ::Polyline(ps->pDC->GetSafeHdc(), pt, sizeof(pt) / sizeof(POINT));
 
	 ::Arc(		ps->pDC->GetSafeHdc(),	
		 start.x+width,		half - dev-dev,
		 start.x+width+width+width,half,
		 start.x+width,		half-dev,
		 start.x+width+width,    half);
 
	 ::Arc(		ps->pDC->GetSafeHdc(),
		 start.x+width,		half,
		 start.x+width+width+width,half+dev+dev,
		 start.x+width+width,    half,
		 start.x+width,		half+dev);
 
	 pt[0].x = start.x+width;
	 pt[0].y = half+dev;
	 pt[1].x = start.x+width;
	 pt[1].y = whole-dev;
 
	 ::Polyline(ps->pDC->GetSafeHdc(), pt, sizeof(pt) / sizeof(POINT));
 
 
	 ::Arc(		ps->pDC->GetSafeHdc(), 
		 start.x-width,		whole-dev-dev, 
		 start.x+width,		whole,
		 start.x,  			whole,
		 start.x+width,		whole-dev);
 
	 ::DeleteObject(SelectObject(ps->pDC->GetSafeHdc(), hOldPen));
 
	 doOffset(ps, false);
 
	 CPoint tip(start.x+width+width, half);

	 //the offset is implicit, and needs removing
	 tip.x -= ps->_offset.cx;
	 tip.y -= ps->_offset.cy;

	 return tip; //tip of the bracket
}




////////////////////////////////////////////////////////////////////////////
//	Desc:
//	params:
//	Returns:
///////////////////////////////////////////////////////////////////////////
int CPrinter::PrintText(PRTTYPE *ps,double LineSpacing)
{         
		doOffset(ps, true);
	
		LOGFONT	      lf;
	    HFONT	      hOldFont, hFont;
	    COLORREF 	  OldColor= RGB(100,100,100);
	    int		      saveContext;
		int			  OldBkMode = OPAQUE;
	    RECT	      rc,LastPos;
	    HBRUSH	      hBrush;
	    HBRUSH	      hOldBrush =  NULL;
	    UINT	      fuFormat = DT_NOPREFIX;

    
        int uiLineSpacing;
        
        if(strlen(ps->Text)==0)
		{
			doOffset(ps, false);
        	return	2 * ps->PointSize;
		}
        	
	      //initialize the rect structure
	    rc.left   = ps->rc.left+ps->MarginOffset;
	    rc.top    = ps->rc.top;
	    rc.right  = ps->rc.right+ps->MarginOffset;
		if(rc.right > (signed)ps->n_maxWidth-ps->MarginOffset)
			rc.right=ps->n_maxWidth-ps->MarginOffset;
	    rc.bottom = ps->rc.bottom;

	      //convert logical coordinates to device coordinates
	    ps->pDC->LPtoDP((LPPOINT)&rc, 2);

	      //save the dc and change map mode to MM_TEXT.  Much
	    saveContext = ps->pDC->SaveDC();
	    ps->pDC->SetMapMode(MM_TEXT);

	    lf.lfHeight = -MulDiv(ps->PointSize,
				  ps->pDC->GetDeviceCaps(LOGPIXELSY), 72);
	    lf.lfWidth = 0;
	    if (RotationAngle == 0)
		{
			if (ps->_doPrintClip)
				lf.lfEscapement = 1;   //solves a lot of clipping problems!! - SJL
			else 
				lf.lfEscapement = 0;   
		}
		else
			lf.lfEscapement = RotationAngle;
		
		//reset
		ps->_doPrintClip = true;

	    lf.lfOrientation = 0;
	    lf.lfWeight = (ps->uTextFlags & TEXT_BOLD) ? FW_BOLD : FW_NORMAL;
	    lf.lfItalic = (BYTE)((ps->uTextFlags & TEXT_ITALIC) ? 1 : 0);
	    lf.lfUnderline = (BYTE)((ps->uTextFlags & TEXT_UNDERLINED) ? 1 : 0);
	    lf.lfStrikeOut = (BYTE)((ps->uTextFlags & TEXT_STRIKEOUT) ? 1 : 0);
	    lf.lfCharSet = ANSI_CHARSET;
	    lf.lfOutPrecision = OUT_STROKE_PRECIS;
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;
		
	    lf.lfQuality = PROOF_QUALITY;
	    lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	    lstrcpy((LPSTR)lf.lfFaceName, (LPCSTR)ps->FontName);
	    hFont = ::CreateFontIndirect(&lf);

	    hOldFont = (HFONT)::SelectObject(ps->pDC->GetSafeHdc(), hFont);


	      //set the alignment flags in the format bitfield
	    fuFormat |= (ps->uTextFlags & TEXT_CENTER) ? DT_CENTER :
					(ps->uTextFlags & TEXT_RIGHT)  ? DT_RIGHT  :DT_LEFT;

		if(ps->uTextFlags & TEXT_VCENTER) 
			fuFormat|=DT_VCENTER;
			
	      //expand tabs if indicated
	    if (ps->uTextFlags & TEXT_EXPANDTABS)
		fuFormat |=  DT_EXPANDTABS;

	      //set text clipping
	    if (ps->uTextFlags & TEXT_NOCLIP)
		fuFormat |=  DT_NOCLIP;
  
	      //if this is a single line then set alignment to bottom
	    if (ps->uTextFlags & TEXT_SINGLELINE)
			fuFormat |=  DT_SINGLELINE;
	    else
			fuFormat |=  DT_WORDBREAK;

	      //if the extents of rect haven't been specified then
	      //calculate them
	    if (ps->rc.right == 0 && ps->rc.bottom == 0)
			ps->pDC->DrawText(ps->Text, -1,&rc, fuFormat | DT_CALCRECT);
				 
				 
		LastPos=rc;


		/*if(    ps->uTextFlags & TEXT_RECT &&
	  		((ps->uFillFlags & 	FILL_BLACK) || 
			(ps->uFillFlags & 	FILL_LTGRAY) ||
            (ps->uFillFlags & 	FILL_DKGRAY))) */ // SJL
				OldBkMode=ps->pDC->SetBkMode(TRANSPARENT);
		
		if(    ps->uTextFlags & TEXT_RECT &&
	  		((ps->uFillFlags & 	FILL_BLACK) ||
            (ps->uFillFlags & 	FILL_DKGRAY)))
	   {
			OldColor= ps->pDC->SetTextColor(COLOR_WHITE);
	   }

	    if (ps->uTextFlags & TEXT_RECT)  
	    {

			hBrush = (HBRUSH)::GetStockObject((ps->uFillFlags & FILL_GRAY)   ? GRAY_BRUSH   :
				      (ps->uFillFlags & FILL_LTGRAY) ? LTGRAY_BRUSH :
				      (ps->uFillFlags & FILL_DKGRAY) ? DKGRAY_BRUSH :
				      (ps->uFillFlags & FILL_BLACK)  ? BLACK_BRUSH  : HOLLOW_BRUSH);
			hOldBrush = (HBRUSH)::SelectObject(ps->pDC->GetSafeHdc(), hBrush);

			ps->pDC->Rectangle(rc.left, rc.top, rc.right, rc.bottom);

	    }

		//now draw the text
	    ps->pDC->DrawText(ps->Text, -1,&rc, fuFormat);


	/*	if(    ps->uTextFlags & TEXT_RECT &&
	  		((ps->uFillFlags & 	FILL_BLACK) || 
			(ps->uFillFlags & 	FILL_LTGRAY) ||
            (ps->uFillFlags & 	FILL_DKGRAY))) */ // SJL
				OldBkMode=ps->pDC->SetBkMode(OldBkMode);
        
	  if(    ps->uTextFlags & TEXT_RECT &&
	  		((ps->uFillFlags & 	FILL_BLACK) ||
            (ps->uFillFlags & 	FILL_DKGRAY)))
	   {
			ps->pDC->SetTextColor(OldColor);
	   }
        
	   	ps->pDC->DrawText(ps->Text, -1,&LastPos, fuFormat | DT_CALCRECT);
    	uiLineSpacing = (int)(ps->PointSize*LineSpacing);
	    
	    if (ps->uTextFlags & TEXT_RECT)
	    	::DeleteObject(::SelectObject(ps->pDC->GetSafeHdc(), hOldBrush));
	    ::DeleteObject(::SelectObject(ps->pDC->GetSafeHdc(), hOldFont));
	    if (saveContext)
			ps->pDC->RestoreDC(saveContext);

	    ps->pDC->DPtoLP((LPPOINT)&LastPos, 2);
		ps->m_MinDisplacement=LastPos.bottom-LastPos.top;	    
		
		ps->pDC->GetTextMetrics( ps->tm );
	    ps->m_NextCharPos=LastPos.right;
		ps->LastPrintArea=LastPos;
		RotationAngle=0;
		
		doOffset(ps, false);

		return uiLineSpacing;
}         



////////////////////////////////////////////////////////////////////////////
//	Desc:
//	params:
//	Returns:
///////////////////////////////////////////////////////////////////////////
int CPrinter::GetPrintInfo(PRTTYPE *ps,double LineSpacing)
{         
		doOffset(ps, true);

	    LOGFONT	      lf;
	    HFONT	      hOldFont, hFont;
	    int		      saveContext;
	    RECT	      rc,LastPos;
	    UINT	      fuFormat = DT_NOPREFIX;
	    
        int uiLineSpacing;
        
        	
	      //initialize the rect structure
	    rc.left   = ps->rc.left+ps->MarginOffset;
	    rc.top    = ps->rc.top;
	    rc.right  = ps->rc.right+ps->MarginOffset;
	    rc.bottom = ps->rc.bottom;

	      //convert logical coordinates to device coordinates
	    ps->pDC->LPtoDP((LPPOINT)&rc, 2);

	      //save the dc and change map mode to MM_TEXT.  Much
	    saveContext = ps->pDC->SaveDC();

	    ps->pDC->SetMapMode(MM_TEXT);

	    lf.lfHeight = -MulDiv(ps->PointSize,
				  ps->pDC->GetDeviceCaps(LOGPIXELSY), 72);
	    lf.lfWidth = 0;
	    lf.lfEscapement = 0;
	    lf.lfOrientation = 0;
	    lf.lfWeight = (ps->uTextFlags & TEXT_BOLD) ? FW_BOLD : FW_NORMAL;
	    lf.lfItalic = (BYTE)((ps->uTextFlags & TEXT_ITALIC) ? 1 : 0);
	    lf.lfUnderline = (BYTE)((ps->uTextFlags & TEXT_UNDERLINED) ? 1 : 0);
	    lf.lfStrikeOut = (BYTE)((ps->uTextFlags & TEXT_STRIKEOUT) ? 1 : 0);
	    lf.lfCharSet = ANSI_CHARSET;
	    lf.lfOutPrecision = OUT_STROKE_PRECIS;
	    lf.lfClipPrecision = CLIP_STROKE_PRECIS;
	    lf.lfQuality = PROOF_QUALITY;
	    lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	    lstrcpy((LPSTR)lf.lfFaceName, (LPCSTR)ps->FontName);

	    hFont = ::CreateFontIndirect(&lf);

	    hOldFont = (HFONT)::SelectObject(ps->pDC->GetSafeHdc(), hFont);


	      //set the alignment flags in the format bitfield
	    fuFormat |= (ps->uTextFlags & TEXT_CENTER) ? DT_CENTER :
					(ps->uTextFlags & TEXT_RIGHT)  ? DT_RIGHT  :DT_LEFT;

		if(ps->uTextFlags & TEXT_VCENTER) 
			fuFormat|=DT_VCENTER;
			
	      //expand tabs if indicated
	    if (ps->uTextFlags & TEXT_EXPANDTABS)
		fuFormat |=  DT_EXPANDTABS;

	      //set text clipping
	    if (ps->uTextFlags & TEXT_NOCLIP)
		fuFormat |=  DT_NOCLIP;

	      //if this is a single line then set alignment to bottom
	    if (ps->uTextFlags & TEXT_SINGLELINE)
		fuFormat |=  DT_SINGLELINE;
	    else
		fuFormat |=  DT_WORDBREAK;

		LastPos=rc;

		ps->pDC->DrawText(ps->Text, -1,
		 &LastPos, fuFormat | DT_CALCRECT);
        
	    uiLineSpacing = (int)(ps->PointSize*LineSpacing);

	    ::DeleteObject(::SelectObject(ps->pDC->GetSafeHdc(), hOldFont));
	    if (saveContext)
			ps->pDC->RestoreDC(saveContext);

	    ps->pDC->DPtoLP((LPPOINT)&LastPos, 2);
		ps->m_MinDisplacement=LastPos.bottom-LastPos.top;	    
	    ps->m_NextCharPos=LastPos.right;
		ps->LastPrintArea=LastPos;

		doOffset(ps, false);

		return uiLineSpacing;
}         

void CPrinter::doOffset(PRTTYPE *ps, bool apply)
{
	bool maintainBottom = false;
	if (ps->rc.bottom == (long)ps->n_maxLength)
		maintainBottom = true;

	doOffset(ps->rc, ps->_offset, apply);	
	doOffset(ps->LastPrintArea, ps->_offset, apply);

	if (maintainBottom)
		ps->rc.bottom = ps->n_maxLength;
}

void CPrinter::doOffset(RECT &rect, CPoint offset,  bool apply)
{
	CRect newRect(rect);

	if (!apply)
	{
		offset.x = (-offset.x);
		offset.y = (-offset.y);
	}

	newRect.OffsetRect(offset);

	rect = newRect;
}

CSize CPrinter::getTextExtent(PRTTYPE *ps, CString str)
{
		LOGFONT	      lf;
		HFONT	      hOldFont, hFont;

	    lf.lfHeight = -MulDiv(ps->PointSize,
				  ps->pDC->GetDeviceCaps(LOGPIXELSY), 72);
	    lf.lfWidth = 0;
	    lf.lfEscapement = RotationAngle;
	    lf.lfOrientation = 0;
	    lf.lfWeight = (ps->uTextFlags & TEXT_BOLD) ? FW_BOLD : FW_NORMAL;
	    lf.lfItalic = (BYTE)((ps->uTextFlags & TEXT_ITALIC) ? 1 : 0);
	    lf.lfUnderline = (BYTE)((ps->uTextFlags & TEXT_UNDERLINED) ? 1 : 0);
	    lf.lfStrikeOut = (BYTE)((ps->uTextFlags & TEXT_STRIKEOUT) ? 1 : 0);
	    lf.lfCharSet = ANSI_CHARSET;
	    lf.lfOutPrecision = OUT_STROKE_PRECIS;
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;
		
	    lf.lfQuality = PROOF_QUALITY;
	    lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	    lstrcpy((LPSTR)lf.lfFaceName, (LPCSTR)ps->FontName);
	    hFont = ::CreateFontIndirect(&lf);

	    hOldFont = (HFONT)::SelectObject(ps->pDC->GetSafeHdc(), hFont);

		//now draw the text
	    CSize ext = ps->pDC->GetTextExtent(str);

	    ::DeleteObject(::SelectObject(ps->pDC->GetSafeHdc(), hOldFont));
		
		return ext;

}
