// RowChooserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "RowChooserDlg.h"
#include "Row.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RowChooserDlg dialog


RowChooserDlg::RowChooserDlg(CWnd* pParent, CString title, int number, CString originalRow, bool forceTenor)
	: CDialog(RowChooserDlg::IDD, pParent),
	_originalRow(originalRow), 
	_title(title),
	_number(number),
	_pParent(pParent),
	_forceTenor(forceTenor)

{
	//{{AFX_DATA_INIT(RowChooserDlg)
	_rounds = FALSE;
	//}}AFX_DATA_INIT

	if (_originalRow.IsEmpty())
	{
		Row row(_number);
		_rowStr = row.getAsText();
	}
	else
	{
		_rowStr = _originalRow;
	}
}


void RowChooserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RowChooserDlg)
	DDX_Control(pDX, IDC_ROW_EDIT, _rowEdit);
	DDX_Control(pDX, IDC_ROW_MSG, _message);
	DDX_Text(pDX, IDC_ROW_EDIT, _rowStr);
	DDX_Check(pDX, IDC_ROUNDS, _rounds);
	DDV_MaxChars(pDX, _rowStr, 30);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(RowChooserDlg, CDialog)
	//{{AFX_MSG_MAP(RowChooserDlg)
	ON_EN_CHANGE(IDC_ROW_EDIT, OnChangeRowEdit)
	ON_BN_CLICKED(IDC_ROUNDS, OnRounds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RowChooserDlg message handlers

void RowChooserDlg::OnChangeRowEdit() 
{
	UpdateData();

	Row row(_number);
	
	CString msg;

	bool valid = row.setAsText(_rowStr, msg, _forceTenor);

	_message.SetWindowText(valid?"":msg);
}

BOOL RowChooserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(_title);

	Row row(_number);

	if (row.getAsText() == _rowStr)
	{
		_rounds = TRUE;
		UpdateData(FALSE);
	} 

	OnChangeRowEdit();

	OnRounds();


	if (_pParent)
	{
		CRect parentRect, thisRect;
		_pParent->GetWindowRect(&parentRect);
		GetWindowRect(&thisRect);
		CRect newRect(parentRect.left + 10, 
						parentRect.top + 15, 
						parentRect.left + 10 + thisRect.Width(),
						parentRect.top + 15 + thisRect.Height());

		CRect screen;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &screen, SPIF_UPDATEINIFILE);
		
		CPoint scrOffset;
		scrOffset.x = screen.right - newRect.right - 8;
		scrOffset.y = screen.bottom - newRect.bottom - 8;

		if (scrOffset.x > 0) scrOffset.x = 0;
		if (scrOffset.y > 0) scrOffset.y = 0;

		newRect.OffsetRect(scrOffset);

		MoveWindow(&newRect);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void RowChooserDlg::OnOK() 
{

	UpdateData();
	Row row(_number);
	CString msg;
	bool valid = row.setAsText(_rowStr, msg, _forceTenor);

	if (valid)
	{
		_originalRow = row.getAsText();
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("Please enter a valid row. \n" + msg);
	}						
}

void RowChooserDlg::OnRounds() 
{
	UpdateData();
	_rowEdit.EnableWindow(!_rounds);	
	_message.EnableWindow(!_rounds);	
	if (_rounds)
	{
		Row row(_number);
		_rowStr = row.getAsText();
		UpdateData(FALSE);
		OnChangeRowEdit();
	}
}
