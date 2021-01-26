// ChildFrm.cpp : implementation of the ChildFrame class
//
#include "stdafx.h"
#include "RingingMaster.h"

#include "TowerView.h"
#include "GrabView.h"
#include "InternetView.h"
#include "MapView.h"
#include "MapDoc.h"

#include "ChildFrm.h"
#include "MapDocumentInfo.h"
#include "GrabDocumentInfo.h"
#include "TowerDocumentInfo.h"
#include "InternetDocumentInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString ChildFrame::_TowerDocumentToken = "<Tower Window>";
const CString ChildFrame::_GrabDocumentToken = "<Grab Window>";
const CString ChildFrame::_MapDocumentToken = "<Map Window>";
const CString ChildFrame::_InternetDocumentToken = "<Internet Window>";

// ChildFrame

IMPLEMENT_DYNCREATE(ChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(ChildFrame, CBCGPMDIChildWnd)
END_MESSAGE_MAP()


// ChildFrame construction/destruction

ChildFrame::ChildFrame():
documentObject(NULL)
{

}

ChildFrame::~ChildFrame()
{
	ASSERT(documentObject == NULL);
	delete documentObject;
}


BOOL ChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
/*
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
*/
	cs.style &= ~WS_SYSMENU;
	return TRUE;
}


// ChildFrame diagnostics

#ifdef _DEBUG
void ChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void ChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

									  
// ChildFrame message handlers
void ChildFrame::ActivateFrame(int /*nCmdShow*/)
{
	CBCGPMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

bool ChildFrame::createDocumentObject()
{
	documentName.Empty();
	documentObject = NULL;

	if (DYNAMIC_DOWNCAST (TowerView, GetActiveView ()) != NULL)
	{
		TowerView* towerView = DYNAMIC_DOWNCAST (TowerView, GetActiveView ());
		documentObject = towerView->getTowerDocumentInfo();
		if (documentObject)
			ASSERT(documentObject->IsKindOf(RUNTIME_CLASS(TowerDocumentInfo)));
		documentName = _TowerDocumentToken;
	}

	if (DYNAMIC_DOWNCAST (GrabView, GetActiveView ()) != NULL)
	{
		GrabView* grabView = DYNAMIC_DOWNCAST (GrabView, GetActiveView ());
		documentObject = grabView->getGrabDocumentInfo();
		if (documentObject)
			ASSERT(documentObject->IsKindOf(RUNTIME_CLASS(GrabDocumentInfo)));
		documentName = _GrabDocumentToken;
	}

	if (DYNAMIC_DOWNCAST (MapView, GetActiveView ()) != NULL)
	{
		MapView* mapView = DYNAMIC_DOWNCAST (MapView, GetActiveView ());
		documentObject = mapView->GetDocument()->getMapDocumentInfo();
		if (documentObject)
			ASSERT(documentObject->IsKindOf(RUNTIME_CLASS(MapDocumentInfo)));
		documentName = _MapDocumentToken;
	}	   

	if (DYNAMIC_DOWNCAST (InternetView, GetActiveView ()) != NULL)
	{
		InternetView* internetView = DYNAMIC_DOWNCAST (InternetView, GetActiveView ());
		documentObject = internetView->getInternetDocumentInfo();
		if (documentObject)
			ASSERT(documentObject->IsKindOf(RUNTIME_CLASS(InternetDocumentInfo)));
		documentName = _InternetDocumentToken;
	}	   
	return (!documentName.IsEmpty());
}


LPCTSTR ChildFrame::GetDocumentName (CObject** pObj)
{
	if (!documentName.IsEmpty())
	{
		(*pObj) = documentObject;
		documentObject = NULL;
		return documentName;
	}

	return CBCGPMDIChildWnd::GetDocumentName (pObj);
}


