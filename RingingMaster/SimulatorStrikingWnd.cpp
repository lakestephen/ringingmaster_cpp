// SimulatorStrikingWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorStrikingWnd.h" 
#include "MemDC.h"
#include "SimulatorManager.h"
#include "StrikingItem.h"
#include "StrikingRow.h"
#include ".\simulatorstrikingwnd.h"

// SimulatorStrikingWnd

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(SimulatorStrikingWnd, CWnd)
SimulatorStrikingWnd::SimulatorStrikingWnd(SimulatorStrikingWndType type) :
_type(type)
{
	simulatorManager()->addEventListener(this);
}

SimulatorStrikingWnd::~SimulatorStrikingWnd()
{				
	simulatorManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(SimulatorStrikingWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


BOOL SimulatorStrikingWnd::Create( CWnd* pParentWnd, UINT nID)
{
	DWORD dwExStyle = NULL;

	BOOL ret = CWnd::CreateEx(dwExStyle, AfxRegisterWndClass(0, 0, 0, 0), 
		"SimulatorStrikingWnd", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), pParentWnd, nID, NULL);	

	return ret;
}



void SimulatorStrikingWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	MemDC memdc(&dc);
	CRect rect;
	GetClientRect(rect);
	memdc.FillSolidRect(rect, ::GetSysColor (COLOR_WINDOW));
	memdc.SetBkMode (TRANSPARENT);
	OnDraw(&memdc, rect);
}

void SimulatorStrikingWnd::OnDraw(CDC* pDC, const CRect& rect)
{
	switch (_type)
	{
	case sswt_all:
		drawAll(pDC, rect);
		break;
	case sswt_single_1:
	case sswt_single_2:
		drawSingle(pDC, rect);

	};
}

void SimulatorStrikingWnd::drawAll(CDC* pDC, const CRect& rect)
{
	const StrikingData& strikingData = simulatorManager()->getStrikingData();

	CSingleLock lock(&strikingData.getCriticalSection(), TRUE);
	StrikingRow* latestRow;
	StrikingRow* latestButOneRow;
	strikingData.getLatestRow(latestRow, latestButOneRow);

	CRect latestButOneRect(rect);
	CRect latestRect(rect);
	latestRect.top = rect.bottom/3;
	latestButOneRect.bottom = rect.bottom/3;

	//first draw the colors	 
	drawColorBlocks(latestButOneRow, latestButOneRect, pDC, simulatorManager()->getNumber());
	drawColorBlocks(latestRow, latestRect, pDC, simulatorManager()->getNumber());

	//draw the text shadows
	drawText(latestButOneRow, latestButOneRect, pDC, simulatorManager()->getNumber(), true);
	drawText(latestRow, latestRect, pDC, simulatorManager()->getNumber(), true);

	//draw the perfect position lines
	drawLines(latestRect, pDC, simulatorManager()->getNumber());

	//draw the text
	drawText(latestButOneRow, latestButOneRect, pDC, simulatorManager()->getNumber(), false);
	drawText(latestRow, latestRect, pDC, simulatorManager()->getNumber(), false);
}

void SimulatorStrikingWnd::drawSingle(CDC* pDC, const CRect& rect)
{
	int bell = simulatorManager()->getSimulatorStrikingSngleBell(_type);

	if (bell > simulatorManager()->getNumber())
		bell = simulatorManager()->getNumber();

	const StrikingData& strikingData = simulatorManager()->getStrikingData();

	CSingleLock lock(&strikingData.getCriticalSection(), TRUE);
	StrikingRow* latestRow;
	StrikingRow* latestButOneRow;
	strikingData.getLatestRow(latestRow, latestButOneRow);

	//first draw the colors	 - drae the latest but one first in case there is nothing in the latest.
	if (!hasRealStrikeItem(latestRow, bell))
		latestRow = latestButOneRow;

	drawSingleColorBlock(latestRow, rect, pDC, simulatorManager()->getNumber(), bell);

	//draw the text shadows
	drawText(latestRow, rect, pDC,0, true, bell);

	drawLines(rect, pDC, -1);

	//draw the text
	drawText(latestRow, rect, pDC, 0, false, bell);
}

void SimulatorStrikingWnd::drawLines(const CRect& rect, CDC* pDC, int number)
{
	if (true) // to2do
	{
		CPen pen(PS_SOLID, 1, RGB(190,190,190));
		CPen * oldPen = pDC->SelectObject(&pen);

		if (number != -1)
		{
			double interval = (double)rect.right / (double)(number+1);
			for (int i=0;i<number;i++)
			{
				int x = (int)(interval*(double)(i+1));
				pDC->MoveTo(x, 0);
				pDC->LineTo(x, rect.bottom);
			}
		
			pDC->MoveTo(0,rect.bottom/3);
			pDC->LineTo(rect.right, rect.bottom/3);
		}
		else
		{
			int x = rect.left + (rect.Width()/2);
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, rect.bottom);

		}

		pDC->SelectObject(oldPen);
	}
}

bool SimulatorStrikingWnd::hasRealStrikeItem(StrikingRow* latestRow, int bell)
{
	if (latestRow)
	{
		POSITION pos = latestRow->GetHeadPosition();
		StrikingItem* realStrikeItem = NULL;

		while(pos) 
		{
			realStrikeItem  = latestRow->GetNext(pos);
			if (realStrikeItem->getBell() == bell)
			{
				return true	;
			}
		}
	}
	return false;
}

