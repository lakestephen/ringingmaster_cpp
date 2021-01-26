// MethodEditWnd.cpp: implementation of the MethodEditWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodEditDlg.h"
#include "StyleEditDlg.h"
#include "RegArchive.h"
#include "MethodEditWnd.h"
#include "UserMessages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodEditWnd::MethodEditWnd() :
_width(0),
_height(0),
_left(0),
_top(0),
_drawing(false),
_mouseDown(0,0),
_drawStart(0,0),
_drawLength(0),
_drawType(none),
_blankStartSize(0)
{
   	//{{AFX_DATA_INIT(MethodEditWnd)
	//}}AFX_DATA_INIT  

}

MethodEditWnd::~MethodEditWnd()
{
	destroyLines();
}

  
IMPLEMENT_DYNAMIC(MethodEditWnd, ScrollWnd)

BEGIN_MESSAGE_MAP(MethodEditWnd, ScrollWnd)
	//{{AFX_MSG_MAP(MethodEditWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()	
	ON_COMMAND(ID_POPUP_ADDWORK_SINGLEDODGE, OnPopupAddworkSingledodge)
	ON_COMMAND(ID_POPUP_ADDWORK_DOUBLE_DODGE, OnPopupAddworkDoubleDodge)
	ON_COMMAND(ID_POPUP_ADDWORK_PLACESDOWN, OnPopupAddworkPlacesdown)
	ON_COMMAND(ID_POPUP_ADDWORK_PLACESUP, OnPopupAddworkPlacesup)
	ON_COMMAND(ID_POPUP_ADDWORK_YORKSHIREDOWN, OnPopupAddworkYorkshiredown)
	ON_COMMAND(ID_POPUP_ADDWORK_YORKSHIREUP, OnPopupAddworkYorkshireup)
	ON_COMMAND(ID_POPUP_ADDWORK_CAMBRIDGEDOWN, OnPopupAddworkCambridgedown)
	ON_COMMAND(ID_POPUP_ADDWORK_CAMBRIDGEUP, OnPopupAddworkCambridgeup)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_SINGLEDODGE, OnUpdatePopupAddwork)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_DELETE, OnPopupDelete)
	ON_UPDATE_COMMAND_UI(ID_POPUP_DELETE, OnUpdatePopupDelete)
	ON_COMMAND(IDC_METHOD_DRAW_CLEAR_ALL, OnMethodDrawClearAll)
	ON_UPDATE_COMMAND_UI(IDC_METHOD_DRAW_CLEAR_ALL, OnUpdateMethodDrawClearAll)
	ON_COMMAND(IDC_METHOD_DRAW_STYLE, OnMethodDrawStyle)
	ON_COMMAND(IDC_METHOD_DRAW_UNDO, OnMethodDrawUndo)
	ON_UPDATE_COMMAND_UI(IDC_METHOD_DRAW_UNDO, OnUpdateMethodDrawUndo)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_DOUBLE_DODGE, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_PLACESDOWN, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_PLACESUP, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_YORKSHIREDOWN, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_YORKSHIREUP, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_CAMBRIDGEDOWN, OnUpdatePopupAddwork)
	ON_UPDATE_COMMAND_UI(ID_POPUP_ADDWORK_CAMBRIDGEUP, OnUpdatePopupAddwork)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_METHOD, OnUpdateMethod)
END_MESSAGE_MAP()

LRESULT MethodEditWnd::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{	
	//pass on to parent
	::PostMessage(GetParent()->m_hWnd, WM_SETMESSAGESTRING, wParam, lParam); 
	return 0;
}

void MethodEditWnd::createLines(int width, int height)
{
	destroyLines();
	_width = width;
	_height = 1;
	for (int i=0;i<height-1;i++)
	{
		addRow();
	}  
	updateDimentions();
}

void MethodEditWnd::addRow()
{
	for (int i=0;i<_width;i++)
	{
		_lines.Add(new Line());
	}
	_height++; 
}

void MethodEditWnd::removeRow()
{
	if (_height > 2)
	{
		_height--;
		for (int i=_lines.GetSize() - _width;i<_lines.GetSize();i++)
		{
			delete _lines.GetAt(i);
			_lines.SetAt(i, NULL) ;
		}
		_lines.SetSize(_lines.GetSize() - _width);
	}
	
	updateDimentions();
}

