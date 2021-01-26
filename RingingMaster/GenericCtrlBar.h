#pragma once
					

class GenericCtrlBar : public CBCGPDockingControlBar
{
public:
	GenericCtrlBar();
	virtual ~GenericCtrlBar();

	DECLARE_DYNAMIC(GenericCtrlBar)

	void AdjustLayout ();	

	BOOL setChild(CWnd* child);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GenericCtrlBar)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:

	CWnd* _child;

	//{{AFX_MSG(GenericCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
};


