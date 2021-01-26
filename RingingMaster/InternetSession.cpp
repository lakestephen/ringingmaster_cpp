// InternetSession.cpp: implementation of the InternetSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "InternetSession.h"
#include "key.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/*todo sort out what should be done with the first parameter how does this get past a firewall (e.g. norton internet)*/ 
InternetSession::InternetSession() :
CInternetSession(Key::calculateKey("Internet", 1234), 1, getConnectionType() , getAddress(), NULL, INTERNET_FLAG_DONT_CACHE)
{
	VERIFY(EnableStatusCallback(TRUE));
} 

CString InternetSession::getAddress()
{
	int b1 = app()->GetProfileInt("Internet", "IP Address 1", 0);
	int b2 = app()->GetProfileInt("Internet", "IP Address 2", 0);
	int b3 = app()->GetProfileInt("Internet", "IP Address 3", 0);
	int b4 = app()->GetProfileInt("Internet", "IP Address 4", 0);
	int port = app()->GetProfileInt("Internet", "Port", 80);

	CString address;
	address.Format("%d.%d.%d.%d:%d", b1,b2,b3,b4,port);
	
	return address;
}

DWORD InternetSession::getConnectionType()
{
	return (app()->GetProfileInt("Internet", "Connection Type", 0)==0)?
			INTERNET_OPEN_TYPE_PRECONFIG:INTERNET_OPEN_TYPE_PROXY;
}



InternetSession::~InternetSession()
{
	Close();
}


DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT |INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD;

//user is responsable for deleting the returned value
BYTE* InternetSession::getHttpFIle(const CString& url, DWORD& length)
{
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	BYTE* output = NULL;
	CString msg;
	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType, dwRet, dwPrompt;

	try
	{
		// check to see if this is a reasonable URL
		if (!AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			log("Error: can only use URLs beginning with http://.") ;
			return NULL;
		}						   

		//set up the call back
		log("Opening Internet...");
		

		//get the connection		
		pServer = GetHttpConnection(strServerName, nPort);

		//deal with the request
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->SendRequest();  
		pFile->QueryInfoStatusCode(dwRet);

		// if access was denied, prompt the user for the password
		if (dwRet == HTTP_STATUS_DENIED)
		{
			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

			// if the user cancelled the dialog, bail out

			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			{
				log("Access denied: Invalid password");
				return NULL;
			}

			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		CString strNewLocation;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

		// were we redirected?
		// these response status codes come from WININET.H

		while (dwRet == HTTP_STATUS_MOVED ||
			dwRet == HTTP_STATUS_REDIRECT ||
			dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{
			log("Redirecting") ;
			CString strNewLocation;
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

			int nPlace = strNewLocation.Find(_T("Location: "));
			if (nPlace == -1)
			{
				log("Error: Site redirects with no new location.") ;
				return NULL;
			}

			strNewLocation = strNewLocation.Mid(nPlace + 10);
			nPlace = strNewLocation.Find('\n');
			if (nPlace > 0)
				strNewLocation = strNewLocation.Left(nPlace);

			// close up the redirected site

			pFile->Close();
			delete pFile;
			pServer->Close();
			delete pServer;

			msg.Format("Caution: redirected to %s", (LPCTSTR) strNewLocation);
			log(msg);

			// figure out what the old place was
			if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
			{
				log("Error: the redirected URL could not be parsed.") ;
				return NULL;
			}

			if (dwServiceType != INTERNET_SERVICE_HTTP)
			{
				log("Error: the redirected URL does not reference a HTTP resource..") ;
				return NULL;
			}

			// try again at the new location
			pServer = GetHttpConnection(strServerName, nPort);
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
				strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
			pFile->SendRequest();

			pFile->QueryInfoStatusCode(dwRet);
			if (dwRet != HTTP_STATUS_OK)
			{
				msg.Format("Error: Got status code %s.", dwRet);
				log(msg);
				return NULL;
			}
		}

		msg.Format("Status Code is %d.", dwRet);
		log(msg);

		const int size = 200000;
		output = new BYTE[size];

		length = pFile->Read(output, size);

	}
	catch (CInternetException* pEx)
	{
		msg.Format("Error: ( %d ).", pEx->m_dwError );
		log(msg);

		pEx->Delete();
	}

	pFile->Close();
	delete pFile;

	pServer->Close();
	delete pServer;

	return output;
}


void InternetSession::OnStatusCallback(DWORD /* dwContext */, DWORD dwInternetStatus,
	LPVOID /* lpvStatusInfomration */, DWORD /* dwStatusInformationLen */)
{
	if (dwInternetStatus ==  INTERNET_STATUS_RESOLVING_NAME          )
			log("Resolving Name");

	if (dwInternetStatus ==  INTERNET_STATUS_NAME_RESOLVED)
			log("Name Resolved");

	if (dwInternetStatus ==  INTERNET_STATUS_CONNECTING_TO_SERVER)
			log("Connecting To Server");

	if (dwInternetStatus ==  INTERNET_STATUS_CONNECTED_TO_SERVER)
			log("Connected To Server");

	if (dwInternetStatus ==  INTERNET_STATUS_SENDING_REQUEST)
			log("Sending Request");

	if (dwInternetStatus ==  INTERNET_STATUS_REQUEST_SENT)
			log("Request Sent");

	if (dwInternetStatus ==  INTERNET_STATUS_RECEIVING_RESPONSE)
			log("Recieving Response");

	if (dwInternetStatus ==  INTERNET_STATUS_RESPONSE_RECEIVED)
			log("Response Recieved");

	if (dwInternetStatus ==  INTERNET_STATUS_CTL_RESPONSE_RECEIVED)
			log("Ctl Response Recieved");

	if (dwInternetStatus ==  INTERNET_STATUS_PREFETCH)
			log("Prefetch");

	if (dwInternetStatus ==  INTERNET_STATUS_CLOSING_CONNECTION)
			log("Closing Connection");

	if (dwInternetStatus ==  INTERNET_STATUS_CONNECTION_CLOSED)
			log("Connection Closed");

	if (dwInternetStatus ==  INTERNET_STATUS_HANDLE_CREATED)
			log("Handle Created");

	if (dwInternetStatus ==  INTERNET_STATUS_HANDLE_CLOSING)
			log("Handle Closing");

	if (dwInternetStatus ==  INTERNET_STATUS_REQUEST_COMPLETE)
			log("Request Complete");

	if (dwInternetStatus ==  INTERNET_STATUS_REDIRECT)
			log("Redirect");

	if (dwInternetStatus ==  INTERNET_STATUS_INTERMEDIATE_RESPONSE)
			log("Intermediate Response");

	if (dwInternetStatus ==  INTERNET_STATUS_STATE_CHANGE)
			log("Response Change");
}

//todo Have the option of logging to file / screen.
void InternetSession::log(CString msg)
{
	TRACE(msg + "\r\n");
}




