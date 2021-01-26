// MethodWndPrint.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodWndPrint.h"
#include "Lead.h"
#include "StyleEditDlg.h"
#include "MoreDlg.h"
#include "GenericEditDlg.h"
#include "FalseRow.h"
#include "MethodPosition.h"
#include "TouchDoc.h"
#include "MethodAnalysis.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MethodWndPrint


MethodWndPrint::MethodWndPrint(CDC* pDC, // do not hold on to this under any circumstances
							   Method* method, 
							   const Style& style, 
							   float scaleX, 
							   float scaleY,
							   int pageX,
							   int pageY,
							   int leftGap,
							   int topGap,
							   BOOL wholeColumns) :
_method(method),
_style(style),
_scaleX(scaleX),
_scaleY(scaleY),
_offsetX(0),
_offsetY(0),
_scaleXY((float)((scaleY+scaleX)/2.0)),
_pageX(pageX),
_pageY(pageY),
_leftGap(leftGap),
_topGap(topGap),
_wholeColumns(wholeColumns),
_pageNumber(0)
{
	init(pDC);
}

MethodWndPrint::~MethodWndPrint()
{
	_leadOrigins.RemoveAll();
	_leadBoxs.RemoveAll();
}

void MethodWndPrint::init(CDC *pDC)
{


	//calculate some initial values
	recalcLeadOrigins();
	recalcLeadBoxes();	  
	
	_style._titleHeight = drawTitle(pDC, false);		

	//todo check that we have analysis
	drawNotation(pDC, &_method->getAnalysis()->getNotationStrings(), 0, false);

	drawStrokes(pDC, false);

	//now recalculate the real values
	recalcLeadOrigins();
	recalcLeadBoxes();	  

	calculateHorizontalPageBreaks();
}


/////////////////////////////////////////////////////////////////////////////
// MethodWndPrint message handlers

void MethodWndPrint::OnDraw(CDC* pDC, int pageNumber)
{				 
	if (pageNumber > getNumberOfPages())
		return; 
   /* todo 
	_pageNumber = pageNumber;

	//work out offset
	_offsetX = _leftGap - getHorzOffset(getHorzPageNumber());
	_offsetY = _topGap - getVertOffset(getVertPageNumber());

	if (_method)
	{	

		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);

		_style._titleHeight = drawTitle(pDC);		

		drawNotation(pDC, &_method->getNotationStrings(), 0);

		drawStrokes(pDC);
		
		drawMusicRows(pDC);	 	

		drawFalseRows(pDC);	 	

		drawBellLines(pDC);
		
		drawLeadLines(pDC);
		
		drawNumbers(pDC);

		drawLines(pDC);

		drawStarts(pDC, _method->GetSize());		

		drawComments(pDC);
	}
	*/
}



void MethodWndPrint::drawLines(CDC *pDC)
{
/* todo
	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);
			
			for (int i=0;i<_method->getNumber();i++)	   // index through all bells
			{
				Style::StyleLine* stLine = _style.getLine(i, _method->getNumber());
				if (stLine->_lineColor != -1) //only process if visible
				{
					//set up the pen
					CPen pen;
					if (!pen.CreatePen(PS_SOLID,scaleXY(stLine->_lineWidth),
									stLine->_lineColor)) 	return;
					CPen * oldPen = pDC->SelectObject(&pen);							   
					//set initial position
					for (int k=0;k<_method->getNumber();k++)
						if (lead->GetAt(0)->getBellInPlace(k) == i+1)
						{
							pDC->MoveTo(translateX(orig.cx + (_style._width*k)), 
										translateY(orig.cy));
							break;
						}		
					//index through all rows
					for (int j=0;j<lead->GetSize();j++)
					{
						for (int k=0;k<_method->getNumber();k++)
						{
							if (lead->GetAt(j)->getBellInPlace(k) == i+1)
							{
							pDC->LineTo(translateX(orig.cx + (_style._width*k)), 
										translateY(orig.cy + (_style._height*j)));
							break;						
							}
						}
					}
					pDC->SelectObject(oldPen);
				}
			}
		}
	}
	*/
}

