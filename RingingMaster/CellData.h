// CellData.h: interface for the CellData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CellData_H__3B2C19A2_3F9A_11D5_B5CA_009027BB3286__INCLUDED_)
#define AFX_CellData_H__3B2C19A2_3F9A_11D5_B5CA_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CellElement;
class FontInfo;
class TouchDoc;
#include "ParseData.h"

class CellData  
{
public:
	int getElementIndexFromElement(CellElement* element);
	int getWordIndexFromElement(CellElement* element);
	CString getWordWithoutNumFromWordIndex(int index);
	CString getWordWithoutNumFromElement(CellElement *element);
	int getNumFromWordIndex(int index);
	int getNumFromElement(CellElement *element);
	void setInvalidWord(int index, const CString& tipText);
	CellElement* getFirstElementInWord(int index);
	CellElement* getLastElementInWord(int index);
	int getFirstElementIndexInWord(int index);
	int getLastElementIndexInWord(int index);
	void addToNextWord(int index);
	int getFirstLetterPos(int index);
	CString getWordFromElement(CellElement* element);
	TouchDoc* getTouchDoc();
	CString getWordFromElementIndex(int index, bool stripNumbers = false);
	CString getWordFromWordIndex(int index);
	int getWordCount();
	void sortParseDataArray(ParseDataArray *pParse);
	void parseCell(ParseDataArray* pParse);
	void parseNumbers(bool inSplice, bool hasDefault);
	bool onDeleteElementRange(int start = 0, int end = -1);
	CString getString(int start = 0, int end = -1, bool includeInvalid = true);
	char getChar(int index);
	bool isValid(int index);
	bool isValidWord(int index);
	int calculateCellWidth(CDC * pDC, const FontInfo& font);
	CellData(TouchDoc* touchDoc);
	virtual ~CellData();
	CellElement* getElement(int index);

	void Serialize(CArchive& ar);

	int getLength() {return _elements.GetSize();}
	bool onAddElement(UINT nChar, int elementPos, CString displayStr = CString(""));
	bool onDeleteElement(int elementPos);
	void destroyElements();
	CArray <CellElement*, CellElement*>& getElements();


protected:
	CArray <CellElement*, CellElement*> _elements;
	

	TouchDoc* _touchDoc;
};

#endif // !defined(AFX_CellData_H__3B2C19A2_3F9A_11D5_B5CA_009027BB3286__INCLUDED_)
