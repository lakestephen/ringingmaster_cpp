// ChildFrm.h : interface of the ChildFrameSplitter class
//


#pragma once  

#include "ChildFrm.h"


class ChildFrameSplitter : public ChildFrame
{

public:
	ChildFrameSplitter();

// Attributes
public:

	CBCGPSplitterWnd m_wndSplitter;

// Operations
public:

// Overrides
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~ChildFrameSplitter();

	virtual void getSplitCounts(int& maxCols, int& maxRows) = 0;
// Generated message map functions
protected:


	DECLARE_MESSAGE_MAP()
};

class ChildFrameSplitter12 : public ChildFrameSplitter
{
	DECLARE_DYNCREATE(ChildFrameSplitter12)

	virtual void getSplitCounts(int& maxCols, int& maxRows)
	{
		maxCols = 1;
		maxRows = 2;
	}
};

class ChildFrameSplitter22 : public ChildFrameSplitter
{
	DECLARE_DYNCREATE(ChildFrameSplitter22)

	virtual void getSplitCounts(int& maxCols, int& maxRows)
	{
		maxCols = 2;
		maxRows = 2;
	}
};