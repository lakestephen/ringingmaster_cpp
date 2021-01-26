// SpeechVoice.h: interface for the SpeechVoice class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

class SpeechVoice  
{
public:
	CString getID();
	CString getName();
	SpeechVoice(CString id, CString name);
	virtual ~SpeechVoice();

protected:
	CString _id;
	CString _name;

};

typedef CArray<SpeechVoice*, SpeechVoice*> SpeechVoices;


