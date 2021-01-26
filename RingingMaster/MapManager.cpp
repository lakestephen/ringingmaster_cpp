// MapManager.cpp: implementation of the MapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapManager.h"
#include "InternetManager.h"
#include "MapDoc.h"
#include "RecordTower.h"
#include "MapHostWrapper.h"
#include "UserMessages.h"
#include "OptionsDlg.h"
#include "key.h"
#include "MapDocumentInfo.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const CString MapManager::MapSection = "Maps";


MapManager::MapManager() :
_wrapper(NULL),
_bCheckForNewMapConVersion(false),
_cachePathChecked(false)
{
	_wrapper = new MapHostWrapper();

	//setup the worker thread
	_mapReqThread = static_cast<MapReqThread*>(
					AfxBeginThread(	RUNTIME_CLASS(MapReqThread),
									THREAD_PRIORITY_BELOW_NORMAL,
									0,		//stack size
									CREATE_SUSPENDED,
									NULL ));	//security attributes
	_mapReqThread->ResumeThread();
}

MapManager::~MapManager()
{

}

void MapManager::closeDown()
{
	CWaitCursor wait;
	//terminate the thread
	if (_mapReqThread)
	{
		MapReqThread* pMapReqThread = _mapReqThread;
		_mapReqThread = NULL;

		HANDLE mapReqThread = pMapReqThread->m_hThread;
		pMapReqThread->terminate();
		DWORD ret = ::WaitForSingleObject(mapReqThread, 10000);

		if (ret == WAIT_TIMEOUT)
			TRACE("MapThread terminate timed out");

	}

	delete _wrapper;
	_wrapper = NULL;

	_mapReqThread = NULL;

	//do not save cache where a successfull download has not taken place.
	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		if (!_mapCacheItems.GetAt(i)->isImageAvailable())
		{
			delete _mapCacheItems.GetAt(i);
			_mapCacheItems.RemoveAt(i--);
		}
	}	

	//delete extra disk usage over cache size
	trimMapCacheToSize();

	//write the map.dat
	CFile file;
	CString path = getCacheDataFilePath();

	if(!file.Open(path, CFile::modeWrite | CFile::modeCreate ))
	{
		TRACE("********SAVE:index Data not found********\n");
		return;
	}

	file.SetLength(0);

	CArchive ar(&file, CArchive::store);

	ar << _mapCacheItems.GetSize();

	//serialize cache
	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		_mapCacheItems.GetAt(i)->Serialize(ar);
		delete _mapCacheItems.GetAt(i);
	}
	_mapCacheItems.RemoveAll();

	//close down
	ar.Flush();
	file.Close();	
}


BEGIN_MESSAGE_MAP(MapManager, CCmdTarget)
	//{{AFX_MSG_MAP(MapManager)
	ON_COMMAND(ID_MAP_OPTIONS, OnMapOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void MapManager::load()
{
		//load cache index
	CFile file;
	CString path = getCacheDataFilePath();

	if(!file.Open(path,CFile::modeRead))
	{
		TRACE("********LOAD: Map Base Data not found********\n");
		return;
	}

	//load the towers into memory
	CArchive ar(&file, CArchive::load);

	int size = 0;
	ar >> size;

	//tidy cache (will write maps where necessary) and unload
	for (int i=0;i<size;i++)
	{
		MapCacheItem* item = new MapCacheItem;
		item->Serialize(ar);
		_mapCacheItems.Add(item);
	}

	//close 
	ar.Flush();
	file.Close();

	//set _NextMapId
   	for (int i=0;i<_mapCacheItems.GetSize();i++)
		MapCacheItem::updateNextMapId(_mapCacheItems.GetAt(i)->getMapId());

	//check all OK
	tidyCache();

}		   						  


void MapManager::tidyCache()
{
	//make sure that all files exist
   	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		if (!_mapCacheItems.GetAt(i)->isImageAvailable())
		{
			delete _mapCacheItems.GetAt(i);

			_mapCacheItems.RemoveAt(i--);
		}
	}

	//make sure that no extra files exist
	CFileFind fileFind;
	if (!fileFind.FindFile(getCachePath() + "*.*"))
		return;

	CStringArray files;
	BOOL caryOn = TRUE;

	while (caryOn)
	{
		caryOn = fileFind.FindNextFile();
		if ((!fileFind.IsDirectory())&&
			(!fileFind.IsDots()) && 
			(fileFind.GetFilePath() != getCacheDataFilePath()))
		{
			
			files.Add(fileFind.GetFilePath());
		}
	}			

	for (int i=0;i<files.GetSize();i++)
	{
		bool fileInUse = false;

		for (int j=0;j<_mapCacheItems.GetSize();j++)
		{
			if (_mapCacheItems.GetAt(j)->getPath() == files.GetAt(i))
			{
				fileInUse = TRUE;
				break;
			}
		}

		if (!fileInUse)
		{
			DeleteFile(files.GetAt(i));
			TRACE("tidyCache Deleting File : " + files.GetAt(i) + "\r\n");

		}
	}		  

}



