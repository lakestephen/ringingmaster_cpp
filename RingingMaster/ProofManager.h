#pragma once

#include "ProofManagerEventListener.h"
#include "ManagerBase.h"

class ProofInput;
class ProofRequest;

class ProofManager : public CCmdTarget, public ManagerBase 
{
public:
	void addEventListener(ProofManagerEventListener* proofManagerEventListener);
	void removeEventListener(ProofManagerEventListener* proofManagerEventListener);
	bool isCalculatingProof();
	int requestProof(const ProofInput& proofInput, Method& method);
	int requestProofAsynch(ProofInput* proofInput, Method* method);
	void requestProofTerminate(int id);
	void save();
	void load();
	ProofManager();
	virtual ~ProofManager();

	afx_msg void OnProofThreadComplete();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProofManager)
	public:
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(ProofManager)

	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

protected:

	CMap<int, int, ProofRequest*, ProofRequest*> _proofRequestMap;
	static UINT doProof( LPVOID pParam );

	void process();

	ProofManagerEventListeners _listenerList;


	
};

