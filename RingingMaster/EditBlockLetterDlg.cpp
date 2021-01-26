// EditBlockLetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "EditBlockLetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// EditBlockLetterDlg dialog


EditBlockLetterDlg::EditBlockLetterDlg(CPoint point, CString letter, TouchBlockArray& touchBlocks,
									   int startLine, int endLine, int min, int max)
	: CDialog(EditBlockLetterDlg::IDD, NULL),
	_originalLetter(letter),
	_touchBlocks(touchBlocks),
	_point(point),
	_max(max),
	_min(min)
{

	if (min>max)
		max = min;

	//{{AFX_DATA_INIT(EditBlockLetterDlg)
	_blockLetter = _T(letter);
	_top = startLine;
	_bottom = endLine;
	//}}AFX_DATA_INIT
}


void EditBlockLetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditBlockLetterDlg)
	DDX_Control(pDX, IDC_TOP_SPIN, _topSpin);
	DDX_Control(pDX, IDC_BOTTOM_SPIN, _bottomSpin);
	DDX_Text(pDX, IDC_BLOCK_LETTER, _blockLetter);
	DDX_Text(pDX, IDC_TOP, _top);
	DDX_Text(pDX, IDC_BOTTOM, _bottom);
	//}}AFX_DATA_MAP
	DDV_MinMaxInt(pDX, _top, _min, _max);
	DDV_MinMaxInt(pDX, _bottom, _min, _max);
}


BEGIN_MESSAGE_MAP(EditBlockLetterDlg, CDialog)
	//{{AFX_MSG_MAP(EditBlockLetterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditBlockLetterDlg message handlers


void EditBlockLetterDlg::OnOK() 
{
	//verification
	UpdateData();
	

	if (_blockLetter.GetLength() < 1)
	{
		AfxMessageBox("The block name must be at least one character long");
		return;
	}
	
	CString msg;
	if (GlobalFunctions::isIligalChar(_blockLetter,msg))
	{
		msg = "Block name " + msg;
		AfxMessageBox(msg);
		return;
	}

	if (_blockLetter != _originalLetter)
	{
		for (int i=0;i<_touchBlocks.GetSize();i++)
		{
			if (_touchBlocks.GetAt(i)->_name == _blockLetter)
			{
				CString str;
				str.Format("There is already a block \'%s\' defined", _blockLetter);
				AfxMessageBox(str);
				return;
			}
		}
	}

	if (_top>_bottom)
	{
		AfxMessageBox("The bottom value must be equal to or greater than the top value");
		return;
	}

	CDialog::OnOK();
}

BOOL EditBlockLetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect thisRect;
	GetWindowRect(&thisRect);
	thisRect = CRect(_point.x,
					 _point.y,
					 _point.x+thisRect.Width(),
					 _point.y+thisRect.Height());

	CRect screen;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &screen, SPIF_UPDATEINIFILE);
	
	CPoint scrOffset;
	scrOffset.x = screen.right - thisRect.right - 8;
	scrOffset.y = screen.bottom - thisRect.bottom - 8;

	if (scrOffset.x > 0) scrOffset.x = 0;
	if (scrOffset.y > 0) scrOffset.y = 0;

	thisRect.OffsetRect(scrOffset);

	MoveWindow(&thisRect);

	_topSpin.SetRange32(_min, _max);
	_bottomSpin.SetRange32(_min, _max);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}
