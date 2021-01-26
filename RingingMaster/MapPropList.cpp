#include "StdAfx.h"
#include ".\Mapproplist.h"

#include "ButtonProp.h"
#include "UserMessages.h"
#include "MapManager.h"
#include "MapDoc.h"
#include "RingingMaster.h"

MapPropList::MapPropList(void)
{
}

MapPropList::~MapPropList(void)
{
}

BEGIN_MESSAGE_MAP(MapPropList, PropListBase)
	//{{AFX_MSG_MAP(MapPropList)
	ON_MESSAGE(BCG_BTN_PRESS, OnBCGBtnPress)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MAP_OPTIONS, OnMapOptions)
	ON_UPDATE_COMMAND_UI(ID_MAP_OPTIONS, OnUpdateMapOptions)
END_MESSAGE_MAP()

void MapPropList::initPropList()
{
	__super::initPropList();
	
	//-----------------------------------------
	//general
	//-----------------------------------------
	
	CBCGPProp* generalRoot = new CBCGPProp (_T("General"), ptm_generalRoot);
	AddProperty (generalRoot);									
	generalRoot->SetDescription("General map settings.");
	generalRoot->AddSubItem (new ButtonProp (_T("Map Provider"), (_variant_t) "",		_T("The current map provider. Press lint to go to there web page."),ptm_providerCurrent, IDB_LINK));
	generalRoot->AddSubItem( new CBCGPProp (_T("Scale"),		(_variant_t) "",	_T("The current scale."),ptm_providerScale));

	CBCGPColorProp* pGrabbedColorProp = new CBCGPColorProp    ("Grabbed Pin Colour", 
														RGB (0, 0, 0),
														NULL, 
														_T("Choose the colour of the pins representing the grabbed towers."),
														ptm_grabbedColor);
	pGrabbedColorProp->EnableOtherButton (_T("Other..."));
	generalRoot->AddSubItem (pGrabbedColorProp);
	CBCGPColorProp* pNonGrabbedColorProp = new CBCGPColorProp    ("Non Grabbed Pin Colour", 
														RGB (0, 0, 0),
														NULL, 
														_T("Choose the colour of the pins representing the towers that are not grabbed."),
														ptm_nonGrabbedColor);
	pNonGrabbedColorProp->EnableOtherButton (_T("Other..."));
	generalRoot->AddSubItem (pNonGrabbedColorProp);

	generalRoot->Expand(TRUE);

	

}

void MapPropList::mainFrame_notifyUpdate(ViewType viewType)
{
	__super::mainFrame_notifyUpdate(viewType);

	MapDoc* pMapDoc = mainFrame()->getActiveMapDoc();
 
	if (viewType == vt_map && pMapDoc)
	{            
		
		CBCGPProp* providerCurrent = FindItemByData(ptm_providerCurrent);
		if (providerCurrent)
		{
			providerCurrent->SetValue((_variant_t)mapManager()->getCurrentProviderName());
			providerCurrent->AllowEdit (FALSE);
		}
		
		CBCGPProp* providerScale = FindItemByData(ptm_providerScale);
		if (providerScale)
		{
			providerScale->RemoveAllOptions();
		
			ScaleTypes scaleTypes;
			ScaleType scale = pMapDoc->getScale();
			
			CString selectedOption;
			
			mapManager()->getSupportedScaleList(scaleTypes);
			for (int i=0;i<scaleTypes.GetSize();i++)
			{
				CString strScale = mapManager()->getScaleString(scaleTypes.GetAt(i));

				CString option = "1:" + strScale;
				providerScale->AddOption(option);

				if (scaleTypes.GetAt(i) == scale)
					selectedOption = option;

			}				  

			providerScale->SetValue((_variant_t)selectedOption);   
			providerScale->AllowEdit (FALSE);

		}

		CBCGPColorProp* grabbedColor = (CBCGPColorProp*)FindItemByData(ptm_grabbedColor);
		if (grabbedColor)
		{
			grabbedColor->SetColor(mapManager()->getPinColorGrabbed());
		}

		CBCGPColorProp* nonGrabbedColor = (CBCGPColorProp*)FindItemByData(ptm_nonGrabbedColor);
		if (nonGrabbedColor)
		{
			nonGrabbedColor->SetColor(mapManager()->getPinColorNonGrabbed());
		}

		//load the expand status
		loadExpandStatus();
	}	
}

void MapPropList::saveExpandStatus()
{
	//only one instance, so do nothing 
}

void MapPropList::loadExpandStatus()
{
	//only one instance, so do nothing 
}

void MapPropList::OnPropertyChanged (CBCGPProp* pProp) const
{
	MapDoc* pMapDoc = mainFrame()->getActiveMapDoc();

	if (pProp && pMapDoc)
	{
		switch (pProp->GetData())
		{
		case ptm_providerScale:
			{
				ScaleTypes scaleTypes;
				
				CString selectedOption(pProp->GetValue());
				
				mapManager()->getSupportedScaleList(scaleTypes);
				for (int i=0;i<scaleTypes.GetSize();i++)
				{
					CString strScale = mapManager()->getScaleString(scaleTypes.GetAt(i));

					CString option = "1:" + strScale;
					if (selectedOption == option)
						mapManager()->setDefaultScale(scaleTypes.GetAt(i));
				}		
			}

			break;

		case ptm_grabbedColor:
			mapManager()->setPinColorGrabbed(((CBCGPColorProp*)pProp)->GetColor());
			break;

		case ptm_nonGrabbedColor:
			mapManager()->setPinColorNonGrabbed(((CBCGPColorProp*)pProp)->GetColor());
			break;

		default:
		case ptm_providerCurrent:
			ASSERT(FALSE);
			break;
		}
	}   
}

										 
LRESULT MapPropList::OnBCGBtnPress(WPARAM /*wParam*/, LPARAM lParam)
{
	MapDoc* pMapDoc = mainFrame()->getActiveMapDoc();
	CBCGPProp* pProp = reinterpret_cast<CBCGPProp*>(lParam);

	if (pProp && pMapDoc)
	{
		switch (pProp->GetData())
		{
		case ptm_providerCurrent:
			mapManager()->openCurrentMapProviderWebSite();
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}	
	Invalidate();	 
				
	return 0;
}

void MapPropList::OnMapOptions()
{
	CBCGPProp* root = FindItemByData(ptm_generalRoot);
	if (root)
		root->Expand();

	::PostMessage(mainFrame()->m_hWnd,WM_COMMAND, ID_MAP_OPTIONS,0);;	
}

void MapPropList::OnUpdateMapOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
