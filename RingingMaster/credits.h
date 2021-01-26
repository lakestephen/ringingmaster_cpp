#ifndef __CREDITS_H_INCLUDED__
#define __CREDITS_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Credits - static text string with scrolling credits

class Credits : public CStatic
{
  DECLARE_DYNAMIC(Credits)

    // Construction and destruction
  public:
                Credits();    // constructor
    virtual    ~Credits(); // destructor

    // values available from external calls
  public:
    static enum creditspeed {SPEED_SLOW = 70, SPEED_MED = 40, SPEED_FAST = 10};

    // internal data
  private:
      // values that can be changed
    COLORREF          m_crBackground;
    COLORREF          m_crHeading1;
    COLORREF          m_crHeading2;
    COLORREF          m_crHeading3;
    COLORREF          m_crNormal;
    enum creditspeed  m_nSpeed;
      // values that are used internally
    int               m_nClip;            // amount of current line scrolled
    int               m_nCurrentHeight;   // height of current item
    UINT              m_nTimerID;         // the timer
    CString           m_strCurrent;       // current string
    int               m_nNextIndex;       // current index into m_strData
    CStringArray           m_strData;          // all the data
    CRect             m_rcClient;         // my client rect (store for speed)

    bool              m_bBitmap;

    int               m_nEscapeChar;
    CFont             m_fntArial;
    COLORREF          m_crColor;

    CBitmap           m_bmpCurrent;
    int               m_nBmpWidth;
      // constants used internally
    enum {
      DISPLAY_TIMER_ID = 150,   // timer id
      SCROLLAMOUNT = -1         // amount to scroll at each timer
    };
    enum {
      CH_HEADING1 = '\n',       // top level title
      CH_HEADING2 = '\r',       // top level group
      CH_HEADING3 = '\t',       // group title
      CH_NORMAL   = '\f',       // normal text
      CH_BITMAP   = '\b'        // bitmap
    };
    enum {
      HT_HEADING1 = 21,         // height of heading 1 text
      HT_HEADING2 = 19,         // height of heading 2 text
      HT_HEADING3 = 17,         // height of heading 3 text
      HT_NORMAL   = 15          // height of normal text
    };

    // Operations
  public:
      // call SetSpeed() to set the scrolling speed (returns previous speed)
    enum creditspeed  SetSpeed (enum creditspeed nSpeed);
      // call these to get/set the associated color (set returns previous color)
    COLORREF          GetColorBk ()   const { return m_crBackground; }
    COLORREF          GetColorH1 ()   const { return m_crHeading1; }
    COLORREF          GetColorH2 ()   const { return m_crHeading2; }
    COLORREF          GetColorH3 ()   const { return m_crHeading3; }
    COLORREF          GetColorText () const { return m_crNormal; }
    COLORREF          SetColorBk (COLORREF crNew);
    COLORREF          SetColorH1 (COLORREF crNew);
    COLORREF          SetColorH2 (COLORREF crNew);
    COLORREF          SetColorH3 (COLORREF crNew);
    COLORREF          SetColorText (COLORREF crNew);
      // call the following to reset the window content
    void              ResetContent ()
                      {
                        m_strData.RemoveAll();
                        if (m_nTimerID > 0) {
                          KillTimer(m_nTimerID);
                          m_nTimerID = 0;
                        }
                        if (m_hWnd && IsWindowVisible())
                          Invalidate();
                      }
      // call the following to add text and/or graphics
    int               AddHeading1 (LPCTSTR text)
                      {
                        return AddData(text, CH_HEADING1);
                      }
    int               AddHeading2 (LPCTSTR text)
                      {
                        return AddData(text, CH_HEADING2);
                      }
    int               AddHeading3 (LPCTSTR text)
                      {
                        return AddData(text, CH_HEADING3);
                      }
    int               AddNormalText (LPCTSTR text)
                      {
                        return AddData(text, CH_NORMAL);
                      }
    int               AddBitmap (LPCTSTR lpszBitmap)
                      {
                        return AddData(lpszBitmap, CH_BITMAP);
                      }
    int               AddBitmap (UINT nBitmapID)
                      {
                        CString str;
                        str.Format(_T("__RC_ID__%d"), nBitmapID);
                        return AddData(str, CH_BITMAP);
                      }
    int               AddBlankLine ()
                      {
                        return AddData(_T(""), _TCHAR('\0'));
                      }
    
    // Implementation
  protected:
    void              Initialize ();
    int               AddData (LPCTSTR text, TCHAR ch)
                      {
                        CString str = text;
                        str += ch;
                        return m_strData.Add((LPCTSTR)str);
                      }
    void              LoadNextLine();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();

  DECLARE_MESSAGE_MAP()
};

#endif
