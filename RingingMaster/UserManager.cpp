// UserManager.cpp: implementation of the UserManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "UserManager.h"
#include "RecordManager.h"
#include "GenericEditDlg.h"
#include "ShellFileOp.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UserManager::UserManager() :
_activeUser(-1)
{

}

UserManager::~UserManager()
{

}

BEGIN_MESSAGE_MAP(UserManager, CCmdTarget)
	//{{AFX_MSG_MAP(UserManager)
	ON_COMMAND(ID_USER_OPTIONS, OnUserOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

	

void UserManager::addEventListener(UserManagerEventListener* userManagerEventListener)
{
	ASSERT(userManagerEventListener != NULL);

	_listenerList.Add(userManagerEventListener);
}

void UserManager::removeEventListener(UserManagerEventListener* userManagerEventListener)
{
	ASSERT(userManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == userManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}

void UserManager::fireNotifyUserListChange()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->userManager_notifyUserListChange(); 
	}  
	
}

void UserManager::getUserList(CStringArray& users, bool includeDeletedUsers)
{
	users.RemoveAll();

	//if the base path is not valid, return nothing
	CFileFind fileFind;
	if (!fileFind.FindFile(_userBasePath + "\\."))
		return;			  				   

	BOOL caryOn = TRUE;

	if (!fileFind.FindFile(_userBasePath + "\\*.*"))
		return;

	while (caryOn)
	{
		caryOn = fileFind.FindNextFile();
		if ((fileFind.IsDirectory())&&(!fileFind.IsDots()))
			users.Add(fileFind.GetFileName());
	}	

	//remove inactive users
	if (!includeDeletedUsers)
	{
		for (int i=0;i<users.GetSize();i++)
		{
			if (users.GetAt(i).Find("~") != -1)
			{
				users.RemoveAt(i);
				i--;
			}
		}
	}

	sortUserList(users);
}				

CString UserManager::getUserBasePath()
{
	return _userBasePath;
}

void UserManager::setUserBasePath(CString basePath)
{
	ASSERT(!basePath.IsEmpty());

	while((basePath.GetLength() > 0)&&
		  (basePath.GetAt(basePath.GetLength()-1) == '\\'))
	{
		basePath.Delete(basePath.GetLength()-1,1);
	}
	
	ASSERT(!basePath.IsEmpty());
	if (_userBasePath != basePath)
	{
		_userBasePath = basePath;

		//if the dir is valid, then load user data
		CFileFind fileFind;
		if (fileFind.FindFile(_userBasePath + "\\."))
		{
			recordManager()->deleteUserData();
			CStringArray strings;
			getUserList(strings);

			if (strings.GetSize() >0)
			{
				_activeUser = 0;
				loadActiveUserData();
			}
			else
				_activeUser = -1;
			
		}
	}
}

bool UserManager::setActiveUserName(CString userName)
{
	return setActiveUserIndex(getUserIndex(userName));
}

//returns true if a change has been made
bool UserManager::setActiveUserIndex(int activeUser)
{
	CWaitCursor wait;
	
	//do we need to do this??
	if (_activeUser == activeUser)
		return false;

	saveActiveUserData();
	recordManager()->deleteUserData();//this is needed if active user is now -1
	
	_activeUser = activeUser;

	loadActiveUserData();

	fireNotifyUserListChange();

	return true;
}

int UserManager::getActiveUserIndex()
{
	CStringArray users;
	getUserList(users);

	if ((_activeUser < 0 )||(_activeUser > users.GetSize())||(users.GetSize() == 0))
		return -1;
	else
		return _activeUser;
}

void UserManager::createUser()
{
	if (getUserBasePath().IsEmpty())
	{
		AfxMessageBox("Please choose a base directory before creating a user.");
		return;
	}

	//check if base dir is valid
	if (!checkAndCreateBaseDirectory())
		return;

	//create user dir.
	GenericEditDlg dlg(NULL, "Choose New User Name", "Enter the name of a new user to be created.", "Default User" );
	while (dlg.DoModal() == IDOK)
	{
		CString msg;
		if (!GlobalFunctions::isIligalChar(dlg._strGenericEdit, msg))
		{
			//are we unique
			CStringArray strings;
			getUserList(strings, true);

			CString name1 = dlg._strGenericEdit;
			name1.MakeLower();

			for (int i=0;i<strings.GetSize();i++)
			{
				CString name2 = strings.GetAt(i);
				name2.MakeLower();
				name2.Replace("~", "");
				if (name1 == name2)
				{
					AfxMessageBox("Please choose a unique name. \nNames must not be in current use, or previously been deleted.");
					return;
				}
			}

			//formalise name
			dlg._strGenericEdit = GlobalFunctions::titleString(dlg._strGenericEdit);

			//this is needed to stop the user data swapping users
			setActiveUserIndex(-1); 

			//create the directory
			if (::CreateDirectory(_userBasePath + "\\" + dlg._strGenericEdit, NULL) != TRUE)
			{
				AfxMessageBox("Unable to create user directory: " + _userBasePath + "\\" + dlg._strGenericEdit);
				return;
			}
			
			//select new user
			setActiveUserIndex(getUserIndex(dlg._strGenericEdit));	 

			fireNotifyUserListChange();

			return;
		}	
		else 
		{
			AfxMessageBox(msg);
		}
	}			   
}

