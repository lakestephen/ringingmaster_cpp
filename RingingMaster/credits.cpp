#include "stdafx.h"
#include "credits.h"

/////////////////////////////////////////////////////////////////////////////
// Credits window

IMPLEMENT_DYNAMIC(Credits, CStatic)

BEGIN_MESSAGE_MAP(Credits, CStatic)
  ON_WM_PAINT()
  ON_WM_TIMER()
  ON_WM_DESTROY()
END_MESSAGE_MAP()


/******************************************************************************
 * FUNCTION  Credits::Credits
 * PURPOSE   constructs the object
 ******************2***********************************************************/
Credits::Credits ()
{
    // general stuff
  m_crBackground = RGB(0, 0, 0);   // default is black
  m_nSpeed = Credits::SPEED_MED;  // default is medium speed
  m_nTimerID=0;

    // text stuff
  m_nCurrentHeight = HT_NORMAL;
  m_nNextIndex=0;
  m_crHeading1 = RGB(255, 0, 0);   // default is red
  m_crHeading2 = RGB(255, 255, 0); // default is yellow
  m_crHeading3 = RGB(0, 255, 255); // default is cyan
  m_crNormal = RGB(255, 255, 255); // default is white

    // bitmap stuff
  m_nClip=0;
  ResetContent();
  m_bBitmap = FALSE;
  m_nBmpWidth = 0;
} /* Credits::Credits */

/******************************************************************************
 * FUNCTION  Credits::~Credits
 * PURPOSE   destructor
 ******************2***********************************************************/
Credits::~Credits ()
{
} /* Credits::~Credits */

/******************************************************************************
 * FUNCTION  Credits::Initialize
 * PURPOSE   sets up the display rect and starts the timer
 ******************2***********************************************************/
void Credits::Initialize ()
{
    // get the client area
  if (IsWindowVisible()) {
    GetClientRect(&m_rcClient);
    CDC *pDC = GetDC();

    CBrush brBackground(m_crBackground);
    pDC->FillRect(&m_rcClient, &brBackground);
    brBackground.DeleteObject();
    ReleaseDC(pDC);

      // start the timer
    if (m_nTimerID > 0)
      KillTimer(m_nTimerID);
    if (m_strData.GetSize() > 0)
      m_nTimerID = SetTimer(DISPLAY_TIMER_ID, m_nSpeed, NULL);
    else
      m_nTimerID = 0;

      // restart at the first line
    m_nNextIndex = 0;
    LoadNextLine();
  }
} /* Credits::Initialize */

/******************************************************************************
 * FUNCTION  Credits::OnTimer
 * PURPOSE   scrolls the window by SCROLLAMOUNT units
 ******************2***********************************************************/
void Credits::OnTimer(UINT nIDEvent) 
{
  if (nIDEvent == DISPLAY_TIMER_ID) {
      // scroll the window
    if (IsWindowVisible()) {
      ScrollWindow(0, SCROLLAMOUNT, &m_rcClient, &m_rcClient);
        // increase the clip position
      m_nClip += abs((int)SCROLLAMOUNT);  
        // DO NOT load the next line until we paint or we will miss a pixel!!
      UpdateWindow(); // paint the window
        // now load the next line if necessary

      // now we can check for a need to load the next line
      if (m_nClip >= m_nCurrentHeight)
        LoadNextLine();
    }
  }
  else {
    CStatic::OnTimer(nIDEvent);
  }
} /* Credits::OnTimer */

/******************************************************************************
 * FUNCTION  Credits::OnPaint
 * PURPOSE   handles a paint message
 ******************2***********************************************************/
void Credits::OnPaint() 
{
    // take paint message off queue with these calls
  PAINTSTRUCT  ps;
  CDC         *pDC = BeginPaint(&ps);
  
    // if we don't have a timer yet, we need to initialize
  if (m_nTimerID == 0) {
      // start the timer, and do some other stuff
    Initialize();
  }
    // if we do have a timer, we're just scrolling
  else {
      // save the DC state for easy restoration
    int ival = pDC->SaveDC();

      // determine the region to update
    CRect rc(m_rcClient);
    rc.top = rc.bottom + SCROLLAMOUNT;

      // fill the background - it is not automatically filled
    CBrush brBackground(m_crBackground);
    pDC->FillRect(&rc, &brBackground);
    brBackground.DeleteObject();

    if (m_bBitmap) {
      CDC memDC;
      memDC.CreateCompatibleDC(pDC);
      CBitmap *pOldBitmap = memDC.SelectObject(&m_bmpCurrent);
      pDC->BitBlt(rc.left + ((m_rcClient.right-m_nBmpWidth)/2), rc.top,
                  m_nBmpWidth, abs((int)SCROLLAMOUNT),
                  &memDC, 0, m_nClip + SCROLLAMOUNT, SRCCOPY);
      memDC.SelectObject(pOldBitmap);
    }
    else if (m_strCurrent.GetLength() > 0) {
      pDC->SelectObject(&m_fntArial);
      pDC->SetTextColor(m_crColor);
      pDC->SetBkMode(TRANSPARENT);
      rc.top = rc.bottom - m_nClip;
      pDC->DrawText((LPCTSTR)m_strCurrent,
                    m_strCurrent.GetLength() - m_nEscapeChar, &rc,
                    DT_TOP | DT_CENTER | DT_NOPREFIX | DT_SINGLELINE);
    }

        // restore the DC to what it was
    pDC->RestoreDC(ival);
  }
  // Do not call base class OnPaint()
  EndPaint(&ps);
} /* Credits::OnPaint */

