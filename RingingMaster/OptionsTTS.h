#if !defined(AFX_OPTIONSTTS_H__E6CF0119_10CE_42ED_AF79_4ACC99795070__INCLUDED_)
#define AFX_OPTIONSTTS_H__E6CF0119_10CE_42ED_AF79_4ACC99795070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsTTS.h : header file
//

#include "CJFlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsTTS dialog

class OptionsTTS : public CBCGPPropertyPage
{
// Construction
public:
	OptionsTTS();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsTTS)
	enum { IDD = IDD_OPTIONS_TTS };
	CBCGPButton	_systemSetupBtn;
	CCJFlatComboBox	_voices;
	CSliderCtrl	_speedCtrl;
	CBCGPButton	_systemVolumeBtn;
	CBCGPButton	_testBtn;
	CSliderCtrl	_volumeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsTTS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsTTS)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTest();
	afx_msg void OnSystemVolume();
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemVolume(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnable();
	afx_msg void OnSelchangeVoices();
	afx_msg void OnSystemSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSTTS_H__E6CF0119_10CE_42ED_AF79_4ACC99795070__INCLUDED_)
