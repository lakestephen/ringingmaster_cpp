#if !defined(AFX_ANALYSISTOWERGRAB_H__410AD774_68E8_11D7_B6A0_000255162CF0__INCLUDED_)
#define AFX_ANALYSISTOWERGRAB_H__410AD774_68E8_11D7_B6A0_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordAnalysisTowerGrab.h : header file
//

#include "ScrollWnd.h"
#include "SuperGridCtrl.h"	  

enum AvailableField;

class RecordGrab;

/////////////////////////////////////////////////////////////////////////////
// RecordAnalysisTowerGrab window

class RecordAnalysisTowerGrab : public SuperGridCtrl 
{
// Construction
public:
	RecordAnalysisTowerGrab();

// Attributes
public:
	CString getEmptyListText();
	CImageList* CreateDragImageEx(int nItem);


// Operations
public:

	int GetIcon(const SGTreeItem* pItem);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordAnalysisTowerGrab)
	public:
	virtual BOOL Create(CWnd* parent);

	RecordTower* _tower;

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~RecordAnalysisTowerGrab();
	void doUpdate(RecordTower* tower);

	// Generated message map functions
protected: 

	CImageList _image;//Must provide an imagelist


	//{{AFX_MSG(RecordAnalysisTowerGrab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISTOWERGRAB_H__410AD774_68E8_11D7_B6A0_000255162CF0__INCLUDED_)
