///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             DirectSound Mixer                             //
//                                                                           //
//                      V1.0 by lob_b@hotmail.com 1999                       //
//                                                                           //
//                           with core inputs from                           //
//                                 Stack-Up                                  //
//                    V1.0 © Tool@theWaterCooler.com 1998                    //
//                    http://www.theWaterCooler.com/Tool                     //
//                        also Petr.Stejskal@vslib.cz                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// PlayBuffer.cpp : Implementation of PlayBuffer class
//

#include "stdafx.h"
#include "PlayBuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PlayBuffer

					  
PlayBuffer::PlayBuffer(const TowerBell& towerBell, LPDIRECTSOUND lpDS) :
TowerBell(towerBell),
_playBuffer(NULL),
_play3DBuffer(NULL)	 
{	
	// Open the wave file       
	FILE* pFile = fopen(getFile(), "rb");
	if(!pFile)
		return;

	// Read in the wave header          
	Riff riff;
	int dataPosition = 0;
	if (!riff.readRiffHeader(pFile, dataPosition)) 
	{
		fclose(pFile);
		return;
	}

	ASSERT(dataPosition > 0);

	// Figure out the size of the data region
	DWORD dwSize = riff.wave.dataHead.dwSize;

	// Is this a stereo or mono file?
	BOOL bStereo = riff.wave.fmtDetails.wChnls > 1 ? true : false;

	//determine flags from what needs setting
	DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D;
	
	if (isSettingVolume())
		dwFlags |= DSBCAPS_CTRLVOLUME;

	if (isSettingFrequency())
		dwFlags |= DSBCAPS_CTRLFREQUENCY;

	if (isSettingPos())
		dwFlags |= DSBCAPS_CTRL3D;

	// Create the sound buffer for the wave file
	if(createSoundBuffer(lpDS, dwFlags, dwSize, riff.wave.fmtDetails.dwSRate, riff.wave.fmtDetails.BitsPerSample, riff.wave.fmtDetails.wBlkAlign, bStereo))
	{
		// Read the data for the wave file into the sound buffer
		if (!readData(pFile, dwSize, dataPosition))
		{
			TRACE("[PlayBuffer] Error Reading Data");		  
			if (_playBuffer)
			{
		 		_playBuffer->Release();
				_playBuffer = NULL;
			}
		}
		else if (dwFlags & DSBCAPS_CTRL3D)
		{
			// Get pointer to 3D buffer
			if (S_OK != _playBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void **)&_play3DBuffer))
			{
				TRACE("[PlayBuffer] DirectSound 3DBuffer not available");   
			}
		}
	}

	setValues();

	fclose(pFile);
}

void PlayBuffer::setValues()
{
	if (_playBuffer) 
	{
		if (isSettingVolume())
		{
			_playBuffer->SetVolume(getVolume()); 
		}

		if (isSettingFrequency())
			_playBuffer->SetFrequency(getFrequency()); 
	}

	if (_play3DBuffer) 
	{
		if (isSettingPos())
		{
			//x is left right
			//y is up down
			//z is front back
			_play3DBuffer->SetPosition((float)getPosX(), (float)getPosY(), 
					(float)getPosZ(), DS3D_IMMEDIATE);
		}

	}
}

PlayBuffer::~PlayBuffer()
{
	stopSound();
	if(_playBuffer)
	{       
		_playBuffer->Release();
	}

	if(_play3DBuffer)
	{       
		_play3DBuffer->Release();
	}
}


