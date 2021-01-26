// TowerBellPersist.h: interface for the TowerBellPersist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWERBELLPERSIST_H__57C95AB5_0937_4EEF_9C53_9C3CDA23E026__INCLUDED_)
#define AFX_TOWERBELLPERSIST_H__57C95AB5_0937_4EEF_9C53_9C3CDA23E026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TowerBellItem.h"
#include "Row.h"

class TowerBellPersist  
{
public:
	TowerBellPersist();
	virtual ~TowerBellPersist();

	void Serialize(CArchive &ar);

	void setPosZ(double z);
	void setPosY(double y);
	void setPosX(double x);
	virtual double getPosY() const ;
	virtual double getPosX() const ;
	virtual double getPosZ() const ;

	virtual bool isSettingPos() const;


	TowerBellItem& getTowerBellItem(Stroke stroke);
	const TowerBellItem& getTowerBellItem(Stroke stroke) const;


	bool operator ==(const TowerBellPersist &towerBell);
	bool operator !=(const TowerBellPersist &towerBell);

protected:


	double _posX;
	double _posY;
	double _posZ;

	TowerBellItem _hand;
	TowerBellItem _back;

};

#endif // !defined(AFX_TOWERBELLPERSIST_H__57C95AB5_0937_4EEF_9C53_9C3CDA23E026__INCLUDED_)
