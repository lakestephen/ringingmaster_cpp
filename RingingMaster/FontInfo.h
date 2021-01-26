// FontInfo.h: interface for the FontInfo class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

//to2do can this class be replaced with CBCGPFontDesc

class FontInfo  
{
public:
	CString getRTFFormat(int id) const;
	CString getDisplayName() const;
	void Serialize(CArchive &ar);
	bool editFont(CWnd * pWnd, bool effects = false);
	void getLogFont(LOGFONT *lf) const;
	FontInfo(const CString& name = "Arial", int size = 80);
	virtual ~FontInfo();   

	CString _name;
	COLORREF _color;
	int _size;
	int _weight;
	BOOL _italic;
	BOOL _strikeout;
	BOOL _underline;

	
};


