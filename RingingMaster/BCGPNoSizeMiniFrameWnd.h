#pragma once


class CBCGPNoSizeMiniFrameWnd :
	public CBCGPMiniFrameWnd
{

	DECLARE_SERIAL(CBCGPNoSizeMiniFrameWnd)

public:
	CBCGPNoSizeMiniFrameWnd();
	~CBCGPNoSizeMiniFrameWnd(void);


	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	
	
	DECLARE_MESSAGE_MAP()

protected:
	int _width;
	int _height;

};

class PlayPositionNoSizeMiniFrameWnd :
	public CBCGPNoSizeMiniFrameWnd
{
	PlayPositionNoSizeMiniFrameWnd();

	DECLARE_SERIAL(PlayPositionNoSizeMiniFrameWnd)


};

class StrikingControlsNoSizeMiniFrameWnd :
	public CBCGPNoSizeMiniFrameWnd
{
	StrikingControlsNoSizeMiniFrameWnd();

	DECLARE_SERIAL(StrikingControlsNoSizeMiniFrameWnd)


};


