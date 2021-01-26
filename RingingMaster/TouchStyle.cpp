// TouchStyle.cpp: implementation of the TouchStyle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TouchStyle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TouchStyle::TouchStyle()
{
	setDefault();
}

TouchStyle::~TouchStyle()
{

}

void TouchStyle::setDefault()
{  
	setColor(tsc_unchangedLHColor,   RGB(170,170,170));
	setColor(tsc_methodSplicedColor, RGB(255,128,0  ));
	setColor(tsc_changedLHColor,     RGB(100,0,  0  ));
	setColor(tsc_callPosColor,       RGB(0,  160,0  ));
	setColor(tsc_callColor,          RGB(0,  0,  255));
	setColor(tsc_blockColor,         RGB(140,140,140));
	setColor(tsc_varianceColor,      RGB(108,0,  108));
	setColor(tsc_groupColor,		  RGB(0, 128,128));
	setColor(tsc_definitonColor,     RGB(140,140,140));
	setColor(tsc_plainLeadTokenColor,RGB(141,183,91 ));
	setColor(tsc_notesColor,		 RGB(0,  0,  0  ));
	setColor(tsc_titleColor,		 RGB(0,  0,  0  ));
	setColor(tsc_authorColor,		 RGB(0,  0,  129));
	

	_fontInfoTitle._name = "Arial";
	_fontInfoTitle._size = 240;
	
	_fontInfoAuthor._name = "Arial";
	_fontInfoAuthor._size = 170;

	_fontInfoMain._name = "Arial";
	_fontInfoMain._size = 200;

	_fontInfoNotes._name = "Arial";
	_fontInfoNotes._size = 100;

	recreateFont();
}


