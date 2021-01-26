// ImportLog.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "ImportLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImportLog dialog


ImportLog::ImportLog(CStringArray& stringArr, CWnd* pParent)
	: ResizeDialog(ImportLog::IDD, pParent),
	_stringArr(stringArr)
{
	//{{AFX_DATA_INIT(ImportLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ImportLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ImportLog)
	DDX_Control(pDX, IDC_LIST_LOG, _log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ImportLog, ResizeDialog)
	//{{AFX_MSG_MAP(ImportLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ImportLog message handlers

BOOL ImportLog::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	AddControl(IDC_LIST_LOG, resize, resize);

	_log.SetHorizontalExtent(1000);
	for (int i=0;i<_stringArr.GetSize();i++)
		_log.AddString(_stringArr.GetAt(i));
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void ImportLog::addString(CString str)
{
	

}
