// ChildFrm.h : interface of the ChildFrame class
//

//todo compare this with the scheduler child frame.
#pragma once


class ChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(ChildFrame)
public:
	ChildFrame();

// Attributes
public:

	CBCGPSplitterWnd m_wndSplitter;

// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow);

	virtual LPCTSTR GetDocumentName (CObject** pObj);
	virtual bool createDocumentObject();

	static const CString _TowerDocumentToken;
	static const CString _GrabDocumentToken;
	static const CString _MapDocumentToken;
	static const CString _InternetDocumentToken;

// Implementation
public:
	virtual ~ChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:

	virtual long getExtraInfo() {return 0;}
	virtual void setExtraInfo(long extraInfo) {}

	CString documentName;
	CObject* documentObject;

	DECLARE_MESSAGE_MAP()
};
