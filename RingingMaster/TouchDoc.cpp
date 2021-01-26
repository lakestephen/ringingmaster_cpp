// TouchDoc.cpp : implementation file
//									 

#include "stdafx.h"
#include "RingingMaster.h"
#include "MainFrame.h"
#include "TouchDoc.h"
#include "NotationExplorerDlg.h"
#include "Method.h"
#include "ParseData.h"
#include "Call.h"
#include "TouchBlueLineView.h"
#include "TouchSimulatorView.h"
#include "TouchView.h"
#include "TouchHoldView.h"
#include "CellElement.h"
#include "Variance.h"
#include "VarianceDlg.h"
#include "MusicDlg.h"
#include "Tower.h"
#include "GenericEditDlg.h"
#include "Variance.h"
#include "Definition.h"
#include "DefinitionEditDlg.h"
#include "NotationResultsDlg.h"
#include "SearchDlg.h"
#include "GotoRowDlg.h"
#include "GotoLeadRowDlg.h"
#include "Lead.h"  
#include "FalseRow.h"
#include "AllTheWorkBell.h"
#include "PrintAnalysisSetupDlg.h"
#include "PrintData.h"
#include "UserMessages.h"
#include "MethodEditDlg.h"
#include "MethodPosition.h"	 
#include "MethodDlg.h"	   
#include "NotesDlg.h"
#include "PrintUtils.h"
#include "NewDocumentWizard.h"
#include "SaveAsSelectionDlg.h"
#include "NotationDlg.h"
#include "TouchObject.h"
#include "PlayManager.h"
#include "MethodSelectRow.h"
#include "ProofManager.h"
#include ".\touchdoc.h"
#include "UndoBar.h"
#include "SimulatorManager.h"
#include "TowerManager.h"
#include "ProofInput.h"
#include "TouchAnalysisCallsMade.h"
#include "TouchAnalysisStatus.h"
#include "TouchAnalysisMusic.h"
#include "TouchAnalysisCourseHead.h"
#include "TouchAnalysisLeadEnds.h"
#include "TouchAnalysisSplice.h"
#include "TouchAnalysisSpliceTransition.h"
#include "TouchAnalysisMultiBlock.h"
#include "TouchAnalysisFalseness.h"
#include "TouchAnalysisAllTheWork.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TouchDoc

IMPLEMENT_DYNCREATE(TouchDoc, CDocument)

TouchDoc::TouchDoc() :
Undo(300),
_method(NULL),
_tab(0),
_number(0),
_activeNotation(""),
_grid(NULL),
_gridPos(0,0,0),
_autoProve(TRUE),
_showGrid(TRUE),
_showCourseHeads(TRUE),
_partCountMax(-1),
_leadCountMax(-1),
_rowCountMax(10000),
_circularTouchMax(2),
_startAtRow(0),
_startStroke(hand),
_courseBased(TRUE),
_plainLeadLetter("p"),
_name("Touch"),
_author(""),
_blockCount(1),
_printAnalysis(FALSE),
_showPrintSetupDlg(TRUE),
_showNotes(TRUE),
_methodPositionBlueLineStatusBar(NULL),
_methodPositionSimulatorStatusBar(NULL),
_proofRequestID(-1), 
_wrapCalls(TRUE)
{
	CString user = app()->GetProfileString("MainFrame","User Name", "");	

	if (!user.IsEmpty())
	{
		_author = "by " + user + ".";
	}

	proofManager()->addEventListener(this);
	playManager()->addEventListener(this);
}

BOOL TouchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	_touchStyle = globalDataManager()->_touchStyle;
	createGrid(1,1);

	setUpdatePoint("NewDocument", false, false, false);	

	return TRUE;
}

void TouchDoc::setInitialData(const NewDocumentWizardTransfer& trans)
{		 
	//transfer the data
	_number		= trans.getNumber();
	_callFrom	= trans.getNumber();
	_name		= trans._name;

	for (int i=0;i<trans._notations.GetSize();i++)
	{
 		_notations.Add(new Notation(*trans._notations.GetAt(i)));
	}

	sortNotations();

	if (_notations.GetSize() == 1)
	{
		_activeNotation = _notations.GetAt(0)->getName(true);
	}
	else if (_notations.GetSize() > 1)
	{
		_activeNotation = Spliced;
	}
	
	SetTitle(trans._name);

	_courseBased = trans._courseBased;

	autoCreateSplicedLetters();
	addDefaultCallPositionsToBlankTouch();
	addDefaultMusic();

	getTouchView()->calculateFontSizes();


	setUpdatePoint("NewDocument", (_notations.GetSize() > 0));	
}

BOOL TouchDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!_grid)
	{
		//we havent deserialised - probably the user has created 
		// a blank document using explorer
		if (!OnNewDocument())
			return FALSE;

		char szfName[2000];

		_splitpath(lpszPathName, NULL, NULL, szfName, NULL);

		CString str(szfName);

		int pos = -1;
		if ((pos = str.ReverseFind('\\')) == -1)
			pos = str.ReverseFind('/');			
		if (pos != -1)
			str = str.Mid(pos, str.GetLength());	

		NewDocumentWizardTransfer trans;
		trans._name = str;
		setInitialData(trans);
	}
	else
	{

		//mark the start of the document
		//need an update first to get the initial grid parsing done 
		//(else first time in we get a plain course
		setUpdatePoint("OpenDocument", false, true, false);	
		
		doTouchProve();

		//do a simple update 
		setUpdatePoint("OpenDocument", false, false, false); //is this done by the proof?	

	}					  
	return TRUE;
}


void TouchDoc::DeleteContents() 
{
	for (int i=0;i<_notations.GetSize();i++)
		delete _notations.GetAt(i);
	_notations.RemoveAll();
	
	destroyGrid();

	for (int i=0;i<_touchBlocks.GetSize();i++)
		delete _touchBlocks.GetAt(i);
	_touchBlocks.RemoveAll();

	for (int i=0;i<_definitions.GetSize();i++)
		delete _definitions.GetAt(i);
	_definitions.RemoveAll();

	for (int i=0;i<_musicRules.GetSize();i++)
		delete _musicRules.GetAt(i);
	_musicRules.RemoveAll();

	
	CDocument::DeleteContents();
}

TouchDoc::~TouchDoc()
{
	simulatorManager()->documentDeleted(this);

	delete _method;
	_method = NULL;

	proofManager()->removeEventListener(this);
	playManager()->removeEventListener(this);

	delete _methodPositionSimulatorStatusBar ;
	_methodPositionSimulatorStatusBar = NULL;

	delete _methodPositionBlueLineStatusBar;
	_methodPositionBlueLineStatusBar = NULL;
}


BEGIN_MESSAGE_MAP(TouchDoc, CDocument)
	//{{AFX_MSG_MAP(TouchDoc)
	
	ON_COMMAND(ID_ADD_FIND_NOTATION, OnAddFindNotation)
	ON_COMMAND(ID_ADD_NOTATION, OnAddNotation)
	ON_COMMAND(ID_ADD_EXPLORER_NOTATION, OnAddExplorerNotation)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_TOUCH_PROVE, OnTouchProve)
	ON_UPDATE_COMMAND_UI(ID_TOUCH_PROVE, OnUpdateTouchProve)
	ON_COMMAND(ID_TOUCH_ADDDEFINITION, OnAddDefinition)
	ON_COMMAND(ID_TOUCH_ADDMUSIC, OnAddMusic)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO, OnUpdateEditGoto)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CONVERTOLEADBASE, OnUpdateToolsConvertoleadbase)
	ON_COMMAND(ID_TOOLS_CONVERTOLEADBASE, OnToolsConvertoleadbaseInit)
	ON_COMMAND(ID_POPUP_EDITNOTES, OnPopupEditnotes)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_TOOLS_TRANSPOSE, OnToolsTranspose)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRANSPOSE, OnUpdateToolsTranspose)
	ON_COMMAND(ID_TOOLS_TIDY, OnToolsTidy)
	ON_COMMAND(ID_TOOLS_RESETTOYCH, OnToolsResettoych)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TIDY, OnUpdateToolsTidy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TouchDoc diagnostics

#ifdef _DEBUG
void TouchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void TouchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TouchDoc serialization
void GridPos::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _x;
		ar << _y;
		ar << _c;
	}
	else
	{
		ar >> version;
		ar >> _x;
		ar >> _y;
		ar >> _c;
	}
}

void TouchDoc::Serialize(CArchive& ar)
{
	//versions 
	//upto 1.07 = version 1, even though not the same document
	//version 1.08 onwards version 2
	//version 2.01 onwards version 3
	//version 3.01 onwards version 4
	//version 3.02 onwards version 5

	int version = 5;
	CString name;
	GetDocTemplate()->GetDocString(name, CDocTemplate::fileNewName);

	if (ar.IsStoring())
	{
		//checks
		ar << name;
		ar << version;

		//global information
		ar << _number;
		ar << _callFrom;
		ar << _name;
		
		//licence checks
		if ((mainFrame()->verifyKey() == FALSE)&&
			(_number > 6))
		{							
#ifndef _DEBUG			
			CString msg;
			msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
				GlobalFunctions::numberName(_number),
				_number);			
			AfxMessageBox(msg);
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
		}

		//our list of notations
		ar << _notations.GetSize();
		for (int i=0;i<_notations.GetSize();i++)
			_notations.GetAt(i)->Serialize(ar);			   
		ar << _activeNotation;
		
		//definitions
		ar << _definitions.GetSize();
		for (int i=0;i<_definitions.GetSize();i++)
			_definitions.GetAt(i)->Serialize(ar);

		//music checks
		ar << _musicRules.GetSize();
		for (int i=0;i<_musicRules.GetSize();i++)
			_musicRules.GetAt(i)->Serialize(ar);

		//the grid
		ar << _grid->GetWidth();
		ar << _grid->GetHeight();
		for (int i=0;i<_grid->GetWidth();i++)
			for (int j=0;j<_grid->GetHeight();j++)
				_grid->GetAt(i,j)->Serialize(ar);

		_gridPos.Serialize(ar);

		//touch blocks
		ar << _touchBlocks.GetSize();
		for (int i=0;i<_touchBlocks.GetSize();i++)
			_touchBlocks.GetAt(i)->Serialize(ar);

		//terminate info
		ar << _partCountMax;
		ar << _leadCountMax;
		ar << _rowCountMax;
		ar << _circularTouchMax;
		ar << _startAtRow;
		ar << _startRow;
		ar << _endRow;

		//other info 
		ar << _startStroke;
		ar << _courseBased;
		ar << _plainLeadLetter;
		ar << _blockCount;

		//non essential GUI information
		ar << _tab; //document tab 
		ar << _autoProve;
		ar << _showGrid;
		ar << _showCourseHeads;		

		/***************new in v 1.08 / v2 ***************************/

		//the notation that will get prepended on to the start of the touch
		_startNotation.Serialize(ar);

		//copy the style for the blue line, then serialize it
		Style style;
		style = getBlueLineView()->getStyle();
		style.Serialize(ar);

		//touch style
		_touchStyle.Serialize(ar);

		/***************new in v 2.01 / v3 ***************************/

		//notes
		ar << _notes;
		ar << _showNotes;

		/***************new in v 3.01 / v4 ***************************/

		ar << _author;

		/***************new in v 3.02 / v5 ***************************/
    
		ar << _wrapCalls;
	}
	else
	{
		//init
		CString thisName;
		int thisVersion;

		//checks
		ar >> thisName;
		if (thisName != name) 			
			AfxThrowArchiveException(CArchiveException::endOfFile);	   

		ar >> thisVersion;

		if (thisVersion == 1) //upto and including version 1.07
			deserialize_v1(ar);
		else if (thisVersion == 2)	 //new in v 1.08
			deserialize_v2(ar);
		else if (thisVersion == 3)	 //new in v 2.01
			deserialize_v3(ar);
		else if (thisVersion == 4)   //new in v 3.01
			deserialize_v4(ar);
		else if (thisVersion == 5)   //new in v 3.02
			deserialize_v5(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
	}
}
void TouchDoc::deserialize_v5(CArchive& ar)
{
	TRACE("Loading a version 5 TouchDoc \r\n");

	int count;
	Style style;

	//global information
	ar >> _number;
	ar >> _callFrom;
	ar >> _name;

	//licence checks
	if ((mainFrame()->verifyKey() == FALSE)&&
		(_number > 6))
	{					
#ifndef _DEBUG						
		CString msg;
		msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
			GlobalFunctions::numberName(_number),
			_number);			
		AfxMessageBox(msg);
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
	}


	//our list of notations			
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Notation * not = new Notation();
		not->Serialize(ar);
		_notations.Add(not);
	}		

	ar >> _activeNotation;
	sortNotations();

	//definitions
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Definition* def = new Definition(this);
		def->Serialize(ar);
		_definitions.Add(def);
	}
	sortDefinitions();
  
	//music
	ar >> count;
	for (int i=0;i<count;i++)
	{
		MusicRule* music = new MusicRule(_number);
		music->Serialize(ar);
		_musicRules.Add(music);
	}
	sortMusicRules();
	
	//the grid
	int x,y;
	ar >> x;
	ar >> y;
	createGrid(x,y);
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
			_grid->GetAt(i,j)->Serialize(ar);

	_gridPos.Serialize(ar);

	//touch blocks
	ar >> count;
	for (int i=0;i<count;i++)
	{
		TouchBlock* tb = new TouchBlock();
		tb->Serialize(ar);
		_touchBlocks.Add(tb);
	}

	//terminate info
	ar >> _partCountMax;
	ar >> _leadCountMax;
	ar >> _rowCountMax;
	ar >> _circularTouchMax;
	ar >> _startAtRow;
	ar >> _startRow;
	ar >> _endRow;

	//other info 
	int stroke;
	ar >> stroke;
	_startStroke = static_cast<Stroke>(stroke);
	ar >> _courseBased;
	ar >> _plainLeadLetter;
	ar >> _blockCount;

	//GUI 
	ar >> _tab;
	ar >> _autoProve;
	ar >> _showGrid;
	ar >> _showCourseHeads;


	/***************new in v 1.08 / v2 ***************************/


	//the notation that will get prepended on to the start of the touch
	_startNotation.Serialize(ar);

	//get the style for the blue line, then serialize it
	style.Serialize(ar);
	
	if (app()->GetProfileInt("Style", "Save With Method", TRUE) == TRUE)
		getBlueLineView()->setStyle(style);

	//touch style
	_touchStyle.Serialize(ar);

	/***************new in v 2.01 / v3 ***************************/

	//notes
	ar >> _notes;
	ar >> _showNotes;

	/***************new in v 3.01 / v4 ***************************/

	ar >> _author;

	/***************new in v 3.02 / v5 ***************************/
    
	ar >> _wrapCalls;
}


void TouchDoc::deserialize_v4(CArchive& ar)
{
	TRACE("Loading a version 4 TouchDoc \r\n");

	int count;
	Style style;

	//global information
	ar >> _number;
	ar >> _callFrom;
	ar >> _name;

	//licence checks
	if ((mainFrame()->verifyKey() == FALSE)&&
		(_number > 6))
	{					
#ifndef _DEBUG						
		CString msg;
		msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
			GlobalFunctions::numberName(_number),
			_number);			
		AfxMessageBox(msg);
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
	}


	//our list of notations			
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Notation * not = new Notation();
		not->Serialize(ar);
		_notations.Add(not);
	}		

	ar >> _activeNotation;
	sortNotations();

	//definitions
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Definition* def = new Definition(this);
		def->Serialize(ar);
		_definitions.Add(def);
	}
	sortDefinitions();
  
	//music
	ar >> count;
	for (int i=0;i<count;i++)
	{
		MusicRule* music = new MusicRule(_number);
		music->Serialize(ar);
		_musicRules.Add(music);
	}
	sortMusicRules();
	
	//the grid
	int x,y;
	ar >> x;
	ar >> y;
	createGrid(x,y);
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
			_grid->GetAt(i,j)->Serialize(ar);

	_gridPos.Serialize(ar);

	//touch blocks
	ar >> count;
	for (int i=0;i<count;i++)
	{
		TouchBlock* tb = new TouchBlock();
		tb->Serialize(ar);
		_touchBlocks.Add(tb);
	}

	//terminate info
	ar >> _partCountMax;
	ar >> _leadCountMax;
	ar >> _rowCountMax;
	ar >> _circularTouchMax;
	ar >> _startAtRow;
	ar >> _startRow;
	ar >> _endRow;

	//other info 
	int stroke;
	ar >> stroke;
	_startStroke = static_cast<Stroke>(stroke);
	ar >> _courseBased;
	ar >> _plainLeadLetter;
	ar >> _blockCount;

	//GUI 
	ar >> _tab;
	ar >> _autoProve;
	ar >> _showGrid;
	ar >> _showCourseHeads;


	/***************new in v 1.08 / v2 ***************************/


	//the notation that will get prepended on to the start of the touch
	_startNotation.Serialize(ar);

	//get the style for the blue line, then serialize it
	style.Serialize(ar);
	
	if (app()->GetProfileInt("Style", "Save With Method", TRUE) == TRUE)
		getBlueLineView()->setStyle(style);

	//touch style
	_touchStyle.Serialize(ar);

	/***************new in v 2.01 / v3 ***************************/

	//notes
	ar >> _notes;
	ar >> _showNotes;

	/***************new in v 2.03 / v4 ***************************/

	ar >> _author;
}


void TouchDoc::deserialize_v3(CArchive& ar)
{
	TRACE("Loading a version 3 TouchDoc \r\n");

	int count;
	Style style;

	//global information
	ar >> _number;
	ar >> _callFrom;
	ar >> _name;

	//licence checks
	if ((mainFrame()->verifyKey() == FALSE)&&
		(_number > 6))
	{					
#ifndef _DEBUG						
		CString msg;
		msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
			GlobalFunctions::numberName(_number),
			_number);			
		AfxMessageBox(msg);
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
	}


	//our list of notations			
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Notation * not = new Notation();
		not->Serialize(ar);
		_notations.Add(not);
	}		

	ar >> _activeNotation;
	sortNotations();

	//definitions
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Definition* def = new Definition(this);
		def->Serialize(ar);
		_definitions.Add(def);
	}
	sortDefinitions();
  
	//music
	ar >> count;
	for (int i=0;i<count;i++)
	{
		MusicRule* music = new MusicRule(_number);
		music->Serialize(ar);
		_musicRules.Add(music);
	}
	sortMusicRules();
	
	//the grid
	int x,y;
	ar >> x;
	ar >> y;
	createGrid(x,y);
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
			_grid->GetAt(i,j)->Serialize(ar);

	_gridPos.Serialize(ar);

	//touch blocks
	ar >> count;
	for (int i=0;i<count;i++)
	{
		TouchBlock* tb = new TouchBlock();
		tb->Serialize(ar);
		_touchBlocks.Add(tb);
	}

	//terminate info
	ar >> _partCountMax;
	ar >> _leadCountMax;
	ar >> _rowCountMax;
	ar >> _circularTouchMax;
	ar >> _startAtRow;
	_startAtRow--;   //made 0 based
	ar >> _startRow;
	ar >> _endRow;

	//other info 
	int stroke;
	ar >> stroke;
	_startStroke = static_cast<Stroke>(stroke);
	//in v4 doc I inverted the meaning of the stroke, so earlier docks have to have there stroke inverted
	_startStroke = (_startStroke == hand)?back:hand;

	ar >> _courseBased;
	ar >> _plainLeadLetter;
	ar >> _blockCount;

	//GUI 
	ar >> _tab;
	ar >> _autoProve;
	ar >> _showGrid;
	ar >> _showCourseHeads;


	/***************new in v 1.08 / v2 ***************************/


	//the notation that will get prepended on to the start of the touch
	_startNotation.Serialize(ar);

	//get the style for the blue line, then serialize it
	style.Serialize(ar);
	
	if (app()->GetProfileInt("Style", "Save With Method", TRUE) == TRUE)
		getBlueLineView()->setStyle(style);

	//touch style
	_touchStyle.Serialize(ar);

	/***************new in v 2.01 / v3 ***************************/

	//notes
	ar >> _notes;
	ar >> _showNotes;
}

