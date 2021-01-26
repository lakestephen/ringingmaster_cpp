// MethodCribWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodCribWnd.h"
#include "MethodCrib.h"
#include "Crib.h"
#include "CribDoc.h"

#include "StyleEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MethodCribWnd

MethodCribWnd::MethodCribWnd() :
MethodWnd(ds_fromCribManager),
_statSize(0,0)
{
}

MethodCribWnd::~MethodCribWnd()
{
}


BEGIN_MESSAGE_MAP(MethodCribWnd, MethodWnd)
	//{{AFX_MSG_MAP(MethodCribWnd)
		ON_COMMAND(ID_CRIB_OPTIONS, OnCribOptions)
	ON_COMMAND(ID_POPUP_EDITNOTES, OnPopupEditnotes)
	ON_COMMAND(ID_POPUP_EDITMETHOD, OnPopupEditmethod)
	ON_COMMAND(ID_FILE_PRINT_MENU, OnFilePrintMenu)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MethodCribWnd message handlers


void MethodCribWnd::OnDraw(CDC *pDC)
{
	//draw the base class
	MethodWnd::OnDraw(pDC);
/*	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	
//	ASSERT(methodCrib);
	if ((!methodCrib)||((_16BitOverload))) 
		return;
	
	drawCrib(pDC, methodCrib);
	drawNotes(pDC, methodCrib);
	*/ //todo
}

void MethodCribWnd::drawCrib(CDC *pDC, MethodCrib* methodCrib)
{

	CribDoc* cribDoc = methodCrib->getDocument();

	ASSERT(cribDoc);

	if (cribDoc == NULL)
		return;


	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;

	cribDoc->getCribFont().getLogFont(&lf);
	
	CFont cribFont;
	cribFont.CreateFontIndirect(&lf);

	
	CFont* oldFont = pDC->SelectObject(&cribFont);
		
	COLORREF oldColor = pDC->SetTextColor(cribDoc->getCribFont()._color);

	Crib* crib = methodCrib->getCrib();
	if (crib)
	{
		CString strCrib = crib->getDescription();
		pDC->DrawText(strCrib,  getCribRect(), DT_WORDBREAK| DT_NOPREFIX);
	}

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(oldFont);		 
}

void MethodCribWnd::drawNotes(CDC *pDC, MethodCrib* methodCrib)
{
	CribDoc* cribDoc = methodCrib->getDocument();

	ASSERT(cribDoc);

	if (cribDoc == NULL)
		return;

	if (!cribDoc->getShowNotes())
		return;

	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;

	cribDoc->getCribNotesFont().getLogFont(&lf);
	
	CFont notesFont;
	notesFont.CreateFontIndirect(&lf);
	
	CFont* oldFont = pDC->SelectObject(&notesFont);
		
	COLORREF oldColor = pDC->SetTextColor(cribDoc->getCribNotesFont()._color);

	pDC->DrawText(cribDoc->getNotes(),  getNotesRect(), DT_WORDBREAK| DT_NOPREFIX);

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(oldFont);		 
}

UINT MethodCribWnd::getContectMenuId()
{
	return IDR_METHOD_CRIB;
}

int MethodCribWnd::getStyleMenuIndex()
{
	return 4;
}

void MethodCribWnd::OnCribOptions() 
{
	mainFrame()->doCribOptions(this);
}

void MethodCribWnd::OnPopupEditnotes() 
{
	::PostMessage(mainFrame()->m_hWnd,WM_COMMAND, ID_POPUP_EDITNOTES,0);	
}

void MethodCribWnd::OnPopupEditmethod() 
{
	::PostMessage(mainFrame()->m_hWnd,WM_COMMAND, ID_POPUP_EDITMETHOD,0);
}

void MethodCribWnd::OnFilePrintMenu() 
{
	::PostMessage(mainFrame()->m_hWnd,WM_COMMAND, ID_FILE_PRINT_MENU,0);
}


CRect MethodCribWnd::getMethodRect()
{
  	CRect rect (0,0,0,0);

/*	if ((_method)&&(_method->GetSize()>0)&&(!_16BitOverload))
	{
		rect = getLeadBox(0);
		rect.right += _style._left;
	}
	 //todo
  */
	return rect;

}

