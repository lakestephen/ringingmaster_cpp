// Style.cpp: implementation of the Style class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "FontInfo.h"
#include "Style.h"
#include "StyleManager.h"
#include "CribManager.h"
#include "SimulatorManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// StyleLine
//////////////////////////////////////////////////////////////////////
Style::StyleLine::StyleLine() :
//line data
_lineColor(ULONG_MAX),
_lineWidth(1),
//text data
_textColor(RGB(0,0,0))
{
}

Style::StyleLine::StyleLine(StyleLine &styleLine)
{
	operator = (styleLine);
}

Style::StyleLine::~StyleLine()
{

}

void Style::StyleLine::operator =(const Style::StyleLine &styleLine)
{
	//line data
	_lineColor = styleLine._lineColor;
	_lineWidth = styleLine._lineWidth;
	//text data
	_textColor = styleLine._textColor;
}

void Style::StyleLine::Serialize(CArchive &ar)
{
	int version = 1;
	
	if (ar.IsStoring())
	{
		ar << version;
		ar << _lineColor ;
		ar << _lineWidth;
		ar << _textColor;
	}
	else 
	{
		ar >> version;
		ar >> _lineColor;
		ar >> _lineWidth;
		ar >> _textColor;
	}															  
}

//////////////////////////////////////////////////////////////////////
// Style
//////////////////////////////////////////////////////////////////////

//default constructor
Style::Style(defaultStyle type) :
_titleHeight(0),
_notationWidth(0)
{
	setDefault(type);
}

//copy constructor
Style::Style(const Style & style) 
{
	setDefault();
	operator = (style);
}



Style::~Style()
{

}

//assignment
void Style::operator =(const Style &style)
{
	for (int i=0;i<MAXBELLS;i++)
	{
		_lines[i] = style._lines[i];
	}	

	for (int j=0;j<MINUS_LINES_MAX;j++)
	{
		_minusLines[j] = style._minusLines[j];
		_useMinus[j] = style._useMinus[j];
	}	

	//general
	_name = style._name;

	//layout
	_left = style._left;
	_top = style._top;
	_width = style._width;
	_height = style._height;
	_leadsPerColumn = style._leadsPerColumn;

	//grid
	_gridWidthH = style._gridWidthH;
	_gridWidthV = style._gridWidthV;
	_gridColorH = style._gridColorH;
	_gridColorV = style._gridColorV;

	//title
	_titleShow = style._titleShow;
	_titleFont = style._titleFont;

	//numbers
	_numbersFont = style._numbersFont ;

	//starts
	_startsShow = style._startsShow;
	_startsFont = style._startsFont;
	_startsBlobs= style._startsBlobs;

	//notation
	_notationShow = style._notationShow;
	_notationFont = style._notationFont;

	//false leads
	_falseRowsColor = style._falseRowsColor;

	//music rows
	_musicRowsColor = style._musicRowsColor;
	
	//stroke
	_handStrokeShow = style._handStrokeShow;
	_backStrokeShow = style._backStrokeShow;
	_strokeFont		= style._strokeFont;

	//in course
	_showInCourse = style._showInCourse;

	//calls
	_commentsShow = style._commentsShow;
	_commentsFont = style._commentsFont;
	
	//other
	_changed = style._changed;

}


