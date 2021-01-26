#pragma once

#include "resource.h"

class ButtonProp : public CBCGPProp
{
public:
	ButtonProp(
		const CString& strName, const _variant_t& varValue, 
		LPCTSTR lpszDescr = NULL, DWORD dwData = 0, UINT imageID = IDB_DOT_DOT_DOT, BOOL hasValueField = TRUE
		);

	void setHasButton(BOOL hadButton);

protected:
	virtual BOOL HasButton () const; 
	virtual void OnClickButton (CPoint point);
	void OnClickName (CPoint point);

	void OnDrawButton (CDC* pDC, CRect rect);
	virtual BOOL HasValueField () const
	{
		return _hasValueField;
	}

	BOOL _hasButton;
	BOOL _hasValueField;

	CImageList _imageList;

};

