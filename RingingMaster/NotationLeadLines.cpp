// NotationLeadLines.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "PropItem.h"
#include "NotationDlg.h"
#include "NotationLeadLines.h"
#include "LeadLineEditDlg.h"
#include "MethodPosition.h"
#include "MethodDlg.h"
#include ".\notationleadlines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationLeadLines property page


NotationLeadLines::NotationLeadLines(Notation & notation) : 
CBCGPPropertyPage(NotationLeadLines::IDD),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationLeadLines)
	//}}AFX_DATA_INIT

}

NotationLeadLines::~NotationLeadLines()
{
}

void NotationLeadLines::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationLeadLines)
	DDX_Control(pDX, IDC_ADD_DEFAULT_LEAD_LINE, _addDefault);
	DDX_Control(pDX, IDC_USE_DEFAULT_LEAD_LINES, _useDefaultLeadLine);
	DDX_Control(pDX, IDC_LEAD_LINES, _leadLine);
	DDX_Control(pDX, IDC_ADD_LEAD_LINE, _addLeadLine);
	DDX_Control(pDX, IDC_REMOVE_LEAD_LINE, _removeLeadLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationLeadLines, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationLeadLines)
	ON_BN_CLICKED(IDC_USE_DEFAULT_LEAD_LINES, OnDefaultLeadLines)
	ON_BN_CLICKED(IDC_ADD_LEAD_LINE, OnAddLeadLine)
	ON_BN_CLICKED(IDC_REMOVE_LEAD_LINE, OnRemoveLeadLine)
	ON_BN_CLICKED(IDC_ADD_DEFAULT_LEAD_LINE, OnAddDefaultLeadLine)
	ON_BN_DOUBLECLICKED(IDC_USE_DEFAULT_LEAD_LINES, OnDefaultLeadLines)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ADD_LEAD_LINE, OnUpdateAddLeadLine)
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_LEAD_LINE, OnUpdateRemoveLeadLine)
	ON_UPDATE_COMMAND_UI(IDC_ADD_DEFAULT_LEAD_LINE, OnUpdateAddDefaultLeadLines)	
	

	ON_MESSAGE(WM_PROP_BUTTON, OnEditLeadLine)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NotationLeadLines message handlers

BOOL NotationLeadLines::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	
	_addLeadLine.SetImage(IDB_NEW);
	_addLeadLine.m_bDrawFocus = FALSE;
	_addLeadLine.SetTooltip("Add Lead Line");
	_addLeadLine.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_removeLeadLine.SetImage(IDB_DELETE);
	_removeLeadLine.m_bDrawFocus = FALSE;
	_removeLeadLine.SetTooltip("Remove Lead Line");
	_removeLeadLine.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addDefault.SetImage(IDB_ADD_DEFAULT);
	_addDefault.m_bDrawFocus = FALSE;
	_addDefault.SetTooltip("Add the default lead line to the user definable list");
	_addDefault.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	EnableToolTips();

	PropItem * propItem;

	//method referanced
	_leadLine.setColumnCount(1);
	_leadLine.setColumnHead(true);
	
	propItem = new PropItem(PT_BUTTON, "After Row");
	_leadLine.AddPropItem(propItem);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}


void NotationLeadLines::OnDefaultLeadLines() 
{
	//possably changing state
	UpdateData();

	_notation.setDefaultLeadLines(_useDefaultLeadLine.GetCheck()); 

	createLeadLineList();		
}

void NotationLeadLines::OnAddLeadLine() 
{
	if (_notation.getDefaultLeadLines()) return;

	UpdateData();

	LeadLineEditDlg dlg(this, _notation);

	while (dlg.DoModal() == IDOK)
	{
		LeadLinesError ret = _notation.addLeadLine(dlg._leadLine - (globalDataManager()->isStartAtRowZero()?0:1));
		if ((ret == LLE_OK)||(ret == LLE_OK_DEFAULT))
		{			
			createLeadLineList();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddLeadLineErrorText(ret));
		}
	} 
}					

void NotationLeadLines::OnRemoveLeadLine() 
{
	//find the item
	PropItem * pItem = _leadLine.getPropItem(_leadLine.GetCurSel());

	if (pItem)
	{		  
		int leadLine = atoi(pItem->_col[0]) -(globalDataManager()->isStartAtRowZero()?0:1) ;
			
		if (leadLine >= 0)
		{
			_notation.deleteLeadLine(_notation.findLeadLine(leadLine));
		}
	}	   
	createLeadLineList();	
}

void NotationLeadLines::createLeadLineList()
{
	//empty the list;
	_leadLine.deleteAllPropItems();

	BOOL editable = !_useDefaultLeadLine.GetCheck();
	
	for (int i=0;i<_notation.getLeadLineCount();i++)
	{
		CString leadLine;
		leadLine.Format("%d", _notation.getLeadLine(i)+(globalDataManager()->isStartAtRowZero()?0:1));
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, leadLine);
		_leadLine.AddPropItem(propItem);
	}
}

void NotationLeadLines::OnUpdateAddLeadLine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultLeadLine.GetCheck());
}

void NotationLeadLines::OnUpdateRemoveLeadLine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultLeadLine.GetCheck()&&
				   _leadLine.getPropItem(_leadLine.GetCurSel()));
}

void NotationLeadLines::OnUpdateAddDefaultLeadLines(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultLeadLine.GetCheck());
}


LRESULT NotationLeadLines::OnEditLeadLine(WPARAM /*wParam*/, LPARAM lParam)	
{
	if (_notation.getDefaultLeadLines()) return 0;

	PropItem * pItem = (PropItem*)lParam;
	if (!pItem) return 0;

	//find the call from the prop item //lets be really safe here
	int leadLine = atoi(pItem->_col[0]) -(globalDataManager()->isStartAtRowZero()?0:1) ;
	
	if (leadLine >= 0)
	{
		int index = _notation.findLeadLine(leadLine);
		if (index == -1) return 0;
		
		//edit the call
		LeadLineEditDlg dlg(this,_notation, leadLine);

		while (dlg.DoModal() == IDOK)
		{
			LeadLinesError ret = _notation.changeLeadLine(index, dlg._leadLine - (globalDataManager()->isStartAtRowZero()?0:1));
			if ((ret == LLE_OK)||(ret == LLE_OK_DEFAULT))
			{			
				createLeadLineList();
				return 0;
			}
			else
			{
				AfxMessageBox(_notation.getAddLeadLineErrorText(ret));
			}
		}
	}
	return 0;	
}

void NotationLeadLines::OnAddDefaultLeadLine() 
{
	if (_notation.getDefaultLeadLines()) return;
	
	_notation.addLeadLineDefault();

	createLeadLineList();	

}


BOOL NotationLeadLines::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{	
		_notation.setDefaultLeadLines(_useDefaultLeadLine.GetCheck());
		return TRUE;
	}

	return FALSE;
}

BOOL NotationLeadLines::OnSetActive()
{
	//lead line stuff
	_useDefaultLeadLine.SetCheck(_notation.getDefaultLeadLines());

	createLeadLineList();	

	return CBCGPPropertyPage::OnSetActive();
}