void MethodWndPrint::drawNumbers(CDC *pDC)
{  
	/* todo

	//create the font using the original font 
	CFont origFont;
	LOGFONT lf;
	_style._numbersFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&origFont);

	//get offset using calculated font
	CSize offset;
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	offset.cy = -(tm.tmHeight/2);
	offset.cx = 1;

	//now do the real scaled font 
	CFont font;
	FontInfo scaleNumberFont = _style._numbersFont;
	scaleNumberFont._size = scaleXY(scaleNumberFont._size);
    scaleNumberFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	pDC->SelectObject(&font);
	pDC->SetTextAlign(TA_CENTER);


	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
		
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l) + offset;
   		
			for (int i=0;i<_method->getNumber();i++)	   // index through all bells
			{
				Style::StyleLine* stLine = _style.getLine(i, _method->getNumber());

				if (stLine->_textColor != -1) //only process if visible 
				{
					pDC->SetTextColor(stLine->_textColor);
					//index through all rows
					for (int j=0;j<lead->GetSize();j++)
					{
						for (int k=0;k<_method->getNumber();k++)
						{
							if (lead->GetAt(j)->getBellInPlace(k) == i+1)
							{
								pDC->TextOut(translateX(orig.cx + (_style._width*k)), 
											 translateY(orig.cy + (_style._height*j)),
									CString(GlobalFunctions::bellNumbersToChar(i+1)));
							}
						}
					}
				}
			}
		}
	}
	pDC->SelectObject(oldFont);
	*/
}

void MethodWndPrint::drawLeadLines(CDC *pDC)
{  
	/* todo
	if (_style._gridColorH == -1) return;

	CPen pen(PS_SOLID, scaleY(_style._gridWidthH), _style._gridColorH);
	CPen * oldPen = pDC->SelectObject(&pen);

	int halfHeight = _style._height/2;

	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);

			for (int i=0;i<lead->getLeadLineCount();i++)
			{
				if ((lead->getLeadLine(i) >=0)&&(lead->getLeadLine(i) < lead->GetSize()))
				{
					int vertPos = orig.cy + (_style._height * lead->getLeadLine(i)) + halfHeight;
	
					pDC->MoveTo(translateX(orig.cx - (_style._width/2)), 
						        translateY(vertPos));
					pDC->LineTo(translateX(orig.cx + ((_method->getNumber()-1) * _style._width)
									+ (_style._width/2)), translateY(vertPos));
				}
			}
		}
	}
	pDC->SelectObject(oldPen);
	*/
}

void MethodWndPrint::drawBellLines(CDC *pDC)
{  
/* todo
	if (_style._gridColorV == -1) return;

	CPen pen(PS_SOLID, scaleX(_style._gridWidthV), _style._gridColorV);
	CPen * oldPen = pDC->SelectObject(&pen);

	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			CSize orig = getLeadOrigin(l);

			for (int i=0; i<_method->getNumber();i++)
			{
				pDC->MoveTo(translateX(orig.cx + (_style._width * i)), 
							translateY(orig.cy));
				pDC->LineTo(translateX(orig.cx + (_style._width * i)), 
							translateY(orig.cy + _method->GetAt(l)->GetUpperBound() * _style._height));
			}
		}
	}
	pDC->SelectObject(oldPen);*/
}
	
