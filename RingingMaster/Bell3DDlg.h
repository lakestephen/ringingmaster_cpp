#if !defined(AFX_BELL3DDLG_H__106D3ABA_60A3_4BC5_B4AB_CE9C9DAE94D8__INCLUDED_)
#define AFX_BELL3DDLG_H__106D3ABA_60A3_4BC5_B4AB_CE9C9DAE94D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bell3DDlg.h : header file
//
				  
#include "ThreeDimEditWnd.h"
#include "ResizeDialog.h"
#include "ThreeDimEditCollection.h"
#include "Tower.h"

class TowerSoundsDlg;

/////////////////////////////////////////////////////////////////////////////
// Bell3DDlg dialog

class Bell3DDlg : public ResizeDialog
{				    
// Construction
public:
	void setup();
	Bell3DDlg(TowerSoundsDlg* pParent, Tower& tower);   // standard constructor
	~Bell3DDlg();

// Dialog Data
	//{{AFX_DATA(Bell3DDlg)
	enum { IDD = IDD_BELL_3D };
	CBCGPButton	_stop;
	CBCGPButton	_apply;
	CBCGPButton	_centerOfGravity;
	CBCGPButton _center;
	CBCGPButton	_zoomOut;
	CBCGPButton	_zoomIn;
	CBCGPButton	_makeCircle;
	CStatic	_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Bell3DDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void writePositions() ;

	ThreeDimEditWnd _3DWnd;

	ThreeDimEditCollection coll;
	TowerSoundsDlg* _parent;


	Tower& _origTower;

	int	_zoomLevel;

	// Generated message map functions
	//{{AFX_MSG(Bell3DDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMakeCircle();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomIn();
	afx_msg void OnCenter();
	afx_msg void OnCenterOfGravity();
	afx_msg void OnApply();
	//}}AFX_MSG
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCenter(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateCenterOfGravity(CCmdUI* pCmdUI); 

	afx_msg LRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnItemMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BELL3DDLG_H__106D3ABA_60A3_4BC5_B4AB_CE9C9DAE94D8__INCLUDED_)
