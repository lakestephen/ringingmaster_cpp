#include "stdafx.h"
#include ".\bcgpnosizeminiframewnd.h"


IMPLEMENT_SERIAL(CBCGPNoSizeMiniFrameWnd, CBCGPMiniFrameWnd,1)

CBCGPNoSizeMiniFrameWnd::CBCGPNoSizeMiniFrameWnd():
_width(10), 
_height(10)
{

}


CBCGPNoSizeMiniFrameWnd::~CBCGPNoSizeMiniFrameWnd(void)
{
}


BEGIN_MESSAGE_MAP(CBCGPNoSizeMiniFrameWnd, CBCGPMiniFrameWnd)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

void CBCGPNoSizeMiniFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	lpwndpos->cx = _width;   
	lpwndpos->cy = min(lpwndpos->cy, _height);

    CBCGPMiniFrameWnd::OnWindowPosChanging(lpwndpos);
}

IMPLEMENT_SERIAL(PlayPositionNoSizeMiniFrameWnd, CBCGPNoSizeMiniFrameWnd,1)

PlayPositionNoSizeMiniFrameWnd::PlayPositionNoSizeMiniFrameWnd()
{
	_width = 100;
	_height = 110;
}


IMPLEMENT_SERIAL(StrikingControlsNoSizeMiniFrameWnd, CBCGPNoSizeMiniFrameWnd,1)

StrikingControlsNoSizeMiniFrameWnd::StrikingControlsNoSizeMiniFrameWnd()
{
	_width = 290;
	_height = 175;
}
