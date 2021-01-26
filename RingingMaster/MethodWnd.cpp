// MethodWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodWnd.h"
#include "Lead.h"
#include "StyleEditDlg.h"
#include "MoreDlg.h"
#include "GenericEditDlg.h"
#include "FalseRow.h"
#include "MethodPosition.h"
#include "TouchDoc.h"
#include "UserMessages.h"
#include "StyleManager.h"
#include "MethodManager.h"
#include "MethodAnalysis.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MethodWnd

IMPLEMENT_DYNCREATE(MethodWnd, ScrollWnd)

MethodWnd::MethodWnd(defaultStyle type ) :
_method(NULL),
_falseRow(NULL),
_contextRow(NULL),
_16BitOverload(false),
_highlightRow(NULL),
_style(type),
_resizing(false),
_hitType(MWRHT_NONE)
{
}

MethodWnd::~MethodWnd()
{
	_tip.DestroyWindow();
	_leadOrigins.RemoveAll();
	_leadBoxs.RemoveAll();
}


BEGIN_MESSAGE_MAP(MethodWnd, ScrollWnd)
	//{{AFX_MSG_MAP(MethodWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TOOLS_STYLE, OnToolsStyle)
	ON_COMMAND(ID_LINEWIDTH_1, OnLinewidth1)
	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_1, OnUpdateLinewidth1)
	ON_COMMAND(ID_LINEWIDTH_2, OnLinewidth2)
	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_2, OnUpdateLinewidth2)
	ON_COMMAND(ID_LINEWIDTH_3, OnLinewidth3)
	ON_UPDATE_COMMAND_UI(ID_LINEWIDTH_3, OnUpdateLinewidth3)
	ON_COMMAND(ID_LINECOLOR, OnLinecolor)
	ON_COMMAND(ID_TEXTCOLOR, OnTextcolor)
	ON_COMMAND(ID_STYLE_MENU_MORE, OnStyleMenuMore)
	ON_COMMAND(ID_STYLE_MENU_SAVE_AS, OnStyleMenuSaveAs)
	ON_UPDATE_COMMAND_UI(ID_STYLE_MENU_SAVE_AS, OnUpdateStyleMenuSaveAs)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_STYLE, OnUpdateToolsStyle)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SET_END_AT_ROW, OnSetEndAtRow)
	ON_UPDATE_COMMAND_UI(ID_SET_END_AT_ROW, OnUpdateSetEndAtRow)
	ON_COMMAND(ID_SET_START_AT_ROW, OnSetStartAtRow)
	ON_UPDATE_COMMAND_UI(ID_SET_START_AT_ROW, OnUpdateSetStartAtRow)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_METHOD_OPTIONS, OnMethodOptions)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_STYLE_MENU_00, ID_STYLE_MENU_09, OnApplyStyle)
	ON_UPDATE_COMMAND_UI(ID_TEXTCOLOR, OnUpdateLinecolor)
	ON_UPDATE_COMMAND_UI(ID_LINECOLOR, OnUpdateLinecolor)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_UPDATE_METHOD, OnUpdateMethod)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MethodWnd message handlers
void MethodWnd::OnDraw(CDC* pDC)
{
	if (_16BitOverload) 
	{
		drawOverload(pDC);
	}
	else if (_method && _method->hasAnalysis())	  //todo do we really need analysis? do we just need display stuff?
	{	

		CRect client;
		GetClientRect(&client);
		client += GetScrollPosition(); //keep this code for selective redraws

		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);

		_style._titleHeight = drawTitle(pDC);		

		drawNotation(pDC, &_method->getAnalysis()->getNotationStrings(), 0);//sets _style._notationWidth	// must be at the top

		drawStrokes(pDC);//sets _style._notationWidth if notation has not	// must be at the top, just under notation
		
		drawMusicRows(pDC); //draw before false rows, as false rows take prcidence

		drawFalseRows(pDC);	 	

		drawBellLines(pDC);
		
		drawLeadLines(pDC);
		
		drawNumbers(pDC);

		drawLines(pDC);

		drawStarts(pDC, _method->getLeadCount());		

		drawComments(pDC);
		
		drawHilightRow(pDC); //needs to be at the end, as using invert
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);

        pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		CFont* oldFont = (CFont*)pDC->SelectObject(&app()->_fontStd);

		rect.DeflateRect(20, 20);
		pDC->DrawText("There is no blue line to show.", rect, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );

		pDC->SelectObject(oldFont);


	}
}

void MethodWnd::drawOverload(CDC *pDC)
{
	pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	CString str;
	
	CSize size = getSize();
	bool width = size.cx >= SHRT_MAX;
	bool height = size.cy >= SHRT_MAX;

	CString insert;
	if ((width)&&(height))
	{
		insert = "width or height";

		str.Format("Method is %d%% too wide", (int)((((float)size.cx/(float)32767)*100)-100));
		pDC->TextOut(20,40,str);	

		str.Format("Method is %d%% too high", (int)((((float)size.cy/(float)32767)*100)-100));
		pDC->TextOut(20,60,str);	

	}
	else if (height) 
	{
		insert = "height";
		str.Format("Method is %d%% too high", (int)((((float)size.cy/(float)32767)*100)-100));
		pDC->TextOut(20,40,str);	

	}
	else if (width) 
	{
		insert = "width";
		str.Format("Method is %d%% too wide", (int)((((float)size.cx/(float)32767)*100)-100));
		pDC->TextOut(20,40,str);	
	}
	else 
		insert = "size";
	

	str.Format("Your operating system is unable to show the %s of your blue line.", insert);
	pDC->TextOut(20,20,str);

	pDC->TextOut(20,100,"You have several options:");	
	pDC->TextOut(20,120,"  1 Change the Style (right click) to take up less room.");	
	pDC->TextOut(20,140,"  2 Put a restriction on the number of changes.");	
	pDC->TextOut(20,160,"  3 Upgrade to Windows NT / 2000;-).");	


}

