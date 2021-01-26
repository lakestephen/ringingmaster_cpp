#if !defined(AFX_NOTATIONDLG_H__4E3D0515_E9C2_11D7_B706_000255162CF0__INCLUDED_)
#define AFX_NOTATIONDLG_H__4E3D0515_E9C2_11D7_B706_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationDlg.h : header file
//

#include "Notation.h"
#include "PropertySheetBase.h"


class NotationPlain;
class NotationCalls;
class NotationCallingPosLead;
class NotationCallingPosMethod;
class NotationCallingPosAgregate;
class NotationSplicePos;
class NotationLeadLines;
class NotationCourseHead;
class NotationNotes;
class NotationInfo;

/////////////////////////////////////////////////////////////////////////////
// NotationDlg dialog

class NotationDlg : public PropertySheetBase
{
// Construction
public:
	NotationDlg(CWnd* pParent, Notation &notation, BOOL fixNumber, TouchDoc* touchDoc, bool editing, bool readOnly = false, bool noCrib = false); 	   // standard constructor
	~NotationDlg();

// Dialog Data
	//{{AFX_DATA(NotationDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CBCGPButton	_infoBut;
	CBCGPButton	_cribBut;
	CBCGPButton	_prevBut;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	  

	void Apply();

	Notation _notation;
	Notation &_originalNotation;	 

	CString _originalName;
	CString _originalShorthand;


	NotationPlain* _plain ;	  
	NotationCalls* _calls;	  
	NotationCallingPosLead* _callPosLead;	  	
	NotationCallingPosMethod* _callPosMethod;	  	
	NotationCallingPosAgregate* _callPosAgregate;	  	
	NotationSplicePos* _splicePos;	  	
	NotationLeadLines*	_leadLines;	  	
	NotationCourseHead* _courseHead;	  	
	NotationInfo* _info;	  
	NotationNotes* _notes;	  

	TouchDoc* _touchDoc;
	bool _editing;
	bool _readOnly;
	bool _noCrib;


	// Generated message map functions
	//{{AFX_MSG(NotationDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnUpdatePreviewButton(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg void OnPreviewButton();
	afx_msg void OnInfoButton();
	afx_msg void OnCribButton();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTATIONDLG_H__4E3D0515_E9C2_11D7_B706_000255162CF0__INCLUDED_)