void MethodWndPrint::drawStarts(CDC *pDC, int max)
{  
	/* todo
	if (!_style._startsShow) return;

	//establish which bell to follow.
	int bellToFollow = -1;

	if ((_method->getBellToFollow() == -1)&&
		(_method->GetAt(0)->getWorkingBellCount() > 0)) //first working bell
		bellToFollow = _method->GetAt(0)->getWorkingBell(0);
	else
		bellToFollow = _method->getBellToFollow();

	if (bellToFollow == -1) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._startsFont._color);

	//setup font
	CFont origFont;
	_style._startsFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&origFont);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	//now do the scaled font 
	CFont font;
	FontInfo scaledFont = _style._startsFont;
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	pDC->SelectObject(&font);	 


	Style::StyleLine* stLine = _style.getLine(bellToFollow-1, _method->getNumber());

	COLORREF color = stLine->_lineColor;
	int width = stLine->_lineWidth + 4 ;
	
	if (color == -1)
	{
		color = RGB(0,0,0);
		width = 0;//could be 5 if need to show blobs without starts
	}			  		
	
	CPen pen(PS_SOLID, scaleXY(width), color);
	CPen * oldPen = pDC->SelectObject(&pen);

 
	//find the number
	for (int l = 0;l<max;l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			int place = 0;
			for (int i=0;i<_method->getNumber();i++)
				if (lead->GetAt(0)->getBellInPlace(i) == bellToFollow)
					place = i;

			if ((!lead->isCommentRow(0,rcut_display))||
				(!_style._commentsShow))
			{
				CSize textOrig = getLeadOrigin(l);


				CString start;
				start.Format("%d", place+1);		
				
				//find the position to draw the number 
				textOrig.cx += ((_style._width * (_method->getNumber()-1)) + (_style._left/3));
				textOrig.cy -= halfTextHeight;
			
				pDC->TextOut(translateX(textOrig.cx), translateY(textOrig.cy), start);
			}


			if (_style._startsBlobs) 
			{
				//find pos to draw blob
				CSize blobOrig = getLeadOrigin(l);
				blobOrig.cx += (_style._width * place);
				pDC->MoveTo(translateX(blobOrig.cx), translateY(blobOrig.cy));
				pDC->LineTo(translateX(blobOrig.cx), translateY(blobOrig.cy));
			}
		}
	}

	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);	 */
}


void MethodWndPrint::drawNotation(CDC *pDC, const CStringArray* notationStrings, int lead, bool doprint)
{  
	/* todo
	_style._notationWidth = 0;

	if (!_style._notationShow) return;
	if (_method->GetSize() < 1) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._notationFont._color);

	//setup font
	CFont origFont;
	_style._notationFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&origFont);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	//set the width of widest notation prior to getting the origin
	int width;
	for (int i=0;i<notationStrings->GetSize();i++)
	{
		width = pDC->GetTextExtent(notationStrings->GetAt(i)).cx + 8;
		
		if (width > _style._notationWidth)
			_style._notationWidth = width;
	}		  

	//now do the scaled font 
	CFont font;
	FontInfo scaledFont = _style._notationFont;
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);

	
	CSize orig = getLeadOrigin(lead);
	CSize textOrig;

	CString notation;
	for (int i=0;i<notationStrings->GetSize();i++)
	{
		notation = notationStrings->GetAt(i);
		
		textOrig = orig;
		if ((_method->getNotationSym())&& 
			(i == notationStrings->GetUpperBound())&&
			lead == 0)
		{
			textOrig.cy += ((_style._height*((i* 2)-1)) +
							(_style._height/2) -
							(halfTextHeight));
		}
		else
		{
			textOrig.cy += ((_style._height*i) +
							(_style._height/2) -
							(halfTextHeight));
		}

	    //for calculation
		pDC->SelectObject(&origFont);

		textOrig.cx -= pDC->GetTextExtent(notation).cx;

    	pDC->SelectObject(&font);

		textOrig.cx -= 8;

		if (doprint)
			pDC->TextOut(translateX(textOrig.cx), translateY(textOrig.cy), notation);
	}  
	pDC->SelectObject(oldFont);

	*/
}