/******************************************************************************
 * FUNCTION  Credits::OnDestroy
 * PURPOSE   kills the current resources that we are using
 ******************2***********************************************************/
void Credits::OnDestroy() 
{
  ResetContent(); // kills the timer
  if (m_fntArial.GetSafeHandle())
    m_fntArial.DeleteObject();
  if (m_bmpCurrent.GetSafeHandle())
    m_bmpCurrent.DeleteObject();

  CStatic::OnDestroy(); // performs the object destruction
} /* Credits::OnDestroy */



///////////////////////////////////////////////////////////////////////////////
// Implementation functions
enum Credits::creditspeed Credits::SetSpeed (enum creditspeed nSpeed)
{
  enum creditspeed nPrevSpeed = m_nSpeed;
  m_nSpeed = nSpeed;
  Initialize();
  return nPrevSpeed;
} /* Credits::SetSpeed */
COLORREF Credits::SetColorBk (COLORREF crNew)
{
  COLORREF cr=m_crBackground;
  m_crBackground = crNew;
  Initialize();
  return cr;
}
COLORREF Credits::SetColorH1 (COLORREF crNew)
{
  COLORREF cr=m_crHeading1;
  m_crHeading1 = crNew;
  Initialize();
  return cr;
}
COLORREF Credits::SetColorH2 (COLORREF crNew)
{
  COLORREF cr=m_crHeading2;
  m_crHeading2 = crNew;
  Initialize();
  return cr;
}
COLORREF Credits::SetColorH3 (COLORREF crNew)
{
  COLORREF cr=m_crHeading3;
  m_crHeading3 = crNew;
  Initialize();
  return cr;
}
COLORREF Credits::SetColorText (COLORREF crNew)
{
  COLORREF cr=m_crNormal;
  m_crNormal = crNew;
  Initialize();
  return cr;
}



///////////////////////////////////////////////////////////////////////////////
// Utility functions
/******************************************************************************
 * FUNCTION  Credits::LoadNextLine
 * PURPOSE   loads the next line to be displayed (text, bitmap, or blank)
 ******************2***********************************************************/
void Credits::LoadNextLine ()
{
    // font variables
  BOOL      bUnderline=FALSE;
  BOOL      bItalic=FALSE;
  int       nWeight=FW_BOLD;

    // reset these variables
  if (m_fntArial.GetSafeHandle())
    m_fntArial.DeleteObject();
  if (m_bmpCurrent.GetSafeHandle())
    m_bmpCurrent.DeleteObject();
  m_bBitmap = FALSE;
  m_nEscapeChar=1;
  m_strCurrent = (m_strData.GetSize() > 0 ? m_strData[m_nNextIndex++] : _T(""));

    // get length of text
  int       nLength = m_strCurrent.GetLength(); // do this for speed

  if (nLength > 0) {
    switch (m_strCurrent[nLength-1]) {

      default:
        m_nEscapeChar = 0;
      case CH_NORMAL:
        m_nCurrentHeight = HT_NORMAL;
        nWeight = FW_THIN;
        m_crColor = m_crNormal;
        break;

      case CH_HEADING1:
        bUnderline = FALSE;
        m_nCurrentHeight = HT_HEADING1;
        m_crColor = m_crHeading1;
        break;

      case CH_HEADING2:
        m_nCurrentHeight = HT_HEADING2;
        m_crColor = m_crHeading2;
        break;

      case CH_HEADING3:
        m_nCurrentHeight = HT_HEADING3;
        m_crColor = m_crHeading3;
        break;

      case CH_BITMAP:
        m_bBitmap = TRUE;

        CString   strBitmap = m_strCurrent.Left(nLength - 1);
        BITMAP    bmInfo;
        BOOL      bOK=FALSE;
          // check for specified numeric id
        if (m_strCurrent.Left(9) == _T("__RC_ID__")) {
          int nID=0;
          strBitmap.Delete(0, 9);
          _stscanf((LPCTSTR)strBitmap, _T("%d"), &nID);
          bOK = m_bmpCurrent.LoadBitmap(nID);
        }
        else {
          bOK = m_bmpCurrent.LoadBitmap((LPCTSTR)strBitmap);
        }
          // load the bitmap - give message on error
        if (!bOK) {
          CString str; 
          str.Format(_T("Could not find bitmap resource \"%s\". "), strBitmap); 
#ifdef _DEBUG
          AfxMessageBox(str); 
#endif
          m_nCurrentHeight = 0;
        }
        else {
            // get bitmap info
          m_bmpCurrent.GetObject(sizeof(BITMAP), &bmInfo);
          m_nCurrentHeight = bmInfo.bmHeight;
          m_nBmpWidth = bmInfo.bmWidth;
        }
        break;
    } /* switch */
  } /* if not empty */
  else {
      // make blank line the size of normal text
    m_nCurrentHeight = HT_NORMAL;
  }

  if (!m_bBitmap && nLength > 0) {
      // create a font
    m_fntArial.CreateFont(m_nCurrentHeight, 0, 0, 0, nWeight, (BYTE)bItalic,
                          (BYTE)bUnderline, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                          VARIABLE_PITCH | 0x04 | FF_DONTCARE, (LPSTR)"Arial");
  }
    // reset current index if necessary
  if (m_nNextIndex >= m_strData.GetSize()) {
    m_nNextIndex = 0;
  }
    // reset clipped region and distance scrolled
  m_nClip = 0;

} /* Credits::LoadNextLine */
