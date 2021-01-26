
#pragma once

class NumericEdit;

// NeTrackPopup


enum PopupStateChangeType
{
	PUSC_OPENING, 
	PUSC_CLOSING,
};


class NeTrackPopup : public CWnd
{
	DECLARE_DYNAMIC(NeTrackPopup)
private:
  float    m_nMin;
  float    m_nMax;
  float    m_nValue;
  double m_dPixelValue;

public:
  NeTrackPopup();
  BOOL Create(int x,int y,float nMin,float nMax,float nValue,NumericEdit* pParent);

protected:
	DECLARE_MESSAGE_MAP()

	NumericEdit* _parent;

public:

  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy);
};