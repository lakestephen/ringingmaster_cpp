
#include "stdafx.h"
#include "resource.h"
#include "RingingMaster.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TipDlg dialog


TipDlg::TipDlg(CWnd* pParent)
	: CDialog(IDD_TIP, pParent)
{
	//{{AFX_DATA_INIT(TipDlg)
	//}}AFX_DATA_INIT

	_show  =  app()->GetProfileInt("Tips", "Show", TRUE);
	_index =  app()->GetProfileInt("Tips", "Index", -1);
}

TipDlg::~TipDlg()
{
	app()->WriteProfileInt("Tips", "Show", _show);
	app()->WriteProfileInt("Tips", "Index", _index);
}
        
void TipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TipDlg)
	DDX_Check(pDX, IDC_STARTUP, _show);
	DDX_Text(pDX, IDC_TIPSTRING, _strTip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(TipDlg, CDialog)
	//{{AFX_MSG_MAP(TipDlg)
	ON_BN_CLICKED(IDC_NEXTTIP, OnNextTip)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TipDlg message handlers

void TipDlg::OnNextTip()
{
	_index++;
	
	int pos = 0;

	if (_index == pos++) _strTip = 
		"Thank you for installing Ringing Master. If you have registered, press the register button on the next dialog. If you find any bugs, or have any suggestions as to what you would like to see in the product e-mail me (See About box). \r\n\r\n        Enjoy !           Stephen J Lake";
	
	else if (_index == pos++) _strTip = 
		"The full version allows you to use methods up to 30 bells, and does not have the nag screen.";

	else if (_index == pos++) _strTip = 
		"The false rows on the method have tool tips that show what rows they are false with.";

	else if (_index == pos++) _strTip = 
		"You can select the format of notations, see File > Options > Method. The input will accept anything valid.";

	else if (_index == pos++) _strTip = 
		"Right clicking on a method will allow you so change the whole style, or the individual bell's style.";

	else if (_index == pos++) _strTip = 
		"Ringing Master is the fastest touch proving program available. It is also the easiest to use.";

	else if (_index == pos++) _strTip = 
		"You can alter and create your own menus, toolbars and keyboard shortcuts. See View > Customize";

	else if (_index == pos++) _strTip = 
		"If you hold down [ALT], you can drag menu buttons to new locations.";

	else if (_index == pos++) _strTip = 
		"The new touch wizard will add calling positions for the chosen method";

	else if (_index == pos++) _strTip = 
		"You can set up as many towers as you like. You can even set up multiple towers using the same sound files, but with different settings See File > Options > Tower.";

	else if (_index == pos++) _strTip = 
		"The new touch wizard will add common calling positions in spliced";

	else if (_index == pos++) _strTip = 
		"Right clicking on a row in a method will let you set that change as the start or end change.";

	else if (_index == pos++) _strTip = 
		"When in  multi block, each row has a tool tip that shows the other rows that are common with it";

	else if (_index == pos++) _strTip = 
		"The interface style can be set to the look and feel of Windows XP. See File > Options > General";

	else if (_index == pos++) _strTip = 
		"You can define your own music analysis by adding a row on the Music tab of the property window";

	else if (_index == pos++) _strTip = 
		"Show and hide the property hint panel. See the Options Dialog.";

	else if (_index == pos++) _strTip = 
		"Advanced users can now splice together methods of differing numbers of bells.";

	else if (_index == pos++) _strTip = 
		"You can turn off the Tabs at the top of the document window that allow the quick changing of the active document. See File > Options > General";
	
	else if (_index == pos++) _strTip = 
		"You can turn stop the last document being loaded on start-up. See File > Options > General";

	else if (_index == pos++) _strTip = 
		"The working directory for your touches can be set. See File > Options > General";

	else if (_index == pos++) _strTip = 
		"The CRU checking style can be set in File > Options > Music. This allows the breakdown of CRU's into there 6 component parts";

	else if (_index == pos++) _strTip = 
		"You can choose to show the changes in the standard 'Musical Changes'. File > Options > Music.";

	else if (_index == pos++) _strTip = 
		"To add edit or delete a tower set-up, for playing sound see File > Options > Tower.";
	
	else if (_index == pos++) _strTip = 
		"You can choose not to include the central council library. File > Options > Library. This will make the explorer load quicker on older PCs.";

	else if (_index == pos++) _strTip = 
		"The automatic generation of calls for type 'm' methods can be changed from near place to far place. See File > Options > Library";

	else if (_index == pos++) _strTip = 
		"The notation display style can be selected from 'Compact -12-34.56' to 'Verbose -.12.-.34.56'. See File > Options > Method";

	else if (_index == pos++) _strTip = 
		"Double clicking on most property controls makes for quicker editing.";

	else if (_index == pos++) _strTip = 
		"The character used in notations for all change can be '-', 'x', 'X'. See File > Options > Method";

	else if (_index == pos++) _strTip = 
		"Styles can be added, edited or deleted. See File > Options > Method";

	else if (_index == pos++) _strTip = 
		"Rows can be referenced by method or by lead. See File > Options > Method";

	else if (_index == pos++) _strTip = 
		"All the colours, and the font on the touch can be customised. See File > Options > Touch";

	else if (_index == pos++) _strTip = 
		"You can click on a row in the Falseness window to take you straight to that row on the Blue Line screen.";

	else if (_index == pos++) _strTip = 
		"The right click menu can quickly print either the touch or the method";

	else if (_index == pos++) _strTip = 
		"Print preview allows you to see what is going to appear on the page. ";

	else if (_index == pos++) _strTip = 
		"You can quickly go to a row from either the touch or the blue line using Edit > Goto. ";

	else if (_index == pos++) _strTip = 
		"The Edit > Find on the blue line window allows you to search for all or part of a change.";

	else if (_index == pos++) _strTip = 
		"You can drag a block definition to a new row, or resize it using the mouse. Try It!";

	else if (_index == pos++) _strTip = 
		"The Edit > Find on the touch window allows you to search for any text on the window.";

	else if (_index == pos++) _strTip = 
		"You can float any of the windows docked in the main window by double clicking on the title bar. Pressing [CTRL] while moving to prevent docking.";

	else if (_index == pos++) _strTip = 
		"The analysis screen will show multi block and spliced analysis as appropriate.";

	else if (_index == pos++) _strTip = 
		"When in spliced, the 'All the work' analysis shows analysis for each splicing position.";

	else if (_index == pos++) _strTip = 
		"You can click on a row in the music analysis to take you straight to that row on the Blue Line screen.";

	else if (_index == pos++) _strTip = 
		"To check the calling positions, and calls in a method, double click on the method in the method list on the property window, and the press the preview.";

	else if (_index == pos++) _strTip = 
		"Did you know that you can now show row in (+ve) and out (-ve) of course. See the Blue Line Style Dialog";

	else if (_index == pos++) _strTip = 
		"If you mouse over a splice you will get the method name.";

	else if (_index == pos++) _strTip = 
		"Touch variances can now be odd and even";

	else if (_index == pos++) _strTip = 
		"Touch Screen can now have notes on them";

	else if (_index == pos++) _strTip = 
		"You can resize blue lines directly on the screen";

	else if (_index == pos++) _strTip = 
		"Bell sounds can now be set differently for hand and backstroke";

	else if (_index == pos++) _strTip = 
		"Bell Sounds: You can set the number of rounds before playing the method";

	else if (_index == pos++) _strTip = 
		"Called changes upto 30 bells can now be practiced from the Called Changes Dialog";

	else if (_index == pos++) _strTip = 
		"You can copy all the analysis on a right click menu.";

	else if (_index == pos++) _strTip = 
		"Did you know that crib sheets will give you a plain English description of how to ring any place notation," ;

	else if (_index == pos++) _strTip = 
		"Print useful sheets for assisting learning the line.";

	else if (_index == pos++) _strTip = 
		"You can now create crib documents from the touch property window.";

	else if (_index == pos++) _strTip = 
		"Did you know that there are details of over 5000 tower built in.";

	else if (_index == pos++) _strTip = 
		"You can add towers to and delete towers from the base database using the right click menus.";

	else if (_index == pos++) _strTip = 
		"Did you know that you can grab towers from the map.";
	
	else if (_index == pos++) _strTip = 
		"In the records module, multiple users can be set up, allowing each person.";
	
	else if (_index == pos++) _strTip = 
		"Right click on a tower to see a map.";
	
	
	else 
	{
		_index = 1; //deliberatly. we do not want to show the first two tips again
		OnNextTip();
	}


	UpdateData(FALSE);
}

HBRUSH TipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_TIPSTRING)
		return (HBRUSH)GetStockObject(WHITE_BRUSH);

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void TipDlg::OnOK()
{
	CDialog::OnOK();
	DestroyWindow();
}

void TipDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Get paint area for the big static control
	CWnd* pStatic = GetDlgItem(IDC_BULB);
	CRect rect;
	pStatic->GetWindowRect(&rect);
	ScreenToClient(&rect);

	// Paint the background white.
	CBrush brush;
	brush.CreateStockObject(WHITE_BRUSH);
	dc.FillRect(rect, &brush);

	// Load bitmap and get dimensions of the bitmap
	CBitmap bmp;
	bmp.LoadBitmap(IDB_LIGHTBULB);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// Draw bitmap in top corner and validate only top portion of window
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&bmp);
	rect.bottom = bmpInfo.bmHeight + rect.top;
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		&dcTmp, 0, 0, SRCCOPY);

	// Draw out "Did you know..." message next to the bitmap
	CString strMessage;
	strMessage.LoadString(CG_IDS_DIDYOUKNOW);
	rect.left += bmpInfo.bmWidth;
	dc.DrawText(strMessage, rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	// Do not call CDialog::OnPaint() for painting messages
}	  

void TipDlg::startAsSplash()
{
#ifndef _DEBUG
	if (_show)
		DoModal();
#endif                                       
}

void TipDlg::startAsMenu()
{
	DoModal();
}

BOOL TipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OnNextTip();	
	
	return TRUE;  // return TRUE unless you set the focus to a control

}


