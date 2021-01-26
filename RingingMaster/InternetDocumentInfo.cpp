#include "StdAfx.h"
#include ".\internetdocumentinfo.h"

InternetDocumentInfo::InternetDocumentInfo(void)
{
}

InternetDocumentInfo::~InternetDocumentInfo(void)
{
}

InternetDocumentInfo::InternetDocumentInfo(CString url) :
_url(url)
{

}
IMPLEMENT_SERIAL( InternetDocumentInfo, CObject, 1)


void InternetDocumentInfo::Serialize(CArchive& ar)
{
	int version = 1;

	if (ar.IsStoring())
	{
		ar << version;
		ar << _url;
	}
	else
	{
		ar >> version;
		ar >> _url;
	}
}

CString InternetDocumentInfo::getURL()
{
	return _url;
}