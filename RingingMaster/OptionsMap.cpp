// OptionsMap.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsMap.h"
#include "MapManager.h"
#include "RecordTower.h"
#include ".\optionsmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsMap dialog


OptionsMap::OptionsMap()
	: CBCGPPropertyPage(OptionsMap::IDD, NULL)
{
	//{{AFX_DATA_INIT(OptionsMap)
	//}}AFX_DATA_INIT
	_size = mapManager()->getDiskCacheSizeMB();
}


void OptionsMap::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsMap)
	DDX_Control(pDX, IDC_STATIC_SP_VERSION_TEXT, _spVersionText);
	DDX_Control(pDX, IDC_STATIC_SP_VERSION, _spVeriosn);
	DDX_Control(pDX, IDC_STATIC_PROVIDER, _staticProvidersCtrl);
	DDX_Control(pDX, IDC_STATIC_MB, _staticMBCtrl);
	DDX_Control(pDX, IDC_STATIC_CACHE, _staticCacheCtrl);
	DDX_Control(pDX, IDC_CLEAR_CACHE, _clearCache);
	DDX_Control(pDX, IDC_SIZE_SPIN, _sizeSpinCtrl);
	DDX_Control(pDX, IDC_UPDATE_CHECK, _updateCheck);
	DDX_Control(pDX, IDC_PROVIDERS, _providersCtrl);
	DDX_Control(pDX, IDC_LINK, _link);
	DDX_Text(pDX, IDC_SIZE_EDIT, _size);
	DDV_MinMaxInt(pDX, _size, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptionsMap, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsMap)
	ON_BN_CLICKED(IDC_UPDATE_CHECK, OnUpdateCheck)
	ON_BN_CLICKED(IDC_LINK, OnLink)
	ON_CBN_SELCHANGE(IDC_PROVIDERS, OnSelchangeProviders)
	ON_BN_CLICKED(IDC_CLEAR_CACHE, OnClearCache)
	ON_UPDATE_COMMAND_UI(IDC_CLEAR_CACHE, OnUpdateClearCache)	
	ON_UPDATE_COMMAND_UI(IDC_LINK, OnUpdateLink)	
	ON_UPDATE_COMMAND_UI(IDC_PROVIDERS, OnUpdateProviders)	
	ON_UPDATE_COMMAND_UI(IDC_SIZE_EDIT, OnUpdateSizeEdit)	
	ON_UPDATE_COMMAND_UI(IDC_SIZE_SPIN, OnUpdateSizeSpin)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptionsMap message handlers



BOOL OptionsMap::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	update(); 

	_link.SetImage(IDB_LINK);
	_link.m_bDrawFocus = FALSE;
	_link.SetTooltip("Open providers web page.");
	_link.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_updateCheck.SetImage(IDB_LINK);
	_updateCheck.m_bDrawFocus = FALSE;
	_updateCheck.SetTooltip("Connect to the internet and download map provider details.");
	_updateCheck.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_clearCache.SetImage(IDB_TRASH);
	_clearCache.m_bDrawFocus = FALSE;
	_clearCache.SetTooltip("Clear out the map cache.");
	_clearCache.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_sizeSpinCtrl.SetRange32(1,10000);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsMap::OnUpdateCheck() 
{
	mapManager()->checkForNewMapConVersion(true);

	update();
}

void OptionsMap::OnLink() 
{
	mapManager()->openCurrentMapProviderWebSite();
}

void OptionsMap::update()
{
	_providersCtrl.ResetContent();
	CStringArray strings;
	mapManager()->getProviderList(strings);
	for (int i=0;i<strings.GetSize();i++)
	{
		_providersCtrl.SetItemData(_providersCtrl.AddString(strings.GetAt(i)),i);
	}
	_providersCtrl.SetCurSel(mapManager()->getCurentHostIndex());

	_spVeriosn.SetWindowText(mapManager()->getDLLVerison());
}

void OptionsMap::OnSelchangeProviders() 
{
	mapManager()->setCurrentHostIndex(_providersCtrl.GetItemData(_providersCtrl.GetCurSel()));
	
	update();
}


void OptionsMap::OnClearCache() 
{
	if (AfxMessageBox("This action will clear out your map cache. Do you want to proceed?", MB_YESNO) == IDYES)
		mapManager()->destroyMapCache(false);	
}

void OptionsMap::OnUpdateClearCache(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(mapManager()->isValid());
}

void OptionsMap::OnUpdateLink(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(mapManager()->isValid());
}

void OptionsMap::OnUpdateSizeSpin(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(mapManager()->isValid());
}

void OptionsMap::OnUpdateSizeEdit(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(mapManager()->isValid());
}

void OptionsMap::OnUpdateProviders(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable(mapManager()->isValid());
	   
	_staticCacheCtrl.EnableWindow(mapManager()->isValid());
	_staticMBCtrl.EnableWindow(mapManager()->isValid());
	_staticProvidersCtrl.EnableWindow(mapManager()->isValid());
	_spVeriosn.EnableWindow(mapManager()->isValid());
	_spVersionText.EnableWindow(mapManager()->isValid());
}

BOOL OptionsMap::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		mapManager()->setDiskCacheSizeMB(_size);

		return TRUE;
	}

	return FALSE;
}
