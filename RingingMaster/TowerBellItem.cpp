// TowerBellItem.cpp: implementation of the TowerBellItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerBellItem.h"
#include "PlayBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TowerBellItem::TowerBellItem() :
_frequencyOverride(0),	 //-100 to 100 %
_volumeOverride(0)	//-1000 to 1000 (-10Db to 10 Db)
{

}

TowerBellItem::~TowerBellItem()
{

}

const CString& TowerBellItem::getFile() const
{	
	return _file;
}

void TowerBellItem::setFile(CString file)
{
	if (_file != file)
	{
		_file = file;
		resetFrequencyOverride();
		resetVolumeOverride();
	}	 
}

double TowerBellItem::getFrequencyOverride() const
{
	return _frequencyOverride;
}

void TowerBellItem::setFrequencyOverride(double frequencyOverride)
{
	_frequencyOverride = frequencyOverride;
}

double TowerBellItem::getVolumeOverride() const
{
	return _volumeOverride;
}

void TowerBellItem::setVolumeOverride(double volumeOverride)
{
	_volumeOverride = volumeOverride;
}

bool TowerBellItem::operator !=(const TowerBellItem &towerBellItem)
{
	return !(operator == (towerBellItem));
}

bool TowerBellItem::operator ==(const TowerBellItem &towerBellItem)
{
	if (_file != towerBellItem._file)	return false;
	if (_frequencyOverride != towerBellItem._frequencyOverride)	return false;
	if (_volumeOverride != towerBellItem._volumeOverride)	return false;

	return true;
}			   


//started at release 3.01
void TowerBellItem::Serialize(CArchive &ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _file;
		ar << _frequencyOverride;
		ar << _volumeOverride;
	}
	else
	{
		ar >> version;

		TRACE("Deserialising TowerBellPersist Version %d \r\n", version);

		ar >> _file;
		ar >> _frequencyOverride;
		ar >> _volumeOverride;
	}
}


void TowerBellItem::resetFrequencyOverride()
{
	_frequencyOverride = 0;
}

bool TowerBellItem::isSettingFrequencyOverride() const
{
	return _frequencyOverride != 0;
}

void TowerBellItem::resetVolumeOverride()
{
	_volumeOverride = 0;
}

bool TowerBellItem::isValid()
{
  	Riff riff;
	CString file;
	return riff.readRiffHeader(_file, file) == TRUE;
}

bool TowerBellItem::isSettingVolumeOverride()
{
	return _volumeOverride != 0;
}
