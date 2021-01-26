// WinPos.h: interface for the WinPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPOS_H__AD3B4EA5_2428_4670_9248_160D74A10E3D__INCLUDED_)
#define AFX_WINPOS_H__AD3B4EA5_2428_4670_9248_160D74A10E3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class WinPos  
{
public:
	static void Load(CWnd *win, CString token, bool includeSize);
	static void Save(CWnd* win, CString token);
	WinPos();
	virtual ~WinPos();

};

#endif // !defined(AFX_WINPOS_H__AD3B4EA5_2428_4670_9248_160D74A10E3D__INCLUDED_)