void MethodWnd::drawHilightRow(CDC *pDC)
{
	if (_highlightRow)
	{
		for (int l = 0;l<_method->getLeadCount();l++)
		{
			if (isLeadVisible(l, pDC))
			{
				const Lead& lead = _method->getLead(l);
				CSize orig = getLeadOrigin(l);
				
				CRect rcRow(orig.cx - (_style._width/2) + 1,
					0,
					orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width/2),
					0);
				
				for (int j=0;j<lead.GetSize();j++)
				{
					if (lead.GetAt(j) == _highlightRow) //todo make sure this draws OK for first and last rows in method.
					{  
						//get the rect size
						rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
						rcRow.bottom = rcRow.top + _style._height-1;
						pDC->InvertRect(rcRow);	   						
					}
				}	 
			}
		}
	}
}

const COLORREF ShouldDrawTypeGrey = RGB(210,210,210); //to2do get from tower profile or style?

void MethodWnd::drawLines(CDC *pDC)
{
	CPen greyPen;
	if (!greyPen.CreatePen(PS_SOLID,1, ShouldDrawTypeGrey)) 	return;

	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);
			
			if (lead.GetSize() > 0)
			{
				for (int i=0;i<_method->getNumber();i++)	   // index through all bells
				{
					Style::StyleLine* stLine = _style.getLine(i, _method->getNumber());
					if (stLine->_lineColor != -1) //only process if visible
					{
						//set up the pen
						CPen pen;
						if (!pen.CreatePen(PS_SOLID,stLine->_lineWidth,	
										stLine->_lineColor)) 	return;
						CPen * oldPen = pDC->SelectObject(&pen);							   
						//set initial position
						for (int k=0;k<_method->getNumber();k++)
						{
							if (lead.GetAt(0)->getBellInPlace(k) == i+1)
							{
								pDC->MoveTo(orig.cx + (_style._width*k), 
											orig.cy);
								break;
							}		
						}

						//index through all rows
						for (int j=0;j<lead.GetSize();j++)
						{
							for (int k=0;k<_method->getNumber();k++)
							{
								ShouldDrawType shouldDrawType = shouldDrawRow(lead.GetAt(j)->getRowIndex(),k);

								if (shouldDrawType == sdt_grey)
									pDC->SelectObject(&greyPen);							   

								if ((shouldDrawType == sdt_grey) ||
									(shouldDrawType == sdt_yes))
								{
									if(lead.GetAt(j)->getBellInPlace(k) == i+1)
									{
										pDC->LineTo(orig.cx + (_style._width*k), 
												orig.cy + (_style._height*j));
										break;						
									}
								}
							}
						}
						pDC->SelectObject(oldPen);
					}
				}
			}
		}
	}
}


void MethodWnd::drawNumbers(CDC *pDC)
{  

	//create the font
	CFont font;
	LOGFONT lf;
	_style._numbersFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

	pDC->SetTextAlign(TA_CENTER);

	//get offset
	CSize offset;
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	offset.cy = -(tm.tmHeight/2);
	offset.cx = 1;


	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{					   		
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l) + offset;
   		
			for (int i=0;i<_method->getNumber();i++)	   // index through all bells
			{
				Style::StyleLine* stLine = _style.getLine(i, _method->getNumber());

				if (stLine->_textColor != -1) //only process if visible 
				{
					pDC->SetTextColor(stLine->_textColor);
					//index through all rows
					for (int j=0;j<lead.GetSize();j++)
					{
						for (int k=0;k<_method->getNumber();k++)
						{
							ShouldDrawType shouldDrawType = shouldDrawRow(lead.GetAt(j)->getRowIndex(),k);//todo should not have to use getRowIndex

							if (shouldDrawType == sdt_grey)
								pDC->SetTextColor(ShouldDrawTypeGrey);

							if ((shouldDrawType == sdt_grey) ||
								(shouldDrawType == sdt_yes))
							{
								if (lead.GetAt(j)->getBellInPlace(k) == i+1)
								{
									pDC->TextOut(orig.cx + (_style._width*k), 
										orig.cy + (_style._height*j),
										CString(GlobalFunctions::bellNumbersToChar(i+1)));
								}
							}
						}
					}
				}
			}
		}
	}
	pDC->SelectObject(oldFont);
}

void MethodWnd::drawLeadLines(CDC *pDC)
{  
	if (_style._gridColorH == -1) return;

	CPen pen(PS_SOLID, _style._gridWidthH, _style._gridColorH);
	CPen * oldPen = pDC->SelectObject(&pen);

	int halfHeight = _style._height/2;

	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);

			//iterate through the rows 
			for (int j=0;j<lead.GetSize();j++)
			{				
				if (lead.GetAt(j)->isLeadLine())
				{
					int vertPos = orig.cy + (_style._height * j) + halfHeight;

					pDC->MoveTo(orig.cx - (_style._width/2), vertPos);
					pDC->LineTo(orig.cx + ((_method->getNumber()-1) * _style._width)
									+ (_style._width/2), vertPos);
				}
			}
		}
	}
	pDC->SelectObject(oldPen);
}

void MethodWnd::drawBellLines(CDC *pDC)
{  
	if (_style._gridColorV == -1) return;

	CPen pen(PS_SOLID, _style._gridWidthV, _style._gridColorV);
	CPen * oldPen = pDC->SelectObject(&pen);

	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			CSize orig = getLeadOrigin(l);

			for (int i=0; i<_method->getNumber();i++)
			{
				pDC->MoveTo(orig.cx + (_style._width * i), 
					orig.cy);
				pDC->LineTo(orig.cx + (_style._width * i), 
					orig.cy + _method->getLead(l).GetUpperBound() * _style._height);
			}
		}
	}
	pDC->SelectObject(oldPen);
}
	
