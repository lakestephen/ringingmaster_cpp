// ribDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CribDoc.h"
#include "Crib.h"
#include "Notation.h"
#include "MethodCrib.h"
#include "NotationDlg.h"
#include "Lead.h"
#include "Style.h"
#include "CribView.h"
#include "CribManager.h"
#include "NotesDlg.h"
#include "PrintData.h"
#include "GotoLeadRowDlg.h"
#include "GotoRowDlg.h"
#include "PlayManager.h"
#include "TowerManager.h"
#include "Tower.h"
#include "ProofManager.h"
#include "ProofInput.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CribDoc

IMPLEMENT_DYNCREATE(CribDoc, CDocument)

BEGIN_MESSAGE_MAP(CribDoc, CDocument)
	//{{AFX_MSG_MAP(CribDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_POPUP_EDITNOTES, OnPopupEditnotes)
	ON_COMMAND(ID_POPUP_EDITMETHOD, OnPopupEditmethod)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CribDoc construction/destruction

CribDoc::CribDoc() :
_method(NULL),
_methodPositionStatusBar(NULL),
_cribNumber(-1)

{	 
	//set up the styles
	_cribStyle = cribManager()->getDefaultCribStyle();

	playManager()->addEventListener(this);

}

CribDoc::~CribDoc()
{
	delete _method;
	_method = NULL;

	delete _methodPositionStatusBar ;
	_methodPositionStatusBar = NULL;

	playManager()->removeEventListener(this);

}

BOOL CribDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CribDoc serialization

void CribDoc::Serialize(CArchive& ar)
{
	int version = 1;
	
	if (ar.IsStoring())
	{
		//version
		ar << version;

		//notation
		_notation.Serialize(ar);

				//licence checks
		if ((mainFrame()->verifyKey() == FALSE)&&
			(_notation.getNumber() > 6))
		{							
#ifndef _DEBUG			
			CString msg;
			msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
				GlobalFunctions::numberName(_notation.getNumber()),
				_notation.getNumber());			
			AfxMessageBox(msg);
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
		}

		//copy the style for the blue line, then serialize it
		Style style;
		style = getCribView()->getStyle();
		style.Serialize(ar);

		_cribStyle.Serialize(ar);

		//notes
		ar << _notes;
		ar << _showNotes;	

		//title
		ar << _title ;

		//crib number 
		ar << _cribNumber;

	}
	else
	{
		ar >> version;

		_notation.Serialize(ar);

		
		//licence checks
		if ((mainFrame()->verifyKey() == FALSE)&&
			(_notation.getNumber() > 6))
		{					
#ifndef _DEBUG						
			CString msg;
			msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) crib, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
				GlobalFunctions::numberName(_notation.getNumber()),
				_notation.getNumber());			
			AfxMessageBox(msg);
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
		}

		//get the style for the blue line, then serialize it
		Style style;
		style.Serialize(ar);		
		getCribView()->setStyle(style);

		_cribStyle.Serialize(ar);

		//notes
		ar >> _notes;
		ar >> _showNotes;

		//title
		ar >> _title;

		//crib number 
		ar >> _cribNumber;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CribDoc diagnostics

#ifdef _DEBUG
void CribDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CribDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CribDoc commands

void CribDoc::setInitialData(const Notation& notation)
{	  

	CWaitCursor wait;

	//save the notation.
	_notation = notation;

	SetTitle(_notation.getName(true));

	setUpdatePoint("NewDocument");	

	SetModifiedFlag();
}


MethodCrib* CribDoc::getMethod()
{
	return _method;
}

const Notation& CribDoc::getNotation()
{ 
	return _notation;
}

void CribDoc::OnPopupEditmethod() 
{
	NotationDlg dlg(getCribView(), _notation, FALSE, NULL, true, false, true);

	if (dlg.DoModal() != IDCANCEL)
	{
		setUpdatePoint("Edit " + _notation.getName(true));
	}		
}


void CribDoc::setUpdatePoint(CString name, bool setModifyed, bool checkPoint, bool bRecalcCrib)
{
	CWaitCursor wait;
	if (setModifyed) SetModifiedFlag(TRUE); 
	if (checkPoint) CheckPoint(name);
	if (bRecalcCrib) recalcCrib();
	UpdateAllViews(NULL);  //this must be before the fireUpdate, else when editing anything, get a crash as the touch window is not referencing the correct _method 
	mainFrame()->fireUpdate();
}	 

void CribDoc::DeleteContents() 
{
	// do nothing
	
	CDocument::DeleteContents();
}

