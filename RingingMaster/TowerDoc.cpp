// TowerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerDoc.h"
#include "UserManager.h"
#include "TowerDocumentInfo.h"
#include "RecordManager.h"
#include "TowerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerDoc

IMPLEMENT_DYNCREATE(TowerDoc, MatrixDoc)

TowerDoc::TowerDoc()
{
}

BOOL TowerDoc::OnNewDocument()
{
	if (!MatrixDoc::OnNewDocument())
		return FALSE;

	SetTitle("Towers");

	//valid user?
	if (!userManager()->checkAndCorrectValidity())
	{
		AfxMessageBox("No valid user setup.");
		return FALSE;
	}

	if (!recordManager()->isValid())
	{
		AfxMessageBox("Tower.dat failed to load.");
		return FALSE;
	}	

	return TRUE;
}

TowerDoc::~TowerDoc()
{
}


BEGIN_MESSAGE_MAP(TowerDoc, MatrixDoc)
	//{{AFX_MSG_MAP(TowerDoc)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerDoc diagnostics

#ifdef _DEBUG
void TowerDoc::AssertValid() const
{
	MatrixDoc::AssertValid();
}

void TowerDoc::Dump(CDumpContext& dc) const
{
	MatrixDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TowerDoc serialization

void TowerDoc::Serialize(CArchive& /*ar*/)
{
	//we do not serialise this data
	AfxThrowArchiveException(CArchiveException::endOfFile);	   
}

/////////////////////////////////////////////////////////////////////////////
// TowerDoc commands


BOOL TowerDoc::OnSaveDocument(LPCTSTR /*lpszPathName*/) 
{
	return TRUE;	
	//return MatrixDoc::OnSaveDocument(lpszPathName);
}	   

void TowerDoc::setTowerDocInfo(TowerDocumentInfo* towerDocInfo)
{
	TowerView* towerView = getTowerView();
	if (towerView)
		towerView->setTowerDocInfo(towerDocInfo);
}

TowerView* TowerDoc::getTowerView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	TowerView* pTowerView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pTowerView = dynamic_cast<TowerView*>(pView);
		if (pTowerView)
			return pTowerView; 
	}   
	return NULL;
}


