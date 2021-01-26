

#include "stdafx.h"
#include "ComboCompletion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAX_HISTORY_ITEMS = 20;

const CString HISTORY_SECTION = "Dropdown History";



/////////////////////////////////////////////////////////////////////////////
// CComboCompletion

CComboCompletion::CComboCompletion(BOOL bAllowSortStyle/*=FALSE*/)
{
	m_bAutoComplete = TRUE;		 
	m_nMaxHistoryItems = MAX_HISTORY_ITEMS;
	m_bSaveRestoreLastCurrent = TRUE;
	m_bAllowSortStyle = bAllowSortStyle;
	
}

CComboCompletion::~CComboCompletion()
{
}


BEGIN_MESSAGE_MAP(CComboCompletion, CComboBox)
//{{AFX_MSG_MAP(CComboCompletion)
ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboCompletion message handlers

BOOL CComboCompletion::PreTranslateMessage(MSG* pMsg)
{
	// Need to check for backspace/delete. These will modify the text in
	// the edit box, causing the auto complete to just add back the text
	// the user has just tried to delete. 
	
	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;
		
		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
			m_bAutoComplete = FALSE;
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CComboCompletion::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (!m_bAutoComplete) 
		return;
	
	// Get the text in the edit box
	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();
	
	// Currently selected range
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
	
	// Search for, and select in, and string in the combo box that is prefixed
	// by the text in the edit box
	if (SelectString(-1, str) == CB_ERR)
	{
		SetWindowText(str);		// No text selected, so restore what was there before
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);	//restore cursor postion
	}
	
	// Set the text selection as the additional text that we have added
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}



/////////////////////////////////////////////////////////////////////////////
// From here on came from the history example 	 - sjl


BOOL CComboCompletion::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (! m_bAllowSortStyle)  // turn off CBS_SORT style
		cs.style &= ~CBS_SORT;
	return CComboBox::PreCreateWindow(cs);
}

