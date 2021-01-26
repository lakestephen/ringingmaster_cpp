// MethodManager.cpp: implementation of the MethodManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MethodManager.h"
#include "NotationExplorerDlg.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MethodManager::MethodManager() :
_explorerDataLoaded(false)
{

}

MethodManager::~MethodManager()
{
	saveExplorerData();
	deleteExplorerData();
}


BEGIN_MESSAGE_MAP(MethodManager, CCmdTarget)
	//{{AFX_MSG_MAP(MethodManager)
	ON_COMMAND(IDC_IMPORT_LIBERARY, OnImportLiberary)
	ON_COMMAND(ID_METHOD_OPTIONS, OnMethodOptions)
	ON_COMMAND(ID_TOOLS_DATABASEMAINTAINANCE, OnToolsDatabasemaintainance)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void MethodManager::loadExplorerData()
{
	if(_explorerDataLoaded)
		return;

	_explorerDataLoaded = true;

	CWaitCursor wait;
	
	if(app()->GetProfileInt("MainFrame", 
									"Include CC Library", TRUE))
		doLoadExplorerData(false); //load the cc data first

	doLoadExplorerData(true);

}

   
void MethodManager::doLoadExplorerData(bool user)
{

	CFile file;
	CString path;
	if (user)
		path = app()->_rootPath + "UserLib.dat"; //to2do move to data area
	else
		path = app()->_rootPath + "CCLib.dat";

	if(!file.Open(path,CFile::modeRead))
	{
		if (user)
		{
			ExplorerFolder* folder;
			folder = new ExplorerFolder;
			folder->setName("User Library");
			folder->setLevel(1);
			folder->setID(ExplorerFolder::createUniqueUserID());
			_explorerFolders.Add(folder);

			TRACE("********LOAD: Explorer User Data not found********\n");
		}
		else
		{
			TRACE("********LOAD: Explorer CC Data not found********\n");
		}

		return;
	}

	CArchive ar(&file, CArchive::load);

	//load the directorys
	ExplorerFolder* folder;
	int size;
	ar >> size;
	for (int i=0;i<size;i++)
	{
		folder = new ExplorerFolder;
		folder->Serialize(ar);
		_explorerFolders.Add(folder);
	}

	//load the methods
	NotationLite* not;
	ar >> size;
	for (int i=0;i<size;i++)
	{
		not = new NotationLite;
		not->Serialize(ar);
		_explorerMethods.Add(not);
	}

	//close down
	ar.Flush();
	file.Close();
}

void MethodManager::saveExplorerData(bool user, bool doDelete)
{
	if(!_explorerDataLoaded)
		return;
	
	_explorerDataLoaded = false;

	CWaitCursor wait;

	CFile file;
	CString path;
	if (user)
		path = app()->_rootPath + "UserLib.dat"; //to2do move to data area
	else
		path = app()->_rootPath + "CCLib.dat";

	//make a seperate listof Notations and folders that are of the correct type
	ExplorerFolders foldersForSave;
	for(int i=0;i<_explorerFolders.GetSize();i++)
	{
		ExplorerFolder* folder = _explorerFolders.GetAt(i);
		if (((folder->getID() > ExplorerFolder::_pivotPoint)&&( user))||
			((folder->getID() < ExplorerFolder::_pivotPoint)&&(!user)))
			foldersForSave.Add(folder);
	}
	NotationLites methodsForSave;
	for(int i=0;i<_explorerMethods.GetSize();i++)
	{
		NotationLite* not = _explorerMethods.GetAt(i);

		if (((not->getFolderID() > ExplorerFolder::_pivotPoint)&&( user))||
			((not->getFolderID() < ExplorerFolder::_pivotPoint)&&(!user)))
			methodsForSave.Add(not);
	}
	
	if ((methodsForSave.GetSize() == 0)&&(foldersForSave.GetSize() == 0))
	{
		TRACE("********No method or folder data to save********\n");
		return;
	}

	if(!file.Open(path, CFile::modeWrite | CFile::modeCreate ))
	{
		CString msg;
		msg.Format("ERROR: Unable to open %s for save", path);
		AfxMessageBox(msg);
		TRACE("********SAVE:Method Data not found********\n");
		return;
	}

	file.SetLength(0);

	CArchive ar(&file, CArchive::store);

	//archive the explorer folders
	ar << foldersForSave.GetSize();
	for(int i=0;i<foldersForSave.GetSize();i++)
	{
		foldersForSave.GetAt(i)->Serialize(ar);
	}

	//archive the explorer notations
	ar << methodsForSave.GetSize();
	for(int i=0;i<methodsForSave.GetSize();i++)
	{
		methodsForSave.GetAt(i)->Serialize(ar);
	}

	//close down
	ar.Flush();
	file.Close();

	//only ever need to save user OR cc data
	//Must do this to get the option of showing CC Lib to work instantly
	//only dont delete when in debug, and modifying the cclib using the override functions
	if (doDelete)
		deleteExplorerData();
	else
		_explorerDataLoaded = true; //as we have not deleted
}


