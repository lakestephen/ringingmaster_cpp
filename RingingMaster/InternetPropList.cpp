// InternetPropList.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "InternetPropList.h"
#include "InternetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InternetPropList

InternetPropList::InternetPropList()
{
}

InternetPropList::~InternetPropList()
{
}


BEGIN_MESSAGE_MAP(InternetPropList, CTreeCtrl)
	//{{AFX_MSG_MAP(InternetPropList)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InternetPropList message handlers

BOOL InternetPropList::Create(CWnd* pParentWnd)
{

	DWORD dwStyle;
	CRect rc;



	dwStyle = WS_CHILD | WS_VISIBLE | TVS_SINGLEEXPAND | TVS_TRACKSELECT;


	BOOL retVal = CTreeCtrl::Create(dwStyle, CRect(0,0,1000,1000), //needs to high to stop the devider being forced down
		pParentWnd, IDC_PROPERTYTREEINTERNET);

	setup();	  
	
	return retVal;
}

void InternetPropList::setup()
{
	theApp.m_Favorites.FillTree (*this);
}

void InternetPropList::OnSelchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTreeItem = GetSelectedItem ();
	if (hTreeItem != NULL)
	{
		CFavorit* pFavorit = (CFavorit*) GetItemData (hTreeItem);
		ASSERT_VALID (pFavorit);

		if (!pFavorit->GetURL ().IsEmpty ())
		{
			InternetDoc* pDoc = mainFrame()->getActiveInternetDoc();
			ASSERT(pDoc);
			if (pDoc)
			{
				pDoc->setInitialData(pFavorit->GetURL ());
			}
		}
	}
	
	*pResult = 0;

}
