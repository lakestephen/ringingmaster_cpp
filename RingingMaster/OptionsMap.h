#if !defined(AFX_OPTIONSMAP_H__A374EDB6_A7C8_47C3_AC30_CEAD807063B4__INCLUDED_)
#define AFX_OPTIONSMAP_H__A374EDB6_A7C8_47C3_AC30_CEAD807063B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMap.h : header file
//

#include "CJFlatComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// OptionsMap dialog

class OptionsMap : public CBCGPPropertyPage
{
// Construction
public:
	void update();
	OptionsMap();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsMap)
	enum { IDD = IDD_OPTIONS_MAP };
	CStatic	_spVersionText;
	CStatic	_spVeriosn;
	CStatic	_staticProvidersCtrl;
	CStatic	_staticMBCtrl;
	CStatic	_staticCacheCtrl;
	CBCGPButton	_clearCache;
	CSpinButtonCtrl	_sizeSpinCtrl;
	CBCGPButton	_updateCheck;
	CCJFlatComboBox	_providersCtrl;
	CBCGPButton	_link;
	int		_size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsMap)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateCheck();
	afx_msg void OnLink();
	afx_msg void OnSelchangeProviders();
	afx_msg void OnClearCache();
	afx_msg void OnUpdateClearCache(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProviders(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeSpin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSizeEdit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnKillActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMAP_H__A374EDB6_A7C8_47C3_AC30_CEAD807063B4__INCLUDED_)
