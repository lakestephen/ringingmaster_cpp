#pragma once


class ResizeDialog : public CBCGPDialog
{
public:
	DECLARE_DYNAMIC(ResizeDialog)
	
	// possible sizing types
	
	enum SizeType
	{
		none,                           // do nothing
		resize,                         // proportional expand/contract
		repos,                          // maintain distance from top/left
		relative,                       // proportional distance from sides
		repos_center					// maintain distance from center
	};
	
protected:
	
	// class to hold item state
	
	class ResizeItem
	{
	public:
		UINT		_resID;					// resource ID
		SizeType	_xSizeType;				// x sizing option
		SizeType	_ySizeType;				// y sizing option
		CRect		_initControlRect;		// initial size
		CRect		_controlRect;			// last size
		bool		_flickerFree;			// flicker-free move?
		double		_xRatio;                // x ratio (for relative)
		double		_yRatio;                // y ratio (for relative)
		
		ResizeItem();
		
		void OnSize(HDWP hdwp,const CRect& parentOld,
				const CRect& parentNew, CRect & initParent, CWnd *parent);
		
		ResizeItem& operator=(const ResizeItem& resizeItem);
	};

	typedef std::vector<ResizeItem> resizeVector;
	
	// data members
	
	resizeVector		_items;				// array of controlled items
	CRect				_initDialogSize;	// original dialog size
	CRect				_dialogSize;		// last dialog size
	CBitmap				_sizeGripBitmap;	// size icon bitmap
	CStatic				_sizeGripWnd;		// size icon window
	bool				_inited;			//set up yet?
	
protected:
	//{{AFX_MSG(ResizeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
	void AddControl(const UINT resID, const SizeType xSize, const SizeType ySize, const bool flickerFree=true);
	
public:
	ResizeDialog(const UINT resID, CWnd *pParent);
	ResizeDialog() {};
	
	//{{AFX_DATA(ResizeDialog)
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(ResizeDialog)
	//}}AFX_VIRTUAL
};

