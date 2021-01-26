// NotationListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationListCtrl.h"
#include "NotationLite.h"
#include "MethodDlg.h"
#include "InfoDlg.h"
#include "MethodPreview.h"
#include "ProofManager.h"
#include "ProofInput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL NotationListCtrl::_lastDirSort = TRUE;


/////////////////////////////////////////////////////////////////////////////
// NotationListCtrl

NotationListCtrl::NotationListCtrl() :
_liteList(NULL),
_lastContextItem(-1)

{
	_imageList.Create(IDB_EXPLORER, 16, 1, RGB(255,0,255));

	setUsePressedString(true);
}

NotationListCtrl::~NotationListCtrl()
{
	delete _liteList;
	_liteList = NULL;
}


void NotationListCtrl::showMethods()
{
	SortList(m_nSortedCol, m_bAscending);

	//switch drawing back on again
	SetRedraw(TRUE);
	SetItemCountEx(_liteListCount + _childFolders.GetSize());
	
	SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	Invalidate();
}


BEGIN_MESSAGE_MAP(NotationListCtrl, CCJListCtrl)
	//{{AFX_MSG_MAP(NotationListCtrl)
		ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)	 
		ON_COMMAND(ID_ME_CRIBMETHOD, OnMeCribmethod)
		ON_COMMAND(ID_ME_PREVIEW, OnMePreview)
		ON_COMMAND(ID_ME_INFO, OnMeInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationListCtrl message handlers
void NotationListCtrl::init()
{
	SetImageList(&_imageList, LVSIL_SMALL);
	SetMinimumColSize(30);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP  );

	AddColumn("Name");
	AddColumn("Number");
	AddColumn("Notation");
	AddColumn("le");
	AddColumn("Folder");	
	AddColumn("lh");
	AddColumn("Shorthand");
	AddColumn("RW Ref");
	AddColumn("Date Tower");
	AddColumn("Place");
	AddColumn("Date Hand");
	AddColumn("CCC");
	AddColumn("PMM");
	AddColumn("TDMM");
	AddColumn("MUG");

}

//overrides should call this first before sorting
bool NotationListCtrl::SortList(int nCol, bool bAscending)
{
	CWaitCursor wait;

	FlatHeaderCtrl* header = dynamic_cast<FlatHeaderCtrl*>(GetHeaderCtrl());

	if (header)
		header->SetSortColumn(nCol, bAscending);

	_lastDirSort = bAscending;


		switch (m_nSortedCol)
	{
	case 0 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareNames );
		break;
	case 1 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareNumber );
		break;
	case 2 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareNotation );
		break;
	case 3 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), comparele );
		break;
	case 4 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareFolderPaths);
		break;
	case 5 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), comparelh );
		break;
	case 6 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareShorthand );
		break;
	case 7 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareRWRef );
		break;
	case 8 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareDateTower );
		break;
	case 9 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), comparePlace );
		break;
	case 10 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareDateHand );
		break;
	case 11 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareCCC );
		break;
	case 12 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), comparePMM );
		break;
	case 13 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareTDMM );
		break;
	case 14 :
		qsort((void*)_liteList, (size_t)_liteListCount, sizeof(NotationLite *), compareMUG );
		break;
	default:
		ASSERT(FALSE);
		break;
	}


	return true;
} 

int NotationListCtrl::compareFolderPaths( const void * arg1, const void * arg2 )
{
    int result = 0;
    if (_lastDirSort)
		result = strcmp((*(NotationLite**)arg1)->getFolderPath(),(*(NotationLite**)arg2)->getFolderPath());
	else
		result = strcmp((*(NotationLite**)arg2)->getFolderPath(),(*(NotationLite**)arg1)->getFolderPath());
	
	//this actually speeds up a qsort
	if (result != 0)
		return result;
	else
		return compareNames(arg1, arg2);
}

int NotationListCtrl::compareNames( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getName(),(*(NotationLite**)arg2)->getName());
	else
		return strcmp((*(NotationLite**)arg2)->getName(),(*(NotationLite**)arg1)->getName());
}

