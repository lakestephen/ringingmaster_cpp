// SimulatorInput.h: interface for the SimulatorInput class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SerialControlAction.h"
#include "MultiBellAction.h"
#include "KeyboardAction.h"
#include "SpeechAction.h"


enum SimulatorInputType
{
	sit_none,
	sit_keyboard,
	sit_speechRecognition,
	sit_multiBellInterface,
	sit_serial,

	//NOTE: Allways add to the end
};

enum SimulatorInputDescriptionType
{
	sidt_short, 
	sidt_long, 
	sidt_long_base,
};


class SimulatorInput  
{
public:
	SimulatorInput();
	SimulatorInput(const SimulatorInput& simulatorInput);
	virtual ~SimulatorInput();

	SimulatorInput& operator =(const SimulatorInput& simulatorInput);
	bool operator ==(const SimulatorInput& simulatorInput) const;

	void Serialize(CArchive &ar);

	void setSimulatorInputType(SimulatorInputType type);
	SimulatorInputType getSimulatorInputType() const;
	CString getTypeText()const ;

protected:
	SimulatorInputType _type;

	KeyboardAction* _keyboardAction;	  
	SpeechAction* _speechAction;
	MultiBellAction* _multiBellAction;
	SerialControlAction* _serialControlAction;

public:
	bool isKeyboard() const; 
	void setKeyboard(KeyboardAction* keyboardAction);
	KeyboardAction* getKeyboard(void) const;

	void setSpeech(SpeechAction* action);
	SpeechAction* getSpeech(void) const;
	bool isSpeech() const;

	void setMultiBellInterface(MultiBellAction* action);
	bool isMultiBellInterface() const;
	MultiBellAction* getMultiBellInterface(void) const;

	void setSerialControl(SerialControlAction* action);
	bool isSerialControl() const;
	SerialControlAction* getSerialControl(void) const;

	CString getDescription(SimulatorInputDescriptionType version = sidt_long) const ;

	bool isValidResponse()const;

	bool isValid() const;
	bool isInput() const;
	void edit(CWnd* wnd);

};

template<>
	inline UINT AFXAPI HashKey(const SimulatorInput& key)
{
	//create a hash of the values
	switch (key.getSimulatorInputType())
	{
	case sit_keyboard:
		{
			int val = 1000;
			ASSERT(key.getKeyboard() != NULL);
			if (key.getKeyboard() != NULL)
			{
				val += key.getKeyboard()->getScancode();
			}
			return val;
		}

	case sit_multiBellInterface:
		{
			int val = 2000;
			ASSERT(key.getMultiBellInterface() != NULL);
			if (key.getMultiBellInterface() != NULL)
			{
				val += key.getMultiBellInterface()->getChannel();
			}
			return val;
		}

	case sit_serial:
		{
			int val = 3000;
			ASSERT(key.getSerialControl() != NULL);
			if (key.getSerialControl() != NULL)
			{
				val += key.getSerialControl()->getPort() * 10;
				val += key.getSerialControl()->getEvent();
			}
			return val;
		}
	case sit_speechRecognition:
		{
			int val = 4000;
			ASSERT(key.getSpeech() != NULL);
			if (key.getSpeech() != NULL)
			{
				val += HashKey<LPCTSTR>(key.getSpeech()->getWord());
			}
			return val;
		}

	default:
		ASSERT(FALSE);
	case sit_none:
		return 0;
	}
}


typedef CArray<SimulatorInput*, SimulatorInput*> SimulatorInputs;