bool SimulatorStrikingWnd::drawSingleColorBlock(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number, int bell)
{
	pDC->FillSolidRect(rect, GetSysColor(COLOR_WINDOW));				

	if (latestRow)
	{
		//iterate through all the places
		for (int place=0;place<number;place++)
		{
			if (bell == latestRow->getPerfectTimeBell(place).getBell())
			{
				drawColorBlock(latestRow, rect, pDC, bell, place);
				return true;
			}
		}
	}	
	return false;
}

void SimulatorStrikingWnd::drawColorBlocks(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number)
{
	pDC->FillSolidRect(rect, GetSysColor(COLOR_WINDOW));				

	if (latestRow)
	{
		//iterate through all the places
		for (int place=0;place<number;place++)
		{
			int bell = latestRow->getPerfectTimeBell(place).getBell();

			//calculate the rect//to2do use place or bell - configurable
			int i = place;

			CRect thisPlaceRect(rect);
			double gap = ((double)rect.right / (double)(number+1));
			thisPlaceRect.left = (int)(gap * (i + 0.5));
			thisPlaceRect.right= (int)(gap * (i + 1.5));
		
			drawColorBlock(latestRow, thisPlaceRect, pDC, bell, place);
		}
	}	 
}


void SimulatorStrikingWnd::drawColorBlock(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int bell, int place)
{
	//go through the strike items to get the percents
	COLORREF color = GetSysColor(COLOR_WINDOW);
	double percent = 0.0;

	POSITION pos = latestRow->GetHeadPosition();
	StrikingItem* realStrikeItem = NULL;
	while(pos) 
	{
		realStrikeItem  = latestRow->GetNext(pos);
		if (realStrikeItem->getBell() == bell)
		{
			double temp = latestRow->getPercentOut(realStrikeItem, place);

			if (abs(temp) > abs(percent))
				percent = temp;			
		}
	}

	//now calculate the color from the percents
	if (percent != 0.0)
	{
		int val = (int)(255.0*(1.0-min(abs(percent)*2.0,1.0)));

		ASSERT(val >=0 && val < 255);
		if (percent  <0.0)
		{
			color = RGB(val,255, val);
		}
		else if (percent > 0.0)
		{
			color = RGB(val, val, 255);
		}
	}        

	pDC->FillSolidRect(rect, color);
}

void SimulatorStrikingWnd::drawText(StrikingRow* latestRow, const CRect& rect, CDC* pDC, int number, bool shadow, int bell)
{
	//when bell is -1 draw all

	if (latestRow)
	{
		//to2do get from some style object

		FontInfo fontInfo;
		fontInfo._size = max(min((rect.Width()/(number+1))*10, rect.Height()*6), 60);//60% of the height

		LOGFONT lf;
		fontInfo.getLogFont(&lf);
		CFont font;
		font.CreateFontIndirect(&lf);
		CFont* oldFont = pDC->SelectObject(&font);
		pDC->SetTextColor(shadow?::GetSysColor(COLOR_WINDOW):RGB(0,0,0));

		//draw all the bells
		POSITION pos = latestRow->GetHeadPosition();
		StrikingItem* realStrikeItem = NULL;

		CSize szText = pDC->GetTextExtent("W");
		int y = rect.top + (rect.Height()/2) - (szText.cy/2);                      

		if (bell == -1)
		{
			CString stroke = ((latestRow->getStroke() == hand)?"h":"b");
			pDC->DrawText(stroke, CRect(0, y, szText.cx, y + szText.cy),
				DT_LEFT);
		}

		while(pos) 
		{
			realStrikeItem  = (StrikingItem *) latestRow->GetNext(pos);

			if ((!realStrikeItem->isComment())&&
				(bell == -1 || bell == realStrikeItem->getBell()))
			{

				int place = latestRow->getPlaceFromBell(realStrikeItem->getBell());
				if (place >= 0)
				{
					CString str(GlobalFunctions::bellNumbersToChar(realStrikeItem->getBell()+1));
					double percent = shadow? 0:latestRow->getPercentOut(realStrikeItem, place);
					//shadow
					int x = 0;
					if (bell == -1)
						x = (int)((double)((double)(place+1.0+percent)/(double)(number+1)) * rect.right) - (szText.cx/2);
					else
						x = (int)((percent * (double)rect.right)) - (szText.cx/2) + (rect.Width()/2);

					pDC->DrawText(str, CRect(x, y, x + szText.cx, y + szText.cy),
						DT_CENTER| DT_VCENTER);
				}
			}
		}

		pDC->SelectObject(oldFont);
	}	
}

BOOL SimulatorStrikingWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	//no erasing is necessary as the paint function is using memDC, and is 
	//painting the whole background
	return TRUE; 
}
void SimulatorStrikingWnd::simulatorManager_notifyBellStateChangeFromSeperateThread(int /*bell*/)
{
	Invalidate();
}


void SimulatorStrikingWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	Invalidate();
}

void SimulatorStrikingWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	UINT menuID = 0;
	//to2do

	switch (_type)
	{
	case sswt_all:
		break;
	case sswt_single_1:
		break;
	case sswt_single_2:
		break;
	};


	if (menuID !=0)
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
								
		CMenu menu;
		VERIFY(menu.LoadMenu (menuID));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(mainFrame(), point.x, point.y, pPopup->Detach (), FALSE, TRUE);		
	}
}

SimulatorStrikingWndType SimulatorStrikingWnd::getType()
{
	return _type;
}
