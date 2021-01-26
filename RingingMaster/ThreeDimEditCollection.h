// ThreeDimEditCollection.h: interface for the ThreeDimEditCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEDIMEDITCOLLECTION_H__B0EBE1CD_2D4C_4609_A0FE_D0419576FE29__INCLUDED_)
#define AFX_THREEDIMEDITCOLLECTION_H__B0EBE1CD_2D4C_4609_A0FE_D0419576FE29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	 

#include "ThreeDimEditItem.h"

class ThreeDimEditCollection  
{
public:
	void setZoom(int pixelsPerMeter, double linePitchMeters);
	void removeAllItems();
	void moveOrigPixels(CPoint point);
	void setupPosition(CPoint windowWidth, double xMeters, double yMeters);
	bool setupPositionWouldMove(CPoint windowWidth, double xMeters, double yMeters);
	int getIconSize();
	double convertPixelsToMeters(int pixels);
	ThreeDimEditItem* getItem(int index);
	ThreeDimEditItem* getItemUsingID(int id);
	int getItemCount();
	void addItem(ThreeDimEditItem* item);
	double getLinePitchMeters();				 
	COLORREF getLineColor();
	ThreeDimEditCollection(UINT imagesID, int iconSize);
	virtual ~ThreeDimEditCollection();
	
	int convertMetersToPixels(double meters);

	CPoint getOriginPixels();		   

protected:
	int _pixelsPerMeter;	 
	double _linePitchMeters; //distance between lines.
	COLORREF _lineColor;

	double _originXMeters;
	double _originYMeters;

	ThreeDimEditItems _items;
	int _iconSize;

	CImageList _images;

	friend class ThreeDimEditItem;

};

#endif // !defined(AFX_THREEDIMEDITCOLLECTION_H__B0EBE1CD_2D4C_4609_A0FE_D0419576FE29__INCLUDED_)