void MethodWnd::drawStarts(CDC *pDC, int max)
{  
	if (!_style._startsShow) return;

	//establish which bell to follow.
	int bellToFollow = _method->getAnalysis()->getCallFrom();
  
	/*todo this cunk of code should be set in the analysis area rather than calculated each time. 
	if ((_method->getAnalysis()->getCallFrom() == -1)&&
		(_method->getLead(0).getWorkingBellCount() > 0)) //first working bell
		bellToFollow = _method->getLead(0).getWorkingBell(0);
	else
		bellToFollow = _method->getCallFrom();

		*/

	if (bellToFollow == -1) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._startsFont._color);

	//setup font
	CFont font;
	_style._startsFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

	
	Style::StyleLine* stLine = _style.getLine(bellToFollow-1, _method->getNumber());

	COLORREF color = stLine->_lineColor;
	int width = stLine->_lineWidth + 4 ;
	
	if (color == -1)
	{
		color = RGB(0,0,0);
		width = 0;//could be 5 if need to show blobs without starts
	}			  		
	
	CPen pen(PS_SOLID, width, color);
	CPen * oldPen = pDC->SelectObject(&pen);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	
 
	//find the number
	for (int l = 0;l<max;l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			int place = 0;
			for (int i=0;i<_method->getNumber();i++)
				if (lead.GetAt(0)->getBellInPlace(i) == bellToFollow)
					place = i;
				  /*  todo this needs to go back in when the comments have been added to the row.
			if ((!lead.isCommentRow(0, rcut_display))||
				(!_style._commentsShow))
			{
				CSize textOrig = getLeadOrigin(l);


				CString start;
				start.Format("%d", place+1);		
				
				//find the position to draw the number 
				textOrig.cx += ((_style._width * (_method->getNumber()-1)) + (_style._left/3));
				textOrig.cy -= halfTextHeight;
			
				pDC->TextOut(textOrig.cx, textOrig.cy, start);
			}


			if (_style._startsBlobs) 
			{
				//find pos to draw blob
				CSize blobOrig = getLeadOrigin(l);
				blobOrig.cx += (_style._width * place);
				pDC->MoveTo(blobOrig.cx, blobOrig.cy);
				pDC->LineTo(blobOrig.cx, blobOrig.cy);
			}	 */
		}
	}

	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);
}

void MethodWnd::drawNotation(CDC *pDC, const CStringArray* notationStrings, int lead)
{	   /*	todo
	_style._notationWidth = 0;

	if (!_style._notationShow) return;
	if (_method->getLeadCount() < 1) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._notationFont._color);

	//setup font
	CFont font;
	_style._notationFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

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

		textOrig.cx -= pDC->GetTextExtent(notation).cx;

		textOrig.cx -= 8;

		pDC->TextOut(textOrig.cx, textOrig.cy, notation);
	}  
	pDC->SelectObject(oldFont);		*/
}

void MethodWnd::drawStrokes(CDC *pDC)
{		/*todo
	if ((!_style._backStrokeShow)&&(!_style._handStrokeShow)&&(!_style._showInCourse)) return;
	if (_method->getLeadCount() < 1) return;

	//only calculate if we need it
	if (_style._showInCourse)
		_method->calculateInCourse();

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._strokeFont._color);

	//setup font
	CFont font;
	_style._strokeFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	//set the width of widest notation prior to getting the origin if less than what is already there
	int width = pDC->GetTextExtent("b").cx + 2;
	if (width > _style._notationWidth)
			_style._notationWidth = width;
	CString str;
	//iterate through the leads
	for (int l = (_style._notationShow)?1:0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);
			orig.cx -= (width + 2);
			orig.cy -= (_style._height + halfTextHeight);

			//index through all rows
			for (int j=0;j<lead.GetSize();j++)
			{
				if (_style._showInCourse)
				{
					str = (lead.GetAt(j)->getIsInCourse())?"+":"-";
				}
				else
				{
					str = (lead.GetAt(j)->getStroke() == hand)?
										((_style._handStrokeShow)?"h":""):
										((_style._backStrokeShow)?"b":"");
				}

				pDC->TextOut(orig.cx  , 
							 orig.cy += _style._height, 
							 str);
			}			
		}
	}
	pDC->SelectObject(oldFont);		*/
}

void MethodWnd::drawComments(CDC *pDC)
{
	/*	todo
	if (!_style._commentsShow) return;

	LOGFONT lf;

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._commentsFont._color);

	//setup font
	CFont font;
	_style._commentsFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	
	
	//iterate through the leads
	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);
			orig.cx += ((_style._width * (_method->getNumber()-1)) + 5) ;//+ (_style._left/3));

			//index through all calls
			for (int j=0;j<lead.getCommentCount(rcut_display);j++)
			{
				RowComment* call = lead.getComment(j, rcut_display);
					pDC->TextOut(orig.cx,
							 orig.cy + ((_style._height*call->getLeadRow()) -
											(halfTextHeight)),
							 call->getDisplayString());
			}
		}
	}
	pDC->SelectObject(oldFont);	  */
}


int MethodWnd::drawTitle(CDC *pDC)
{  
	//create the font
	CFont title, method;
	int height = 0;
	TEXTMETRIC tm;
	LOGFONT lf;
	
	//setup title font
	_style._titleFont.getLogFont(&lf);
	title.CreateFontIndirect(&lf);
	//get title string
	CString strTitle = _method->getAnalysis()->getName();
	int dotPos = strTitle.Find('.',0);
	if (dotPos > 1)
		strTitle = strTitle.Left(dotPos);

	//set up DC
	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._titleFont._color);

	//write the title text out
	CFont * oldFont = pDC->SelectObject(&title);
	if (_style._titleShow)
	{
		//height += 5;
		pDC->GetTextMetrics(&tm);
		pDC->TextOut(_style._left, height, strTitle);
		height += tm.tmHeight;
	}

	//clean up
	pDC->SelectObject(oldFont);
	return height;	
}