void UserManager::deleteUser()
{
	if (_activeUser == -1)
	{
		AfxMessageBox("Please select a user to delete");
		return;
	}

	CStringArray strings;
	getUserList(strings);

	if (strings.GetSize() <=1)
	{
		AfxMessageBox("You must leave at least one user on the system.");
		return;
	}


	CString msg1;
	
	msg1.Format("When a user is deleted, all the data associated with that user is hidden. \nAre you sure you want to proceed?");

	if (AfxMessageBox(msg1, MB_YESNO| MB_ICONQUESTION) == IDYES)		
	{
		//save data
		saveActiveUserData();

		// unload data
		recordManager()->deleteUserData();

		//remove directory
		CString from = getUserBasePath() + "\\" + getActiveUserName() ;
		CString to = getUserBasePath() + "\\~" + getActiveUserName() ;
		
		{
			CShellFileOp fileOP(mainFrame()->m_hWnd, FO_RENAME, from, to, NULL, "Renameing");
		}

		//set active user
		CStringArray strings;
		getUserList(strings);

		if (strings.GetSize() > 0)
		{
			_activeUser = 0; 
			loadActiveUserData();
		}
		else
			_activeUser = -1;

		fireNotifyUserListChange();
	}	
}

void UserManager::sortUserList(CStringArray &users)
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<users.GetUpperBound();i++)
		{
			if (users[i] > users[i+1])
			{
				changed = true;
				CString temp    = users.GetAt(i);
				users.SetAt(i,users.GetAt(i+1));
				users.SetAt(i+1, temp);
			}
		}
	}
}

void UserManager::save()
{
	if (!_userBasePath.IsEmpty()) //this can be empty when the loadusers function has not been called
	{
		app()->WriteProfileInt("Users", "Active", _activeUser);
		app()->WriteProfileString("Users", "Base Path", _userBasePath);
		saveActiveUserData();
	}						 
	
}

void UserManager::load()
{
	_activeUser = app()->GetProfileInt("Users", "Active", 0);
	if (app()->_windowsVersion == VER_PLATFORM_WIN32_WINDOWS)
	{
		_userBasePath = app()->GetProfileString("Users", "Base Path", "C:\\My Documents\\Ringing Master User Data");
	}
	else if (app()->_windowsVersion == VER_PLATFORM_WIN32_NT)
	{
		_userBasePath = app()->GetProfileString("Users", "Base Path", "C:\\Documents and Settings\\All Users\\Application Data\\Ringing Master User Data");
	}
		

	ASSERT(!_userBasePath.IsEmpty());

	loadActiveUserData();
}

bool UserManager::isValid()
{
	if (getActiveUserIndex() == -1)
		load();

	CStringArray users;
	getUserList(users);

	return !((getUserBasePath().IsEmpty())||
			 (users.GetSize() == 0)||
			 (getActiveUserIndex() == -1));
}

bool UserManager::checkAndCorrectValidity()
{
	if (isValid())
		return true;

	AfxMessageBox("Please add or select a user.", MB_ICONHAND);
	OnUserOptions();

	return isValid();
}

bool UserManager::checkAndCreateBaseDirectory()
{
	CFileFind fileFind;
	if (!fileFind.FindFile(_userBasePath + "\\."))
	{
		CString msg;
		msg.Format("The base directory \"%s\" is not valid. Ok to create?", _userBasePath);
		if (AfxMessageBox(msg , MB_YESNO|MB_ICONQUESTION) != IDYES)
		{
			AfxMessageBox("Unable to create base directory.");
			return false;
		}

		//create the dir
		if (RingingMasterApp::CreateAllDirectories(_userBasePath.GetBuffer(_userBasePath.GetLength())) != TRUE)
		{
			_userBasePath.ReleaseBuffer();
			AfxMessageBox("Unable to create base directory.");
			return false;
		}
		_userBasePath.ReleaseBuffer();
	}
	return true;
}

int UserManager::getUserIndex(CString userName)
{
	CStringArray users;

	getUserList(users);

	for (int i=0;i<users.GetSize();i++)
	{
		if (users.GetAt(i) == userName)
			return i;
	}
	return -1;
}

CString UserManager::getActiveUserName()
{
	CStringArray users;

	getUserList(users);
	
	int index = getActiveUserIndex();
	if ((index >=0)&&(index < users.GetSize()))
		return users.GetAt(index);
	return "";
}

void UserManager::loadActiveUserData()
{
	if (recordManager()->isValid())
	{	
		//load in the user tower data

		CFile file;
		CString path = getUserBasePath() + "\\" + getActiveUserName() + "\\" + "user.dat";
		
		if((!file.Open(path,CFile::modeRead))||(getActiveUserName().IsEmpty()))
		{
			//get a basic load as this is a valid condition when
			// we are creating a new user
			recordManager()->setHomeTower(recordManager()->getHomeTowerIndex());
			recordManager()->createAllTowers();
			//todo recordManager()->resetFilter(vt_tower);
			//todo recordManager()->resetFilter(vt_grab);

			TRACE("********LOAD: Tower User not found********\n");
			return;
		}

		CArchive ar(&file, CArchive::load);

		recordManager()->SerializeUserData(ar);

		//close down
		ar.Flush();
		file.Close();

		fireNotifyUserListChange();
	}
}

void UserManager::saveActiveUserData()
{
	//do not save back an load that has failed.
	if (!recordManager()->isValid())
	{
		TRACE("********SAVE: No Tower Data********\n");
		return;
	}

	if (getActiveUserName().IsEmpty())
	{
		TRACE("********SAVE:User Name not found********\n");
		return;
	}
	
	CFile file;
	CString path = getUserBasePath() + "\\" + getActiveUserName() + "\\" + "user.dat";

	if(!file.Open(path, CFile::modeWrite | CFile::modeCreate ))
	{
		TRACE("********SAVE:User Data not found********\n");
		return;
	}

	file.SetLength(0);

	CArchive ar(&file, CArchive::store);

	recordManager()->SerializeUserData(ar);

	//close down
	ar.Flush();
	file.Close();
}

void UserManager::OnUserOptions() 
{		 
	OptionsDisplayFilter filter(false);
	filter._users = true;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();	
}
