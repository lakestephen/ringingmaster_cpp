#if !defined(AFX_INTERNETDOC_H__C23F17B6_9A86_11D7_B6CF_000255162CF0__INCLUDED_)
#define AFX_INTERNETDOC_H__C23F17B6_9A86_11D7_B6CF_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InternetDoc.h : header file
//

class InternetView;

#define FIRST_HISTORY_COMMAND	0x1000
#define HISTORY_LEN	10

#include "HistoryObj.h"

typedef CList<HistoryObj*, HistoryObj*> _T_HistotyList;

/////////////////////////////////////////////////////////////////////////////
// InternetDoc document

class InternetDoc : public CDocument
{
protected:
	InternetDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(InternetDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InternetDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void setInitialData(CString url);
	virtual ~InternetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	InternetView* getInternetView();
	CString _initialURL;
	//{{AFX_MSG(InternetDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	CArray<HistoryObj*, HistoryObj*>	m_arHistory;
	int	m_iHistoryOffset;

// Operations
public:
	void GetBackList (_T_HistotyList& lst) const;
	void GetFrwdList (_T_HistotyList& lst) const;

	BOOL IsBackAvailable () const
	{
		return m_iHistoryOffset < m_arHistory.GetSize () - 1;
	}

	BOOL IsFrwdAvailable ()
	{
		return m_iHistoryOffset > 0;
	}

	HistoryObj* GoBack ()
	{
		if (m_iHistoryOffset >= m_arHistory.GetSize ())
		{
			ASSERT (FALSE);
			return NULL;
		}

		return m_arHistory [++ m_iHistoryOffset];
	}

	HistoryObj* GoForward ()
	{
		if (m_iHistoryOffset < 1)
		{
			ASSERT (FALSE);
			return NULL;
		}

		return m_arHistory [--m_iHistoryOffset];
	}

	HistoryObj* Go (UINT uiCmd);
	HistoryObj* AddURLToHistory (const CString& strTitle, const CString& strURL);

private:
	CString _onOpenDocument;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNETDOC_H__C23F17B6_9A86_11D7_B6CF_000255162CF0__INCLUDED_)
