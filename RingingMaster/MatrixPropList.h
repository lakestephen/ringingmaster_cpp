#pragma once

#include "PropListBase.h"
#include "MatrixManagerEventListener.h"
#include "UserManagerEventListener.h"

class MatrixPropList :
	public PropListBase, public MatrixManagerEventListener, public UserManagerEventListener
{
public:
	MatrixPropList();
	~MatrixPropList(void);


	DECLARE_MESSAGE_MAP()

public:
	void initPropList();

	void mainFrame_notifyDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc);
	void userManager_notifyUserListChange();
	void OnPropertyChanged (CBCGPProp* pProp) const;


	afx_msg LRESULT OnBCGBtnPress(WPARAM wParam, LPARAM lParam);

	void matrixManager_updateFilterList();

protected:
	void loadExpandStatus();
	void saveExpandStatus();

	afx_msg void OnDeleteUserFilter();
	afx_msg void OnUpdateDeleteUserFilter(CCmdUI *pCmdUI);

	afx_msg void OnAddUserFilter();
	afx_msg void OnUpdateAddUserFilter(CCmdUI *pCmdUI);

	afx_msg void OnApplyUserFilter();
	afx_msg void OnUpdateApplyUserFilter(CCmdUI *pCmdUI);

	CMap<int, int, int, int> _filterMap;

public:
	afx_msg void OnUserOptions();
	afx_msg void OnUpdateUserOptions(CCmdUI *pCmdUI);
};
