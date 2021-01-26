#include "stdafx.h"
#include "RingingMaster.h"
#include "Lead.h"
#include "Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



Lead::Lead() 
{
	
}

Lead::~Lead() 
{
 
}


int Lead::getHuntCount() const
{
	if (GetSize() < 2) return 1;

	int count = 0;
	for (int i=0;i<GetAt(0)->getNumber();i++)
	{
		if (GetAt(0)->getBellInPlace(i) == GetAt(GetUpperBound())->getBellInPlace(i))
			count++;
	}
	return count;
}
