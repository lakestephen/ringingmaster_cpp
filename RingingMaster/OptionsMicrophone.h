#if !defined(AFX_OPTIONSMICROPHONE_H__94FCC778_E1CA_4494_AF63_E4EAFB61381A__INCLUDED_)
#define AFX_OPTIONSMICROPHONE_H__94FCC778_E1CA_4494_AF63_E4EAFB61381A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMicrophone.h : header file
//


#include "CJFlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsMicrophone dialog

class OptionsMicrophone : public CBCGPPropertyPage
{
// Construction
public:
	OptionsMicrophone();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsMicrophone)
	enum { IDD = IDD_OPTIONS_MICROPHONE };
	CBCGPButton	_systemSetupBtn;
	CBCGPButton	_settingsBtn;
	CBCGPButton	_configMikeBtn;
	CBCGPButton _systemVolumeBtn;
	CBCGPButton _trainBtn;
	CBCGPButton _testBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsMicrophone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(OptionsMicrophone)
	afx_msg void OnSetup();
	afx_msg void OnEnable();
	afx_msg void OnUpdateGeneral(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemVolume(CCmdUI* pCmdUI);
	virtual BOOL OnInitDialog();
	afx_msg void OnSystemVolume();
	afx_msg void OnConfigMic();
	afx_msg void OnSystemSetup();
	afx_msg void OnSettings();
	afx_msg void OnTrain();
	afx_msg void OnBnClickedTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMICROPHONE_H__94FCC778_E1CA_4494_AF63_E4EAFB61381A__INCLUDED_)