void TouchDoc::deserialize_v2(CArchive& ar)
{
	TRACE("Loading a version 2 TouchDoc \r\n");

	int count;
	Style style;

	//global information
	ar >> _number;
	ar >> _callFrom;
	ar >> _name;

	//licence checks
	if ((mainFrame()->verifyKey() == FALSE)&&
		(_number > 6))
	{					
#ifndef _DEBUG						
		CString msg;
		msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
			GlobalFunctions::numberName(_number),
			_number);			
		AfxMessageBox(msg);
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
	}


	//our list of notations			
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Notation * not = new Notation();
		not->Serialize(ar);
		_notations.Add(not);
	}		

	ar >> _activeNotation;
	sortNotations();

	//definitions
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Definition* def = new Definition(this);
		def->Serialize(ar);
		_definitions.Add(def);
	}
	sortDefinitions();
  
	//music
	ar >> count;
	for (int i=0;i<count;i++)
	{
		MusicRule* music = new MusicRule(_number);
		music->Serialize(ar);
		_musicRules.Add(music);
	}
	sortMusicRules();
	
	//the grid
	int x,y;
	ar >> x;
	ar >> y;
	createGrid(x,y);
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
			_grid->GetAt(i,j)->Serialize(ar);

	_gridPos.Serialize(ar);

	//touch blocks
	ar >> count;
	for (int i=0;i<count;i++)
	{
		TouchBlock* tb = new TouchBlock();
		tb->Serialize(ar);
		_touchBlocks.Add(tb);
	}

	//terminate info
	ar >> _partCountMax;
	ar >> _leadCountMax;
	ar >> _rowCountMax;
	ar >> _circularTouchMax;
	ar >> _startAtRow;
	_startAtRow--;   //made 0 based
	ar >> _startRow;
	ar >> _endRow;

	//other info 
	int stroke;
	ar >> stroke;
	_startStroke = static_cast<Stroke>(stroke);
	//in v4 doc I inverted the meaning of the stroke, so earlier docks have to have there stroke inverted
	_startStroke = (_startStroke == hand)?back:hand;

	ar >> _courseBased;
	ar >> _plainLeadLetter;
	ar >> _blockCount;

	//GUI 
	ar >> _tab;
	ar >> _autoProve;
	ar >> _showGrid;
	ar >> _showCourseHeads;


	/***************new in v 1.08 / v2 ***************************/


	//the notation that will get prepended on to the start of the touch
	_startNotation.Serialize(ar);

	//get the style for the blue line, then serialize it
	style.Serialize(ar);
	
	if (app()->GetProfileInt("Style", "Save With Method", TRUE) == TRUE)
		getBlueLineView()->setStyle(style);

	//touch style
	_touchStyle.Serialize(ar);


}

void TouchDoc::deserialize_v1(CArchive& ar)
{
	TRACE("Loading a version 1 TouchDoc \r\n");
	int count;
	
	//global information
	ar >> _number;
	ar >> _callFrom;
	ar >> _name;

	//licence checks
	if ((mainFrame()->verifyKey() == FALSE)&&
		(_number > 6))
	{					
#ifndef _DEBUG						
		CString msg;
		msg.Format("Unregistered version limited to Minor (6): Please Register\n\nAs this is a %s (%d) touch, it will not open.\nYou will now get an unexpected file format error, \nas the unregistered version does not support more than 6 bells", 
			GlobalFunctions::numberName(_number),
			_number);			
		AfxMessageBox(msg);
		AfxThrowArchiveException(CArchiveException::endOfFile);	   
#endif
	}


	//our list of notations			
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Notation * not = new Notation();
		not->Serialize(ar);
		_notations.Add(not);
	}		

	ar >> _activeNotation;
	sortNotations();

	//definitions
	ar >> count;
	for (int i=0;i<count;i++)
	{
		Definition* def = new Definition(this);
		def->Serialize(ar);
		_definitions.Add(def);
	}
	sortDefinitions();
  
	//music
	ar >> count;
	for (int i=0;i<count;i++)
	{
		MusicRule* music = new MusicRule(_number);
		music->Serialize(ar);
		_musicRules.Add(music);
	}
	sortMusicRules();
	
	//the grid
	int x,y;
	ar >> x;
	ar >> y;
	createGrid(x,y);
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
			_grid->GetAt(i,j)->Serialize(ar);

	_gridPos.Serialize(ar);

	//touch blocks
	ar >> count;
	for (int i=0;i<count;i++)
	{
		TouchBlock* tb = new TouchBlock();
		tb->Serialize(ar);
		_touchBlocks.Add(tb);
	}

	//terminate info
	ar >> _partCountMax;
	ar >> _leadCountMax;
	ar >> _rowCountMax;
	ar >> _circularTouchMax;
	ar >> _startAtRow;
	_startAtRow--;   //made 0 based
	ar >> _startRow;
	ar >> _endRow;

	//other info 
	int stroke;
	ar >> stroke;
	_startStroke = static_cast<Stroke>(stroke);
	//in v4 doc I inverted the meaning of the stroke, so earlier docks have to have there stroke inverted
	_startStroke = (_startStroke == hand)?back:hand;

	ar >> _courseBased;
	ar >> _plainLeadLetter;
	ar >> _blockCount;

	//GUI 
	ar >> _tab;
	ar >> count; //this is the old tab setting - Now does nothing	
	ar >> _autoProve;
	ar >> _showGrid;
	ar >> _showCourseHeads;

}


void TouchDoc::addDefaultCallPositionsToBlankTouch()
{	 
	int i,j, k;
	
	if (_courseBased)
	{

		//add default calling positions
		CStringArray strArr;
		if (!isSpliced())
		{
			Notation* not = getActiveNotation();
			if (not)
				not->getCallingPosInOrder(strArr, TRUE);
		}
		else
		{
			if (_notations.GetSize() > 0)
			{
				Notation* not = _notations.GetAt(0);
				if (not)
					not->getCallingPosInOrder(strArr, TRUE);
				
				for (i=1;i<_notations.GetSize();i++)
				{
					CStringArray tempStrArr;
					not = _notations.GetAt(i);
					if (not)
						not->getCallingPosInOrder(tempStrArr, TRUE);
					//make sure that they are the same
					if (strArr.GetSize() != tempStrArr.GetSize())
					{
						strArr.RemoveAll();
						break;
					}
					for (j=0;j<strArr.GetSize();j++)
					{
						if (strArr.GetAt(j) != tempStrArr.GetAt(j))
						{
							strArr.RemoveAll();
							break;
						}
					}
				}
			}
		}
		
		
				
		if (strArr.GetSize() > 0)
		{
			addRow();											
			addColumn();											

			for ( i=0;i<strArr.GetSize();i++)
			{
				CellData *pCellData = getCellData(GridPos(i,0,0));
				if (pCellData)
				{
					CString str = strArr.GetAt(i);
					for (int j=0;j<str.GetLength();j++)
						pCellData->onAddElement(str.GetAt(j), j) ;
					addColumn();											
				} 											
			}
			//add the first splice (This stops validation errors on the calling positions)
			if (isSpliced())
			{
				CString letter;
				for (k=0;k<_notations.GetSize();k++)
				{
					letter = _notations.GetAt(k)->getSpliceLetter();
					if (!letter.IsEmpty())
						break;
				}

				if (!letter.IsEmpty())
				{
					addRow();
					addColumn();											
					CellData *pCellData = getCellData(GridPos(i+1,1,0));
					for (j=0;j<letter.GetLength();j++)
						pCellData->onAddElement(letter.GetAt(j), j) ;
				}
			}
			tidyGrid();
			parseGrid();
			_gridPos._x = 0;
			_gridPos._y = (_grid->GetHeight() > 1)?1:0;
			_gridPos._c = 0;
		}

	}
}


void TouchDoc::autoCreateSplicedLetters()
{
	for (int i=0;i<_notations.GetSize();i++)
	{
		Notation* not = _notations.GetAt(i);
		if (not)
		{
			if (not->getSpliceLetter().IsEmpty())
			{
				//generate a unique spliced letter
				CString name = not->getName();
				for (int j=0;j<name.GetLength();j++)
				{
					CString letter;
					letter = name.Left(j+1);
					if (isSpliceUnique(letter))
					{
						not->setSpliceLetter(letter);
						break;
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// TouchDoc commands

//take control of the notation object

int TouchDoc::getTab()
{
	return _tab;
}

void TouchDoc::setTab(int tab)
{
	ASSERT((tab >= 0)&&(tab <= 3));
	_tab = tab;
	//do not set modifyed flag here. Not a savable change
}

void TouchDoc::OnAddMusic()
{
	MusicRule* musicRule = new MusicRule(_number);

	MusicDlg dlg(musicRule); 

	if (dlg.DoModal() == IDOK)
	{
		//check it is unique
		_musicRules.Add(musicRule);
		sortMusicRules();
		setUpdatePoint("Add Music");
	}
	else
		delete musicRule;
}


void TouchDoc::OnAddDefinition()
{
	Definition* definition = new Definition(this);

	DefinitionEditDlg dlg(definition, false, this);

	if (dlg.DoModal() == IDOK)
	{
		//check it is unique
		_definitions.Add(definition);
		sortDefinitions();
		setUpdatePoint("Add Definition");
	}
	else
		delete definition;
}




void TouchDoc::setBlockCount(int blockCount)
{
	if (blockCount <=0 || blockCount >= 10000)
	{
		setUpdatePoint("", true, true, true, true);
		return;
	}

	if (_blockCount != blockCount)
	{
		_blockCount = blockCount;

		setUpdatePoint("Set Checking Type / Count");
	}
}

void TouchDoc::editMusic(const CString& name, const CString& displayString, const CString& newRule)
{
	MusicRule * music = NULL;

	//find the definition
	for (int i=0;i<_musicRules.GetSize();i++)
		if ((_musicRules.GetAt(i)->getName() == name)&&
			(_musicRules.GetAt(i)->getDisplayString() == displayString))
			music = _musicRules.GetAt(i);

	if (!music) return;

	
	music->setRule(newRule); 

	//check it is unique
	sortMusicRules();
	setUpdatePoint("Edit Music");


}

void TouchDoc::editMusic(const CString& name, const CString& displayString)
{
	MusicRule* musicRule= NULL;

	//find the variance
	for (int i=0;i<_musicRules.GetSize();i++)
		if ((_musicRules.GetAt(i)->getName() == name)&&
			(_musicRules.GetAt(i)->getDisplayString() == displayString))
			musicRule = _musicRules.GetAt(i);

	if (!musicRule) return;

	MusicDlg dlg(musicRule);

	if (dlg.DoModal() == IDOK)
	{
		//check it is unique
		sortMusicRules();
		setUpdatePoint("Edit Music");
	}

	sortMusicRules();
}

void TouchDoc::deleteMusic(const CString& name, const CString& displayString)
{
	for (int i=0;i<_musicRules.GetSize();i++)
	{
		if ((_musicRules.GetAt(i)->getName() == name)&&
			(_musicRules.GetAt(i)->getDisplayString() == displayString))
		{
			MusicRule* mus = _musicRules.GetAt(i);
			_musicRules.RemoveAt(i);
			delete mus;
			mus = NULL;
		}
	}

	sortMusicRules();
	setUpdatePoint("Delete Variance");
	
}


void TouchDoc::editDefinition(CString name, CString definitionStr)
{
	Definition* definition = NULL;

	//find the definition
	for (int i=0;i<_definitions.GetSize();i++)
		if (_definitions.GetAt(i)->getName(true) == name)
			definition = _definitions.GetAt(i);

	if (!definition) return;

	definition->setDefinition(definitionStr);

	//check it is unique
	sortDefinitions();
	setUpdatePoint("Edit Definition");


}

void TouchDoc::editDefinition(CString name)
{
	Definition* definition = NULL;

	//find the definition
	for (int i=0;i<_definitions.GetSize();i++)
		if (_definitions.GetAt(i)->getName(true) == name)
			definition = _definitions.GetAt(i);

	if (!definition) return;

	DefinitionEditDlg dlg(definition, true, this);

	if (dlg.DoModal() == IDOK)
	{
		//check it is unique
		sortDefinitions();
		setUpdatePoint("Edit Definition");
	}

	sortDefinitions();
}

void TouchDoc::OnAddNotation() 
{
   	Notation * not = new Notation();
	not->create(false, "",_number,true,"");
	
	NotationDlg dlg(getTouchView(), *not, TRUE, this, false);
	if (dlg.DoModal() != IDCANCEL)
	{

 		_notations.Add(not);
 		sortNotations();
		autoCreateSplicedLetters();
		setUpdatePoint("Add " + not->getName(true));

		if (_activeNotation == "")
		{
			setActiveNotation(not->getName(true));
		}
	} 	
	else
	{
		delete not;
		not = NULL;
	}

}

void TouchDoc::deleteNotation(const CString& name) //do not use in message response
{
	if (_activeNotation == name) //poss drop out of any method
		setActiveNotation("");

	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == name)
		{
			Notation *not = _notations.GetAt(i);
			_notations.RemoveAt(i);
			delete not;
			not = NULL;
		}
	}

	if (_notations.GetSize() == 1) //poss drop out of spliced
		setActiveNotation(_notations.GetAt(0)->getName(true));

	setUpdatePoint("Delete " + name);
}

void TouchDoc::deleteDefinition(const CString& name) //do not use in message response
{
	for (int i=0;i<_definitions.GetSize();i++)
	{
		if (_definitions.GetAt(i)->getName(true) == name)
		{
			Definition *def = _definitions.GetAt(i);
			_definitions.RemoveAt(i);
			delete def;
			def = NULL;
		}
	}
	
	sortDefinitions();
	setUpdatePoint("Delete Definition");
}

void TouchDoc::editNotation(const CString& name) //do not use in message response
{
	CWaitCursor wait;

	bool weAreTheActiveNotation = (_activeNotation == name);

	Notation* not = NULL;

	//find the notaion
	for (int i=0;i<_notations.GetSize();i++)
		if (_notations.GetAt(i)->getName(true) == name)
			not = _notations.GetAt(i);

	if (!not) return;

	NotationDlg dlg(getTouchView(), *not, TRUE, this, true);

	UINT id = dlg.DoModal() ;
	if (id != IDCANCEL)
	{
		sortNotations();
		setUpdatePoint("Edit " + not->getName(true));
	}

	if (weAreTheActiveNotation) 
		setActiveNotation(not->getName(true), false);
}
			

void TouchDoc::addNotations(NotationLites& notations) 
{
	int insertCount = 0;
	CString lastInsertedMethod;

	for (int i=0;i<notations.GetSize();i++)
	{
		NotationLite* lite = notations.GetAt(i);
		Notation * not = new Notation();
		*not = *lite;
		delete lite;

		//check for correct number
		if (not->getNumber() != _number)
		{
			CString msg;
			msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch",   //to2do can we hav mixed touches?
				not->getName(true), GlobalFunctions::numberName(_number));
			AfxMessageBox(msg);
			delete not;
		}
		else if (!isNotationNameUnique(not->getName(true)))
		{
			CString msg;
			msg.Format("Not adding as %s is not a unique name", 
				not->getName(true));
			AfxMessageBox(msg);
			delete not;
		}
		else if (not->getName(false).IsEmpty())
		{
			CString msg;
			msg.Format("No Name");
			AfxMessageBox(msg);
			delete not;
		}
		else if (!isSpliceUnique(not->getSpliceLetter()))
		{
			CString msg;
			msg.Format("%s is not a unique shorthand", 
				not->getSpliceLetter());
			AfxMessageBox(msg);
			delete not;
		}
		else
		{
 			_notations.Add(not);
			sortNotations();
			autoCreateSplicedLetters();
			insertCount++;
			lastInsertedMethod = not->getName(true);

			if (_activeNotation == "")
			{
				setActiveNotation(not->getName(true));
			}

			//are we adding the first notations?
			if (_notations.GetSize() == 1)
				OnToolsResettoych();
		}
	}
	if (insertCount == 1)	
		setUpdatePoint("Insert " + lastInsertedMethod);
	else if (insertCount >1)
	{
		CString str;
		str.Format("Insert %d Methods", insertCount);
		setUpdatePoint(str);
	}

}

	  
void TouchDoc::OnAddExplorerNotation() 
{
	NotationLites notations;
	NotationExplorerDlg dlg(mainFrame(), notations, false, true, _number);
	if (dlg.DoModal() == IDOK)
		addNotations(notations);

}

void TouchDoc::OnAddFindNotation()
{
	NotationLites notations;
	SearchDlg dlg(notations, _number);
	if (dlg.DoModal() == IDOK)
		addNotations(notations);
}


bool TouchDoc::isNotationNameUnique(const CString& name)
{
	bool unique = true;
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == name)
			 unique = false;
	}
	return unique;
}

bool TouchDoc::isSpliceUnique(CString splice)
{
	if (splice.IsEmpty()) return true;
	bool unique = true;
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getSpliceLetter() == splice)
			 unique = false;
	}
	return unique;
}

void TouchDoc::sortNotations()
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<_notations.GetUpperBound();i++)
		{
			if (_notations[i]->getName() > _notations[i+1]->getName())
			{
				changed = true;
				Notation * temp    = _notations.GetAt(i);
				_notations.SetAt(i,_notations.GetAt(i+1));
				_notations.SetAt(i+1, temp);
			}
		}
	}
}


void TouchDoc::sortDefinitions()
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<_definitions.GetUpperBound();i++)
		{
			if ((_definitions.GetAt(i)->getType() >  _definitions.GetAt(i+1)->getType())||
				((_definitions.GetAt(i)->getType() == _definitions.GetAt(i+1)->getType())&&
				 (_definitions.GetAt(i)->getName() >  _definitions.GetAt(i+1)->getName())))
			{
				changed = true;
				Definition* temp    = _definitions.GetAt(i);
				_definitions.SetAt(i,_definitions.GetAt(i+1));
				_definitions.SetAt(i+1, temp);
			}
		}
	}
}

void TouchDoc::sortMusicRules()
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<_musicRules.GetUpperBound();i++)
		{
			if ((_musicRules.GetAt(i)->getName() >  _musicRules.GetAt(i+1)->getName())||
				((_musicRules.GetAt(i)->getName() == _musicRules.GetAt(i+1)->getName())&&
				 (_musicRules.GetAt(i)->getDisplayString() >  _musicRules.GetAt(i+1)->getDisplayString())))
			{
				changed = true;
				MusicRule* temp    = _musicRules.GetAt(i);
				_musicRules.SetAt(i,_musicRules.GetAt(i+1));
				_musicRules.SetAt(i+1, temp);
			}
		}
	}
}


void TouchDoc::bulkUndo(int undoCount) 
{
	getTouchView()->resetExtSelValid();
	DoUndo(undoCount);
	setUpdatePoint("", true, false);
	getTouchView()->updateCaret();
}

void TouchDoc::bulkRedo(int redoCount) 
{
	getTouchView()->resetExtSelValid();
	DoRedo( redoCount);
	setUpdatePoint("", true, false);
	getTouchView()->updateCaret();
}

void TouchDoc::OnEditUndo() 
{  
	//this is a single undo
	getTouchView()->resetExtSelValid();
	DoUndo(1);
	setUpdatePoint("", true, false);
	//view stuff
	getTouchView()->calculateFontSizes();
	getTouchView()->updateCaret();
}

void TouchDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanUndo());
	if (CanUndo())
		pCmdUI->SetText("Undo " + UndoWhat());
	else			
		pCmdUI->SetText("Can't Undo");	
}

void TouchDoc::OnEditRedo() 
{
	//this is a single redo
	getTouchView()->resetExtSelValid();
	DoRedo(1);
	setUpdatePoint("", true, false);
	//view stuff
	getTouchView()->calculateFontSizes();
	getTouchView()->updateCaret();
}

void TouchDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanRedo());
	if (CanRedo())
		pCmdUI->SetText("Redo " + RedoWhat());
	else			
		pCmdUI->SetText("Can't Redo");	 
}

void TouchDoc::setUpdatePoint(CString name, bool setModifyed, bool checkPoint, bool prove, bool tidy)
{
	CWaitCursor wait;
	if (setModifyed) SetModifiedFlag(TRUE); 
	if (checkPoint) CheckPoint(name);
	if (tidy) tidyGrid();
	parseGrid();
	invalidateColumnWidths();
	mainFrame()->fireUpdate();
	UpdateAllViews(NULL);
	if (prove) 
		doTouchProve();
}	 

int TouchDoc::getNumber()
{
	return _number;
}


bool TouchDoc::canSetNumber()
{
	return(_notations.GetSize() == 0 && !hasUserMusic());
}

//put in to stop problems with dialog box while calling from the prop list
void TouchDoc::setNumberIndirect(int number)
{
	if (_number != number)
		PostMessage(mainFrame()->GetSafeHwnd(), TOUCH_DOC_UPDATE_NUMBER, (WPARAM)this, number);
}

void TouchDoc::setNumber(int number)
{    
	ASSERT(_number != number);

	if (_number != number)
	{

		if (!canSetNumber()) 
		{
			if (AfxMessageBox("To set the number of bells, all methods and music detection must be removed. \nDo you want to continue?", MB_YESNO) != IDYES)
			{
				setUpdatePoint("", false, false, false, false);
				return;
			}
		}

		//remove all music 
		for (int i=0;i<_musicRules.GetSize();i++)
			delete _musicRules.GetAt(i);
		_musicRules.RemoveAll();

		//remove all notations
		for (int i=0;i<_notations.GetSize();i++)
			delete _notations.GetAt(i);
		_notations.RemoveAll(); 
		_activeNotation.Empty();

		//make the change to the number
		_number = number;

		// make sure call from is not higher than _number
		if (_callFrom >= _number)
			_callFrom = _number;

		//set start row.
		Row row(_number);
		_startRow = row.getAsText();
		_endRow = row.getAsText();  

		//sort music
		addDefaultMusic();	

		//view stuff
		getTouchView()->calculateFontSizes();

		setUpdatePoint("Change number of bells");
	} 
}


void TouchDoc::onFileClose() //just to provide access to a protected member
{
	OnFileClose();
}

void TouchDoc::cribNotation(const CString& name)
{
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == name)
		{
			_notations.GetAt(i)->crib();
		}
	}
}

void TouchDoc::setActiveNotation(const CString& name, bool update, bool allowSpliceConversion )
{
	if ((name == Spliced)||(name == ""))
	{
		if (_activeNotation != name)
		{
			_activeNotation = name;
			setUpdatePoint("Set Active Method",true, update);
		}
		return;
	}

	//we are not spliced
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == name)
		{
			if (_activeNotation != name)
			{
				_activeNotation = name;
				setUpdatePoint("Set Active Method",true, update);
			}	 
			else
			{
				//the name is the same
				if ((allowSpliceConversion)&&(_notations.GetSize() >1))
				{
					_activeNotation = Spliced;
    				setUpdatePoint("Set Active Method",true, update);
				}
			}

			return;
		}
	}
}
							
Notation * TouchDoc::getActiveNotation()
{
	for (int i=0;i<_notations.GetSize();i++)
	{
		if (_notations.GetAt(i)->getName(true) == _activeNotation)
		{
			return _notations.GetAt(i);
		}
	}

	return NULL;  
}

void TouchDoc::destroyGrid()
{
	if (_grid)
	{
		for (int i=0;i<_grid->GetWidth();i++)
			for (int j=0;j<_grid->GetHeight();j++)
			{
				delete _grid->GetAt(i,j);
				_grid->SetAt(NULL,i,j);
			}
	
		delete _grid;
		_grid = NULL;

		invalidateColumnWidths();
	}
}

void TouchDoc::createGrid(int x, int y)
{
	destroyGrid();

	_grid = new GridArray(x,y,NULL);

	//now populate the grid
	for (int i=0;i<_grid->GetWidth();i++)
		for (int j=0;j<_grid->GetHeight();j++)
		{
			_grid->SetAt(new CellData(this),i,j);
		}										
}

void TouchDoc::addColumn(int pos)
{
	_grid->AddColumn(NULL, pos);
	for (int j=0;j<_grid->GetHeight();j++)
	{
		_grid->SetAt(new CellData(this),(pos == -1)?_grid->GetWidth()-1:pos,j);
	}		
}

void TouchDoc::addRow(int pos)
{
	_grid->AddRow(NULL, pos);
	for (int i=0;i<_grid->GetWidth();i++)
	{
		_grid->SetAt(new CellData(this),i,(pos == -1)?_grid->GetHeight()-1:pos);
	}		
}

void TouchDoc::deleteRow(int row)
{
	if ((row<0)||(row > _grid->GetHeight()))
		return ;

	for (int i=0;i<_grid->GetWidth();i++)
	{
		delete _grid->GetAt(i,row);
		_grid->SetAt(NULL, i,row);
	}
	_grid->RemoveRow(row);
}		

void TouchDoc::deleteColumn(int column)
{
	if ((column<0)||(column > _grid->GetWidth()))
		return ;

	for (int j=0;j<_grid->GetHeight();j++)
	{
		delete _grid->GetAt(column, j);
		_grid->SetAt(NULL, column, j);
	}
	_grid->RemoveColumn(column);
}		

int TouchDoc::getColumnWidth(CDC *pDC, int column)
{
	//recalculate the widths
	if (_columnWidths.GetSize() != _grid->GetWidth())
	{
		_columnWidths.RemoveAll();
		int columnWidth;
		int cellWidth;

		for (int i=0;i<_grid->GetWidth();i++)
		{
			columnWidth = 0;

			for (int j=0;j<_grid->GetHeight();j++)
			{
				cellWidth = _grid->GetAt(i,j)->calculateCellWidth(pDC, getFontMain());
				if (cellWidth > columnWidth)
					columnWidth = cellWidth;
			}

			_columnWidths.Add(max(columnWidth + CELL_PAD_LEFT + CELL_PAD_RIGHT, CELL_MIN_WIDTH));
		}
	}
	
	//extract the data
	if ((column >=0)&&(column < _grid->GetWidth()))
		return _columnWidths.GetAt(column);
	else 
		return 0;
}

void TouchDoc::invalidateColumnWidths()
{
	_columnWidths.RemoveAll();
}

void TouchDoc::parseGrid()
{
	parseInit();

	//first pass stuff
	parseCallPos();
	parseSplices();
	parseMainBodyCallsAndBlocks();

	//logical stuff
	parseMultipleCallPosOnFirstLine();	
	parseBlockUseWithinBlock();
	parseSplicedCallsNotDefinedInEachMethod();
	parseSplicedCallPosMethodNotDefinedInEachMethod();
	parseSplicedCallPosAgregateNotDefinedInEachMethod();
	parseSpliceCountDifferentInEachMethod();
	parseVarianceLodgic();
	parseGroupLodgic();
	parseGroupOnDifferentLines();
	parseVarianceGroupInteractionLodgic();
	parseSpaceOnlyInCell();
	parseCallsInColumnWithoutCallingPos();
	parseSplicedNotBlocks();
	parseDefinitionsOnWrongSide();
	
	//done last so that invalidity can be passed to the numbers
	//must be done after parseSplices()
	parseMainBodyNumbers(); 
}

void TouchDoc::parseInit()
{
	int i,j, k;

	//set all of data elements to PT_NONE;
	for ( j=0;j<_grid->GetHeight();j++)
	{
		for ( i=0;i<_grid->GetWidth();i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for ( k=0;k<cellData->getElements().GetSize();k++)
					cellData->getElements().GetAt(k)->initElement();
			}
		}
	}

	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		CellData* cellData = definition->getCellData();
		for ( k=0;k<cellData->getElements().GetSize();k++)
			cellData->getElements().GetAt(k)->initElement();
	}
}

void TouchDoc::parseCallPos()
{
	if (!_courseBased) return; 
	
	int i,j;
	
	//CALL POSITIONS
	ParseDataArray* pParse = new ParseDataArray();
	
	if (isSpliced())
	{
		//go through each notation
		for (i=0;i<_notations.GetSize();i++)
		{
			Notation * not = _notations.GetAt(i);
			if (not)
			{
				not->prepare();
				
				//call pos 
				for ( j=0;j<not->getCallPosMethodCount();j++)
				{
					NotationCallPosMethod * CPM = not->getCallPosMethod(j);
					pParse->Add(new ParseData(CPM->_name, PT_CALL_POS));
				} 
				//agregate call pos 
				for ( j=0;j<not->getCallPosAgregateCount();j++)
				{
					NotationCallPosAgregate* CPA = not->getCallPosAgregate(j);
					pParse->Add(new ParseData(CPA->_name, PT_CALL_POS_AGREGATE));
				} 
			}
		}
	}
	else
	{
		Notation * not = getActiveNotation();
		if (not)
		{
			not->prepare();
			//call pos 
			for ( i=0;i<not->getCallPosMethodCount();i++)
			{
				NotationCallPosMethod * CPM = not->getCallPosMethod(i);
				pParse->Add(new ParseData(CPM->_name, PT_CALL_POS));
			} 
			//agregate call pos 
			for ( j=0;j<not->getCallPosAgregateCount();j++)
			{
				NotationCallPosAgregate* CPA = not->getCallPosAgregate(j);
				pParse->Add(new ParseData(CPA->_name, PT_CALL_POS_AGREGATE));
			} 

		}
	}		
		
	pParse->Add(new ParseData(" ", PT_WHITESPACE));

	if (_grid->GetHeight() >0)
	{
		for ( i=0;i<((isSpliced())?_grid->GetWidth()-2:_grid->GetWidth());i++)
		{	
			CellData* cellData = _grid->GetAt(i,0);
			if (cellData)
			{
				cellData->parseCell(pParse);
			}
		}
	}

	for ( i=0;i<pParse->GetSize();i++)
		delete pParse->GetAt(i);
	delete pParse;
}

void TouchDoc::parseMainBodyCallsAndBlocks()
{
	int i,j;
	
	//CALLS
	ParseDataArray* pParse = new ParseDataArray();

	//calls
	//get the active notation
	if (isSpliced())
	{
		//go through each notation
		for (i=0;i<_notations.GetSize();i++)
		{
			Notation * not = _notations.GetAt(i);
			if (not)
			{
				not->prepare();

				for ( j=0;j<not->getCallCount();j++)
				{
					NotationCall * call = not->getCall(j);
					pParse->Add(new ParseData(call->_nameShorthand, PT_CALL));
					pParse->Add(new ParseData(call->_nameLonghand, PT_CALL));
				}
			}	 
		}
	}
	else
	{
		Notation * not = getActiveNotation();
		if (not)
		{
			not->prepare();

			for ( i=0;i<not->getCallCount();i++)
			{
				NotationCall * call = not->getCall(i);
				pParse->Add(new ParseData(call->_nameShorthand, PT_CALL));
				pParse->Add(new ParseData(call->_nameLonghand, PT_CALL));
			}
		}
	}

	if (!_courseBased)
		pParse->Add(new ParseData(_plainLeadLetter, PT_PLAIN_LEAD));

	pParse->Add(new ParseData(" ", PT_WHITESPACE));

	//parse the definitions before adding all the ParseData
	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		if (definition->getType() == dt_call)
		{
			CellData* cellData = definition->getCellData();
			cellData->parseCell(pParse);
		}
	}

	//this should be AFTER parsing definitions 
	pParse->Add(new ParseData("[", PT_VARIANCE));
	pParse->Add(new ParseData("]", PT_VARIANCE));

	pParse->Add(new ParseData("(", PT_GROUP));
	pParse->Add(new ParseData(")", PT_GROUP));


	//blocks 
	for (i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		pParse->Add(new ParseData(tb->_name, PT_BLOCK));
	}

	//definitions
	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* def = _definitions.GetAt(i);
		pParse->Add(new ParseData(def->getName(), PT_DEFINITION));
	}

	//do the parsing
	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{
		for ( i=0;i<((isSpliced())?_grid->GetWidth()-2:_grid->GetWidth());i++)
		{	
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				cellData->parseCell(pParse);
			}
		}
	}


	for (i=0;i<pParse->GetSize();i++)
		delete pParse->GetAt(i);
	delete pParse;	 
}

void TouchDoc::parseDefinitionsOnWrongSide()
{
	int i,j,k,l;

	for ( j=0;j<_grid->GetHeight();j++)
	{
		for ( i=0;i<_grid->GetWidth();i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getWordCount();k++)
				{
					CellElement* element;
					element = cellData->getFirstElementInWord(k);
					if (element)
					{
						if (element->getType() == PT_DEFINITION)
						{
							CString name = cellData->getWordFromWordIndex(k);
							//find the definition
							for (l=0;l<_definitions.GetSize();l++)
							{
								if (_definitions.GetAt(l)->getName() == name)
								{
									Definition* def = _definitions.GetAt(l);
									//what side are we on;
									bool splice = (isSpliced())&&(i>=_grid->GetWidth()-2);
									bool defSpliced = def->getType() == dt_splice;
									if (splice && (!defSpliced))
										cellData->setInvalidWord(k, "Call definition used as splice");
									if ((!splice) && defSpliced)
										cellData->setInvalidWord(k, "Spliced definition used as call");
								}
							}
						}
					}
				}						
			}
		}
	}
}

void TouchDoc::parseVarianceGroupInteractionLodgic()
{
	if (isSpliced())
	{  
		parseVarianceGroupInteractionLodgic(0,_grid->GetWidth()-2);
		parseVarianceGroupInteractionLodgic(_grid->GetWidth()-2,_grid->GetWidth());
	}
	else
	{
		parseVarianceGroupInteractionLodgic(0,_grid->GetWidth());
	}  
}

void TouchDoc::parseVarianceGroupInteractionLodgic(int start,int end)
{
	int i,j,k;
	CList<CellElement*, CellElement*> elements;

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=max(start,0);i<min(end,_grid->GetWidth());i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if (element->isValid())
					{
						if((element->getChar() == '(')||
						   (element->getChar() == '['))
						{
							elements.AddHead(element);
						}
						else if((element->getChar() == ')')||
								(element->getChar() == ']'))						    
						{
							bool noMatch = false;
							//check for a mismatch
							while ((!elements.IsEmpty())&&
								   ((element->getChar() == ')' && elements.GetHead()->getChar() != '(')||
								    (element->getChar() == ']' && elements.GetHead()->getChar() != '[')))
							{
								elements.RemoveHead();
								noMatch = true;
							}
							//have we got a mismatch
							if (noMatch)
							{
								if (!elements.IsEmpty())
								{
									CellElement* stElement = elements.GetHead();
									stElement->setInvalid("Variances and groups must not intertwine");
									element->setInvalid("Variances and groups must not intertwine");
									elements.RemoveHead();
								}
							}
							 //we are OK
							else if (!elements.IsEmpty())
							{
								elements.RemoveHead();
							}
						}						
					}
				}				
			}
		}
	}
}

void TouchDoc::parseVarianceLodgic()
{
	if (isSpliced())
	{  
		parseVarianceLodgic(0,_grid->GetWidth()-2);
		parseNoVarianceInSplice(_grid->GetWidth()-2,_grid->GetWidth());
	}
	else
	{
		parseVarianceLodgic(0,_grid->GetWidth());
	}  
}

void TouchDoc::parseNoVarianceInSplice(int start,int end)
{
	int i,j,k;
	bool inBrace = false;
	CellElement* startBrace = NULL;

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=max(start,0);i<min(end,_grid->GetWidth());i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if ((element->getChar() == '[')||
						(element->getChar() == ']'))
					{
						element->setInvalid("Variance cant be used in splices");
					}
				}				
			}
		}
	}
	
	if (inBrace)
		startBrace->setInvalid("No matching closing brace");
}

void TouchDoc::parseVarianceLodgic(int start,int end)
{
	int i,j,k;
	bool inBrace = false;
	CellElement* startBrace = NULL;

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=max(start,0);i<min(end,_grid->GetWidth());i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if (!inBrace)
					{
						if (element->getChar() == '[')
						{
							inBrace = true;
							startBrace = element;
						}
						else if (element->getChar() == ']')
						{
							element->setInvalid("No matching opening brace");
						}
					}
					else//in brace 
					{
						if (element->getChar() == ']')
						{
							inBrace = false;
						}
						else if (element->getChar() == '[')
						{
							startBrace->setInvalid("No matching closing brace");
							inBrace = true;
							startBrace = element;
						}
					}
				}				
			}
		}
	}
	
	if (inBrace)
		startBrace->setInvalid("No matching closing brace");
}

void TouchDoc::parseGroupOnDifferentLines()
{
	if (isSpliced())
	{  
		parseGroupOnDifferentLines(0,_grid->GetWidth()-2);
		parseGroupOnDifferentLines(_grid->GetWidth()-2,_grid->GetWidth());
	}
	else
	{
		parseGroupOnDifferentLines(0,_grid->GetWidth());
	}  
}

void TouchDoc::parseGroupOnDifferentLines(int start,int end)
{
	int i,j,k;
	CellElement* startBrace = NULL;
	int startLine = 0;
	bool inBrace = false;

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=max(start,0);i<min(end,_grid->GetWidth());i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if (element->isValid())
					{
						if (!inBrace)
						{
							if (element->getChar() == '(')
							{
								inBrace = true;
								startBrace = element;
								startLine = j;
							}
						}
						else//in brace 
						{
							if (element->getChar() == ')')
							{
								inBrace = false;
								if (startLine != j)
								{
									element->setInvalid("Groups can't span rows");
									startBrace->setInvalid("Groups can't span rows");
								}
							}
						}
					}
				}				
			}
		}
	}
}

void TouchDoc::parseGroupLodgic()
{
	if (isSpliced())
	{  
		parseGroupLodgic(0,_grid->GetWidth()-2);
		parseGroupLodgic(_grid->GetWidth()-2,_grid->GetWidth());
	}
	else
	{
		parseGroupLodgic(0,_grid->GetWidth());
	}  
}

void TouchDoc::parseGroupLodgic(int start,int end)
{
	int i,j,k;
	bool inBrace = false;
	CellElement* startBrace = NULL;

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=max(start,0);i<min(end,_grid->GetWidth());i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if (!inBrace)
					{
						if (element->getChar() == '(')
						{
							inBrace = true;
							startBrace = element;
						}
						else if (element->getChar() == ')')
						{
							element->setInvalid("No matching opening brace");
						}
					}
					else//in brace 
					{
						if (element->getChar() == ')')
						{
							inBrace = false;
						}
						else if (element->getChar() == '(')
						{
							startBrace->setInvalid("No matching closing brace");
							inBrace = true;
							startBrace = element;
						}
					}
				}				
			}
		}
	}
	
	if (inBrace)
		startBrace->setInvalid("No matching closing brace");
}