void MethodEditWnd::destroyLines()
{
	for (int i=0;i<_lines.GetSize();i++)
	{
		delete _lines.GetAt(i);
		_lines.SetAt(i,NULL);
	}
	_lines.RemoveAll();
	_lines.FreeExtra();
	_undo.RemoveAll();
	_undo.FreeExtra();
	_width = 0;
	_height = 0;
}

void MethodEditWnd::initLines()
{
	for (int i=0;i<_lines.GetSize();i++)
	{
		_lines[i]->_type = none;		
		_lines[i]->_number = NOBELL;
	}
	_undo.RemoveAll();
	_undo.FreeExtra();
}

MethodEditWnd::Line * MethodEditWnd::getLine(int x, int y)
{
	ASSERT((((y*_width) + x) < _lines.GetSize())&&
		   (((y*_width) + x) >= 0));
	return _lines[(y*_width) + x];
}

void MethodEditWnd::OnDraw(CDC* pDC)
{
	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);

	drawGrid(pDC);	
	drawNumbers(pDC);
	drawLines(pDC);
	drawNotation(pDC);
}	   

void MethodEditWnd::drawGrid(CDC *pDC)
{
	CPen penV(PS_SOLID, _style._gridWidthV, _style._gridColorV);
	CPen penH(PS_SOLID, _style._gridWidthH, _style._gridColorH);

	//columns
	CPen * oldPen = pDC->SelectObject(&penV);
	if (_style._gridColorV != -1)
	{
		for (int i=0;i< _width + 1;i++)
		{
			pDC->MoveTo(_left + (_style._width * i), _top);
			pDC->LineTo(_left + (_style._width * i), 
									_top + (_style._height * _height));
		}
	}

	//rows
	pDC->SelectObject(&penH);
	if (_style._gridColorH)
	{
		for (int j=0;j< _height + 1;j++)
		{
			pDC->MoveTo(_left, _top + (_style._height * j));
			pDC->LineTo(_left + (_style._width * _width), 
									_top + (_style._height * j));
		}
	}

	pDC->SelectObject(oldPen);
}

void MethodEditWnd::drawLines(CDC *pDC)
{
	int i,j;
	//create all the pens possable
	CPen * defaultPen = new CPen;
	CPen * pens = new CPen[_width]; 

	if (!defaultPen->CreatePen(PS_SOLID, 1, RGB(0,0,0))) return;

	for (i=0;i<_width;i++)
		if (!pens[i].CreatePen(PS_SOLID, _style.getLine(i, getNumber())->_lineWidth, 
									  _style.getLine(i, getNumber())->_lineColor))	return;

	CPen * oldPen = pDC->SelectObject(defaultPen);
	
	//columns
	for (i=0;i<_width;i++)
	{
		for (j=0;j<_height-1;j++)
		{
			Line * line = getLine(i,j);
			if (line->_type != none)
			{
				if (line->_number == NOBELL)
					pDC->SelectObject(defaultPen);
				else 
					pDC->SelectObject(pens[line->_number]);
				
				pDC->MoveTo(posToPoint(CSize(i,j), false));
				pDC->LineTo(posToPoint(CSize(i + (int)line->_type, j+1), false));
			}
		}
	}

	//clean up
	pDC->SelectObject(oldPen);
	delete [] pens;
	pens = NULL;
	delete defaultPen;
	defaultPen = NULL;
}

