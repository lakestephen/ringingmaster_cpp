#include "stdafx.h"
#include "Riff.h"

Riff::Riff()
{
}

Riff::~Riff()
{
}

BOOL Riff::readRiffHeader(const char *fileName, CString& file)
{
	CFileFind find;
	if (!find.FindFile(fileName))
		return false;

	find.FindNextFile();
	file = find.GetFileName();

	// Open the wave file       
	FILE* pFile = fopen(fileName, "rb");
	if(!pFile)
		return false;

	// Read in the wave header          
	int dataPosition = 0;
	if (!readRiffHeader(pFile, dataPosition))
	{
		fclose(pFile);
		return false;
	}

	fclose(pFile);
	return true;
}

BOOL Riff::readRiffHeader(FILE* pFile, int& dataPosition) 
{
	//read the riff header chunk
	if ((!fread(&riffHead, sizeof(riffHead), 1, pFile))||
		(CString((LPCTSTR)riffHead.name, 4) != "RIFF"))
		return FALSE;

	//read the WAVE header name
	if ((!fread(&wave.name, sizeof(wave.name), 1, pFile))||
		(CString((LPCTSTR)wave.name, 4) != "WAVE"))
		return FALSE;
	
	//read the fmt chunk
	bool fmtDone = false;
	bool dataDone = false;
	while (!fmtDone || !dataDone)
	{
		ChunkHead chunkHead;

		if (!fread(&chunkHead, sizeof(chunkHead), 1, pFile))
			return FALSE;

		DWORD remainingChunk = chunkHead.dwSize;

		if (CString((LPCTSTR)chunkHead.name, 4) == "fmt ")
		{
			fmtDone = true;
			wave.fmtHead = chunkHead;
			if (!fread(&wave.fmtDetails, sizeof(wave.fmtDetails), 1, pFile))
				return FALSE;

			remainingChunk -= sizeof(wave.fmtDetails);
		}
		else if (CString((LPCTSTR)chunkHead.name, 4) == "data")
		{
			dataDone = true;
			wave.dataHead = chunkHead;

			fpos_t pos;
			if (fgetpos(pFile, &pos)!=0)
				return FALSE;
			dataPosition = (int)pos;
		}
		else
		{
			TRACE("[PlayBuffer.ReadRiffHeader]Read unknown chunk: %s\r\n", CString((LPCTSTR)chunkHead.name, 4)); 
		}

		//read any unleft chunks. These can be whole chunks or 
		// partial chunks where the format has been extended
		if (remainingChunk >0)
			if (fseek(pFile, remainingChunk, SEEK_CUR) != 0)
				return FALSE;

	}

	return TRUE;
}


