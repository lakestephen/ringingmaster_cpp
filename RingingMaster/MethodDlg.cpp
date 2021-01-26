// MethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodDlg.h"
#include "MethodPreview.h"
#include "MethodSelectRow.h"
#include "MethodSelectLeadLine.h"
#include "MethodPreviewWnd.h"
#include "MethodSelectRowWnd.h"
#include "UserMessages.h"
#include "MethodSelectLeadLineWnd.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MethodDlg dialog


MethodDlg::MethodDlg(CWnd* pParent, Method* method,CString token):
ResizeDialog(MethodDlg::IDD, pParent),
_method(method),
_methodWnd(NULL),
_token(token)
{
	//{{AFX_DATA_INIT(MethodDlg)
	_bell = _T("");
	_lead = _T("");
	_place = _T("");
	_row = _T("");
	_message = _T("");
	//}}AFX_DATA_INIT
}


MethodDlg::~MethodDlg()
{
	delete _method;
	_method = NULL;
	delete _methodWnd; 
	_methodWnd = NULL; 
}

void MethodDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MethodDlg)
	DDX_Text(pDX, IDC_BELL_STATIC, _bell);
	DDX_Text(pDX, IDC_LEAD_STATIC, _lead);
	DDX_Text(pDX, IDC_PLACE_STATIC, _place);
	DDX_Text(pDX, IDC_ROW_STATIC, _row);
	DDX_Text(pDX, IDC_MES, _message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MethodDlg, ResizeDialog)
	//{{AFX_MSG_MAP(MethodDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_METHOD_WND_TERMINATE, OnMethodWndTerminate)
END_MESSAGE_MAP()	 


/////////////////////////////////////////////////////////////////////////////
// MethodDlg message handlers

BOOL MethodDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
	
	//get the position of the place holder static item
	CRect drawRect;
	GetDlgItem(IDC_METHOD_WND_HOLD)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);
/* todo
	MethodPreview* mPrev = dynamic_cast<MethodPreview*>(_method);
	MethodSelectLeadLine* mSelLeadLine = dynamic_cast<MethodSelectLeadLine*>(_method);
	MethodSelectRow* mSelRow = dynamic_cast<MethodSelectRow*>(_method);

	if (mPrev)
	{
		//create the method window
		MethodPreviewWnd * mPrevWnd = new MethodPreviewWnd(); 
		_methodWnd = mPrevWnd;
		
		mPrevWnd->Create(WS_EX_CLIENTEDGE, "Method Wnd", WS_TABSTOP, drawRect,		
								this, AAA_METHOD_WND, mPrev);
	}		
	else if (mSelLeadLine)
	{
		//create the method window
		MethodSelectLeadLineWnd * mSelLeadWnd = new MethodSelectLeadLineWnd(); 
		_methodWnd = mSelLeadWnd;

		mSelLeadWnd->Create(WS_EX_CLIENTEDGE, "Method Wnd", WS_TABSTOP, drawRect,		
								this, AAA_METHOD_WND, mSelLeadLine);
	}	
	else if (mSelRow)
	{
		//create the method window
		MethodSelectRowWnd * mSelRowWnd = new MethodSelectRowWnd(); 
		_methodWnd = mSelRowWnd;

		mSelRowWnd->Create(WS_EX_CLIENTEDGE, "Method Wnd", WS_TABSTOP, drawRect,		
								this, AAA_METHOD_WND, mSelRow);
	}
	else
	{
		ASSERT(FALSE);
	}
	
	SetWindowText(_method->getCurNot()->getName(true));

	
	_methodWnd->SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	AddControl(AAA_METHOD_WND, resize, resize);
	AddControl(IDC_PLACE_STATIC, repos, repos);	
	AddControl(IDC_ROW_STATIC, repos, repos);	
	AddControl(IDC_LEAD_STATIC, repos, repos);	
	AddControl(IDC_BELL_STATIC, repos, repos);	
	AddControl(IDC_MES, resize, repos);	

	WinPos::Load(this, "MethodDlg:" + _token, true);
						*/
	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL MethodDlg::DestroyWindow() 
{
	WinPos::Save(this,  "MethodDlg:" + _token);
	
	return ResizeDialog::DestroyWindow();
}



LRESULT MethodDlg::OnSetMethodPos(WPARAM wParam, LPARAM lParam)
{
	MethodPosition * pos = (MethodPosition*)lParam;	   
	if (wParam == TRUE)
	{

		_lead  = pos->format(rpft_lead);
		_row   = pos->format(rpft_rowDisplay);
 		_place = pos->format(rpft_mp_place);
		_bell  = pos->format(rpft_mp_bell);
	}
	else 
	{
		_place.Empty();
		_row.Empty();
		_lead.Empty();
		_bell.Empty();
	}

	UpdateData(FALSE);

	delete pos;
	return 0;
}

LRESULT MethodDlg::OnSetMessageString(WPARAM wParam, LPARAM /*lParam*/)
{	
	
	UINT    nIDMsg = (UINT)wParam;
	CString strMsg;

	if (nIDMsg)
	{
		if (strMsg.LoadString(nIDMsg) != 0)
		{
			_message = strMsg;
			UpdateData(FALSE);
		}
		else
			TRACE1("Warning: no message line prompt for ID %x%04X\n", nIDMsg);
	}
	return 0;
}

LRESULT MethodDlg::OnMethodWndTerminate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	EndDialog(IDOK);
	return 0;
}