BOOL PlayBuffer::createSoundBuffer(LPDIRECTSOUND lpDS, 
								  DWORD dwFlags, 
								  DWORD dwBufSize, 
								  DWORD dwFreq, 
								  DWORD dwBitsPerSample, 
								  DWORD dwBlkAlign, 
								  BOOL bStereo)
{
	PCMWAVEFORMAT pcmwf;
	DSBUFFERDESC dsbdesc;

	// Set up wave format structure.
	memset( &pcmwf, 0, sizeof(PCMWAVEFORMAT) );
	pcmwf.wf.wFormatTag         = WAVE_FORMAT_PCM;      
	pcmwf.wf.nChannels          = (unsigned short)(bStereo ? 2 : 1);
	pcmwf.wf.nSamplesPerSec     = dwFreq;
	pcmwf.wf.nBlockAlign        = (WORD)dwBlkAlign;
	pcmwf.wf.nAvgBytesPerSec    = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample        = (WORD)dwBitsPerSample;

	// Set up PlayBufferDESC structure.
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));  // Zero it out. 
	dsbdesc.dwSize              = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags             = dwFlags;
	dsbdesc.dwBufferBytes       = dwBufSize; 
	dsbdesc.lpwfxFormat         = (LPWAVEFORMATEX)&pcmwf;
	dsbdesc.guid3DAlgorithm = DS3DALG_HRTF_FULL;

	if (DS_OK != lpDS->CreateSoundBuffer(&dsbdesc, &_playBuffer, NULL))
	{
		TRACE("[PlayBuffer::CreateSoundBuffer]Error "); 
		if (_playBuffer)
			_playBuffer = NULL;
		_playBuffer = NULL;
		return FALSE;
	}

	return TRUE;
}

BOOL PlayBuffer::readData(FILE* pFile, DWORD dwSize, DWORD dwPos) 
{
	// Seek to correct position in file (if necessary)
	if (dwPos != 0xffffffff) 
	{
		if (fseek(pFile, dwPos, SEEK_SET) != 0) 
		{
			return FALSE;
		}
	}

	// Lock data in buffer for writing
	LPVOID pData1;
	DWORD  dwData1Size;
	LPVOID pData2;
	DWORD  dwData2Size;
	HRESULT rval;

	rval = _playBuffer->Lock(0, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, DSBLOCK_FROMWRITECURSOR);
	if (rval != DS_OK)
	{
		return FALSE;
	}

	// Read in first chunk of data
	if (dwData1Size > 0) 
	{
		if (fread(pData1, dwData1Size, 1, pFile) != 1) 
		{               
			return FALSE;
		}
	}

	// read in second chunk if necessary
	if (dwData2Size > 0) 
	{
		if (fread(pData2, dwData2Size, 1, pFile) != 1) 
		{
			return FALSE;
		}
	}

	// Unlock data in buffer
	rval = _playBuffer->Unlock(pData1, dwData1Size, pData2, dwData2Size);
	if (rval != DS_OK)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL PlayBuffer::playSound(DWORD dwFlags)
{
	if(_playBuffer)	// Make sure we have a valid sound buffer
	{
		DWORD dwStatus;
		if (DS_OK != _playBuffer->GetStatus(&dwStatus))
		{
			TRACE("[PlayBuffer::PlaySound] Error getting Status");
			return FALSE;
		}

		if((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
		{
			_playBuffer->Stop();
			_playBuffer->SetCurrentPosition(0);
		}

		if (DS_OK != _playBuffer->Play(0, 0, dwFlags))		// Play the sound
		{
			TRACE("[PlayBuffer::PlaySound] Error Playing");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL PlayBuffer::stopSound()
{
	if(_playBuffer)	// Make sure we have a valid sound buffer
	{
		DWORD dwStatus;
		if (DS_OK != _playBuffer->GetStatus(&dwStatus))
		{
			TRACE("[PlayBuffer::PlaySound] Error getting Status");
			return FALSE;
		}

		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
		{  
			if (DS_OK != _playBuffer->Stop())		// Stop the sound
			{
				TRACE("[PlayBuffer::PlaySound] Error Stopping");
				return FALSE;
			}

		}
	}
	return TRUE;
}


BOOL PlayBuffer::isValid()
{
	if (_playBuffer)
		return TRUE;
	else
		return FALSE;
}



bool PlayBuffer::update(TowerBell &towerBell)
{
	//has anything changed?
	if (*this == towerBell)
		return true;

	if (isUpdateable(towerBell))
	{
		TowerBell::operator = (towerBell);	
		
		setValues();

		TRACE("updating PlayBuffer" + getFile() + "\r\n");
		
		return true;
	}
	
	return false;
}

bool PlayBuffer::isUpdateable(const TowerBell &towerBell)
{
	if (getFile() != towerBell.getFile())
		return false;
	if (isSettingFrequency() != towerBell.isSettingFrequency())
		return false;
	if (isSettingPos() != towerBell.isSettingPos())
		return false;
	if (isSettingVolume() != towerBell.isSettingVolume())
		return false;
	
	return true;
}
