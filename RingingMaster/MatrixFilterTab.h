#pragma once


class MatrixFilterSort;
class MatrixEntityBase;


// MatrixFilterDlg dialog

class MatrixFilterTab : public CBCGPTabWnd
{

public:
	MatrixFilterTab(MatrixFilterSort& filterSort);   
	virtual ~MatrixFilterTab();

	BOOL Create(const RECT& rect, CWnd* pParentWnd);

	afx_msg void OnSelchangeCondition();
	afx_msg void OnSelchangeSorting();
	afx_msg void OnSelchangeColumn();

	void update();

protected:
	MatrixFilterSort& _filterSort;
	CCheckListBox _filterCtrl;
	CCheckListBox _sortCtrl;
	CCheckListBox _columnCtrl;
	MatrixEntityBase* _entitybase;
	CFont _font;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
