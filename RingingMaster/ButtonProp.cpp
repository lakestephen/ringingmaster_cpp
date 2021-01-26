#include "stdafx.h"

#include "ButtonProp.h"
#include "UserMessages.h"
#include "BCGPButton.h"



ButtonProp::ButtonProp (const CString& strName, const _variant_t& varValue, 
		LPCTSTR lpszDescr, DWORD dwData, UINT imageID, BOOL hasValueField) :
 CBCGPProp (strName, varValue, lpszDescr, dwData), 
	 _hasButton(TRUE),
	 _hasValueField(hasValueField)
{
	_imageList.Create(imageID,16,1,RGB(192,192,192));
}
 
void ButtonProp::OnClickButton (CPoint point)
{
	if (m_pWndList)
		SendMessage(m_pWndList->m_hWnd, BCG_BTN_PRESS, MAKEWPARAM(point.x, point.y), (LPARAM)this );
 }

void ButtonProp::OnClickName (CPoint point)
{
	if (m_pWndList && !_hasValueField && m_rectButton.PtInRect (point))
		SendMessage(m_pWndList->m_hWnd, BCG_BTN_PRESS, MAKEWPARAM(point.x, point.y), (LPARAM)this );
}

BOOL ButtonProp::HasButton () const 
{ 
	return _hasButton; 
}

void ButtonProp::setHasButton(BOOL hadButton)
{
	_hasButton = hadButton   ;
}
					
void ButtonProp::OnDrawButton (CDC* pDC, CRect rect)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);
	ASSERT_VALID (m_pWndList);

	CBCGPToolbarComboBoxButton button;

	CBCGPVisualManager::BCGBUTTON_STATE state = 
					m_bButtonIsDown ?
					CBCGPVisualManager::ButtonsIsPressed :
					CBCGPVisualManager::ButtonsIsRegular;

	CBCGPVisualManager::GetInstance ()->OnFillButtonInterior (pDC,
		&button, rect, state);

	pDC->FillRect (rect, /*m_pWndList->m_bControlBarColors ?
		&globalData.brBarFace :*/ &globalData.brBtnFace);

	
	if (_imageList.GetImageCount() > 0 )
	{
		_imageList.Draw(pDC, 0, rect.TopLeft(), ILD_TRANSPARENT);
	}
	else
	{
		CString str = _T("...");
		pDC->DrawText (str, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	CBCGPVisualManager::GetInstance ()->OnDrawButtonBorder (pDC, &button, rect, state);
}	