void CComboCompletion::PreSubclassWindow() 
{
	// warn if creating with CBS_SORT style
	// (unfortunately we can't turn it off)
	if (! m_bAllowSortStyle && GetStyle() & CBS_SORT)
		TRACE("WARNING: Creating History combo with CBS_SORT style\n");
	CComboBox::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CComboCompletion message handlers

// this version of AddString adds a string only if it doesn't already exist
// in the list, and in any case, makes sure that the string is the first
// in the list (ie most recent in history)
// also makes sure number of items in the list doesn't exceed the maximum allowed
int CComboCompletion::AddString(LPCTSTR lpszString)
{
	// if it's not set up as a history combo then call base class
	if (m_sKeyPrefix.IsEmpty())
		return CComboBox::AddString(lpszString);
	
	int nRet = -1;
	// don't add if already there
	CString sString(lpszString);
	sString.TrimLeft(" ");
	sString.TrimRight(" ");
	nRet = CComboBox::InsertString(0, sString);
	int nIndex = FindStringExact(0, sString);
	if (nIndex != -1 && nIndex != 0)
		DeleteString(nIndex);
	
	// if we've got too many items then delete the last one
	// truncate list to m_nMaxHistoryItems
	int nNumItems = GetCount();
	for (int n = m_nMaxHistoryItems; n < nNumItems; n++)
		DeleteString(m_nMaxHistoryItems);
	
	SetCurSel(nRet);
	return nRet;
}

// loads the history from the specified profile area, and returns the 
// text selected
// the profile area is kept so that in doesn't need to specified again
// when saving the history
CString CComboCompletion::LoadHistory(LPCTSTR lpszKeyPrefix, 
									  BOOL bSaveRestoreLastCurrent/*=TRUE*/, 
									  LPCTSTR lpszKeyCurItem/*=NULL*/)
{
	
	if (lpszKeyPrefix == NULL )
		return "";
	

	m_sKeyPrefix = lpszKeyPrefix;
	m_sKeyCurItem = lpszKeyCurItem == NULL ? "" : lpszKeyCurItem;
	m_bSaveRestoreLastCurrent = bSaveRestoreLastCurrent;
	CWinApp* pApp = AfxGetApp();
	
	int n = 0;
	CString sText;
	do
	{
		CString sKey;
		sKey.Format("%s%d", m_sKeyPrefix, n++);
		sText = pApp->GetProfileString(HISTORY_SECTION, sKey);
		if (!sText.IsEmpty())
			CComboBox::AddString(sText);
	}while (!sText.IsEmpty() && n < m_nMaxHistoryItems);

	if (m_bSaveRestoreLastCurrent)
	{
		CString sKey;
		if (!m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
		sText = pApp->GetProfileString(HISTORY_SECTION, sKey);
		if (!sText.IsEmpty())
		{
			int nIndex = FindStringExact(-1, sText);
			if (nIndex != -1)
				SetCurSel(nIndex);
			else if (GetStyle() & CBS_DROPDOWN)
				SetWindowText(sText);
		}
	}

	SetEditSel(0,1000) ;
	SetFocus();

	return sText;
}

// saves the history to the profile specified when calling LoadHistory
// if no profile information (ie LoadHistory() wasn't called with it) then
// this function does nothing
void CComboCompletion::SaveHistory(BOOL bAddCurrentItemToHistory/*=TRUE*/)
{

	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp);
	
	if (bAddCurrentItemToHistory)
	{
		CString sCurItem;
		GetWindowText(sCurItem);
		// trim it, so we items which differ only by a leading/trailing space
		sCurItem.TrimLeft();
		sCurItem.TrimRight();
		if (! sCurItem.IsEmpty())
			AddString(sCurItem);
	}
	
	// save history to info cached earlier
	int nMax = min(GetCount(), m_nMaxHistoryItems + 1);
	for (int n = 0; n < nMax; n++)
	{
		CString sKey;
		sKey.Format("%s%d", m_sKeyPrefix, n);
		CString sText;
		GetLBText(n, sText);
		pApp->WriteProfileString(HISTORY_SECTION, sKey, sText);
	}
	// remove redundant items
	for (int n = nMax; n < 1000/* prevent runaway*/; n++)
	{
		CString sKey;
		sKey.Format("%s%d", m_sKeyPrefix, n);
		CString sText = pApp->GetProfileString(HISTORY_SECTION, sKey);
		if (sText.IsEmpty())
			break;
		pApp->WriteProfileString(HISTORY_SECTION, sKey, NULL); // remove entry
	}
	if (m_bSaveRestoreLastCurrent)
	{
		CString sText;
		GetWindowText(sText);
		CString sKey;
		if (!m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
		pApp->WriteProfileString(HISTORY_SECTION, sKey, sText);
	}
}

// removes all the items from the history list, and optionally deletes
// the registry items. Note that if the history list is generated from
// a CRecentFileList, then registry entries will not be deleted
void CComboCompletion::ClearHistory(BOOL bDeleteRegistryEntries/*=TRUE*/)
{
	ResetContent();
	if (bDeleteRegistryEntries)
	{
		// remove profile entries
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp);
		CString sKey;
		for (int n = 0; n < 1000/* prevent runaway*/; n++)
		{
			sKey.Format("%s%d", HISTORY_SECTION, n);
			CString sText = pApp->GetProfileString(HISTORY_SECTION, sKey);
			if (sText.IsEmpty())
				break;
			pApp->WriteProfileString(HISTORY_SECTION, sKey, NULL); // remove entry
		}
		if (! m_sKeyCurItem.IsEmpty())
			sKey = m_sKeyCurItem;
		else if (m_sKeyPrefix.IsEmpty())
			sKey = "Last";
		else
			sKey = m_sKeyPrefix;
		pApp->WriteProfileString(HISTORY_SECTION, sKey, NULL);
	}
}

void CComboCompletion::SetMaxHistoryItems(int nMaxItems)
{
	m_nMaxHistoryItems = nMaxItems;
	
	// truncate list to nMaxItems
	int nNumItems = GetCount();
	for (int n = m_nMaxHistoryItems; n < nNumItems; n++)
		DeleteString(m_nMaxHistoryItems);
}

void CComboCompletion::StoreValue(BOOL bIgnoreIfEmpty/*=TRUE*/)
{
	// add the current value to the list
	CString sValue;
	GetWindowText(sValue);
	if (bIgnoreIfEmpty && sValue.IsEmpty())
		return;
	AddString(sValue);
}

