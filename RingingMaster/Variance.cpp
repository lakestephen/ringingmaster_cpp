// Variance.cpp: implementation of the Variance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Variance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Variance::Variance() :
_omit(true),
_varianceType(vt_parts)
{

}

Variance::Variance(CString gridString) :
_omit(true),
_varianceType(vt_parts)
{	  
	setGridString(gridString);
}

Variance::Variance(const Variance& variance) :
_omit(variance._omit),
_varianceType(variance._varianceType)
{
	for(int i=0;i<variance._parts.GetSize();i++)
	{
		_parts.Add(variance._parts.GetAt(i));
	}
}

void Variance::operator =(const Variance &variance)
{
	_omit = variance._omit;
	_varianceType = variance._varianceType;

	_parts.RemoveAll();

	for(int i=0;i<variance._parts.GetSize();i++)
	{
		_parts.Add(variance._parts.GetAt(i));
	}
}

Variance::~Variance()
{

}

//this is for the edit box
CString Variance::getVarianceStr()
{
	CString str, temp;

	switch (_varianceType)
	{
	case vt_parts:
		{
			for (int i=0;i<_parts.GetSize();i++)
			{
				temp.Format("%d", _parts.GetAt(i));
				str += temp;
				if (i < _parts.GetUpperBound())
					str+= ",";

			}
		}
		break;
	case vt_even:
		str = "E";
		break;
	case vt_odd:
		str = "O";
		break;
	}

	return str;
}

//this is for the property notation
CString Variance::getDisplayString()
{
	CString str, temp;
	switch (_varianceType)
	{
	case vt_parts:
		{
			if (_omit) 
				str = "Omit from part";
			else
				str = "Include in part";

			if (_parts.GetSize() > 1)
				str += "s ";
			else
				str += " ";

			str += getVarianceStr();
		}
		break;
	case vt_even:
		{
			if (_omit) 
				str = "Omit from even parts";
			else
				str = "Include in even parts";
		}
		break;
	case vt_odd:
		{
			if (_omit) 
				str = "Omit from odd parts";
			else
				str = "Include in odd parts";
		}
		break;
	}

	return str;
}

CString Variance::getGridString()
{
	CString str;
	if (_omit)
		str += "-";
	else
		str += "+";

	str += getVarianceStr();

	return str;
}

void Variance::setGridString(CString gridString)
{
	_omit = true;
	_parts.RemoveAll();

	if (gridString.IsEmpty())
		return;

	if (gridString.GetAt(0) == '+')
		_omit = false;

	setParts(gridString);
}

BOOL Variance::isOmit()
{
	return _omit;
}

//serialise not actually used
void Variance::Serialize(CArchive &ar)
{
	int version = 2;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _omit;
		ar << _parts.GetSize();
		for (int i=0;i<_parts.GetSize();i++)
			ar << _parts.GetAt(i);
		ar << _varianceType;
	}
	else
	{
		ar >> version;

		TRACE("Deserialising Variance Version %d \r\n", version);

		if (version == 1)
			deserialize_v1(ar);
		else if (version == 2)
			deserialize_v2(ar);
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   
	}
}
//version 2 is version 2.01 onwards
void Variance::deserialize_v2(CArchive& ar) 
{
	ar >> _omit;
	int size;			 
	ar >> size;
	int value;
	for (int i=0;i<size;i++)
	{
		ar >> value;
		_parts.Add(value);
	}
	int varianceType;
	ar >> varianceType;
	_varianceType = (VarianceType)varianceType;
	
}

//version 1 is version 1.07 onwards
void Variance::deserialize_v1(CArchive& ar) 
{
	ar >> _omit;
	int size;
	ar >> size;
	int value;
	for (int i=0;i<size;i++)
	{
		ar >> value;
		_parts.Add(value);
	}
}


bool Variance::setParts(CString parts)
{   
	int i,j;

	_parts.RemoveAll();

	while(parts.Replace("  "," "));
	while(parts.Replace("+",""));
	while(parts.Replace("-",""));

	if (parts.GetLength() <= 0)
		return false;

	if (parts.GetAt(0) == 'E')
	{
		_varianceType = vt_even;
		return true;
	}
	else if (parts.GetAt(0) == 'O')
	{
		_varianceType = vt_odd;
		return true;
	}
	else
		_varianceType = vt_parts;

	for (i=0;i<parts.GetLength();i++)
		if (!isdigit(parts.GetAt(i)))
			parts.SetAt(i, ' ');

	parts.TrimLeft();
	parts.TrimRight();	 

	for (i=0,j=0;i<=parts.GetLength();i++)
		if ((i == parts.GetLength())||
			(parts.GetAt(i) == ' ' ))
		{
			int num = atoi(parts.Mid(j,i-j));
			if ((num > 0)&&(num < 100000))
				_parts.Add(num);
			j=i;
		}

	sortParts();

	return (_parts.GetSize() >0);		
}

//true for omit, false for include
void Variance::setOmit(bool omit)
{
	_omit = omit;
}

void Variance::sortParts()
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<_parts.GetUpperBound();i++)
		{
			if (_parts.GetAt(i) == _parts.GetAt(i+1))
			{
				changed = true;
				_parts.RemoveAt(i);
				i--;
			}
			else if (_parts.GetAt(i) > _parts.GetAt(i+1))
			{
				changed = true;
				int temp = _parts.GetAt(i);
				_parts.SetAt(i,_parts.GetAt(i+1));
				_parts.SetAt(i+1, temp);
			}
		}
	}
}


bool Variance::isBenign() const
{
	return ((_varianceType == vt_parts)&&(_parts.GetSize() == 0));
}

bool Variance::isInPart(int partIndex) const
{
	//can we find the part in the list?
	bool partInList = false;

	switch (_varianceType)
	{
	case vt_parts:
		{	
			for (int i=0;i<_parts.GetSize();i++)
			{
				if (_parts.GetAt(i) == (partIndex +1)) //parts are in text format and 1 based
				{
					partInList = true;
					break;
				}
			}
		}
		break;
	case vt_even:
		partInList = !GlobalFunctions::isEven(partIndex);
		break;
	case vt_odd:
		partInList = GlobalFunctions::isEven(partIndex);
		break;
	}

	//in part when either 
	return (partInList != (_omit == TRUE)); 
}


VarianceType Variance::getVarianceType()
{
	return _varianceType;
}

void Variance::setVarianceType(VarianceType varianceType)
{
	_varianceType = varianceType;
}

bool Variance::operator ==(const Variance &variance) const
{  
	bool partsOK = _parts.GetSize() == variance._parts.GetSize();
	
	if (partsOK)
	{
		for (int i=0;i<_parts.GetSize();i++)
		{	
			if (_parts.GetAt(i) != variance._parts.GetAt(i))
			{
				partsOK = false;
				break;
			}		  
		}
	}	
	
	return ((partsOK)&&
			(_omit == variance._omit)&&
			(_varianceType == variance._varianceType));
}