CRect MethodCribWnd::getCribRect()
{
	CRect methodRect = getMethodRect();

	//now calculate the crib box
	CRect clientRect;
	GetClientRect(&clientRect);

	int right = max(clientRect.right-10,methodRect.right + 200);

	CRect cribRect(methodRect.right, 
					methodRect.top, 
					right, 
					methodRect.top + 200);

	//use drawrect to calculate the rect required
/* todo 	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	if (methodCrib)
	{
		CribDoc* cribDoc = methodCrib->getDocument();

		if (cribDoc)
		{
			CDC* pDC = GetDC();
			pDC->SetMapMode(MM_TEXT);
			pDC->SetBkMode(TRANSPARENT);

			LOGFONT lf;

			cribDoc->getCribFont().getLogFont(&lf);
			
			CFont cribFont;
			cribFont.CreateFontIndirect(&lf);

			CFont* oldFont = pDC->SelectObject(&cribFont);
				
			Crib* crib = methodCrib->getCrib();
			if (crib)
			{
				CString strCrib = crib->getDescription();
				pDC->DrawText(strCrib,  cribRect, DT_WORDBREAK|DT_CALCRECT| DT_NOPREFIX);
			}
			pDC->SelectObject(oldFont);		 

			ReleaseDC(pDC);
		}	
	}
			*/
	return cribRect;

}

CRect MethodCribWnd::getNotesRect()
{
	CRect cribRect = getCribRect();
	CRect methodRect = getMethodRect();

	int top = max(cribRect.bottom, methodRect.bottom) + 20;
	CRect notesRect(_style.getLeft(), 
					top,
					cribRect.right, 
					top + 10);

	//use drawrect to calculate the rect required
/* todo	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	CribDoc* cribDoc = methodCrib->getDocument();

	if ((methodCrib)&&(cribDoc)&&(cribDoc->getShowNotes()))
	{
		CDC* pDC = GetDC();
		pDC->SetMapMode(MM_TEXT);
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;

		cribDoc->getCribNotesFont().getLogFont(&lf);
		
		CFont notesFont;
		notesFont.CreateFontIndirect(&lf);

		CFont* oldFont = pDC->SelectObject(&notesFont);
			
		pDC->DrawText(cribDoc->getNotes(),  notesRect, DT_WORDBREAK|DT_CALCRECT| DT_NOPREFIX);

		pDC->SelectObject(oldFont);		 

		ReleaseDC(pDC);

	}	
	   */

	return notesRect;
}

CSize MethodCribWnd::getSize()
{
	CRect notesRect = getNotesRect();
	CRect cribRect = getCribRect();

	
	return CSize(max(notesRect.right, cribRect.right) + 10, notesRect.bottom + 10);	
}

CString MethodCribWnd::getCopyDataString()
{
	CString str = MethodWnd::getCopyDataString();
	
/*	MethodCrib* methodCrib = dynamic_cast<MethodCrib*>(_method);
	CribDoc* cribDoc = methodCrib->getDocument();

	if ((cribDoc)&&(methodCrib))
	{
		Crib* crib = methodCrib->getCrib();
		if (crib)
		{
			str += "\r\n\r\n";
			str += crib->getDescription();
		}

		str += "\r\n\r\n";
		str += cribDoc->getNotes();
	}
*/ //todo	

	return str;
}



void MethodCribWnd::OnTimer(UINT nIDEvent) 
{
	MethodWnd::OnTimer(nIDEvent);

	if (nIDEvent == AAA_CRIB_SIZE_TIMER)
	{

		CSize size = getSize();

		if (size != _statSize)
		{
			SetScrolls();
			_statSize = size;
		}
	}
}

int MethodCribWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MethodWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(AAA_CRIB_SIZE_TIMER, 2000, NULL);
	
	return 0;
}

void MethodCribWnd::OnDestroy() 
{
	KillTimer(AAA_CRIB_SIZE_TIMER);

	MethodWnd::OnDestroy();
	
}

void MethodCribWnd::doToolsStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _method->getNumber(), slt_crib);
	styleEditDlg.DoModal();
	notifyStyleChange();
	SetScrolls();
}