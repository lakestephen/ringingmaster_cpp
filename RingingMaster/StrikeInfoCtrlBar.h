// StrikeInfoCtrlBar.h: interface for the StrikeInfoCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "StrikeManagerEventListener.h"
#include "DockingToolBar.h"
#include "PropList.h"



class StrikeInfoCtrlBar : public CBCGPDockingControlBar, StrikeManagerEventListener   
{
public:
	StrikeInfoCtrlBar();
	virtual ~StrikeInfoCtrlBar();

	DECLARE_DYNAMIC(StrikeInfoCtrlBar)

	void AdjustLayout ();
		
public:
	void strikeManager_notifyUpdateStrikeList();
	void toolbarReset() ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StrikeInfoCtrlBar)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	void addLine(CString col0, CString col1, COLORREF color = -1);

	// Generated message map functions
protected:
   	DockingToolBar _toolBar;
	PropList _propList;

	void update();


	//{{AFX_MSG(StrikeInfoCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateSetBell(CCmdUI* pCmdUI);
	afx_msg void OnSetBell();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHand();
	afx_msg void OnUpdateHand(CCmdUI *pCmdUI);
	afx_msg void OnBack();
	afx_msg void OnUpdateBack(CCmdUI *pCmdUI);
	afx_msg void OnHandBack();
	afx_msg void OnUpdateHandBack(CCmdUI *pCmdUI);
};

