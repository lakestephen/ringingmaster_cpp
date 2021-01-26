// NotationLite.h: interface for the NotationLite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTATIONLITE_H__1A525284_B199_11D5_B4F1_000255162CF0__INCLUDED_)
#define AFX_NOTATIONLITE_H__1A525284_B199_11D5_B4F1_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NotationPersist.h"
#include "GlobalDataManager.h"

class Notation;

class NotationLite : public NotationPersist 
{
public:
	static CString _ClipboardToken;
	static void CopyToClipboard(CWnd* wnd, NotationLite* notation);
	static NotationLite* NotationLite::PasteFromClipboard(CWnd* wnd);
	static BOOL NotationLite::IsClipboardFormatAvailable();
	
	CString getFolderPath();
	void setName(CString str);
	int getNumber();
	CString getSpliceLetter();
	void setFolderID(unsigned long folderID);
	unsigned long getFolderID();
	NotationLite::NotationLite(const NotationLite& notation);
	NotationLite();
	virtual ~NotationLite();

	CString getName(bool showNumber = false);
	CString getNotation(bool showSymmetry = false, SeperationStyle seperationStyle = SEP_NONE);
	CString getLeadEnd();
	const CString& getLeadHead() {return _leadHead;}
	CString getCCInfo(NotationCCInfoType type);


	void operator =(const Notation& notation);
	void operator =(const NotationLite& notation);


protected:
	CString getDisplayString(CString &str, SeperationStyle seperationStyle);

	CString _folderPath;


};

typedef CArray<NotationLite*, NotationLite*> NotationLites;


#endif // !defined(AFX_NOTATIONLITE_H__1A525284_B199_11D5_B4F1_000255162CF0__INCLUDED_)
