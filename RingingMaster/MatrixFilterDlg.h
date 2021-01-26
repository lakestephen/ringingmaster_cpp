#pragma once

#include "ResizeDialog.h"

#include "MatrixManager.h"
#include "MatrixFilterTab.h"
#include "MatrixFilterSort.h"
#include "MatrixFilterDescriptionWnd.h"

class MatrixFilterSort;

// MatrixFilterDlg dialog

class MatrixFilterDlg : public ResizeDialog
{

public:
	MatrixFilterDlg(MatrixFilterSort& originalFilterSort, bool editing, CWnd* pParent );   
	virtual ~MatrixFilterDlg();

// Dialog Data
	enum { IDD = IDD_MATRIX_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	
	afx_msg LRESULT OnMatrixFilterUpdate(WPARAM, LPARAM);
	void update();
	
	bool _editing;
	CBCGPButton _up;
	CBCGPButton _down;
	CBCGPButton _remove;
	CString _name;
	CEdit _nameCtrl;

	DECLARE_MESSAGE_MAP()
public:
	MatrixFilterTab _tabCtrl;
	MatrixFilterDescriptionWnd _descCtrl;


	MatrixFilterSort& _originalFilterSort;
	MatrixFilterSort _filterSort;


	afx_msg void OnDestroy();

	
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedUp();
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI); 
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI); 
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	
	static CString CapitalString(CString strString);
protected:
	
	virtual void OnOK();
public:
	afx_msg void OnEnChangeMatrixFilterName();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
