#if !defined(AFX_INTERNETVIEW_H__C23F17B4_9A86_11D7_B6CF_000255162CF0__INCLUDED_)
#define AFX_INTERNETVIEW_H__C23F17B4_9A86_11D7_B6CF_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InternetView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InternetView html view

class InternetDoc;
class InternetDocumentInfo;

class InternetView : public CHtmlView
{
protected:
	InternetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(InternetView)

// html Data
public:

	InternetDoc* GetDocument();

	//{{AFX_DATA(InternetView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void setInitialData(CString URL);
	InternetDocumentInfo* getInternetDocumentInfo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InternetView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnInitialUpdate();
	virtual void OnProgressChange(long nProgress, long nProgressMax);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~InternetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(InternetView)
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnGoStartPage();
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
	afx_msg void OnViewFontsLargest();
	afx_msg void OnViewFontsLarge();
	afx_msg void OnViewFontsMedium();
	afx_msg void OnViewFontsSmall();
	afx_msg void OnViewFontsSmallest();
	afx_msg void OnUpdateGoForward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGoBack(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#ifndef _DEBUG  // debug version in TouchView.cpp
inline InternetDoc * InternetView::GetDocument()
   { return (InternetDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETVIEW_H__C23F17B4_9A86_11D7_B6CF_000255162CF0__INCLUDED_)
