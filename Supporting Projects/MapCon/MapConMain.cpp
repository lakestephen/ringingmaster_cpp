// MapConMain.cpp: implementation of the MapConMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapCon.h"
#include "MapConMain.h"
#include "../../RingingMaster/scale.h"
#include "../../RingingMaster/ImageFormats.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum hosts
{
/****************************************************************\
  Make sure these are in alphabetic order
\****************************************************************/

	h_getamap,
	h_multimap,
//	h_streetmap,
};


/****************************************************************\
  Helper functions
\****************************************************************/
bool copyString(char* dest, int destSize, CString src)
{
	if (src.GetLength() >= destSize)
		return false;

	strcpy_s(dest, src.GetLength(), src.GetBuffer(src.GetLength()));
	src.ReleaseBuffer();

	return true;
}

bool copyString(CString& dest, char* src, int srcSize)
{
	strcpy_s(dest.GetBufferSetLength(srcSize+2),srcSize, src);
	dest.ReleaseBuffer();

	return true;
}

bool isEven(int num)
{
	int test = num/2;
	test = test*2;
	if (test - num == 0)
		return TRUE;
	else
		return FALSE;
}



/*			
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
*/



bool getScaleString(int hostId, ScaleType scale, CString& strScale)
{
	//be carefull when changing this, as we can get problems with the MapCon.dll 
	// being out of step with the main program.

	//only list the ones that are supported.

	switch (hostId)
	{
	case h_multimap:
		{
			switch (scale)
			{
			case s_25_000:		strScale = "25000";    break;
			case s_50_000:		strScale = "50000";    break;
			case s_none:
			default:			strScale = "";    break;
			}		  
		}
		break;
	case h_getamap:
		{
			switch (scale)
			{
			case s_25_000:		strScale = "7";    break;
			case s_50_000:		strScale = "6";    break;
			case s_none:
			default:			strScale = "";    break;
			}		  
		}
		break;
	default:
		strScale = "";    
		break;
	}

	return true;
}


/****************************************************************\
  Ringing Master should pass in the largest interface version 
  that it supports, and this return weather it can use it. 
  Ringing Master may call this function several times if it 
  still supports previous versions.
\****************************************************************/


bool WINAPI isInterfaceCompatable(int RMInterfaceVersion) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (RMInterfaceVersion == 2); //at the moment, only a single version
}




/****************************************************************\
  Returns the simple file version. use this to check if the 
  users version is the latest. This will enable the client to do 
  a check of the web site for the latest version for the 
  supported interface version.
\****************************************************************/

int WINAPI getDLLVersion() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 3;
}


/****************************************************************\
  Get the number of supported hosts
\****************************************************************/
int WINAPI getHostCount() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 2;
}		 


/****************************************************************\
  Get the text name of the host
\****************************************************************/
bool WINAPI getHostName(int hostId, CString& retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  
	switch (hostId)
	{
	case h_multimap:
		retVal = "Multimap";
		return true;
	case h_getamap:
		retVal = "Get a Map (OS)";
		return true;
	default:
		return false;
	}
}		 

/****************************************************************\
  Get the url of the chosen host
\****************************************************************/

CString WINAPI getHostWebAddress(int hostId, CString& )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
		return "http://www.multimap.co.uk";
	case h_getamap:
		return "http://www.getamap.co.uk";
	default:
		return  "";
	}
}

/****************************************************************\
  Get the center of the image
\****************************************************************/

bool WINAPI getHostGridRefImageCenter(int hostId, ScaleType /*scale*/, int& reqEastings, int& reqNorthings)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
	case h_getamap:
		{
			// Monks Kirby 0446302832
			//  X = 446000, Y = 283000

			int eastOffset = reqEastings%10; 
			if (eastOffset >= 5)
				reqEastings += (10-eastOffset);
			else
				reqEastings -= eastOffset;

			int northOffset = reqNorthings%10; 
			if (northOffset >= 5)
				reqNorthings += (10-northOffset);
			else
				reqNorthings -= northOffset;

			// no action in y
			return true;
		}
		break;
	default:
		return false;
	}
} 