void MethodManager::deleteExplorerData()
{
	//remove the explorer folders
	for(int i=0;i<_explorerFolders.GetSize();i++)
	{
		delete _explorerFolders.GetAt(i);
	}
	_explorerFolders.RemoveAll();

	//remove the explorer notations
	for(int i=0;i<_explorerMethods.GetSize();i++)
	{
		delete _explorerMethods.GetAt(i);
	}
	_explorerMethods.RemoveAll();
}

CString MethodManager::getFolderPath(unsigned long folderID)
{
	//try and get from the cache
	CString folderPath;
	if (_folderNameCache.Lookup(folderID, folderPath))
		return folderPath;
	
	bool found = false;
	int i;
	for (i=0;i<_explorerFolders.GetSize();i++)
	{
		ExplorerFolder* folder = _explorerFolders.GetAt(i);
		if (folder->getID() == folderID)
		{
			//we have found the folder
			found = true;
			break;
		}
	}
	

	if (found)
	{
		CString folderName = _explorerFolders.GetAt(i)->getName();
		int level = _explorerFolders.GetAt(i)->getLevel();

		//work our way up the tree
		for(;i>=0;i--)
		{
			if (_explorerFolders.GetAt(i)->getLevel() == level -1)
			{
				level = _explorerFolders.GetAt(i)->getLevel();
				folderName = _explorerFolders.GetAt(i)->getName() + "\\" + folderName;

			}
			if (level == 1)
				break;
		}
		//write to the cache
		_folderNameCache.SetAt(folderID, folderName);

		//return
		return folderName;

	}

	return "";
}

void MethodManager::OnImportLiberary() 
{
	if (AfxMessageBox("Warning: Importing a new library will delete all existing libraries. \n Do you wish to continue?",
		MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		//save the user data
		saveExplorerData();
		deleteExplorerData();

		//reset the unique ID
		ExplorerFolder::_CCUniqueId = ExplorerFolder::_pivotPoint - 1;
		
		NotationLites notations;  // just a dummy
		NotationExplorerDlg dlg(mainFrame(), notations, false, false, 8, true);

		dlg.DoModal();
		
		for (int i=0;i<notations.GetSize();i++)
			delete notations.GetAt(i);	

		//save the created CC data, then delete. 
		//this will force a reload of all data, including user data, then terminate
		_explorerDataLoaded = true; //to allow a save
		saveExplorerData(false);
		deleteExplorerData();

	}	
}



NotationLite* MethodManager::getMethod(int i)
{
	return _explorerMethods.GetAt(i);
}

int MethodManager::getMethodCount()
{
	return _explorerMethods.GetSize();
}

ExplorerFolder* MethodManager::getFolder(int i)
{
	return _explorerFolders.GetAt(i);
}

int MethodManager::getFolderCount()
{
	return _explorerFolders.GetSize();
}

void MethodManager::removeAllFolders()
{
	_explorerFolders.RemoveAll();
}

void MethodManager::removeMethod(int i)
{
	_explorerMethods.RemoveAt(i);
}

void MethodManager::removeAllMethods()
{
	_explorerMethods.RemoveAll();
}

void MethodManager::addMethod(NotationLite *lite)
{
	_explorerMethods.Add(lite);
}

void MethodManager::addMethodFolder(ExplorerFolder *folder)
{	   
	_explorerFolders.Add(folder);
}

void MethodManager::resetFolderNameCache()
{
	_folderNameCache.RemoveAll();
}

void MethodManager::OnMethodOptions() 
{
	OptionsDisplayFilter filter(false);
	filter._method= true;
	filter._print= true;
	filter._displayScreenType = os_Method;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();
}


void MethodManager::OnToolsDatabasemaintainance() 
{
	NotationLites notations;
	NotationExplorerDlg dlg(mainFrame(), notations, true);
	dlg.DoModal();	
}
