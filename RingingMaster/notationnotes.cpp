// NotationNotes.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationNotes.h"
#include "NotationDlg.h"
#include "Notation.h"
#include ".\notationnotes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationNotes property page

NotationNotes::NotationNotes( Notation &notation) : 
CBCGPPropertyPage(NotationNotes::IDD, NULL),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationNotes)
	_notes = _T("");
	//}}AFX_DATA_INIT
}

NotationNotes::~NotationNotes()
{
}

void NotationNotes::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationNotes)
	DDX_Text(pDX, IDC_NOTES, _notes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationNotes, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationNotes)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationNotes message handlers


BOOL NotationNotes::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		_notation.setNotes(_notes);	
		return TRUE;
	}

	return FALSE;
}

BOOL NotationNotes::OnSetActive()
{
	_notes = _notation.getNotes();	

	return CBCGPPropertyPage::OnSetActive();
}
