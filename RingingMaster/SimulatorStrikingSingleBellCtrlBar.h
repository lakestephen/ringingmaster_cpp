// SimulatorStrikingSingleBellCtrlBar.h: interface for the SimulatorStrikingSingleBellCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GraphWnd.h"
#include "DockingToolBar.h"
#include "SimulatorStrikingWnd.h"

class SimulatorStrikingSingleBellCtrlBar : public CBCGPDockingControlBar   
{
public:
	SimulatorStrikingSingleBellCtrlBar(SimulatorStrikingWndType type);
	virtual ~SimulatorStrikingSingleBellCtrlBar();

	DECLARE_DYNAMIC(SimulatorStrikingSingleBellCtrlBar)

	void AdjustLayout ();
		
public:

	void toolbarReset() ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SimulatorStrikingSingleBellCtrlBar)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
   	DockingToolBar _toolBar;
	SimulatorStrikingWnd _strikingWnd;



	//{{AFX_MSG(SimulatorStrikingSingleBellCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateSetBell(CCmdUI* pCmdUI);
	afx_msg void OnSetBell1();
	afx_msg void OnSetBell2();
	//}}AFX_MSG
	

	DECLARE_MESSAGE_MAP()
};