void MapManager::trimMapCacheToSize()
{
	//remove any extra files Etc
	mainFrame()->closeAllMapDocuments();
	tidyCache();

	//get details of all files.
	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		CFileFind fileFind;
		MapCacheItem* item = _mapCacheItems.GetAt(i);
		if (fileFind.FindFile(item->getPath()))
		{
			fileFind.FindNextFile();
			CTime time;	   //does not need to be COleDateTime
			fileFind.GetLastAccessTime(time);
			item->setFileDetails((DWORD)fileFind.GetLength() , time);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	//sort on last accessed time
	qsort((void*)_mapCacheItems.GetData(), 
			  (size_t)_mapCacheItems.GetSize(), 
			  sizeof(MapCacheItem *), 
			  MapCacheItem::sortAccessTime);

	//remove oldest (item and file) first untill under the limit kill the item first to flush cache
	int cacheSize = getDiskCacheSizeMB() * 1024 * 1024;

	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		MapCacheItem* item = _mapCacheItems.GetAt(i);
		cacheSize -= item->getFileLength();
		if (cacheSize < 0)
		{
			item->destroyMap();
			delete item;
			_mapCacheItems.RemoveAt(i--);
		}
	}


}


void MapManager::addEventListener(MapManagerEventListener* mapManagerEventListener)
{
	ASSERT(mapManagerEventListener != NULL);

	_listenerList.Add(mapManagerEventListener);
}

void MapManager::removeEventListener(MapManagerEventListener *mapManagerEventListener)
{
	ASSERT(mapManagerEventListener!= NULL);

	int hasRemoved = 0;

	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == mapManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	}

	ASSERT(hasRemoved == 1);
}

//returns the map Id number
int MapManager::requestMapId(MapGridRef gridRef, ScaleType scale)
{
	MapCacheItem* request = new MapCacheItem(gridRef, scale, getCurentHostIndex());

	//can we get it locally out the cache
	int mapId = isMapInCache(*request);

	if (mapId == -1)
	{
		//add to cache
		_mapCacheItems.Add(request);
		return request->assignMapId();
	}
	else
	{
		//already in cache
		delete request;
		return mapId;
	}
}

void MapManager::requestFromInternet(int mapId)
{
		//check for new version here, as we do not want to do it in the Thread
	if (mapManager()->checkForNewMapConVersion(false))
		AfxMessageBox("Updated map connection details downloaded and installed.");

	mapManager()->_mapReqThread->PostThreadMessage(MAP_REQ_GET_MAP, NULL, (LONG)mapId);
}

int MapManager::getDiskCacheSizeMB()
{
	return app()->GetProfileInt(MapSection, "Cache Size MB Disk", 10);
}

void MapManager::setDiskCacheSizeMB(int size)
{
	ASSERT(size > 0);
	app()->WriteProfileInt(MapSection, "Cache Size MB Disk", max(0,size));
}


COLORREF MapManager::getPinColorGrabbed()
{
	return app()->GetProfileInt(MapSection, "Pin Color Grabbed", RGB(0,155,0));
}

