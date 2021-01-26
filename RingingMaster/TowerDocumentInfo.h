#pragma once

class TowerDocumentInfo :
	public CObject
{
public:
	TowerDocumentInfo(void);
	TowerDocumentInfo(int selectedTowerIndex);
	~TowerDocumentInfo(void);


	void Serialize(CArchive&);

	DECLARE_SERIAL( TowerDocumentInfo )

	int getSelectedTowerIndex(); //change to uniqieid

protected:
	int _selectedTowerIndex;

};

