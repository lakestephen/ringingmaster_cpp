// TowerTTSItem.h: interface for the TowerTTSItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWERTTSITEM_H__F43C58EE_BC68_4477_8D43_0049D0C130CB__INCLUDED_)
#define AFX_TOWERTTSITEM_H__F43C58EE_BC68_4477_8D43_0049D0C130CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum TowerTTSTypes
{
	tttst_start, 
	tttst_go, 
	tttst_thatsAll, 
	tttst_rounds,
	tttst_call,
	tttst_splice,
	tttst_part,	
	tttst_stand,

	//only add new ones to the end of this list, and change the serialisation 
	// if needed, priovide a specific sort mechanisam

	//also if you add new items, then the deserialisation of earlier versions will have to 
	// add the extra items into the list.
};

class TowerTTSItem  
{
public:
	static CString getItemTypeText(TowerTTSTypes type);
	void Serialize(CArchive &ar);
	TowerTTSItem(TowerTTSTypes type) ;
	TowerTTSItem();
	TowerTTSItem(const TowerTTSItem& towerTTSItem);
	
	bool operator !=(const TowerTTSItem& towerTTSItem);
	bool operator ==(const TowerTTSItem& towerTTSItem);

	virtual ~TowerTTSItem() {};

	
	TowerTTSTypes _type;
	BOOL _active;
	CString _string;
};

typedef CArray<TowerTTSItem*, TowerTTSItem*> TowerTTSItems;

#endif // !defined(AFX_TOWERTTSITEM_H__F43C58EE_BC68_4477_8D43_0049D0C130CB__INCLUDED_)
