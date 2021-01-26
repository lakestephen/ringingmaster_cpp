// CellData.cpp: implementation of the CellData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CellData.h"
#include "CellElement.h"
#include "TouchDoc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




CellData::CellData(TouchDoc* touchDoc) :
_touchDoc(touchDoc)
{

}

CellData::~CellData()
{
	destroyElements();
}

void CellData::Serialize(CArchive& ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _elements.GetSize();
		for (int i=0;i<_elements.GetSize();i++)
			_elements.GetAt(i)->Serialize(ar);
	}
	else
	{
		ar >> version;

		int size;
		ar >> size;

		for (int i=0;i<size;i++)
		{
			CellElement * element = new CellElement(this);
			element->Serialize(ar);
			_elements.Add(element);
		}
	}
}

bool CellData::onAddElement(UINT nChar, int elementPos, CString displayStr)
{
	if (iswprint(static_cast<wint_t>(nChar))) 
	{
		CellElement* element = new CellElement(this);
		element->setChar(static_cast<char>(nChar));
		element->setData(displayStr);
		_elements.InsertAt(elementPos, element);
		return true;
	} 
	return false;
}

bool CellData::onDeleteElement(int elementPos)
{
	if ((elementPos>=0)&&
		(elementPos < _elements.GetSize()))
	{
		delete _elements.GetAt(elementPos);
		_elements.RemoveAt(elementPos);
		return true;
	} 
	return false;
}

bool CellData::onDeleteElementRange(int start, int end)
{
	if (end == -1) end = getLength();

	if ((start>=0)&&
		(start < _elements.GetSize())&&
		(end >=0)&&
		(end <= _elements.GetSize()))
	{
		for (int i=start;i<end;i++)
		{
			delete _elements.GetAt(start);
			_elements.RemoveAt(start);
		}
		return true;
	} 
	return false;

}

int CellData::calculateCellWidth(CDC *pDC, const FontInfo& font)
{
	int stringWidth = 0;
	if (pDC)
		for (int i=0;i<getLength();i++)
			stringWidth += getElement(i)->getExtent(pDC, font).cx;
	
	return stringWidth;
}

//wordIndex
bool CellData::isValidWord(int index)
{
	if ((index <0)&&
		(index >= getWordCount()))
		return false;

	int start = getFirstElementIndexInWord(index);
	int end = start + getWordFromWordIndex(index).GetLength();

	for (int i=start;i<end;i++)
	{
		if (!_elements.GetAt(i)->isValid())
			return false;
	}
	return true;
}

//elementIndex
bool CellData::isValid(int index)
{
	if ((index >=0)&&
		(index < _elements.GetSize())&&
		(_elements.GetAt(index)->isValid()))
	{
		return true;
	}
	return false;
}

char CellData::getChar(int index)
{
	if ((index >=0)&&(index < _elements.GetSize()))
		return _elements.GetAt(index)->getChar();;
	return '\0';
}

void CellData::destroyElements()
{
	for(int i=0;i<_elements.GetSize();i++)
	{
		delete _elements.GetAt(i);
	}
	_elements.RemoveAll();	
}

CArray <CellElement*, CellElement*>& CellData::getElements()
{
	return _elements;
}


CString CellData::getString(int start, int end, bool includeInvalid)
{
	if (end == -1) end = getLength();

	CString str("");
	if ((start>=0)&&
		(start < _elements.GetSize())&&
		(end >=0)&&
		(end <= _elements.GetSize()))
	{
		for (int i=start;i<end;i++)
		{
			if ((includeInvalid)||
				(_elements.GetAt(i)->getType() == PT_NONE))
				str += _elements.GetAt(i)->getChar();
			else
				str += " ";
		}
	}
	return str;
}

CellElement* CellData::getElement(int index)
{
	if ((index>=0)&&
		(index < _elements.GetSize()))
	{
		return _elements.GetAt(index);
	}
	return NULL;
}



