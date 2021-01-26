#pragma once
#include "proplistbase.h"


enum propertyTypeMap
{
	ptm_generalRoot,
	ptm_providerCurrent,
	ptm_providerScale,
	ptm_grabbedColor,
	ptm_nonGrabbedColor,
};

class MapPropList :
	public PropListBase
{
public:
	MapPropList(void);
	~MapPropList(void);

	DECLARE_MESSAGE_MAP()

public:
	void initPropList();

	void mainFrame_notifyUpdate(ViewType viewType);

	void OnPropertyChanged (CBCGPProp* pProp) const;


	afx_msg LRESULT OnBCGBtnPress(WPARAM wParam, LPARAM lParam);


protected:
	void loadExpandStatus();
	void saveExpandStatus();
public:
	afx_msg void OnMapOptions();
	afx_msg void OnUpdateMapOptions(CCmdUI *pCmdUI);
};
