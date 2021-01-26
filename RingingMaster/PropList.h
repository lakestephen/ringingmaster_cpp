#if !defined(AFX_PROPLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
#define AFX_PROPLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropList.h : header file

/////////////////////////////////////////////////////////////////////////////
// PropList window

class PropItem;
#include "CJListBox.h" 

class PropList : public CCJListBox		//todo replace this class with BCG stuff
{
// Construction
public:
	PropList();	 

// Attributes
public:

// Operations
public:
	int AddPropItem(PropItem* pItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropList)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void setColumnStyle(int index, UINT style);
	int findItemIndex(const CString &col0);
	int insertPropItem(int index, PropItem* pItem);
	void setColumnHeads(CString col1, CString col2 = CString(""), CString col3 = CString(""));
	PropItem* findItem(const CString& col0);
	virtual void doKillFocusCmbBox(PropItem* pItem);
	virtual void doKillFocusEditBox(PropItem* pItem);
	virtual void DoChangeEditBox(PropItem* pItem);
	CBCGPButton* getButtonWnd();
	void setButtonToolTip(CString tip);
	void setButtImages(UINT _img, UINT _imgHot);
	PropItem * getPropItem(int index);
	void deleteAllPropItems();
	void deletePropItem(int index);
	void setColumnHead(bool head = true);
	void setColumnCount(int columnCount);
	virtual ~PropList();
   	int _devider[2];
	virtual void DoSelchangeCmbBox(PropItem* pItem);


	// Generated message map functions
protected:

	UINT _columnStyle[3];

	UINT _buttImg;
	UINT _buttImgHot;
	CString _buttToolTip;
	bool _showColumnHeads;
	BOOL _columnCount;
	//{{AFX_MSG(PropList)
	afx_msg void OnSelchange();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnKillfocusCmbBox();
	afx_msg void OnSelchangeCmbBox();
	afx_msg void OnKillfocusEditBox();
	afx_msg void OnChangeEditBox();
	afx_msg void OnButton();

	DECLARE_MESSAGE_MAP()


	virtual void doButton(PropItem * pItem);

	CComboBox _combo;
	CEdit _edit;
	CBCGPButton _button;
	

	int _nDivTop;
	int _nDivBtm;
	int _nOldDivX;
	int _nLastBox;
	BOOL _deviderMove;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPLIST_H__74205380_1B56_11D4_BC48_00105AA2186F__INCLUDED_)
