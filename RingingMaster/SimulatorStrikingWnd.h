#pragma once

#include "SimulatorManagerEventListener.h"
class StrikingRow;


enum SimulatorStrikingWndType
{
	sswt_all, 
	sswt_single_1,
	sswt_single_2,
};

class SimulatorStrikingWnd : public CWnd, SimulatorManagerEventListener
{
	DECLARE_DYNAMIC(SimulatorStrikingWnd)

public:
	SimulatorStrikingWnd(SimulatorStrikingWndType type);
	virtual ~SimulatorStrikingWnd();

protected:
	void drawText(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number, bool shadow, int bell = -1);
	bool hasRealStrikeItem(StrikingRow* latestRow, int bell);
	bool drawSingleColorBlock(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number, int bell);
	void drawColorBlocks(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number);
	void drawColorBlock(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int bell, int place);
	void drawLines(const CRect& rect, CDC* pDC, int number);

	SimulatorStrikingWndType _type;

	void OnDraw(CDC* pDC, const CRect& rect);
	
	void drawAll(CDC* pDC, const CRect& rect);
	void drawSingle(CDC* pDC, const CRect& rect);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);

	SimulatorStrikingWndType getType();

	afx_msg void OnPaint();

	void simulatorManager_notifyBellStateChangeFromSeperateThread(int bell);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