void TouchStyle::Serialize(CArchive& ar)
{
	int version = 3;
	if (ar.IsStoring())
	{
		ar << version;

		ar << tsc_highest_val;

		for (int i=0;i<tsc_highest_val;i++) 
		{
			ar << getColor((TouchStyleColorType)i);
		}

		_fontInfoMain.Serialize(ar);
		_fontInfoNotes.Serialize(ar);
		_fontInfoTitle.Serialize(ar); //added in version 3 release 3.01
		_fontInfoAuthor.Serialize(ar);//added in version 3 release 3.01
	
	}
	else
	{
		ar >> version;

		TRACE("Deserialising TouchStyle Version %d \r\n", version);

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

//version 3 is version 3.01 onwards
void TouchStyle::deserialize_v3(CArchive& ar) 
{
	int color;
	int number;
	
	ar >> number;

	for (int i=0;i<number;i++) 
	{
		ar >> color;
			
		setColor((TouchStyleColorType)i, color);
	}

	_fontInfoMain.Serialize(ar);
	_fontInfoNotes.Serialize(ar);
	_fontInfoTitle.Serialize(ar);  //added in version 3 release 3.01
	_fontInfoAuthor.Serialize(ar); //added in version 3 release 3.01


	recreateFont();
}

//version 2 is version 2.01 onwards
void TouchStyle::deserialize_v2(CArchive& ar) 
{
	int color;
	int number;
	
	ar >> number;

	for (int i=0;i<number;i++) 
	{
		ar >> color;
			
		setColor((TouchStyleColorType)(i+2), color); //plus 2 is because 	tsc_title,		tsc_author, TouchStyleColorType			

	}

	_fontInfoMain.Serialize(ar);
	_fontInfoNotes.Serialize(ar);

	recreateFont();
}

//version 1 is version 1.07 onwards
void TouchStyle::deserialize_v1(CArchive& ar) 
{
	int color;

	for (int i=0;i<10;i++) //10 is the number in version 1.07 & 1.08
	{
		ar >> color;
			
		setColor((TouchStyleColorType)(i +2), color);//plus 2 is because 	tsc_title,		tsc_author, TouchStyleColorType			
	}

	_fontInfoMain.Serialize(ar);

	recreateFont();
}

//assignment
void TouchStyle::operator =(const TouchStyle &touchStyle)
{				
	for (int i=0;i<tsc_highest_val;i++) 
	{
		_colors[i] = touchStyle._colors[i];
	}

	_fontInfoMain    = touchStyle._fontInfoMain;
	_fontInfoNotes   = touchStyle._fontInfoNotes;
	_fontInfoTitle   = touchStyle._fontInfoTitle;
	_fontInfoAuthor  = touchStyle._fontInfoAuthor;

	recreateFont();

}

void TouchStyle::recreateFont()
{
	LOGFONT lf;
	_fontInfoMain.getLogFont(&lf);
	_fontMain.DeleteObject();
	_fontMain.CreateFontIndirect(&lf);				

	_fontInfoNotes.getLogFont(&lf);
	_fontNotes.DeleteObject();
	_fontNotes.CreateFontIndirect(&lf);				

	_fontInfoTitle.getLogFont(&lf);
	_fontTitle.DeleteObject();
	_fontTitle.CreateFontIndirect(&lf);				

	_fontInfoAuthor.getLogFont(&lf);
	_fontAuthor.DeleteObject();
	_fontAuthor.CreateFontIndirect(&lf);				
}

COLORREF TouchStyle::getColor(TouchStyleColorType type)
{
	ASSERT((type <= tsc_highest_val)&&(type >= 0));

	if (type == tsc_highest_val)
		return RGB(0,0,0);

	return _colors[type];
}

void TouchStyle::setColor(TouchStyleColorType type, const COLORREF val)
{
	ASSERT((type < tsc_highest_val)&&(type >= 0));
	if ((type < tsc_highest_val)&&(type >= 0))
		_colors[type] = val;

	switch (type)
	{
	case tsc_notesColor:
		_fontInfoNotes._color = val;
		break;
	case tsc_titleColor:
		_fontInfoTitle._color = val;
		break;
	case tsc_authorColor:
		_fontInfoAuthor._color = val;
	}
}

void TouchStyle::setFontInfoMain(FontInfo& fontInfo)
{
	_fontInfoMain = fontInfo;
	
	if (_fontInfoMain._size < 60)
		_fontInfoMain._size = 60;

	recreateFont();
}

void TouchStyle::setFontInfoNotes(FontInfo& fontInfo)
{
	_fontInfoNotes = fontInfo;
	
	if (_fontInfoNotes._size < 60)
		_fontInfoNotes._size = 60;

	setColor(tsc_notesColor, _fontInfoNotes._color);

	recreateFont();
}

void TouchStyle::setFontInfoTitle(FontInfo& fontInfo)
{
	_fontInfoTitle = fontInfo;
	
	if (_fontInfoTitle._size < 60)
		_fontInfoTitle._size = 60;

	setColor(tsc_titleColor, _fontInfoTitle._color);

	recreateFont();
}

void TouchStyle::setFontInfoAuthor(FontInfo& fontInfo)
{
	_fontInfoAuthor = fontInfo;
	
	if (_fontInfoAuthor._size < 60)
		_fontInfoAuthor._size = 60;

	setColor(tsc_authorColor, _fontInfoAuthor._color);

	recreateFont();
}



const FontInfo& TouchStyle::getFontInfoMain()
{
	return _fontInfoMain;
}         

const FontInfo& TouchStyle::getFontInfoNotes()
{
	return _fontInfoNotes;
}

const FontInfo& TouchStyle::getFontInfoTitle()
{
	return _fontInfoTitle;
}

const FontInfo& TouchStyle::getFontInfoAuthor()
{
	return _fontInfoAuthor;
}



CFont& TouchStyle::getFontMain()
{
	return _fontMain;
}

CFont& TouchStyle::getFontNotes()
{
	return _fontNotes;
}

CFont& TouchStyle::getFontTitle()
{
	return _fontTitle;
}

CFont& TouchStyle::getFontAuthor()
{
	return _fontAuthor;
}

CString TouchStyle::getColorName(TouchStyleColorType type)
{
	switch(type) {
	
	case tsc_unchangedLHColor:
		return "Unchanged LH";
	case tsc_changedLHColor:		
		return "Changed LH";
	case tsc_methodSplicedColor:	
		return "Splices";
	case tsc_callPosColor:			
		return "Calling Position";
	case tsc_callColor:				
		return "Calls";
	case tsc_blockColor:			
		return "Block Definition";
	case tsc_varianceColor:			
		return "Variance";
	case tsc_groupColor:			
		return "Group";
	case tsc_definitonColor:		
		return "Definition";
	case tsc_plainLeadTokenColor:   
		return "Plain Lead token";
	case tsc_notesColor:   
		return "Notes";
	case tsc_titleColor:   
		return "Title";
	case tsc_authorColor:   
		return "Author";
	default:
		return "";
	}			  
}
