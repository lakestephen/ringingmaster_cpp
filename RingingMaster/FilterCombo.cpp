// FilterCombo.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "FilterCombo.h"
#include "FontInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GLYPH_WIDTH  10
#define SPACING      7


/////////////////////////////////////////////////////////////////////////////
// FilterCombo

FilterCombo::FilterCombo(FilterComboBtn* filterComboBtn) :
_filterComboBtn(filterComboBtn)
{
	m_img.Create(IDB_USER, GLYPH_WIDTH, 1, RGB(255,0,255));
}

FilterCombo::~FilterCombo()	 
{
}


BEGIN_MESSAGE_MAP(FilterCombo, CComboBox)
	//{{AFX_MSG_MAP(FilterCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FilterCombo message handlers

BOOL FilterCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CFrameWnd* pMainFrame = GetTopLevelFrame ();
		if (pMainFrame != NULL)
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				pMainFrame->SetFocus ();
				return TRUE;

			/*case VK_RETURN:
				pMainFrame->PostMessage (WM_COMMAND, ID_FILTER_COMBO,
										(LPARAM) GetSafeHwnd ());
				return TRUE;*/
			}
		}

		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void FilterCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{

	lpMeasureItemStruct->itemHeight = 14;
	lpMeasureItemStruct->itemWidth  = 250;
}

	
void FilterCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
//	TRACE("FilterCombo::DrawItem");
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX); 
	
	CRect rc = lpDIS->rcItem;
	
	CDC dc;
	dc.Attach(lpDIS->hDC);

	if (lpDIS->itemState & ODS_FOCUS)
		dc.DrawFocusRect(&rc);
	
	if (lpDIS->itemID == -1)
		return;

	int nIndexDC = dc.SaveDC();
	
	CBrush br;
	
	COLORREF clrSample = RGB(0,0,0);

	if (lpDIS->itemState & ODS_SELECTED)
	{
		br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrSample = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		br.CreateSolidBrush(dc.GetBkColor());
	}
	
	dc.SetBkMode(TRANSPARENT);
	dc.FillRect(&rc, &br);
	
	// which one are we working on?
	CString csName;
	
	if (_filterComboBtn)
		csName = _filterComboBtn->GetItem(lpDIS->itemID);
	else
		GetLBText(lpDIS->itemID, csName);

	// draw the cute user glyph
	DWORD dwData = GetItemData(lpDIS->itemID);
	if (dwData == 1)
		m_img.Draw(&dc, 0, CPoint(rc.left+5, rc.top+2),ILD_TRANSPARENT);
	
	rc.left += GLYPH_WIDTH;
	
	int iOffsetX = SPACING;



	// font name in GUI font
	CSize sz = dc.GetTextExtent(csName);
	int iPosY = 0;

	iPosY = (rc.Height() - sz.cy) / 2;

	//select the correct font 
	FontInfo boldFontInfo("MS Sans Serif");
	boldFontInfo._weight = FW_BOLD;
	LOGFONT lf;
	boldFontInfo.getLogFont(&lf);
	CFont boldFont;
	boldFont.CreateFontIndirect(&lf);

	if ((dwData == 1)&&(lpDIS->itemID != 0))
		dc.SelectObject(boldFont);
	else
		dc.SelectObject(&app()->_fontStd);


	if ((lpDIS->itemID == 0)&&(!(lpDIS->itemState & ODS_SELECTED)))
		dc.SetTextColor(RGB(100,100,100));

	dc.TextOut(rc.left+iOffsetX, rc.top + iPosY,csName);

	dc.RestoreDC(nIndexDC);

	dc.Detach();

}


