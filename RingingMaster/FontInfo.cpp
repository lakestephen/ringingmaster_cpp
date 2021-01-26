// FontInfo.cpp: implementation of the FontInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"

#include "FontInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FontInfo::FontInfo(const CString& name, int size ):
_name(name),
_size(size),
_color(RGB(0,0,0)),
_weight(FW_NORMAL),
_italic(false),
_strikeout(false),
_underline(false)
{			 
}

FontInfo::~FontInfo()
{

}

void FontInfo::getLogFont(LOGFONT *lf) const
{
	CFont font;
	font.CreatePointFont(_size, _name);
	font.GetLogFont(lf);
	lf->lfItalic = static_cast<BYTE>(_italic);
	lf->lfWeight = _weight;
	lf->lfStrikeOut = static_cast<BYTE>(_strikeout);
	lf->lfUnderline = static_cast<BYTE>(_underline);
}

//return value is OK on dlg
bool FontInfo::editFont(CWnd * pWnd, bool effects)
{
	//load data
	LOGFONT lf;
	DWORD dwFlags = NULL;

	getLogFont(&lf);
	
	if (effects)
		dwFlags = CF_SCREENFONTS|CF_EFFECTS;
	else 
		dwFlags = CF_SCREENFONTS;

	CFontDialog dlg(&lf,dwFlags,NULL,pWnd);

	dlg.m_cf.rgbColors = _color;
		
	if (dlg.DoModal() == IDOK)
	{
		//save data
		_name		= dlg.GetFaceName();
		_size		= dlg.GetSize();
		_color		= dlg.GetColor();
		_weight		= dlg.GetWeight();
		_strikeout	= (dlg.IsStrikeOut() == TRUE);
		_underline	= (dlg.IsUnderline() == TRUE);
		_italic		= (dlg.IsItalic() == TRUE);
   
		return true;
	}	
	return false;
}

void FontInfo::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _name;
		ar << _color;
		ar << _size;
		ar << _weight;
		ar << _italic;
		ar << _strikeout;
		ar << _underline;
	}
	else
	{
		ar >> version;
		ar >> _name;
		ar >> _color;
		ar >> _size;
		ar >> _weight;
		ar >> _italic;
		ar >> _strikeout;
		ar >> _underline;
	}
}


CString FontInfo::getRTFFormat(int id) const
{
	CString font;
	
	font.Format("{\\f%d\\fcharset0 %s;}", id, _name);

	return font;
}

CString FontInfo::getDisplayName() const
{
	CString str;
	str.Format("%s(%d)", _name, _size/10);
	return str;
}
