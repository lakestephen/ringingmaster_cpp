#if !defined(AFX_STATUSBARBCG_H__31D12A02_C86F_11D5_B510_000255162CF0__INCLUDED_)
#define AFX_STATUSBARBCG_H__31D12A02_C86F_11D5_B510_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusBar.h : header file
//

#include "MainFrameEventListener.h"


enum FileType;
enum ViewType;
/////////////////////////////////////////////////////////////////////////////
// StatusBar window

enum StatusBarAniState
{
	ANIST_CLEAR,
	ANIST_TICK,
	ANIST_CROSS,
	ANIST_ANI
};

class StatusBar : public CBCGPStatusBar, MainFrameEventListener 
{
// Construction
public:
	StatusBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void stopInternetAnimation();
	void startInternetAnimation();
	void SetProgress (long nCurr, long nTotal);
	void mainFrame_notifyIdle();
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~StatusBar();

	// Generated message map functions
protected:

	long _progressCurr;
	long _progressTotal;
						   
	ViewType _lastViewType;
	void setPaneText(int index, CString str);
	int getSize();
	void clearAllPaneText();
	void updateAnimation(BOOL force);

	void clearProofAnimation();
	void stopProofAnimation(BOOL showTick);
	void startProofAnimation();

	StatusBarAniState _state;
	CImageList	_proofAni;
	CBitmap _bmpTick;
	CBitmap _bmpCross;
	CBitmap _bmpClear;

	CBitmap _bmpMBI;
	CBitmap _bmpMBICross;

	CBitmap _bmpKeyboard;
	CBitmap _bmpKeyboardCross;

	CBitmap _bmpSpeech;
	CBitmap _bmpSpeechCross;

	CBitmap _bmpSerialControl;
	CBitmap _bmpSerialControlCross;

	CBitmap _bmpStatus;	   
	CImageList _imlStatusAnimation;



	//{{AFX_MSG(StatusBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBARBCG_H__31D12A02_C86F_11D5_B510_000255162CF0__INCLUDED_)
