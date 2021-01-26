// ExplorerFolder.h: interface for the ExplorerFolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ExplorerFolder_H__24D073E8_AECC_11D5_BB7F_CBFD6250B474__INCLUDED_)
#define AFX_ExplorerFolder_H__24D073E8_AECC_11D5_BB7F_CBFD6250B474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Notation;

class ExplorerFolder  
{
public:
	void setID(unsigned long ID);
	int getLevel();
	unsigned long getID();
	void setLevel(int level);
	static unsigned long createUniqueUserID();
	static unsigned long createUniqueCCID();
	CString getName();
	void setName(const CString& name);
	virtual void Serialize(CArchive& ar);
	ExplorerFolder();
	virtual ~ExplorerFolder();

	static unsigned long _userUniqueId;
	static unsigned long _CCUniqueId;
	const static unsigned long _pivotPoint;
protected:
	unsigned long _id;
	int _level;
	CString _name;
	BOOL _editable;
	BOOL _canCreateChildren;
};

typedef CArray<ExplorerFolder*, ExplorerFolder*> ExplorerFolders;

#endif // !defined(AFX_ExplorerFolder_H__24D073E8_AECC_11D5_BB7F_CBFD6250B474__INCLUDED_)
