#include "StdAfx.h"
#include "SpeechAction.h"
#include "MultiBellInterfaceManager.h"

SpeechAction::SpeechAction(const CString& word)

{
	_word = word;
	_word = _word.Trim();
	_word = _word.MakeLower();
}


SpeechAction::SpeechAction()
{
}

SpeechAction::~SpeechAction(void)
{
}

CString SpeechAction::getDescription() const
{
	return _word;
}

const CString& SpeechAction::getWord()   const
{
	return _word;
}

void SpeechAction::setWord(const CString& word)
{
	_word = word;
	_word = _word.MakeLower();
}

bool SpeechAction::operator ==(const SpeechAction & speechAction) const
{
	return (_word == speechAction._word);
}

void SpeechAction::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  

		ar << _word;
	}
	else
	{
		ar >> version;  

		ar >> _word;
	}
}

bool SpeechAction::isValid() const
{
	return !_word.IsEmpty();
}