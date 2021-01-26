// NotationSplicePos.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationDlg.h"
#include "NotationSplicePos.h"
#include "PropItem.h"
#include "Notation.h"
#include "NotationDlg.h"
#include "SpliceEditDlg.h"
#include ".\notationsplicepos.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationSplicePos property page



NotationSplicePos::NotationSplicePos(Notation &notation) : 
CBCGPPropertyPage(NotationSplicePos::IDD),
_notation(notation)	

{
	//{{AFX_DATA_INIT(NotationSplicePos)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

NotationSplicePos::~NotationSplicePos()
{
}

void NotationSplicePos::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationSplicePos)
	DDX_Control(pDX, IDC_USE_DEFAULT_SPLICES, _useDefaultSplice);
	DDX_Control(pDX, IDC_ADD_DEFAULT_SPLICES, _addDefault);
	DDX_Control(pDX, IDC_REMOVE_SPLICE, _removeSplice);
	DDX_Control(pDX, IDC_ADD_SPLICE, _addSplice);
	DDX_Control(pDX, IDC_SPLICES, _splice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationSplicePos, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationSplicePos)
	ON_BN_CLICKED(IDC_ADD_DEFAULT_SPLICES, OnAddDefaultSplices)
	ON_BN_CLICKED(IDC_ADD_SPLICE, OnAddSplice)
	ON_BN_CLICKED(IDC_REMOVE_SPLICE, OnRemoveSplice)
	ON_BN_CLICKED(IDC_USE_DEFAULT_SPLICES, OnUseDefaultSplices)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ADD_SPLICE, OnUpdateAddSplice)
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_SPLICE, OnUpdateRemoveSplice)
	ON_UPDATE_COMMAND_UI(IDC_ADD_DEFAULT_SPLICES, OnUpdateAddDefaultSplice)	

	ON_MESSAGE(WM_PROP_BUTTON, OnEditSplice)

END_MESSAGE_MAP()



BOOL NotationSplicePos::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
		
	_addSplice.SetImage(IDB_NEW);
	_addSplice.m_bDrawFocus = FALSE;
	_addSplice.SetTooltip("Add Splice");
	_addSplice.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_removeSplice.SetImage(IDB_DELETE);
	_removeSplice.m_bDrawFocus = FALSE;
	_removeSplice.SetTooltip("Remove Splice");
	_removeSplice.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_addDefault.SetImage(IDB_ADD_DEFAULT);
	_addDefault.m_bDrawFocus = FALSE;
	_addDefault.SetTooltip("Add the default splice to the user definable list");
	_addDefault.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	EnableToolTips();

	PropItem * propItem;

	//method referanced
	_splice.setColumnCount(1);
	_splice.setColumnHead(true);
	
	propItem = new PropItem(PT_BUTTON, "After Row");
	_splice.AddPropItem(propItem);

	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationSplicePos::OnAddDefaultSplices() 
{
	if (_notation.getDefaultSplicePosition()) return;
	
	_notation.addSplicePositionDefault();

	createSpliceList();	
}

void NotationSplicePos::OnAddSplice() 
{
	if (_notation.getDefaultSplicePosition()) return;

	UpdateData();

	SpliceEditDlg dlg(this, _notation);

	while (dlg.DoModal() == IDOK)
	{
		SpliceError ret = _notation.addSplicePosition(dlg._splice - (globalDataManager()->isStartAtRowZero()?0:1));
		if ((ret == SPE_OK)||(ret == SPE_OK_DEFAULT))
		{			
			createSpliceList();
			return;
		}
		else
		{
			AfxMessageBox(_notation.getAddSplicePositionErrorText(ret));
		}
	} 	
}

void NotationSplicePos::OnRemoveSplice() 
{
	//find the item
	PropItem * pItem = _splice.getPropItem(_splice.GetCurSel());

	if (pItem)
	{		  
		int splice = atoi(pItem->_col[0]) -(globalDataManager()->isStartAtRowZero()?0:1) ;
			
		if (splice >= 0)
		{
			_notation.deleteSplicePosition(_notation.findSplicePosition(splice));
		}
	}	   
	createSpliceList();	
}

void NotationSplicePos::OnUseDefaultSplices() 
{
	//possably changing state
	UpdateData();

	_notation.setDefaultSplicePosition(_useDefaultSplice.GetCheck()); 

	createSpliceList();
}


void NotationSplicePos::createSpliceList()
{
	//empty the list;
	_splice.deleteAllPropItems();

	BOOL editable = !_notation.getDefaultSplicePosition();
	
	for (int i=0;i<_notation.getSplicePositionCount();i++)
	{
		CString splice;
		splice.Format("%d", _notation.getSplicePosition(i)+(globalDataManager()->isStartAtRowZero()?0:1));
		
		PropItem * propItem = new PropItem(editable?PT_BUTTON:PT_STATIC_GREY, splice);
		_splice.AddPropItem(propItem);
	}
}

void NotationSplicePos::OnUpdateAddSplice(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultSplice.GetCheck());
}

void NotationSplicePos::OnUpdateRemoveSplice(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultSplice.GetCheck()&&
				   _splice.getPropItem(_splice.GetCurSel()));
}

void NotationSplicePos::OnUpdateAddDefaultSplice(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!_useDefaultSplice.GetCheck());
}

LRESULT NotationSplicePos::OnEditSplice(WPARAM /*wParam*/, LPARAM lParam)	
{
	if (_notation.getDefaultSplicePosition()) return 0;

	PropItem * pItem = (PropItem*)lParam;
	if (!pItem) return 0;

	//find the call from the prop item //lets be really safe here
	int splice = atoi(pItem->_col[0]) - (globalDataManager()->isStartAtRowZero()?0:1) ;
	
	if (splice >= 0)
	{
		int index = _notation.findSplicePosition(splice);
		if (index == -1) return 0;
		
		//edit the call
		SpliceEditDlg dlg(this,_notation, splice);

		while (dlg.DoModal() == IDOK)
		{
			SpliceError ret = _notation.changeSplicePosition(index, dlg._splice- (globalDataManager()->isStartAtRowZero()?0:1));
			if ((ret == SPE_OK)||(ret == SPE_OK_DEFAULT))
			{			
				createSpliceList();
				return 0;
			}
			else
			{
				AfxMessageBox(_notation.getAddSplicePositionErrorText(ret));
			}
		}
	}
	return 0;	
}



BOOL NotationSplicePos::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
 		_notation.setDefaultSplicePosition(_useDefaultSplice.GetCheck());
		return TRUE;
	}

	return FALSE;
}

BOOL NotationSplicePos::OnSetActive()
{
	_useDefaultSplice.SetCheck(_notation.getDefaultSplicePosition());

	createSpliceList();	

	return CBCGPPropertyPage::OnSetActive();
}
