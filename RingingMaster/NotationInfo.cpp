// NotationInfo.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "NotationInfo.h"
#include "NotationDlg.h"
#include "PropItem.h"
#include ".\notationinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationInfo property page

NotationInfo::NotationInfo(Notation & notation) : 
CBCGPPropertyPage(NotationInfo::IDD, NULL),
_notation(notation)
{
	//{{AFX_DATA_INIT(NotationInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

NotationInfo::~NotationInfo()
{
}

void NotationInfo::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationInfo)
	DDX_Control(pDX, IDC_NOTATION_INFO, _info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationInfo, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationInfo message handlers

BOOL NotationInfo::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	//method referanced
	_info.setColumnCount(2);
	_info.setColumnHead(true);
	
	PropItem * propItem;
	
	propItem = new PropItem(PT_BUTTON, "Type", "Information");
	_info.AddPropItem(propItem);
	
	_info._devider[0] = 230;
	
	return TRUE;  // return TRUE unless you set the focus to a control

}


BOOL NotationInfo::OnKillActive() 
{

	if (CBCGPPropertyPage::OnKillActive()) //does the update data
	{
		_notation.setCCInfo(cc_RW,			_info.getPropItem(1)->_col[1]);
		_notation.setCCInfo(cc_towerBells,	_info.getPropItem(2)->_col[1]);
		_notation.setCCInfo(cc_place,		_info.getPropItem(3)->_col[1]);
		_notation.setCCInfo(cc_handbells,	_info.getPropItem(4)->_col[1]);
		_notation.setCCInfo(cc_CCC,			_info.getPropItem(5)->_col[1]);
		_notation.setCCInfo(cc_PMM,			_info.getPropItem(6)->_col[1]);
		_notation.setCCInfo(cc_TDMM,		_info.getPropItem(7)->_col[1]);
		_notation.setCCInfo(cc_MUG,			_info.getPropItem(8)->_col[1]);
		
		return TRUE;
	}
	
	return FALSE;
}

/*
	cc_towerBells,		//date first rung or spliced on tower bells
	cc_place,			//place first rung
	cc_handbells,		//date first rung or spliced on hand bells
	cc_CCC,				//ref of earlier Central Council Collection
	cc_PMM,				//ref on Collection of Plain Minor Methods
	cc_TDMM,			//ref on Treble Dodging Minor Methods
	cc_MUG,				//code for the 360 plain triples methods rung by the Manchester Uni guild
	cc_RW				//ringing World Ref
*/

BOOL NotationInfo::OnSetActive()
{
	//add each type in turn
	PropItem * propItem;

	_info.deleteAllPropItems();

	//cc_RW				//ringing World Ref
	propItem = new PropItem(PT_EDIT, "Reference to pages in Ringing World",		
							_notation.getCCInfo(cc_RW));
	_info.AddPropItem(propItem);

	//cc_towerBells,		//date first rung or spliced on tower bells
	propItem = new PropItem(PT_EDIT, "Date first peal rung on tower bells (or spliced)", 
							_notation.getCCInfo(cc_towerBells));
	_info.AddPropItem(propItem);
	
	//cc_place,			//place first rung
	propItem = new PropItem(PT_EDIT, "Location of first peal on tower bells", 
							_notation.getCCInfo(cc_place));
	_info.AddPropItem(propItem);
	
	//cc_handbells,		//date first rung or spliced on hand bells
	propItem = new PropItem(PT_EDIT, "Date first rung on hand bells (or spliced)",	
							_notation.getCCInfo(cc_handbells));
	_info.AddPropItem(propItem);
	
	//cc_CCC,				//ref of earlier Central Council Collection
	propItem = new PropItem(PT_EDIT, "Reference of earlier Central Council Collections", 
							_notation.getCCInfo(cc_CCC));
	_info.AddPropItem(propItem);
	
	//cc_PMM,				//ref on Collection of Plain Minor Methods
	propItem = new PropItem(PT_EDIT, "Reference of Collection of Plain Minor Methods",		
							_notation.getCCInfo(cc_PMM));
	_info.AddPropItem(propItem);
	
	//cc_TDMM,			//ref on Treble Dodging Minor Methods
	propItem = new PropItem(PT_EDIT, "Reference of Treble Dodging Minor Methods",		
							_notation.getCCInfo(cc_TDMM));
	_info.AddPropItem(propItem);
	
	//cc_MUG,				//code for the 360 plain triples methods rung by the Manchester Uni guild
	propItem = new PropItem(PT_EDIT, "Reference of 360 plain triples rung by Manchester University Guild",		
							_notation.getCCInfo(cc_MUG));
	_info.AddPropItem(propItem);
		
	return CBCGPPropertyPage::OnSetActive();
}
