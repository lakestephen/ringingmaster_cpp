// SGItemInfo.cpp: implementation of the SGItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SGItemInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SGItemInfo::SGItemInfo() :
m_bCheck(0),
m_iImage((const int)-1),
m_lParam(NULL),
m_clf((COLORREF)-1),
m_text_clf((COLORREF)-1),
_rowPosition(NULL)
{
}

SGItemInfo::~SGItemInfo()
{
	POSITION pos = m_listdata.GetStartPosition();
	while(pos != NULL)
	{
		int nKey;
		CStringList* b; 
		m_listdata.GetNextAssoc(pos, nKey, b);
		if(b!=NULL)
		{
			b->RemoveAll();
			delete b;
		}
	}
	m_listdata.RemoveAll();
}

void SGItemInfo::SetItemText(const CString& strItem)
{ 
	m_strItemName = strItem; 
}

void SGItemInfo::SetItemText(const CString& strItem, COLORREF text_clf, COLORREF clf) 
{ 
	m_strItemName = strItem; 
	m_clf = clf;
	m_text_clf = text_clf;
}

void SGItemInfo::AddSubItemText(const CString& strSubItem)
{ 
	m_SubItems.Add(strSubItem); 
}

void SGItemInfo::AddSubItemText(const CString& strSubItem, COLORREF clf)
{ 
	int nIndex = m_SubItems.Add(strSubItem); 
	m_mapClf.SetAt(nIndex, clf);
}

void SGItemInfo::SetSubItemText(int iSubItem, const CString& strSubItem, int iImage)
{ 
	m_SubItems.SetAtGrow(iSubItem, strSubItem);
	m_iImageSubItems.SetAt(iSubItem, iImage);
}

void SGItemInfo::SetSubItemImage(int iSubItem,int iImage)
{
	m_iImageSubItems.SetAt(iSubItem, iImage);
}

void SGItemInfo::SetSubItemText(int iSubItem, const CString& strSubItem)
{ 
	m_SubItems.SetAtGrow(iSubItem, strSubItem); 
}

void SGItemInfo::SetSubItemText(int iSubItem, const CString& strSubItem, COLORREF clf)
{
	m_SubItems.SetAtGrow(iSubItem, strSubItem);
	m_mapClf.SetAt(iSubItem, clf);
}

const CString& SGItemInfo::GetItemText(void)
{
	return m_strItemName; 
}

CString SGItemInfo::GetSubItem(int iSubItem)
{ 
	if(iSubItem < m_SubItems.GetSize())
		return m_SubItems.GetAt(iSubItem); 
	else
		return _T("");
}

int SGItemInfo::GetItemCount(void) const 
{ 
	return m_SubItems.GetSize(); 
}

//all cols in this row is default edit
void SGItemInfo::SetControlType(CONTROLTYPE enumCtrlType, int nCol)
{ 
	m_controlType.SetAt(nCol, enumCtrlType); 
}

BOOL SGItemInfo::GetControlType(int nCol, CONTROLTYPE& controlType) const
{
	if(!m_controlType.Lookup(nCol,controlType))
	{
		controlType = static_ctrl;//default;
		return 0;
	}
	return 1;
}

void SGItemInfo::SetListData(int iSubItem, CStringList* strInitArr)
{
	CStringList* list;
	list = new CStringList;//will be deleted in destructor
	list->AddTail(strInitArr);
	m_listdata.SetAt(iSubItem, list);
}	

BOOL SGItemInfo::GetListData(int iSubItem, CStringList*& pList) const
{	
	return m_listdata.Lookup(iSubItem, pList);
}

//used if LVS_EX_CHECKBOXES style
void SGItemInfo::SetCheck(BOOL bCheck)
{ 
	m_bCheck = bCheck;
}

BOOL SGItemInfo::GetCheck(void) const 
{
	return m_bCheck;
}

//icon stuff
void SGItemInfo::SetImage(int iImage)
{
	m_iImage = iImage;
}

//return icon in tree column
int SGItemInfo::GetImage(void) const 
{
	return m_iImage;
}

int SGItemInfo::GetSubItemImage(int iSubItem) const 
{
	int iSubImage=-1;
	m_iImageSubItems.Lookup(iSubItem, iSubImage);
	return iSubImage;
}
//color stuff
COLORREF SGItemInfo::GetItemClr(void) const 
{
	return m_clf;
}

COLORREF SGItemInfo::GetItemTextClr(void) const 
{
	return m_text_clf;
}

COLORREF SGItemInfo::GetBkColor(int iSubItem) const
{
	COLORREF clref;
	if(!m_mapClf.Lookup(iSubItem,clref))
	{
		return (COLORREF)-1;
	}
	return clref;
}


//YOU SHOULD MODIFY THIS WHEN EVER YOU ADD NEW DATA TO THIS CLASS
void SGItemInfo::CopyObjects(SGItemInfo* pItemInfo)
{
	SetItemText(pItemInfo->GetItemText());
	m_SubItems.Copy(pItemInfo->m_SubItems);
	CopyControls(pItemInfo);
	CopyColors(pItemInfo);
	SetCheck(pItemInfo->GetCheck());
	SetImage(pItemInfo->GetImage());
	m_lParam = pItemInfo->m_lParam;
	m_clf=pItemInfo->m_clf;
	m_text_clf = pItemInfo->m_text_clf;
	CopySubItemImageList(pItemInfo);
}

void SGItemInfo::CopySubItemImageList(SGItemInfo* pItemInfo)
{
	POSITION pos = pItemInfo->m_iImageSubItems.GetStartPosition();
	while(pos != NULL)
	{
		int nKey;
		int iImage=-1;
		pItemInfo->m_iImageSubItems.GetNextAssoc(pos, nKey, iImage);
		if(iImage!=-1)
			m_iImageSubItems.SetAt(nKey, iImage);
	}
}	

void SGItemInfo::CopyControls(SGItemInfo* pItemInfo)
{
	for(int nCol=0; nCol < pItemInfo->GetItemCount(); nCol++)
	{
		SGItemInfo::CONTROLTYPE ctrlType;
		if(pItemInfo->GetControlType(nCol, ctrlType))//true if other than edit-control
		{
			
			SetControlType(ctrlType, nCol);
			//should test if this is listdata material
			CStringList*list = NULL;
			pItemInfo->GetListData(nCol, list);
			if(list!=NULL)
				SetListData(nCol, list);	
		}
	}
}

void SGItemInfo::CopyColors(SGItemInfo* pItemInfo)
{
	POSITION pos = pItemInfo->m_mapClf.GetStartPosition();
	while(pos != NULL)
	{
		int nKey;
		COLORREF clref;
		pItemInfo->m_mapClf.GetNextAssoc(pos, nKey, clref);
		m_mapClf.SetAt(nKey, clref);
	}
}