void MethodEditWnd::drawNumbers(CDC *pDC)
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

	int i,j;

	for (i=0;i<_width;i++)
	{
		for (j=0;j<_height-1;j++)
		{
			Line * line = getLine(i,j);
			if ((line->_type != none)&&(line->_number != NOBELL))
			{
				Style::StyleLine* stLine = _style.getLine(line->_number, getNumber());

				if (stLine->_textColor != -1) //only process if visible 
				{
					pDC->SetTextColor(stLine->_textColor);
					
					CPoint startPoint = posToPoint(CSize(i,j), false) + offset;
					CPoint endPoint = posToPoint(CSize(i + (int)line->_type, j+1), false) + offset;
					pDC->TextOut(startPoint.x, startPoint.y , CString(GlobalFunctions::bellNumbersToChar(line->_number +1)));
					pDC->TextOut(endPoint.x, endPoint.y , CString(GlobalFunctions::bellNumbersToChar(line->_number +1)));
				}
			}
		}
	}



	pDC->SelectObject(oldFont);

}
void MethodEditWnd::drawNotation(CDC *pDC)
{
	if (_style._notationShow)
	{
		//create the font
		CFont title, method;
		LOGFONT lf;
		
		//setup method font
		FontInfo methodFontInfo = _style._notationFont;
		methodFontInfo.getLogFont(&lf);
		method.CreateFontIndirect(&lf);
		
		//set up DC
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetTextColor(_style._notationFont._color);
		
		//write the method text out
		CFont * oldFont = pDC->SelectObject(&method);
		
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		int halfTextHeight = tm.tmHeight/2;	

		CString notation;
	
		bool even = GlobalFunctions::isEven(getNumber());
		
		for (int j=0;j< _height - 1;j++)
		{
			bool placeMade = false;
			notation.Empty();

			for (int i=0;i<_width;i++)
			{
				if (getLine(i,j)->_type == down)
				{
					{
						notation += GlobalFunctions::bellNumbersToChar(i+1);
						placeMade = true;
					}
				}
			}
			if ((!placeMade)&&(even))
			{
				notation = GlobalFunctions::bellNumbersToChar(ALLCHANGE);
			}

			pDC->TextOut(_left + 3 +  (_style._width  * _width), 
						 _top  +      (_style._height * j) + halfTextHeight,//+ (_style._height/2),
						 notation);
		}
		
		//clean up
		pDC->SelectObject(oldFont);
	}
}

//parent must allways be of type RuleWnd
MethodEditDlg * MethodEditWnd::GetParent()
{
	CWnd * pParent = CWnd::GetParent();
	
	//check parent is of the correct type
	if (pParent->IsKindOf(RUNTIME_CLASS(MethodEditDlg)))
	{
		return (MethodEditDlg*)pParent;
	}
	
	//default
	return NULL;
}



void MethodEditWnd::updateDimentions()
{
	SetScrollSizes(getDimensions(styleBorder));
	centre();
}


CSize MethodEditWnd::pointToPos(const CPoint &point)
{
	CSize pos = point + GetScrollPosition();
	pos.cx -= _left;
	pos.cx = (pos.cx - ((pos.cx < 0)?_style._width:0))/_style._width;
	pos.cy -= _top;
	pos.cy = (pos.cy - ((pos.cy < 0)?_style._height:0))/_style._height;	
	return pos;
}

CPoint MethodEditWnd::posToPoint(const CSize &pos, bool incScroll)
{
	return CPoint(_left + (pos.cx * _style._width ) + (_style._width  / 2), 
				  _top  + (pos.cy * _style._height) + (_style._height / 2)) 
				  - ((incScroll)?GetScrollPosition() : CSize(0,0));
}

CPoint MethodEditWnd::snapToGrid(const CPoint &point)
{
	return posToPoint(pointToPos(point));
}

// 3 state.
// noBorder, styleBorder, actualBorder
CSize MethodEditWnd::getDimensions(dimension dim)
{
	switch (dim)
	{
	case noBorder:
		return CSize(_style._width  * _width, _style._height * _height);
	case styleBorder:
		return CSize((_style._left * 2)+(_style._width  * _width), (_style._top * 2)+(_style._height * _height));
	case actualBorder:
		return CSize((_left * 2)+(_style._width  * _width), (_top * 2)+(_style._height * _height));
	default:
		return CSize(0,0);
	}
}

bool MethodEditWnd::isValid()
{
	for (int i=0;i<_lines.GetSize();i++)
	{
		if 	(_lines[i]->_type == none)
		{
			return false;
		}
	}
	return true;
}

int MethodEditWnd::getLineCount()
{
	int count = 0;
	for (int i=0;i<_lines.GetSize();i++)
	{
		if 	(_lines[i]->_type != none)
		{
			count++	;
		}		   
	}
	return count;
}



int MethodEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (ScrollWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	_style.setDefault(ds_drawAMethod);

	RegArchive ar(CArchive::load, "Style", "MethodEditWnd");
	
	if (ar.isValid()) _style.Serialize(ar);

	OnSetMessageString(AFX_IDS_IDLEMESSAGE,0);

	SetTimer(AAA_MOUSE_LEFT_WND, 500,NULL);

	return 0;
}

