#pragma once

struct ChunkHead
{
	BYTE        name[4];			// chunk name 
	DWORD       dwSize;				// Size of the chunk
};

struct FmtDetails
{
	WORD        waveFormatPCM;		// WAVE_FORMAT_PCM (should be 1)
	WORD        wChnls;				// Number of Channels
	DWORD       dwSRate;			// Sample Rate (samples per seconds)
	DWORD       dwBytesPerSec;		// Sample Rate (bytes per second)
	WORD        wBlkAlign;			// block align (should be 1)
	WORD        BitsPerSample;		// bits per sample
};


struct Wave
{
	BYTE		name[4];			// "Wave"
	ChunkHead   fmtHead;			// fmtHead.dwSize should 16
	FmtDetails	fmtDetails;			//fmt header
	ChunkHead   dataHead;			//data header
};

class Riff
{
public:
	Riff();
	~Riff();

	ChunkHead  riffHead;
	Wave	   wave;

	BOOL readRiffHeader(FILE* pFile, int& dataPosition);
	BOOL readRiffHeader(const char *fileName, CString& file);

};
