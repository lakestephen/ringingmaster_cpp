#pragma once

#include "PropList.h"
#include "PropItem.h"


enum AvailableField;

class RecordAnalysisTowerTower : public PropList   
{
public:
	RecordAnalysisTowerTower();


// Attributes
public:

// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordAnalysisTowerTower)
	public:
	virtual BOOL Create(CWnd* parent, CRect rect = CRect(0,0,0,0));
	//}}AFX_VIRTUAL

	RecordTower* _tower;

// Implementation
public:
	void doButton(PropItem * pItem);
	void doUpdate(RecordTower* tower);
	virtual ~RecordAnalysisTowerTower();
	void doKillFocusEditBox(PropItem* pItem);
	void doKillFocusCmbBox(PropItem* pItem);
	void doHandleAction(PropItem* pItem);

	// Generated message map functions
protected:

	void addLine(AvailableField field, RecordTower* tower, COLORREF color = -1);
	//{{AFX_MSG(RecordAnalysisTowerTower)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnLaunchWebPage();
	afx_msg void OnUpdateLaunchWebPage(CCmdUI* pCmdUI);
	afx_msg void OnShowMap();
	afx_msg void OnUpdateShowMap(CCmdUI* pCmdUI);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};
