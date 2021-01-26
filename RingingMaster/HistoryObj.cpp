// HistoryObj.cpp: implementation of the HistoryObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "HistoryObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HistoryObj::HistoryObj(const CString& strTitle, const CString& strURL,
						const UINT uiCommand) :
	m_strTitle (strTitle),
	m_strURL (strURL),
	m_uiCommand (uiCommand)
{
}


HistoryObj::~HistoryObj()
{

}
