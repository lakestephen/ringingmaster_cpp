// MapDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapDoc.h"
#include "MapManager.h"
#include "RecordManager.h"
#include "PrintData.h"
#include "MapView.h"
#include "MapDocumentInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapDoc

IMPLEMENT_DYNCREATE(MapDoc, CDocument)

MapDoc::MapDoc() :
_mapId(-1),
_selectedTowerIndex(0),
_contextTower(NULL)
{
	mapManager()->addEventListener(this);
	mainFrame()->addEventListener(this);
	recordManager()->addEventListener(this);
}

BOOL MapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;  

	SetTitle("Map");

	return TRUE;
}

MapDoc::~MapDoc()
{
	mapManager()->removeEventListener(this);
	mainFrame()->removeEventListener(this);
	recordManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(MapDoc, CDocument)
	//{{AFX_MSG_MAP(MapDoc)
	ON_COMMAND(ID_MAP_EAST, OnMapEast)
	ON_COMMAND(ID_MAP_NORTH, OnMapNorth)
	ON_COMMAND(ID_MAP_SOUTH, OnMapSouth)
	ON_COMMAND(ID_MAP_WEST, OnMapWest)
	ON_COMMAND(ID_RELOAD_MAP, OnReloadMap)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapDoc diagnostics

#ifdef _DEBUG
void MapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void MapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MapDoc serialization

void MapDoc::Serialize(CArchive& /*ar*/)
{
	//we do not serialise this data
	AfxThrowArchiveException(CArchiveException::endOfFile);	
}

/////////////////////////////////////////////////////////////////////////////
// MapDoc commands

BOOL MapDoc::OnSaveDocument(LPCTSTR /*lpszPathName*/) 
{
	return TRUE;	
//	return CDocument::OnSaveDocument(lpszPathName);
}

void MapDoc::setInitialData(const RecordTower* tower) 
{
	SetTitle("Map of " + tower->getDisplayName());

	requestMapId(tower->getMapRef(), mapManager()->getDefaultScale());

	_selectedTowerIndex = tower->getIndex();
}

void MapDoc::setInitialData(MapDocumentInfo* info)
{
	SetTitle(info->getName());

	requestMapId(info->getMapRef(), mapManager()->getDefaultScale());

	_selectedTowerIndex = info->getSelectedTowerIndex();
}

void MapDoc::setNewScale(ScaleType scale)
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	if (mapCacheItem)
		requestMapId(mapCacheItem->getCenterOfMap(), scale);
}

void MapDoc::OnReloadMap() 
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	if (mapCacheItem)
	{
		mapCacheItem->destroyMap();	
		mainFrame()->fireUpdate();
	}	
}

void MapDoc::OnMapEast() 
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	requestMapId(mapCacheItem->getNextMap(d_e), mapCacheItem->getScale());
}

void MapDoc::OnMapNorth() 
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	requestMapId(mapCacheItem->getNextMap(d_n), mapCacheItem->getScale());
}

void MapDoc::OnMapSouth() 
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	requestMapId(mapCacheItem->getNextMap(d_s), mapCacheItem->getScale());
}

void MapDoc::OnMapWest() 
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	requestMapId(mapCacheItem->getNextMap(d_w), mapCacheItem->getScale());
}

void MapDoc::requestMapId(MapGridRef gridRef, ScaleType scaleType)
{
	_mapId = mapManager()->requestMapId(gridRef, scaleType);

	mainFrame()->fireUpdate();	
}


void MapDoc::setSelectedTowerIndex(int selectedTowerIndex)
{
	if (_selectedTowerIndex	!= selectedTowerIndex)
	{
		_selectedTowerIndex	= selectedTowerIndex;
						   
		RecordTower* tower = recordManager()->getTower(_selectedTowerIndex);

		if (tower)
		{
			SetTitle("Map of " + tower->getDisplayName());
		}

		mainFrame()->fireUpdate();
	}
}


void MapDoc::mapManager_notifyMapLoaded(int mapId)
{
	if (_mapId == mapId)
	{
		mainFrame()->fireUpdate();
	}
}

MapCacheItem* MapDoc::getMapCacheItem()
{
	return mapManager()->getMapCacheItem(_mapId);
}

BOOL MapDoc::OnPreparePrinting(CPrintInfo* pInfo, CView* view)
{
	TRACE("OnPreparePrinting\r\n");
	
	
	//a bit of jiggery to force the standard print dialog not to show.
	// see DoPreparePrinting for information
	BOOL holdDirectState = pInfo->m_bDirect;
	if (!_showPrintSetupDlg)
	{
		pInfo->m_bDirect = TRUE;
	}	

	BOOL ret = view->DoPreparePrinting(pInfo); //this is where we get the print dialog

	//go back st the previous state
	pInfo->m_bDirect = holdDirectState ;

	return ret;
}

void MapDoc::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnBeginPrinting\r\n");

	PrintData* printData = new PrintData;
	pInfo->m_lpUserData = printData;
}

void MapDoc::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	//only do stuff if we are printing / preview
	if ((pInfo)&&(pDC->IsPrinting()))
	{
		PrintData* printData = (PrintData*)pInfo->m_lpUserData;

		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= printData->_numPages );
		TRACE("MapDoc::OnPrepareDC  curPage = %d  numPages = %d pInfo->m_bContinuePrinting = %s\r\n",pInfo->m_nCurPage, printData->_numPages,  pInfo->m_bContinuePrinting?"TRUE":"FALSE");
	}
}	 

