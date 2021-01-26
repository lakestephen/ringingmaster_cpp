#include "StdAfx.h"
#include ".\rowholder.h"

#include "Method.h"
#include "Lead.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


RowHolder::RowHolder(int number) : //todo can we remove this class when Method just becomes a row holder?
_number(number)
{

}

RowHolder::RowHolder(const Method &method) : 
_number(method.getNumber())
{
	
	/* todo if still using this class, iterate through the rows.
	//we go through all the leads / rows, and copy them.
	for (int i = 0;i<method.GetSize();i++)
	{
		Lead* lead = method.GetAt(i);
		for (int j = (i==0)?0:1;j<lead->GetSize();j++)
		{
			Row* row = lead->GetAt(j);
			RowExtra* rowExtra = new RowExtra(*row, lead->GetUpperBound());
			rowExtra->setRow(&method, i,j);

			RowComments commentList;
			lead->getCommentList(commentList, j, rcut_sound);
			for (int k=0;k<commentList.GetSize();k++)
			{
				rowExtra->Add(new RowComment(*commentList.GetAt(k)));
			}

			Add(rowExtra);
		}
	}	*/
}

RowHolder::~RowHolder(void)
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
	
	RemoveAll();
}

int RowHolder::getNumber() const
{
	return _number;
}
