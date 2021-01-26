#pragma once
#include "matrixentitybase.h"

class MatrixEntityGrab :
	public MatrixEntityBase
{
public:
	MatrixEntityGrab(void);
	virtual ~MatrixEntityGrab(void);

public:

	MatrixEntityToken getMatrixEntityToken();
	
	UINT getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity);

	bool getEntityFromId(long id, SaveableObject*& entity);
	long getIdFromEntity(SaveableObject* entity, long& id);

	void createColumns(MatrixItemBases& columns);
	void createRelations(MatrixRelations& relations);

	void doubleClick(SaveableObject* originalEntity, CWnd* parent);

	int getUnfilteredIdCount() const;
	MatrixItemToken  getDefaultSortToken();
	
	void getStatusBarText(CString& pane1);
protected:

	void resetIdList(CArray<long, long>& ids);

public:
	CString getName(void);
	static const CString Name;



};
