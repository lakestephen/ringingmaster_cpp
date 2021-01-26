#pragma once
//


#include "MatrixDoc.h"

class GrabDocumentInfo;
class GrabView;
/////////////////////////////////////////////////////////////////////////////
// GrabDoc document

class GrabDoc : public MatrixDoc
{
protected:
	GrabDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(GrabDoc)

// Attributes
public:
	void setGrabDocInfo(GrabDocumentInfo* grabDocInfo);
	GrabView* getGrabView();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GrabDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GrabDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(GrabDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
