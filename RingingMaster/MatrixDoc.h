#pragma once

class MatrixGridCtrl;
class MatrixView;
/////////////////////////////////////////////////////////////////////////////
// MatrixDoc document

class MatrixDoc : public CDocument
{
protected:
	MatrixDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MatrixDoc)

// Attributes
public:
	MatrixGridCtrl* getMatrixGridCtrl();
	MatrixView* getMatrixView();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatrixDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MatrixDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(MatrixDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

