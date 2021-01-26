// ProofManager.cpp: implementation of the ProofManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "ProofManager.h"
#include "UserMessages.h"
#include "ProofRequest.h"
#include "ProofInput.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



ProofManager::ProofManager() 
{

}

BEGIN_MESSAGE_MAP(ProofManager, CCmdTarget)
	//{{AFX_MSG_MAP(ProofManager)
	ON_COMMAND(PROOF_THREAD_COMPLETE, OnProofThreadComplete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


ProofManager::~ProofManager()
{
	int id;
	ProofRequest* request;
	POSITION pos = _proofRequestMap.GetStartPosition();

	//set all to early terminate
	while (pos)
	{
		_proofRequestMap.GetNextAssoc(pos, id, request);
		if (request)
			request->requestEarlyTerminate();
	}

	//keep going untill all are terminated. 
	while(isCalculatingProof())
	{
		TRACE("[ProofManager::~ProofManager] Waiting \r\n");
		Sleep(100);
		process();
	}

	ASSERT(!isCalculatingProof());
}

void ProofManager::addEventListener(ProofManagerEventListener* proofManagerEventListener)
{
	//if we are the first to be added, then open the port
	ASSERT(proofManagerEventListener != NULL);

	_listenerList.Add(proofManagerEventListener);

}

void ProofManager::removeEventListener(ProofManagerEventListener* proofManagerEventListener)
{
	ASSERT(proofManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == proofManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}


void ProofManager::load()
{
}

void ProofManager::save()
{
}

void ProofManager::requestProofTerminate(int id)
{
	ProofRequest* request = NULL;
	if (_proofRequestMap.Lookup(id, request))
	{
		TRACE("[ProofManager::requestProofTerminate] %s\r\n", request->toString() );	
		request->requestEarlyTerminate();
	}	
}

int ProofManager::requestProof(const ProofInput& proofInput, Method& method)
{		
	//some checks
	ASSERT(proofInput.getNotations().GetSize() > 0); 
	if (proofInput.getNotations().GetSize() == 0)
		return -1;

	//do the proof
	ProofRequest request(proofInput, method);
	request.doProof();

	return request.getUniqueId();
}	

int ProofManager::requestProofAsynch(ProofInput* proofInput, Method* method)
{
	ASSERT(method);

	//do some checks
	ASSERT(proofInput);	
	if (!proofInput)
		return -1;

	ASSERT(proofInput->getNotations().GetSize() > 0); 
	if (proofInput->getNotations().GetSize() == 0)
		return -1;

	ProofRequest* request = new ProofRequest(proofInput, method);

	TRACE("[ProofManager::requestProofAsynch] %s\r\n", request->toString() );	

	//todo the is should be with the request.
	int requestID = request->getUniqueId();

	//add to proof map
	_proofRequestMap.SetAt(requestID, request);
	
	CWinThread* thread = 
		AfxBeginThread(	doProof, request, 
					THREAD_PRIORITY_LOWEST, 
					0,		//stack size
					CREATE_SUSPENDED, 
					NULL );	//security attributes


	thread->ResumeThread();
	
	return requestID;

}


UINT ProofManager::doProof( LPVOID pParam)
{			 
	ProofRequest* request = reinterpret_cast<ProofRequest*>(pParam);
	
	request->doProof();

	//let the manager know
	::PostMessage(mainFrame()->GetSafeHwnd() ,WM_COMMAND, PROOF_THREAD_COMPLETE, 0 );

	return 0;
}


void ProofManager::OnProofThreadComplete()
{
	process();
}

void ProofManager::process()
{
	//through all the methods in the map, and see if any have finished.
	int requestID;
	ProofRequest* request;
	POSITION pos = _proofRequestMap.GetStartPosition();

	while (pos)
	{
		_proofRequestMap.GetNextAssoc(pos, requestID, request);

		if((request)&&(request->hasActuallyTerminated()))
		{
			if (request->hasTerminatedEarly())
			{
				//if it terminated early, then just delete it
				TRACE("[ProofManager::process] Terminated Early: %s\r\n", request->toString());
				_proofRequestMap.RemoveKey(requestID);
				delete request;
				request = NULL;
			}	
			else								
			{
				//see if anyone wants it.
				bool consumed = false;
				for (int i=0;i<_listenerList.GetSize();i++)
				{
					if (_listenerList.GetAt(i)->proofManager_notifyProofComplete(request->getMethod()))
					{
						consumed = true;
						request->takeOwnershipOfMethod(); //stop the request deleting it.
						TRACE("[ProofManager::process] Consumed: %s \r\n", request->toString() );
						break;
					}
				}

				if (!consumed)
				{
					//no one wanted it
					TRACE("[ProofManager::process] Not Consumed: %s\r\n", request->toString() );
					ASSERT(FALSE);
				}

				delete request;
				request = NULL;
			}

			//all terminated methods get removed fromthe map
			_proofRequestMap.RemoveKey(requestID);
		}
	}
}

bool ProofManager::isCalculatingProof()
{
	return _proofRequestMap.GetCount() > 0;
}