void CribDoc::recalcCrib()
{   /*
	ProofInput* proofInput = new ProofInput(_notation.getNumber());
	proofInput->addNotation(_notation);
	proofInput->setLeadLimit(1);
	proofInput->setCallFrom(_cribNumber);
	proofInput.setPerformAnalysis(true); //todo do we want to do analysis here?

	delete _method;
	_method = new MethodCrib;
	proofManager()->requestProof(proofInput, *_method);

	if ((_cribNumber == -1)&&(_method->GetSize() >0))
	{
		//calculate the lowest non hunt crib number
		Ints hunts;

		_method->GetAt(0)->getHuntBells(hunts);
		
		_cribNumber = 1;

		for (int i=0;i<_method->getNumber();i++)
		{
			bool huntBell = false;
			for (int j=0;j<hunts.GetSize();j++)
			{
				if (i==j)
					huntBell = true;
			}
			
			if (!huntBell)
			{
				_cribNumber = i+1;
				break;
			}
		}		

		//recalculate to make the crib number effective
		ProofInput* proofInput = new ProofInput(_notation.getNumber());
		proofInput->addNotation(_notation);
		proofInput->setLeadLimit(1);
		proofInput->setCallFrom(_cribNumber);
		proofInput.setPerformAnalysis(true); //todo do we want to do analysis here?

		delete _method;
		_method = new MethodCrib;
		proofManager()->requestProof(proofInput, *_method);

	}

	if (!_title.IsEmpty())
	{
		_method->_proofInput->setName(_title);
	}
	   */ //todo
}

BOOL CribDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	setUpdatePoint("OpenDocument", false);	
	
	return TRUE;
}



void CribDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified());
}

void CribDoc::bulkUndo(int undoCount) 
{
	DoUndo(undoCount);
	setUpdatePoint("", true, false);
}

void CribDoc::bulkRedo(int redoCount) 
{
	DoRedo(redoCount);
	setUpdatePoint("", true, false);
}   

void CribDoc::OnEditRedo() 
{
	DoRedo(1);
	setUpdatePoint("", true, false);
	
}

void CribDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanRedo());
	if (CanRedo())
		pCmdUI->SetText("Redo " + RedoWhat());
	else			
		pCmdUI->SetText("Can't Redo");
	
}

void CribDoc::OnEditUndo() 
{
	DoUndo(1);
	setUpdatePoint("", true, false);
}

void CribDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanUndo());
	if (CanUndo())
		pCmdUI->SetText("Undo " + UndoWhat());
	else			
		pCmdUI->SetText("Can't Undo");

}
		 
void CribDoc::undoRange(int count)
{
	DoUndo(count);
	setUpdatePoint("", true, false);
}

void CribDoc::redoRange(int count)
{
	DoRedo(count);
	setUpdatePoint("", true, false);
}


CribView* CribDoc::getCribView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	CribView* pCribView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pCribView = dynamic_cast<CribView*>(pView);
		if (pCribView)
			return pCribView;
	}   
	return NULL;
}

const FontInfo& CribDoc::getCribFont()
{					 
	return _cribStyle._cribFont;
}

const FontInfo& CribDoc::getCribNotesFont()
{					 
	return _cribStyle._cribNotesFont;
}

void CribDoc::setCribFont(const FontInfo& font)
{					 
	_cribStyle._cribFont = font;
	setUpdatePoint("Set Crib Font");
}

void CribDoc::setCribNotesFont(const FontInfo& font)
{					 
	_cribStyle._cribNotesFont = font;
	setUpdatePoint("Set Crib Notes Font");
}

void CribDoc::setNotes(CString notes)
{
	if (_notes != notes)
	{
		_notes = notes;
		setUpdatePoint("Edit Notes");
	}
}

bool CribDoc::editNotes(CPoint point)
{
	NotesDlg dlg(NULL, _notes, point);

	dlg.DoModal();

	if (dlg._notes != _notes)
	{
		_notes = dlg._notes;
		setUpdatePoint("Edit Notes");
		return true;
	}
	return false;
}

CString CribDoc::getNotes()
{
	return _notes;
}

void CribDoc::OnPopupEditnotes() 
{
	CPoint point;
	GetCursorPos(&point);
	if (editNotes(point))
	{
		_showNotes = TRUE;
		setUpdatePoint("", false, false, false);
	}
}

void CribDoc::setShowNotes(BOOL showNotes)
{
	if (showNotes != _showNotes)
	{
		_showNotes = showNotes;								 		
		setUpdatePoint(showNotes?"Show Notes":"Hide Notes");
	}														 
}