void MethodWnd::drawMusicRows(CDC *pDC)
{					/*	todo
	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);
		
			CRect rcRow(orig.cx - (_style._width/2) + 1,
						0,
						orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width/2),
						0);


			for (int j=0;j<lead.GetSize();j++)
			{
				if (lead.GetAt(j)->isMusic()) //todo check how this works for the first and last row in the method.
				{  
					//get the rect size
					rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
					rcRow.bottom = rcRow.top + _style._height-1;

					MusicResult * musicResult = NULL;
					//get the right one
					if (lead.GetAt(j)->isMusic()) 
						musicResult = lead.GetAt(j)->getMusicResult();
					else
						musicResult = _method->getLead(l+1).GetAt(0)->getMusicResult();
					if (musicResult != NULL)
					{
						COLORREF color = (musicResult->getUseStyleColor())?
							_style._musicRowsColor:
							musicResult->getColor();

						if (color != -1)
							pDC->FillSolidRect(rcRow,color);
					}
				}
			}
		}
	}		 */
}

void MethodWnd::drawFalseRows(CDC* pDC)
{  
	if (_style._falseRowsColor == -1) return;

	COLORREF color = _style._falseRowsColor;	
	
	int R = GetRValue(_style._falseRowsColor);
	int G = GetGValue(_style._falseRowsColor);
	int B = GetBValue(_style._falseRowsColor);

	R += (R>128)?-30:40;
	G += (G>128)?-30:40;
	B += (B>128)?-30:40;

	COLORREF hilight =	RGB(R,G,B);

	COLORREF drawColor;

	for (int l = 0;l<_method->getLeadCount();l++)
	{
		if (isLeadVisible(l, pDC))
		{
			const Lead& lead = _method->getLead(l);
			CSize orig = getLeadOrigin(l);
		
			CRect rcRow(orig.cx - (_style._width/2) + 1,
						0,
						orig.cx + (_style._width * (_method->getNumber()-1)) + (_style._width/2),
						0);

			for (int j=0;j<lead.GetSize();j++)
			{
				if (lead.GetAt(j)->isFalse()) //todo check how this works for the first and last row
				{  
					drawColor = color;
					
					if (_falseRow)
					{
						for (int i=0;i<_falseRow->GetSize();i++)
							if (lead.GetAt(j)->getRowIndex() == _falseRow->GetAt(i)->getMethodRow())
							{
								drawColor = hilight;								
								break;
							}
					}

					//get the rect size
					rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
					rcRow.bottom = rcRow.top + _style._height-1;

					pDC->FillSolidRect(rcRow,drawColor);
				}
				else if (lead.GetAt(j)->isCommon()) //check how this works in for the first and last rows.
				{  
					if (_falseRow)
					{
						for (int i=0;i<_falseRow->GetSize();i++)
						{
							if (lead.GetAt(j)->getRowIndex() == _falseRow->GetAt(i)->getMethodRow())
							{
								//get the rect size
								rcRow.top    = orig.cy + (_style._height*j) - (_style._height/2);
								rcRow.bottom = rcRow.top + _style._height-1;

								pDC->FillSolidRect(rcRow,hilight);

								break;
							}
						}
					} 
				}
			}	 
		}
	}  
}


CSize MethodWnd::getLeadOrigin(int leadNo)
{
	return _leadOrigins.GetAt(leadNo);
}

const CRect& MethodWnd::getLeadBox(int leadNo)
{
	return _leadBoxs.GetAt(leadNo);
}



//everything is zero based (apart from bell)
bool MethodWnd::rowHitTest(const CPoint & point, MethodPosition * pos, bool /*forStatus*/)
{
	//todo make sure this is streem lined. It gets called a lot.

	//get lead
	int lead = leadHitTest(point);
	if (lead == -1) 
	{
		pos->setInvalid();
		return false;
	}
	const CRect& leadBox = getLeadBox(lead);

	//get row
	int leadRow = ((point.y - leadBox.top) / _style._height) ;
	pos->setRow(_method, lead, leadRow);
	//get lead line
	pos->setLeadLine((point.y - (_style._height/2) - leadBox.top) / _style._height);

	//get place
	pos->setPlace((point.x - leadBox.left) / _style._width);

	//get bell
	pos->setBell(_method->getLead(pos->getLead()).GetAt(pos->getLeadRow())->getBellInPlace(pos->getPlace()));

	//get stroke
	pos->setStroke(_method->getRow(*pos)->getStroke());

	return true;   
}


int MethodWnd::leadHitTest(const CPoint &point)
{
	int lead = -1 ;
	for (int i=0;i<_method->getLeadCount();i++)
	{
		if (getLeadBox(i).PtInRect(point))
			lead = i; // we still continue, as another lead may be overlapping, in which case we want the highest numbered lead.
	} 	
	return lead;
}

void MethodWnd::SetScrolls()
{
	if (_method)
	{	
		//recalculate the _leadOrigins if not valid, as we need them for the paint, even if not correct.
		if ((_leadOrigins.GetSize() != _method->getLeadCount())||
			(_leadBoxs.GetSize() != _method->getLeadCount()))
		{
			recalcLeadOrigins();
			recalcLeadBoxes();
		}

		//force a repaint so that the title size can be calculated
		OnPaint();
		
		//recalculate the _leadOrigins with the new title size 
		//done before the getSize, as this needs the origins and boxes to be accurate
		recalcLeadOrigins();
		recalcLeadBoxes();

		CSize size = getSize();
		if ((app()->_windowsVersion == VER_PLATFORM_WIN32_WINDOWS)&&
			((size.cx >= SHRT_MAX)||
			 (size.cy >= SHRT_MAX)))
		{
			_16BitOverload = true;
			SetScrollSizes(500,200);
			Invalidate();
		}
		else
		{
			_16BitOverload = false;			
			SetScrollSizes(getSize(), 
							CSize((_style._width * (_method->getNumber()-1)) + _style._left, 
							_style._height * ((_method->getLeadCount() > 0)?(_method->getLead(0).GetSize()):10)), 
							CSize(_style._width    , _style._height    ));
		}
	}
	else 
		SetScrollSizes(10,10);

	Invalidate();
}	

CSize MethodWnd::getSize()
{
	CSize size(0,0);

	if (!_method)
		return size;
	
	//width 
	size.cx = getLeadBox(_method->getLeadCount()-1).right + (int)((float)_style.getLeft()*1.5);
	
	//height
	for (int i=0;i<_method->getLeadCount();i++)
	{
		if (getLeadBox(i).bottom > size.cy)
		size.cy = getLeadBox(i).bottom;
	}					  
	size.cy += _style._top;

	return size;		   
}
	