void TouchDoc::parseMainBodyNumbers()
{
	int i,j;

	//check if we can have default calls.
	bool hasDefault;	 
	if (isSpliced())
	{
		hasDefault = true;
		for (i=0;i<_notations.GetSize();i++)
		{
			Notation * not = _notations.GetAt(i);
			if (not)
			{
				if (!not->hasStandardCall())
					hasDefault = false;
			}
		}
	}
	else
	{
		Notation * not = getActiveNotation();
		hasDefault  = ((not)&&(not->hasStandardCall()));
	}											   

	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{
		for ( i=0;i<_grid->GetWidth();i++)
		{	
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				cellData->parseNumbers(isSpliced() && i>=_grid->GetWidth()-2, 
										hasDefault);
			}
		}
	}

	//parse the definitions 
	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		CellData* cellData = definition->getCellData();
		cellData->parseNumbers((definition->getType() == dt_splice),
			hasDefault);
	}
}


void TouchDoc::parseSpliceCountDifferentInEachMethod()
{
	//can now cope with this -  make into warning


/*	if (!isSpliced()) return;
	if (_notations.GetSize() <= 1) return;

	int i,j,k,l;
	Notation* not= NULL;

	//first check the methods
	not = _notations.GetAt(0);
	int numberOfSplicesInFirstMethod = not->getSplicePositionCount();
	bool problem = false;

	if (numberOfSplicesInFirstMethod == 0)
		problem = true;
	else
	{
		for (i=1;i<_notations.GetSize();i++)
		{
			not = _notations.GetAt(i);
			if (not->getSplicePositionCount() != numberOfSplicesInFirstMethod)
			{
				problem = true;
				break;
			}
		}
	}

	if (!problem)
		return; //no more to do.

	numberOfSplicesInFirstMethod = -1;
	CString initialNotation;

	//do the parse
	if (_grid->GetWidth() > 2)
	{
		for ( j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
		{
			CellData* cellData = _grid->GetAt(_grid->GetWidth()-2, j);
			if (cellData)
			{
				for (k=0;k<cellData->getWordCount();k++)
				{
					CellElement* element = cellData->getFirstElementInWord(k);
					if ((element->getType() == PT_SPLICE)||
						(element->getType() == PT_SPLICE_NUM))
					{
						//get the method
						for (l=0;l<_notations.GetSize();l++)
						{
							not = _notations.GetAt(l);
							if ((not->getSpliceLetter() == cellData->getWordFromWordIndex(k))||
								(not->getName(true) == cellData->getWordFromWordIndex(k))||
								(not->getName(false) == cellData->getWordFromWordIndex(k)))
							{
								if ( numberOfSplicesInFirstMethod == -1)
								{
									numberOfSplicesInFirstMethod = not->getSplicePositionCount();
									initialNotation = not->getName();
								}
																
								if (numberOfSplicesInFirstMethod != not->getSplicePositionCount())
								{
									CString str;
									str.Format("%s has a different numbber of splicing positions to %s",
										 not->getName(), initialNotation);
									cellData->setInvalidWord(k, str);
										
								}
								if (not->getSplicePositionCount() == 0)
								{
									CString str;
									str.Format("%s has no splicing positions defined",
										 not->getName());
									cellData->setInvalidWord(k, str);
								}
							}
						}
					}
				}
			}
		}
	}*/
}

void TouchDoc::parseSplices()
{		
	if (!isSpliced())
		return;

	int i,j;

	//SPLICES
	ParseDataArray* pParse = new ParseDataArray();

	
	pParse->Add(new ParseData(" ", PT_WHITESPACE));

	//splices
	for (i=0;i<_notations.GetSize();i++)
	{
		CString spliceLetter = _notations.GetAt(i)->getSpliceLetter();
		if (!spliceLetter.IsEmpty())
			pParse->Add(new ParseData(spliceLetter, PT_SPLICE));		
		pParse->Add(new ParseData(_notations.GetAt(i)->getName(true), PT_SPLICE));		
		pParse->Add(new ParseData(_notations.GetAt(i)->getName(false), PT_SPLICE));		
	}

	//parse the definitions before adding all the ParseData
	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		if (definition->getType() == dt_splice)
		{
			CellData* cellData = definition->getCellData();
			cellData->parseCell(pParse);
		}
	}



	pParse->Add(new ParseData("[", PT_VARIANCE));
	pParse->Add(new ParseData("]", PT_VARIANCE));

	pParse->Add(new ParseData("(", PT_GROUP));
	pParse->Add(new ParseData(")", PT_GROUP));

	//definitions
	for (i=0;i<_definitions.GetSize();i++)
	{
		Definition* def = _definitions.GetAt(i);
		pParse->Add(new ParseData(def->getName(), PT_DEFINITION));
	}

	//do the parse
	if (_grid->GetWidth() > 2)
	{
		for ( j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
		{
			CellData* cellData = _grid->GetAt(_grid->GetWidth()-2, j);
			if (cellData)
			{
				cellData->parseCell(pParse);
			}
		}
	}

	for (i=0;i<pParse->GetSize();i++)
		delete pParse->GetAt(i);
	delete pParse;
}

void TouchDoc::parseMultipleCallPosOnFirstLine()
{
	if (!_courseBased) return;
	
	for (int i=0;i<_grid->GetWidth();i++)
	{	
		CellData* cellData = _grid->GetAt(i,0);
		if (cellData)
		{
			int callPosCount = 0;
			for (int k=0;k<cellData->getWordCount();k++)
			{
				CellElement* element = cellData->getFirstElementInWord(k);
				if ((element->getType() == PT_CALL_POS)||
					(element->getType() == PT_CALL_POS_AGREGATE))
				{
					callPosCount ++;
					if (callPosCount > 1)
						cellData->setInvalidWord(k,
						"Only one calling position or agregate calling position per column is permitted");
				}
			}
		}
	}
}
 
void TouchDoc::parseBlockUseWithinBlock()
{		
	//logical parsing.
	//check for calling positions outside the first line
	for (int j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{
		TouchBlock* tb = getTouchBlockOnRow(j);
		if (tb)
		{
			for (int i=0;i<_grid->GetWidth();i++)
			{	
				CellData* cellData = _grid->GetAt(i,j);
				if (cellData)
				{
					for (int k=0;k<cellData->getLength();k++)
					{
						if (cellData->getElement(k)->getType() == PT_BLOCK)
						{
							cellData->getElement(k)->setInvalid(
								"A block cannot be used within another block definition");
						}
					}
				}
			}
		}
	}
}

void TouchDoc::parseSplicedNotBlocks()
{
	if (!isSpliced())
		return;

	int i,j,k;

	for (i=0;i<_grid->GetWidth();i++)
	{
		for (j=0;j<_grid->GetHeight();j++)
		{	
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if ((element->getType() == PT_BLOCK)||
						(element->getType() == PT_BLOCK_NUM))
					{
						element->setInvalid("Blocks can't be used when splicing");
					}
				}
			}
		}
	}
}

void TouchDoc::parseSplicedCallsNotDefinedInEachMethod()
{
	if (!isSpliced()) return;
	if (_notations.GetSize() <= 1) return;
	
	int i,j,k,l;
	Notation* not= NULL;
	//create a list of strings of common calls 
	//first create a list of calls that are available to first method
	CStringArray _commonCalls;
	not = _notations.GetAt(0);

	for (i=0;i<not->getCallCount();i++)
	{
		_commonCalls.Add(not->getCall(i)->_nameShorthand);
		_commonCalls.Add(not->getCall(i)->_nameLonghand);
	}

	//go through each notation in turn, and remove crom common calls 
	//all those calls that are not defined in each method.
	// We end up with a list of calls that are truely common
	for (i=1;i<_notations.GetSize();i++)
	{
		not = _notations.GetAt(i);
																		
		for (j=0;j<_commonCalls.GetSize();j++)
		{
			CString str = _commonCalls.GetAt(j);
			bool onList = false;
			for (k=0;k<not->getCallCount();k++)
			{
				if ((not->getCall(k)->_nameShorthand == str)||
					(not->getCall(k)->_nameLonghand == str))
				{
					onList = true;
					break;
				}
			}
			if (!onList)
			{
				_commonCalls.RemoveAt(j);
				j--;
			}
		}	 
	}
	
	//we now have a list of common calls.
	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{
		for (i=0;i<_grid->GetWidth();i++)
		{	
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					if (cellData->getElement(k)->getType() == PT_CALL)
					{
						bool onList = false;
						for (l=0;l<_commonCalls.GetSize();l++)
						{
							 if (_commonCalls.GetAt(l) == 
								cellData->getWordFromElementIndex(k, true))
							{
								onList = true;
								break;
							}
						}

						if (!onList)
						{
							CString msg;
							msg.Format("The call \'%s\' is not defined in all methods", cellData->getWordFromElementIndex(k));

							cellData->getElement(k)->setInvalid(msg);
						} 
					}
				}
			}
		}
	}
}
void TouchDoc::parseSplicedCallPosMethodNotDefinedInEachMethod()
{
	if (!isSpliced()) return;
	if (!_courseBased) return;
	if (_notations.GetSize() <= 1) return;
	
	int i,j,k,l;
	Notation* not= NULL;
	//create a list of strings of common calls 
	//first create a list of calls that are available to first method
	CStringArray _commonCalls;
	not = _notations.GetAt(0);

	for (i=0;i<not->getCallPosMethodCount();i++)
		_commonCalls.Add(not->getCallPosMethod(i)->_name);

	for (i=1;i<_notations.GetSize();i++)
	{
		not = _notations.GetAt(i);
		
		for (j=0;j<_commonCalls.GetSize();j++)
		{
			CString str = _commonCalls.GetAt(j);
			bool onList = false;
			for (k=0;k<not->getCallPosMethodCount();k++)
			{
				if (not->getCallPosMethod(k)->_name == str)
				{
					onList = true;
					break;
				}
			}
			if (!onList)
			{
				_commonCalls.RemoveAt(j);
				j--;
			}
		}	 
	}
	
	//we now have a list of common calls.
	for (i=0;i<_grid->GetWidth();i++)
	{	
		CellData* cellData = _grid->GetAt(i,0);
		if (cellData)
		{
			for (k=0;k<cellData->getLength();k++)
			{
				if (cellData->getElement(k)->getType() == PT_CALL_POS)
				{
					bool onList = false;
					for (l=0;l<_commonCalls.GetSize();l++)
					{
						if (_commonCalls.GetAt(l) == 
							cellData->getWordFromElementIndex(k))
						{
							onList = true;
							break;
						}
					}

					if (!onList)
					{
						CString msg;
						msg.Format("The calling position \'%s\' is not defined in all methods", 
										cellData->getWordFromElementIndex(k));

						cellData->getElement(k)->setInvalid(msg);

						//now go down the column and set all calls invalid
						for (int y=1;y<_grid->GetHeight();y++) 
						{
							CellData* vertCell = _grid->GetAt(i,y);
							if (vertCell) 
							{
								for (int z=0;z<vertCell->getLength();z++) 
								{
									vertCell->getElement(z)->setInvalid(msg);
								}
							}
						}
					} 
				}
			}
		}
	}
}

void TouchDoc::parseSplicedCallPosAgregateNotDefinedInEachMethod()
{
	if (!isSpliced()) return;
	if (!_courseBased) return;
	if (_notations.GetSize() <= 1) return;
	
	int i,j,k,l;
	Notation* not= NULL;
	//create a list of strings of common calls 
	//first create a list of calls that are available to first method
	CStringArray _commonCalls;
	not = _notations.GetAt(0);

	for (i=0;i<not->getCallPosAgregateCount();i++)
		_commonCalls.Add(not->getCallPosAgregate(i)->_name);

	for (i=1;i<_notations.GetSize();i++)
	{
		not = _notations.GetAt(i);
		
		for (j=0;j<_commonCalls.GetSize();j++)
		{
			CString str = _commonCalls.GetAt(j);
			bool onList = false;
			for (k=0;k<not->getCallPosAgregateCount();k++)
			{
				if (not->getCallPosAgregate(k)->_name == str)
				{
					onList = true;
					break;
				}
			}
			if (!onList)
			{
				_commonCalls.RemoveAt(j);
				j--;
			}
		}	 
	}
	
	//we now have a list of common calls.
	for (i=0;i<_grid->GetWidth();i++)
	{	
		CellData* cellData = _grid->GetAt(i,0);
		if (cellData)
		{
			for (k=0;k<cellData->getLength();k++)
			{
				if (cellData->getElement(k)->getType() == PT_CALL_POS_AGREGATE)
				{
					bool onList = false;
					for (l=0;l<_commonCalls.GetSize();l++)
					{
						if (_commonCalls.GetAt(l) == 
							cellData->getWordFromElementIndex(k))
						{
							onList = true;
							break;
						}
					}

					if (!onList)
					{
						CString msg;
						msg.Format("The agregate calling position \'%s\' is not defined in all methods", 
										cellData->getWordFromElementIndex(k));

						cellData->getElement(k)->setInvalid(msg);
						//now go down the column and set all calls invalid
						for (int y=1;y<_grid->GetHeight();y++) 
						{
							CellData* vertCell = _grid->GetAt(i,y);
							if (vertCell) 
							{
								for (int z=0;z<vertCell->getLength();z++) 
								{
									vertCell->getElement(z)->setInvalid(msg);
								}
							}
						}		 
					} 
				}
			}
		}
	}
}

void TouchDoc::parseSpaceOnlyInCell()
{
	int i,j, k;

	for ( j=0;j<_grid->GetHeight();j++)
	{
		for ( i=0;i<_grid->GetWidth();i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				CString str = cellData->getString();
				while (str.Remove(' '))
				{}
				if (str.IsEmpty())
				{
					for (k=0;k<cellData->getLength();k++)
					{
						CellElement* element = cellData->getElement(k);
						if (element)
							element->setInvalid("Invalid whitespace");
					}						
				}
			}
		}
	}
}

void TouchDoc::parseCallsInColumnWithoutCallingPos()
{
	if (!_courseBased) return;
	if (_grid->GetHeight() < 2) return;
	int i,j, k;
	CellData* cellData = NULL;

	//set all of data elements to PT_NONE;
	for ( i=0;i<((isSpliced())?_grid->GetWidth()-2:_grid->GetWidth());i++)
	{
		//does the column have a calling position
		cellData = _grid->GetAt(i,0);
		bool hasCallPos = false;
		if (cellData)
		{
			for (k=0;k<cellData->getLength();k++)
			{
				if ((cellData->getElement(k)->getType() == PT_CALL_POS)||
					(cellData->getElement(k)->getType() == PT_CALL_POS_AGREGATE))
				{
					hasCallPos = true;
					break;
				}
			}
		}

		//if we have not got a call pos invalidate
		if (!hasCallPos)
		{			  
			for ( j=1;j<_grid->GetHeight();j++)
			{
				cellData = _grid->GetAt(i,j);
				if (cellData)
				{
					for (k=0;k<cellData->getLength();k++)
					{
						CellElement* element = cellData->getElement(k);
						if (element->getType() != PT_NONE)
							element->setInvalid("There is no calling position or agregate calling position defined.");
					}
				}
			}
		}
	}						  
}


CellData* TouchDoc::getCellData(const GridPos &gridPos)
{
	return _grid->GetAt(gridPos._x,gridPos._y);
}


void TouchDoc::tvEditCharLeft()
{
	_gridPos._c--;
	
	if (_gridPos._c < 0)
	{
		_gridPos._x--;
		if (_gridPos._x < 0) 
		{
			_gridPos._x = _grid->GetWidth() -1 ;
			_gridPos._y--;
			if (_gridPos._y < 0) 
			{
				_gridPos._y = 0;
				_gridPos._x = 0;
				_gridPos._c = 0;
				return;
			}
		}
		_gridPos._c = getCellData(_gridPos )->getLength();
	}
}

void TouchDoc::tvEditCharRight()
{
	_gridPos._c++;

	if (_gridPos._c > getCellData(_gridPos )->getLength())
	{	
		_gridPos._x++;
		if (_gridPos._x > _grid->GetWidth()-1) 
		{
			_gridPos._x -= _grid->GetWidth() ;
			_gridPos._y++;
			if (_gridPos._y > _grid->GetHeight()-1) 
			{
				_gridPos._y = _grid->GetHeight() -1;
				_gridPos._x = _grid->GetWidth() -1;
				_gridPos._c = getCellData(_gridPos )->getLength();
				return;
			}
		}
		_gridPos._c = 0;
	}
}

void TouchDoc::tvEditLineDown()
{
	_gridPos._y++;
	if (_gridPos._y > _grid->GetHeight() -1) 
	{
		_gridPos._y = _grid->GetHeight() -1;
	}

	if(_gridPos._c > getCellData(_gridPos )->getLength())
	{
		_gridPos._c = getCellData(_gridPos )->getLength();
	}
}

void TouchDoc::tvEditLineUp()
{
	_gridPos._y--;
	if (_gridPos._y < 0) 
	{
		_gridPos._y = 0;
	}

	if(_gridPos._c > getCellData(_gridPos )->getLength())
	{
		_gridPos._c = getCellData(_gridPos )->getLength();
	}
}

void TouchDoc::tvEditChar(UINT nChar, CString data)
{
	CellData *pCellData = getCellData(_gridPos);
	if (pCellData)
	{
		if (globalDataManager()->isOverstrike())
			pCellData->onDeleteElement(_gridPos._c);

		if (pCellData->onAddElement(nChar, _gridPos._c, data))
							
		{
			_gridPos._c++;
			if (_gridPos._x == _grid->GetWidth()-1)
				addColumn();
			if (_gridPos._y == _grid->GetHeight()-1)
				addRow();

			if (globalDataManager()->isOverstrike())
				setUpdatePoint("Overtype");
			else
				setUpdatePoint("Typing");
		}
	} 
}

void TouchDoc::tvEditDeleteWord()
{
	CellData * cell = getCellData(_gridPos );
	
	if (_gridPos._c >= cell->getLength())
	{
		return;
	}
	else
	{
		if ((_gridPos._c == 0)||(cell->getChar(_gridPos._c) != ' '))
		{
			//delete the actual word
			while ((_gridPos._c < cell->getLength())&&
				(cell->getChar(_gridPos._c) != ' ')&&
				(cell->onDeleteElement(_gridPos._c)))
			{	 
			}				
		}
		else
		{
			//delete the next white space 
			while ((_gridPos._c < cell->getLength())&&
				   (cell->getChar(_gridPos._c) == ' ')&&
				   (cell->onDeleteElement(_gridPos._c)))
			{	 
			}
		}

		if ((_gridPos._c == 0)||(cell->getChar(_gridPos._c-1) == ' '))
		{
			//delete the next white space 
			while ((_gridPos._c < cell->getLength())&&
				   (cell->getChar(_gridPos._c) == ' ')&&
				   (cell->onDeleteElement(_gridPos._c)))
			{	 
			}
		}
		else
		{
			//delete the actual word
			while ((_gridPos._c < cell->getLength())&&
				(cell->getChar(_gridPos._c) != ' ')&&
				(cell->onDeleteElement(_gridPos._c)))
			{	 
			}				
		}
	}
	
	setUpdatePoint("Delete");
}

void TouchDoc::tvEditDeleteWordBack()
{
	CellData * cell = getCellData(_gridPos );
	
	if (_gridPos._c == 0)
	{
		return;
	}
	else
	{
		if ((_gridPos._c == cell->getLength())||(cell->getChar(_gridPos._c-1) == ' '))
		{
			//delete the spaces
			while ((_gridPos._c > 0)&&
				(cell->getChar(_gridPos._c-1) == ' ')&&
				(cell->onDeleteElement(_gridPos._c-1)))
			{
				_gridPos._c--;
			}				
		}
		else
		{
			//delete the words 
			while ((_gridPos._c > 0)&&
				(cell->getChar(_gridPos._c-1) != ' ')&&
				(cell->onDeleteElement(_gridPos._c-1)))
			{
				_gridPos._c--;
			}				
		}

		if ((_gridPos._c == cell->getLength())||(cell->getChar(_gridPos._c) != ' '))
		{
			//delete the words 
			while ((_gridPos._c > 0)&&
				(cell->getChar(_gridPos._c-1) != ' ')&&
				(cell->onDeleteElement(_gridPos._c-1)))
			{
				_gridPos._c--;
			}				
		}
		else
		{
			//delete the spaces
			while ((_gridPos._c > 0)&&
				(cell->getChar(_gridPos._c-1) == ' ')&&
				(cell->onDeleteElement(_gridPos._c-1)))
			{
				_gridPos._c--;
			}				
		}
	}
	
	setUpdatePoint("Delete");
}

