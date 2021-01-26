// ChildFrm.cpp : implementation of the ChildFrameSplitter class
//
#include "stdafx.h"
#include "RingingMaster.h"
#include "ChildFrmSplitter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ChildFrameSplitter

IMPLEMENT_DYNCREATE(ChildFrameSplitter22, ChildFrameSplitter)
IMPLEMENT_DYNCREATE(ChildFrameSplitter12, ChildFrameSplitter)

BEGIN_MESSAGE_MAP(ChildFrameSplitter, ChildFrame)
END_MESSAGE_MAP()


// ChildFrameSplitter construction/destruction

ChildFrameSplitter::ChildFrameSplitter()
{

}

ChildFrameSplitter::~ChildFrameSplitter()
{
}

BOOL ChildFrameSplitter::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	int maxCols = 2;
	int maxRows = 2;

	getSplitCounts(maxCols, maxRows);

	return m_wndSplitter.Create(this,
		maxRows, maxCols,			
		CSize(10, 10),	
		pContext);
}