/****************************************************************\
  Get the url of the main page. Pass in the 10 digit grid ref
\****************************************************************/
bool WINAPI getHostInitialPageURL(int hostId, 
									ScaleType iScale, 
									int reqEastings, 
									int reqNorthings, 
									CString& url)
{				
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
		{
			CString strScale;

			if ((getHostGridRefImageCenter(hostId, iScale, reqEastings, reqNorthings))&&
				(getScaleString(hostId, iScale, strScale)))
			{		
				reqEastings  *= 100;
				reqNorthings *= 100;
				url.Format("http://www.multimap.com/map/browse.cgi?X=%d&Y=%d&scale=%s&coordsys=gb&mapsize=big",
							reqEastings,reqNorthings,strScale);    
				return true;
			}
		}
	case h_getamap:
		{	
			CString strScale;

			if ((getHostGridRefImageCenter(hostId, iScale, reqEastings, reqNorthings))&&
				(getScaleString(hostId, iScale, strScale)))
			{		
				reqEastings  *= 100;
				reqNorthings *= 100;

				int envXMax = reqEastings + 1000;
				int envXMin = reqEastings - 1000;
				
				int envYMax = reqNorthings + 1000;
				int envYMin = reqNorthings - 1000;

				url.Format("http://getamap.ordnancesurvey.co.uk/jsp/form.jsp?mapAction=zoomabs&mapRevert=zoomin&mapX=200&mapY=200&envXMax=%d&envYMax=%d&envXMin=%d&envYMin=%d&isGeo=&isNI=&zoomLevel=%s&gazName=g&gazString=&hasResult=y&resName=TEST&resX=%d&resY=%d",
						   envXMax, envYMax, envXMin, envYMin, strScale, reqEastings, reqNorthings);    
				return true;
			}
		}
	default:
		return false;
	}				
}

/****************************************************************\
  Get the number of times we have to cycle through getHostImageURL
\****************************************************************/
int WINAPI getHostIterationCount(int hostId, ScaleType /*iScale*/) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
		return 1;
	case h_getamap:
		return 1;
	default:
		return 0;
	}
}

/****************************************************************\
  Get the url of the image. Pass in the main page code
\****************************************************************/

bool getHostImageURLMultiMap(						 ScaleType /*iScale*/,
													 CString& dest, 
													 const CString& page, 
													 int iteration) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//multi map only has a single iteration.

	switch (iteration)
	{
	case 0:
		{
			CString strPage = page;

			int start = strPage.Find("name=\"multimap\"");
			if (start != -1)
			{
				strPage = strPage.Mid(start, 250);
				//we are left with a small snippet
				start = strPage.Find("src=\"");
				int end = strPage.Find(".gif");
				if ((start != -1)&&(end != -1))
				{
					strPage = strPage.Mid(start+5, end - (start + 1)); //subtract the length of src=", and add length of .gif

					if (!strPage.IsEmpty())
					{
						dest = strPage;
						return true;
					}
				}
			}
		}
	default:
		return false;
	}
}	

bool getHostImageURLGetAMap(						 ScaleType /*iScale*/,
													 CString& dest, 
													 const CString& page, 
													 int iteration) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//multi map only has a single iteration.

	switch (iteration)
	{
	case 0:
		{	
			CString strPage = page;

			int start = strPage.Find("http://getamap.ordnancesurvey.co.uk/");
			if (start != -1)
			{
				strPage = strPage.Mid(start, 250);
				//we are left with a small snippet
				start = 0;
				int end = strPage.Find(".png");
				if ((start != -1)&&(end != -1))
				{
					strPage = strPage.Mid(start, end + 4); //add length of .png

					if (!strPage.IsEmpty())
					{
						dest = strPage;
						return true;
					}
				}
			}


		}
	default:
		return false;
	}
}	



bool WINAPI getHostImageURL(int hostId, 
							ScaleType iScale,
							CString& dest, 
							const CString& page,  
							int iteration) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (hostId)
	{
	case h_multimap:
		return getHostImageURLMultiMap(iScale, dest, page, iteration);
	case h_getamap:
		return getHostImageURLGetAMap(iScale, dest, page, iteration);
	default:
		return false;
	}
}

/****************************************************************\
  get the image type
\****************************************************************/
int WINAPI getHostImageType(int hostId, ScaleType /*iScale*/) 
{			
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
		return IT_GIF;
	case h_getamap:
		return IT_PNG;
	default:
		return IT_NONE;
	}
}

/****************************************************************\
  check to see if scale is supported. RingingMaster will call 
  repeatedly with different scales to enumerate what is useable.
\****************************************************************/
bool WINAPI getHostScaleSupport(int hostId, ScaleType iScale) 
{			
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (hostId)
	{
	case h_multimap:
		return ((iScale == s_25_000)||
			    (iScale == s_50_000));
	case h_getamap:
		return ((iScale == s_25_000)||
			    (iScale == s_50_000));
	default:
		return false;
	}
}


