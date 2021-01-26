// TowerBellItem.h: interface for the TowerBellItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOWERBELLITEM_H__105AF11B_F225_471E_B3B9_88167D419ED5__INCLUDED_)
#define AFX_TOWERBELLITEM_H__105AF11B_F225_471E_B3B9_88167D419ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TowerBellItem  
{
public:
	bool isValid();
	
	void setVolumeOverride(double volumeOverride);
	double getVolumeOverride() const;
	void resetVolumeOverride();
	bool isSettingVolumeOverride();

	void setFrequencyOverride(double frequencyOverride);
	double getFrequencyOverride() const ;
	void resetFrequencyOverride();
	bool isSettingFrequencyOverride() const;
	
	void setFile(CString file);
	const CString& getFile() const;
	TowerBellItem();
	virtual ~TowerBellItem();
	bool operator ==(const TowerBellItem &towerBellItem);
	bool operator !=(const TowerBellItem &towerBellItem);
					 
	void Serialize(CArchive &ar);

protected:
	CString _file;
	double _frequencyOverride;
	double _volumeOverride;

};

#endif // !defined(AFX_TOWERBELLITEM_H__105AF11B_F225_471E_B3B9_88167D419ED5__INCLUDED_)
