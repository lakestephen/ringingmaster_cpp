// InfoDlg.cpp: implementation of the InfoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "InfoDlg.h"
#include "Notation.h"
#include "Method.h"
#include "Lead.h"
#include "ProofManager.h"
#include "MethodAnalysis.h"
#include "ProofInput.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoDlg::InfoDlg(CWnd* pWnd, Notation& notation) :
GenericPropDlg(pWnd, notation.getName(true) + " statistics", "MethodInfo", 140)
{
	notation.prepare();

	ProofInput proofInput(notation.getNumber());
	proofInput.addNotation(notation);
	proofInput.setPerformAnalysis(); //todo do we want to do analysis here?

	Method method;
	proofManager()->requestProof(proofInput, method);

	CString val;

	addProp("Name", notation.getName());

	addProp("Number", GlobalFunctions::numberName(notation.getNumber()));

	addProp("Notation Type", notation.isSymmetric()?"Symmetric":"Asymmetric");

	if (method.getAnalysis()->isTrue())
		addProp("The plain course is", "true");
	else
		addProp("The plain course is", "false");

	val.Format("%d", notation.getChangesPerPlainLead());
	addProp("Changes in a plain lead", val);

	val.Format("%d", method.getLeadCount());
	addProp("Leads in a plain course", val);

	val.Format("%d", method.getRowCount());
	addProp("Changes in a plain course", val);
	  
	val.Format("%d", method.getLead(0).getHuntCount());
	addProp("Number bells in the hunt", val);
		
	val.Format("%d", notation.getCallCount());
	addProp("Number of calls defined", val);
}

InfoDlg::~InfoDlg()
{

}
