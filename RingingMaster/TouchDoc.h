// TouchDoc.h : header file
//

#pragma once

#include "Notation.h"	// Added by ClassView
#include "Undo.h"
#include "TwoDArray.h"
#include "CellData.h"
#include "Layout.h"
#include "ParseData.h"
#include "TouchBlock.h"
#include "Definition.h"
#include "Variance.h"
#include "Method.h"
#include "Call.h"
#include "CPage.h"
#include "printWhatAnalysis.h"	// Added by ClassView
#include "TouchStyle.h"
#include "NotationLite.h"
#include "PlayManagerEventListener.h"
#include "ProofManagerEventListener.h"
#include "TouchPropListExpanded.h"
#include "Splice.h"
#include "MusicRule.h"



class TouchSimulatorView;
class TouchBlueLineView;
class TouchView;
class TouchHoldView;
class Splice;
class GridPos;
class NewDocumentWizardTransfer;
class TouchObject;
class UndoButton;
class ProofInput;
class MethodPosition;
class MusicResult;

enum ENUM_CXIMAGE_FORMATS;



const CString LeadBased		= "Lead Based";
const CString CourseBased	= "Course Based";        
const CString Spliced		= "Spliced";   
const CString SingleBlock	= "Single Block";
const CString MultiBlock	= "Multi Block";
const CString Handstroke	= "Handstroke";
const CString Backstroke	= "Backstroke";
const CString Inactive		= "Inactive";
const CString Active		= "Active";




/////////////////////////////////////////////////////////////////////////////
// TouchDoc document


#define CELL_PAD_TOP    0
#define CELL_PAD_BOTTOM 1
#define CELL_PAD_LEFT   4
#define CELL_PAD_RIGHT  4
#define CELL_MIN_WIDTH  5
#define CARET_WIDTH     2


class GridPos
{
public:	
	GridPos (int x = 0, int y = 0, int c = 0) :
	  _x(x),
	  _y(y),
	  _c(c)
	  {
	  }

	operator CPoint() const 
	{
		return CPoint(_x,_y);
	}

	bool operator == (GridPos& gridPos) const
	{
		return ((_x == gridPos._x)&&
				(_y == gridPos._y)&&
				(_c == gridPos._c));
	}
	void Serialize(CArchive& ar);

	int _x;
	int _y;
	int _c;
};


class StyleColorPackage
{
public:
	StyleColorPackage(TouchStyleColorType type,COLORREF color) :
		_type(type), 
		_color(color)
	{

	}

	TouchStyleColorType _type;
	COLORREF _color;
};


typedef TwoDArray<CellData*> GridArray;


class TouchDoc : public CDocument, public Undo, public PlayManagerEventListener, ProofManagerEventListener
{


protected:
	TouchDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TouchDoc)

// Attributes
public:


// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

	void deserialize_v1(CArchive& ar);//upto version 1.07
	void deserialize_v2(CArchive& ar);//version 1.08 onwards
	void deserialize_v3(CArchive& ar);//version 2.01 onwards
	void deserialize_v4(CArchive& ar);//version 3.01 onwards
	void deserialize_v5(CArchive& ar);//version 3.02 onwards


	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

