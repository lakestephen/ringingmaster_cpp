#include "StdAfx.h"
#include ".\MatrixPropListExpanded.h"

MatrixPropListExpanded::MatrixPropListExpanded(void) :
_focusType(ptm_userRoot), 
_scrollPos(0),
_columnPos(-1),
_users(TRUE),
_filters(TRUE),
_loaded(FALSE)
{
}

MatrixPropListExpanded::~MatrixPropListExpanded(void)
{
}
