// Style.h: interface for the Style class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLE_H__226FB0B4_4B5E_11D4_B4CD_009027BB3286__INCLUDED_)
#define AFX_STYLE_H__226FB0B4_4B5E_11D4_B4CD_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FontInfo.h"


#define MINUS_LINES_MAX 4

enum defaultStyle
{
	ds_fromStyleManager,
	ds_fromCribManager,
	ds_fromSimulatorManager,
	ds_normal,
	ds_drawAMethod,
	ds_methodPreview,
	ds_methodSelect, 
	ds_callChange,
	ds_crib,
	ds_simulator,
};

class Style  
{
public:


	void setDefault(defaultStyle type = ds_normal);
	void Serialize(CArchive& ar);	
	void deserialize_v3(CArchive& ar);
	void deserialize_v2(CArchive& ar);
	void deserialize_v1(CArchive& ar); 
	Style(defaultStyle type = ds_normal);
	Style(const Style &style);
	virtual ~Style();

	int getTop() {return (_top) + _titleHeight;}
	int getLeft() {return (_left) + _notationWidth;}
	//general
	CString _name;
		
	//layout
	int _left;
	int _top;
	int _width;
	int _height;
	int _leadsPerColumn;

	//grid
	int		 _gridWidthH;
	int		 _gridWidthV;
	COLORREF _gridColorH;
	COLORREF _gridColorV;

	//title
	BOOL	_titleShow;
	FontInfo _titleFont;

	//numbers
	FontInfo _numbersFont;

	//starts
	BOOL	 _startsShow;
	FontInfo _startsFont;
	BOOL	 _startsBlobs;

	//notation
	BOOL	 _notationShow;
	FontInfo _notationFont;

	//false leads
	COLORREF _falseRowsColor;

	//false leads
	COLORREF _musicRowsColor;

	//stroke
	BOOL	 _handStrokeShow;
	BOOL	 _backStrokeShow;
	FontInfo _strokeFont;

	//in course
	BOOL	 _showInCourse;	

	//minus check boxes
	BOOL     _useMinus[MINUS_LINES_MAX];
	
	//comments
	BOOL	 _commentsShow;
	FontInfo _commentsFont;

	class StyleLine  
	{
	public:
		void Serialize(CArchive &ar);
		StyleLine();
		StyleLine(StyleLine & styleLine);
		void StyleLine::operator =(const StyleLine &styleLine);
		virtual ~StyleLine();
		//line data
		COLORREF	_lineColor;
		int			_lineWidth;
		//text data
		COLORREF	_textColor;
	};

	//other: non persistant
	bool _changed;
	int _titleHeight;
	int _notationWidth;


	void operator =(const Style &style);

	

	//exclusivly return the minus line
	Style::StyleLine * getMinusLine(int index)
	{
		return &_minusLines[index];
	}

	//exclusivly return the normal line
	Style::StyleLine * getNormalLine(int index)
	{
		return &_lines[index];
	}

	//return the line that is appropriate to the number - if there is a tenor override, then return that
	Style::StyleLine * getLine(int index, int number )
	{
		int tenorNumber = getTenorNumer(index, number) ;
		if (tenorNumber != -1)
			return &_minusLines[tenorNumber];
		else		
			return &_lines[index];
	}

	int getTenorNumer(int index, int number)
	{
		int tenorNumber = number - index - 1;
		if ((tenorNumber > 3)||(tenorNumber < 0))
			return -1;
		else
		{
			if (_useMinus[tenorNumber])
				return tenorNumber;
			else 
				return -1;
		}
	}

protected:
	StyleLine _lines[MAXBELLS];

	StyleLine _minusLines[MINUS_LINES_MAX];


};

typedef CArray <Style, Style> Styles;

#endif // !defined(AFX_STYLE_H__226FB0B4_4B5E_11D4_B4CD_009027BB3286__INCLUDED_)