void CellData::parseNumbers(bool inSplice, bool hasDefault)
{
	int i;
	for (i=_elements.GetUpperBound();i>-1;i--)
	{
		CellElement* element = _elements.GetAt(i);
		
		
		if ((element->getType() == PT_NONE)&&
			(isdigit(element->getChar())))
		{
			ParseType nextType = PT_NONE;
			if (i<_elements.GetUpperBound())
				nextType = _elements.GetAt(i+1)->getType();

			switch (nextType)
			{
			case PT_CALL:
			case PT_CALL_NUM:
				if (!inSplice) 
				{
					element->setType(PT_CALL_NUM);
					addToNextWord(i);
				}
				break;
			case PT_NONE:
				if (!inSplice) 
				{
					element->setType(PT_CALL_DEFAULT_NUM);
					if (!hasDefault)
					{
						if (_touchDoc->isSpliced())
							element->setInvalid("Default call not defined in all methods");
						else
							element->setInvalid("No default call defined");
					}
				}				
				break;
			case PT_WHITESPACE:
			case PT_CALL_DEFAULT_NUM:
				if (!inSplice) 
				{
					element->setType(PT_CALL_DEFAULT_NUM);
					addToNextWord(i);
					if (!hasDefault)
					{
						if (_touchDoc->isSpliced())
							element->setInvalid("Default call not defined in all methods");
						else
							element->setInvalid("No default call defined");
					}
				}				
				break;
			case PT_VARIANCE:
				if (!inSplice) 
				{
					element->setType(PT_CALL_DEFAULT_NUM);
					if (!hasDefault)
					{
						if (_touchDoc->isSpliced())
							element->setInvalid("Default call not defined in all methods");
						else
							element->setInvalid("No default call defined");
					}
				}				
				break;

			case PT_GROUP:
				if (_elements.GetAt(i+1)->getChar() == '(')
				{
					element->setType(PT_GROUP_NUM);
					addToNextWord(i);
				}
				else if ((_elements.GetAt(i+1)->getChar() == ')')&&(!inSplice))
				{
					element->setType(PT_CALL_DEFAULT_NUM);
					if (!hasDefault)
					{
						if (_touchDoc->isSpliced())
							element->setInvalid("Default call not defined in all methods");
						else
							element->setInvalid("No default call defined");
					}
				}
				break;
			case PT_GROUP_NUM:
				{
					element->setType(PT_GROUP_NUM);
					addToNextWord(i);
				}
				break;
			case PT_BLOCK:
			case PT_BLOCK_NUM:
				if (!inSplice) 
				{
					element->setType(PT_BLOCK_NUM);
					addToNextWord(i);
				}
				break;
			case PT_PLAIN_LEAD:
			case PT_PLAIN_LEAD_NUM:
				if (!inSplice) 
				{
					element->setType(PT_PLAIN_LEAD_NUM);
					addToNextWord(i);
				}
				break;
			case PT_DEFINITION:
			case PT_DEFINITION_NUM:
				element->setType(PT_DEFINITION_NUM);
				addToNextWord(i);
				break;	
			case PT_SPLICE:
			case PT_SPLICE_NUM:
				if (inSplice) 
				{
					element->setType(PT_SPLICE_NUM);
					addToNextWord(i);
				}
				break;
			}
		}
	}
}

void CellData::parseCell(ParseDataArray *pParse)
{
	if (!pParse) return;

	sortParseDataArray(pParse);
	
	int i,j,pos;	
				
	for (i=0;i<pParse->GetSize();i++)
	{
		CString str = getString(0,-1,false);
		pos = -1;
		CString parseName = pParse->GetAt(i)->getName();
		while ((pos = str.Find(parseName,pos+1)) != -1)			
			
		{
			//check we are not trampling on anything
			bool isOk = true;
			for (j=0;j<parseName.GetLength();j++)
				if (_elements.GetAt(pos+j)->getType() != PT_NONE)
				{
					isOk = false;
					//break;
				}

			//set the elements up
			if (isOk)
				for (j=0;j<parseName.GetLength();j++)
				{
					CellElement* element = _elements.GetAt(pos+j);
					element->setType(pParse->GetAt(i)->getType()); 
					element->setWordVals(j,parseName.GetLength());
				}
		}
	}  				
}

