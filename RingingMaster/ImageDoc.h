// ImageDoc.h : interface of the ImageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEDOC_H__09D3C24F_DF94_11D7_B6FB_000255162CF0__INCLUDED_)
#define AFX_IMAGEDOC_H__09D3C24F_DF94_11D7_B6FB_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ImageView;

class ImageDoc : public CDocument
{
protected: // create from serialization only
	ImageDoc();
	DECLARE_DYNCREATE(ImageDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	inline float GetZoomFactor() { return m_ZoomFactor; }

	BOOL OnPreparePrinting(CPrintInfo* pInfo, CView* view);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnFilePrintPreview(CView* view);
	void OnFilePrint();
	void OnFilePrintToolbar();
	void OnFilePrintMenu();
	BOOL _showPrintSetupDlg;

	bool saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format);


	CxImage* getImage();
	virtual ~ImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	float m_ZoomFactor;

	ImageView* getImageView();

	CxImage* _image;
// Generated message map functions
protected:
	//{{AFX_MSG(ImageDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageZoomin();
	afx_msg void OnUpdateImageZoomin(CCmdUI *pCmdUI);
	afx_msg void OnImageZoomout();
	afx_msg void OnUpdateImageZoomout(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDOC_H__09D3C24F_DF94_11D7_B6FB_000255162CF0__INCLUDED_)
