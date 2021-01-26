/*
NumericEdit - A clone of the spinedit control used in Jasc's Paint Shop Pro
Contact : jg@jgsoftware.com
Use the code for anything, just submit suggestions and bug fixes to above address.

4/13/2004 - 1.0 Initial release

4/20/2004 - 1.02 Bug fixes and some enhancements

Fixed bug where the edit would only accept hex digits. Also now allow negative sign "-" to be entered 
Added NES_METER style to make the small bar under the edit field optional 
Control will now reconfigure itself when changing style bits 
Fixed max value bug. The full range is now accesible with the popup 
Added NES_LIVEUPDATE style. This style allows the control to notify it's parent during value tracking. Without this style, the parent is updated only at the end of tracking 
Demo has been updated to show the new features 
Added DDX_NumericEdit() function for DDX support 
Now using the active window caption color for the bar colors 
Changed hardcoded custom messages to use windows registered messages so as not to cause any conflict with other custom messages
*/

#pragma once

#include "NeEdit.h"
#include "NeTrackPopup.h"
#include "NeButton.h"

// NumericEdit

static const UINT NEN_CHANGED = ::RegisterWindowMessage(_T("NEN_CHANGED-{CD9B6FDC-F420-4956-91B0-AB882EC15A89}"));

// Styles
#define NES_SPIN       0x0001
#define NES_POPUP      0x0002
#define NES_METER      0x0004
#define NES_LIVEUPDATE 0x0008


class NumericEdit : public CWnd
{
	DECLARE_DYNAMIC(NumericEdit)

public:
	NumericEdit();

private:
  float    m_nMin;
  float    m_nMax;
  float  m_nValue;
  BOOL   m_fCaptured;
  CRect  m_rcBar;
  double m_dPixelValue;
  BOOL   m_fTracking;

  void ResizeChildren(int cx,int cy);

protected:
	DECLARE_MESSAGE_MAP()
  NeButton       m_wndButton;
  CSpinButtonCtrl m_wndSpinButton;
  NeEdit         m_wndEdit;

  BOOL _fInFunction;

public:
  float  GetValue();
  void SetValue(float nValue, bool doUpdateText = true);
  void GetRange(float& nMin,float& nMax);
  void SetRange(float nMin,float nMax);
  BOOL GetReadOnly();
  void SetReadOnly(BOOL fReadOnly);

  afx_msg void OnPaint();
  BOOL Create(DWORD dwStyle,const CRect& rcRect,CWnd* pParent,UINT nID);

public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnStyleChanged(int nWhich, LPSTYLESTRUCT lpStyleStruct);
  void ShowPopup();
  void PopupStateChange(PopupStateChangeType popupStateChange);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};
