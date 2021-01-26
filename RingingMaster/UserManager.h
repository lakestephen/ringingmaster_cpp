// UserManager.h: interface for the UserManager class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"	// Added by ClassView
#include "ManagerBase.h"
#include "UserManagerEventListener.h"

#pragma once

class UserManager : public CCmdTarget, public ManagerBase
{
public:
	void removeEventListener(UserManagerEventListener* userManagerEventListener);
	void addEventListener(UserManagerEventListener* userManagerEventListener);
	void saveActiveUserData();
	void loadActiveUserData();
	int getUserIndex(CString userName);
	bool checkAndCreateBaseDirectory();
	bool checkAndCorrectValidity();
	bool isValid();
	void load();
	void save();
	void deleteUser();
	void createUser();
	CString getActiveUserName();
	int getActiveUserIndex();
	bool setActiveUserName(CString userName);
	bool setActiveUserIndex(int activeUser);
	void setUserBasePath(CString basePath);
	CString getUserBasePath();
	void getUserList(CStringArray& strings, bool includeDeletedUsers	= false);
	UserManager();
	virtual ~UserManager();


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UserManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(UserManager)
		afx_msg void OnUserOptions();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

protected:
	
	void fireNotifyUserListChange();

	UserManagerEventListeners _listenerList;

	void sortUserList(CStringArray& users);
	int _activeUser;
	CString _userBasePath;
};