// Implementation
public:
	ProofInput* setupProofInput();
	void bulkUndo(int undoCount) ;
	void bulkRedo(int redoCount) ;
	void setActiveNotation(const CString& name, bool update = true, bool allowSpliceConversion = false);
	bool proofManager_notifyProofComplete(Method* method);
	void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage);
	TouchStyleColorType getTouchStyleColorTypeFromParseType(ParseType parseType);
	bool saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format);
	bool saveAsRTF(CString fileName);
	void setInitialData(const NewDocumentWizardTransfer& trans);
	void setMethodPositionBlueLineStatusBar(MethodPosition* methodPositionStatusBar);
	void setMethodPositionSimulatorStatusBar(MethodPosition* methodPositionStatusBar, CString simulatorMethodStatusBar);
	void getMainStatusBarText(CString &pane1, CString &pane2, CString &pane3);
	void getBlueLineStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5, CString &pane6);
	void getSimulatorStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5);
	void setShowNotes(BOOL showNotes);
	CString getNotes();
	bool editNotes(CPoint point);
	void setNotes(CString notes);
	const Notations& getNotations();
	void OnToolsConvertoleadbaseFinish(BOOL autoProveState) ;
	CString getCallsMadeString(CStringArray* calls);
	void editStyle();
	void setStyleColorIndirect(TouchStyleColorType type, COLORREF color);
	void setStyleColor(TouchStyleColorType type, COLORREF color);
	COLORREF getStyleColor(TouchStyleColorType type);
	const FontInfo& getFontTitle();
	const FontInfo& getFontAuthor();
	const FontInfo& getFontMain();
	const FontInfo& getFontNotes();
	void setFontMain(FontInfo& fontInfo);
	void setFontTitle(FontInfo& fontInfo);
	void setFontAuthor(FontInfo& fontInfo);
	void setFontNotes(FontInfo& fontInfo);
	void onVisualSetStartAtRow();
	bool applyStartNotation(CString startNotation);
	void setStartNotation(CString startNotation);
	void doEditStartNotation();
	BOOL OnPreparePrinting(CPrintInfo* pInfo, CView* view);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnFilePrintPreview(CView* view);
	void OnFilePrint();
	void OnFilePrintToolbar();
	void OnFilePrintMenu();
	int  OnPrintAnalysis();
	void OnPopupPrintAnalysis();

	

	void doPrintAnalysis(CDC* pDC, CPrintInfo* pInfo) ;
	void sortVariances(Variances& variances);
	void getUsedVariances(Variances& variances);
	void doGoto(const RowPosition& rowPosition, bool nullable = false, bool changeView = true, bool quiet = false);
	BOOL isMultiBlock();
	int getBlockCount();
	void autoCreateSplicedLetters();
	void addDefaultCallPositionsToBlankTouch();
	afx_msg void OnAddFindNotation();
	int getUsedDefinitionsCount();
	bool isDefinitionUsed(Definition* definition);
	void createSplices(Splices& splices);
	void createMusicRules(MusicRules& musicRules);
	TouchBlueLineView* getBlueLineView();
	TouchView* getTouchView();
	TouchHoldView* getTouchHoldView();
	TouchSimulatorView* getTouchSimulatorView();
	void createCalls(Calls& calls);
	void setEndWithRow(CString str);
	void parseCallsInColumnWithoutCallingPos();
	void parseSpaceOnlyInCell();
	bool isNotationNameUnique(const CString& name);
	bool isSpliceUnique(CString splice);
	bool isDefinitionUnique(const CString& name);

	BOOL getShowCourseHeads();
	void setName(const CString& name);
	void setAuthor(const CString& name);
	CString getAuthor();
	void setPlainLeadLetter(const CString& plainLead);
	CString getName();
	void parseVarianceLodgic();
	void parseNoVarianceInSplice(int start,int end);
	void parseVarianceLodgic(int start, int end);
	void parseGroupLodgic();
	void parseGroupLodgic(int start, int end);
	void parseGroupOnDifferentLines();
	void parseGroupOnDifferentLines(int start, int end);
	void parseVarianceGroupInteractionLodgic();
	void parseVarianceGroupInteractionLodgic(int start, int end);
	void sortDefinitions();
	void sortMusicRules();
	void editMusic(const CString& name, const CString& displayString);
	void editMusic(const CString& name, const CString& displayString, const CString& newRule);
	void setBlockCount(int checkingBlockCount);
	void editDefinition(CString name);
	void editDefinition(CString name, CString definitionStr);
	void deleteMusic(const CString& name, const CString& displayString);
	void deleteDefinition(const CString& name);
	void parseMainBodyNumbers();
	void parseSplicedCallsNotDefinedInEachMethod();
	void parseSplicedCallPosMethodNotDefinedInEachMethod();
	void parseSplicedCallPosAgregateNotDefinedInEachMethod();
	void parseDefinitionsOnWrongSide();
	void checkBlocks();
	void setCourseBased(BOOL courseBased);
	void OnSetStartStroke(Stroke stroke);
	bool isTouchBlockOnNode(int row);
	void tvDeleteRow();
	void tvDeleteColumn();
	void tvInsertRow();
	void tvInsertColumn();
	TouchBlock* getTouchBlockOnRow(int row);
	void negotiateBlockMove(TouchBlock *touchBlock, int val, bool start);
	void addTouchBlock(TouchBlock* tb);
	bool isSpliced();
	const CString& getActiveNotationString() const;
	void parseMultipleCallPosOnFirstLine();
	void parseBlockUseWithinBlock();
	void parseSplicedNotBlocks();
	void parseMainBodyCallsAndBlocks();
	void parseCallPos();
	void parseInit();
	void tidyGrid();
	void tvEditDeleteSelection(const GridPos& start, const GridPos& end, bool update = true);
	void deleteRow(int row);
	void deleteColumn(int column);
	bool isRowEmpty(int row);
	bool isColumnEmpty(int column);
	void tvEditTextEnd();
	void tvEditTextHome();
	void tvEditLineEnd();
	void tvEditLineHome();
	void tvEditWordRight();
	void tvEditWordLeft();
	void tvEditUntab();
	void tvEditTab();
	void tvEditDeleteBack();
	void tvEditDelete();
	void tvEditDeleteWord();
	void tvEditDeleteWordBack();
	void tvEditChar(UINT nChar, CString data = CString(""));
	void tvEditLineUp();
	void tvEditLineDown();
	void tvEditCharRight();
	CellData* getCellData(const GridPos& gridPos);
	void tvEditCharLeft();
	void invalidateColumnWidths();
	Notation * getActiveNotation();
	void onFileClose();
	int getNumber();
	void setNumberIndirect(int number);
	void setNumber(int number);
	bool canSetNumber();
	void editNotation(const CString& name);
	void setTab(int tab = 0);
	int getTab();
	virtual ~TouchDoc();
	void doTouchProve();
	void setUpdatePoint(CString name, bool setModifyed = true, 
	bool checkPoint = true, bool prove = true, bool tidy = true);
	void setStartWithChange(CString newChange);
	void setEndWithChange(CString newChange);
	void setPartCount(CString newPart);
	void setLeadCount(CString newLead);
	void setRowCount(CString newRow);
	void setCircularTouchCount(CString newCircle);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:

	int _proofRequestID;
	TouchStyle _touchStyle;
	CString _startRow;
	CString _endRow;
	Definitions _definitions;
	MusicRules _musicRules;
	int _partCountMax;
	int _leadCountMax;
	int _rowCountMax;
	int _circularTouchMax;
	int _startAtRow;
	int _blockCount;
	void parseGrid();
	Ints _columnWidths;

	BOOL _showPrintSetupDlg;
	BOOL _printAnalysis;
	PrintWhatAnalysis _printWhatAnalysis;

	GridArray * _grid;
	void destroyGrid();
	void createGrid(int x,int y);
	int _number;
	afx_msg void OnAddExplorerNotation();
	void addNotations(NotationLites& notations);

	
	void sortNotations();
	void writeTouchObject(TouchObject& touchObject);
	int _tab;
	Notations _notations;

	CString _activeNotation;
	GridPos _gridPos;
	BOOL _autoProve;
	BOOL _showCourseHeads;
	BOOL _showGrid;
	BOOL _callFrom;	 
	CString _notes;
	BOOL _showNotes;
	int _wrapCalls; //used as a BOOL at the moment
	


	Notation _startNotation; //the notation that will get prepended on to the start of the touch



	//{{AFX_MSG(TouchDoc)
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnTouchProve();
	afx_msg void OnUpdateTouchProve(CCmdUI* pCmdUI);


	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnEditGoto();
	afx_msg void OnUpdateEditGoto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsConvertoleadbase(CCmdUI* pCmdUI);
	afx_msg void OnToolsConvertoleadbaseInit();
	afx_msg void OnPopupEditnotes();
	afx_msg void OnFileSaveAs();
	afx_msg void OnToolsTranspose();
	afx_msg void OnUpdateToolsTranspose(CCmdUI* pCmdUI);
	afx_msg void OnToolsTidy();
	afx_msg void OnToolsResettoych();

	//}}AFX_MSG

