// ImageFormats.h: interface for the ImageFormats class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEFORMATS_H__80884510_91C8_11D7_B6C8_000255162CF0__INCLUDED_)
#define AFX_IMAGEFORMATS_H__80884510_91C8_11D7_B6C8_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//this is used for the DLL comms. It ensures that the enumeration always stays consistant
enum ImageType
{
	IT_NONE, 
	IT_BMP,
	IT_GIF,
	IT_JPG,
	IT_PNG,
	IT_MNG,
	IT_ICO,
	IT_TIF,
	IT_TGA,
	IT_PCX,
	IT_WBMP,
	IT_WMF,
	IT_J2K,			// Beta
	IT_JBG,

	//NOTE for future releases, always add to the end.
};

struct ImageFormats  
{
public:
	int nID;
	const char* description;
	const char* ext;
};		   
					 

#endif // !defined(AFX_IMAGEFORMATS_H__80884510_91C8_11D7_B6C8_000255162CF0__INCLUDED_)