BOOL MethodWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		switch (_hitType)
		{
		case MWRHT_LEFT:
			SetCursor(app()->LoadStandardCursor(IDC_SIZEWE)); 
			return TRUE;
		case MWRHT_RIGHT:
			SetCursor(app()->LoadStandardCursor(IDC_SIZEWE)); 
			return TRUE;
		case MWRHT_TOP:
			SetCursor(app()->LoadStandardCursor(IDC_SIZENS)); 
			return TRUE;
		case MWRHT_BOTTOM:
			SetCursor(app()->LoadStandardCursor(IDC_SIZENS)); 
			return TRUE;
		default:
			SetCursor(app()->LoadStandardCursor(IDC_CROSS)); //IDC_SMALL_CROSS	   
			return TRUE;
		}
	}

	return ScrollWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL MethodWnd::Create(DWORD dwExStyle, 
					   LPCTSTR lpszWindowName, 
					   DWORD dwStyle, 
					   const RECT& rect, 
					   CWnd* pParentWnd, 
					   UINT nID, 
					   Method * method) 
{
	BOOL result = ScrollWnd::Create(dwExStyle, lpszWindowName, dwStyle, rect, pParentWnd, nID) ;

	_tip.Create(this);		
	_method = method;

	SetScrolls();

	OnSetMessageString(AFX_IDS_IDLEMESSAGE,0);

	SetTimer(AAA_MOUSE_LEFT_WND,500,NULL);

	return result;	
}

void MethodWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	if (_method)
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;

		//get the appropriate menu
		VERIFY(menu.LoadMenu (getContectMenuId()));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		//this is needed to allow mainframe to access the colors
		mainFrame()->_pMethodWnd = this;

		styleManager()->setupStyleMenu (pPopup->GetSubMenu(getStyleMenuIndex()));

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create (this, point.x, point.y, pPopup->Detach(), FALSE, TRUE);	

		//set to null to protect against a hanging pointer
		mainFrame()->_pMethodWnd = NULL;	
	}
}

void MethodWnd::OnToolsStyle() 
{
	doToolsStyle();
}

void MethodWnd::doToolsStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _method?_method->getNumber():MAXBELLS);
	styleEditDlg.DoModal();
	notifyStyleChange();
	SetScrolls();
}

void MethodWnd::OnUpdateToolsStyle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_method)&&
				   (_method->getNumber() >=3) &&
 				   (_method->getNumber() <= MAXBELLS));	
}

void MethodWnd::OnLinewidth1() 
{
	MethodPosition pos;
	
	if (rowHitTest(_contextMouseDown, &pos))
	{
		_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth = 1;
		Invalidate();
	}
	notifyStyleChange();
}

void MethodWnd::OnUpdateLinewidth1(CCmdUI* pCmdUI) 
{
	MethodPosition pos;
	if ((_method)&&
		(rowHitTest(_contextMouseDown, &pos))&&
		(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineColor != -1))
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth == 1);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}

void MethodWnd::OnLinewidth2() 
{
	MethodPosition pos;
	
	if (rowHitTest(_contextMouseDown, &pos))
	{
		_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth = 2;
		Invalidate();
	}
	notifyStyleChange();
}

void MethodWnd::OnUpdateLinewidth2(CCmdUI* pCmdUI) 
{
	MethodPosition pos;
	if ((_method)&&
		(rowHitTest(_contextMouseDown, &pos))&&
		(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineColor != -1))
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth == 2);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}

void MethodWnd::OnLinewidth3() 
{
	MethodPosition pos;
	
	if (rowHitTest(_contextMouseDown, &pos))
	{
		_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth = 3;
		Invalidate();
	}
	notifyStyleChange();
}

void MethodWnd::OnUpdateLinewidth3(CCmdUI* pCmdUI) 
{
	MethodPosition pos;
	
	if ((_method)&&
		(rowHitTest(_contextMouseDown, &pos))&&
		(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineColor != -1))
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(_style.getLine(pos.getBell()-1, _method->getNumber())->_lineWidth == 3);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(FALSE);
	}
}	


void MethodWnd::OnLinecolor() 
{
	COLORREF color = CBCGPColorMenuButton::GetColorByCmdID (ID_LINECOLOR);	
	MethodPosition pos;
	if (rowHitTest(_contextMouseDown, &pos))
	{
		_style.getLine(pos.getBell()-1, _method->getNumber())->_lineColor = color;
		Invalidate();
	}
	notifyStyleChange();
}

void MethodWnd::OnUpdateLinecolor(CCmdUI* pCmdUI) 
{
	MethodPosition pos;
	pCmdUI->Enable((_method)&&rowHitTest(_contextMouseDown, &pos));
}

void MethodWnd::OnTextcolor() 
{
	COLORREF color = CBCGPColorMenuButton::GetColorByCmdID (ID_TEXTCOLOR);	
	MethodPosition pos;
	if (rowHitTest(_contextMouseDown, &pos))
	{
		_style.getLine(pos.getBell()-1, _method->getNumber())->_textColor = color;
		Invalidate();
	}
	notifyStyleChange();
}


void MethodWnd::OnApplyStyle(UINT nID) 
{
	int index = nID - ID_STYLE_MENU_00;
	if ((index>=0)&&(index < styleManager()->getStyleCount()))
	{
		notifyStyleChange(false);
		_style = *styleManager()->getStyle(index);
		SetScrolls();
	}
	else 
	{
		ASSERT(FALSE);
	}
}


void MethodWnd::OnStyleMenuMore() 
{
	//fill the list.
	CStringArray strings;
	for (int i=0;i< styleManager()->getStyleCount();i++)
	{
		strings.Add(styleManager()->getStyle(i)->_name);
	}	

	MoreDlg dlg(this, strings, "Style");
	if (dlg.DoModal() == IDOK)
	{
		int index = dlg.getIndex();
		if ((index>=0)&&(index < styleManager()->getStyleCount()))
		{
			_style = *styleManager()->getStyle(index);
			SetScrolls();
		}
	}	 	
}


