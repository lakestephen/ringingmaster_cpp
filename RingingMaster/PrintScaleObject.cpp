// PrintScaleObject.cpp: implementation of the PrintScaleObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PrintScaleObject.h"
#include "PrintData.h"
#include "PrintUtils.h"
#include "CPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrintScaleObject::PrintScaleObject(CDC* scrnDC, CDC* printDC, CPage* page, CPrintInfo* pInfo, CSize objectSize) :
_objectSize(objectSize)
{
	//determine the scales for the print
	float pixPerInchXScrn = (float)scrnDC->GetDeviceCaps(LOGPIXELSX);
	float pixPerInchYScrn = (float)scrnDC->GetDeviceCaps(LOGPIXELSY);

	float pixPerInchXPrint = (float)printDC->GetDeviceCaps(LOGPIXELSX);
	float pixPerInchYPrint = (float)printDC->GetDeviceCaps(LOGPIXELSY);

	//calculate the page size
	double topGap = (double)((double)(app()->GetProfileInt("Print", "Touch Top", 1000))/2540.0);	
	double dVertPageSize = page->getLengthInches() - PrintUtils::getFootnoteHeight() - topGap;
	int iVertPageSize = page->ConvertToMappedUnits(dVertPageSize, VERTRES);

	double leftGap = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	
	double dHorzPageSize = page->getWidthinches() - leftGap;
	int iHorzPageSize = page->ConvertToMappedUnits(dHorzPageSize, HORZRES);

	_scaleX = pixPerInchXPrint/pixPerInchXScrn;
	_scaleY = pixPerInchYPrint/pixPerInchYScrn;
	_scaleXY = (float)((_scaleY+_scaleX)/2.0);
	_pageX =iHorzPageSize;
	_pageY = iVertPageSize;
	_leftGap = page->ConvertToMappedUnits(leftGap,HORZRES);
	_topGap = page->ConvertToMappedUnits(topGap,VERTRES);
	_pageNumber = pInfo->m_nCurPage;

	_offsetX = _leftGap - getHorzOffset(getHorzPageNumber());
	_offsetY = _topGap - getVertOffset(getVertPageNumber());
}


PrintScaleObject::~PrintScaleObject()
{
	  
}

int PrintScaleObject::scaleX(int x)
{
	return (int)((float)x * _scaleX);
}

int PrintScaleObject::scaleY(int y)
{
	return (int)((float)y * _scaleY);
}

int PrintScaleObject::scaleXY(int xy)
{
	return (int)((float)xy * _scaleXY);
}

int PrintScaleObject::translateX(int x)
{
	return ((int)((float)x * _scaleX)) + _offsetX;

}

int PrintScaleObject::translateY(int y)
{
	return ((int)((float)y * _scaleY)) + _offsetY;

}

int PrintScaleObject::translateXY(int xy)
{
	return (int)((float)xy * _scaleXY);
}

CRect PrintScaleObject::translateRect(CRect rect)
{
	rect.top = translateY(rect.top);
	rect.bottom = translateY(rect.bottom);
	rect.left = translateX(rect.left);
	rect.right = translateX(rect.right);

	return rect;	   
}


int PrintScaleObject::getVertOffset(int vertPageNumber)
{
	return _pageY * vertPageNumber;
}

int PrintScaleObject::getHorzOffset(int horzPageNumber)
{
/*	if (_wholeColumns) 
		return translateX(getLeadBox(_pageColumns.GetAt(horzPageNumber) * _style._leadsPerColumn).left);
	else
*/		return _pageX * horzPageNumber;
}

int PrintScaleObject::getNumberOfPages()
{
	return getNumberOfHorzPages() * getNumberOfVertPages();
}

int PrintScaleObject::getHorzPageNumber()
{
	return (int)((_pageNumber-1)/getNumberOfVertPages());

}

int PrintScaleObject::getVertPageNumber()
{
	return (_pageNumber - (getHorzPageNumber() * getNumberOfVertPages()))-1;
}

//in printer units
int PrintScaleObject::getNumberOfVertPages()
{
	return (getVertSize()/_pageY) + 1;
}

int PrintScaleObject::getNumberOfHorzPages()
{
/*	if (_wholeColumns) 
		return _pageColumns.GetSize();
	else
*/		//just blat it on the paper
		return (getHorzSize() / _pageX) + 1;
}

//returns the vertical size in printer units
int PrintScaleObject::getVertSize()
{
	return scaleY(_objectSize.cy);

}

//returns the Horizontal size in printer units
int PrintScaleObject::getHorzSize() 
{
	return scaleX(_objectSize.cx);

}

