// MethodManager.h: interface for the MethodManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NotationLite.h"
#include "ExplorerFolder.h"
#include "ManagerBase.h"

class MethodManager : public CCmdTarget, public ManagerBase
{
public:

	void save(){};
	void load(){};
    
	void resetFolderNameCache();
	void addMethodFolder(ExplorerFolder* folder);
	void addMethod(NotationLite* lite);
	void removeAllMethods();
	void removeMethod(int i);
	void removeAllFolders();
	int getFolderCount();
	ExplorerFolder* getFolder(int i);
	int getMethodCount();
	NotationLite* getMethod(int i);
	MethodManager();
	virtual ~MethodManager();

	void saveExplorerData(bool user = true, bool doDelete = true);
	void loadExplorerData();
	void deleteExplorerData();
	CString getFolderPath(unsigned long folderID);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(MethodManager)
	afx_msg void OnImportLiberary();
	afx_msg void OnMethodOptions();
	afx_msg void OnToolsDatabasemaintainance();
	//}}AFX_MSG



	DECLARE_MESSAGE_MAP()

	
protected:				
	void doLoadExplorerData(bool user);

private:	
	bool _explorerDataLoaded;
	NotationLites _explorerMethods;
	ExplorerFolders _explorerFolders;

	CMap<unsigned long,unsigned long, CString, LPCSTR> _folderNameCache;

};


