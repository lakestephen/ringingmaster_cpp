#include "StdAfx.h"
#include ".\simulatorguibell.h"

SimulatorGUIBell::SimulatorGUIBell(void)
{
}

SimulatorGUIBell::~SimulatorGUIBell(void)
{
}

void SimulatorGUIBell::setBellRect(const CRect& rect)
{
	_bellRect = rect;
}

const CRect& SimulatorGUIBell::getBellRect() const
{
	return _bellRect;
}

void SimulatorGUIBell::setText(CString text, bool leftAlign)
{
	_text = text;
	_leftAlign = leftAlign;
}

bool SimulatorGUIBell::isLeftAlign() const
{
	return _leftAlign;
}

const CString& SimulatorGUIBell::getText() const
{
	return _text;
}
