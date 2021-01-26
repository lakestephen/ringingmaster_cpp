#include "stdafx.h"
#include "RingingMaster.h"

#include "ResizeDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// used if OEMRESOURCE not defined in precompiled headers

#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif


/////////////////////////////////////////////////////////////////////////////
// ResizeDialog dialog


ResizeDialog::ResizeDialog(const UINT resID,CWnd *pParent)
: CBCGPDialog(resID,pParent),
_inited(false)
{
	//{{AFX_DATA_INIT(ResizeDialog)
	//}}AFX_DATA_INIT
}

IMPLEMENT_DYNAMIC(ResizeDialog, CBCGPDialog)

BEGIN_MESSAGE_MAP(ResizeDialog, CBCGPDialog)
	//{{AFX_MSG_MAP(ResizeDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResizeDialog dialog message handlers

BOOL ResizeDialog::OnInitDialog() 
{
	CRect bitmapRect, clientRect;
	
	// call the base class
	CBCGPDialog::OnInitDialog();
	
	// get the dialog size
	GetWindowRect(_initDialogSize); //the size is kept to operate repos_cent
	_dialogSize = _initDialogSize;
	
	// set up the size grip bitmap
	_sizeGripBitmap.LoadOEMBitmap(OBM_SIZE);
	_sizeGripWnd.Create(NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP,
		CRect(0,0,10,10),this,AAA_EVENT_SIZE_GRIP);
	_sizeGripWnd.SetBitmap(_sizeGripBitmap);
	
	// move the bitmap to the bottom-right corner
	GetClientRect(clientRect);
	_sizeGripWnd.GetWindowRect(bitmapRect);
	ScreenToClient(bitmapRect);
	_sizeGripWnd.SetWindowPos(NULL,clientRect.right-bitmapRect.Width(),
		clientRect.bottom-bitmapRect.Height(), 0,0, SWP_NOZORDER | SWP_NOSIZE);
	
	// make it auto-position
	AddControl(AAA_EVENT_SIZE_GRIP, repos, repos);
	
	_inited = true;
	return TRUE;
}			   

void ResizeDialog::OnSize(UINT nType,int cx,int cy)
{
	CRect rect;
	HDWP hdwp;
	resizeVector::iterator it;
	
	// call the base class
	CBCGPDialog::OnSize(nType,cx,cy);
	
	if(_items.size())
	{
		// get the new size
		GetWindowRect(rect);
		
		// start deferring window pos
		hdwp=BeginDeferWindowPos(20);
		
		// children can resize themselves
		for(it=_items.begin();it!=_items.end();it++)
		{
			it->OnSize(hdwp,_dialogSize,rect,_initDialogSize, this);
		}
		
		// do the deferred window position change
		EndDeferWindowPos(hdwp);
	}
	
	// remember new size
	
	_dialogSize = rect;
}

void ResizeDialog::OnGetMinMaxInfo(MINMAXINFO *lpMMI)
{
	if(_inited)
	{
		lpMMI->ptMinTrackSize=CPoint(_initDialogSize.Width(), _initDialogSize.Height());
		lpMMI->ptMaxTrackSize = CPoint(GetSystemMetrics(SM_CXSCREEN),
									   GetSystemMetrics(SM_CYSCREEN));
	}
}

BOOL ResizeDialog::OnEraseBkgnd(CDC *pDC)
{
	resizeVector::const_iterator it;
	
	for(it=_items.begin();it!=_items.end();it++)
	{
	
		if(it->_flickerFree)
		{
			pDC->ExcludeClipRect(it->_controlRect);
		}
	}
	
	// call the base class
	CBCGPDialog::OnEraseBkgnd(pDC);
	return FALSE;
}

void ResizeDialog::AddControl(const UINT resID, const SizeType xSize, 
							   const SizeType ySize, const bool flickerFree)
{
	ResizeItem item;
	
	// create the new item
	ASSERT(GetDlgItem(resID) != NULL);
	GetDlgItem(resID)->GetWindowRect(item._initControlRect);
	ScreenToClient(item._initControlRect);
	
	item._controlRect = item._initControlRect;
	
	item._flickerFree	= flickerFree;
	item._resID			= resID;
	item._xSizeType		= xSize;
	item._ySizeType		= ySize;
	
	if(xSize == relative)
	{
		item._xRatio=(double)item._controlRect.left/
			((double)_dialogSize.Width()-(double)item._controlRect.left);
	}
	
	if(ySize == relative)
	{
		item._yRatio=(double)item._controlRect.top/
			((double)_dialogSize.Height()-(double)item._controlRect.top);
	}

	
	// add to the array
	_items.push_back(item);
}