void MethodEditWnd::OnDestroy() 
{
	KillTimer(AAA_MOUSE_LEFT_WND);
		
	ScrollWnd::OnDestroy();
	
	RegArchive ar(CArchive::store, "Style", "MethodEditWnd");
	_style.Serialize(ar);
	
	
}

CString MethodEditWnd::calcNotation()
{
	CString notationStr("");
	bool placeMade = false;
	bool even = GlobalFunctions::isEven(getNumber());

	//we allways create as a asymmettric method	
	for (int j=0;j<_height-1;j++)
	{
		placeMade = false;

		for (int i=0;i<_width;i++)
		{
			if (getLine(i,j)->_type == down)
			{
				{
					notationStr += GlobalFunctions::bellNumbersToChar(i+1);
					placeMade = true;
				}
			}
		}
		if ((!placeMade)&&(even))
		{
			notationStr += '-';
		}				  
		if (j<_height-2)//dont put the last dot on
		{
			notationStr += '.';
		}
	}
	return notationStr;
}

void MethodEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	ScrollWnd::OnSize(nType, cx, cy);
	centre();
}

void MethodEditWnd::centre()
{
	CRect client;
	GetClientRect(&client);
	
	if (client.Width() > getDimensions(styleBorder).cx)
	{
		_left = (client.Width() - getDimensions(noBorder).cx)/2;
	}
	else 
	{
		_left = _style._left;
	}

	if (client.Height() > getDimensions(styleBorder).cy)
	{
		_top = (client.Height() - getDimensions(noBorder).cy)/2;
	}
	else 
	{
		_top = _style._top;
	}

	Invalidate();
}


void MethodEditWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	point = snapToGrid(point);

	if (!isOutOfBounds(point))
	{
		_drawing = true;
		_mouseDown = point;
		SetCapture();
		_drawStart = CSize(0,0);
		_drawLength = 0;
		_drawType = none;
	}

	ScrollWnd::OnLButtonDown(nFlags, point);
}

void MethodEditWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_drawing)
	{
		CClientDC dc(this);
		//erase the old line
		drawInvertLine(dc);
		//validate
		validateLine(point); 
		//draw the new line //may be zero length
		drawInvertLine(dc);
	}
	
	//set the status text
	CSize pos;

	if (isOutOfBounds(point))
	{
		//notify the parent
		_methodEditPos._bell = -1;
		_methodEditPos._place = -1;
		_methodEditPos._row = -1;
		if (::IsWindow(GetParent()->m_hWnd))
			::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, FALSE,NULL);
	}
	else 
	{
		pos = pointToPos(point);

		int bell = -1;
		Line * line = NULL;

		if (pos.cy < _height-1)
		{
			line = getLine(pos);
		}
		else if(pos.cy == _height-1)
		{
			line = getLineIn(pos);
		}

		if (line)
		{
			if ((line->_type == none)&&(pos.cy >0))
			{
				line = getLineIn(pos);	
				if ((line)&&(line->_type != none))
					bell = line->_number+1;
			}
			else
			{
				bell = line->_number+1;
			}
		}
		
		_methodEditPos._bell = bell;
		_methodEditPos._place = ++pos.cx;
		_methodEditPos._row = ++pos.cy;

		//notify the parent
		if (::IsWindow(GetParent()->m_hWnd))
			::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, TRUE, NULL);
	}
	
	ScrollWnd::OnMouseMove(nFlags, point);
}

void MethodEditWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_drawing)
	{
		CClientDC dc(this);
		drawInvertLine(dc);
		_drawing = false;
		ReleaseCapture();
		//validate
		validateLine(point); 

		//set the lines
		for (int i=0;i<_drawLength ; i++)
		{
			addLine(none, (i==_drawLength-1)?true:false);
		}
		
		//sort colors
		cascade();
		//display
		Invalidate();		
  	}
	
	ScrollWnd::OnLButtonUp(nFlags, point);
}

bool MethodEditWnd::isOutOfBounds(const CSize & pos)
{
	return ((pos.cx < 0)||
			(pos.cx > _width-1)||
			(pos.cy < 0)||
			(pos.cy > _height-1));
}

bool MethodEditWnd::isOutOfBounds(const CPoint & pos)
{
	return ((pos.x < _left)||
			(pos.x > _left + (_width  * _style._width )-1)||
			(pos.y < _top )||
			(pos.y > _top  + (_height * _style._height)-1));
}

