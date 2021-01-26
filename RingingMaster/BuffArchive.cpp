// BuffArchive.cpp: implementation of the BuffArchive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuffArchive.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuffArchive::BuffArchive(UINT nMode):
CArchive(new CMemFile, nMode)
{

}

BuffArchive::~BuffArchive()
{
	CMemFile * memFile = (CMemFile*)GetFile();
	if (IsLoading())
	{
		Close();
		memFile->Close();
	}
	delete memFile;
	memFile = NULL;
}

BYTE* BuffArchive::getData(int& length)
{
	ASSERT(IsStoring());

	CMemFile * memFile = (CMemFile*)GetFile();

	Close();

	length = (int)memFile->GetLength();

	BYTE* buff = new BYTE[(unsigned int)memFile->GetLength()];

	memFile->SeekToBegin();
	memFile->Read(buff, length);
	memFile->Close();

	return buff;
}

void BuffArchive::setData(BYTE* buff, int length)
{
	ASSERT(IsLoading());

	CMemFile * memFile = (CMemFile*)GetFile();

	memFile->Write( buff, length);
	memFile->SeekToBegin();
}