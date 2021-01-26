#pragma once


#include "MainFrameEventListener.h"

class PropListBase :
	public CBCGPPropList, public MainFrameEventListener
{
public:
	PropListBase(void);
	~PropListBase(void);

	virtual void initPropList();

	virtual void loadExpandStatus() = 0;
	virtual void saveExpandStatus() = 0;
	virtual void mainFrame_notifyDocViewChanged(ViewType oldViewType, ViewType newViewType, CDocument* oldDoc, CDocument* newDoc);
	void update();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