void MapManager::setPinColorGrabbed(COLORREF color)
{
	app()->WriteProfileInt(MapSection, "Pin Color Grabbed", color);
	mainFrame()->fireUpdate();
}

COLORREF MapManager::getPinColorNonGrabbed()
{
	return app()->GetProfileInt(MapSection, "Pin Color Non Grabbed", RGB(0,0,255));
}

void MapManager::setPinColorNonGrabbed(COLORREF color)
{
	app()->WriteProfileInt(MapSection, "Pin Color Non Grabbed", color);

	mainFrame()->fireUpdate();
}


CDocument* MapManager::showTowerMapDoc(const RecordTower *recordTower)
{
	CWaitCursor wait;

	if ((recordTower)&&(recordTower->getMapRef().isValidOSGridRef())&&(isValid())) 
	{
		CDocument* pDoc = app()->openDocument(vt_map);
		MapDoc* mapDoc = dynamic_cast<MapDoc*>(pDoc);

		if (mapDoc)
		{
			mapDoc->setInitialData(recordTower);
			return mapDoc;
		}
	}

	return NULL;
}

CDocument* MapManager::showTowerMapDoc(MapDocumentInfo* info)
{
	CWaitCursor wait;

	if ((info)&&(info->getMapRef().isValidOSGridRef())&&(isValid())) 
	{
		CDocument* pDoc = app()->openDocument(vt_map);
		MapDoc* mapDoc = dynamic_cast<MapDoc*>(pDoc);

		if (mapDoc)
		{
			mapDoc->setInitialData(info);
			return mapDoc;
		}
	}

	return NULL;
}

BOOL MapManager::isValid()
{
	return _wrapper->isValid();
}

//returns true if updated.
//must be called from the main thread
bool MapManager::checkForNewMapConVersion(bool showMessages)
{
	CWaitCursor wait;

	if (_bCheckForNewMapConVersion)
		return false;

	_bCheckForNewMapConVersion = true;

	bool haveError = false;

	if (!internetManager()->checkConnection(false))
	{
		if (showMessages)
			AfxMessageBox("Please establish an internet connection. \nSee Tools > Options > Internet");
		return false;
	}

	//get the version information off the web site, check if we have the latest version

	if (isNewerMapConVersionAvailable())
	{
		//replace version. This should free the dll.
		delete _wrapper;
		_wrapper = NULL;

		//download the new version
		DWORD length = 0;
		BYTE* buffer = internetManager()->getHttpFIle("http://homepages.nildram.co.uk/~thelakes/MapCon.dll", length);

		if (length > 1024 * 100)	//the dll will be at least 100 k
		{
			//copy in new version
			CFile file;
			CFileException ex;
			CString path = app()->_rootPath + "\\MapCon.dll";

			if(file.Open(path,CFile::modeWrite|CFile::modeCreate), &ex)
			{
				try
				{
					file.Write(buffer, length);
					file.Close();
				}
				catch (CFileException* e)
				{
					AfxMessageBox("Cant Write \'MapCon.dll\'.");
					e->ReportError();
					haveError = true;
					e->Delete();
				}
			}
			else
			{
				AfxMessageBox("Cant Open MapCon.dll\'.");
				ex.ReportError();
				haveError = true;
			}

			//restart
			_wrapper = new MapHostWrapper();

			if (mainFrame())
				mainFrame()->fireUpdate(); 

			//clean up
			destroyMapCache(true);
			setCurrentHostIndex(0);
			delete[] buffer;

			if (!haveError)
			{
				if (showMessages)
					AfxMessageBox("Updated map service provider file downloaded and installed.");
				return true;
			}

		}
	}

	if ((showMessages) &&(!haveError))
		AfxMessageBox("You already have the latest map service provider file." );

	return false;
}

