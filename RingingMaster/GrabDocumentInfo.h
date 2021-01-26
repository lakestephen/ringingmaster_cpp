#pragma once

class GrabDocumentInfo :
	public CObject
{
public:
	GrabDocumentInfo(void);
	GrabDocumentInfo(int selectedGrabIndex);
	~GrabDocumentInfo(void);


	void Serialize(CArchive&);

	DECLARE_SERIAL( GrabDocumentInfo )

	int getSelectedGrabIndex();

protected:
	int _selectedGrabIndex;

};