BOOL CribDoc::getShowNotes()
{
	return _showNotes;
}





void CribDoc::setTitle(CString title)
{
	if (_title == title)
		return;
	/*
	if ((_method)&&(_method->getName() != title))
	{
		_title = title;

		CString path = m_strPathName;
		if (path.IsEmpty())
			SetTitle(_title);

		setUpdatePoint("Set Title");
	}
	else
	{
		_title = "";
	}*/ //todo
}

CString CribDoc::getTitle()
{   /*
	if ((_title.IsEmpty())&&(_method)) 
	{
		return _method->getName();		
	}
	  */ //todo
	return _title;	
}

void CribDoc::setMethodPositionStatusBar(MethodPosition *methodPositionStatusBar)
{
	delete _methodPositionStatusBar ;
	_methodPositionStatusBar = methodPositionStatusBar;
}

void CribDoc::getBlueLineStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5,CString &pane6)
{


	if (_methodPositionStatusBar != NULL)
	{
		pane1 = _methodPositionStatusBar->format(rpft_lead);
		pane2 = _methodPositionStatusBar->format(rpft_rowDisplay);
 		pane3 = _methodPositionStatusBar->format(rpft_mp_place);
		pane4 = _methodPositionStatusBar->format(rpft_mp_bell);
		pane5 = _methodPositionStatusBar->format(rpft_mp_stroke);
	}
	else 
	{
		pane1.Empty();
		pane2.Empty();
		pane3.Empty();
		pane4.Empty();
		pane5.Empty();
	}		

	Tower* tower = towerManager()->getTower();
	if (tower && _method && !playManager()->isStopped())
		pane6 = GlobalFunctions::getPealTimeText(tower->getNumberInclCover(_method->getNumber()), 
												tower->getSpeed(),
												tower->getOpenHandstrokeGap());
	else
		pane6.Empty();
}

int CribDoc::getCribNumber()
{
	return _cribNumber;
}

void CribDoc::setCribNumber(int cribNumber)
{
	if (cribNumber <= 0 ||
		cribNumber > getNumber())
	{
		setUpdatePoint("", false, false, false);
		return; 
	}

	if (_cribNumber != cribNumber)
	{
		_cribNumber = cribNumber;
		setUpdatePoint("Set Crib Number");
	}
}

int CribDoc::getNumber()
{
	return _notation.getNumber();

}





BOOL CribDoc::OnPreparePrinting(CPrintInfo* pInfo, CView* view)
{
	TRACE("OnPreparePrinting\r\n");
	
	
	//a bit of jiggery to force the standard print dialog not to show.
	// see DoPreparePrinting for information
	BOOL holdDirectState = pInfo->m_bDirect;
	if (!_showPrintSetupDlg)
	{
		pInfo->m_bDirect = TRUE;
	}	

	BOOL ret = view->DoPreparePrinting(pInfo); //this is where we get the print dialog

	//go back st the previous state
	pInfo->m_bDirect = holdDirectState ;

	return ret;
}

void CribDoc::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnBeginPrinting\r\n");

	PrintData* printData = new PrintData;
	pInfo->m_lpUserData = printData;
}

void CribDoc::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	//only do stuff if we are printing / preview
	if ((pInfo)&&(pDC->IsPrinting()))
	{
		PrintData* printData = (PrintData*)pInfo->m_lpUserData;

		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= printData->_numPages );
		TRACE("CribDoc::OnPrepareDC  curPage = %d  numPages = %d pInfo->m_bContinuePrinting = %s\r\n",pInfo->m_nCurPage, printData->_numPages,  pInfo->m_bContinuePrinting?"TRUE":"FALSE");
	}
}	 

void CribDoc::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnEndPrinting\r\n");

	PrintData* printData = (PrintData*)pInfo->m_lpUserData;
	delete printData;
	
	pInfo->m_lpUserData = NULL;
}

void CribDoc::OnFilePrintPreview(CView* view) 
{
	BCGPPrintPreview (view);
}

//this is the standard print. - standard print setup
// will be used by the print button on teh 
//  print preview screen, and also external printing
//this should go direct to the print dialog, otherwise the Setup Dlg will be shown twice
void CribDoc::OnFilePrint() 
{
	_showPrintSetupDlg = TRUE;
	//DO NOT SET _printAnalysis here it is used for both analysis and main print!!!
}

//print from toolbar -  no setup at all
void CribDoc::OnFilePrintToolbar() 
{
	_showPrintSetupDlg = FALSE;
}

