#pragma once


// TowerSoundsInputsBase dialog

#include "SimulatorInputSelectorWnd.h"
#include "KeyboardManagerEventListener.h"
#include "MultiBellInterfaceManagerEventListener.h"
#include "SpeechManagerEventListener.h"
#include "SerialControlManagerEventListener.h"
class Tower;
class SimulatorAction;



#define PROFILE_BELL_CTRL_COUNT 10

class TowerSoundsInputsBase : public CBCGPPropertyPage, 
											KeyboardManagerEventListener, 
											MultiBellInterfaceManagerEventListener, 
											SpeechManagerEventListener, 
											SerialControlManagerEventListener
{

// Construction
public:
	TowerSoundsInputsBase(UINT nIDTemplate, Tower& internalTower);   // standard constructor
	~TowerSoundsInputsBase();
												  

// Dialog Data
	//{{AFX_DATA(TowerSoundsInputsBase)
	CScrollBar	_sbLines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerSoundsInputsBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	SimulatorInputSelectorWnd _ipSelectWnd[PROFILE_BELL_CTRL_COUNT];

	Tower& _internalTower;


	// Generated message map functions
	//{{AFX_MSG(TowerSoundsInputsBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void update(void);
	void updateScroll(int scrollPos = 0);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnSimulatroAddBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSimulatroUpdateInput(WPARAM wParam, LPARAM lParam);


	void keyboardManager_notifyKeyPressFromSeperateThread(const KeyboardResponse& response, DWORD timestamp);
	afx_msg LRESULT OnKeyboardInterfaceEvent(WPARAM wParam, LPARAM lParam);
	void keyboardManager_notifyGetKeyList(KeyboardActionSimulatorEventDataMap& keyboardActionSimulatorEventDataMap) ;

	void multiBellInterfaceManager_notifyMBIInterfaceEventFromSeperateThread(const MultiBellResponse& multiBellResponse, DWORD timestamp);
	afx_msg LRESULT OnMultiBellInterfaceEvent(WPARAM wParam, LPARAM lParam);
	void multiBellInterfaceManager_notifyMBIStatusEvent(bool portOK, bool MBIOK);
	void multiBellInterfaceManager_notifyGetMultiBellInterfaceList(MultiBellActionSimulatorEventDataMap& multiBellActionSimulatorEventDataMap);

	void speechManager_notifySpeechRecognisedFromSeperateThread(const SpeechResponse& response, DWORD timestamp);
	void speechManager_notifyGetSpeechList(SpeechActionEventDataMap& speechActionEventDataMap);
	afx_msg LRESULT OnSpeechEvent(WPARAM wParam, LPARAM lParam);
	

	void serialControlManager_notifySerialControlEventFromSeperateThread(const SerialControlResponse& response, DWORD timestamp);
	afx_msg LRESULT OnSerialControlInterfaceEvent(WPARAM wParam, LPARAM lParam);
	void serialControlManager_notifyGetSerialControlList(SerialControlResponseSimulatorEventDataMap& serialControlResponseSimulatorEventDataMap);

	virtual SimulatorAction& getActionItem(int index) = 0;
	virtual int getActionCount() = 0;
	virtual CString getActionName(int index) = 0;
	virtual void addActionItem(int index) = 0;
	virtual int getPROFILE_BELL_CTRL_COUNT() {return PROFILE_BELL_CTRL_COUNT;}

private:
	void setup();
	void desetup();
	bool _setup;

	int getInputCount();
	bool getActionIndex(int inputIndex, int& actionIndex, int& actionInputInex);

public:
	virtual BOOL OnSetActive();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
