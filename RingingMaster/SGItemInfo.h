// SGItemInfo.h: interface for the SGItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SGITEMINFO_H__B2F28C74_F06D_11D5_B53E_000255162CF0__INCLUDED_)
#define AFX_SGITEMINFO_H__B2F28C74_F06D_11D5_B53E_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RowPosition;

class SGItemInfo //should have called this LV_INSERTITEM ..what ever
{
public:
	SGItemInfo();

	~SGItemInfo();
	void SetItemText(const CString& strItem);		
	void SetItemText(const CString& strItem, COLORREF text_clf, COLORREF clf);
	void AddSubItemText(const CString& strSubItem);
	void AddSubItemText(const CString& strSubItem, COLORREF clf);
	void SetSubItemText(int iSubItem, const CString& strSubItem, int iImage);
	void SetSubItemImage(int iSubItem,int iImage);

	void SetSubItemText(int iSubItem, const CString& strSubItem);
	void SetSubItemText(int iSubItem, const CString& strSubItem, COLORREF clf);

	const CString& GetItemText(void);
	CString GetSubItem(int iSubItem);
	int GetItemCount(void) const ;

	enum CONTROLTYPE {static_ctrl/*default*/, edit, combobox};
	//all cols in this row is default edit
	void SetControlType(CONTROLTYPE enumCtrlType, int nCol=-1);

	BOOL GetControlType(int nCol, CONTROLTYPE& controlType) const;

	void SetListData(int iSubItem, CStringList*strInitArr);

	BOOL GetListData(int iSubItem, CStringList*& pList) const;

	//used if LVS_EX_CHECKBOXES style
	void SetCheck(BOOL bCheck);
	BOOL GetCheck(void) const ;
	//icon stuff
	void SetImage(int iImage);
	int GetImage(void) const ;//return icon in tree column
	int GetSubItemImage(int iSubItem) const;
	//color stuff
	COLORREF GetItemClr(void) const;
	COLORREF GetItemTextClr(void) const ;
	COLORREF GetBkColor(int iSubItem) const;

	LPARAM m_lParam;//why not use this like you use it in listbox, listctrl...

	//YOU SHOULD MODIFY THIS WHEN EVER YOU ADD NEW DATA TO THIS CLASS
	void CopyObjects(SGItemInfo* pItemInfo);

	void CopySubItemImageList(SGItemInfo* pItemInfo);

	void CopyControls(SGItemInfo* pItemInfo);

	void CopyColors(SGItemInfo* pItemInfo);


	//stephens added data
	RowPosition* _rowPosition; 

private:
	CONTROLTYPE m_enumCtrlType; 
	CMap<int,int, CONTROLTYPE, CONTROLTYPE&> m_controlType;//hmm
	CMap<int,int, COLORREF, COLORREF&> m_mapClf;//colors
	CMap<int,int, CStringList*, CStringList*> m_listdata;//listbox
	CString m_strItemName;//col 0...
	CStringArray m_SubItems;//col 1... N
	BOOL m_bCheck;
	int m_iImage;
	CMap<int,int,int,int&> m_iImageSubItems;
	COLORREF m_clf;
	COLORREF m_text_clf;

};




#endif // !defined(AFX_SGITEMINFO_H__B2F28C74_F06D_11D5_B53E_000255162CF0__INCLUDED_)