void MethodWndPrint::drawStrokes(CDC *pDC, bool doprint)
{  
	/* todo
	if ((!_style._backStrokeShow)&&(!_style._handStrokeShow)&&(!_style._showInCourse)) return;
	if (_method->GetSize() < 1) return;

		//only calculate if we need it
	if (_style._showInCourse)
		_method->calculateInCourse();

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._strokeFont._color);

	//setup font
	CFont origFont;
	_style._strokeFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&origFont);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	//set the width of widest notation prior to getting the origin if less than what is already there
	int width = pDC->GetTextExtent("b").cx + 2;
	if (width > _style._notationWidth)
			_style._notationWidth = width;

	CString str;

	//now do the scaled font 
	CFont font;
	FontInfo scaledFont = _style._strokeFont;
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	pDC->SelectObject(&font);	 

	if (doprint) 
	{
		//iterate through the leads
		for (int l = (_style._notationShow)?1:0;l<_method->GetSize();l++)
		{
			if (validLead(l))
			{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);
			orig.cx -= (width + 2);
			orig.cy -= (_style._height + halfTextHeight);

				//index through all rows
				for (int j=0;j<lead->GetSize();j++)
				{
					if (_style._showInCourse)
					{
						str = (lead->GetAt(j)->getIsInCourse())?"+":"-";
					}
					else
					{
						str = (lead->GetAt(j)->getStroke() == hand)?
											((_style._handStrokeShow)?"h":""):
											((_style._backStrokeShow)?"b":"");
					}

					pDC->TextOut(translateX(orig.cx)  , 
								 translateY(orig.cy += _style._height), 
								 str);
				}			
			}
		}
	}
	pDC->SelectObject(oldFont);	   */
}

void MethodWndPrint::drawComments(CDC *pDC)
{
	/* todo
	if (!_style._commentsShow) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._commentsFont._color);

	//setup font
	CFont origFont;
	_style._commentsFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&origFont);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	//now do the scaled font 
	CFont font;
	FontInfo scaledFont = _style._commentsFont;
	scaledFont._size = scaleXY(scaledFont._size);
	scaledFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	pDC->SelectObject(&font);	 

	
	//iterate through the leads
	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);
			orig.cx += ((_style._width * (_method->getNumber()-1)) + 5) ;//+ (_style._left/3));

			//index through all calls
			for (int j=0;j<lead->getCommentCount(rcut_display);j++)
			{
				RowComment* comment = lead->getComment(j, rcut_display);
				pDC->TextOut(translateX(orig.cx),
							 translateY(orig.cy + ((_style._height*comment->getLeadRow()) -
										(halfTextHeight))),
							 comment->getDisplayString());
			}
		}
	}
	pDC->SelectObject(oldFont);			

	*/
}


int MethodWndPrint::drawTitle(CDC *pDC, bool doprint)
{  

	//create the font
	CFont origFont;
	int height = 0;
	TEXTMETRIC tm;
	LOGFONT lf;
	/* todo

	//setup title font
	_style._titleFont.getLogFont(&lf);
	origFont.CreateFontIndirect(&lf);
	//get title string
	CString strTitle = _method->getName();
	int dotPos = strTitle.Find('.',0);
	if (dotPos > 1)
		strTitle = strTitle.Left(dotPos);

	//get notation string
	CString strNotation = "(" + _method->getNotationDisplayString() + ")";

	//set up DC
	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._titleFont._color);

	//write the title text out
	CFont * oldFont = pDC->SelectObject(&origFont);

	if (_style._titleShow)
	{
		pDC->GetTextMetrics(&tm);
		
		//now do the scaled font 
		CFont font;
		FontInfo scaledFont = _style._titleFont;
		scaledFont._size = scaleXY(scaledFont._size);
		scaledFont.getLogFont(&lf);
		font.CreateFontIndirect(&lf);
	    pDC->SelectObject(&font);	 

		if (doprint) 
			pDC->TextOut(translateX(_style._notationWidth), translateY(0), strTitle);
		height = tm.tmHeight;
	}

	//clean up
	pDC->SelectObject(oldFont);

	*/
	return height;	
}

void MethodWndPrint::drawMusicRows(CDC *pDC)
{  
/* todo
for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);
		
			CRect rcRow(orig.cx - (_style._width/2) + 1,
						0,
						orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width/2),
						0);


			for (int j=0;j<lead->GetSize();j++)
			{
				if (lead->GetAt(j)->isMusic())//todo check how this works for the first and last row in the method.
				{  

					MusicResult * musicResult = NULL;
					//get the right one
					if (lead->GetAt(j)->isMusic()) 
						musicResult = lead->GetAt(j)->getMusicResult();
					else
						musicResult = _method->GetAt(l+1)->GetAt(0)->getMusicResult();
					if (musicResult != NULL)
					{
						COLORREF color = (musicResult->getUseStyleColor())?
							_style._musicRowsColor:
							musicResult->getColor();

						if (color != -1) 
						{
							//get the rect size
							rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
							rcRow.bottom = rcRow.top + _style._height-1;
							pDC->FillSolidRect(translateRect(rcRow),color);
						}
					}
				}
			}
		}
	}
	*/
}