void MethodWnd::OnStyleMenuSaveAs() 
{
	//get the name of the new style
	GenericEditDlg dlg(NULL, "Save Current Style As", "Enter style name", "Enter style name" );

	do 
	{
		if (dlg.DoModal() == IDCANCEL) return;
		dlg._strGenericEdit = GlobalFunctions::titleString(dlg._strGenericEdit);
		if (!styleManager()->isStyleUnique(dlg._strGenericEdit))
		{
			CString str;
			if (AfxMessageBox(dlg._strGenericEdit + " already exists. Do you want to replace it?", MB_YESNO|MB_ICONQUESTION) == IDYES)
			{
				int index = styleManager()->getStyleIndexFromName(dlg._strGenericEdit);
				if (index > 0) //dont want to alter the default style
				{
					Style style(_style);
					style._changed = false;
					style._name = dlg._strGenericEdit ;
					styleManager()->setAt(index,style);

					notifyStyleChange(false);
					return;
				}
			}
		}
		else
		{
			Style style(_style);
			style._changed = false;
			style._name = dlg._strGenericEdit ;
			styleManager()->addStyle(style);
			notifyStyleChange(false);
			return;												 
		}
	}while(true);	
}

void MethodWnd::OnUpdateStyleMenuSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_method)&&_style._changed); 	
}

LRESULT MethodWnd::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{	
	//pass on to parent
	::PostMessage(GetParent()->m_hWnd, WM_SETMESSAGESTRING, wParam, lParam); 
	return 0;
}

void MethodWnd::setMethod(Method *method)
{
	//apply the method 
	_method = method;
	
	//clear out old refs
	_highlightRow = NULL;
	_falseRow = NULL;
	_contextRow = NULL;

	//force out a recalc
	if (IsWindow(m_hWnd))SetScrolls();
}



void MethodWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == AAA_MOUSE_LEFT_WND)
	{
		//detect mouse leaving window
		CRect rect;
		GetWindowRect(&rect);
		CPoint pt;
		GetCursorPos(&pt);
		if (!rect.PtInRect(pt))
		{
			//notify the parent
			MethodPosition * pos = new MethodPosition();
			::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, FALSE, (LPARAM)pos);
			//for the false row highlight
			_falseRow = NULL;
			Invalidate(); 
		}
	}

	ScrollWnd::OnTimer(nIDEvent);
}

void MethodWnd::OnDestroy() 
{
  	KillTimer(AAA_MOUSE_LEFT_WND);

	ScrollWnd::OnDestroy();	

}

void MethodWnd::recalcLeadOrigins()
{
	CSize origin;

	_leadOrigins.RemoveAll();
	
	if (_method == NULL)
		return;

	for (int leadNo=0;leadNo<_method->getLeadCount();leadNo++)
	{
		//origins
		origin.cx = _style.getLeft();
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
			origin.cy +=  (_method->getLead(i).GetUpperBound() * 
							_style._height);
		}				 	

		_leadOrigins.Add(origin);

	}
}

void MethodWnd::recalcLeadBoxes()
{
	CRect box;
	CSize origin;

	_leadBoxs.RemoveAll();
	
	if (_method == NULL)
		return;

	for (int leadNo=0;leadNo<_method->getLeadCount();leadNo++)
	{
		origin = getLeadOrigin(leadNo);
		//lead box
		box.top    = origin.cy - (_style._height/2);
		box.left   = origin.cx - (_style._width/2);
		box.bottom = origin.cy + (_method->getLead(leadNo).GetSize() * _style._height) - (_style._height/2);
		box.right  = origin.cx + (_method->getNumber() * _style._width) - (_style._width/2);

		_leadBoxs.Add(box);
	}
}

LRESULT MethodWnd::OnUpdateMethod(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	SetScrolls();
	return 0;
}



void MethodWnd::OnSetEndAtRow() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
	if ((pTouchDoc)&&(_contextRow))
	{
		pTouchDoc->setEndWithChange(_contextRow->getAsText());
	}	
}

void MethodWnd::OnUpdateSetEndAtRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_method)&&
				   (_contextRow)&&
				   (mainFrame()->getActiveTouchDoc()));	
}

void MethodWnd::OnSetStartAtRow() 
{
	TouchDoc* pTouchDoc = mainFrame()->getActiveTouchDoc();
	if ((pTouchDoc)&&(_contextRow))
	{
		pTouchDoc->setStartWithChange(_contextRow->getAsText());
	}	
}

void MethodWnd::OnUpdateSetStartAtRow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_method)&&
				   (_contextRow)&&
				   (mainFrame()->getActiveTouchDoc()));	
}

void MethodWnd::hilightRow(const RowPosition& rowPosition) 
{
	if (_method)
	{
		_highlightRow = _method->getRow(rowPosition);
		scrollToRow(rowPosition);	
	}
	Invalidate();
}

void MethodWnd::hilightRow(Row* hilightRow) 
{
	_highlightRow = hilightRow;

	//scroll
	if ((_highlightRow)&&(_method))
	{
		for (int i=0;i<_method->getLeadCount();i++)
		{
			const Lead& lead = _method->getLead(i);
			for (int j=0;j<lead.GetSize();j++)
			{
				if (_highlightRow == lead.GetAt(j))
				{
					RowPosition rowPosition;
					rowPosition.setRow(_method, i,j);
					scrollToRow(rowPosition);
				}
			}
		}
	}

	Invalidate();
}


Row* MethodWnd::getHilightRow()
{
	return _highlightRow;
}

