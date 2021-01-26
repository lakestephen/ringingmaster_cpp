// MapHostWrapper.h: interface for the MapHostWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPHOSTWRAPPER_H__96354964_90FE_11D7_B6C4_000255162CF0__INCLUDED_)
#define AFX_MAPHOSTWRAPPER_H__96354964_90FE_11D7_B6C4_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapGridRef.h"
#include "Scale.h"
#include "ImageFormats.h"

class MapHostWrapper  
{
public:
	BOOL isInterfaceCompatable();
	int getDLLVersion();

	int getHostCount();
	CString getHostName(int hostId);
	CString getHostWebAddress(int hostId);

	CString getHostInitialPageURL(int hostId, ScaleType scale, MapGridRef mapGridRef);
	int getHostIterationCount(int hostId, ScaleType scale);
	CString getHostImageURL(int hostId, ScaleType scale, int iteration, CString page);

	ImageType getHostImageType(int hostId, ScaleType scale);
	void getHostCropSize(int hostId, ScaleType scale, int& top, int& left, int& bottom, int& right);
	void getHostPostCropSize(int hostId, ScaleType scale, int& width, int& height);
	void getHostGridRefCorners(int hostId, ScaleType scale, MapGridRef gridRefReq, MapGridRef &bottomLeft, MapGridRef& topRight);
	void getHostNextMapCenter(int hostId, ScaleType scale, int& eastings, int& northings);
	void getHostGridRefImageCenter(int hostId, ScaleType scale, MapGridRef& gridRefReq);
	
	
	bool isScaleSupported(int hostId, ScaleType scale);

	BOOL isValid();
	
	MapHostWrapper();
	virtual ~MapHostWrapper();

protected:
	HINSTANCE _hMapCon;
	BOOL _interfaceCompatable;

};

#endif // !defined(AFX_MAPHOSTWRAPPER_H__96354964_90FE_11D7_B6C4_000255162CF0__INCLUDED_)