void MethodEditWnd::validateLine(const CPoint &point)
{
	//_mouseDown should always be in bounds
	if(point.y > _mouseDown.y)
	{
		_drawStart = pointToPos(_mouseDown);
		CSize drawEnd = pointToPos(point);

		//set direction
		if		(drawEnd.cx == _drawStart.cx) _drawType = down;
		else if	(drawEnd.cx >  _drawStart.cx) _drawType = right;
		else if	(drawEnd.cx <  _drawStart.cx) _drawType = left;

		//set line length
		_drawLength = drawEnd.cy - _drawStart.cy;
		
		//check out of bounds
		while ((_drawLength > 0)&&
			(isOutOfBounds(_drawStart + 
			CSize(_drawLength * (int)_drawType, _drawLength)))) 
		{
			_drawLength--;
		}

		checkClashes();
	}
	else
	{
		_drawStart = pointToPos(point); //v.temporary _drawStart
		CSize drawEnd = pointToPos(_mouseDown);
		
		//set direction
		if		(drawEnd.cx == _drawStart.cx) _drawType = down;
		else if	(drawEnd.cx <  _drawStart.cx) _drawType = left;
		else if	(drawEnd.cx >  _drawStart.cx) _drawType = right;

		//set line length
		_drawLength = drawEnd.cy - _drawStart.cy;
	
		//recalculate _drawStart from drawEnd and length
		_drawStart.cx = drawEnd.cx - (_drawLength * (int)_drawType);
		
		//check out of bounds
		while ((_drawLength > 0)&&(isOutOfBounds(_drawStart))) 
		{
			_drawLength--;
			_drawStart.cy++;
			_drawStart.cx += (int) _drawType;
		}

		//check clashes

		CSize tempStart(0,0);
		int tempLength = 0;
		for (int i= _drawLength - 1;i>=0;i--)
		{				
			CSize start(_drawStart.cx + (i       * (int)_drawType), 
												_drawStart.cy + i);
			int end =   _drawStart.cx + ((i + 1) * (int)_drawType);

			if ((						getLine(start          )->_type != none  ) ||
				((end > 0) &&		   (getLine(end-1, start.cy)->_type == right)) ||
				(						getLine(end,   start.cy)->_type == down  ) ||
				((end < _width - 1) && (getLine(end+1, start.cy)->_type == left )) )
			{
				//move the start
				_drawStart = tempStart;				
				//change the length 
				_drawLength = tempLength;
				break;
			} 
			else 
			{
				tempStart = start;
				tempLength++;
			}
		}	 
	}		 
}
  
void MethodEditWnd::drawInvertLine(CDC &dc)
{
	int OldMode = dc.SetROP2(R2_NOT);

	CPoint point = posToPoint(_drawStart);
	dc.MoveTo(point);
	point.x += (_drawLength * _style._width * (int)_drawType);
	point.y += (_drawLength * _style._height);
	dc.LineTo(point);

	dc.SetROP2(OldMode);	  
}

void MethodEditWnd::cascade()
{
	//set the first row correctly
	for (int i=0;i<_width;i++)
	{
		getLine(i,0)->_number = i;
	}

	for (int j=1;j<_height-1;j++)
		for (int i=0;i<_width;i++)
			{
				getLine(i,j)->_number = NOBELL;
			}

	//go down each subsiquent row and cascade the numbers
	for (int j=0;j<_height-2;j++)
	{
		for (int i=0;i<_width;i++)
		{
			Line * line = getLine(i,j);
			if (line->_type != none)
			{
				getLine(i + (int)line->_type, j+1)->_number = line->_number;
			}
		}
	}																		 	  																		 
}

void MethodEditWnd::addLine(lineType type, bool last)
{
	if (_drawStart.cy >= _height-1) _drawStart.cy -= (_height-1);

	if (type != none) 
	{
		_drawLength = 1;
		_drawType = type;
		checkClashes();
	}				   

	//change the type
	if (_drawLength > 0)
	{
		//add to the undo array.
		Undo undo;
		undo._pos = _drawStart;
		undo._was._type = getLine(_drawStart)->_type;
		undo._last = last;
		_undo.Add(undo);
	
		getLine(_drawStart)->_type = _drawType;
	}

	//reposition start
	_drawStart.cy++;

	_drawStart.cx += (int)_drawType;

}

