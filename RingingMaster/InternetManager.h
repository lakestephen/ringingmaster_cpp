// InternetManager.h: interface for the InternetManager class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "ManagerBase.h"

class InternetSession;

class InternetManager : public ManagerBase 
{
public:
	void checkMessage();
	void deleteSession();
	bool checkConnection(bool showMessage);
	InternetManager();
	virtual ~InternetManager();

	CString getPage(const CString& url);
	BYTE* getHttpFIle(const CString& url, DWORD& length);

	void save(){};
	void load(){};

protected:	
	InternetSession* _session;
	InternetSession* getSession(bool newSession = false);
	bool _checkedMessage;

};

