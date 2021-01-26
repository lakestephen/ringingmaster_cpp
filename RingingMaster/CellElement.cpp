// CellElement.cpp: implementation of the CellElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "CellElement.h"
#include "CellData.h"
#include "Variance.h"
#include "TouchDoc.h"
#include "FontInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const float CellElement::SmallFontFactor = 0.55f;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CellElement::CellElement(CellData* cellData) :
_cellData(cellData)
{
	initElement();
}

CellElement::~CellElement()
{

}

void CellElement::Serialize(CArchive &ar)
{		   
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _value;
		ar << _data;
	}
	else
	{
		ar >> version;
		ar >> _value;
		ar >> _data;
	}		 
}

void CellElement::setChar(char elementChar)
{
	_value = elementChar;
	initElement();

}

void CellElement::initElement()
{	 
	_type = PT_NONE;
	_valid = false;
	_tipText = "Unrecognised symbol";
	_posInWord = 0;
	_wordLength = 1;
}

void CellElement::setData(CString data)
{
	_data = data;
}

bool CellElement::isValid()
{
	return _valid;
}

void CellElement::setInvalid(const CString& tipText)
{
	_valid = false;
	_tipText = tipText;
}

void CellElement::setType(ParseType type) 
{
	_type = type;
	_valid = (_type != PT_NONE);
	if (_valid)
		_tipText = "";
	else
	{
		_tipText = "Unrecognised symbol";
		_posInWord = 0;
		_wordLength = 1;
	}
}

CString CellElement::getTipText()
{
	if (_tipText.IsEmpty())
	{
		CString text = _cellData->getWordFromElement(this);
		text.TrimLeft();
		text.TrimRight();

		//get the document
		if (_type == PT_WHITESPACE)
		{
			text = "Space";
		}		
		else if ((_type == PT_VARIANCE)&&(_value == '['))
		{
			Variance var(_data);	
			text = var.getDisplayString();
		} 
		else if ((_type == PT_SPLICE)||(_type == PT_SPLICE_NUM))
		{
			TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc) 
			{
				const Notations& notations = pTouchDoc->getNotations();
				for (int i=0;i<notations.GetSize();i++)
				{
					CString spliceLetter = _cellData->getWordWithoutNumFromElement(this);
					if (notations.GetAt(i)->isSpliceLetter(spliceLetter))
					{
						text = notations.GetAt(i)->getName();
						break;
					}
				}
			}
		}
		else if (_type == PT_CALL_DEFAULT_NUM)
		{

			TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc) 
			{
				CString callLetter = _cellData->getWordWithoutNumFromElement(this);

				callLetter.TrimLeft();
				callLetter.TrimRight();

				if (pTouchDoc->isSpliced())
				{
					//find each default call and see if its longhand matches
					//do not need to worry about checking for long hands as these will be caught by the default processing

					CString longhand;
					
					const Notations& notations = pTouchDoc->getNotations();
					for (int i=0;i<notations.GetSize();i++)
					{
						Notation* notation = notations.GetAt(i);
						//go through each call
						NotationCall* call = notation->getStandardCall();

						if ((call)&&
							((longhand.IsEmpty())||(longhand == call->_nameLonghand)))
							longhand = call->_nameLonghand;
						else
							break;

						if (i == notations.GetUpperBound())
						{
							int number = _cellData->getNumFromElement(this);
							if (number > 1)
								text.Format("%d %s\'s", number, longhand);
							else
								text = longhand;
							
							break;
						}
					}
				}
				else
				{
					Notation* notation = pTouchDoc->getActiveNotation();
					NotationCall* call = notation->getStandardCall();

					CString callStr = (call->_nameLonghand.GetLength() >0)? call->_nameLonghand: call->_nameShorthand;

					if (call)
					{
						int number = _cellData->getNumFromElement(this);
						if (number > 1)
							text.Format("%d %s\'s", number, callStr);
						else
							text =  callStr;
					}
				}
			}

		}
		else if ((_type == PT_CALL)||(_type == PT_CALL_NUM))
		{
			TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
			if (pTouchDoc) 
			{
				CString callLetter = _cellData->getWordWithoutNumFromElement(this);

				callLetter.TrimLeft();
				callLetter.TrimRight();

				if (pTouchDoc->isSpliced())
				{
					//find each shorthand in each method that matches, then check each longhand matches 
					//do not need to worry about checking for long hands as these will be caught by the default processing

					CString longhand;
					
					const Notations& notations = pTouchDoc->getNotations();
					for (int i=0;i<notations.GetSize();i++)
					{
						bool foundCall = false;
						Notation* notation = notations.GetAt(i);
						//go through each call
						for (int j=0;j<notation->getCallCount();j++)
						{
							NotationCall* call = notation->getCall(j);

							if (call->_nameShorthand == callLetter)
							{
								if ((longhand.IsEmpty())||(longhand == call->_nameLonghand))
								{
									longhand = call->_nameLonghand;
									foundCall = true;
									break;
								}
							} 
						}
						if (!foundCall) 
							break;

						if (i == notations.GetUpperBound())
						{
							int number = _cellData->getNumFromElement(this);
							if (number > 1)
								text.Format("%d %s\'s", number, longhand);
							else
								text = longhand;
							
							break;
						}
					}
				}
				else
				{
					Notation* notation = pTouchDoc->getActiveNotation();
					for (int i=0;i<notation->getCallCount();i++)
					{
						NotationCall* call = notation->getCall(i);
						CString callStr = (call->_nameLonghand.GetLength() >0)? call->_nameLonghand: call->_nameShorthand;

						if (callLetter == call->_nameShorthand)
						{
							int number = _cellData->getNumFromElement(this);
							if (number > 1)
								text.Format("%d %s\'s", number, callStr);
							else
								text = callStr;							
							break;
						}								
					} 				
				}
			}
		}
		else if (_type == PT_CALL_POS_AGREGATE)
		{
		 //to2do get the breakdown of the agregate
		}


		//default processing
		if (text == " ")
			return "";
		else
			return getParseText(_type) + ": " + text + "" ;
	}

	return _tipText;
}


