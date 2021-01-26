#pragma once

#include "MapGridRef.h"

class MapDocumentInfo :
	public CObject
{

public:
	MapDocumentInfo();
	MapDocumentInfo(const CString& _name,
					const MapGridRef& _mapGridRef,
					int _selectedTowerIndex);

	virtual ~MapDocumentInfo(void);

	void Serialize(CArchive&);

	DECLARE_SERIAL( MapDocumentInfo )

	const MapGridRef& getMapRef();
	const CString& getName();
	int getSelectedTowerIndex();

protected:
	CString _name;
	MapGridRef _mapGridRef;
	int _selectedTowerIndex;
};
