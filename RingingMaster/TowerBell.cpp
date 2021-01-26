// TowerBell.cpp: implementation of the TowerBell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerBell.h"
#include "PlayBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerBell::TowerBell(const TowerBellPersist& towerBellPersist, Stroke stroke, double overallVolume, double overallfrequency, BOOL using3DSound) :
TowerBellPersist(towerBellPersist),
_stroke(stroke),
_overallVolume(overallVolume),
_using3DSound(using3DSound),
_overallFrequency(overallfrequency)
{

}

TowerBell::~TowerBell()
{

}

//0 is loudest, -10000 is quietest

int TowerBell::getVolume() const
{
	int vol = (int)((_overallVolume + TowerBellPersist::getTowerBellItem(_stroke).getVolumeOverride())*100.0);

	//as the 3d stuff is quieter, boost a little
	if (isSettingPos())
		vol += 275;

	return max(DSBVOLUME_MIN, min(DSBVOLUME_MAX, vol));
}

bool TowerBell::isSettingVolume()	const
{
	return getVolume() != 0;
}

double TowerBell::getPosY() const 
{
	if (_using3DSound)
		return TowerBellPersist::getPosY();
	else
		return 0;
}

double TowerBell::getPosX() const 
{
	if (_using3DSound)
		return TowerBellPersist::getPosX();
	else
		return 0;
}

double TowerBell::getPosZ() const 
{
	if (_using3DSound)
		return TowerBellPersist::getPosZ();
	else
		return 0;
}

bool TowerBell::isSettingPos() const
{
	if (_using3DSound)
		return TowerBellPersist::isSettingPos();
	else
		return false;
}



CString TowerBell::getFile() const
{
	return TowerBellPersist::getTowerBellItem(_stroke).getFile();
}
	
bool TowerBell::isSettingFrequency() const
{
	return (getFrequencyPercentage() != 100.0);
}

int TowerBell::getFrequency() const
{
	int frequency = 22050;
	
	Riff riff;															
	CString file;
	if (riff.readRiffHeader(getFile(), file))
	{
		frequency = riff.wave.fmtDetails.dwSRate;
	}

	frequency = (int)((double)frequency * (getFrequencyPercentage()/100.0));

	return max(DSBFREQUENCY_MIN, min(DSBFREQUENCY_MAX, frequency));
}

double TowerBell::getFrequencyPercentage()	const
{
	return _overallFrequency + TowerBellPersist::getTowerBellItem(_stroke).getFrequencyOverride();
}


bool TowerBell::operator !=(const TowerBell &towerBell)
{
	return !(operator == (towerBell));
}

bool TowerBell::operator ==(const TowerBell &towerBell)
{
	if (getFile() != towerBell.getFile())	return false;
	if (getFrequencyPercentage() != towerBell.getFrequencyPercentage())	return false;
	if (getVolume()!= towerBell.getVolume())	return false;

	if (getPosX()!= towerBell.getPosX())	return false;
	if (getPosY() != towerBell.getPosY())	return false;
	if (getPosZ()!= towerBell.getPosZ())	return false;

	return true;
}			   