void CellData::sortParseDataArray(ParseDataArray *pParse)
{
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<pParse->GetUpperBound();i++)
		{
			if ((pParse->GetAt(i )->getName().GetLength() <  pParse->GetAt(i+1)->getName().GetLength())||
				((pParse->GetAt(i)->getName().GetLength() == pParse->GetAt(i+1)->getName().GetLength())&&
				(pParse->GetAt(i)->getName() < pParse->GetAt(i+1)->getName())))
			{
				changed = true;
				ParseData * temp    = pParse->GetAt(i);
				pParse->SetAt(i,pParse->GetAt(i+1));
				pParse->SetAt(i+1, temp);
			}
		}
	}
}

int CellData::getWordCount()
{
	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return wordCount;
}

//index is the wordiNDEX
CString CellData::getWordFromWordIndex(int index)
{
	ASSERT ( index < getWordCount());

	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		if (wordCount == index)
			return getString().Mid(i,_elements.GetAt(i)->getWordLength());
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return CString("");
}

CString CellData::getWordFromElement(CellElement *element)
{
	int elementIndex = getElementIndexFromElement(element);

	if (elementIndex != -1)
		return getWordFromElementIndex(elementIndex);

	return CString("");
}

CString CellData::getWordFromElementIndex(int index, bool stripNumbers)
{
	ASSERT ( index < _elements.GetSize());

	int wordLength = _elements.GetAt(index)->getWordLength();
	int wordPos    = _elements.GetAt(index)->getPosInWord();

	ASSERT(index - wordPos+ wordLength <= _elements.GetSize());

	CString str(getString());
	CString retStr = str.Mid(index - wordPos, wordLength);
	
	if (stripNumbers)
	{
		CString remChar;
		for (int i=0;i<10;i++)
		{
			remChar.Format("%d", i);
			retStr.Remove(remChar[0]);
		}
	}
	return retStr;
}

TouchDoc* CellData::getTouchDoc()
{
	return _touchDoc;
}


//index is element Index
int CellData::getFirstLetterPos(int index)
{
	if ((index < 0)||(index >= _elements.GetSize())) return -1;

	return index -= _elements.GetAt(index)->getPosInWord();
}

void CellData::addToNextWord(int index)
{
	if ((index < 0)||(index > _elements.GetUpperBound()))
		return;

	if ((_elements.GetAt(index)->getType() == PT_NONE)||
		(_elements.GetAt(index)->getType() == PT_WHITESPACE))
		return;

	if (index == _elements.GetUpperBound())
	{
		_elements.GetAt(index)->setWordVals(0,1);
		return;
	}
	
	int length = getWordFromElementIndex(index+1).GetLength();

	for (int i=0;i<length+1;i++)
	{
		_elements.GetAt(index+i)->setWordVals(i,length+1);
	}
	
	if (!_elements.GetAt(index+1)->isValid())
		_elements.GetAt(index)->setInvalid(_elements.GetAt(index+1)->getTipText());

}

//index is wordIndex
CellElement* CellData::getFirstElementInWord(int index)
{
	ASSERT ( index < getWordCount());

	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		if (wordCount == index)
			return _elements.GetAt(i);
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return NULL;
}

//index is wordIndex
CellElement* CellData::getLastElementInWord(int index)
{
	ASSERT ( index < getWordCount());

	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		if (wordCount == index)
			return _elements.GetAt(i+getWordFromWordIndex(index).GetLength()-1);
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return NULL;
}


//index is wordIndex
int CellData::getFirstElementIndexInWord(int index)
{
	ASSERT ( index < getWordCount());

	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		if (wordCount == index)
			return i;
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return -1;
}

