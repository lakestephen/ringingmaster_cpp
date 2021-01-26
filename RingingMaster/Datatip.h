#ifndef _DataTip_h__
    #define _DataTip_h__

class DataTip : public CWnd
{
// Construction
public:
	DataTip();

// Attributes
public:
    void SetOffset(long x, long y)  { _offset = CPoint(x, y); }
    void SetOffset(CPoint offset)   { _offset = offset; }

// Operations
public:
	virtual BOOL Create(CWnd* pParentWnd);
    void Set(CPoint point);
    void Hide( );

 // class operations
public:
    static void SetOnDelay(short onDelay)    { _onDelay = onDelay; }
    static void SetOffDelay(short offDelay)  { _offDelay = offDelay; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DataTip)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void removeAllStrings();
	void addString(CString &str);
	virtual ~DataTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(DataTip)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void Display( );

protected:
    CWnd*   _parent;
    CPoint  _offset;
    CRect   _captureRect;
    CPoint  _origin;
    BOOL    _ready;
	CArray <CString, CString&> _strings;

protected:
    static void RegisterWnd( );
    static void Initialise( );

    static BOOL _registered;
    static short _onDelay;
    static short _offDelay;
    static short _count;

    // keyboard hook
    static LRESULT CALLBACK KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam);	
    static HHOOK _hookProc;
    static DataTip* _current;
};

#endif