void MethodWndPrint::drawFalseRows(CDC *pDC)
{  
	if (_style._falseRowsColor == -1) return;
 /* todo
	COLORREF color = _style._falseRowsColor;	
	
	int R = GetRValue(_style._falseRowsColor);
	int G = GetGValue(_style._falseRowsColor);
	int B = GetBValue(_style._falseRowsColor);

	R += (R>128)?-30:40;
	G += (G>128)?-30:40;
	B += (B>128)?-30:40;

	COLORREF drawColor;

	for (int l = 0;l<_method->GetSize();l++)
	{
		if (validLead(l))
		{
			Lead * lead = _method->GetAt(l);
			CSize orig = getLeadOrigin(l);
		
			CRect rcRow(orig.cx - (_style._width/2) + 1,
						0,
						orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width/2),
						0);

			for (int j=0;j<lead->GetSize();j++)
			{
				if (lead->GetAt(j)->isFalse())//todo check how this works for the first and last row in the method.
				{  
					drawColor = color;
					

					//get the rect size
					rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
					rcRow.bottom = rcRow.top + _style._height-1;

					pDC->FillSolidRect(translateRect(rcRow),drawColor);
				}
			}	 
		}
	}
	*/
}


CSize MethodWndPrint::getLeadOrigin(int leadNo)
{
	return _leadOrigins.GetAt(leadNo);
}

const CRect& MethodWndPrint::getLeadBox(int leadNo)
{
	return _leadBoxs.GetAt(leadNo);
}


void MethodWndPrint::recalcLeadOrigins()
{
	CSize origin;

	_leadOrigins.RemoveAll();
	
	if (_method == NULL)
		return;
/*
	for (int leadNo=0;leadNo<_method->GetSize();leadNo++)
	{
		//origins
		origin.cx = _style._notationWidth;
		origin.cy = _style.getTop();

		int colNo = leadNo / _style._leadsPerColumn;

		//x pos
		origin.cx += ((_style._left + 
					   (_style._width * (_method->getNumber()-1))) *
					   (colNo)) ;
		//y pos
		int firstLeadInCol = colNo * _style._leadsPerColumn;
		for (int i=firstLeadInCol;i<leadNo;i++)
		{
			origin.cy +=  (_method->GetAt(i)->GetUpperBound() * 
							_style._height);
		}				 	

		_leadOrigins.Add(origin);
	}		  */
}

//this lead box in calculated differently to the MethodWnd one. it encompasses the righ hand gap

void MethodWndPrint::recalcLeadBoxes()
{
	CRect box;
	CSize origin;

	_leadBoxs.RemoveAll();
	
	if (_method == NULL)
		return;

		/* todo
	for (int leadNo=0;leadNo<_method->GetSize();leadNo++)
	{
		origin = getLeadOrigin(leadNo);
		//lead box
		box.top    = origin.cy - (_style._height/2);
		box.left   = origin.cx;
		box.bottom = origin.cy + (_method->GetAt(leadNo)->GetSize() * _style._height) - (_style._height/2);
		box.right  = origin.cx + (_method->getNumber() * _style._width) + 
									_style._left;

		_leadBoxs.Add(box);
	}		*/
}

int MethodWndPrint::scaleX(int x)
{
	return (int)((float)x * _scaleX);
}

int MethodWndPrint::scaleY(int y)
{
	return (int)((float)y * _scaleY);
}

int MethodWndPrint::scaleXY(int xy)
{
	return (int)((float)xy * _scaleXY);
}

int MethodWndPrint::translateX(int x)
{
	return ((int)((float)x * _scaleX)) + _offsetX;

}

int MethodWndPrint::translateY(int y)
{
	return ((int)((float)y * _scaleY)) + _offsetY;

}

