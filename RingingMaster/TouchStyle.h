
#pragma once


#include "FontInfo.h"

enum TouchStyleColorType
{		 
	tsc_titleColor,			//added in 3.01
	tsc_authorColor,			//added in 3.01

	tsc_callPosColor,			
	tsc_callColor,				
	tsc_methodSplicedColor,	
	tsc_unchangedLHColor,		
	tsc_changedLHColor,		
	tsc_blockColor,			
	tsc_definitonColor,		
	tsc_varianceColor,			
	tsc_groupColor,			
	tsc_plainLeadTokenColor,  
	tsc_notesColor, 	
	
	//leave these at the end 
	tsc_highest_val
};


class TouchStyle  
{
public:
	TouchStyle();
	virtual ~TouchStyle();

	CFont& getFontNotes();
	CFont& getFontMain();
	CFont& getFontTitle();
	CFont& getFontAuthor();

	const FontInfo& getFontInfoNotes();
	const FontInfo& getFontInfoMain();
	const FontInfo& getFontInfoTitle();
	const FontInfo& getFontInfoAuthor();

	void setFontInfoMain(FontInfo& fontInfo);
	void setFontInfoNotes(FontInfo& fontInfo);
	void setFontInfoTitle(FontInfo& fontInfo);
	void setFontInfoAuthor(FontInfo& fontInfo);

	static CString getColorName(TouchStyleColorType type);
	void setColor(TouchStyleColorType type, const COLORREF val);
	COLORREF getColor(TouchStyleColorType type);


	void Serialize(CArchive& ar);	
	void operator =(const TouchStyle &touchStyle);


private:
	void deserialize_v1(CArchive& ar);//upto version 1.07 
	void deserialize_v2(CArchive& ar);//version 2.01 onwards
	void deserialize_v3(CArchive& ar);//version 2.03 onwards
 

	void setDefault();
	void recreateFont();
	FontInfo _fontInfoMain;		
	FontInfo _fontInfoNotes;		
	FontInfo _fontInfoTitle;		
	FontInfo _fontInfoAuthor;		
	CFont _fontMain;
	CFont _fontNotes;
	CFont _fontTitle;
	CFont _fontAuthor;
	COLORREF _colors[tsc_highest_val];

};
