// GrabDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GrabDoc.h"
#include "UserManager.h"
#include "RecordManager.h"
#include "GrabDocumentInfo.h"
#include "GrabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GrabDoc
IMPLEMENT_DYNCREATE(GrabDoc, MatrixDoc)

GrabDoc::GrabDoc()
{
}

BOOL GrabDoc::OnNewDocument()
{
	if (!MatrixDoc::OnNewDocument())
		return FALSE;

	SetTitle("Grabs");

	//valid user?
	if (!userManager()->checkAndCorrectValidity())
	{
		AfxMessageBox("No valid user setup.");
		return FALSE;
	}

	if (!recordManager()->isValid())
	{
		AfxMessageBox("Grab.dat failed to load.");
		return FALSE;
	}	

	return TRUE;
}

GrabDoc::~GrabDoc()
{
}


BEGIN_MESSAGE_MAP(GrabDoc, MatrixDoc)
	//{{AFX_MSG_MAP(GrabDoc)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GrabDoc diagnostics

#ifdef _DEBUG
void GrabDoc::AssertValid() const
{
	MatrixDoc::AssertValid();
}

void GrabDoc::Dump(CDumpContext& dc) const
{
	MatrixDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GrabDoc serialization

void GrabDoc::Serialize(CArchive& /*ar*/)
{
	//we do not serialise this data
	AfxThrowArchiveException(CArchiveException::endOfFile);	   
}

/////////////////////////////////////////////////////////////////////////////
// GrabDoc commands


BOOL GrabDoc::OnSaveDocument(LPCTSTR /*lpszPathName*/) 
{
	return TRUE;	
	//return MatrixDoc::OnSaveDocument(lpszPathName);
}	   

void GrabDoc::setGrabDocInfo(GrabDocumentInfo* grabDocInfo)
{
	GrabView* grabView = getGrabView();
	if (grabView)
		grabView->setGrabDocInfo(grabDocInfo);
}

GrabView* GrabDoc::getGrabView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	GrabView* pGrabView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pGrabView = dynamic_cast<GrabView*>(pView);
		if (pGrabView)
			return pGrabView; 
	}   
	return NULL;
}

