#pragma once
#include "proplistbase.h"

enum PropertyTypeCrib
{
	ptc_generalRoot,
	ptc_titleEdit,
	ptc_cribFrom,
	
	ptc_methodRoot,
	ptc_methodItem,

	ptc_styleRoot,
	ptc_styleCribFont,
	ptc_styleCribNotesFont,
	ptc_styleBlueLine,

	ptc_notesRoot, 
	ptc_notesShow, 
	ptc_notesEdit, 

};

class CribPropList :
	public PropListBase
{
public:
	CribPropList(void);
	~CribPropList(void);

	DECLARE_MESSAGE_MAP()

public:
	void initPropList();

	void mainFrame_notifyUpdate(ViewType viewType);

	void OnPropertyChanged (CBCGPProp* pProp) const;


	afx_msg LRESULT OnBCGBtnPress(WPARAM wParam, LPARAM lParam);


protected:
	void loadExpandStatus();
	void saveExpandStatus();
};
