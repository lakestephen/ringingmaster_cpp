// MatrixView.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixView.h"
#include "MatrixGridCtrl.h"
#include "MatrixManager.h"
#include "MatrixEntityBase.h"


// MatrixView


IMPLEMENT_DYNCREATE(MatrixView, CBCGPGridView)

MatrixView::MatrixView() 
{

}

MatrixView::~MatrixView()
{
}

BEGIN_MESSAGE_MAP(MatrixView, CBCGPGridView)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &MatrixView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &MatrixView::OnUpdateFileSaveAs)

	ON_COMMAND(ID_FILE_PRINT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CBCGPGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()


// MatrixView drawing

void MatrixView::OnDraw(CDC* /*pDC*/)
{
	CDocument* pDoc = GetDocument();
	
}


// MatrixView diagnostics

#ifdef _DEBUG
void MatrixView::AssertValid() const
{
	CBCGPGridView::AssertValid();
}

#ifndef _WIN32_WCE
void MatrixView::Dump(CDumpContext& dc) const
{
	CBCGPGridView::Dump(dc);
}
#endif
#endif //_DEBUG


CBCGPGridCtrl* MatrixView::CreateGrid()
{
	return matrixManager()->createEntityGridCtrl(getMatrixEntityToken(), mfst_user);
}

MatrixEntityToken MatrixView::getMatrixEntityToken() const
{
	ASSERT(FALSE);//please override this.
	return met_tower;
}

MatrixGridCtrl* MatrixView::getMatrixGridCtrl()
{
	return (MatrixGridCtrl*)GetGridCtrl();
}

BOOL MatrixView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	MatrixGridCtrl* matrixGridCtrl = getMatrixGridCtrl();

	if ((matrixGridCtrl)&&(matrixGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)))
		return TRUE;

	return CBCGPGridView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MatrixView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void MatrixView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void MatrixView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//remove the find dialog box.
	if (getMatrixGridCtrl() && 
		(pActivateView != this || !bActivate))
	{
		getMatrixGridCtrl()->destroyFindWindow();
	}		

	__super::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL MatrixView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void MatrixView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}