//index is wordIndex
int CellData::getLastElementIndexInWord(int index)
{
	ASSERT ( index < getWordCount());

	int wordCount = 0;
	for (int i=0;i<_elements.GetSize();)
	{
		if (wordCount == index)
			return i+getWordFromWordIndex(index).GetLength()-1;
		wordCount++;
		i+= _elements.GetAt(i)->getWordLength();
	}
	return -1;
}



void CellData::setInvalidWord(int index, const CString &tipText)
{
	int pos = getFirstElementIndexInWord(index);
	int length = getWordFromWordIndex(index).GetLength();

	if (pos < 0) return;
	if (length < 1) return;

	for (int i = pos ;i<pos+length;i++)
	{
		CellElement* element = getElement(i);
		ASSERT(element != NULL);

		if (element)
			element->setInvalid(tipText);
	}
}

int CellData::getNumFromElement(CellElement *element)
{
	int wordIndex = getWordIndexFromElement(element);

	if (wordIndex != -1)
		return getNumFromWordIndex(wordIndex);

	return 1;
}

//if no multiplyer, return 1
int CellData::getNumFromWordIndex(int index)
{
	if ((index < 0)||(index > getWordCount())) return false;
	
	int start = getFirstElementIndexInWord(index);
	int end = start + getWordFromWordIndex(index).GetLength();
	CString number;

	for (int i=start;i<end;i++)
	{
		CellElement* element = getElement(i);
		ASSERT(element != NULL);

		if ((element->getType() == PT_CALL_NUM)||
			(element->getType() == PT_CALL_DEFAULT_NUM)||
			(element->getType() == PT_SPLICE_NUM)||
			(element->getType() == PT_BLOCK_NUM)||			
			(element->getType() == PT_DEFINITION_NUM)||
			(element->getType() == PT_GROUP_NUM)||
			(element->getType() == PT_PLAIN_LEAD_NUM))
			number += element->getChar();
	}

	if (number.GetLength() > 0)
	{
		return atoi(number);
	}
	else
		return 1;
}

CString CellData::getWordWithoutNumFromElement(CellElement *element)
{
	int wordIndex = getWordIndexFromElement(element);

	if (wordIndex != -1)
		return getWordWithoutNumFromWordIndex(wordIndex);

	return CString("");
}


//index is the WORD index rather than the element index
CString CellData::getWordWithoutNumFromWordIndex(int index)
{
	if ((index < 0)||(index > getWordCount())) return "";
	
	int start = getFirstElementIndexInWord(index);
	int end = start + getWordFromWordIndex(index).GetLength();
	CString word;

	for (int i=start;i<end;i++)
	{
		CellElement* element = getElement(i);
		ASSERT(element != NULL);
		if (element->getType() == PT_CALL_DEFAULT_NUM)
		{
			return DEFAULT_CALL_TOKEN;
		}	
		if ((element->getType() == PT_CALL_NUM)||
			(element->getType() == PT_SPLICE_NUM)||
			(element->getType() == PT_BLOCK_NUM)||			
			(element->getType() == PT_DEFINITION_NUM)||
			(element->getType() == PT_GROUP_NUM))
		{
			//DONT ADD ANYTHING
		}
		else
		{
			word += element->getChar();
		}
	}

	return word;
}

int CellData::getWordIndexFromElement(CellElement *element)
{			  
	int elementIndex = getElementIndexFromElement(element);
	if (elementIndex != -1)
	{
		for (int i=0;i<getWordCount();i++)
		{
			if ((elementIndex >= getFirstElementIndexInWord(i))&&
				(elementIndex <= getLastElementIndexInWord(i)))
				return i;
		}
	}

	return -1;
}

int CellData::getElementIndexFromElement(CellElement *element)
{
	for (int i=0;i<	_elements.GetSize();i++)
	{
		if (element == _elements.GetAt(i))
			return i;
	}

	return -1;
}
