// PlayBuffer.h : Definition of PlayBuffer class
//

#if !defined(AFX_PlayBuffer_H__7517D749_96E3_11D2_BBF3_9EB4940D843C__INCLUDED_)
#define AFX_PlayBuffer_H__7517D749_96E3_11D2_BBF3_9EB4940D843C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Riff.h"
#include "TowerBell.h"

class PlayBuffer :  public TowerBell
{
// Attribute
protected:
	LPDIRECTSOUNDBUFFER		_playBuffer;			// Sound buffer
	LPDIRECTSOUND3DBUFFER	_play3DBuffer;			// 3D buffer

// Construction / Destruction
public:
	PlayBuffer(const TowerBell& towerBell, LPDIRECTSOUND lpDS);
	~PlayBuffer();

// Methods
public:
	bool update(TowerBell& towerBell);
	BOOL	playSound(DWORD dwFlags);
	BOOL	stopSound();
	BOOL	isValid();

protected:
	BOOL	createSoundBuffer(LPDIRECTSOUND lpDS, DWORD dwFlags, DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, BOOL bStereo);
	BOOL	readData(FILE* pFile, DWORD dwSize, DWORD dwPos);
	bool isUpdateable(const TowerBell& towerBell);
	void setValues();
};


typedef CMap<int,int,PlayBuffer*, PlayBuffer*> PlayBufferMap;

#endif // !defined(AFX_PlayBuffer_H__7517D749_96E3_11D2_BBF3_9EB4940D843C__INCLUDED_)