bool MapManager::isNewerMapConVersionAvailable()
{
	CString page = internetManager()->getPage("http://homepages.nildram.co.uk/~thelakes/mapcon.xml");

	if (!page.IsEmpty())
	{

	/*	CString page =
		"<mapcon> \r\n" \
		"\t <interface>1</interface> \r\n" \
		"\t <version>2</version>" \
		"</mapcon>" \

		"<mapcon> \r\n" \
		"\t <interface>2</interface> \r\n" \
		"\t <version>4</version>" \
		"</mapcon>";
	 */
//		TRACE(page+ "\r\n");

		if (page.GetLength() >0)
		{
			while(page.Replace("\r", ""));
			while(page.Replace("\n", ""));
			while(page.Replace("\t", ""));
			while(page.Replace(" ", ""));

			//note that this token defines the interface number that we can cope with.

			CString token =  "<mapcon><interface>1</interface><version>";

			int offset = 0;

			while ((offset = page.Find(token, offset)) != -1)
			{
				offset += token.GetLength();

				CString snip = page.Mid(offset, 3);
				TRACE(snip + "\r\n");

				while(snip.Replace("<", ""));
				while(snip.Replace("/", ""));
				while(snip.Replace("v", ""));
				while(snip.Replace("e", ""));

				if (snip.GetLength() >0)
				{
					int availableVersion = atoi(snip);

					if (availableVersion > 0)
					{
						int dllVersion = _wrapper->getDLLVersion();

						if (dllVersion < availableVersion)
							return true;
					}
				}
			}
		}
	}

	return false;
}


void MapManager::mapLoadedFromThread(int mapId)
{
	//inform waiting documents
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->mapManager_notifyMapLoaded(mapId);
	}
}

void MapManager::openCurrentMapProviderWebSite()
{
	if (isValid())
	{
		app()->launchBrowser(_wrapper->getHostWebAddress(getCurentHostIndex()));
	}
}


CString MapManager::getCurrentProviderName()
{
	if (isValid())
	{
		return _wrapper->getHostName(getCurentHostIndex());
	}
	
	return "";
	
}


void MapManager::getProviderList(CStringArray& strings)
{
	if (isValid())
	{
		for (int i=0;i<_wrapper->getHostCount();i++)
		{
			strings.Add(_wrapper->getHostName(i));
		}
	}
}

void MapManager::getSupportedScaleList(ScaleTypes &scaleTypes)
{
	scaleTypes.RemoveAll();
	
	if (isValid())
	{
		for (int i=s_2_500;i<s_none;i++)
		{
			if (_wrapper->isScaleSupported(getCurentHostIndex(), (ScaleType)i))
			{
				scaleTypes.Add((ScaleType)i);
			}
		}
	}
}


BOOL MapManager::isShowingPushPins()
{
	return app()->GetProfileInt(MapSection, "Showing PushPins", TRUE);
}

void MapManager::toggleShowingPushPins()
{
	BOOL showingPushPins = !isShowingPushPins();
	app()->WriteProfileInt(MapSection, "Showing PushPins", showingPushPins);
	mainFrame()->fireUpdate();
}

BOOL MapManager::isShowingMap()
{
	return app()->GetProfileInt(MapSection, "Showing Map", TRUE);
}

void MapManager::toggleShowingMap()
{
	BOOL showingMap = !isShowingMap();
	app()->WriteProfileInt(MapSection, "Showing Map", showingMap);
	mainFrame()->fireUpdate();
}


void MapManager::setDefaultScale(ScaleType scale)
{
	app()->WriteProfileInt(MapSection, "Default Map Scale", scale);
}

ScaleType MapManager::getDefaultScale()
{
	return (ScaleType)app()->GetProfileInt(MapSection, "Default Map Scale", s_25_000);
}


int MapManager::getCurentHostIndex()
{
	int providerIndex = app()->GetProfileInt(MapSection, "Current Provider", 0);

	if ((isValid())&&(providerIndex >=0)&&(providerIndex < _wrapper->getHostCount()))
		return providerIndex;

	return 0;
}

void MapManager::setCurrentHostIndex(int index)
{
	int providerIndex = 0;

	if ((isValid())&&(index >=0)&&(index < _wrapper->getHostCount()))
		providerIndex = index;

	app()->WriteProfileInt(MapSection, "Current Provider", providerIndex);

	mainFrame()->fireUpdate();
}