void MethodWnd::scrollToRow(RowPosition rowPosition, bool force)
{
	if (!rowPosition.isValid())
		return;

	CRect client;
	GetClientRect(&client);

	//work out the box for the row
//	CPoint topLeft = getLeadOrigin(rowPosition.getLead());

	CRect rect = getLeadBox(rowPosition.getLead());
	rect.top += (_style._height*rowPosition.getLeadRow());
	rect.bottom = rect.top + _style._height;


	//dont scroll if visible
	CRect scrollBox = client + GetScrollPosition();
	//TRACE("scroll bottom %d, rect.bottom %d \r\n", scrollBox.bottom, rect.bottom);

	if (!force && 
		scrollBox.PtInRect(rect.TopLeft()) &&
		scrollBox.PtInRect(rect.BottomRight()))
		return;
	
	if (_totalSize.cx < client.Width())
	{
		rect.left = 0;
		rect.right = 0;
	}

	if (_totalSize.cy < client.Height())
	{
		rect.top = 0;
		rect.bottom = 0;
	}

	ScrollToPosition(rect.TopLeft());

}

void MethodWnd::OnFilePrint() 
{
	::PostMessage(GetParent()->m_hWnd,WM_COMMAND, ID_FILE_PRINT,0);	
}

void MethodWnd::OnEditFind() 
{
	::PostMessage(GetParent()->m_hWnd,WM_COMMAND, ID_EDIT_FIND,0);	
}

void MethodWnd::OnEditGoto() 
{
	::PostMessage(GetParent()->m_hWnd,WM_COMMAND, ID_EDIT_GOTO,0);	
}

int MethodWnd::getNumber()
{
	return _method->getNumber();
}



void MethodWnd::notifyStyleChange(bool flagChange)
{
	//notify the parent
	::PostMessage(GetParent()->m_hWnd, WM_STYLE_CHANGED, NULL, NULL);
	
	//mark the style as changed for 'save style as'
	_style._changed = flagChange;
}

void MethodWnd::OnEditCopy() 
{
	//put your text in source
	if(OpenClipboard())
	{
		EmptyClipboard();

		CString strSource = getCopyDataString();
		
		HGLOBAL strBuff;
		char * buffer;
		
		//str
		strBuff = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+1);
		buffer = (char*)GlobalLock(strBuff);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(strBuff);
		SetClipboardData(CF_TEXT,strBuff);

		//bitmap
		CBitmap* bit = getCopyDataBitmap();
			
		::SetClipboardData (CF_BITMAP, bit->GetSafeHandle() ) ;

		bit->Detach();

		delete bit;

		CloseClipboard();
	}
}

CString MethodWnd::getCopyDataString()
{
	return "";//todo _method->getCopyDataString();
}

void MethodWnd::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_method != NULL);
}

CBitmap* MethodWnd::getCopyDataBitmap()
{
	CBitmap* memBitmap = new CBitmap(); //my new bitmap
	CBitmap* oldBitmap = NULL; // bitmap originally found in memDC

   	//device contexts
	CDC* pDC = GetDC();
	CDC bitmapDC;

	// Create a Bitmap DC
	bitmapDC.CreateCompatibleDC(pDC);
	
	//set the map mode
	pDC->SetMapMode(MM_TEXT);
	bitmapDC.SetMapMode(MM_TEXT);

	//get the whole client size and offset to paint the correct area
	CSize size = getSize();

	//create a bitmap, and select into the memDC
	memBitmap->CreateCompatibleBitmap(pDC, size.cx, size.cy);
	oldBitmap = bitmapDC.SelectObject(memBitmap);
	
	// by default shift viewport origins in negative direction of scroll
/*	memDC.SetViewportOrg(-GetScrollPosition());
	dc.SetViewportOrg(-GetScrollPosition());*/ //I have left this code here for when we have the ability to copy a sub set of the image

	
	//erase the background
    bitmapDC.FillSolidRect(0,0,size.cx, size.cy, GetSysColor(COLOR_WINDOW));  

	//perform the GDI calls
	OnDraw(&bitmapDC);

	
	//Swap back the original bitmap.
	bitmapDC.SelectObject(oldBitmap);

	ReleaseDC(pDC);

	return memBitmap;
}



void MethodWnd::OnMethodOptions() 
{
	mainFrame()->PostMessage(WM_COMMAND, ID_METHOD_OPTIONS);
}

const int HitTestBorder = 2;

LRESULT MethodWnd::OnNcHitTest(CPoint point) 
{		 				
	_hitType = MWRHT_NONE;
	const UINT nHitTest = ScrollWnd::OnNcHitTest(point);
	if ((nHitTest == HTCLIENT)&&(_method)&&(_method->getLeadCount()>0)&&(!_16BitOverload))
	{
		CRect rect = getLeadBox(0);
		rect.left += _style._width / 2;
		rect.right -= _style._width / 2;
		rect.top += _style._height/ 2;
		rect.bottom -= _style._height/ 2;

		CPoint localPoint = point;
		ScreenToClient(&localPoint);
		localPoint += GetScrollPosition();

		if ((localPoint.x > rect.left - HitTestBorder)&&
			(localPoint.x < rect.left + HitTestBorder))
			_hitType = MWRHT_LEFT;
		if ((localPoint.x > rect.right - HitTestBorder)&&
			(localPoint.x < rect.right + HitTestBorder))
			_hitType = MWRHT_RIGHT;
		if ((localPoint.y > rect.top - HitTestBorder)&&
			(localPoint.y < rect.top + HitTestBorder))
			_hitType = MWRHT_TOP;
		if ((localPoint.y > rect.bottom - HitTestBorder)&&
			(localPoint.y < rect.bottom + HitTestBorder))
			_hitType = MWRHT_BOTTOM;
	}

	return nHitTest;
}

void MethodWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_resizing)
	{
		doResizeLineInvert();
		
		setResizePos(point);

		doResizeLineInvert();
	}
	else if(_method)
	{	

		_contextMouseDown = point + GetScrollPosition();

		MethodPosition * pos = new MethodPosition();

		//Effectivly switches the tip off
		_tip.removeAllStrings(); 

		BOOL result = rowHitTest(_contextMouseDown, pos, true);

		_contextRow = _method->getRow(*pos);

		/*	todo we need to change the way the false rows stuff works
		//get the list of false rows that this is over. if any.
		FalseRow* falseRow = _method->getFalseRow(_contextRow);
		if (falseRow != _falseRow)
		{
			_falseRow = falseRow;
			Invalidate();
		}	 

		//sort tool tips
		if (result)
		{
			CString str;
			if(falseRow)
			{
				if (_contextRow->isCommon())
					str = falseRow->getTipText(*pos, true);
				else if (_contextRow->isFalse())
					str = falseRow->getTipText(*pos, false);

			} 			

			//default string
			if (str.IsEmpty())
				str = pos->format(rpft_full);

			_tip.addString(str);
			_tip.Set(point); //should be without scroll
		}
			 */
		//notify the parent
		::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, result, (LPARAM)pos);
	}

	ScrollWnd::OnMouseMove(nFlags, point);
}

void MethodWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	_resizing = false;

	if(_method)
	{
		if (_hitType != MWRHT_NONE)
		{
			CRect windowRect;
			GetWindowRect(windowRect);
			//do not let mouse leave the list box boundary
			::ClipCursor(windowRect);

			_resizeType = _hitType;
			_resizing = true;
			
			setResizePos(point);

			doResizeLineInvert();

			//capture the mouse
			SetCapture();	 
		}
		else
		{
			CPoint lDown = point + GetScrollPosition();

			MethodPosition pos ;

			rowHitTest(lDown , &pos, true);

			_highlightRow = _method->getRow(pos);

			Invalidate();
		}
	}

	ScrollWnd::OnLButtonDown(nFlags, point);
}



void MethodWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_resizing)
	{
		//if columns were being resized then this indicates
		//that mouse is up so resizing is done.  Need to redraw
		//columns to reflect their new widths.
		
		//if mouse was captured then release it
		if (GetCapture()==this)
			::ReleaseCapture();

		::ClipCursor(NULL);

		setResizePos(point);

		if (_resizeType == 	MWRHT_LEFT)
		{
			_style._left = _resizePos - _style._notationWidth  ;
		}
		else if (_resizeType == MWRHT_RIGHT)
		{			
			_style._width = (_resizePos - _style.getLeft()) / (getNumber()-1);
		}
		else if (_resizeType == MWRHT_TOP)
		{
			_style._top = _resizePos - _style._titleHeight;
		}
		else if(_resizeType == 	MWRHT_BOTTOM)
		{
			if (_method->getLead(0).GetSize() > 1) //stop devide by zero
				_style._height = (_resizePos - _style.getTop()) / (_method->getLead(0).GetSize()-1);
		}

		//redraw
		_leadOrigins.RemoveAll();
		_leadBoxs.RemoveAll();
		notifyStyleChange();
		SetScrolls();
		Invalidate();
	}
	
	_resizing = FALSE;
	ScrollWnd::OnLButtonUp(nFlags, point);
}

void MethodWnd::doResizeLineInvert()
{
	CClientDC dc(this);

	CRect clientRect;
	GetClientRect(clientRect);

	if ((_resizeType == 	MWRHT_LEFT)||
		(_resizeType == 	MWRHT_RIGHT))
	{
		SetCursor(app()->LoadStandardCursor(IDC_SIZEWE));
		GlobalFunctions::InvertLine(&dc,CPoint(_resizePos,clientRect.top),CPoint(_resizePos,clientRect.bottom)); 
	}
	else if ((_resizeType == 	MWRHT_TOP)||
		(_resizeType == 	MWRHT_BOTTOM))
	{
		SetCursor(app()->LoadStandardCursor(IDC_SIZENS));
		GlobalFunctions::InvertLine(&dc,CPoint(clientRect.left, _resizePos),CPoint(clientRect.right,_resizePos)); 
	}

	//set cursor
}


void MethodWnd::setResizePos(CPoint point)
{
	if (_resizeType == 	MWRHT_LEFT)
	{
		_resizePos = point.x;
		if (_resizePos < _style._notationWidth + 1 )
			_resizePos = _style._notationWidth + 1 ;
		if (_resizePos > _style._notationWidth + 500 )
			_resizePos = _style._notationWidth + 500;
	}
	else if (_resizeType == MWRHT_RIGHT)
	{
		_resizePos = point.x;
		int pos = _resizePos - _style.getLeft();
	
		pos /= (getNumber() -1);
		
		if (pos < 1)
			pos = 1;
		if (pos > 500)
			pos = 500;
		_resizePos = _style.getLeft() + ((getNumber() -1) * pos);
	}
	else if (_resizeType == MWRHT_TOP)
	{
		_resizePos = point.y;
		if (_resizePos < _style._titleHeight + 1)
			_resizePos = _style._titleHeight + 1;
		if (_resizePos > _style._titleHeight + 500)
			_resizePos = _style._titleHeight + 500;
	}
	else if(_resizeType == 	MWRHT_BOTTOM)
	{
		_resizePos = point.y;
		int pos = _resizePos - _style.getTop();
	
		if (_method->getLead(0).GetSize() > 1) //otherwise device by zero error.
			pos /= (_method->getLead(0).GetSize() -1);
		
		if (pos < 1)
			pos = 1;
		if (pos > 500)
			pos = 500;
		_resizePos = _style.getTop() + ((_method->getLead(0).GetSize() -1) * pos);
	}
}

int MethodWnd::getStyleMenuIndex()
{
	return 0;
}

// caller responsable for deleting
CxImage* MethodWnd::getImage()
{
	CxImage* image = new CxImage();

	CBitmap* bit = getCopyDataBitmap();
		
	image->CreateFromHBITMAP((HBITMAP)bit->GetSafeHandle());

	bit->Detach();

	delete bit;		

	return image;
}

ShouldDrawType MethodWnd::shouldDrawRow(int /*rowIndex*/, int /*place*/)
{
	return sdt_yes;
}

BOOL MethodWnd::isLeadVisible(int lead, CDC* pDC)
{
	CRect leadRect(getLeadBox(lead));
	leadRect.InflateRect(100,100);
	return pDC->RectVisible(leadRect);
}