void Style::Serialize(CArchive& ar)
{
	int version = 3;
	if (ar.IsStoring())
	{
		ar << version;
		//name 
		ar << _name;
		//layout
		ar << _left << _top << _width << _height << _leadsPerColumn;
		//grid
		ar << _gridWidthH << _gridWidthV << _gridColorH << _gridColorV;
		//title
		ar << _titleShow,
		_titleFont.Serialize(ar);
		//numbers
		_numbersFont.Serialize(ar);
		//starts
		ar << _startsShow << _startsBlobs;
		_startsFont.Serialize(ar);
		//notation
		ar << _notationShow;
		_notationFont.Serialize(ar);
		//false leads
		ar << _falseRowsColor;
		//music rows color
		ar << _musicRowsColor;
		//stroke
		ar << _handStrokeShow <<_backStrokeShow;
		_strokeFont.Serialize(ar);
		//inCourse
		ar << _showInCourse;
		//calls
		ar << _commentsShow;
		_commentsFont.Serialize(ar);
		//rows
		for (int i=0;i<MAXBELLS;i++)
		{
			_lines[i].Serialize(ar);
		}
		for (int j=0;j<MINUS_LINES_MAX;j++)
		{
			ar << _useMinus[j];
			_minusLines[j].Serialize(ar);
		}	

	}
	else
	{
		ar >> version;

		TRACE("Deserialising Style Version %d \r\n", version);

		if (version == 1)
			deserialize_v1(ar);
		else if (version == 2)
			deserialize_v2(ar);
		else if (version == 3)
			deserialize_v3(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   

	}
}			

//version 3 is version 2.01 onwards
void Style::deserialize_v3(CArchive& ar) 
{
	//name 
	ar >> _name;
	//layout
	ar >> _left >> _top >> _width >> _height >> _leadsPerColumn;
	//grid
	ar >> _gridWidthH >> _gridWidthV >> _gridColorH >> _gridColorV;
	//title
	ar >> _titleShow,
	_titleFont.Serialize(ar);
	//numbers
	_numbersFont.Serialize(ar);
	//starts
	ar >> _startsShow >> _startsBlobs;
	_startsFont.Serialize(ar);
	//notation
	ar >> _notationShow;
	_notationFont.Serialize(ar);
	//false leads
	ar >> _falseRowsColor;
	//music rows color
	ar >> _musicRowsColor;
	//stroke
	ar >> _handStrokeShow >>_backStrokeShow;
	_strokeFont.Serialize(ar);
	//inCourse
	ar >> _showInCourse;
	//calls
	ar >> _commentsShow;
	_commentsFont.Serialize(ar);
	//rows
	for (int i=0;i<MAXBELLS;i++)
	{
		_lines[i].Serialize(ar);
	}
	for (int j=0;j<MINUS_LINES_MAX;j++)
	{
		ar >> _useMinus[j];
		_minusLines[j].Serialize(ar);
	}	
}

//version 2 is version 1.08 onwards
void Style::deserialize_v2(CArchive& ar) 
{
	//name 
	ar >> _name;
	//layout
	ar >> _left >> _top >> _width >> _height >> _leadsPerColumn;
	//grid
	ar >> _gridWidthH >> _gridWidthV >> _gridColorH >> _gridColorV;
	//title
	ar >> _titleShow,
	_titleFont.Serialize(ar);
	//numbers
	_numbersFont.Serialize(ar);
	//starts
	ar >> _startsShow >> _startsBlobs;
	_startsFont.Serialize(ar);
	//notation
	ar >> _notationShow;
	_notationFont.Serialize(ar);
	//false leads
	ar >> _falseRowsColor;
	//music rows color
	ar >> _musicRowsColor;
	//stroke
	ar >> _handStrokeShow >>_backStrokeShow;
	_strokeFont.Serialize(ar);
	//calls
	ar >> _commentsShow;
	_commentsFont.Serialize(ar);
	//rows
	for (int i=0;i<MAXBELLS;i++)
	{
		_lines[i].Serialize(ar);
	}
	for (int j=0;j<MINUS_LINES_MAX;j++)
	{
		ar >> _useMinus[j];
		_minusLines[j].Serialize(ar);
	}	
}

//version 1 is version 1.07 onwards
void Style::deserialize_v1(CArchive& ar) 
{
	//name 
	ar >> _name;
	//layout
	ar >> _left >> _top >> _width >> _height >> _leadsPerColumn;
	//grid
	ar >> _gridWidthH >> _gridWidthV >> _gridColorH >> _gridColorV;
	//title
	ar >> _titleShow,
		_titleFont.Serialize(ar);
	//numbers
	_numbersFont.Serialize(ar);
	//starts
	ar >> _startsShow >> _startsBlobs;
	_startsFont.Serialize(ar);
	//notation
	ar >> _notationShow;
	_notationFont.Serialize(ar);
	//false leads
	ar >> _falseRowsColor;
	//stroke
	ar >> _handStrokeShow >>_backStrokeShow;
	_strokeFont.Serialize(ar);
	//calls
	ar >> _commentsShow;
	_commentsFont.Serialize(ar);
	//rows
	for (int i=0;i<MAXBELLS;i++)
	{
		_lines[i].Serialize(ar);
	}
}



void Style::setDefault(defaultStyle type)
{

	if (type == ds_fromStyleManager)
	{
		operator = (*styleManager()->getStyle());
		return;
	}

	if (type == ds_fromCribManager) 
	{
		operator = (cribManager()->getDefaultBlueLineStyle());
		return;
	}				 	

	if (type == ds_fromSimulatorManager) 
	{
		operator = (simulatorManager()->getDefaultBlueLineStyle());
		return;
	}				 	

	_name = CString("Default style");
	
	for (int i=0;i<MINUS_LINES_MAX;i++)
	{					
		_useMinus[i] = FALSE;
	}
	
	//layout
	_left = 50;
	_top = 20;
	_width = 11;
	_height = 11;
	
	_leadsPerColumn = 2;
	
	//grid
	_gridColorH = RGB(192,192,192);
	_gridColorV = ULONG_MAX;
	_gridWidthH = 2;
	_gridWidthV = 1;
	
	//title
	_titleShow = TRUE;
	_titleFont._name = "Arial";
	_titleFont._size = 220;
	_titleFont._color = RGB(0,0,0);
	
	//numbers
	_numbersFont._name = "Arial";
	_numbersFont._size = 80;
	_numbersFont._color = RGB(0,0,0);
	
	//starts
	_startsShow = TRUE;
	_startsFont._name = "Arial";
	_startsFont._size = 100;
	_startsFont._color = RGB(0,0,0);
	_startsFont._weight = FW_BOLD;
	_startsBlobs = TRUE;
	
	//notation
	_notationShow = FALSE;
	_notationFont._name = "Arial";
	_notationFont._size = 80;
	_notationFont._color = RGB(80,80,80);
	
	//false leads
	_falseRowsColor = RGB(255,192,203);

	//music rows
	_musicRowsColor = RGB(192, 220, 192);
	
	//stroke
	_handStrokeShow = FALSE;
	_backStrokeShow = FALSE;
	_strokeFont._name = "Arial";
	_strokeFont._size = 80;
	_strokeFont._color = RGB(0,0,0);

	_showInCourse = FALSE;
	
	//calls
	_commentsShow = TRUE;
	_commentsFont._name = "Arial";
	_commentsFont._size = 90;
	_commentsFont._color = RGB(100,100,100);
	_commentsFont._weight = FW_BOLD;
	
	//other
	_changed = FALSE;
	_titleHeight = 0;
	_notationWidth = 0;
	
	
	
	if (type == ds_methodPreview)
	{
		_notationShow = TRUE;	
	}

	if (type == ds_callChange)
	{
		_notationShow = FALSE;	
		_titleShow = FALSE;
		_startsShow = FALSE;
		_startsBlobs = FALSE;  
		_handStrokeShow = FALSE;
		_backStrokeShow = FALSE;
		_notationShow = FALSE;
		_falseRowsColor = ULONG_MAX;  
		_musicRowsColor = ULONG_MAX;
		_notationShow = FALSE;

		_left = 10;
		_top = 10;
		_width = 9;
		_height = 11;
	
		_commentsFont._size = 80;		
		_commentsFont._weight = FW_NORMAL;

		_leadsPerColumn = SHRT_MAX;

	}				  
	if (type == ds_simulator)
	{
		_notationShow = FALSE;	
		_titleShow = FALSE;
		_startsShow = FALSE;
		_startsBlobs = FALSE;  
		_handStrokeShow = FALSE;
		_backStrokeShow = FALSE;
		_notationShow = FALSE;
		_falseRowsColor = ULONG_MAX;  
		_musicRowsColor = ULONG_MAX;
		_notationShow = FALSE;

		_left = 25;
		_top = 20;
		_width = 15;
		_height = 15;
	
		_commentsFont._size = 120;		
		_commentsFont._weight = FW_NORMAL;

		_leadsPerColumn = SHRT_MAX;

		_numbersFont._size = 120;

	}

	srand(GetTickCount());

	switch (type)
	{
	case ds_drawAMethod:
		{
			_name = CString("Visual Notation edit");

			for (int i=0;i<MAXBELLS;i++)
			{	
				int R = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int G = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int B = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int N = (int)((float)rand() * (float)((float)3/(float)RAND_MAX));

				_lines[i]._lineColor = RGB(R,G,B);
				_lines[i]._lineWidth = N + 1;
				_lines[i]._textColor = ULONG_MAX;
			}

			//layout
			_left = 10;
			_top = 10;
			_width = 15;
			_height = 15;

			_leadsPerColumn = 2;

			//grid
			_gridColorV = RGB(192,192,192);
			_gridWidthH = 1;

			//starts
			_startsFont._size = 120;

			//notation
			_notationShow = TRUE;
			

			//calls
			_commentsShow = FALSE;
			_commentsFont._size = 80;

		}
		break;
	case ds_simulator:
	case ds_callChange:
		{
			_lines[0]._lineColor = RGB(255,0,0);
			_lines[0]._textColor = RGB(0,0,0); 
			_lines[1]._lineWidth = 2;
			_lines[1]._lineColor = RGB(0,0,255);
			_lines[1]._textColor = RGB(0,0,0); 
		}

		break;
	case ds_crib:
		{
			for (int i=0;i<MAXBELLS;i++)
			{	
				int R = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int G = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int B = (int)((float)rand() * (float)((float)255/(float)RAND_MAX));
				int N = (int)((float)rand() * (float)((float)3/(float)RAND_MAX));

				_lines[i]._lineColor = RGB(R,G,B);
				_lines[i]._lineWidth = N + 1;
				_lines[i]._textColor = ULONG_MAX;
			}
		}
		break;

	case ds_normal:
	default:
		{
			_lines[0]._lineColor = RGB(255,0,0);
			_lines[0]._textColor = ULONG_MAX; 
			_minusLines[0]._lineWidth = 2;
			_minusLines[0]._lineColor = RGB(0,0,255);
			_minusLines[0]._textColor = ULONG_MAX; 
			_useMinus[0] = TRUE;

		}
		break;

	}	 
}
	



