// NotationLite.cpp: implementation of the NotationLite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationLite.h"
#include "Notation.h"
#include "MethodManager.h"
#include "BuffArchive.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString NotationLite::_ClipboardToken = "Ringing Master Notation Lite Info";


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NotationLite::NotationLite(const NotationLite& notation)
{
	operator = (notation);
}

NotationLite::NotationLite()
{

}

NotationLite::~NotationLite()
{

}

void NotationLite::operator =(const Notation& notation)
{
	NotationPersist::operator = (notation);
}

void NotationLite::operator =(const NotationLite& notation)
{
	NotationPersist::operator = (notation);
}

unsigned long NotationLite::getFolderID()
{
	return _folder;
}

void NotationLite::setFolderID(unsigned long folderID)
{
	_folder = folderID;
}

CString NotationLite::getCCInfo(NotationCCInfoType type)
{
	for (int i=0;i<_info.GetSize();i++)
	{
		if (_info[i]->_type == type)
		{
			return _info[i]->_text;
		}
	}
	return CString("");
}

CString NotationLite::getName(bool showNumber)
{
	return _name + ((showNumber)?(" " + GlobalFunctions::numberName(_number)):"");
}

CString NotationLite::getNotation(bool showSymmetry, SeperationStyle seperationStyle)
{
	if (seperationStyle == SEP_NONE)
		seperationStyle = globalDataManager()->getSeperationStyle();

	return ((showSymmetry)?((_symmetric )?"& ":"+ "):"") + 
			 getDisplayString(_notation, seperationStyle);
}

CString NotationLite::getDisplayString(CString &str, SeperationStyle seperationStyle)
{
	CString displayStr = str;
	//sort seperators
	if (seperationStyle == SEP_COMPACT)
	{
		while (displayStr.Replace("-.","-"));
		while (displayStr.Replace(".-","-"));
	}

	while (displayStr.Replace('-', globalDataManager()->getAllChangeChar()));

	return displayStr;
}	


CString NotationLite::getLeadEnd()
{
	return getDisplayString(_leadEnd, globalDataManager()->getSeperationStyle());
}

CString NotationLite::getSpliceLetter()
{
	return _spliceLetter;
}

int NotationLite::getNumber()
{
	return _number;
}

void NotationLite::setName(CString str)
{
	_name = str;
}

CString NotationLite::getFolderPath()
{
	return methodManager()->getFolderPath(_folder);
}
			
void NotationLite::CopyToClipboard(CWnd* wnd, NotationLite *notation)
{
	if(OpenClipboard(wnd->GetSafeHwnd()))
	{
		EmptyClipboard();

		BuffArchive ar(CArchive::store);

		notation->Serialize(ar);

		int length =0;
		BYTE* serialisedData = ar.getData(length);

		//info
		HGLOBAL hData = GlobalAlloc(GMEM_DDESHARE, length);
		BYTE* buffer = (BYTE*)GlobalLock(hData);
		
		memcpy(buffer, serialisedData, length);

		GlobalUnlock(hData);
		SetClipboardData(::RegisterClipboardFormat(_ClipboardToken), hData);		

		CloseClipboard();

		delete [] serialisedData;
	}

}

BOOL NotationLite::IsClipboardFormatAvailable()
{	
	return (::IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardToken)));
}

NotationLite* NotationLite::PasteFromClipboard(CWnd* wnd)
{
	if((::IsClipboardFormatAvailable(::RegisterClipboardFormat(_ClipboardToken))) &&
	   (OpenClipboard(wnd->GetSafeHwnd())))
	{
		//data
		HANDLE hData = GetClipboardData( ::RegisterClipboardFormat(_ClipboardToken));
		BYTE * buffer = (BYTE*)GlobalLock( hData );
		int length = GlobalSize(hData);
		
		BuffArchive ar(CArchive::load);

		ar.setData(buffer, length);

		NotationLite* newLite = new NotationLite;

		newLite->Serialize(ar);

		GlobalUnlock( hData );

		CloseClipboard();

		return newLite;
	}

	return NULL;
}


