// TowerBell.h: interface for the TowerBell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWERBELL_H__5E206788_E071_4E4B_A831_4BA1F83E8B0C__INCLUDED_)
#define AFX_TOWERBELL_H__5E206788_E071_4E4B_A831_4BA1F83E8B0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerBellPersist.h"

class TowerBell : protected TowerBellPersist
{
public:
	int getFrequency() const;
	bool isSettingFrequency() const;
	CString getFile() const;
	bool isSettingVolume() const;
	int getVolume() const;

	TowerBell(const TowerBellPersist& towerBellPersist, Stroke stroke, double overallVolume, double overallfrequency, BOOL using3DSound);
	virtual ~TowerBell();

	double getPosY() const ;
	double getPosX() const ;
	double getPosZ() const ;

	bool isSettingPos() const;

	bool operator !=(const TowerBell &towerBell);
	bool operator ==(const TowerBell &towerBell);



protected:
	double _overallVolume;
	double _overallFrequency;
	BOOL _using3DSound;
	Stroke _stroke;

	

private:
	double getFrequencyPercentage() const;
};

#endif // !defined(AFX_TOWERBELL_H__5E206788_E071_4E4B_A831_4BA1F83E8B0C__INCLUDED_)
