// StrikeGraphCtrlBar.h: interface for the StrikeGraphCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "StrikeManagerEventListener.h"
#include "GraphWnd.h"
#include "DockingToolBar.h"

class StrikeGraphCtrlBar : public CBCGPDockingControlBar, StrikeManagerEventListener   
{
public:
	StrikeGraphCtrlBar();
	virtual ~StrikeGraphCtrlBar();

	DECLARE_DYNAMIC(StrikeGraphCtrlBar)

	void AdjustLayout ();
		
public:
	void strikeManager_notifyUpdateStrikeList();
	void toolbarReset() ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StrikeGraphCtrlBar)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
   	DockingToolBar _toolBar;
	GraphWnd _graphWnd;

	void update();


	//{{AFX_MSG(StrikeGraphCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateSetBell(CCmdUI* pCmdUI);
	afx_msg void OnSetBell();
	//}}AFX_MSG
	
	afx_msg void OnHand();
	afx_msg void OnUpdateHand(CCmdUI *pCmdUI);
	afx_msg void OnBack();
	afx_msg void OnUpdateBack(CCmdUI *pCmdUI);
	afx_msg void OnHandBack();

	afx_msg void OnUpdateHandBack(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
};

