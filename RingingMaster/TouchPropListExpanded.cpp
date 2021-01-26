#include "StdAfx.h"
#include ".\touchproplistexpanded.h"

TouchPropListExpanded::TouchPropListExpanded(void) :
_focusType(ptt_setupRoot),
_scrollPos(0),
_columnPos(-1), 
_mainSetup(TRUE),
_mainSetupAdvanced(FALSE),
_mainStart(FALSE),
_mainEnd(FALSE),
_mainMethod(FALSE),
_mainDefinition(FALSE),
_mainMusic(FALSE),
_mainStyle(FALSE),
_mainNotes(FALSE), 
_loaded(FALSE)
{
}

TouchPropListExpanded::~TouchPropListExpanded(void)
{
}
