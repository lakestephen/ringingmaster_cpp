#pragma once

class NumericEdit;

class NeEdit : public CEdit
{
	DECLARE_DYNAMIC(NeEdit)

private:
  BOOL   m_fOverFlow;
  CBrush m_brError;

public:
	NeEdit() ;

protected:
	DECLARE_MESSAGE_MAP()

		CFont _fontStd;

	NumericEdit* _parent;
public:
	BOOL Create(DWORD dwStyle, const RECT& rect, NumericEdit* pParentWnd);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnEnChange();
  afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};