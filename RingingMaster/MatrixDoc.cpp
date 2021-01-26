// MatrixDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixDoc.h"
#include "MatrixView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MatrixDoc

IMPLEMENT_DYNCREATE(MatrixDoc, CDocument)

MatrixDoc::MatrixDoc()
{
}

BOOL MatrixDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

MatrixDoc::~MatrixDoc()
{
}


BEGIN_MESSAGE_MAP(MatrixDoc, CDocument)
	//{{AFX_MSG_MAP(MatrixDoc)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MatrixDoc diagnostics

#ifdef _DEBUG
void MatrixDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void MatrixDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MatrixDoc serialization

void MatrixDoc::Serialize(CArchive& /*ar*/)
{
	//we do not serialise this data
	AfxThrowArchiveException(CArchiveException::endOfFile);	   
}

/////////////////////////////////////////////////////////////////////////////
// MatrixDoc commands


BOOL MatrixDoc::OnSaveDocument(LPCTSTR /*lpszPathName*/) 
{
	return TRUE;	
	//return CDocument::OnSaveDocument(lpszPathName);
}	   

MatrixGridCtrl* MatrixDoc::getMatrixGridCtrl()
{
	MatrixView* view = getMatrixView();
	if (view)
		return view->getMatrixGridCtrl();

	return NULL;
}

MatrixView* MatrixDoc::getMatrixView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	MatrixView* pMatrixView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pMatrixView = dynamic_cast<MatrixView*>(pView);
		if (pMatrixView)
			return pMatrixView; 
	}   
	return NULL;
}



