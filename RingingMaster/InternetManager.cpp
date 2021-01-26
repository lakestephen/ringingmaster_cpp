// InternetManager.cpp: implementation of the InternetManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "InternetManager.h"
#include "InternetSession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InternetManager::InternetManager() :
_session(NULL),
_checkedMessage(false)
{

}

InternetManager::~InternetManager()
{
	deleteSession();
}


void InternetManager::deleteSession()
{
	delete _session;
	_session = NULL;
}


InternetSession* InternetManager::getSession(bool newSession)
{
	if (newSession)
		deleteSession();

	if (!_session)
		_session = new InternetSession();

	return _session;
}

//this will actually connect to the internet!!!!
bool InternetManager::checkConnection(bool showMessage)
{
	deleteSession();

	CString page = getPage("http://homepages.nildram.co.uk/~thelakes/connect.txt");

	if (!page.IsEmpty())
	{


//		TRACE(page + "\r\n");

		if (page == "You are connected")
		{
			if (showMessage)
				AfxMessageBox("Connection verified.");
			return true;
		}
		else
		{
			if (showMessage)
				AfxMessageBox("Connection problem. Can't download test page.");
			return false;
		}
	}

	if (showMessage)
		AfxMessageBox("Connection problem. Can't get connection.");
	return false;	
}


BYTE* InternetManager::getHttpFIle(const CString &url, DWORD &length)
{
	checkMessage();

	if (getSession())
	{
		return getSession()->getHttpFIle(url, length);
	}
	return NULL;
}

CString InternetManager::getPage(const CString& url)
{
	DWORD length = 0;
	BYTE* buffer = getHttpFIle(url, length);

	CString output;

	if (buffer != NULL)
	{
		CString temp(buffer);
		output = temp.Left(length);

		output.FreeExtra();
	}

	delete[] buffer;

	return output;
}

void InternetManager::checkMessage()
{
	if (!_checkedMessage)
	{
		//have this first to avoid an infinite loop on checking message
		_checkedMessage = true;		

		CString page = getPage("http://homepages.nildram.co.uk/~thelakes/message.xml");

		if (!page.IsEmpty())
		{
	/*
			page = 
			"<mapcon> "\
				"<version>1</version> "\
				"<message>This is a test</message>"\
			"</mapcon>";

	*/		CString pageCopy = page; //as we are remobing all spaces

			while(page.Replace("\r", ""));
			while(page.Replace("\n", ""));
			while(page.Replace("\t", ""));
			while(page.Replace(" ", ""));

//			TRACE(page + "\r\n");
			CString token =  "<version>";
			int offset = 0;

			if ((offset = page.Find(token)) != -1)
			{
				offset += token.GetLength();

				CString snip = page.Mid(offset, 4);

				while(snip.Replace("<", ""));
				while(snip.Replace("/", ""));
				while(snip.Replace("v", ""));
				while(snip.Replace("e", ""));
				
				TRACE(snip + "\r\n");

				int messageNumber = atoi(snip);

				int lastMessageNumber = app()->GetProfileInt("Internet", "Last Message Number", 0);

				if (messageNumber > lastMessageNumber)
				{

					app()->WriteProfileInt("Internet", "Last Message Number", messageNumber);

					CString messageToken1 = "<message>";
					CString messageToken2 = "</message>";

					int offset1, offset2 = 0;
					if (((offset1 = pageCopy.Find(messageToken1)) != -1)&&
						((offset2 = pageCopy.Find(messageToken2, offset1)) != -1))

					{
						int start = offset1 + messageToken1.GetLength();
						
						CString message = pageCopy.Mid(start, offset2 - start);

						//as this could be called from a thread, post meassage in this function
						if (!message.IsEmpty())
							mainFrame()->setInternetMessage(message);
					}
				}
			}
		}
	}				 
}

