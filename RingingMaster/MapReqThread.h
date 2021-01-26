#if !defined(AFX_MAPREQTHREAD_H__7915C08D_94EC_11D7_B6CA_000255162CF0__INCLUDED_)
#define AFX_MAPREQTHREAD_H__7915C08D_94EC_11D7_B6CA_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapReqThread.h : header file
//


class CxImage;
class MapCacheItem;

/////////////////////////////////////////////////////////////////////////////
// MapReqThread thread

class MapReqThread : public CWinThread
{
	DECLARE_DYNCREATE(MapReqThread)
protected:
	MapReqThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void terminate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapReqThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CxImage* getMapImageExternal(const MapCacheItem& mapCacheItem);
	void cropImage(const MapCacheItem& mapCacheItem, CxImage* image);

	virtual ~MapReqThread();

	// Generated message map functions
	//{{AFX_MSG(MapReqThread)
		afx_msg void OnGetMap(UINT wParam, LONG lParam);
		afx_msg void OnTerminate(UINT wParam, LONG lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BOOL _terminated;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPREQTHREAD_H__7915C08D_94EC_11D7_B6CA_000255162CF0__INCLUDED_)