void TouchDoc::tvEditDeleteSelection(const GridPos& start, const GridPos& end, bool update)
{

	for (int i=start._y; i<=end._y;i++)
	{
		for (int j=((i==start._y)?start._x:0);j<=((i==end._y)?end._x:_grid->GetWidth()-1);j++)
		{
			CellData* cell = getCellData(GridPos(j,i));
			if (cell)
				cell->onDeleteElementRange((((i==start._y)&&(j==start._x))?start._c:0 ),
				                           (((i==end._y  )&&(j==end._x  ))?end._c:-1));
		}
	}
	
	if (update)
	{
		_gridPos = start;
	
		setUpdatePoint("Delete Selection", true, true, true, true);
	}
}

void TouchDoc::tidyGrid()
{
	//columns
	for (int i=0;i<_grid->GetWidth()-1;i++)
	{
		if (isColumnEmpty(i))
		{
			deleteColumn(i);
			i--;
		}
	}

	//rows
	for (int j=0;j<_grid->GetHeight()-1;j++)
	{
		if (isRowEmpty(j))
		{
			deleteRow(j);
			for (int k=0;k<_touchBlocks.GetSize();k++)
			{
				TouchBlock* tb = _touchBlocks.GetAt(k);
				if (tb->_start > j) tb->_start--;
				if (tb->_end   > j) tb->_end--;
				if ((tb->_start == tb->_end)|| 
					(tb->_start < ((_courseBased)?1:0))||
					(tb->_end   < ((_courseBased)?2:1)))
				{
					delete tb;
					tb = NULL;
					_touchBlocks.RemoveAt(k);
					k--;
				}
			}
			j--;
		}
	}

	//check that the caret is in bounds
	if (_gridPos._x < 0) _gridPos._x = 0;
	if (_gridPos._y < 0) _gridPos._y = 0;
	if (_gridPos._x >= _grid->GetWidth()) _gridPos._x = _grid->GetWidth()-1;
	if (_gridPos._y >= _grid->GetHeight()) _gridPos._y = _grid->GetHeight()-1;
	//get the grid position.
	CellData* cellData = getCellData(_gridPos);
	if (cellData == NULL)
		_gridPos._y = 0;
	else
		if (_gridPos._c > cellData->getLength())
			_gridPos._c = cellData->getLength();

	if (_gridPos._c < 0)
		_gridPos._c = 0;

}


void TouchDoc::tvEditDelete()
{
	CellData *pCellData = getCellData(_gridPos);

	if ((pCellData)&&(pCellData->onDeleteElement(_gridPos._c)))
	{
		setUpdatePoint("Delete");
	} 
}

void TouchDoc::tvEditWordRight()
{
	CellData * cell = getCellData(_gridPos );

	if (_gridPos._c == cell->getLength())
		tvEditTab();
	else
	{	 
		for (int i=_gridPos._c;i<cell->getLength();i++)
		{
			_gridPos._c++;
			if ((cell->getChar(i) == ' ')&&
				((i==cell->getLength()-1)||(cell->getChar(i+1) != ' ')))
				break;
		}
	}
}

bool TouchDoc::isRowEmpty(int row)
{
	if ((row<0)||(row > _grid->GetHeight()))
		return false;

	for (int i=0;i<_grid->GetWidth();i++)
		if (_grid->GetAt(i,row)->getLength() != 0)
			for (int k=0;k<_grid->GetAt(i,row)->getLength() ;k++)
				if (_grid->GetAt(i,row)->getChar(k) != ' ')
					return false;
	return true;
}

bool TouchDoc::isColumnEmpty(int column)
{
	if ((column<0)||(column > _grid->GetWidth()))
		return false;

	for (int j=0;j<_grid->GetHeight();j++)
	{
		CellData* cellData = _grid->GetAt(column,j);
		if (cellData->getLength() != 0)
			for (int k=0;k<cellData->getLength() ;k++)
				if (cellData->getChar(k) != ' ')
					return false;
	}
	return true;
}

void TouchDoc::tvEditDeleteBack()
{
	CellData *pCellData = getCellData(_gridPos);
	if ((pCellData)&&(pCellData->onDeleteElement(_gridPos._c-1)))
	{
		_gridPos._c--;
		if (_gridPos._c <0)
			_gridPos._c = 0;

		setUpdatePoint("Delete");
	} 
}

void TouchDoc::tvEditTab()
{
	_gridPos._x++;
	_gridPos._c = 0;
	if (_gridPos._x > _grid->GetWidth()-1) 
	{
		_gridPos._x -= _grid->GetWidth() ;
		_gridPos._y++;
		if (_gridPos._y > _grid->GetHeight()-1) 
		{
			_gridPos._y = _grid->GetHeight() -1;
			_gridPos._x = _grid->GetWidth() -1;
		}
	}
}

void TouchDoc::tvEditUntab()
{
	_gridPos._x--;
	_gridPos._c = 0;
	if (_gridPos._x < 0) 
	{
		_gridPos._x += _grid->GetWidth() ;
		_gridPos._y--;
		if (_gridPos._y < 0) 
		{
			_gridPos._y = 0;
			_gridPos._x = 0;
		}
	}
}

void TouchDoc::tvEditWordLeft()
{
	CellData * cell = getCellData(_gridPos );

	if (_gridPos._c == 0)
		tvEditCharLeft();
	else
	{	 
		//first bypass any white space immeadiatly to the left 
		for (int i=_gridPos._c;i>0;i--)
		{
			if (cell->getChar(i-1) != ' ')
				break;
			_gridPos._c--;
		}
		
		//we now in, or imediatly to the right of a word.
		//now find the next white space
		for (int i=_gridPos._c;i>0;i--)
		{
			if (cell->getChar(i-1) == ' ')
				break;
			_gridPos._c--;
		}
	}
}


void TouchDoc::tvEditLineHome()
{
	_gridPos._x = 0;
	_gridPos._c   = 0;
}

void TouchDoc::tvEditLineEnd()
{
	_gridPos._x = _grid->GetWidth() -1;
	_gridPos._c = getCellData(_gridPos )->getLength();
}

void TouchDoc::tvEditTextHome()
{
	_gridPos._c   = 0;
}

void TouchDoc::tvEditTextEnd()
{
	_gridPos._c = getCellData(_gridPos )->getLength();
}

//multi thread version
void TouchDoc::doTouchProve() 
{	
//	TRACE("[TouchDoc::doTouchProve] %s \r\n", getName());

	//bomb out if necessary
	if ((!_autoProve)||
		((getActiveNotation() == NULL)&&(!isSpliced())))
	{
		if (_method)
		{
			Method * method = _method;
			_method = NULL;
			setUpdatePoint("",false, false, false);    
			delete method;
		}		   
		return;
	}	

	//terminate the previous proof.
	proofManager()->requestProofTerminate(_proofRequestID);

	//set up the method ready for proof
	ProofInput* proofInput = setupProofInput();
	proofInput->setPerformAnalysis(); 
	_proofRequestID = proofManager()->requestProofAsynch(proofInput, new Method);
}

ProofInput* TouchDoc::setupProofInput()
{
	ProofInput* proofInput = new ProofInput(_number);	
	proofInput->addNotation(getNotations());		

	Calls calls;
	createCalls(calls);//todo should these all model the notations.
	proofInput->addCall(calls);

	Splices splices;
	createSplices(splices);//todo should these all model the notations.
	proofInput->addSplice(splices);

	MusicRules musicRules;
	createMusicRules(musicRules);
	proofInput->addMusicRule(musicRules);

	proofInput->setRowLimit(_rowCountMax);
	proofInput->setLeadLimit(_leadCountMax);
	proofInput->setPartLimit(_partCountMax);
	proofInput->setCircularCourseLimit(_circularTouchMax);
	proofInput->setStartStroke((_startStroke == hand)?back:hand);	//we invert the stroke so that it is teh first change that we  are talking about rather thanthe last rounds
	proofInput->setStartAtRow(_startAtRow);
	proofInput->setName(getName());
	proofInput->setBlockCount(_blockCount);
	proofInput->setCallFrom(_callFrom);
	proofInput->setStartNotation(_startNotation); 
	proofInput->setPlainLeadLetter(getPlainLeadToken());
	proofInput->setStartRow(_startRow);
	proofInput->setEndRow(_endRow);
	proofInput->setWrapCalls(_wrapCalls == TRUE); 
	
	return proofInput;
}


bool TouchDoc::proofManager_notifyProofComplete(Method* method)
{
	if (method && method->getUniqueId() == _proofRequestID)
	{
		_proofRequestID = -1;

		ASSERT(method != 0);

		if (method == NULL) 
			return false;
		
		//keep the old method intact untill after updates
		Method* oldMethod = _method;
		
		//set the method in the document
		_method	= method;
		
		//tell the blue line about it 
		getBlueLineView()->getMethodWnd().setMethod(method);

		//update
		mainFrame()->fireUpdate();
		UpdateAllViews(NULL);	

		//finally delete the old method
		delete oldMethod;

		return true; 
	}
	
	return false;
}

void TouchDoc::createMusicRules(MusicRules& musicRules)
{
	for (int i=0;i<_musicRules.GetSize();i++)
	{
		musicRules.Add(new MusicRule(*_musicRules.GetAt(i)));
	}
}

