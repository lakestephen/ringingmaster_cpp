// MethodCallChangeWnd.cpp: implementation of the MethodCallChangeWnd class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodCallChangeWnd.h" 
#include "MethodPosition.h"
#include "Lead.h"
#include "StyleEditDlg.h"
#include "RegArchive.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(MethodCallChangeWnd, MethodWnd)


MethodCallChangeWnd::MethodCallChangeWnd() :
MethodWnd(ds_callChange),
_commentsWidth(0)
{

}

MethodCallChangeWnd::~MethodCallChangeWnd()
{
   	RegArchive ar(CArchive::store, "Style", "MethodCallChangeWnd");
	_style.Serialize(ar);

}

BEGIN_MESSAGE_MAP(MethodCallChangeWnd, MethodWnd)
	//{{AFX_MSG_MAP(MethodCallChangeWnd)
	ON_COMMAND(IDC_CC_OPTIONS, OnCCOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL MethodCallChangeWnd::Create( DWORD dwExStyle, 
							   LPCTSTR lpszWindowName, 
							   DWORD dwStyle, 
							   const RECT& rect, 
							   CWnd* pParentWnd, 
							   UINT nID, 
							   MethodCallChange * method) 
{
	BOOL ret = MethodWnd::Create(	dwExStyle, 
					 			lpszWindowName, 
					 			dwStyle, 
								rect, 
								pParentWnd, 
								nID, 
								method);
	if (ret)
	{
		//get the style for this window from the registry
		RegArchive ar(CArchive::load, "Style", "MethodCallChangeWnd");
		
		if (ar.isValid()) _style.Serialize(ar);					

		recalcLeadOrigins();
		recalcLeadBoxes();	
		
	}
	return ret;
}




void MethodCallChangeWnd::doToolsStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _method->getNumber(), slt_callchange);
	styleEditDlg.DoModal();
	notifyStyleChange();
	SetScrolls();
}


UINT MethodCallChangeWnd::getContectMenuId()
{
	return IDR_METHOD_STYLE_CALLEDCHANGES;
}

void MethodCallChangeWnd::drawComments(CDC *pDC)
{
//	if (!_style._commentsShow) return;

	LOGFONT lf; 

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(_style._commentsFont._color);

	//setup font
	CFont font;
	_style._commentsFont.getLogFont(&lf);
	font.CreateFontIndirect(&lf);
	CFont * oldFont = pDC->SelectObject(&font);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int halfTextHeight = tm.tmHeight/2;	

	
	int tabs[1];
	tabs[0] = getTabStops(pDC);

	_commentsWidth = 0;

/*
	//iterate through the leads
	for (int l = 0;l<_method->GetSize();l++)
	{
		Lead * lead = _method->GetAt(l);
		CSize orig = getLeadOrigin(l);
		orig.cx += ((_style._width * (_method->getNumber()-1)) + 5) + 10;//+ (_style._left/3));

		//index through all calls
		for (int j=0;j<lead->getCommentCount(rcut_display);j++)
		{
			RowComment* call = lead->getComment(j,rcut_display);
			int commentsWidth = 
				pDC->GetTabbedTextExtent(call->getDisplayString(),1,tabs).cx;
			if (_commentsWidth < commentsWidth)
				_commentsWidth = commentsWidth;

			pDC->TabbedTextOut(orig.cx,
						 orig.cy + ((_style._height*call->getLeadRow()) -
									(halfTextHeight)),
						 call->getDisplayString(),1,tabs,orig.cx);
		}
	}
	*/ //todo
	pDC->SelectObject(oldFont);	
}

int MethodCallChangeWnd::getTabStops(CDC *pDC)
{
	int size = 0;
	/*
	for (int l = 0;l<_method->GetSize();l++)
	{
		Lead * lead = _method->GetAt(l);
		for (int j=0;j<lead->getCommentCount(rcut_display);j++)
		{
			CStringArray strings;
			RowComment* call = lead->getComment(j, rcut_display);
			getStrings(strings, call->getDisplayString());
			for (int i=0;i<strings.GetSize();i++)
			{
				int width = pDC->GetTextExtent(strings.GetAt(i)).cx;
				if (width > size)
					size = width;
			}
		}
	}
				*/ //todo
	return size + 10;
}

void MethodCallChangeWnd::getStrings(CStringArray& strings, CString str)
{
	int startPos = 0;
	for (int i=0;i<str.GetLength();i++)
	{
		if (str.GetAt(i) == '\t')
		{
//			TRACE(str.Mid(startPos, i-startPos) + "\r\n");
			strings.Add(str.Mid(startPos, i-startPos));
			startPos = i+1;
		}
	}
}

CSize MethodCallChangeWnd::getSize()
{
	CSize size = MethodWnd::getSize();

	size.cx += _commentsWidth;

	return size;

}

void MethodCallChangeWnd::OnCCOptions() 
{
	if (GetParent() && IsWindow(GetParent()->m_hWnd))
		GetParent()->PostMessage(WM_COMMAND, IDC_CC_OPTIONS); 
}