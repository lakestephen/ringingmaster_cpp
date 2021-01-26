// PropertyCtrlBar.h: interface for the PropertyCtrlBar class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "MainFrameEventListener.h"
#include "DockingToolbar.h"

class TouchMainPropList;
class MatrixPropList;
class InternetPropList;
class CribPropList;
class MapPropList;
class PropListBase;


class PropertyCtrlBar : public CBCGPDockingControlBar, MainFrameEventListener   
{
public:
	PropertyCtrlBar();
	virtual ~PropertyCtrlBar();

	DECLARE_DYNAMIC(PropertyCtrlBar)

	void AdjustLayout ();
		
public:
	void mainFrame_notifyDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropertyCtrlBar)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
   	DockingToolBar _defaultToolBar;
   	DockingToolBar _touchMainToolBar;
   	DockingToolBar _mapToolBar;
   	DockingToolBar _recordToolBar;
													  
	TouchMainPropList* _touchMain;
	TouchMainPropList* _touchBlueLine;
	TouchMainPropList* _touchSimulator;
	MatrixPropList* _matrix;
    InternetPropList* _internet;
	CribPropList* _crib;
    MapPropList* _map;

	CWnd* getViewTypeWnd(ViewType viewType);
	CBCGPToolBar* getViewTypeToolbar(ViewType viewType);
	PropListBase* getPropListBase(ViewType viewType);

	//{{AFX_MSG(PropertyCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnSortingAlpha();
	afx_msg void OnUpdateSortingAlpha(CCmdUI* pCmdUI);
	afx_msg void OnSortingHirch();
	afx_msg void OnUpdateSortingHirch(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
