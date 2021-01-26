#if !defined(AFX_NOTATIONLISTCTRL_H__B9957702_B773_11D5_BB7F_F333FB19907B__INCLUDED_)
#define AFX_NOTATIONLISTCTRL_H__B9957702_B773_11D5_BB7F_F333FB19907B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationListCtrl.h : header file
//
#include "CJListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// NotationListCtrl window

class NotationListCtrl : public CCJListCtrl
{
// Construction
public:
	NotationListCtrl();

// Attributes
public:

// Operations
public:

	virtual bool SortList(int nCol, bool bAscending);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationListCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString getFolderName(unsigned long item);
	virtual CString getEmptyListText();
	virtual void showMethods();
	virtual void init();
	virtual ~NotationListCtrl();

	static int compareFolderPaths( const void * arg1, const void * arg2 );	
	static int compareNames( const void * arg1, const void * arg2 );
	static int compareNumber( const void * arg1, const void * arg2 );
	static int compareNotation( const void * arg1, const void * arg2 );
	static int comparele( const void * arg1, const void * arg2 );
	static int comparelh( const void * arg1, const void * arg2 );
	static int compareShorthand( const void * arg1, const void * arg2 );
	static int compareRWRef( const void * arg1, const void * arg2 );
	static int compareDateTower( const void * arg1, const void * arg2 );
	static int comparePlace( const void * arg1, const void * arg2 );
	static int compareDateHand( const void * arg1, const void * arg2 );
	static int compareCCC( const void * arg1, const void * arg2 );
	static int comparePMM( const void * arg1, const void * arg2 );
	static int compareTDMM( const void * arg1, const void * arg2 );
	static int compareMUG( const void * arg1, const void * arg2 );

	NotationLite** _liteList;
	int _liteListCount;

protected:
	UnsignedLongs _childFolders;

	CString getItemString(int row, int column);

	CString getPressedTestString(int row, int column);


	static BOOL _lastDirSort;

	int _lastContextItem;

	CImageList _imageList;	 

	//{{AFX_MSG(NotationListCtrl)
		afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnMeCribmethod();
		afx_msg void OnMePreview();
		afx_msg void OnMeInfo();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTATIONLISTCTRL_H__B9957702_B773_11D5_BB7F_F333FB19907B__INCLUDED_)