void MapDoc::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnEndPrinting\r\n");

	PrintData* printData = (PrintData*)pInfo->m_lpUserData;
	delete printData;
	
	pInfo->m_lpUserData = NULL;
}

void MapDoc::OnFilePrintPreview(CView* view) 
{
	BCGPPrintPreview (view);
}

//this is the standard print. - standard print setup
// will be used by the print button on teh 
//  print preview screen, and also external printing
//this should go direct to the print dialog, otherwise the Setup Dlg will be shown twice
void MapDoc::OnFilePrint() 
{
	_showPrintSetupDlg = TRUE;
	//DO NOT SET _printAnalysis here it is used for both analysis and main print!!!
}

//print from toolbar -  no setup at all
void MapDoc::OnFilePrintToolbar() 
{
	_showPrintSetupDlg = FALSE;
}

//print frm menu - using setup dialogs
void MapDoc::OnFilePrintMenu() 
{
	_showPrintSetupDlg = TRUE;
}

void MapDoc::mainFrame_notifyUpdate(ViewType /*ViewType*/)
{
	//update the pin list
	_towersOnMap.RemoveAll();

	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	//pass through every tower, and see if it is on this map.
	if (mapCacheItem)
	{
		for (int i=0;i<recordManager()->getAllTowersCount();i++)
		{
			RecordTower* recordTower = recordManager()->getAllTower(i);
			if (recordTower)
			{
				MapGridRef mapGridRef = recordTower->getMapRef();

				if (mapGridRef.isValidOSGridRef())
				{
					if (mapCacheItem->isGridRefOnMap(mapGridRef))
					{
						_towersOnMap.Add(recordTower);
					}
				}
			}
		}
	}

	UpdateAllViews(NULL);
}

RecordTowers& MapDoc::getTowersOnMap()
{
	return _towersOnMap;
}


void MapDoc::setCurrentGridRef(MapGridRef mapGridRef)
{
	_currentMapGridRef = mapGridRef;
}

MapGridRef MapDoc::getCurrentGridRef()
{
	return _currentMapGridRef;
}

void MapDoc::getStatusBarText(CString &pane1)
{
	if (_currentMapGridRef.isValidOSGridRef())
	{
		pane1 = _currentMapGridRef.getAlphaNumeric();
	}
	else pane1 = "";
}

RecordTower* MapDoc::recordManager_notifyGetSelectedTower(ViewType viewType)
{
	if (viewType != vt_map)
		return NULL;

	if (mainFrame()->getActiveMapDoc() == this)
		return recordManager()->getTower(_selectedTowerIndex);

	return NULL;
}



void MapDoc::setContextTower(RecordTower *tower)
{
	_contextTower = tower;
}

RecordTower* MapDoc::getContextTower()
{
	return _contextTower;
}

int MapDoc::getMapRefId()
{
	return _mapId;
}

ScaleType MapDoc::getScale()
{  
	MapCacheItem* item = getMapCacheItem();
	if (item)
		return item->getScale();
	
	return s_none;
}

void MapDoc::OnFileSaveAs() 
{
#ifdef _DEBUG

	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT(pTemplate != NULL);

#endif //_DEBUG

	CString title;
	VERIFY(title.LoadString(AFX_IDS_SAVEFILE));

	CString strFilter;
	CString strDefault;

	//FILES
	strFilter += RingingMasterApp::GetImageFileTypes();			

	//all files
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += "|*.*||";

	
	CString fileName;

	CFileDialog dlg(FALSE, 
				"",NULL,
				OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
				strFilter,NULL);


	dlg.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);;
	dlg.m_ofn.lpstrTitle = title;

	if (dlg.DoModal() == IDOK)
	{
		fileName.ReleaseBuffer();

			   
		//extract the path
		char szDrive[200];
		char szDir[200];
		char szFileName[200];
		char szExt[200];

		_splitpath(fileName, szDrive, szDir, szFileName, szExt);

		CString extention = szExt;


		ENUM_CXIMAGE_FORMATS imageFormat = RingingMasterApp::GetImageType(extention);

		if (imageFormat != CXIMAGE_FORMAT_UNKNOWN)
		{
			saveAsImage(fileName, imageFormat);	
		}
		else 
		{
			if (!DoSave(fileName))
			{
				TRACE("Warning: File save-as failed.\n");	
			}
		}

	}
	else
	{
		fileName.ReleaseBuffer();
	}
}

bool MapDoc::saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format)
{
	//file name will be a full path.
	TRACE(fileName + "\r\n");

	ASSERT(!fileName.IsEmpty());
	
	if (fileName.IsEmpty())
		return false;
	 
	CxImage* image = getMapView()->getImage();
	
	if (image)
	{
		if (format == CXIMAGE_FORMAT_GIF)
			image->DecreaseBpp(8, true);

		image->Save(fileName, format);
	}

	delete image;

	return true;
}


MapView* MapDoc::getMapView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	MapView* pMapView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pMapView = dynamic_cast<MapView*>(pView);
		if (pMapView)
			return pMapView; 
	}   
	return NULL;
}


MapDocumentInfo* MapDoc::getMapDocumentInfo()
{
	MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(_mapId);

	if (mapCacheItem)
		return new MapDocumentInfo(GetTitle(), mapCacheItem->getCenterOfMap(), _selectedTowerIndex);

	return NULL;
}