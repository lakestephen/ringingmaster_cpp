#pragma once

#include "MatrixConstants.h"
#include "MatrixManagerEventListener.h"


class MatrixEntityBase;
class MatrixFilterSort;
enum MatrixEntityToken;


// MatrixGridCtrl

class MatrixGridCtrl : public CBCGPGridCtrl, MatrixManagerEventListener
{
	DECLARE_DYNAMIC(MatrixGridCtrl)

public:
	MatrixGridCtrl(MatrixEntityBase* entityBase);
	virtual ~MatrixGridCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp);
	BOOL CALLBACK gridCallback(BCGPGRID_DISPINFO* pdi);

	void getSelectedRowIds(CArray<long, long>& ids );
	static int compareLong(const void *arg1, const void *arg2);

public:
	void selectEntityFromId(int entityId);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	SaveableObject* getActiveItem();

	void applyFilterSort(MatrixFilterSort * filterSort);
	MatrixEntityBase* getEntitity();
	bool getEntitityToken(MatrixEntityToken& token);
	void update();
	BOOL OnDrawItem (CDC* pDC, CBCGPGridRow* pItem);

	void matrixManager_entityAdded(MatrixEntityToken token, long id);
	void matrixManager_entityRemoved(MatrixEntityToken token, long id);
	void matrixManager_refresh();


protected:
	MatrixEntityBase* _matrixEntity;
	CImageList _imageList;
	CString _pressedString;

	afx_msg void OnShowAll();
	afx_msg void OnUpdateShowAll(CCmdUI *pCmdUI);
	void Sort (int nColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE);

	
	bool doFindText(const CString& findText, int index);
	CFindReplaceDialog *_pFindDialog; 
	static const UINT _findDialogMessage;
	CString _searchStr;
	void doFind(CString findText, bool searchDown);

public:
	afx_msg void OnInvertSelection();
public:
	afx_msg void OnMemoryAssign();
	afx_msg void OnMainAssign();
	afx_msg void OnMemorySwap();
	afx_msg void OnMemoryOr();
	afx_msg void OnMemoryMinus();
	afx_msg void OnMemoryAnd();
	afx_msg void OnMemoryAddItem();
	afx_msg void OnMemoryRemoveItem();
	afx_msg void OnUpdateMemoryAddItem(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMemoryRemoveItem(CCmdUI *pCmdUI);
	afx_msg void OnEditGoto();
	afx_msg void OnUpdateEditGoto(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrevious();
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg LRESULT OnFindDialogMessage(WPARAM wParam, LPARAM lParam);		
	afx_msg void OnDestroy();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);

	void destroyFindWindow();

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

