#pragma once

#include "MatrixView.h"

class GrabDocumentInfo;
/////////////////////////////////////////////////////////////////////////////
// GrabView view

class GrabView : public MatrixView
{
protected:
	GrabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(GrabView)

// Attributes
public:
	GrabDocumentInfo* getGrabDocumentInfo();
	void setGrabDocInfo(GrabDocumentInfo* grabDocInfo);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerView)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
/*todo 	
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);*/
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GrabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	MatrixEntityToken getMatrixEntityToken() const;
	
	//{{AFX_MSG(GrabView)
	afx_msg void OnPopupEditgrab();
	afx_msg void OnPopupSethometower();
	afx_msg void OnPopupShowmap();
	afx_msg void OnPopupGrabtower();
	afx_msg void OnPopupEditDayOrder();
	afx_msg void OnPopupDeletegrab();
	afx_msg void OnLaunchWebPage();
	afx_msg void OnUpdateLaunchWebPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupShowmap(CCmdUI* pCmdUI);

/*	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintMenu();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintPreview();
*/
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

