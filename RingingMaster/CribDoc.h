
#pragma once

#include "Notation.h"	// Added by ClassView

			
class MethodCrib;
class Notation;	
class CribView	  ;
#include "Undo.h"
#include "CribStyle.h"

#include "MethodPosition.h"
#include "PlayManagerEventListener.h"

class CribDoc : public CDocument, public Undo, public PlayManagerEventListener

{
protected: // create from serialization only
	CribDoc();
	DECLARE_DYNCREATE(CribDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CribDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:

	void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage);
	void doGoto(const RowPosition& rowPosition, bool nullable = false, bool quiet = false);
	void setUpdatePoint(CString name, bool setModifyed = true, bool checkPoint = true, bool bRecalcCrib = true);

	void bulkUndo(int undoCount) ;
	void bulkRedo(int redoCount) ;

	bool saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format);
	bool saveAsRTF(CString fileName);


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


	void setMethodPositionStatusBar(MethodPosition* methodPositionStatusBar);
	void getBlueLineStatusBarText(CString &pane1, CString &pane2,CString &pane3,CString &pane4,CString &pane5,CString &pane6);


	void setShowNotes(BOOL showNotes);
	BOOL getShowNotes();
	CString getNotes();
	bool editNotes(CPoint point);
	void setNotes(CString notes);

	const FontInfo& getCribFont();
	const FontInfo& getCribNotesFont();
	void setCribFont(const FontInfo& font);
	void setCribNotesFont(const FontInfo& font);

	void recalcCrib();
	const Notation& getNotation();
	MethodCrib* getMethod();

	void redoRange(int count);
	void undoRange(int count);

	void setInitialData(const Notation& notation);
	virtual ~CribDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	MethodPosition* _methodPositionStatusBar;

	Notation _notation;
	MethodCrib * _method;

	CribView* getCribView();

	CribStyle _cribStyle;

	CString _notes;
	BOOL _showNotes;

	CString _title;

	int _cribNumber;

	BOOL _showPrintSetupDlg;
	BOOL _printAnalysis;



// Generated message map functions
protected:


	//{{AFX_MSG(CribDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnPopupEditnotes();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditGoto();
	afx_msg void OnUpdateEditGoto(CCmdUI* pCmdUI);

	//}}AFX_MSG
	public:
	int getNumber();
	void setCribNumber(int cribNumber);
	int getCribNumber();
	CString getTitle();
	void setTitle(CString title);
	afx_msg void OnPopupEditmethod();
	DECLARE_MESSAGE_MAP()			  

};