/****************************************************************\
get the cropping sizes
\****************************************************************/
bool WINAPI getHostCropSize(  int hostId,
							  ScaleType iScale, 
							  int& top, 
							  int& left, 
							  int& bottom, 
							  int& right) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (getHostScaleSupport(hostId, iScale))
	{
		switch (hostId)
		{
		case h_multimap:
			switch (iScale)
			{
			case s_25_000:
				top= 13;
				left = 3;
				bottom = 16;  
				right = 1;  
				return true;
			case s_50_000:
				top= 13;
				left = 0;
				bottom = 16;  
				right = 0;  
				return true;
			default:
				break;
			}
		case h_getamap:
			switch (iScale)
			{
			case s_25_000:
				top= 0;
				left = 0;
				bottom = 13;  
				right = 0;  
				return true;
			case s_50_000:
				top= 0;
				left = 0;
				bottom = 13;  
				right = 0;  
				return true;
			default:
				break;
			}
		default:
			break;
		}
	}

	top = 0;
	left = 0;
	bottom = 0;
	right = 0;

	return false;
}

/****************************************************************\
get the post cropping sizes of image in pixels
\****************************************************************/
bool WINAPI getHostPostCropSize(  int hostId,
								  ScaleType iScale, 
								  int& width, 
								  int& height) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int top;
	int left;
	int bottom;
	int right;

	if ((getHostScaleSupport(hostId, iScale))&&
		(getHostCropSize(hostId, iScale, top, left, bottom, right)))
	{

		switch (hostId)
		{
		case h_multimap:
			switch (iScale)
			{
			case s_25_000:
				width = 700 - (left + right);
				height = 400 - (top + bottom);
				return true;
			case s_50_000:
				width = 700 - (left + right);
				height = 400 - (top + bottom);
				return true;
			default:
				break;
			}
		case h_getamap:
			switch (iScale)
			{
			case s_25_000:
				width = 400 - (left + right);
				height = 400 - (top + bottom);
				return true;
			case s_50_000:
				width = 400 - (left + right);
				height = 400 - (top + bottom);
				return true;
			default:
				break;
			}
		default:
			break;
		}
	}

	width = 0;
	height = 0;

	return false;
}

/****************************************************************\
get the corners of the image
\****************************************************************/
bool WINAPI getHostGridRefCorners(int hostId,
														ScaleType iScale, 
														int reqEastings, 
														int reqNorthings, 
														int& top, 
														int& left, 
														int& bottom, 
														int& right) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (getHostScaleSupport(hostId, iScale)&&
		getHostGridRefImageCenter(hostId, iScale, reqEastings, reqNorthings))
	{
		switch (hostId)
		{
		case h_multimap:
			switch (iScale)
			{
			case s_25_000:
				top =		reqNorthings + 12; //in 100 meters
				left =		reqEastings  - 23;
				bottom =	reqNorthings - 13;
				right =		reqEastings  + 23;
				return true;
			case s_50_000:
				top =		reqNorthings + 24; //in 100 meters
				left =		reqEastings  - 46;
				bottom =	reqNorthings - 26;
				right =		reqEastings  + 46;
				return true;
			default:
				break;
			}
		case h_getamap:
			switch (iScale)
			{
			case s_25_000:
				top =		reqNorthings + 10; //in 100 meters
				left =		reqEastings  - 10;
				bottom =	reqNorthings - 10; //todo this needs to be less	as bottom cropped
				right =		reqEastings  + 10;
				return true;
			case s_50_000:
				top =		reqNorthings + 20; //in 100 meters //todo make this (and all others) be in 1 meter increments
				left =		reqEastings  - 20;
				bottom =	reqNorthings - 20; //todo this needs to be less	as bottom cropped
				right =		reqEastings  + 20;
				return true;
			default:
				break;
			}
		default:
			break;
		}
	}

	top = 0;
	left = 0;
	bottom = 0;
	right = 0;

	return false;
}

/****************************************************************\
Get Next Map Center
\****************************************************************/
bool WINAPI getHostNextMapCenter(int hostId,
								ScaleType iScale, 
								int& eastings, 
								int& northings) 
{		

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//NOTE: These values must relate to the map providers increments
	
	if (getHostScaleSupport(hostId, iScale))
	{
		switch (hostId)
		{
		case h_multimap:
			switch (iScale)
			{
			case s_25_000:
				eastings =		20; //in 100 meters
				northings =		10;
				return true;
			case s_50_000:
				eastings =		40; //in 100 meters
				northings =		20;
				return true;
			default:
				break;
			}
		case h_getamap:
			switch (iScale)
			{
			case s_25_000:
				eastings =		10; //in 100 meters
				northings =		10;
				return true;
			case s_50_000:
				eastings =		20; //in 100 meters
				northings =		20;
				return true;
			default:
				break;
			}
		default:
			break;
		}
	}

	eastings = 0;
	northings = 0;

	return false;
}






