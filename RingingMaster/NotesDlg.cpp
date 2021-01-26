// NotesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotesDlg.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotesDlg dialog


NotesDlg::NotesDlg(CWnd* pParent, CString notes, CPoint requestPosition)
	: ResizeDialog(NotesDlg::IDD, pParent),
	_requestPosition(requestPosition)
{
	//{{AFX_DATA_INIT(NotesDlg)
	//}}AFX_DATA_INIT
	_notes = notes;
}


void NotesDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotesDlg)
	DDX_Text(pDX, IDC_NOTES, _notes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotesDlg, ResizeDialog)
	//{{AFX_MSG_MAP(NotesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotesDlg message handlers

BOOL NotesDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	AddControl(IDC_NOTES, resize, resize);	

	WinPos::Load(this, "NotesDlg", true);

	
	if ((_requestPosition.x != -1)&&(_requestPosition.y != -1))
		GlobalFunctions::negotiateSpace(this, _requestPosition); 

	return TRUE;  // return TRUE unless you set the focus to a control

}



BOOL NotesDlg::DestroyWindow() 
{
	WinPos::Save(this, "NotesDlg");
	
	return ResizeDialog::DestroyWindow();
}

void NotesDlg::OnCancel() 
{
	UpdateData();
	
	ResizeDialog::OnCancel();
}
