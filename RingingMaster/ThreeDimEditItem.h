// ThreeDimEditItem.h: interface for the ThreeDimEditItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEDIMEDITITEM_H__762CB975_22CF_479A_8509_51CE3483273E__INCLUDED_)
#define AFX_THREEDIMEDITITEM_H__762CB975_22CF_479A_8509_51CE3483273E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ThreeDimEditCollection;

class ThreeDimEditItem  
{
public:
	int getID();
	CString& getTooltip();
	double getYMeters();
	double getXMeters();
	bool isSelectable();
	void setItemPosition(CPoint point);
	CPoint getItemPosition();
	void draw(CDC *pDC,  bool selected);
	ThreeDimEditItem(ThreeDimEditCollection& threeDimEditCollection, int id, double x, double y, int imageIndex, 
						bool selectable, CString tooltip, CString text);
	virtual ~ThreeDimEditItem();

protected:
					    
	ThreeDimEditCollection& _threeDimEditCollection;

	double _x; //meters
	double _y; //meters

	bool _selectable;
	int _id;
	
	CString _tooltip;
	int _imageIndex;
	CString _text;

}; 

typedef CArray<ThreeDimEditItem*, ThreeDimEditItem*> ThreeDimEditItems;

#endif // !defined(AFX_THREEDIMEDITITEM_H__762CB975_22CF_479A_8509_51CE3483273E__INCLUDED_)
