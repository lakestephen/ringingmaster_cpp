#pragma once

#include "StdAfx.h"	// Added by ClassView
#include "StrikeManagerEventListener.h"
#include "MainFrameEventListener.h"
#include "StrikeAnalysisType.h"
#include "ManagerBase.h"


class StrikeManager : public CCmdTarget, public ManagerBase, MainFrameEventListener
{
public:
	StrikeManager();
	virtual ~StrikeManager(); 

	void addEventListener(StrikeManagerEventListener* strikeManagerEventListener);
	void removeEventListener(StrikeManagerEventListener* strikeManagerEventListener);

	void save();
	void load();
	void closeDown();
	void openUp();

	void mainFrame_notifyUpdate(ViewType viewType);
	
	void fireNotifyStrikeChanged();

	int getMaxBells();
	int getAnalysisBell();
	bool setAnalysisBell(int analysisBell);
	BOOL getOrderByBells();
	void updateDocument();

	StrikeAnalysisType getStrikeAnalysisType();
	void setStrikeAnalysisType(StrikeAnalysisType strikeAnalysisType);
	

	afx_msg void OnShowPlace();
	afx_msg void OnUpdateShowPlace(CCmdUI *pCmdUI);
	afx_msg void OnShowBell();
	afx_msg void OnUpdateShowBell(CCmdUI *pCmdUI);

	afx_msg void OnImportBagley();

	DECLARE_MESSAGE_MAP()

	StrikeManagerEventListeners _listenerList;


protected:
	int _analysisBell;
	BOOL _orderByBells;
	StrikeAnalysisType _strikeAnalysisType;
};


