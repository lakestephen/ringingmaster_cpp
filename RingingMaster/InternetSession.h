// InternetSession.h: interface for the InternetSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERNETSESSION_H__93E03DE9_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
#define AFX_INTERNETSESSION_H__93E03DE9_FAEF_11D6_B63D_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class InternetSession : public CInternetSession 
{
public:
	void log(CString msg);
	InternetSession();
	virtual ~InternetSession();

	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen);

	BYTE* getHttpFIle(const CString& url, DWORD& length);


protected:
	static CString getAddress();
	static DWORD getConnectionType();

};


#endif // !defined(AFX_INTERNETSESSION_H__93E03DE9_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