CString MapManager::getDLLVerison()
{
	CString spVersion;

	if (isValid())
		spVersion.Format("%d", _wrapper->getDLLVersion());

	return spVersion;
}


MapHostWrapper* MapManager::getMapHostWrapper()
{
	return _wrapper;
}

MapCacheItem* MapManager::getMapCacheItem(int mapId)
{
	//to2do convert to a CMap

	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		if (_mapCacheItems.GetAt(i)->getMapId() == mapId)
			return _mapCacheItems.GetAt(i);
	}

	return NULL;
}


// return true if in cache
int MapManager::isMapInCache(const MapCacheItem& request)
{
	//go through each map and see if it is the same map
	//if available, return the mapID.

	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		if (_mapCacheItems.GetAt(i)->isSameMap(request))
		{
			return _mapCacheItems.GetAt(i)->getMapId();
		}
	}
	
	return -1;
}

void MapManager::destroyMapCache(bool includeDocumets)
{
	//close all map documents
	if (includeDocumets)
		mainFrame()->closeAllMapDocuments();
	
	tidyCache();

	Ints mapIds;
	mainFrame()->getAllUsedMaps(mapIds);		

	// delete all cache items. 
	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		bool mapInUse = false;
		MapCacheItem* item = _mapCacheItems.GetAt(i);
		for(int j=0;j<mapIds.GetSize();j++)
		{
			if (item->getMapId() == mapIds.GetAt(j))
				mapInUse = true;
		}

		if (!mapInUse)
		{
			item->destroyMap();
			delete item;
			_mapCacheItems.RemoveAt(i--);
		}								 		
	}									 
}


void MapManager::cacheTimerEvent()
{
	//get a list of all used references 
	Ints mapIds;
	mainFrame()->getAllUsedMaps(mapIds);

	//now free if not on used list
	for (int i=0;i<_mapCacheItems.GetSize();i++)
	{
		MapCacheItem* item = _mapCacheItems.GetAt(i);
		
		bool mapInUse = false; 

		for (int j=0;j<mapIds.GetSize();j++)
			if (item->getMapId() == mapIds.GetAt(j))
				mapInUse = true;

		if (!mapInUse)
			item->free();
	} 
}

CString MapManager::getCachePath()
{
	if (!_cachePathChecked)
	{
		//check that the cache path exists
		CString path = app()->_rootPath + "Cache";	//to2do move to data area

		CFileFind fileFind;
		if (!fileFind.FindFile(path + "\\."))
		{
			if (!::CreateDirectory(path , NULL))
			{
				AfxMessageBox("Unable to create cache directory: " + path);
				return "";
			}
		}

		_cachePathChecked = true;
	}

	return app()->_rootPath + "Cache\\";//to2do move to data area  
}


CString MapManager::getScaleString(ScaleType scale)
{
	CString	strScale;
	switch (scale)
	{

		case s_2_500:		strScale = "2500";    break;	
		case s_5_000:		strScale = "5000";    break;
		case s_10_000:		strScale = "10000";    break;
		case s_25_000:		strScale = "25000";    break;
		case s_50_000:		strScale = "50000";    break;
		case s_100_000:		strScale = "100000";    break;
		case s_200_000:		strScale = "200000";    break;
		case s_250_000:		strScale = "250000";    break;
		case s_500_000:		strScale = "500000";    break;
		case s_1000_000:	strScale = "1000000";    break;
		case s_2000_000:	strScale = "2000000";    break;
		case s_4000_000:	strScale = "4000000";    break;
		case s_none:
		default:			strScale = "";    break;
	}

	return strScale;
}




CString MapManager::getCacheDataFilePath()
{
	return getCachePath() + "map.dat";
}

void MapManager::OnMapOptions() 
{		 
	OptionsDisplayFilter filter(false);
	filter._map = true;
	filter._internet = true;
	filter._displayScreenType = os_Map;
	OptionsDlg propSheet(mainFrame(), filter);
	propSheet.DoModal();	
}
