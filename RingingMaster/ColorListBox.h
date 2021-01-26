#if !defined(AFX_COLORLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
#define AFX_COLORLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ColorListBox.h : header file

//-------------------------------------------------------------------
//
//	ColorListBox class - 
//		A CListBox-derived class with optional colored items.
//
//		Version: 1.0	01/10/1998 Copyright � Patrice Godard
//
//		Version: 2.0	09/17/1999 Copyright � Paul M. Meidinger
//
//-------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// ColorListBox window

class StringColor
{
public:
	StringColor(const CString& str, COLORREF color) :
	_str(str),
	_color(color)
	{ }

	CString getString() {return _str;}
	COLORREF getColor() {return _color;}
protected:
	CString _str;
	COLORREF _color;
};

typedef CArray<StringColor*, StringColor*> StringColors;



class ColorListBox : public CListBox
{
// Construction
public:
	ColorListBox();

// Attributes
public:

// Operations
public:
	int AddString(LPCTSTR lpszItem);											// Adds a string to the list box
	int AddString(LPCTSTR lpszItem, COLORREF rgb);						// Adds a colored string to the list box
	int InsertString(int nIndex, LPCTSTR lpszItem);						// Inserts a string to the list box
	int InsertString(int nIndex, LPCTSTR lpszItem, COLORREF rgb);	// Inserts a colored string to the list box
	void SetItemColor(int nIndex, COLORREF rgb);							// Sets the color of an item in the list box
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColorListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ColorListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(ColorListBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTBOX_H__5529A6B1_584A_11D2_A41A_006097BD277B__INCLUDED_)