void TouchDoc::createSplices(Splices &splices)
{
	if (!isSpliced())
		return;
		
	if (_grid->GetWidth() <= 2) 
		return;
	
//	TRACE("SPLICES \n");

	int groupCount = 0;
	int groupIndex = 0;
	bool inGroup = false;


	//default variance (omit from no parts)
	Variance blankVariance;	
	Variance variance(blankVariance);;

	for (int j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{
		CellData* cellData = _grid->GetAt(_grid->GetWidth()-2, j);
		if (cellData)
		{
			for (int k=0;k<cellData->getWordCount();k++)
			{
				if (cellData->isValidWord(k))
				{
					switch (cellData->getFirstElementInWord(k)->getType())
					{
						case PT_GROUP:
						case PT_GROUP_NUM:
						{
							//change to the actual varianve, or back to the plain variance
							if (cellData->getLastElementInWord(k)->getChar() == '(')
							{
								groupCount = cellData->getNumFromWordIndex(k) - 1; //we want 1-n COPY's
								groupIndex = splices.GetSize();
								inGroup = true;
							}
							else if ((cellData->getLastElementInWord(k)->getChar() == ')')&&
									 (inGroup))
							{
								inGroup = false;
								
								int lastGroupIndex = splices.GetSize();
								for (int n=0;n<groupCount;n++)
								{
									for(int p=groupIndex;p<lastGroupIndex;p++)
									{
										splices.Add(new Splice(*splices.GetAt(p)));
									}
								}
							}
						}
						break;
						case PT_VARIANCE:
						{
							//change to the actual varianve, or back to the plain variance
							if (cellData->getFirstElementInWord(k)->getChar() == ']')
							{
								variance = blankVariance;
							}
							else if (cellData->getFirstElementInWord(k)->getChar() == '[')
							{
								//find the variance
								variance.setGridString(cellData->getFirstElementInWord(k)->getData());
							}
						}
						break;
						case PT_SPLICE:			
						case PT_SPLICE_NUM:			
						{
							for (int l=0;l<cellData->getNumFromWordIndex(k);l++)
							{
								CString spliceLetter = cellData->getWordWithoutNumFromWordIndex(k);
								CString name;
								for (int x=0;x<_notations.GetSize();x++)
								{
									if (_notations.GetAt(x)->isSpliceLetter(spliceLetter))
									{
										name = _notations.GetAt(x)->getName();
										spliceLetter = _notations.GetAt(x)->getSpliceLetter();	//set the splice letter as it may be a longhand
										break;
									}
								}

								if (name.IsEmpty())
									name = spliceLetter;

								splices.Add(new Splice(variance,
														j,						//composition row			
													spliceLetter, //this is the splice
													name));
							}
						}
						break;
						case PT_DEFINITION:
						case PT_DEFINITION_NUM:
						{
							for (int l=0;l<cellData->getNumFromWordIndex(k);l++)
							{
								//find the definition
								for (int i=0;i<_definitions.GetSize();i++)
								{
									if (_definitions.GetAt(i)->getName() == cellData->getWordWithoutNumFromWordIndex(k))
									{
										//go through the splices in the definition
										CellData* defCell = _definitions.GetAt(i)->getCellData();
										for (int m=0;m<defCell->getWordCount();m++)
										{
											if ((defCell->isValid(m))&&
												((defCell->getFirstElementInWord(m)->getType() == PT_SPLICE)||
												 (defCell->getFirstElementInWord(m)->getType() == PT_SPLICE_NUM)))
											{
												for (int n=0;n<defCell->getNumFromWordIndex(m);n++)
												{
													CString spliceLetter = defCell->getWordWithoutNumFromWordIndex(m);
													CString name;
													for (int x=0;x<_notations.GetSize();x++)
													{
														if (_notations.GetAt(x)->isSpliceLetter(spliceLetter))
														{
															name = _notations.GetAt(x)->getName();
															spliceLetter = _notations.GetAt(x)->getSpliceLetter();//set the splice letter as it may be a longhand
															break;
														}
													}

													if (name.IsEmpty())
														name = spliceLetter;

													splices.Add(new Splice(variance,
																			j,						//composition row			
																		spliceLetter,//this is the splice
																		name));
												}
											}
										}
									}
								}
							}
						}
						break;
						default:
						{
							break;
						}
					}				
				}
			}			
		}
	}


//   TRACE("\n");
}

const CString BLOCK_TOKEN = "BLOCK_TOKEN";

void TouchDoc::createCalls(Calls& calls)
{
	//set up the calls 
	CellData * curCell;
	CellData * posCell;

	int groupCount = 0;
	int groupIndex = 0;
	bool inGroup = false;

	//default variance (omit from no parts)
	Variance blankVariance;	
	Variance variance(blankVariance);

	for (int j=(_courseBased)?1:0;j<_grid->GetHeight();j++)
	{
		for (int i=0;i<_grid->GetWidth();i++)
		{
			posCell = getCellData(GridPos(i,0));
			curCell = getCellData(GridPos(i,j));

			//what is the first (and only) valid position
			bool validPosition = false;
			int m=0;
			for (m=0;m<posCell->getWordCount();m++)
			{
				if ((posCell->isValidWord(m))&&
					(posCell->getFirstElementInWord(m)->getType() == PT_CALL_POS)||
					(posCell->getFirstElementInWord(m)->getType() == PT_CALL_POS_AGREGATE))
				{
					validPosition = true;
					break;
				}									
			}

			if ((validPosition)||(!_courseBased))
			{
				for (int k=0;k<curCell->getWordCount();k++)
				{
					if (curCell->isValidWord(k))
					{
						switch (curCell->getFirstElementInWord(k)->getType())
						{
						case PT_GROUP:
						case PT_GROUP_NUM:
						{
							//change to the actual varianve, or back to the plain variance
							if (curCell->getLastElementInWord(k)->getChar() == '(')
							{
								groupCount = curCell->getNumFromWordIndex(k) - 1; //we want 1-n COPY's
								groupIndex = calls.GetSize();
								inGroup = true;
							}
							else if ((curCell->getLastElementInWord(k)->getChar() == ')')&&
									 (inGroup))
							{
								inGroup = false;
								
								int lastGroupIndex = calls.GetSize();
								for (int n=0;n<groupCount;n++)
								{
									for(int p=groupIndex;p<lastGroupIndex;p++)
									{
										calls.Add(new Call(*calls.GetAt(p)));
									}
								}
							}
						}
						break;
						case PT_VARIANCE:
							{
								//change to the actual varianve, or back to the plain variance
								if (curCell->getFirstElementInWord(k)->getChar() == ']')
								{
									variance = blankVariance;
								}
								else if (curCell->getFirstElementInWord(k)->getChar() == '[')
								{
									variance.setGridString(curCell->getFirstElementInWord(k)->getData());
								}
							}
							break;
						case PT_CALL:
						case PT_CALL_NUM:
						case PT_CALL_DEFAULT_NUM:
							{
								for (int l=0;l<curCell->getNumFromWordIndex(k);l++)
								{
									calls.Add(
										new Call((_courseBased)?posCell->getFirstElementInWord(m)->getType():PT_NONE, //parse type of the position
												variance,
												j,						//composition row			
												curCell->getWordWithoutNumFromWordIndex(k),	//this is the call
												(_courseBased)?posCell->getWordFromWordIndex(m):""));	//this is the position
								}
							}
							break;
						case PT_PLAIN_LEAD:
						case PT_PLAIN_LEAD_NUM:
							{
								for (int l=0;l<curCell->getNumFromWordIndex(k);l++)
								{
									calls.Add(
										new Call(PT_NONE, //parse type of the position
												variance,
												j,						//composition row			
												"",	//this is the call
												""));	//this is the position
								}
							}
						break;
						case PT_DEFINITION:
						case PT_DEFINITION_NUM:
						{
							//this is for multiple definitions
							for (int l=0;l<curCell->getNumFromWordIndex(k);l++)
							{
								//find the definition
								for (int i=0;i<_definitions.GetSize();i++)
								{
									if (_definitions.GetAt(i)->getName() == curCell->getWordWithoutNumFromWordIndex(k))
									{
										//go through the calls in the definition
										CellData* defCell = _definitions.GetAt(i)->getCellData();
										for (int p=0;p<defCell->getWordCount();p++)
										{
											if ((defCell->isValid(p))&&
												((defCell->getFirstElementInWord(p)->getType() == PT_CALL)||
												 (defCell->getFirstElementInWord(p)->getType() == PT_CALL_NUM)||
												 (defCell->getFirstElementInWord(p)->getType() == PT_CALL_DEFAULT_NUM)||
												 (defCell->getFirstElementInWord(p)->getType() == PT_PLAIN_LEAD)||
												 (defCell->getFirstElementInWord(p)->getType() == PT_PLAIN_LEAD_NUM)))
											{
												for (int n=0;n<defCell->getNumFromWordIndex(p);n++)
												{
													calls.Add(new Call((_courseBased)?posCell->getFirstElementInWord(m)->getType():PT_NONE, //parse type of the position
																		variance,
																		j,						//composition row			
																		defCell->getWordWithoutNumFromWordIndex(p),
																		(_courseBased)?posCell->getWordFromWordIndex(m):""));//this is the position
												}
											}
										}
									}
								}
							}
						}
						break;
						case PT_BLOCK:			
						case PT_BLOCK_NUM:		
						{
							for (int l=0;l<curCell->getNumFromWordIndex(k);l++)
							{
								//add a temp place holder										
								calls.Add(new Call(PT_NONE,
												variance,
												j,						//composition row			
												curCell->getWordWithoutNumFromWordIndex(k),	//this is the call
												BLOCK_TOKEN));	//this is the position
							}
						}
						break;
						case PT_WHITESPACE:
						case PT_NONE:
						case PT_SPLICE:			
						case PT_SPLICE_NUM:		
						default:
							break;
						}
					}
				}	
			}
		}
	}
	
   
   if (calls.GetSize() >0)
	{
		//create an array of composition rows against a copy of the calls;
		CArray <Calls*, Calls*> compositionRow;
		for(int i=0;i<calls.GetAt(calls.GetUpperBound())->getCompositionRow();i++)
		{
			Calls* rowCalls = new Calls();
			compositionRow.Add(rowCalls);

			for(int j=0;j<calls.GetSize();j++)
			{
				Call* call = calls.GetAt(j);
				if (call->getCompositionRow() == i)
				{
					if (call->getPosition() != BLOCK_TOKEN)
						rowCalls->Add(new Call(*call));
				}
			}
		}

		//now add the blocks in backwards, to maintain indes value;
		//go through all the 
		for (int i=calls.GetUpperBound();i>=0;i--)
		{
			//get the call
			Call* blockCall = calls.GetAt(i);
			//is the call a block token
			if (blockCall->getPosition() == BLOCK_TOKEN)
			{
				//go through all teh defined blocks
				for (int k=0;k<_touchBlocks.GetSize();k++)
				{
					//find the block 
					TouchBlock* touchBlock = _touchBlocks.GetAt(k);
					if (touchBlock->_name == blockCall->getCall())
					{
						//remove the placeholder
						calls.RemoveAt(i);

						for (int l=touchBlock->_start;l<touchBlock->_end;l++)
						{
							if (l<compositionRow.GetSize())
							{
								Calls* rowCalls = compositionRow.GetAt(l);
								//add the calls
								for (int m=0;m<rowCalls->GetSize();m++)
								{
									Call* insertCall = new Call(*rowCalls->GetAt(m));
									//take on the attributes of the placeholder
									insertCall->setComopositionRow(blockCall->getCompositionRow());
									if (!blockCall->getVariance().isBenign()) 
										insertCall->setVariance(blockCall->getVariance());
									calls.InsertAt(i, insertCall);
									i++;
										
								}
							}
						}

						delete blockCall;
						blockCall = NULL;
						//dont need to go through any more defined blocks. Thus:
						break;
					}
				}
			}
		}

		
		//lets tidy up again
		for(int i=0;i<compositionRow.GetSize();i++)
		{
			Calls* rowCalls = compositionRow.GetAt(i);
			for (int j=0;j<rowCalls->GetSize();j++)
			{
				delete rowCalls->GetAt(j);
			}
			delete rowCalls;
		}
	}

	//dump out the calls
#ifdef _DEBUG
//	TRACE("CALLS \n");

	for (int i=0;i<calls.GetSize();i++)
	{
		//calls.GetAt(i)->Dump();
	}
//   TRACE("\n");
#endif

}


void TouchDoc::OnTouchProve() 
{
	CString msg;
	if (_autoProve == TRUE)
	{
		_autoProve = FALSE;	
		msg = "Turn off Proof";
	}
	else 
	{
		_autoProve = TRUE;	
		msg = "Turn on Proof";
	}

	setUpdatePoint(msg);
}

void TouchDoc::OnUpdateTouchProve(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((_autoProve == TRUE)?1:0);	
}

void TouchDoc::setPartCount(CString newPart)
{
	int iNewPart = atoi(newPart);

	if (iNewPart < 1)
		iNewPart = -1; //no lead limit

	if (iNewPart > 10000)
	{
		AfxMessageBox("Please choose a integer less that 10,000");
		return;
	}
	
	if (iNewPart != _partCountMax)
	{
		_partCountMax = iNewPart;
		setUpdatePoint("Set Part Limit");
	}
}

void TouchDoc::setLeadCount(CString newLead)
{
	int iNewLead = atoi(newLead);

	if (iNewLead < 1)
		iNewLead = -1; //no lead limit

	if (iNewLead > 10000)
	{
		AfxMessageBox("Please choose a integer less that 10,000");
		return;
	}
	
	if (iNewLead != _leadCountMax)
	{
		_leadCountMax = iNewLead;
		setUpdatePoint("Set Lead Limit");
	}
}


void TouchDoc::setRowCount(CString newRow)
{
	int iNewRow = atoi(newRow);

	if (iNewRow < 1)
	{
		AfxMessageBox("Please choose a integer greater that 0");
		return;
	}

	if (iNewRow > 1000000)
	{
		AfxMessageBox("Please choose a integer less that 1,000,000");
		return;
	}
	
	if (iNewRow != _rowCountMax)
	{
		_rowCountMax = iNewRow;
		setUpdatePoint("Set Row Limit");
	}
}

void TouchDoc::setCircularTouchCount(CString newCircle)
{
	int iNewCircle= atoi(newCircle);

	if (iNewCircle < 1)
		iNewCircle = -1;

	if (iNewCircle > 100000)
	{
		AfxMessageBox("Please choose a integer less that 100,000");
		return;
	}
	
	if (iNewCircle != _circularTouchMax)
	{
		_circularTouchMax = iNewCircle;
		setUpdatePoint("Set Circular Touch Limit");
	}
	
}

//set as zero based
void TouchDoc::setStartAtRow(int newRow)
{
	if ((newRow < 0) || (newRow >= getFirstLeadRowCount()))
	{
		setUpdatePoint("", false, false, false, false);
		return;
	}

	if (newRow != _startAtRow)
	{
		_startAtRow = newRow;  
		setUpdatePoint("Set Start At Row");
	}
}

int TouchDoc::getFirstLeadRowCount()   //todo this is an expensive call and gets called a lot. can we makeit better?
{
	if (_notations.GetSize() <1)
	{
		return 10000;
	}

	Notation* notToCopy = NULL;
	if (isSpliced())
	{
		//find the first notation in the touch
		//cant be bothered to do this properly. Thus piggy back on the create Splice code
		int methodIndex = 0;
		
		Splices splices;
		createSplices(splices);
		
		if (splices.GetSize() > 0)
		{
			Splice* splice = splices.GetAt(0); 
			CString spliceLetter = splice->_spliceShorthand;
			for (int i=0;i<_notations.GetSize();i++) 
			{
				if (_notations.GetAt(i)->isSpliceLetter(spliceLetter))
				{
					methodIndex = i;
				}
			}	 
		}	
		
			//delete the splices
		for (int i=0;i<splices.GetSize();i++)
			delete splices.GetAt(i);


		notToCopy = _notations.GetAt(methodIndex);
	}
	else
	    notToCopy = getActiveNotation();
	
	if (!notToCopy)
		return 10000;

	Notation notCopy(*notToCopy);

	//remove all the call positions from the copy
	for (int i=0;i<notCopy.getCallPosMethodCount();i++)
	{
		notCopy.deleteCallPosMethod(0);
	}

	ProofInput proofInput(_number);
	proofInput.addNotation(notCopy);
	proofInput.setLeadLimit(1);

	MethodSelectRow met;
	proofManager()->requestProof(proofInput, met);
	
	if (met.getLeadCount() >0)
	{
		int val = met.getLead(0).GetSize();
		return val;
	}

	return 10000;

}

void TouchDoc::onVisualSetStartAtRow()
{
	if (_startAtRow <0)
		_startAtRow = 0;
	
	if (_notations.GetSize() <1)
	{
		AfxMessageBox("First add a method to the touch");
		return;
	}

	Notation* notToCopy = NULL;
	if (isSpliced())
	{
		//find the first notation in the touch
		//cant be bothered to do this properly. Thus piggy back on the create Splice code
		int methodIndex = 0;
		
		Splices splices;
		createSplices(splices);
		
		if (splices.GetSize() > 0)
		{
			Splice* splice = splices.GetAt(0); 
			CString spliceLetter = splice->_spliceShorthand;
			for (int i=0;i<_notations.GetSize();i++) 
			{
				if (_notations.GetAt(i)->isSpliceLetter(spliceLetter))
				{
					methodIndex = i;
				}
			}	 
		}	
		
			//delete the splices
		for (int i=0;i<splices.GetSize();i++)
			delete splices.GetAt(i);


		notToCopy = _notations.GetAt(methodIndex);
	}
	else
	    notToCopy = getActiveNotation();
	
	Notation notCopy(*notToCopy);

	//remove all the call positions from the copy
	for (int i=0;i<notCopy.getCallPosMethodCount();i++)
	{
		notCopy.deleteCallPosMethod(0);
	}

	ProofInput proofInput (_number);
	proofInput.addNotation(notCopy);
	proofInput.setLeadLimit(1);

	MethodSelectRow* met = new MethodSelectRow;
	proofManager()->requestProof(proofInput, *met);

	met->_selectedPosition.setRow(met, 0, _startAtRow);

	MethodDlg dlg(NULL, met, "TouchDoc");

	dlg.DoModal();	

	if (met->_selectedPosition.isValid())
	{
		if (_startAtRow != met->_selectedPosition.getLeadRow() )
		{
			_startAtRow = met->_selectedPosition.getLeadRow() ;
			setUpdatePoint("Set Start At Row");
		}
	}	
}


void TouchDoc::setStartWithChange(CString newChange)
{
	Row row(_number);
	
	CString msg;

	newChange.MakeLower();

	if ((newChange.Find("rou") != -1)||	 //are we attempting to write rounds - lets be a little flexible here
		(row.setAsText(newChange, msg))) 
	{
		if (_startRow != row.getAsText())
		{
			_startRow = row.getAsText();
			setUpdatePoint("Set Start With Change");
		}
	}
	else
	{
		AfxMessageBox("Not setting start with change because:  \r\n" + msg);
		setUpdatePoint("", false, false, false, false);
	}
}								 

void TouchDoc::setEndWithChange(CString newChange)
{
	Row row(_number);
	
	CString msg;

	newChange.MakeLower();

	if ((newChange.Find("rou") != -1)||	 //are we attempting to write rounds - lets be a little flexible here
		(row.setAsText(newChange, msg))) 
	{
		if (_endRow != row.getAsText())
		{
			_endRow = row.getAsText();
			setUpdatePoint("Set End With Change");
		}
	}
	else
	{
		AfxMessageBox("Not setting end with change because:  \r\n" + msg);
		setUpdatePoint("", false, false, false, false);
	}
}								 
const CString& TouchDoc::getActiveNotationString() const
{
	return _activeNotation;
}

bool TouchDoc::isSpliced()
{
	return (_activeNotation == Spliced);
}

void TouchBlock::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _start;
		ar << _end;
		ar << _name;
	}
	else
	{
		ar >> version;
		ar >> _start;
		ar >> _end;
		ar >> _name;
	}
}

void TouchDoc::addTouchBlock(TouchBlock *tb)
{
	_touchBlocks.Add(tb);
	setUpdatePoint("Add Touch Block");
}

void TouchDoc::negotiateBlockMove(TouchBlock *touchBlock, int val, bool start)
{
	if (touchBlock == NULL)
		return;

	if (start)
	{
		if (val >= touchBlock->_end)
			return;

		if (val < ((_courseBased)?1:0))
			return;

		for (int i=0;i<_touchBlocks.GetSize();i++)
		{
			TouchBlock* tb = _touchBlocks.GetAt(i);
			if (tb != touchBlock)
			{
				//check for crssing another block
				if ((touchBlock->_start >= tb->_end)&&
					(val < tb->_end))
					return;
			}
		}
		touchBlock->_start = val;
	}
	else
	{
		if (val <= touchBlock->_start)
			return;

		if (val >= _grid->GetHeight())
			return;
		for (int i=0;i<_touchBlocks.GetSize();i++)
		{
			TouchBlock* tb = _touchBlocks.GetAt(i);
			if (tb != touchBlock)
			{
				//check for crssing another block
				if ((touchBlock->_end <= tb->_start)&&
					(val > tb->_start))
					return;
			}
		}
		touchBlock->_end = val;
	}
}

TouchBlock* TouchDoc::getTouchBlockOnRow(int row)
{
	//check for block definition
	for (int i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		if ((tb->_start <= row)&&
			(tb->_end   > row))
			return tb;		
	}

	return NULL;
}

bool TouchDoc::isTouchBlockOnNode(int row)
{
	//check for block definition
	for (int i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		if ((tb->_start == row)||
			(tb->_end == row))
			return true;		
	}

	return false;
}


void TouchDoc::tvInsertColumn()
{
	addColumn(_gridPos._x);	
	_gridPos._c = 0;
	setUpdatePoint("Insert Column", true, true, false, false);
}

void TouchDoc::tvInsertRow()
{
	addRow(_gridPos._y);	
	_gridPos._c = 0;
	for (int i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		if (tb->_start > _gridPos._y) tb->_start++;
		if (tb->_end   > _gridPos._y) tb->_end++;
	}
	
	checkBlocks();

	setUpdatePoint("Insert Row", true, true, false, false);
}

void TouchDoc::tvDeleteColumn()
{
	deleteColumn(_gridPos._x);	
	_gridPos._c = 0;
	setUpdatePoint("Delete Column");
}

void TouchDoc::tvDeleteRow()
{
	deleteRow(_gridPos._y);
	_gridPos._c = 0;
	for (int i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		if (tb->_start > _gridPos._y) tb->_start--;
		if (tb->_end   > _gridPos._y) tb->_end--;
	}
	
	checkBlocks();
	
	setUpdatePoint("Delete Row");
}


void TouchDoc::OnSetStartStroke(Stroke startStroke)
{
	if(_startStroke != startStroke)
	{
		_startStroke = startStroke;

		setUpdatePoint("Set Stroke");
	}
}

void TouchDoc::setCourseBased(BOOL courseBased)
{
	if (_courseBased != courseBased)
	{   
		_courseBased = courseBased;

		checkBlocks();

		setUpdatePoint("Set Call Type");
	}
}

void TouchDoc::checkBlocks()
{
	for (int i=0;i<_touchBlocks.GetSize();i++)
	{
		TouchBlock* tb = _touchBlocks.GetAt(i);
		if (tb->_start >= tb->_end)			
		{											 
			delete tb;
			tb = NULL;
			_touchBlocks.RemoveAt(i);
			i--;
		}
		else if (tb->_start < ((_courseBased)?1:0))
		{
			if (tb->_end > ((_courseBased)?1:0))
			{
				tb->_start = ((_courseBased)?1:0);
			}
			else
			{
				delete tb;
				tb = NULL;
				_touchBlocks.RemoveAt(i);
				i--;
			}
		}
	}
}


CString TouchDoc::getName()
{
	return _name;
}

void TouchDoc::setName(const CString& name)
{
	if (_name != name)
	{
		_name = name;

		CString path = m_strPathName;
		if (path.IsEmpty())
			SetTitle(_name);

		setUpdatePoint("Change Title");
	}
}

CString TouchDoc::getAuthor()
{
	return _author;
}

void TouchDoc::setAuthor(const CString& author)
{
	if (_author != author)
	{
		_author = author;

		setUpdatePoint("Change Author");
	}
}

void TouchDoc::setPlainLeadLetter(const CString& plainLead)
{
	if (plainLead.IsEmpty())
	{
		return;
	}
	if (_plainLeadLetter != plainLead)
	{
		_plainLeadLetter = plainLead;
		setUpdatePoint("Plain Lead Token");
	}
}


BOOL TouchDoc::getShowCourseHeads()
{
	return _showCourseHeads;
}

bool TouchDoc::isDefinitionUnique(const CString& name)
{
		//check for unique name
	for (int i=0;i<_definitions.GetSize();i++)
	{
		if (name == _definitions.GetAt(i)->getName())
			return false;
	}
	
	return true;
}


TouchView* TouchDoc::getTouchView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	TouchView* pTouchView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pTouchView = dynamic_cast<TouchView*>(pView);
		if (pTouchView)
			return pTouchView; 
	}   
	return NULL;
}

TouchBlueLineView* TouchDoc::getBlueLineView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	TouchBlueLineView* pTouchBlueLineView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pTouchBlueLineView = dynamic_cast<TouchBlueLineView*>(pView);
		if (pTouchBlueLineView)
			return pTouchBlueLineView;
	}   
	return NULL;
}

TouchHoldView* TouchDoc::getTouchHoldView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	TouchHoldView* pTouchHoldView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pTouchHoldView = dynamic_cast<TouchHoldView*>(pView);
		if (pTouchHoldView)
			return pTouchHoldView;
	}   
	return NULL;
}

TouchSimulatorView* TouchDoc::getTouchSimulatorView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	TouchSimulatorView* pTouchSimulatorView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pTouchSimulatorView = dynamic_cast<TouchSimulatorView*>(pView);
		if (pTouchSimulatorView)
			return pTouchSimulatorView;
	}   
	return NULL;
}
bool TouchDoc::isDefinitionUsed(Definition *definition)
{
	for (int j=0;j<_grid->GetHeight();j++)
	{
		for (int i=0;i<_grid->GetWidth();i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (int k=0;k<cellData->getWordCount();k++)
				{
					if ((cellData->getWordWithoutNumFromWordIndex(k) == definition->getName())&&
						((cellData->getFirstElementInWord(k)->getType() == PT_DEFINITION)||
						 (cellData->getFirstElementInWord(k)->getType() == PT_DEFINITION_NUM)))
						return true;
				}
			}
		}
	}
	return false;
}

int TouchDoc::getUsedDefinitionsCount()
{
	int count = 0;
	for (int i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		if (isDefinitionUsed(definition))
			count++;
	}
	return count;
}




BOOL TouchDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	BOOL ret = CDocument::CanCloseFrame(pFrame);

	if (ret == TRUE)
	{
		proofManager()->requestProofTerminate(_proofRequestID);
	}

	return ret;
}

//virtual override - copy of MFC code with alterations
BOOL TouchDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName	 = app()->getDocumentDirectory() + _name;
				
			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				do { e->Delete(); } while (0);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

BOOL TouchDoc::isMultiBlock()
{
	return (_blockCount > 1);
}

int TouchDoc::getBlockCount()
{
	return _blockCount;
}

void TouchDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified());
}

void TouchDoc::OnEditGoto() 
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

//nullable means that the hilight row will be cleared if it is not valid
void TouchDoc::doGoto(const RowPosition& rowPosition, bool nullable, bool changeView, bool quiet)
{
	if (_method)
	{	
		if ((rowPosition.isValid())||(nullable))
		{
			//show the blue line window.
			TouchBlueLineView* pTouchBLView = getBlueLineView();

			if (pTouchBLView)
			{
				//show the window
				if (changeView)
					getTouchHoldView()->SetActiveView(1);

				//hilight the row
				pTouchBLView->getMethodWnd().hilightRow(rowPosition);			
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

void TouchDoc::getUsedVariances(Variances& variances)
{

	int i,j,k,l;

	CStringArray strings;

	//parse the document for variances
	for (j=((_courseBased)?1:0);j<_grid->GetHeight();j++)
	{	
		for ( i=0;i<_grid->GetWidth();i++)
		{
			CellData* cellData = _grid->GetAt(i,j);
			if (cellData)
			{
				for (k=0;k<cellData->getLength();k++)
				{
					CellElement* element = cellData->getElement(k);
					if ((element->getChar() == '[')&&(!element->getData().IsEmpty()))
					{
						//check for repetes						
						bool repete = false;
						for (l=0;l<strings.GetSize();l++)
						{
							if (element->getData() == strings.GetAt(l))
							{
								repete=true;
							}
						}

						if (!repete)							
							strings.Add(element->getData());
					}
				}
			}
		}
	}

	for(i=0;i<strings.GetSize();i++)
	{
		variances.Add(new Variance(strings.GetAt(i)));
	}		

	//sort 
	sortVariances(variances);

}

void TouchDoc::sortVariances(Variances& variances)
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<variances.GetUpperBound();i++)
		{
			if (variances.GetAt(i)->getGridString() == 
				variances.GetAt(i+1)->getGridString())
			{
				changed = true;
				delete variances.GetAt(i);
				variances.RemoveAt(i);
				i--;
			}
			else if (variances.GetAt(i)->getGridString() > 
				variances.GetAt(i+1)->getGridString())
			{
				changed = true;
				Variance* temp    = variances.GetAt(i);
				variances.SetAt(i,variances.GetAt(i+1));
				variances.SetAt(i+1, temp);
			}
		}
	}
}

void TouchDoc::OnUpdateEditGoto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_method != NULL);	
}

