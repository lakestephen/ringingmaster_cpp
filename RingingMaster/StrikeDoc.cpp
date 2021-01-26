// StrikeDoc.cpp : implementation of the StrikeDoc class
//

#include "stdafx.h"
#include "ringingmaster.h"

#include "StrikeDoc.h"
#include ".\strikedoc.h"
#include "PlayManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// StrikeDoc

IMPLEMENT_DYNCREATE(StrikeDoc, CDocument)

BEGIN_MESSAGE_MAP(StrikeDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
END_MESSAGE_MAP()


// StrikeDoc construction/destruction

StrikeDoc::StrikeDoc() : 
_selectedRow(-1)
{
	playManager()->addEventListener(this);
}

StrikeDoc::~StrikeDoc()
{
	playManager()->removeEventListener(this);
}

BOOL StrikeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	return TRUE;
}




// StrikeDoc serialization

void StrikeDoc::Serialize(CArchive& ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		_strikingData.Serialize(ar);
	}
	else
	{
		ar >> version;  
		_strikingData.Serialize(ar);
		_strikingData.calculateStats();
	}
}


// StrikeDoc diagnostics

#ifdef _DEBUG
void StrikeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void StrikeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// StrikeDoc commands


void StrikeDoc::setInitialData(const StrikingData& strikingData)
{
	_strikingData = strikingData;
	_strikingData.createNewUniqueId();
	_strikingData.calculateStats();

	SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
}

void StrikeDoc::setUpdatePoint(CString name, bool setModifyed)
{
	CWaitCursor wait;
	if (setModifyed) SetModifiedFlag(TRUE); 
}


void StrikeDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

	((MainFrame*) AfxGetMainWnd ())->UpdateMRUFilesList ();
}

const StrikingData& StrikeDoc::getStrikingData()
{
	return _strikingData;
}

void StrikeDoc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(IsModified());
}

void StrikeDoc::playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage)
{
	if ((!rowPositionChangedPackage._tracking) || 
		(rowPositionChangedPackage._playRequestId != _strikingData.getUniqueId()) ||
		(!rowPositionChangedPackage._rowPosition.isValid(true)))
	{
		doGoto(-1, true);	
	}
	else
	{
		doGoto(rowPositionChangedPackage._rowPosition.getLeadRow(true),  true);	
	}
}

//to2do implement the Goto GUI
//to2do also implement status bar info, and left click on row.
//to2do also implement printing
//to2do also add some percents to the rows
//to2do add some tooltips with row numbers and percents
//to2do scroll to as part ofg the goto - remember that the threads can get in the way.

void StrikeDoc::doGoto(int row,  bool quiet) 
{
	if (row >=0 && row < _strikingData.GetSize())
	{
		_selectedRow = row;
	}
	else
	{
		_selectedRow = -1;
		if (!quiet)
		{
			CString str;
			str.Format("%d is not a valid row", row);
			AfxMessageBox(str);
		}
	}
	//can be from a seperate thread

	POSITION viewPosition = GetFirstViewPosition();   

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		if (pView)
			pView->Invalidate();
	}   
}
