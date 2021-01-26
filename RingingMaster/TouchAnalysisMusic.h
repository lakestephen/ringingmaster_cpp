#pragma once


#include "ScrollWnd.h"
#include "SuperGridCtrl.h"	  
#include "TouchAnalysisBase.h"


class Method;
class MusicResult;
class CPage;
class MethodAnalysis;


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisMusic window

class TouchAnalysisMusic : public SuperGridCtrl, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisMusic();

// Attributes
public:

// Operations
public:
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	CString getEmptyListText();
	CImageList* CreateDragImageEx(int nItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisMusic)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
	//}}AFX_VIRTUAL

	static double print(CPage *page, double nextRow);	 

// Implementation
public:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	void addMusicNode(SGTreeItem* parent, MusicResult* result, bool enable = true);

	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisMusic();

	int GetIcon(const SGTreeItem* pItem);
								 

	// Generated message map functions
protected:		

	double printMusicLine(CPage *page, double nextRow, double column, MusicResult *result, bool enable,double labelOffset);//todo move out to the analysis windows
	CString createMusicPrintLabel(MusicResult* result);	

	CString copyMusicNode(MusicResult *result);
	CImageList _image;//Must provide an imagelist
	
	//{{AFX_MSG(TouchAnalysisMusic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnUpdatePrintAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnPrintAnalysis();
	afx_msg void OnPopupEditCopy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
};

