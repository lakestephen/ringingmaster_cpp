// CellElement.h: interface for the CellElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELLELEMENT_H__B4C1DEE7_437E_11D5_B5CC_009027BB3286__INCLUDED_)
#define AFX_CELLELEMENT_H__B4C1DEE7_437E_11D5_B5CC_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParseData.h"
				   
class CellData;
class FontInfo;

class CellElement  
{
public:
	void initElement();
	void draw(int x, int y, CDC* pDC, COLORREF color, const FontInfo& font);
	CSize getExtent(CDC* pDC, const FontInfo& font);
	CString getTipText();
	void setInvalid(const CString& tipText);
	bool isValid();
	void Serialize(CArchive& ar);
	CellElement(CellData* cellData);
	virtual ~CellElement();

	void setChar(char elementChar);
	char getChar() {return _value;}

	void setData(CString data);
	CString getData() {return _data;}

	void setType(ParseType type);
	ParseType getType() {return _type;}

	void setWordVals(int pos, int length);
	int getPosInWord() {return _posInWord;}
	int getWordLength() {return _wordLength;}

	static const float SmallFontFactor;

protected:
	//serialisable data	
	char _value;
	CString _data;

	//non serialisable data
	bool _valid;
	CString _tipText;
	ParseType _type;
	int _posInWord;
	int _wordLength;
	CellData* _cellData; //parent
};

#endif // !defined(AFX_CELLELEMENT_H__B4C1DEE7_437E_11D5_B5CC_009027BB3286__INCLUDED_)