void TouchDoc::doPrintAnalysis(CDC* pDC, CPrintInfo* pInfo) 
{
		//this gets called for each page in turn
	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	//apply the appropriate offset;
	double vertSize = page->getLengthInches() - PrintUtils::getFootnoteHeight();
	TRACE("VERT SIZE %f", vertSize);
	page->setOffset(-(vertSize  * (pInfo->m_nCurPage -1)), 0.0);

	
	//leave the bottom clear
	//clear the area that has been printed on;
	page->ExcludeClipRect(page->getLengthInches() - PrintUtils::getFootnoteHeight(), 0.0,
							page->getLengthInches() + 0.1, 
							page->getWidthinches());

	//  ***********the actual printing**********
	double nextRow = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);		

	//if there is more one item to print in this sequence, print an overall title
	CString name = _name;

	if (!_activeNotation.IsEmpty())

	name += (" (" + _activeNotation + ")"); 
	//main title prints the title of the document and 
	CString titleToPrint = "Analysis for " + name;

	nextRow = PrintUtils::printTitle(page, titleToPrint, nextRow, true);
	
	if (_printWhatAnalysis._bStatus)
		nextRow = TouchAnalysisStatus::print(page, nextRow);
	
	if (_printWhatAnalysis._bMusic)
		nextRow = TouchAnalysisMusic::print(page, nextRow);
	
	if (_printWhatAnalysis._bCourse)
		nextRow = TouchAnalysisCourseHead::print(page, nextRow);

	if (_printWhatAnalysis._bLeadEnds)
		nextRow = TouchAnalysisLeadEnds::print(page, nextRow);
	
	if ((_printWhatAnalysis._bSplice)&&(isSpliced()))
		nextRow = TouchAnalysisSplice::print(page, nextRow);
	
	if ((_printWhatAnalysis._bSpliceTransitions)&&(isSpliced()))
		nextRow = TouchAnalysisSpliceTransition::print(page, nextRow);

	if (_printWhatAnalysis._bAllWork)
		nextRow = TouchAnalysisAllTheWork::print(page, nextRow);

	if ((_printWhatAnalysis._bMultiBlock)&&(isMultiBlock()))
		nextRow = TouchAnalysisMultiBlock::print(page, nextRow);

	if (_printWhatAnalysis._bCallsMade)
		nextRow = TouchAnalysisCallsMade::print(page, nextRow);

	//falseness at the end as it can take up a lot of room. 
	//this stratagy leaves the shortedr info at the top, - more readable
	if (_printWhatAnalysis._bFalseness)
		nextRow = TouchAnalysisFalseness::print(page, nextRow);

	//calculate the number of pages
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   
	printData->_numPages = (int)(nextRow/vertSize) + 1;

	//before printing footnote and shareware stuff, remove the page offset / clip 
	page->setOffset(0.0,0.0);
	page->ResetClipRect();

	CString pageText;

	if (printData->_numPages > 1)
		pageText.Format("Page %d of %d",pInfo->m_nCurPage ,printData->_numPages);
		
	PrintUtils::printFootnote(page, pageText, PrintUtils::getFootnoteHeight());

	TRACE("Document has %d pages. Printing Page %d \r\n", printData->_numPages, pInfo->m_nCurPage );
	TRACE("Document total length: %f \r\n", nextRow);

	if (!pInfo->m_bPreview)
		PrintUtils::printShareware(page);	

	delete page;
	
}
BOOL TouchDoc::OnPreparePrinting(CPrintInfo* pInfo, CView* view)
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

void TouchDoc::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnBeginPrinting\r\n");

	PrintData* printData = new PrintData;
	pInfo->m_lpUserData = printData;
}

void TouchDoc::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	//only do stuff if we are printing / preview
	if ((pInfo)&&(pDC->IsPrinting()))
	{
		PrintData* printData = (PrintData*)pInfo->m_lpUserData;

		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= printData->_numPages );
		TRACE("TouchDoc::OnPrepareDC  curPage = %d  numPages = %d pInfo->m_bContinuePrinting = %s\r\n",pInfo->m_nCurPage, printData->_numPages,  pInfo->m_bContinuePrinting?"TRUE":"FALSE");
	}
}	 

void TouchDoc::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnEndPrinting\r\n");

	PrintData* printData = (PrintData*)pInfo->m_lpUserData;
	delete printData;
	
	pInfo->m_lpUserData = NULL;
}

void TouchDoc::OnFilePrintPreview(CView* view) 
{
	_printAnalysis = FALSE;
	BCGPPrintPreview (view);
}

//this is the standard print. - standard print setup
// will be used by the print button on teh 
//  print preview screen, and also external printing
//this should go direct to the print dialog, otherwise the Setup Dlg will be shown twice
void TouchDoc::OnFilePrint() 
{
	_showPrintSetupDlg = TRUE;
	//DO NOT SET _printAnalysis here it is used for both analysis and main print!!!
}

//print from toolbar -  no setup at all
void TouchDoc::OnFilePrintToolbar() 
{
	_showPrintSetupDlg = FALSE;
	_printAnalysis = FALSE;
}

//print frm menu - using setup dialogs
void TouchDoc::OnFilePrintMenu() 
{
	_printAnalysis = FALSE;
	_showPrintSetupDlg = TRUE;
}

int TouchDoc::OnPrintAnalysis() 
{
	_printAnalysis = TRUE; 
	_showPrintSetupDlg = TRUE;

	PrintAnalysisSetupDlg dlg(mainFrame());
	int ID = dlg.DoModal();		
	_printWhatAnalysis.setFromRegistry();

	return ID;
}

void TouchDoc::OnPopupPrintAnalysis() 
{
	_printAnalysis = TRUE; 
	_showPrintSetupDlg = TRUE;
}

void TouchDoc::doEditStartNotation()
{
	MethodEditDlg dlg(NULL, getNumber(), _startNotation.getNotation(),1);

	if ((dlg.DoModal()== IDOK)&&(applyStartNotation(dlg._notation)))
	{
		setUpdatePoint("Set Start Notation");
	}

}

void TouchDoc::setStartNotation(CString startNotation)
{
	//check for an actual change
	if (_startNotation.getNotation() == startNotation)
		return;
	
	//verify that it is valid notation
	if (applyStartNotation(startNotation))
	{
		setUpdatePoint("Set Start Notation");
	}

}

bool TouchDoc::applyStartNotation(CString startNotation)
{
	Notation notation;
	notation.create(TRUE, "", getNumber(), false, startNotation);
	if (_startNotation.getNotation() != notation.getNotation())
	{
		_startNotation = notation;
		return true;			 
	}
	return false;
}

void TouchDoc::setFontMain(FontInfo& fontInfo)
{
	_touchStyle.setFontInfoMain(fontInfo);
	getTouchView()->calculateFontSizes();
	setUpdatePoint("Set Main Font");
}

void TouchDoc::setFontTitle(FontInfo& fontInfo)	  
{
	_touchStyle.setFontInfoTitle(fontInfo);
	getTouchView()->calculateFontSizes();
	setUpdatePoint("Set Title Font");
}

void TouchDoc::setFontAuthor(FontInfo& fontInfo)	  
{
	_touchStyle.setFontInfoAuthor(fontInfo);
	getTouchView()->calculateFontSizes();
	setUpdatePoint("Set Author Font");
}

void TouchDoc::setFontNotes(FontInfo& fontInfo)	  
{
	_touchStyle.setFontInfoNotes(fontInfo);
	getTouchView()->calculateFontSizes();
	setUpdatePoint("Set Notes Font");
}

//put in to stop problems with dialog box while calling from the prop list
void TouchDoc::setStyleColorIndirect(TouchStyleColorType type, COLORREF color)
{							   
	PostMessage(mainFrame()->GetSafeHwnd(), TOUCH_DOC_UPDATE_STYLE_COLOR, (WPARAM)this, (LPARAM)new StyleColorPackage(type, color));
}

void TouchDoc::setStyleColor(TouchStyleColorType type, COLORREF color)
{
	_touchStyle.setColor(type, color);
	setUpdatePoint("Set " + TouchStyle::getColorName(type) + " Color");
}

const FontInfo& TouchDoc::getFontMain()
{
	return _touchStyle.getFontInfoMain();
}

const FontInfo& TouchDoc::getFontTitle()
{
	return _touchStyle.getFontInfoTitle();
}

const FontInfo& TouchDoc::getFontAuthor()
{
	return _touchStyle.getFontInfoAuthor();
}

const FontInfo& TouchDoc::getFontNotes()
{
	return _touchStyle.getFontInfoNotes(); 
}

COLORREF TouchDoc::getStyleColor(TouchStyleColorType type)
{
	return _touchStyle.getColor(type);
}


void TouchDoc::editStyle()
{
	getBlueLineView()->OnToolsStyle();

}

void TouchDoc::OnUpdateToolsTranspose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);		
	//to2do pCmdUI->Enable(_courseBased);	
}

void TouchDoc::OnToolsResettoych() 
{
	//clear the grid
	while (_grid->GetHeight() > 1)
		deleteRow(0);
	_gridPos._c = 0;
	_gridPos._x = 0;
	_gridPos._y = 0;
	tidyGrid();	 	  

	if (_courseBased)
		addDefaultCallPositionsToBlankTouch();	

	setUpdatePoint("Reset");
}

void TouchDoc::OnToolsTidy() 
{
	CWaitCursor wait;
	//gather the information
	TouchObject touchObject;
	touchObject.createTouchObject(this);

	//tidy the touchObject
	touchObject.tidy();

	//write the touch object to the touch grid
	writeTouchObject(touchObject);	 

	//update and prove if necessary
	setUpdatePoint("Tidy");

	//to2do get tidy to cope with all situations.
}


void TouchDoc::OnUpdateToolsTidy(CCmdUI *pCmdUI)
{
	//to2do
	pCmdUI->Enable(FALSE);
}


void TouchDoc::OnToolsTranspose() 
{
	//to2do deal with splicing

	if (!_courseBased)
		return;

	CWaitCursor wait;

	//gett the number to convert to.
	//to2do get number. do not include the curent number, and do not include any hunting bells.
	int convertTo = 3;
	int convertFrom = _callFrom;
	_callFrom = convertTo;


	//gather the information
	TouchObject touchObject;
	touchObject.createTouchObject(this);


	//create map of transpositions
	

	//add new calling positions to method(s) 



	//tidy the touchObject
	touchObject.tidy();

	//write the touch object to the touch grid
	writeTouchObject(touchObject);	 

	//update and prove if necessary
	CString updateStr;
	updateStr.Format("Transpose From %d To %d", convertFrom, convertTo);
	setUpdatePoint(updateStr);
}

void TouchDoc::writeTouchObject(TouchObject& touchObject) 
{
	//to2do also use this for the convert to lead based.

	//clear the grid
	while (_grid->GetHeight() > 1)
		deleteRow(0);
	_gridPos._c = 0;
	_gridPos._x = 0;
	_gridPos._y = 0;
	tidyGrid();	 	  

	//to2do use addDefaultCallPositionsToBlankTouch to add positions

	//create ordered list of positions
	CStringArray positions;
	for (int i=0;i<touchObject._calls.GetSize();i++)
	{
		Call* call = touchObject._calls.GetAt(i);

		//have we had this position before?
		bool positionUsed = false;
		for (int j=0;j<positions.GetSize();j++)
		{
			if (positions.GetAt(j) == call->getPosition())
			{
				positionUsed = true;
				break;
			}	
		}

		if (!positionUsed)
			positions.Add(call->getPosition());
	}
	
	//write the positions to the grid
	addRow();
	for (int i=0;i<positions.GetSize();i++)
	{
		CString position = positions.GetAt(i);
		if (position.GetLength() >0)
		{
			CellData *pCellData = getCellData(_gridPos);
			pCellData->onAddElement(position.GetAt(0), 0);
			_gridPos._x ++;
			_gridPos._c = 0;
			addColumn();
		}
	}

	//write the calls
	_gridPos._x = 0;
	_gridPos._y = 1;
	_gridPos._c = 0;
	addRow();

	for (int i=0;i<touchObject._calls.GetSize();i++)
	{
		Call* call = touchObject._calls.GetAt(i);

		//get the grid cursor in the right place

		for (int count=0;_gridPos._x<=positions.GetSize();_gridPos._x++)
		{
			//safty clause
			if (count > positions.GetSize()*2)
				break;

			//we are going to the next line.
			if (_gridPos._x == positions.GetSize())
			{
				_gridPos._x=0;
				_gridPos._y ++;
				_gridPos._x = 0;
				_gridPos._c = 0;
				addRow();
			}

			if (positions.GetAt(_gridPos._x) == call->getPosition())
			{
				_gridPos._c = 0;
				break;
			}

		}

		CString callName = call->getCall();
		if (callName == DEFAULT_CALL_TOKEN)
			callName.Format("%d", call->getMultiplyer());

		for (int j=0;j<callName.GetLength();j++)
		{
			CellData *pCellData = getCellData(_gridPos);
			pCellData->onAddElement(callName.GetAt(j), _gridPos._c);
			_gridPos._c++;
		}
	}


	//clear up
	tidyGrid();
	_gridPos._c = 0;
	_gridPos._x = 0;
	_gridPos._y = 0;


}
void TouchDoc::OnUpdateToolsConvertoleadbase(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);		
	/*to2do 
	pCmdUI->Enable(_courseBased);	
	pCmdUI->SetText((_courseBased)?"Convert to &Lead Based":"Convert to &Course Based");	
	  */
}

void TouchDoc::OnToolsConvertoleadbaseInit() 
{
/*	to2do if (!_courseBased)
		return;

	CWaitCursor wait;

	if (_autoProve)
	{
		OnToolsConvertoleadbaseFinish(TRUE);
	}
	else
	{

		//force on, and prove
		_autoProve = TRUE;
		doTouchProve(false);

		//wait for the proveEvent to terminate
		while (TouchProveThreadDataIn::_calculatingProof)
		{
			Sleep(5);
			TRACE("Waiting for the proof to finish\r\n");
		}

		Sleep(200);

		//now place a message on the blue line view to finish the conversion.
		//this is necessary because the blue line view is used to pass the 
		//prooved method back to the document. The message passing back the 
		///method has to be processed first.
		
		getBlueLineView()->PostMessage(WM_DO_CONVERT_TO_LEAD_BASED, 0, 0);
		
	}  */

}

void TouchDoc::OnToolsConvertoleadbaseFinish(BOOL /*autoProveState*/) 
{
/* to2do 	CWaitCursor wait;


	//have we got valid data
	if (_method)
	{
		const CallMadeArray& callsMade = _method->getCallsMade();
		if ((callsMade.GetSize() >0 )&&(callsMade.GetAt(0)->GetSize() > 0)) 
		{
			bool writeOnePart = true;

			//convert to strings
			CStringArray callParts;
			for (int i=0;i<callsMade.GetSize();i++) 
			{
				callParts.Add(getCallsMadeString(callsMade.GetAt(i)));
			}

			//are all parts the same?
			for (i=0;i<callParts.GetSize()-1;i++)
			{
				for (int j=i+1;j<callParts.GetSize();j++)
				{
					if (callParts.GetAt(i) != callParts.GetAt(j))
					{
						writeOnePart = false;						
						break;
					}
				}
			}

			//convert to strings
			CStringArray spliceParts;
			if (_method->isSpliced())
			{

				// convert to lead based
				const SplicesMadeArray& splicesMade = _method->getSplicesMade();
				for (int i=0;i<splicesMade.GetSize();i++)
				{	
					CStringArray* spliceLead = splicesMade.GetAt(i);
					spliceParts.Add(getCallsMadeString(spliceLead));
				}				
				
				//are all parts the same?
				for (i=0;i<spliceParts.GetSize()-1;i++)
				{
					for (int j=i+1;j<spliceParts.GetSize();j++)
					{
						if (spliceParts.GetAt(i) != spliceParts.GetAt(j))
						{
							writeOnePart = false;						
							break;
						}
					}
				}	 
			}

			if ((writeOnePart)&&
				(callParts.GetSize() >1))
			{
				CString callPart = callParts.GetAt(0);
				callParts.RemoveAll();
				callParts.Add(callPart);

				//do the same if in spliced.
				if ((_method->isSpliced())&&(spliceParts.GetSize() > 1))
				{
					CString splicePart = spliceParts.GetAt(0);
					spliceParts.RemoveAll();
					spliceParts.Add(splicePart);
				}
			}

			//clear the grid
			GridPos start;
			GridPos end(_grid->GetWidth()-1,_grid->GetHeight()-1,00000);
			tvEditDeleteSelection(start, end, false);

			//write the new data
			_courseBased = FALSE;

			_gridPos = start;

			for (i=0;i<callParts.GetSize();i++)
			{
				//calls
				CellData *pCellData = getCellData(_gridPos);
				if (pCellData)
				{
					for (int j=0;j<callParts.GetAt(i).GetLength();j++)
					{
						if (pCellData->onAddElement(callParts.GetAt(i).GetAt(j), _gridPos._c, ""))
						{
							_gridPos._c++;
							if (_gridPos._x == _grid->GetWidth()-1)
								addColumn();
							if (_gridPos._y == _grid->GetHeight()-1)
								addRow();
						}
					}
				}

				//splices
				if (isSpliced())
				{
					_gridPos._x = 1;
					_gridPos._c = 0;

					pCellData = getCellData(_gridPos);
					if (pCellData)
					{
						for (int j=0;j<spliceParts.GetAt(i).GetLength();j++)
						{
							if (pCellData->onAddElement(spliceParts.GetAt(i).GetAt(j), _gridPos._c, ""))
							{
								_gridPos._c++;
								if (_gridPos._x == _grid->GetWidth()-1)
									addColumn();
								if (_gridPos._y == _grid->GetHeight()-1)
									addRow();
							}
						}
					}
				}

				//move to the next line
				_gridPos._x = 0;
				_gridPos._y++;
				_gridPos._c = 0;
			}

    		
			//update
			setUpdatePoint("Convert to Lead Based.");
			_gridPos._x = 0;
			_gridPos._y = 0;
			_gridPos._c = 0;
		}
		else
		{
			AfxMessageBox("Unable to convert to Lead Based. No valid calls.");
		}				  

	}
	else
	{
		AfxMessageBox("Unable to convert to Lead Based.");
	}

	//put it back how it was
	if (autoProveState == FALSE)
	{
		_autoProve = FALSE;

		//wait for the proveEvent to terminate
		while (TouchProveThreadDataIn::_calculatingProof)
		{
			Sleep(5);
			TRACE("Waiting for the proof to finish\r\n");
		}

		doTouchProve(true);
	}


	//just put the doc back to where it came from 
	setUpdatePoint("", false, false, true, false);*/

}


CString TouchDoc::getCallsMadeString(CStringArray* calls)
{
	if (calls == NULL)
		return "";
	CString ret;
	for (int i=0;i<calls->GetSize();i++)
	{
		ret += calls->GetAt(i);
		ret += "";
	}
	return ret;
}

const Notations& TouchDoc::getNotations()
{
	return _notations;
}

void TouchDoc::setNotes(CString notes)
{
	if (_notes != notes)
	{
		_notes = notes;
		setUpdatePoint("Edit Notes");
	}
}

bool TouchDoc::editNotes(CPoint point)
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

CString TouchDoc::getNotes()
{
	return _notes;
}

void TouchDoc::OnPopupEditnotes() 
{
	CPoint point;
	GetCursorPos(&point);
	if (editNotes(point))
	{
		_showNotes = TRUE;
		setUpdatePoint("", false, false, false, false);
	}
}