CSize MethodEditWnd::getPos(CPoint &point)
{
	if (isOutOfBounds(point))
	{
		return CSize(-1,-1);
	}
	else
	{
		return pointToPos(point);
	}
}

void MethodEditWnd::checkClashes()
{
  		//check clashes
	for (int i=0;i<_drawLength;i++)
	{
		CSize start(_drawStart.cx + (i       * (int)_drawType), 
												_drawStart.cy + i);
		int end =   _drawStart.cx + ((i + 1) * (int)_drawType);
		
		if ((						getLine(start          )->_type != none  ) ||
			((end > 0) &&		   (getLine(end-1, start.cy)->_type == right)) ||
			(						getLine(end,   start.cy)->_type == down  ) ||
			((end < _width - 1) && (getLine(end+1, start.cy)->_type == left )) )
		{
			_drawLength = i;
		} 
	}

}

BOOL MethodEditWnd::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	SetCursor(app()->LoadStandardCursor(IDC_CROSS)); //IDC_SMALL_CROSS	   
	return TRUE;
}

void MethodEditWnd::OnPopupDelete()
{
	if (!isOutOfBounds(_contextMouseDown))
	{
		//add to the undo array.
		Undo undo;
		undo._pos = pointToPos(_contextMouseDown);
		undo._was._type = getLine(undo._pos)->_type;
		undo._last = true;
		_undo.Add(undo);		
		
		//remove, and redraw
		getLine(undo._pos)->_type = none;
		cascade();
		Invalidate();
	}	
}

MethodEditWnd::Line * MethodEditWnd::getLineIn(const CSize &pos)
{
	if (!isOutOfBounds(pos))
	{
		CSize posIn;
		Line * line;

		posIn = pos + CSize(-1,-1);
		if (!isOutOfBounds(posIn))
		{
			line = getLine(posIn);
			if (line->_type == right) return line;
		}	

		posIn = pos + CSize(0,-1);
		if (!isOutOfBounds(posIn))
		{
			line = getLine(posIn);
			if (line->_type == down) return line;
		}	

		posIn = pos + CSize(1,-1);
		if (!isOutOfBounds(posIn))
		{
			line = getLine(posIn);
			if (line->_type == left) return line;
		}	
	}		
	return NULL;
}
		  
bool MethodEditWnd::createMethod(int number, CString notation)
{	  
	Notation not;
	not.create(true, "",number, false, notation);

	not.prepare();

	//set up the number of rows

	if (not.getChangesPerPlainLead() == 0)
	{
		if (_blankStartSize > 0)
			createLines(number, _blankStartSize+1);
		else
			createLines(number, (number*2)+1);
	}
	else
	{
		createLines(number, not.getChangesPerPlainLead()+1);

		lineType lTp = right;

		//fill the rows
		for (int i=0;i<_height-1;i++)
		{
			lTp = right;

			for (int j=0;j<_width;j++)
			{
				if (not.contains(i,j+1))
					getLine(j,i)->_type = down;
				else
				{
					getLine(j,i)->_type = lTp;
					lTp = (lineType)((int)lTp * (-1)); //turn left to right & right to left
				}
			}
		}
	}
		
	updateDimentions();
	//sort colors
	cascade();
	//display
	Invalidate();

	return true;
} 

void MethodEditWnd::setStart(const CSize& start)
{
	_drawStart = start;
}

void MethodEditWnd::OnPopupAddworkSingledodge() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx > _width-2) return;

	setStart(start);
	addLine(right);
	addLine(left);
	
	setStart(start + CSize(1,0));
	addLine(left);
	addLine(right, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkDoubleDodge() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx > _width-2) return;

	setStart(start);
	addLine(right);
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(1,0));
	addLine(left);
	addLine(right);
	addLine(left);
	addLine(right, true);

	cascade();
	Invalidate();
}



