#pragma once
#include "afxwin.h"


// NewTowerWizard dialog

class NewTowerWizard : public CDialog
{
	DECLARE_DYNAMIC(NewTowerWizard)

public:
	NewTowerWizard(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewTowerWizard();

// Dialog Data
	enum { IDD = IDD_TOWER_WIZARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTemplateButt();

	CString _treblePath;
protected:
	virtual void OnOK();
public:
	CString _staticPath;
	afx_msg void OnBnClickedCreateBlank();
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnUpdateUsingSimulator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUsingSimulatorInTower(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUsingSimulatorInTowerMBI(CCmdUI* pCmdUI) ;
	CButton _usingSimulatorCtrl;
	BOOL _usingSimulator;
	int _simulatorUseType;
	int _reflectorNumber;
	int _comPort;
	BOOL _addSpeechCalls;
};
