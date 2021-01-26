// MapHostWrapper.cpp: implementation of the MapHostWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "MapHostWrapper.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



MapHostWrapper::MapHostWrapper() :
_hMapCon(NULL),
_interfaceCompatable(TRUE)
{
	_hMapCon = AfxLoadLibrary(TEXT("MapCon.dll"));

	if (!_hMapCon)
	{
		AfxMessageBox("The map service provider file (MapCon.dll) failed to load.\nMaps will be unavailable. \nSee Tools > Options > Maps to repair.", MB_ICONWARNING);
		_interfaceCompatable = FALSE;
	}
	else
	{
		_interfaceCompatable = isInterfaceCompatable();
		if (!_interfaceCompatable)
		{
			AfxMessageBox("The map service provider file (MapCon.dll) has an incompatible interface.\nMaps will be unavailable. \nSee Tools > Options > Maps to repair.", MB_ICONWARNING);
		}
	}	

}

MapHostWrapper::~MapHostWrapper()
{
	if (_hMapCon != NULL)
		AfxFreeLibrary(_hMapCon);
}


BOOL MapHostWrapper::isValid()
{
	return ((_hMapCon != NULL)&&(_interfaceCompatable));
}


typedef bool (WINAPI *PisInterfaceCompatable)(int);

BOOL MapHostWrapper::isInterfaceCompatable()
{
	if (isValid())
	{
		PisInterfaceCompatable isInterfaceCompatable = 
			(PisInterfaceCompatable)GetProcAddress( _hMapCon, "isInterfaceCompatable");

		ASSERT(isInterfaceCompatable != NULL);

		if (isInterfaceCompatable != NULL)
		{
			return isInterfaceCompatable(2);
		}
	}	

	return FALSE;
}


typedef int (WINAPI *PgetDLLVersion)() ;

int MapHostWrapper::getDLLVersion()
{
	if (isValid())
	{
		PgetDLLVersion getDllVersion = 
			(PgetDLLVersion)GetProcAddress( _hMapCon, "getDLLVersion");

		ASSERT(getDllVersion != NULL);

		if (getDllVersion != NULL)
		{
			return getDllVersion();
		}
	}	

	return FALSE;
}

typedef int (WINAPI *PgetHostCount)() ;

int MapHostWrapper::getHostCount()
{
	if (isValid())
	{
		PgetHostCount getHostCount= 
			(PgetHostCount)GetProcAddress( _hMapCon, "getHostCount");

		ASSERT(getHostCount != NULL);

		if (getHostCount != NULL)
		{
			return getHostCount();
		}
	}	

	return FALSE;
}


typedef bool (WINAPI *PgetHostName)(int, CString&);

CString MapHostWrapper::getHostName(int hostId)
{
	CString name;

	if (isValid())
	{
		ASSERT((hostId >=0 )&&( hostId < getHostCount()));

		PgetHostName getHostName= 
			(PgetHostName)GetProcAddress( _hMapCon, "getHostName");
													 

		ASSERT(getHostName != NULL);

		if (getHostName != NULL)
		{							 
			getHostName(hostId, name);
		}		  
	}	

	return name;
}	

typedef bool (WINAPI *PgetHostWebAddress)(int, CString&);

CString MapHostWrapper::getHostWebAddress(int hostId)
{
	CString address;

	if (isValid())
	{
		ASSERT((hostId >=0 )&&( hostId < getHostCount()));

		PgetHostWebAddress getHostWebAddress= 
			(PgetHostWebAddress)GetProcAddress( _hMapCon, "getHostWebAddress");

		ASSERT(getHostWebAddress != NULL);

		if (getHostWebAddress != NULL)
		{							 
			getHostWebAddress(hostId, address);
		}		  
	}	

	return address;
}	



typedef int(WINAPI *PgetHostIterationCount)(int, ScaleType);

int MapHostWrapper::getHostIterationCount(int hostId, ScaleType scale)
{
	if (isValid())
	{
		PgetHostIterationCount getHostIterationCount= 
			(PgetHostIterationCount)GetProcAddress( _hMapCon, "getHostIterationCount");

		ASSERT(getHostIterationCount != NULL);

		if (getHostIterationCount != NULL)
		{
			return getHostIterationCount(hostId, scale);
		}
	}	

	return FALSE;
}


typedef bool (WINAPI *PgetHostInitialPageURL)(int, ScaleType, int, int , CString&) ;

CString MapHostWrapper::getHostInitialPageURL(int hostId, ScaleType scale, MapGridRef mapGridRef)
{
	CString url;	 

	if (isValid())
	{
		PgetHostInitialPageURL getHostInitialPageURL = 
			(PgetHostInitialPageURL)GetProcAddress( _hMapCon, "getHostInitialPageURL" );

		ASSERT(getHostInitialPageURL != NULL);

		if (getHostInitialPageURL != NULL)
		{
			getHostInitialPageURL(hostId, 
								scale,
								mapGridRef.getEastings(),
								mapGridRef.getNorthings(),
								url);
		}
	}

	TRACE(url + "\r\n"); 	

	return url;
}


typedef bool (WINAPI *PgetHostImageUrl)(int, ScaleType, CString&, const CString&, int) ;

