#pragma once

class InternetDocumentInfo :
	public CObject
{
public:
	InternetDocumentInfo(void);
	InternetDocumentInfo(CString url);
	~InternetDocumentInfo(void);


	void Serialize(CArchive&);

	DECLARE_SERIAL( InternetDocumentInfo )

	CString getURL();

protected:
	CString _url;

};
