// SpeechVoice.cpp: implementation of the SpeechVoice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SpeechVoice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpeechVoice::SpeechVoice(CString id, CString name) :
_id(id), 
_name(name)
{

}

SpeechVoice::~SpeechVoice()
{

}

CString SpeechVoice::getName()
{
	return _name;
}

CString SpeechVoice::getID()
{
	return _id;
}
