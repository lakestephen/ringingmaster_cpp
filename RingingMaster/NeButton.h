#pragma once


class NumericEdit;


// NeButton

class NeButton : public CBCGPButton
{
	DECLARE_DYNAMIC(NeButton)

public:
  NeButton();

protected:
	DECLARE_MESSAGE_MAP()

	NumericEdit* _parent;
public:
	BOOL Create(LPCTSTR lpszCaption,  DWORD dwStyle, const RECT &rect, NumericEdit *pParentWnd);
	afx_msg LRESULT OnBnSetStyle(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
