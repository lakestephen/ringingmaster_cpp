// Key.h: interface for the Key class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEY_H__F6474653_F361_11D4_B571_009027BB3286__INCLUDED_)
#define AFX_KEY_H__F6474653_F361_11D4_B571_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum KeyRetType
{
	KR_OK,
	KR_BAD_USER,
	KR_BAD_VERSION,
	KR_BAD_KEY_LENGTH,
	KR_BAD_KEY_FORMAT,
	KR_BAD_KEY_WITHDRAWN,
};

class Key  
{
public:
	static CString getErrorString(KeyRetType error);
	static CString calculateKey(CString user, int version);
	static KeyRetType verifyKey(CString user, int version, CString key);
	static KeyRetType verifyKeyRange(CString user, Ints * version, CString key);

	Key();
	virtual ~Key();	
private:	
	static CString getCheckSum(CString key);
	static int getUserInt(CString user);
	static CString intToBase62String(int conversionInt);		   
	static int base62StringToInt(LPCTSTR conversionString);
	static const char lookup[];	   
};

#endif // !defined(AFX_KEY_H__F6474653_F361_11D4_B571_009027BB3286__INCLUDED_)