void MethodEditWnd::OnPopupAddworkPlacesdown() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx < 1) return;

	setStart(start);
	addLine(down);
	addLine(left);
	addLine(down);

	setStart(start + CSize(-1,1));
	addLine(right, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkPlacesup() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx > _width-2) return;

	setStart(start);
	addLine(down);
	addLine(right);
	addLine(down);

	setStart(start + CSize(1,1));
	addLine(left, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkYorkshiredown() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx < 1) return;

	setStart(start);
	addLine(left);
	addLine(right);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(-1,0));
	addLine(right);
	addLine(left);
	addLine(right);

	setStart(start + CSize(0,4));
	addLine(left);

	setStart(start + CSize(-1,6));
	addLine(right);
	addLine(left);
	addLine(right, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkYorkshireup() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx > _width-2) return;

	setStart(start);
	addLine(right);
	addLine(left);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(left);
	addLine(right);

	setStart(start + CSize(1,0));
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(0,4));
	addLine(right);

	setStart(start + CSize(1,6));
	addLine(left);
	addLine(right);
	addLine(left, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkCambridgedown() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx < 1) return;

	setStart(start);
	addLine(left);
	addLine(right);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(right);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(-1,0));
	addLine(right);
	addLine(left);
	addLine(right);

	setStart(start + CSize(0,4));
	addLine(left);

	setStart(start + CSize(-1,6));
	addLine(right);
	addLine(left);
	addLine(right);

	setStart(start + CSize(0,10));
	addLine(left);

	setStart(start + CSize(-1,12));
	addLine(right);
	addLine(left);
	addLine(right, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnPopupAddworkCambridgeup() 
{
	CSize start = getPos(_contextMouseDown);
	if (start.cx > _width-2) return;
	
	setStart(start);
	addLine(right);
	addLine(left);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(left);
	addLine(right);
	addLine(down);
	addLine(left);
	addLine(down);
	addLine(right);
	addLine(left);
	addLine(right);

	setStart(start + CSize(1,0));
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(0,4));
	addLine(right);

	setStart(start + CSize(1,6));
	addLine(left);
	addLine(right);
	addLine(left);

	setStart(start + CSize(0,10));
	addLine(right);

	setStart(start + CSize(1,12));
	addLine(left);
	addLine(right);
	addLine(left, true);

	cascade();
	Invalidate();
}

void MethodEditWnd::OnUpdatePopupAddwork(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_methodEditPos._place>=0)&&
				   (_methodEditPos._place<=_width)&&
				   (_methodEditPos._row>=0)&&
				   (_methodEditPos._row<= _height));
}

void MethodEditWnd::OnUpdatePopupDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_methodEditPos._place>=0)&&
				   (_methodEditPos._place<=_width)&&
				   (_methodEditPos._row>=0)&&
				   (_methodEditPos._row<= _height));
}

void MethodEditWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
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


	_contextMouseDown = point;
	ScreenToClient(&_contextMouseDown);

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_METHOD_EDIT));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
}



MethodEditPos MethodEditWnd::getMethodEditPos()
{
	return _methodEditPos;
}

void MethodEditWnd::OnMethodDrawClearAll() 
{
	if (AfxMessageBox("Do you want to delete all lines?"
		, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES)
	{
		initLines();
		Invalidate();
	}
}

void MethodEditWnd::OnUpdateMethodDrawClearAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(getLineCount() > 0);
}

void MethodEditWnd::OnMethodDrawStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _width, slt_drawAMethod);
	styleEditDlg.DoModal();
	Invalidate();
}

void MethodEditWnd::OnMethodDrawUndo() 
{
	while (true)
	{
		if (_undo.GetSize() > 0)
		{
			Undo undo(_undo.GetAt(_undo.GetUpperBound()));
			Line * line = getLine(undo._pos);
			line->_type = undo._was._type;
			_undo.RemoveAt(_undo.GetUpperBound());
			if (_undo.GetSize() >0)
			{
				if ((_undo.GetAt(_undo.GetUpperBound()))._last)
				{
					break;
				}
			}										
		}
		else 
		{
			break;
		}			
	}

	cascade();
	Invalidate();
}

void MethodEditWnd::OnUpdateMethodDrawUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_undo.GetSize() > 0);
}


void MethodEditWnd::OnTimer(UINT nIDEvent) 
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
			if (::IsWindow(GetParent()->m_hWnd))
				::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, FALSE,NULL);
		}
	}
	
	ScrollWnd::OnTimer(nIDEvent);
}

LRESULT MethodEditWnd::OnUpdateMethod(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	Invalidate();
	return 0;
}

void MethodEditWnd::setBlankStartSize(int size)
{
	_blankStartSize = size;
}
