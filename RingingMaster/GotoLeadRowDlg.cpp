//todo change this class to refelect the reality of the new structure of Method

#include "stdafx.h"
#include "RingingMaster.h"
#include "GotoLeadRowDlg.h"
#include "Method.h"
#include "Lead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GotoLeadRowDlg dialog

int GotoLeadRowDlg::_lead = 1;
int GotoLeadRowDlg::_row  = 1;


GotoLeadRowDlg::GotoLeadRowDlg(CWnd* pParent, Method* method)
	: CDialog(GotoLeadRowDlg::IDD, pParent),
	_method(method),
	_inited(false)
{
	//{{AFX_DATA_INIT(GotoLeadRowDlg)
	//}}AFX_DATA_INIT
}


void GotoLeadRowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GotoLeadRowDlg)
	DDX_Control(pDX, IDC_LEAD, _leadCtrl);
	DDX_Control(pDX, IDC_ROW_SPIN, _rowSpin);
	DDX_Control(pDX, IDC_LEAD_SPIN, _leadSpin);
	DDX_Text(pDX, IDC_LEAD, _lead);
	DDX_Text(pDX, IDC_ROW, _row);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _lead, 0, _method->getLeadCount());
}


BEGIN_MESSAGE_MAP(GotoLeadRowDlg, CDialog)
	//{{AFX_MSG_MAP(GotoLeadRowDlg)
	ON_EN_CHANGE(IDC_LEAD, OnChangeLead)
	ON_EN_CHANGE(IDC_ROW, OnChangeRow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GotoLeadRowDlg message handlers

BOOL GotoLeadRowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_leadSpin.SetRange32(1,_method->getLeadCount());
	verify();
	_leadCtrl.SetFocus();

	_inited = true;

	return FALSE;
}






void GotoLeadRowDlg::OnChangeLead() 
{
	if(_inited)
	{
		UpdateData(TRUE);
		
		verify();
	}
}

void GotoLeadRowDlg::verify()	 
{
	bool needUpdate = false;
			  /*
	if ((_lead <= 0)||(_lead > _method->GetSize()))
	{
		_lead = 1;
		needUpdate = true;
	}

	if ((_lead >0)&&(_lead <= _method->GetSize()))
	{
		int maxRowsInLead = _method->GetAt(_lead-1)->GetUpperBound();

		//the last lead can have 1 more row
		if (_lead == _method->getLeadCount())
			maxRowsInLead++;
		
		_rowSpin.SetRange32(1,maxRowsInLead);

		if (_row <= 0)
		{
			_row = 0;
			needUpdate = true;
		}		  
		else if (_row >= maxRowsInLead)
		{
			_row = maxRowsInLead;
			needUpdate = true;
		}
	}
					*/ //todo
	if (needUpdate)
		UpdateData(FALSE);

}

void GotoLeadRowDlg::OnChangeRow() 
{
	if(_inited)
	{
		UpdateData();
		verify();
	}
}