CSize CellElement::getExtent(CDC *pDC, const FontInfo& font)
{
	CSize size = pDC->GetTextExtent(CString(_value));
	if ((_value == '[')||(_value == ']'))
	{
		FontInfo smallerFontInfo = font;
		smallerFontInfo._size = (int)(((float)smallerFontInfo._size)*SmallFontFactor);
		smallerFontInfo._size = max(smallerFontInfo._size, 60); 
		LOGFONT lf;
		smallerFontInfo.getLogFont(&lf);
		CFont smallerFont;
		smallerFont.CreateFontIndirect(&lf);	   		
		CFont* oldFont = pDC->SelectObject(&smallerFont);
		size.cx += pDC->GetTextExtent(_data).cx;
		pDC->SelectObject(oldFont);
	}

	return size;
}

void CellElement::draw(int x, int y, CDC *pDC, COLORREF color, const FontInfo& font)
{
	pDC->SetTextColor(color);
	if (_value == '[')
	{
		pDC->TextOut(x,y,"[");
		int bracketWidth = pDC->GetTextExtent("[").cx;
		FontInfo smallerFontInfo = font;
		smallerFontInfo._size = (int)(((float)smallerFontInfo._size)*SmallFontFactor);
		smallerFontInfo._size = max(smallerFontInfo._size, 60); 
		LOGFONT lf;
		smallerFontInfo.getLogFont(&lf);
		CFont smallerFont;
		smallerFont.CreateFontIndirect(&lf);	   		
		CFont* oldFont = pDC->SelectObject(&smallerFont);
		pDC->TextOut(x+bracketWidth, y, _data);
		pDC->SelectObject(oldFont);
	}
	else if (_value == ']')
	{
		FontInfo smallerFontInfo = font;
		smallerFontInfo._size = (int)(((float)smallerFontInfo._size)*SmallFontFactor);
		smallerFontInfo._size = max(smallerFontInfo._size, 60); 
		LOGFONT lf;
		smallerFontInfo.getLogFont(&lf);
		CFont smallerFont;
		smallerFont.CreateFontIndirect(&lf);	   		
		CFont* oldFont = pDC->SelectObject(&smallerFont);
		
		pDC->TextOut(x+1, y, _data);
		
		int width = pDC->GetTextExtent(_data).cx;
		pDC->SelectObject(oldFont);
		
		pDC->TextOut(x+width,y,"]");
	}
	else
		pDC->TextOut(x,y,CString(_value));
}



void CellElement::setWordVals(int pos, int length)
{
	ASSERT(length >0);
	_posInWord = pos;
	_wordLength = length;
}



