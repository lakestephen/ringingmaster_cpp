#pragma once

#include "PropListBase.h"
#include "SimulatorManagerEventListener.h"

class TouchDoc;

class TouchMainPropList :
	public PropListBase, 
	public SimulatorManagerEventListener
{
public:
	TouchMainPropList(ViewType viewType);
	~TouchMainPropList(void);


	DECLARE_MESSAGE_MAP()

public:
	void initPropList();

	void mainFrame_notifyUpdate(ViewType viewType);
	void updateMethodList(TouchDoc * pTouchDoc);
	void updateDefinitionsList(TouchDoc * pTouchDoc);
	void updateMusicList(TouchDoc * pTouchDoc);

	void OnPropertyChanged (CBCGPProp* pProp) const;


	afx_msg LRESULT OnBCGBtnPress(WPARAM wParam, LPARAM lParam);

	void simulatorManager_notifyProfileChange();

protected:
	void loadExpandStatus();
	void saveExpandStatus();

	const ViewType _viewType;	//this is used to define the type of view that this is implementing

	afx_msg void OnTouchMainDelete();
	afx_msg void OnTouchMainAdd();
	afx_msg void OnTouchMainCrib();
	afx_msg void OnAddExplorerNotation();
	afx_msg void OnAddFindNotation();
	afx_msg void OnUpdateTouchMainDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTouchMainAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTouchMainCrib(CCmdUI *pCmdUI);

};
