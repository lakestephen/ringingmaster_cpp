// PlayRequest.cpp: implementation of the PlayRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PlayRequest.h"
#include "Method.h"
#include "Tower.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlayRequest::PlayRequest(const Method &method) :
RowHolder(method), //todo change roholder to method 
_pullsPerChange(0),
_lastRowIndefiniteRepeat(false), 
_methdodRequestId(method.getUniqueId()), 
_isStrikingData(false)
{

}    

PlayRequest::PlayRequest(const StrikingData& strikingData) :
RowHolder(strikingData.getNumber()),
_pullsPerChange(0),
_lastRowIndefiniteRepeat(false), 
_methdodRequestId(0),
_isStrikingData(true)
{
	_strikingData = strikingData;
}

PlayRequest::~PlayRequest()
{
}

int PlayRequest::getPullsPerChange()
{
	return _pullsPerChange;
}

void PlayRequest::setPullsPerChange(int pullsPerChange)
{
	_pullsPerChange = pullsPerChange;
}

void PlayRequest::setLastRowIndefiniteRepeat(bool lastRowIndefiniteRepeat)
{
	_lastRowIndefiniteRepeat = lastRowIndefiniteRepeat;
}

bool PlayRequest::getLastRowIndefiniteRepeat()
{
	return _lastRowIndefiniteRepeat;
}

int PlayRequest::getUniqueId() const
{
	if (_isStrikingData) 
		return _strikingData.getUniqueId();
	return _methdodRequestId;
}


bool PlayRequest::getAnotherRow(Stroke /*stroke*/) 
{
	return false;
}

int PlayRequest::getStartChangeRepeat(Tower* tower) const 
{
	return tower->getStartChangeRepeat();
}

bool PlayRequest::isStrikingData() const
{
	return _isStrikingData;
}

int PlayRequest::getEndChangeRepeat(Tower* tower) const
{
	return tower->getEndChangeRepeat();
}


const StrikingData& PlayRequest::getStrikingData()
{
	return _strikingData;
}
