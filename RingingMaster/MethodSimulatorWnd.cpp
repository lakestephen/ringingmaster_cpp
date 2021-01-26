// MethodSimulatorWnd.cpp: implementation of the MethodSimulatorWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MethodSimulatorWnd.h"
#include "SimulatorManager.h"
#include "Lead.h"
#include "StyleEditDlg.h"
#include "MethodPosition.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodSimulatorWnd::MethodSimulatorWnd() :
MethodWnd(ds_fromSimulatorManager), 
_latestPlace(-1), 
_latestRowIndex(-1)
{
	simulatorManager()->addEventListener(this);
}

MethodSimulatorWnd::~MethodSimulatorWnd()
{
	simulatorManager()->setDefaultBlueLineStyle(_style);

	simulatorManager()->removeEventListener(this);
}     


BEGIN_MESSAGE_MAP(MethodSimulatorWnd, MethodWnd)
	ON_MESSAGE(SIMULATOR_METHOD_STRUCTURE_CHANGED, OnSimulatorMethodStructureChanged)
	ON_COMMAND(ID_SIMULATOR_OPTIONS, OnSimulatorOptions)
END_MESSAGE_MAP()


void MethodSimulatorWnd::OnSimulatorOptions() 
{
	mainFrame()->PostMessage(WM_COMMAND, ID_SIMULATOR_OPTIONS); 
}

void MethodSimulatorWnd::simulatorManager_notifyNewMethod(Method* method)
{
	_latestPlace = -1;
	_latestRowIndex = -1;
	setMethod(method);
}

void MethodSimulatorWnd::simulatorManager_notifyMethodStructureChangedFromSeperateThread()
{
	//do not want to do this on the same thread.
	PostMessage(SIMULATOR_METHOD_STRUCTURE_CHANGED, 0,0);
}

LRESULT MethodSimulatorWnd::OnSimulatorMethodStructureChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
{	/* todo		
	recalcLeadOrigins();
	recalcLeadBoxes();	
	SetScrolls();

	//notify the parent
	if (_method && _method->GetSize() > 0 && 
		_method->GetAt(_method->GetUpperBound())->GetUpperBound() > 0)
	{
		MethodPosition* methodPosition = new MethodPosition();
		Lead* lead = _method->GetAt(_method->GetUpperBound());
		methodPosition->setRow(_method, _method->GetUpperBound(), 
									lead->GetUpperBound(), false);
		methodPosition->setStroke(lead->GetAt(lead->GetUpperBound())->getStroke());
		::PostMessage(GetParent()->m_hWnd, WM_SET_METHOD_POS, TRUE, (LPARAM)methodPosition);
	}
	*/

	return 0;
}

void MethodSimulatorWnd::simulatorManager_notifyLatestPositionFromSeperateThread(int rowIndex, int place)
{
	_latestRowIndex = rowIndex;
	_latestPlace = place;

	if (place == 0)
	{	//now scroll
		RowPosition rowPos;
		rowPos.setRow(_method, rowIndex );
		scrollToRow(rowPos, true);
	}
	Invalidate();
}

UINT MethodSimulatorWnd::getContectMenuId()
{ 
	return IDR_METHOD_STYLE_SIMULATOR;
}

ShouldDrawType MethodSimulatorWnd::shouldDrawRow(int rowIndex, int place)
{
	if (rowIndex < _latestRowIndex)
		return sdt_yes;
	else if (rowIndex == _latestRowIndex)
	{
		if (place <= _latestPlace) 
			return sdt_yes;
		else return sdt_grey;
	}
	else
	{
		//greater than _latestPlace;
		if ((simulatorManager()->getSimulatorStateType() == sst_connectedSilent) ||
			(simulatorManager()->getSimulatorStateType() == sst_disconnected))
			return sdt_no;
		else
			return sdt_grey;
	}                       	
}

CSize MethodSimulatorWnd::getLeadOrigin(int leadNo)
{
	if (leadNo > _leadOrigins.GetUpperBound())
	{
		recalcLeadOrigins();
		recalcLeadBoxes();	
	}
	ASSERT(leadNo <= _leadOrigins.GetUpperBound());

	return _leadOrigins.GetAt(leadNo);
}

const CRect& MethodSimulatorWnd::getLeadBox(int leadNo)
{						
	if (leadNo > _leadBoxs.GetUpperBound())
	{
		recalcLeadOrigins();
		recalcLeadBoxes();	
	}

	ASSERT(leadNo <= _leadBoxs.GetUpperBound());

	return _leadBoxs.GetAt(leadNo);
}

void MethodSimulatorWnd::doToolsStyle() 
{
	StyleEditDlg styleEditDlg(_style, this, _method->getNumber(), slt_simulator);
	styleEditDlg.DoModal();
	notifyStyleChange();
	SetScrolls();
}