LRESULT ResizeDialog::OnNcHitTest(CPoint point) 
{
	//force the grip bitmap to act as if it was actually part 
	//of the window frame. This takes care of both the cursor, 
	//and the functionality.

	CRect gripRect;
	_sizeGripWnd.GetWindowRect(gripRect);

	if (gripRect.PtInRect(point))
	{
		return HTBOTTOMRIGHT;	 
	}
	//for all other areas, use the default processing
	return CBCGPDialog::OnNcHitTest(point);
}

/////////////////////////////////////////////////////////////////////////////
// ResizeItem 

ResizeDialog::ResizeItem::ResizeItem() :
_resID(0),                
_xSizeType(none),             
_ySizeType(none),             
_controlRect(0,0,0,0),           
_flickerFree(false),           
_xRatio(0),                 
_yRatio(0)          
{

}

ResizeDialog::ResizeItem& ResizeDialog::ResizeItem::operator=(const ResizeItem& resizeItem)
{
	_controlRect	= resizeItem._controlRect;
	_resID			= resizeItem._resID;
	_xSizeType		= resizeItem._xSizeType;
	_ySizeType		= resizeItem._ySizeType;
	_flickerFree	= resizeItem._flickerFree;
	_xRatio			= resizeItem._xRatio;
	_yRatio			= resizeItem._yRatio;

	return *this;
}

void ResizeDialog::ResizeItem::OnSize(HDWP hdwp,const CRect& parentOld,
				const CRect& parentNew, CRect & initParent, CWnd *parent)
{
	CSize diff;
	CRect controlRect;
	CWnd *pWnd;
	int   newpos,newsize;
	
	// get the control window
	pWnd = parent->GetDlgItem(_resID);
	
	// get the size difference
	diff.cx = parentNew.Width() - parentOld.Width();
	diff.cy = parentNew.Height() - parentOld.Height();
	
	// preset for no change
	controlRect=_controlRect;
	
	// process horizontal option
	switch(_xSizeType)
	{
    case resize:
		controlRect.right += diff.cx;
		break;
		
    case repos:
		controlRect.left += diff.cx;
		controlRect.right += diff.cx;
		break;
		
    case relative:
		newpos=static_cast<int>((_xRatio*parentNew.Width())/(1.0f + _xRatio));
		newsize=controlRect.Width();
		controlRect.left = newpos;
		controlRect.right = newpos+newsize;
		break;
	
	case repos_center:
		controlRect.left  = _initControlRect.left  + 
			((parentNew.Width() - initParent.Width())/2);
		controlRect.right = _initControlRect.right + 
			((parentNew.Width() - initParent.Width())/2);  
		break;
	}
	
	// process vertical option
	switch(_ySizeType)
	{
    case resize:
		controlRect.bottom += diff.cy;
		break;
		
    case repos:
		controlRect.top += diff.cy;
		controlRect.bottom += diff.cy;
		break;
		
    case relative:
		newpos=static_cast<int>((_yRatio*(double)parentNew.Height())/(1.0+_yRatio));
		newsize=controlRect.Height();
		controlRect.top = newpos;
		controlRect.bottom = newpos+newsize;
		break;
	
	case repos_center:
		controlRect.top    = _initControlRect.top +
			((parentNew.Height() - initParent.Height())/2);
		controlRect.bottom = _initControlRect.bottom + 
			((parentNew.Height() - initParent.Height())/2);	 
		break;
	}
	
	// set new size
	if(controlRect!=_controlRect)
	{
		DeferWindowPos(hdwp,*pWnd,NULL,controlRect.left,controlRect.top,controlRect.Width(),controlRect.Height(),SWP_NOZORDER);
		_controlRect = controlRect;
	}
}
