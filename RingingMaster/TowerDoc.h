#pragma once
//


#include "MatrixDoc.h"

class TowerDocumentInfo;
class TowerView;
/////////////////////////////////////////////////////////////////////////////
// TowerDoc document

class TowerDoc : public MatrixDoc
{
protected:
	TowerDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TowerDoc)

// Attributes
public:
	void setTowerDocInfo(TowerDocumentInfo* towerDocInfo);
	TowerView* getTowerView();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TowerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(TowerDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