CString MapHostWrapper::getHostImageURL(int hostId, ScaleType scale, int iteration, CString page)
{
	CString url;

	if (isValid())
	{
		PgetHostImageUrl getHostImageUrl = 
			(PgetHostImageUrl)GetProcAddress( _hMapCon, "getHostImageURL");

		ASSERT(getHostImageUrl != NULL);

		if (getHostImageUrl != NULL)
		{
			getHostImageUrl(hostId,
								scale,
								url, 
								page,
								iteration);

		}
	}

	TRACE(url + "\r\n"); 	

	return url;
}
					  

typedef int (WINAPI *PgetHostImageType)(int, ScaleType) ;

ImageType MapHostWrapper::getHostImageType(int hostId, ScaleType scale)
{
	if (isValid())
	{
		PgetHostImageType getHostImageType = 
			(PgetHostImageType)GetProcAddress( _hMapCon, "getHostImageType");

		ASSERT(getHostImageType != NULL);

		if (getHostImageType != NULL)
		{
			return (ImageType)getHostImageType(hostId, scale);
		}
	}	

	return IT_NONE;
}


typedef int (WINAPI *PgetHostCropSize)(int, int, int&, int&, int&, int&) ;

void MapHostWrapper::getHostCropSize(int hostId, ScaleType scale, int& top, int& left, int& bottom, int& right)
{
	if (isValid())
	{
		PgetHostCropSize getHostCropSize = 
			(PgetHostCropSize)GetProcAddress( _hMapCon, "getHostCropSize");

		ASSERT(getHostCropSize != NULL);

		if (getHostCropSize != NULL)
		{
			getHostCropSize(hostId, scale, top, left, bottom, right);
		}
	}											 	
}

typedef bool (WINAPI *PgetHostScaleSupport)(int, ScaleType);

bool MapHostWrapper::isScaleSupported(int hostId, ScaleType scale)
{
	if (isValid())
	{
		PgetHostScaleSupport getHostScaleSupport = 
			(PgetHostScaleSupport)GetProcAddress( _hMapCon, "getHostScaleSupport");

		ASSERT(getHostScaleSupport != NULL);

		if (getHostScaleSupport != NULL)
		{
			return getHostScaleSupport(hostId, scale);
		}
	}			
	
	return false;
}

typedef int (WINAPI *PgetHostGridRefCorners)(int, ScaleType, int, int, int&, int&, int&, int&) ;

void MapHostWrapper::getHostGridRefCorners(int hostId, ScaleType scale, MapGridRef gridRefReq, MapGridRef& bottomLeft, MapGridRef& topRight)
{
	if (isValid())
	{
		PgetHostGridRefCorners getHostGridRefCorners = 
			(PgetHostGridRefCorners)GetProcAddress( _hMapCon, "getHostGridRefCorners");

		ASSERT(getHostGridRefCorners != NULL);

		if (getHostGridRefCorners != NULL)
		{
			int top, left, bottom, right;
			int eastings = gridRefReq.getEastings();
			int northings = gridRefReq.getNorthings();

			getHostGridRefCorners(hostId, scale, eastings, northings, top, left, bottom, right);

			bottomLeft.setGridRef(left, bottom);
			topRight.setGridRef(right, top);
		}
	}											 	
}


typedef int (WINAPI *PgetHostNextMapCenter)(int, ScaleType, int&, int&) ;

void MapHostWrapper::getHostNextMapCenter(int hostId, ScaleType scale, int& eastings, int& northings)
{
	if (isValid())
	{
		PgetHostNextMapCenter getHostNextMapCenter = 
			(PgetHostNextMapCenter)GetProcAddress( _hMapCon, "getHostNextMapCenter");

		ASSERT(getHostNextMapCenter != NULL);

		if (getHostNextMapCenter != NULL)
		{
			getHostNextMapCenter(hostId, scale, eastings, northings);
		}
	}											 	
}

typedef int (WINAPI *PgetHostPostCropSize)(int, ScaleType, int&, int&) ;

void MapHostWrapper::getHostPostCropSize(int hostId, ScaleType scale, int& width, int& height)
{
	if (isValid())
	{
		PgetHostPostCropSize getHostPostCropSize = 
			(PgetHostPostCropSize)GetProcAddress( _hMapCon, "getHostPostCropSize");

		ASSERT(getHostPostCropSize != NULL);

		if (getHostPostCropSize != NULL)
		{
			getHostPostCropSize(hostId, scale, width, height);
		}
	}											 	
}


typedef bool (WINAPI *PgetHostGridRefImageCenter)(int, ScaleType, int&, int&) ;

void MapHostWrapper::getHostGridRefImageCenter(int hostId, ScaleType scale, MapGridRef& gridRefReq)
{
	if (isValid())
	{
		PgetHostGridRefImageCenter getHostGridRefImageCenter = 
			(PgetHostGridRefImageCenter)GetProcAddress( _hMapCon, "getHostGridRefImageCenter");

		ASSERT(getHostGridRefImageCenter != NULL);

		if (getHostGridRefImageCenter != NULL)
		{
			int reqEastings = gridRefReq.getEastings();
			int reqNorthings = gridRefReq.getNorthings();

			getHostGridRefImageCenter(hostId, scale, reqEastings, reqNorthings);

			gridRefReq.setGridRef(reqEastings, reqNorthings);
		}
	}											 	

}