protected:
	MethodPosition* _methodPositionBlueLineStatusBar;
	MethodPosition* _methodPositionSimulatorStatusBar;
	CString _simulatorMethodStatusBar;


	
	
	void parseSplices();
	void parseSpliceCountDifferentInEachMethod();

	void addDefaultMusic();
	bool hasUserMusic();


	TouchBlockArray _touchBlocks;
	Stroke _startStroke;
	BOOL _courseBased;
	CString _plainLeadLetter;

	CString _name;
	CString _author;

	TouchPropListExpanded _touchPropListExpanded;

	Method * _method;

	DECLARE_MESSAGE_MAP()

public:
	bool setupPlayMenu(CMenu* pPopup);
	bool setupTouchMenu(CMenu* pPopup);
	int getCallFrom(void);
	void setCallFrom(int callFrom);
	BOOL isCourseBased();
	CString getPlainLeadToken();
	GridPos& getGridPos();
	GridArray* getGridArray();
	TouchStyle& getTouchStyle();
	TouchBlockArray& getTouchBlocks();
	BOOL isShowGrid();
	void addColumn(int pos = -1);
	void addRow(int pos = -1);
	int getColumnWidth(CDC * pDC, int column);
	void setShowGrid(bool showGrid);
	void setShowCourseHeads(bool showCourseHeads);
	BOOL isShowNotes();
	Definitions& getDefinitions();
	MusicRules& getMusicRules();
	BOOL isPrintAnalysis();
	Method* getMethod();
	PrintWhatAnalysis& getPrintWhatAnalysis();
	const CString& getStartRow();
	const CString& getEndRow();
	int getStartAtRow();
	void setStartAtRow(int newRow);
	int getFirstLeadRowCount();
	Stroke getStartStroke();
	const Notation& getStartNotation();
	int getRowCountMax();
	int getLeadCountMax();
	int getPartCountMax();
	int getCircularTouchMax();
	void deleteNotation(const CString& name);
	afx_msg void OnAddNotation();
	afx_msg void OnAddDefinition();
	afx_msg void OnAddMusic();
	void cribNotation(const CString& name);
	TouchPropListExpanded& getTouchPropListExpanded();

	afx_msg void OnUpdateToolsTidy(CCmdUI *pCmdUI);

	BOOL getWrapCalls();
	void setWrapCalls(BOOL wrapCalls);
};