int NotationListCtrl::compareNumber( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return ((*(NotationLite**)arg1)->getNumber() - (*(NotationLite**)arg2)->getNumber());
	else
		return ((*(NotationLite**)arg2)->getNumber() - (*(NotationLite**)arg1)->getNumber());
}

int NotationListCtrl::compareNotation( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getNotation(),(*(NotationLite**)arg2)->getNotation());
	else
		return strcmp((*(NotationLite**)arg2)->getNotation(),(*(NotationLite**)arg1)->getNotation());
}

int NotationListCtrl::comparele( const void * arg1, const void * arg2 )
{
    int result = 0;

    if (_lastDirSort)
		result = strcmp((*(NotationLite**)arg1)->getLeadEnd(),(*(NotationLite**)arg2)->getLeadEnd());
	else
		result = strcmp((*(NotationLite**)arg2)->getLeadEnd(),(*(NotationLite**)arg1)->getLeadEnd());

	//this actually speeds up a qsort
	if (result != 0)
		return result;
	else
		return compareNames(arg1, arg2);
}

int NotationListCtrl::comparelh( const void * arg1, const void * arg2 )
{
    int result = 0;
	if (_lastDirSort)
		result = strcmp((*(NotationLite**)arg1)->getLeadHead(),(*(NotationLite**)arg2)->getLeadHead());
	else
		result = strcmp((*(NotationLite**)arg2)->getLeadHead(),(*(NotationLite**)arg1)->getLeadHead());

	//this actually speeds up a qsort
	if (result != 0)
		return result;
	else
		return compareNames(arg1, arg2);

}

int NotationListCtrl::compareShorthand( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getSpliceLetter(),(*(NotationLite**)arg2)->getSpliceLetter());
	else
		return strcmp((*(NotationLite**)arg2)->getSpliceLetter(),(*(NotationLite**)arg1)->getSpliceLetter());
}

int NotationListCtrl::compareRWRef( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_RW),(*(NotationLite**)arg2)->getCCInfo(cc_RW));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_RW),(*(NotationLite**)arg1)->getCCInfo(cc_RW));
}

int NotationListCtrl::compareDateTower( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_towerBells),(*(NotationLite**)arg2)->getCCInfo(cc_towerBells));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_towerBells),(*(NotationLite**)arg1)->getCCInfo(cc_towerBells));
}

int NotationListCtrl::comparePlace( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_place),(*(NotationLite**)arg2)->getCCInfo(cc_place));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_place),(*(NotationLite**)arg1)->getCCInfo(cc_place));
}

int NotationListCtrl::compareDateHand( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_handbells),(*(NotationLite**)arg2)->getCCInfo(cc_handbells));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_handbells),(*(NotationLite**)arg1)->getCCInfo(cc_handbells));
}

int NotationListCtrl::compareCCC( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_CCC),(*(NotationLite**)arg2)->getCCInfo(cc_CCC));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_CCC),(*(NotationLite**)arg1)->getCCInfo(cc_CCC));
}

int NotationListCtrl::comparePMM( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_PMM),(*(NotationLite**)arg2)->getCCInfo(cc_PMM));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_PMM),(*(NotationLite**)arg1)->getCCInfo(cc_PMM));
}

int NotationListCtrl::compareTDMM( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_TDMM),(*(NotationLite**)arg2)->getCCInfo(cc_TDMM));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_TDMM),(*(NotationLite**)arg1)->getCCInfo(cc_TDMM));
}

int NotationListCtrl::compareMUG( const void * arg1, const void * arg2 )
{
    if (_lastDirSort)
		return strcmp((*(NotationLite**)arg1)->getCCInfo(cc_MUG),(*(NotationLite**)arg2)->getCCInfo(cc_MUG));
	else
		return strcmp((*(NotationLite**)arg2)->getCCInfo(cc_MUG),(*(NotationLite**)arg1)->getCCInfo(cc_MUG));
}


CString NotationListCtrl::getEmptyListText()
{
	return "No methods";
}

void NotationListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	if (pItem->mask & LVIF_IMAGE)
	{
		if (pItem->iItem < _childFolders.GetSize())
			pItem->iImage = 0;
		else
			pItem->iImage = 2;
	}
	
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		// then display the appropriate column
		lstrcpy(pItem->pszText, getItemString(pItem->iItem, pItem->iSubItem));
	}
	
	*pResult = 0;
}		  	

CString NotationListCtrl::getItemString(int row, int column)
{
	int childFoldersSize = _childFolders.GetSize();
	if (row < childFoldersSize)
	{
		if (column == 0)
			return getFolderName(_childFolders.GetAt(row));
		else
			return "";
	}
	else
	{

		NotationLite* lite = _liteList[row - childFoldersSize];

		switch(column)
		{
		case 0:
			return  lite->getName();
		case 1:
			return  GlobalFunctions::numberName(lite->getNumber());
		case 2:
			return  lite->getNotation(true);
		case 3:
			return  lite->getLeadEnd();
		case 4:
			return  lite->getFolderPath();
		case 5:
			return  lite->getLeadHead();
		case 6:
			return  lite->getSpliceLetter();
		case 7:
			return  lite->getCCInfo(cc_RW);
		case 8:
			return  lite->getCCInfo(cc_towerBells);
		case 9:
			return  lite->getCCInfo(cc_place);
		case 10:
			return  lite->getCCInfo(cc_handbells);
		case 11:
			return  lite->getCCInfo(cc_CCC);
		case 12:
			return  lite->getCCInfo(cc_PMM);
		case 13:
			return  lite->getCCInfo(cc_TDMM);
		case 14:
			return  lite->getCCInfo(cc_MUG);
			
		default:
			ASSERT(FALSE);
			return "";		
		}
	}
}


CString NotationListCtrl::getPressedTestString(int row, int column)
{
	return getItemString(row,column);
}

void NotationListCtrl::OnMeCribmethod() 
{

	//get the item
	NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

	if (!lite) return;

	Notation notation;
	notation = *lite;

	if ((mainFrame()->verifyKey() == FALSE)&&(notation.getNumber() > 6))
	{
		AfxMessageBox("Unregistered version limited to Minor (6): Please Register");
		return;
	}

	notation.crib();
}		  

void NotationListCtrl::OnMePreview() 
{
	//get the item
	NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

	if (!lite) return;

	Notation notation;
	notation = *lite;

	if ((mainFrame()->verifyKey() == FALSE)&&(notation.getNumber() > 6))
	{
		AfxMessageBox("Unregistered version limited to Minor (6): Please Register");
		return;
	}
	   
	CString msg;

	if (notation.verify(msg))
	{
		ProofInput proofInput(notation.getNumber());	
		proofInput.addNotation(notation);
		proofInput.setPerformAnalysis(); //todo do we want to do analysis here?
		MethodPreview* mPrev = new MethodPreview;
		proofManager()->requestProof(proofInput, *mPrev);

		MethodDlg dlg(this, mPrev, "NotationListCtrl");
		dlg.DoModal();	
	}
	else
	{
		AfxMessageBox(msg);
	}
}

void NotationListCtrl::OnMeInfo() 
{
	//get the item
	NotationLite* lite = _liteList[_lastContextItem - _childFolders.GetSize()];

	if (!lite) return;

	Notation notation;
	notation = *lite;

	if ((mainFrame()->verifyKey() == FALSE)&&(notation.getNumber() > 6))
	{
		AfxMessageBox("Unregistered version limited to Minor (6): Please Register");
		return;
	}

	CString msg;

	if (notation.verify(msg))
	{
		if (!notation.getNotation().IsEmpty())
		{
			InfoDlg dlg(this, notation);
			dlg.DoModal();
		}	
	}
	else
	{
		AfxMessageBox(msg);
	}				
}


CString NotationListCtrl::getFolderName(unsigned long /*item*/)
{ 
	//virtual in base clase
	ASSERT(FALSE);
	return "";	  
}

