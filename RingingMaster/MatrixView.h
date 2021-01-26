#pragma once

class MatrixGridCtrl;

enum MatrixEntityToken;
// MatrixView view

class MatrixView : public CBCGPGridView
{
	DECLARE_DYNCREATE(MatrixView)

protected:
	MatrixView();           // protected constructor used by dynamic creation
	virtual ~MatrixView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	
	CBCGPGridCtrl* CreateGrid();
	MatrixGridCtrl* getMatrixGridCtrl();

private:

protected:
	
	virtual MatrixEntityToken getMatrixEntityToken() const;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg void OnFilePrintPreview();

};


