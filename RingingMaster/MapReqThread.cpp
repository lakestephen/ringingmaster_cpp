// MapReqThread.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapReqThread.h"
#include "UserMessages.h"
#include "MapManager.h"
#include "MapHostWrapper.h"
#include "InternetManager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MapReqThread

IMPLEMENT_DYNCREATE(MapReqThread, CWinThread)

MapReqThread::MapReqThread() :
_terminated(FALSE)
{
}

MapReqThread::~MapReqThread()
{
}

BOOL MapReqThread::InitInstance()
{
	return TRUE;
}

int MapReqThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(MapReqThread, CWinThread)
	//{{AFX_MSG_MAP(MapReqThread)
		ON_THREAD_MESSAGE(MAP_REQ_TERMINATE, OnTerminate)
		ON_THREAD_MESSAGE(MAP_REQ_GET_MAP, OnGetMap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapReqThread message handlers



void MapReqThread::terminate()
{
	_terminated = TRUE;
	PostThreadMessage(MAP_REQ_TERMINATE,0,0);
}

void MapReqThread::OnTerminate(UINT /*wParam*/, LONG /*lParam*/)	
{
	::PostQuitMessage(0);	  

}

void MapReqThread::OnGetMap(UINT /*wParam*/, LONG lParam)	
{
	int mapId = lParam;

	if ((!_terminated)&&
		(mapManager()->isValid())&&
		(mapId != -1))
	{
		MapCacheItem* mapCacheItem = mapManager()->getMapCacheItem(mapId);
		if ((mapCacheItem)&&
			(!mapCacheItem->isImageAvailable()))
		{
			CxImage* image = getMapImageExternal(*mapCacheItem);

			if ((app())&&(image))
			{
				cropImage(*mapCacheItem, image);

				if ((!_terminated)&&(app()))
				{
					mapCacheItem->setImage(image);

					app()->PostThreadMessage(MAP_REQ_APPLY_MAP, (WPARAM)-1, (LONG)mapId);
				}
				else 
				{
					delete image;
				}
			}
		}
	}
}


void MapReqThread::cropImage(const MapCacheItem& mapCacheItem, CxImage* image)
{
	if (image)
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();
		int left,top,right,bottom;

		wrapper->getHostCropSize(mapCacheItem.getHostIndex(), mapCacheItem.getScale(), top, left,bottom, right);

		image->Crop(left,top,image->GetWidth()-right,image->GetHeight() - bottom);
	}
}

//called from seperate thread.
CxImage* MapReqThread::getMapImageExternal(const MapCacheItem& mapCacheItem)
{
	if (mapManager()->isValid())
	{
		MapHostWrapper* wrapper = mapManager()->getMapHostWrapper();

		//get the initial url.
		CString url = wrapper->getHostInitialPageURL(mapCacheItem.getHostIndex(), 
													 mapCacheItem.getScale(),
													 mapCacheItem.getCenterOfMap());

		TRACE(url + "\r\n");
			
		//work through the iterations to getto the url of the image
		for (int i=0;i<wrapper->getHostIterationCount(mapCacheItem.getHostIndex(), mapCacheItem.getScale());i++)
		{
			CString page = internetManager()->getPage(url);
			url = wrapper->getHostImageURL(mapCacheItem.getHostIndex() , mapCacheItem.getScale(), i, page);
		}		

		//check that the file version matches
		ENUM_CXIMAGE_FORMATS xFormat = CXIMAGE_FORMAT_BMP; //holding
		bool ok = false;

		switch (wrapper->getHostImageType(mapCacheItem.getHostIndex(), mapCacheItem.getScale() ))
		{
		case IT_BMP:
			xFormat = CXIMAGE_FORMAT_BMP;
			ok = (url.Find(".bmp") != -1);
			break;
		case IT_GIF:
			xFormat = CXIMAGE_FORMAT_GIF;
			ok = (url.Find(".gif") != -1);
			break;
		case IT_JPG:
			xFormat = CXIMAGE_FORMAT_JPG;
			ok = (url.Find(".jpg") != -1);
			break;
		case IT_PNG:
			xFormat = CXIMAGE_FORMAT_PNG;
			ok = (url.Find(".png") != -1);
			break;
		case IT_ICO:
			xFormat = CXIMAGE_FORMAT_ICO;
			ok = (url.Find(".ico") != -1);
			break;
		case IT_TIF:
			xFormat = CXIMAGE_FORMAT_TIF;
			ok = (url.Find(".tif") != -1);
			break;
		case IT_TGA:
			xFormat = CXIMAGE_FORMAT_TGA;
			ok = (url.Find(".tga") != -1);
			break;				
		case IT_PCX:
			xFormat = CXIMAGE_FORMAT_PCX;
			ok = (url.Find(".pcx") != -1);
			break;
		case IT_WBMP:
			xFormat = CXIMAGE_FORMAT_WBMP;
			ok = (url.Find(".bmp") != -1);
			break;
		case IT_WMF:
			xFormat = CXIMAGE_FORMAT_WMF;
			ok = (url.Find(".wmf") != -1);
			break;
		}

		if (ok)
		{					
			//get the image
			DWORD length;

			BYTE* buffer = internetManager()->getHttpFIle(url, length);
			CxImage* image = new CxImage((BYTE*)buffer,length,xFormat);
			
			delete[] buffer;
							
			return image;
		}
	}
	
	return NULL;
}

