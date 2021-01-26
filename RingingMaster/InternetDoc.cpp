// InternetDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "InternetDoc.h"
#include "InternetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InternetDoc

IMPLEMENT_DYNCREATE(InternetDoc, CDocument)

InternetDoc::InternetDoc()
{
	m_iHistoryOffset = 0;
	m_arHistory.SetSize (0, 1);

}

BOOL InternetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE; 

	SetTitle("Web");
	return TRUE;
}

InternetDoc::~InternetDoc()
{
		for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		ASSERT (m_arHistory [i] != NULL);
		delete m_arHistory [i];
	}

}


BEGIN_MESSAGE_MAP(InternetDoc, CDocument)
	//{{AFX_MSG_MAP(InternetDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InternetDoc diagnostics

#ifdef _DEBUG
void InternetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void InternetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// InternetDoc serialization

void InternetDoc::Serialize(CArchive& ar)
{
	//we do not serialise this data

	if (ar.IsLoading())
	{
		getInternetView()->setInitialData(_onOpenDocument);	
	}
	else
	{
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// InternetDoc commands

void InternetDoc::setInitialData(CString url)
{
	getInternetView()->setInitialData(url);
}



InternetView* InternetDoc::getInternetView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	InternetView* pInternetView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pInternetView = dynamic_cast<InternetView*>(pView);
		if (pInternetView)
			return pInternetView; 
	}   
	return NULL;
}




HistoryObj* InternetDoc::AddURLToHistory (const CString& strTitle, const CString& strURL)
{
	ASSERT (m_arHistory.GetSize () <= HISTORY_LEN);

	for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		HistoryObj* pObj = m_arHistory [i];
		ASSERT (pObj != NULL);

		if (pObj->GetTitle () == strTitle &&
			pObj->GetURL () == strURL)
		{
			return pObj;
		}
	}

	if (m_arHistory.GetSize () == HISTORY_LEN)
	{
		delete m_arHistory [0];
		m_arHistory.RemoveAt (0);
	}

	HistoryObj* pObj = new HistoryObj (strTitle, strURL, 
		FIRST_HISTORY_COMMAND + m_arHistory.GetSize ());
	m_arHistory.InsertAt (0, pObj);

	m_iHistoryOffset = 0;
	return pObj;
}
//****************************************************************************************
void InternetDoc::GetBackList (_T_HistotyList& lst) const
{
	lst.RemoveAll ();
	for (int i = m_iHistoryOffset + 1; i < m_arHistory.GetSize () ; i ++)
	{
		lst.AddTail (m_arHistory [i]);
	}
}
//****************************************************************************************
void InternetDoc::GetFrwdList (_T_HistotyList& lst) const
{
	lst.RemoveAll ();
	for (int i = m_iHistoryOffset - 1; i >= 0; i --)
	{
		ASSERT (i < m_arHistory.GetSize ());
		lst.AddTail (m_arHistory [i]);
	}
}
//****************************************************************************************
HistoryObj* InternetDoc::Go (UINT uiCmd)
{
	for (int i = 0; i < m_arHistory.GetSize (); i ++)
	{
		HistoryObj* pObj = m_arHistory [i];
		ASSERT (pObj != NULL);

		if (pObj->GetCommand () == uiCmd)
		{
			m_arHistory.RemoveAt (i);
			m_arHistory.Add (pObj);

			m_iHistoryOffset = 0;
			return pObj;
		}
	}

	return NULL;
}

BOOL InternetDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	_onOpenDocument = lpszPathName;	

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	
	return TRUE;
}
