// TowerSoundsTTS.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsTTS.h"
#include "TowerTTSItemDlg.h"
#include "Tower.h"
#include "PropItem.h"
#include "SpeechManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsTTS dialog


TowerSoundsTTS::TowerSoundsTTS( Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsTTS::IDD),
	_internalTower(internalTower)

{
	//{{AFX_DATA_INIT(TowerSoundsTTS)
	//}}AFX_DATA_INIT
}


void TowerSoundsTTS::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsTTS)
	DDX_Control(pDX, IDC_SPEECHVOICES, _speechList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TowerSoundsTTS, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(TowerSoundsTTS)
	ON_MESSAGE(WM_PROP_BUTTON, OnEditItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsTTS message handlers

BOOL TowerSoundsTTS::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_speechList.setColumnCount(3);
	_speechList.setColumnHead(true);
	_speechList.SetFont(&app()->_fontStd);
	PropItem* propItem = new PropItem(PT_BUTTON, "Type", "Active", "Text");
	_speechList.AddPropItem(propItem);	
	_speechList._devider[0] =50; 
	_speechList._devider[1] =110; 
	_speechList.setColumnStyle(2, DT_END_ELLIPSIS);


	_speechList.EnableWindow(speechManager()->isTTSAvailable());

	update();

	return TRUE;  // return TRUE unless you set the focus to a control

}



LRESULT TowerSoundsTTS::OnEditItem(WPARAM /*wParam*/, LPARAM lParam)	
{
	PropItem * pItem = (PropItem*)lParam;

	if (pItem) 
	{
		for (int i=0;i<_internalTower.getTowerTTSItemCount();i++)
		{
			TowerTTSItem* item = _internalTower.getTowerTTSItem(i);
			if (item)
			{
				if (TowerTTSItem::getItemTypeText(item->_type) == pItem->_col[0])
				{
					TowerTTSItemDlg dlg(this, *item);
					if (dlg.DoModal() == IDOK)
					{
						update();
					}

					break;
				}
			}
		}
	}

	return 0;
}

void TowerSoundsTTS::update()
{
	_speechList.deleteAllPropItems();

	for (int i=0;i<_internalTower.getTowerTTSItemCount();i++)
	{
		TowerTTSItem* item = _internalTower.getTowerTTSItem(i);
		if (item)
		{
			PropItem* pItem = new PropItem((speechManager()->isTTSAvailable()?PT_BUTTON:PT_STATIC_GREY), 
											TowerTTSItem::getItemTypeText(item->_type), 
											(speechManager()->isTTSAvailable())?(item->_active?"enabled":"disabled"):"disabled" , 
											item->_string);
			_speechList.AddPropItem(pItem);
		}
	}	
}
