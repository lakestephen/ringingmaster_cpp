// StrikeManager.cpp: implementation of the StrikeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Strikemanager.h"
#include "StrikeDoc.h"
#include "BagleyImportFilter.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StrikeManager::StrikeManager() :
_analysisBell(0), 
_orderByBells(FALSE), 
_strikeAnalysisType(sat_handback)
{

}



StrikeManager::~StrikeManager()
{
}

BEGIN_MESSAGE_MAP(StrikeManager, CCmdTarget)
	//{{AFX_MSG_MAP(StrikeManager)
	ON_COMMAND(ID_SHOW_PLACE, OnShowPlace)
	ON_UPDATE_COMMAND_UI(ID_SHOW_PLACE, OnUpdateShowPlace)
	ON_COMMAND(ID_SHOW_BELL, OnShowBell)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BELL, OnUpdateShowBell)
	ON_COMMAND(IDC_IMPORT_BAGLEY, OnImportBagley)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
   

void StrikeManager::OnImportBagley() 
{
	BagleyImportFilter filter;

	StrikingData strikingData;
	CString fileName;
	
	if (filter.loadFile(mainFrame(), strikingData, fileName))
	{
		CDocument* pDoc = app()->openDocument(vt_strike);

		StrikeDoc* strikeDoc = dynamic_cast<StrikeDoc*>(pDoc);

		ASSERT(strikeDoc);

		if (strikeDoc)
		{
			strikeDoc->SetTitle(fileName);

			strikeDoc->setInitialData(strikingData);
		}   
	}       
}

void StrikeManager::closeDown()
{
	mainFrame()->removeEventListener(this);
}

void StrikeManager::addEventListener(StrikeManagerEventListener* strikeManagerEventListener)
{
	ASSERT(strikeManagerEventListener != NULL);

	_listenerList.Add(strikeManagerEventListener);
}

void StrikeManager::removeEventListener(StrikeManagerEventListener* strikeManagerEventListener)
{
	ASSERT(strikeManagerEventListener != NULL);

	int hasRemoved = 0;
	
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		if (_listenerList.GetAt(i) == strikeManagerEventListener)
		{
			_listenerList.RemoveAt(i);
			i--;
			hasRemoved++;
		}
	} 

	ASSERT(hasRemoved == 1);
}

void StrikeManager::save()
{
	AfxGetApp()->WriteProfileInt("Strike", "Analysis Bell", _analysisBell );	
	AfxGetApp()->WriteProfileInt("Strike", "Order By Bells", _orderByBells);
	AfxGetApp()->WriteProfileInt("Strike", "Strike Analysis Type", _strikeAnalysisType);
	
}

void StrikeManager::load()
{
	_analysisBell = AfxGetApp()->GetProfileInt("Strike", "Analysis Bell", 0);	
	_orderByBells = AfxGetApp()->GetProfileInt("Strike", "Order By Bells", 0);	
	_strikeAnalysisType = (StrikeAnalysisType)AfxGetApp()->GetProfileInt("Strike", "Strike Analysis Type", sat_handback);	

}

void StrikeManager::openUp()
{
	mainFrame()->addEventListener(this);
}

void StrikeManager::mainFrame_notifyUpdate(ViewType viewType)
{
	if (viewType == vt_strike)
		fireNotifyStrikeChanged();
}


void StrikeManager::fireNotifyStrikeChanged()
{
	for (int i=0;i<_listenerList.GetSize();i++)
	{
		_listenerList.GetAt(i)->strikeManager_notifyUpdateStrikeList();
	}
}

int StrikeManager::getMaxBells()
{
	StrikeDoc* pStrikeDoc = mainFrame()->getActiveStrikeDoc();

	if (pStrikeDoc)
		return pStrikeDoc->getStrikingData().getNumber();

	assert(false);
	return MAXBELLS;
}

int StrikeManager::getAnalysisBell()
{
	return max(0,min(getMaxBells()-1, _analysisBell));
}


bool StrikeManager::setAnalysisBell(int analysisBell)
{
	if (_analysisBell != analysisBell)
	{
		_analysisBell = analysisBell;
		fireNotifyStrikeChanged();
		return true;
	}

	return false;
}
void StrikeManager::OnShowPlace()
{
	_orderByBells = FALSE;
	updateDocument();
}

void StrikeManager::OnUpdateShowPlace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!_orderByBells);
}

void StrikeManager::OnShowBell()
{
	_orderByBells = TRUE;
	updateDocument();
}

void StrikeManager::OnUpdateShowBell(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_orderByBells);
}

BOOL StrikeManager::getOrderByBells()
{
	return _orderByBells;
}

void StrikeManager::updateDocument()
{
	StrikeDoc* pStrikeDoc = mainFrame()->getActiveStrikeDoc() ;

	assert(pStrikeDoc);

	if(pStrikeDoc)
	{
		pStrikeDoc->UpdateAllViews(NULL);
	}
}

StrikeAnalysisType StrikeManager::getStrikeAnalysisType()
{
	return _strikeAnalysisType;
}

void StrikeManager::setStrikeAnalysisType(StrikeAnalysisType strikeAnalysisType)
{
	_strikeAnalysisType = strikeAnalysisType;
	fireNotifyStrikeChanged();
}

