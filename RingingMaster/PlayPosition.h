// PlayPosition.h: interface for the PlayPosition class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "PlayManagerEventListener.h"
#include "RowPosition.h"
#include "TowerManagerEventListener.h"

class PlayPosition : public CBCGPDockingControlBar, PlayManagerEventListener, TowerManagerEventListener
{
public:
	void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage);
	void towerManager_notifyUpdateTowerList();
	PlayPosition();
	virtual ~PlayPosition();

	DECLARE_DYNAMIC(PlayPosition)

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PlayPosition)
	public:
	//}}AFX_VIRTUAL



protected:
	RowPosition _rowPosition;
	CString _pealTime;
	int _lastNumberInclCover;

	CButton _trackCheck;
	BOOL _checkState;


	//{{AFX_MSG(PlayPosition)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrack();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnUpdateTrack(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()


};