void TouchDoc::setShowNotes(BOOL showNotes)
{
	if (showNotes != _showNotes)
	{
		_showNotes = showNotes;								 		
		setUpdatePoint(showNotes?"Show Notes":"Hide Notes");
	}														 
}

void TouchDoc::getMainStatusBarText(CString &pane1, CString &pane2,CString &pane3)
{
	pane1.Format("%d,%d,%d",  _gridPos._x +1,
							_gridPos._y +1 ,
							_gridPos._c +1 );


	Tower* tower = towerManager()->getTower();
	if (tower && _method && !playManager()->isStopped())
		pane2 = GlobalFunctions::getPealTimeText(tower->getNumberInclCover(_method->getNumber()), 
												tower->getSpeed(),
												tower->getOpenHandstrokeGap());
	else
		pane2.Empty();

	pane3 = ((globalDataManager()->isOverstrike())?"OVR":"INS");
}

void TouchDoc::getBlueLineStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5, CString &pane6)
{
	if (_methodPositionBlueLineStatusBar != NULL)
	{																		 
		pane1 = _methodPositionBlueLineStatusBar->format(rpft_lead);
		pane2 = _methodPositionBlueLineStatusBar->format(rpft_rowDisplay);
 		pane3 = _methodPositionBlueLineStatusBar->format(rpft_mp_place);
		pane4 = _methodPositionBlueLineStatusBar->format(rpft_mp_bell);
		pane5 = _methodPositionBlueLineStatusBar->format(rpft_mp_stroke);
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

void TouchDoc::getSimulatorStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5)
{

	if (_methodPositionSimulatorStatusBar != NULL)
	{		
		pane1 = _methodPositionSimulatorStatusBar->format(rpft_lead);
		pane2 = _methodPositionSimulatorStatusBar->format(rpft_rowDisplay);
		pane3 = _methodPositionSimulatorStatusBar->format(rpft_mp_stroke);
	}
	else 
	{
		pane1.Empty();
		pane2.Empty();
		pane3.Empty();
	}	

	CString methodName;
	if (simulatorManager()->getSimulatorStateType() == sst_connectedMethod)
		methodName = _simulatorMethodStatusBar;
	pane4 = methodName;

	Tower* tower = towerManager()->getTower();
	if (tower && _method && !playManager()->isStopped())
		pane5 = GlobalFunctions::getPealTimeText(tower->getNumberInclCover(_method->getNumber()), 
												tower->getSpeed(),
												tower->getOpenHandstrokeGap());
	else
		pane5.Empty();

			
}

void TouchDoc::setMethodPositionBlueLineStatusBar(MethodPosition *methodPositionStatusBar)
{
	delete _methodPositionBlueLineStatusBar ;
	_methodPositionBlueLineStatusBar = methodPositionStatusBar;
}

void TouchDoc::setMethodPositionSimulatorStatusBar(MethodPosition *methodPositionStatusBar, CString simulatorMethodStatusBar)
{
	delete _methodPositionSimulatorStatusBar ;
	_methodPositionSimulatorStatusBar = methodPositionStatusBar;
	
	_simulatorMethodStatusBar = simulatorMethodStatusBar;
}

void TouchDoc::OnFileSaveAs() 
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
	strFilter += "Touch File (*.tch)|*.tch|";
	strFilter += "Word Document File (*.doc)|*.doc|";
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



bool TouchDoc::saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format)
{
	//file name will be a full path.
	TRACE(fileName + "\r\n");

	ASSERT(!fileName.IsEmpty());
	
	if (fileName.IsEmpty())
		return false;
	 
	CxImage* image = NULL;
	
	SaveAsSelectionDlg dlg;
	dlg.DoModal();

	if (dlg.isTouchWindow())
		image = getTouchView()->getImage();
	else
		image = getBlueLineView()->getImage();

	if (image)
	{
		if (format == CXIMAGE_FORMAT_GIF)
			image->DecreaseBpp(8, true);

		image->Save(fileName, format);
	}

	delete image;

	return true;
}

#define MAIN_FONT 0
#define NOTES_FONT 1

bool TouchDoc::saveAsRTF(CString fileName)
{
	/* todo
	//file name will be a full path.
	TRACE(fileName + "\r\n");
	CString data, temp; 
	
	//file name will be a full path.
	FILE *f;
	f=fopen(fileName,"wt");
	if (f == NULL)
	{
		AfxMessageBox("A sharing violation occured while accessing \r\n"+fileName);
		return false;
	}
	
	//header
	data+= "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057";
	
	//font table
	data+= "{\\fonttbl";
	data+= _touchStyle.getFontInfoMain().getRTFFormat(MAIN_FONT);
	data+= _touchStyle.getFontInfoNotes().getRTFFormat(NOTES_FONT); //to2do author and title
	data+= "}";
	
	//Color Table
	data+= "{\\colortbl ";
	for (int i=0;i<tsc_highest_val;i++) 
		data+= GlobalFunctions::ColorToRTF(_touchStyle.getColor((TouchStyleColorType)i));
	data+= "}";
	
	//comment
	data+= "{\\*\\Generated By Ringing Master ;}";
	
	//additional information
	data+= "\\viewkind4\\uc1\\pard";

	//name
	if (!getName().IsEmpty())
	{
		data += GlobalFunctions::TextToRTF(
			MAIN_FONT, (int)(_touchStyle.getFontInfoTitle()._size)/5, _touchStyle.getFontInfoTitle()._color, getName());
	}

	//author
	if (!getAuthor().IsEmpty())
	{
		data += GlobalFunctions::TextToRTF(
			MAIN_FONT, _touchStyle.getFontInfoAuthor()._size/5, _touchStyle.getFontInfoAuthor()._color, "   " + getAuthor());
	}


	//table	
	CString cellFormat = "\\clNoWrap\\clpadl100\\clpadfl3\\clpadr100\\clpadfr3";
		
	// populate header row
	data += "\\par\\trowd\\trautofit1\\intbl"	;
	
	int width = _grid->GetWidth();
	if (_touchBlocks.GetSize() >0)
		width ++;

	for (int i=0;i< width;i++)
	{
		temp.Format("\\cellx%d%s", i+1, cellFormat); 
		data += temp;
	}

	Row* lastRow = NULL;
	Row* thisRow = NULL;
	CString thisStr, lastStr, thisChar;

	
	//write table contents	 
	for (int j=0;j<_grid->GetHeight()-1;j++)
	{
		data += "\\trowd\\trautofit1\\intbl"	 ;

		for (int i=0;i< width;i++)
		{
			temp.Format("\\cellx%d%s", i+1, cellFormat); 
			data += temp;
		}

		data += "\n{";	
		data += cellFormat;
		
		if (_showCourseHeads)
		{

			if (j < _method->getCourseHeads().GetHeight())
				thisRow = _method->getCourseHeads().GetAt(0,j);
			else
				thisRow = NULL;


			if (thisRow)
			{
				if (lastRow)
					lastStr = lastRow->getAsText();
				else 
					lastStr.Empty();

				thisStr = thisRow->getAsTextDiff(lastRow);
				for(int i=0;i<thisStr.GetLength();i++)
				{
					thisChar = thisStr.GetAt(i);
					if (thisChar == ' ')
					{
						if (i < lastStr.GetLength())
						{
							thisChar = lastStr.GetAt(i);
						}
						temp.Format("\\cf%d ",tsc_unchangedLHColor); 
						data += temp;
					}
					else
					{
						temp.Format("\\cf%d ",tsc_changedLHColor); 
						data += temp;
					}
					data += thisChar;
				}
				lastRow = thisRow;
			}
		}

		data += "\\cell" + cellFormat + " "  ;

		for (int i=0;i< _grid->GetWidth()-1;i++)
		{
			for (int k=0;k<getCellData(GridPos(i,j))->getLength();k++)
			{

				CellElement* element = getCellData(GridPos(i,j))->getElement(k);

				temp.Format("\\cf%d ",getTouchStyleColorTypeFromParseType(element->getType())); 
				data += temp;

				data +=element->getChar();

				if (!element->getData().IsEmpty())
				{
					temp.Format("\\fs%d \\super ", (int)(_touchStyle.getFontInfoMain()._size/5 * .55));
					data += temp;

					data += element->getData();

					temp.Format("\\nosupersub \\fs%d", _touchStyle.getFontInfoMain()._size/5);
					data += temp;
				}
			}
			  
			data += "\\cell" + cellFormat + " "  ;
		}

		if (_touchBlocks.GetSize() >0)
		{
			for (int i=0;i<_touchBlocks.GetSize();i++)
			{
				TouchBlock* tb = _touchBlocks.GetAt(i);
				if ((j>=tb->_start)&&(j<tb->_end))
				{
					temp.Format("\\cf%d ",tsc_blockColor);
					data += temp;;
					data+= " | " + tb->_name;
				}

			}
			data += "\\cell" + cellFormat + " "  ;
		}
		
		data += "}\n{\\trowd\\trautofit1\\intbl"	;
		for (int i=0;i< width;i++)
		{
			temp.Format("\\cellx%d%s", i+1, cellFormat); 
			data += temp;
		}

		data += "\\row }\n";   
	}
	

	data += "\\pard\\par\n";

	//definitions
	for (int i=0;i<_definitions.GetSize();i++)
	{
		Definition* definition = _definitions.GetAt(i);
		if (isDefinitionUsed(definition))
		{
			CellData* cell = definition->getCellData();
			//draw the name, and the equals

			temp.Format("\\cf%d ",tsc_definitonColor);
			data += temp;;
			
			data += definition->getName() + " = ";

			//draw the rest 
			for (int j=0;j<cell->getLength();j++)
			{
				CellElement* element = cell->getElement(j);
				temp.Format("\\cf%d ",getTouchStyleColorTypeFromParseType(element->getType()));
				data += temp;
				data += element->getChar();
			}
			data += "\\par\n"; 
		}
	}



	//notes
	if (_showNotes)
	{
		CString notes = getNotes();
		notes.Replace("\r\n","\n");
		notes.Replace("\n\r","\n");
		notes.Replace("\n","\\par");

		if (!notes.IsEmpty())
			data += GlobalFunctions::TextToRTF(
				NOTES_FONT, _touchStyle.getFontInfoNotes()._size/5, tsc_notesColor, "\\par" + notes);
	}
	
	data+= "}";

	fprintf(f,data);

	fclose(f);
				 */
	return true;
}


TouchStyleColorType TouchDoc::getTouchStyleColorTypeFromParseType(ParseType parseType)
{
	switch(parseType)
	{
	case PT_CALL_POS:
	case PT_CALL_POS_AGREGATE:
		return tsc_callPosColor;
	case PT_CALL:
	case PT_CALL_NUM:
	case PT_CALL_DEFAULT_NUM:
		return tsc_callColor;
	case PT_SPLICE:
	case PT_SPLICE_NUM:
		return tsc_methodSplicedColor;		
	case PT_BLOCK:
	case PT_BLOCK_NUM:
		return tsc_blockColor;	
	case PT_DEFINITION:
	case PT_DEFINITION_NUM:
		return tsc_definitonColor;	
	case PT_PLAIN_LEAD:
	case PT_PLAIN_LEAD_NUM:
		return tsc_plainLeadTokenColor;	
	case PT_VARIANCE:
		return tsc_varianceColor;				
	case PT_GROUP:
	case PT_GROUP_NUM:
		return tsc_groupColor;				
	case PT_WHITESPACE:
		return tsc_titleColor;
	case PT_NONE:
		return tsc_highest_val;
	default:
		ASSERT(FALSE);
		return tsc_highest_val;
	}		
}


void TouchDoc::playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage)
{
	if ((!_method) || 
		(!rowPositionChangedPackage._tracking) || 
		(rowPositionChangedPackage._playRequestId != _method->getUniqueId()))
	{
		static RowPosition invalidRow;
		ASSERT(!invalidRow.isValid());
		doGoto(invalidRow, true, false, true);	
	}
	else
		doGoto(rowPositionChangedPackage._rowPosition, true, false, true);	
}



bool TouchDoc::setupPlayMenu(CMenu* pPopup)
{
	switch (getTab())
	{
	case 0: //touch main
	case 1:	//touch blue line
		pPopup->RemoveMenu(ID_PLAY_METHOD, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_PLAY_ROUNDS, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_PLAY_ROUNDS_AUTO, MF_BYCOMMAND);		
		pPopup->RemoveMenu(ID_COME_ROUND_NEXT_TIME, MF_BYCOMMAND);		
		pPopup->RemoveMenu(ID_STAND_UP, MF_BYCOMMAND);		
		pPopup->RemoveMenu(ID_PLAY_METHOD, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_SIMULATOR_RESET, MF_BYCOMMAND);
		break;
	case 2:	//simulator
		pPopup->RemoveMenu(ID_PLAY, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_INC_LEAD, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_DEC_LEAD, MF_BYCOMMAND);
		break;
	}

	mainFrame()->tidyMenu(pPopup);

	return false;
}

bool TouchDoc::setupTouchMenu(CMenu* pPopup)
{
	switch (getTab())
	{
	case 0: //touch main
		//method only stuff
		pPopup->RemoveMenu(ID_METHOD_STYLE_DUMMY, MF_BYCOMMAND);

		//simulator only stuff
		pPopup->RemoveMenu(ID_CREATE_STRIKE_REPORT, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_STRIKING_TEST, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_STRIKING_TEST_RESULTS, MF_BYCOMMAND);

		pPopup->RemoveMenu(ID_CALLS_USE_TOUCH, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLS_PLAIN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLS_RANDOM, MF_BYCOMMAND);

		pPopup->RemoveMenu(ID_CALLPLAIN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLBOB, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLSINGLE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLX, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLY, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLZ, MF_BYCOMMAND);

		break;
	case 1:	//touch blue line
		//touch main only stuff
		pPopup->RemoveMenu(ID_TOUCH_ADDVARIANCE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_ADDBLOCKDEFINITION_MENU, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_INSERT_ROW, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_INSERT_COLUMN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_DELETE_ROW, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_DELETE_COLUMN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_SHOW_GRID, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_SHOW_COURSE_HEADS, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_CONVERTOLEADBASE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_TRANSPOSE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_TIDY, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_RESETTOYCH, MF_BYCOMMAND);

		//simulator only stuff
		pPopup->RemoveMenu(ID_CREATE_STRIKE_REPORT, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_STRIKING_TEST, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_STRIKING_TEST_RESULTS, MF_BYCOMMAND);

		pPopup->RemoveMenu(ID_CALLS_USE_TOUCH, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLS_PLAIN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLS_RANDOM, MF_BYCOMMAND);

		pPopup->RemoveMenu(ID_CALLPLAIN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLBOB, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLSINGLE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLX, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLY, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_CALLZ, MF_BYCOMMAND);

		break;
	case 2:	//simulator
							
		//method only stuff
		pPopup->RemoveMenu(ID_METHOD_STYLE_DUMMY, MF_BYCOMMAND);

		//touch main only stuff
		pPopup->RemoveMenu(ID_TOUCH_ADDVARIANCE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_ADDBLOCKDEFINITION_MENU, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_INSERT_ROW, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_INSERT_COLUMN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_DELETE_ROW, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_DELETE_COLUMN, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_SHOW_GRID, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_SHOW_COURSE_HEADS, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_CONVERTOLEADBASE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_TRANSPOSE, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_TIDY, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOOLS_RESETTOYCH, MF_BYCOMMAND);

		//touchmain and method only
		pPopup->RemoveMenu(ID_TOUCH_ADDDEFINITION, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOUCH_ADDMUSIC, MF_BYCOMMAND);
		pPopup->RemoveMenu(ID_TOUCH_PROVE, MF_BYCOMMAND);		
		
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	mainFrame()->tidyMenu(pPopup);

	return false;
}

int TouchDoc::getCallFrom(void)
{
	return _callFrom;
}

void TouchDoc::setCallFrom(int callFrom)
{
	if ((callFrom <= 0)||//if we get a 0 it is an error
		(callFrom > _number))
	{
		setUpdatePoint("", false, false, false, false);
		return;				
	}

	if (_callFrom != callFrom)
	{
		_callFrom = callFrom;
		
		setUpdatePoint("Set Call From");
	}	
}

BOOL TouchDoc::isCourseBased()
{
	return _courseBased;
}

CString TouchDoc::getPlainLeadToken()
{
	if (_plainLeadLetter.IsEmpty())
		_plainLeadLetter = "p";

	return _plainLeadLetter;
}					

GridPos& TouchDoc::getGridPos()
{
	return _gridPos;
}

GridArray* TouchDoc::getGridArray()
{
	return _grid;
}

TouchStyle& TouchDoc::getTouchStyle()
{
	return _touchStyle;
}

TouchBlockArray& TouchDoc::getTouchBlocks()
{
	return _touchBlocks;
}

BOOL TouchDoc::isShowGrid()
{
	return _showGrid;
}

void TouchDoc::setShowGrid(bool showGrid)
{
	_showGrid = showGrid; //to2do these should be on teh property list.
}

void TouchDoc::setShowCourseHeads(bool showCourseHeads)
{
	_showCourseHeads = showCourseHeads; //to2do these should be on teh property list.
}

BOOL TouchDoc::isShowNotes()
{
	return _showNotes;
}

Definitions& TouchDoc::getDefinitions()
{
	return _definitions;
}

MusicRules& TouchDoc::getMusicRules()
{
	return _musicRules;
}

BOOL TouchDoc::isPrintAnalysis()
{
	return _printAnalysis;
}

Method* TouchDoc::getMethod()
{
	return _method;
}

PrintWhatAnalysis& TouchDoc::getPrintWhatAnalysis()
{
	return _printWhatAnalysis;
}

const CString& TouchDoc::getStartRow()
{
	return _startRow;
}

const CString& TouchDoc::getEndRow()
{
	return _endRow;
}

int TouchDoc::getStartAtRow()
{
	return _startAtRow;
}

Stroke TouchDoc::getStartStroke()
{
	return _startStroke;
}

const Notation& TouchDoc::getStartNotation()
{
	return _startNotation;
}
 
int TouchDoc::getRowCountMax()
{
	return _rowCountMax;
}

int TouchDoc::getLeadCountMax()
{
	return _leadCountMax;
}

int TouchDoc::getPartCountMax()
{
	return _partCountMax;
}

int TouchDoc::getCircularTouchMax()
{
	return _circularTouchMax;
}

TouchPropListExpanded& TouchDoc::getTouchPropListExpanded()
{
	return _touchPropListExpanded;
}

bool TouchDoc::hasUserMusic()
{
	for (int i=0;i<_musicRules.GetSize();i++)
		if (_musicRules.GetAt(i)->getType() == MRT_USER)
			return true;

	return false;
}


void TouchDoc::addDefaultMusic()
{
	//first remove all music 
	ASSERT(_musicRules.GetSize() == 0);
	for (int i=0;i<_musicRules.GetSize();i++)
		delete _musicRules.GetAt(i);
	_musicRules.RemoveAll();

 	//sort out the default music
	MusicRule* mc = new MusicRule(_number);
	mc->setType(MRT_MUSICAL_CHANGES);
	_musicRules.Add(mc);
	if ((_number >= 7)&&(GlobalFunctions::isEven(_number)))
	{
		mc = new MusicRule(_number);
		mc->setType(MRT_CRU);
		_musicRules.Add(mc);
	}	     
	
	sortMusicRules();
}

void TouchDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

	((MainFrame*) AfxGetMainWnd ())->UpdateMRUFilesList ();
}

BOOL TouchDoc::getWrapCalls()
{
	return _wrapCalls;
}

void TouchDoc::setWrapCalls(BOOL wrapCalls)
{
	if (wrapCalls != _wrapCalls)
	{
		_wrapCalls = wrapCalls;								 		
		setUpdatePoint(wrapCalls?"Wrap Calls":"Do Not Wrap Calls");
	}														 
}