int MethodWndPrint::translateXY(int xy)
{
	return (int)((float)xy * _scaleXY);
}

CRect MethodWndPrint::translateRect(CRect rect)
{
	rect.top = translateY(rect.top);
	rect.bottom = translateY(rect.bottom);
	rect.left = translateX(rect.left);
	rect.right = translateX(rect.right);

	return rect;	   
}


							
//returns the vertical size in printer units
int MethodWndPrint::getVertSize()
{
	//loop through the boxes, and get the positions of the bottom
	int height = 0;
	/* todo
	for (int l = 0;l<_method->GetSize();l++) 
	{
    	CRect leadRect = getLeadBox(l);

		if (leadRect.bottom > height)
			height = leadRect.bottom;
	}
			 */
	return scaleY(height);

}

//returns the Horizontal size in printer units
int MethodWndPrint::getHorzSize() 
{
	int width = 0;
	/* todo
	for (int l = 0;l<_method->GetSize();l++) 
	{
    	CRect leadRect = getLeadBox(l);

		if (leadRect.right > width)
			width = leadRect.right;
	}
	 */
	return scaleX(width);

}


bool MethodWndPrint::validLead(int i) 
{
	if (!_wholeColumns)
		return true;

	int column = i / _style._leadsPerColumn;

	int horzPageNumber = getHorzPageNumber();
	if (_pageColumns.GetSize() > horzPageNumber)
	{
		int startCol = _pageColumns.GetAt(horzPageNumber);
		int endCol = 100000;
		if (_pageColumns.GetSize() > horzPageNumber +1)
			endCol = _pageColumns.GetAt(horzPageNumber +1);
		if ((column >= startCol)&&(column < endCol))
		{
//			TRACE("true horzPageNumber = %d, i = %d , column = %d, startCol = %d, endCol = %d\r\n", _pageNumber, i, column, startCol, endCol);
			return true;
		}
	}
	return false;
}

int MethodWndPrint::getVertOffset(int vertPageNumber)
{
	return _pageY * vertPageNumber;
}

int MethodWndPrint::getHorzOffset(int horzPageNumber)
{
	if (_wholeColumns) 
		return translateX(getLeadBox(_pageColumns.GetAt(horzPageNumber) * _style._leadsPerColumn).left);
	else
		return _pageX * horzPageNumber;
}

//in printer units
int MethodWndPrint::getNumberOfVertPages()
{
	return (getVertSize()/_pageY) + 1;
}

int MethodWndPrint::getNumberOfHorzPages()
{
	if (_wholeColumns) 
		return _pageColumns.GetSize();
	else
		//just blat it on the paper
		return (getHorzSize() / _pageX) + 1;
}

void MethodWndPrint::calculateHorizontalPageBreaks() 
{

	if (_leadBoxs.GetSize() == 0)
		return;

	if (_pageX < translateX(getLeadBox(0).Width()))
		_wholeColumns = FALSE;

	if (!_wholeColumns)
	{
		TRACE(" No Whole Colums\r\n");
		return ;
	}

	
	//loop through all columns, and add to an array the index 
	// of the columns that start the pages
	_pageColumns.RemoveAll();
	_pageColumns.Add(0);
	int offset = 0;
	int numberOfColumns = ((_method->getLeadCount()-1) / _style._leadsPerColumn) +1 ;
	for (int i=0;i<numberOfColumns; i++) 
	{
		CRect rect = getLeadBox(i * _style._leadsPerColumn)	;
		int right = scaleX(rect.right);
		if (right > (offset + _pageX))
		{
			//set the offset to the prev lead
			offset = scaleX(rect.left);;
			_pageColumns.Add(i);
			TRACE("column = %d\r\n",i);
		}
	}
}


int MethodWndPrint::getNumberOfPages()
{
	return getNumberOfHorzPages() * getNumberOfVertPages();
}

int MethodWndPrint::getHorzPageNumber()
{
	return (int)((_pageNumber-1)/getNumberOfVertPages());

}

int MethodWndPrint::getVertPageNumber()
{
	return (_pageNumber - (getHorzPageNumber() * getNumberOfVertPages()))-1;
}
