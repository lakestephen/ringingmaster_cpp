#include "StdAfx.h"
#include ".\strikingerrors.h"

StrikingErrors::StrikingErrors(void) :
_useStrikingErrors(TRUE),
_quiet(FALSE),
_handstrokeLateness(0),
_backstrokeLateness(0), 
_handstrokeQuality(0),
_backstrokeQuality(0),
_followPreviousBellError(FALSE), 
_lockHandAndBack(TRUE)
{
	reset();
}

void StrikingErrors::reset()
{	
	_useStrikingErrors = TRUE;
	_quiet = FALSE;
	_handstrokeLateness = 0;
	_backstrokeLateness = 0;
	_handstrokeQuality = 0;
	_backstrokeQuality = 0;
	_followPreviousBellError = FALSE; 
	_lockHandAndBack = TRUE;
}

StrikingErrors::~StrikingErrors(void)
{
}



void StrikingErrors::Serialize(CArchive &ar)
{		
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;
		ar << _useStrikingErrors;
		ar << _quiet;
		ar << _handstrokeLateness; 
		ar << _backstrokeLateness; 
		ar << _handstrokeQuality;
		ar << _backstrokeQuality;
		ar << _followPreviousBellError;
		ar << _lockHandAndBack;
	}
	else
	{
		ar >> version;
		ar >> _useStrikingErrors;
		ar >> _quiet;
		ar >> _handstrokeLateness; 
		ar >> _backstrokeLateness; 
		ar >> _handstrokeQuality;
		ar >> _backstrokeQuality;
		ar >> _followPreviousBellError;
		ar >> _lockHandAndBack;
	}
}

bool StrikingErrors::operator !=(const StrikingErrors &strikingErrors)
{
	return !(operator==(strikingErrors));
}

bool StrikingErrors::operator ==(const StrikingErrors &strikingErrors)
{
	if (_useStrikingErrors  != strikingErrors._useStrikingErrors) return false;
	if (_quiet			    != strikingErrors._quiet) return false;
	if (_handstrokeLateness	!= strikingErrors._handstrokeLateness) return false;
	if (_backstrokeLateness	!= strikingErrors._backstrokeLateness) return false;
	if (_handstrokeQuality	!= strikingErrors._handstrokeQuality) return false;
	if (_backstrokeQuality	!= strikingErrors._backstrokeQuality) return false;
	if (_followPreviousBellError	!= strikingErrors._followPreviousBellError) return false;
	if (_lockHandAndBack != strikingErrors._lockHandAndBack) return false;

	return true; 
}

void StrikingErrors::setHandstrokeLateness(double handstrokeLateness)
{
	_handstrokeLateness = handstrokeLateness;

	if (_lockHandAndBack)
		_backstrokeLateness = handstrokeLateness;
}

void StrikingErrors::setBackstrokeLateness(double backstrokeLateness)
{
	_backstrokeLateness = backstrokeLateness;

	if (_lockHandAndBack)
		_handstrokeLateness = backstrokeLateness;

}

void StrikingErrors::setHandstrokeQuality(double handstrokeQuality)
{
	_handstrokeQuality = handstrokeQuality;

	if (_lockHandAndBack)
		_backstrokeQuality = handstrokeQuality;
}

void StrikingErrors::setBackstrokeQuality(double backstrokeQuality)
{
	_backstrokeQuality = backstrokeQuality;

	if (_lockHandAndBack)
		_handstrokeQuality = backstrokeQuality;
}

void StrikingErrors::setFollowPreviousBellError(BOOL followPreviousBellError)
{
	_followPreviousBellError = followPreviousBellError;
}

void StrikingErrors::setQuiet(BOOL quiet)
{
	_quiet = quiet;
}

void StrikingErrors::setUseStrikingErrors(BOOL useStrikingErrors)
{
	_useStrikingErrors = useStrikingErrors;
}

double StrikingErrors::getHandstrokeLateness()
{
	return _handstrokeLateness;
}

double StrikingErrors::getBackstrokeLateness()
{
	return _backstrokeLateness;	
}

double StrikingErrors::getHandstrokeQuality()
{
	return _handstrokeQuality;
}

double StrikingErrors::getBackstrokeQuality()
{
	return _backstrokeQuality;
}

BOOL StrikingErrors::getFollowPreviousBellError()
{
	return _followPreviousBellError;
}

BOOL StrikingErrors::getQuiet()
{
	return _quiet;
}

BOOL StrikingErrors::getUseStrikingErrors()
{
	return _useStrikingErrors;
}

void StrikingErrors::setLockHandAndBack(BOOL lockHandAndBack)
{
	_lockHandAndBack = lockHandAndBack;

	if (_lockHandAndBack)
	{
		_backstrokeLateness = _handstrokeLateness;
		_backstrokeQuality = _handstrokeQuality;
	}
}

BOOL StrikingErrors::getLockHandAndBack()
{
	return _lockHandAndBack;
}
