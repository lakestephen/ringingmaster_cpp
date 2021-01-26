
#pragma once


#include "RecordManagerEventListener.h"

class RecordAnalysisTowerGrab;
class RecordAnalysisTowerTower;
class RecordTower;

enum RecordAnalysisTowerType
{
	ratt_home,
	ratt_selected
};

/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerTab window

class RecordAnalysisTowerTab : public CBCGPTabWnd, public MainFrameEventListener, public RecordManagerEventListener
{
// Construction
public:
	RecordAnalysisTowerTab(RecordAnalysisTowerType type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordAnalysisTowerTab)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
	//}}AFX_VIRTUAL

	RecordAnalysisTowerType _type;

	RecordTower* getTower();
	void update(RecordTower* recordTower);

	void mainFrame_notifyDocViewChanged(ViewType /*oldViewType*/, ViewType /*newViewType*/, CDocument* /*oldDoc*/, CDocument* /*newDoc*/);

// Implementation
public:
	virtual ~RecordAnalysisTowerTab();

	// Generated message map functions
protected:

	RecordAnalysisTowerTower* _tower;
	RecordAnalysisTowerGrab* _grab;
	//{{AFX_MSG(RecordAnalysisTowerTab)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	void recordManager_notifyHomeTowerChange(RecordTower* /*homeTower*/);
};