//print frm menu - using setup dialogs
void CribDoc::OnFilePrintMenu() 
{
	_showPrintSetupDlg = TRUE;
}


void CribDoc::OnFileSaveAs() 
{
		
#ifdef _DEBUG
	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT(pTemplate != NULL);

#endif //_DEBUG

	CString title;
	VERIFY(title.LoadString(AFX_IDS_SAVEFILE));

	CString strFilter;
	CString strDefault;

	//FILES
	strFilter += "Crib File (*.crb)|*.crb|";
//	strFilter += "Word Document File (*.doc)|*.doc|"; //to2do
	strFilter += RingingMasterApp::GetImageFileTypes();			

	//all files
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += "|*.*||";

	
	CString fileName;

	CFileDialog dlg(FALSE, 
				"",NULL,
				OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
				strFilter,NULL);


	dlg.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);;
	dlg.m_ofn.lpstrTitle = title;

	if (dlg.DoModal() == IDOK)
	{
		fileName.ReleaseBuffer();

			   
		//extract the path
		char szDrive[200];
		char szDir[200];
		char szFileName[200];
		char szExt[200];

		_splitpath(fileName, szDrive, szDir, szFileName, szExt);

		CString extention = szExt;


		ENUM_CXIMAGE_FORMATS imageFormat = RingingMasterApp::GetImageType(extention);

		if (imageFormat != CXIMAGE_FORMAT_UNKNOWN)
		{
			saveAsImage(fileName, imageFormat);	
		}
		else if (extention == ".doc")
		{
			saveAsRTF(fileName);	
		}
		else 
		{
			if (!DoSave(fileName))
			{
				TRACE("Warning: File save-as failed.\n");	
			}
		}

	}
	else
	{
		fileName.ReleaseBuffer();
	}	
}
bool CribDoc::saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format)
{
	//file name will be a full path.
	TRACE(fileName + "\r\n");

	ASSERT(!fileName.IsEmpty());
	
	if (fileName.IsEmpty())
		return false;
	 
	CxImage* image = getCribView()->getImage();

	if (image)
	{
		if (format == CXIMAGE_FORMAT_GIF)
			image->DecreaseBpp(8, true);

		image->Save(fileName, format);
	}

	delete image;

	return true;
}


bool CribDoc::saveAsRTF(CString fileName)
{
	//file name will be a full path.
	TRACE(fileName + "\r\n");
	 //to2do 


	return true;
}


void CribDoc::playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage)
{
	if ((!_method) || 
		(!rowPositionChangedPackage._tracking) || 
		(rowPositionChangedPackage._playRequestId != _method->getUniqueId()))
	{
		static RowPosition invalidRow;
		ASSERT(!invalidRow.isValid());
		doGoto(invalidRow, true, true);	
	}
	else
		doGoto(rowPositionChangedPackage._rowPosition, true, true);	
}

//nullable means that the hilight row will be cleared if it is not valid
void CribDoc::doGoto(const RowPosition& rowPosition, bool nullable, bool quiet)
{
	if (_method && rowPosition.isValid())
	{	
		RowPosition rowPosMod;
		rowPosMod.setRow(_method, 0,  rowPosition.getLeadRow());

		if ((rowPosMod.isValid())||(nullable))
		{
			//show the blue line window.
			CribView* pCribView = getCribView();

			if (pCribView)
			{
				//hilight the row

				pCribView->getMethodWnd().hilightRow(rowPosMod);			
			}
		}
		else
		{
			if (!quiet)
				AfxMessageBox("Not a valid row");
		}
	}
	else if (!quiet)
	{
		AfxMessageBox("Not a valid row");
	}

}

void CribDoc::OnEditGoto() 
{
	if(!_method)
		return;

	if (globalDataManager()->isMethodRowType())
	{
		GotoRowDlg dlg(NULL, _method->getRowCount());
		if(dlg.DoModal() == IDOK)
		{
			RowPosition rowPosition;
			rowPosition.setRow(_method, dlg._row-1);

			doGoto(rowPosition);
		}
	}
	else
	{
		GotoLeadRowDlg dlg(NULL, _method);
		if(dlg.DoModal() == IDOK)
		{
			RowPosition rowPosition;
			rowPosition.setRow(_method, dlg._lead-1, dlg._row-1);
			doGoto(rowPosition);
		}
	}	
}

void CribDoc::OnUpdateEditGoto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_method != NULL);	
}


void CribDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

	((MainFrame*) AfxGetMainWnd ())->UpdateMRUFilesList ();
}
