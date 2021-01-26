// SimulatorInput.cpp: implementation of the SimulatorInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorInput.h"

#include "KeyboardManager.h"
#include "SpeechManager.h"
#include "SerialControlManager.h"
#include "MultiBellInterfaceManager.h"

#include "SerialControlResponse.h"
#include "SerialControlItem.h"

#include "MultiBellItem.h"

#include "KeyboardItem.h"
#include "KeyboardResponse.h"

#include "SpeechItem.h"
#include "SpeechResponse.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimulatorInput::SimulatorInput() :
_type(sit_none),
_speechAction(NULL),
_keyboardAction(NULL),
_multiBellAction(NULL),
_serialControlAction(NULL)
{
							   
}

SimulatorInput::SimulatorInput(const SimulatorInput& simulatorInput)  :
_keyboardAction(NULL),
_multiBellAction(NULL),
_speechAction(NULL),
_serialControlAction(NULL)
{
	operator = (simulatorInput);
}

SimulatorInput::~SimulatorInput()
{
	delete _serialControlAction;
	_serialControlAction = NULL;

	delete _keyboardAction;
	_keyboardAction = NULL;

	delete _multiBellAction;
	_multiBellAction = NULL;

	delete _speechAction;
	_speechAction = NULL;
}


SimulatorInput& SimulatorInput::operator =(const SimulatorInput & simulatorInput)
{
	_type						= simulatorInput._type;

	{
		delete _keyboardAction;
		_keyboardAction = NULL;

		KeyboardResponse* response = dynamic_cast<KeyboardResponse*>(simulatorInput._keyboardAction);
		KeyboardItem* item = dynamic_cast<KeyboardItem*>(simulatorInput._keyboardAction);
		KeyboardAction* action = dynamic_cast<KeyboardAction*>(simulatorInput._keyboardAction);
		if (response)
			_keyboardAction = new KeyboardResponse(*response);
		else if (item)
			_keyboardAction = new KeyboardItem(*item);
		else if (action)
			_keyboardAction = new KeyboardAction(*action);//must be last as it is the base class of the others
		else
			_keyboardAction = NULL;  
	}

	{
		delete _multiBellAction;
		_multiBellAction = NULL;

		MultiBellResponse* response = dynamic_cast<MultiBellResponse*>(simulatorInput._multiBellAction);
		MultiBellItem* item = dynamic_cast<MultiBellItem*>(simulatorInput._multiBellAction);
		MultiBellAction* action = dynamic_cast<MultiBellAction*>(simulatorInput._multiBellAction);
		if (response)
			_multiBellAction = new MultiBellResponse(*response);
		else if (item)
			_multiBellAction = new MultiBellItem(*item);
		else if (action)
			_multiBellAction = new MultiBellAction(*action);//must be last as it is the base class of the others
		else
			_multiBellAction = NULL;  
	}

	{
		delete _speechAction;
		_speechAction = NULL;

		SpeechResponse* response = dynamic_cast<SpeechResponse*>(simulatorInput._speechAction);
		SpeechItem* item = dynamic_cast<SpeechItem*>(simulatorInput._speechAction);
		SpeechAction* action = dynamic_cast<SpeechAction*>(simulatorInput._speechAction);
		if (response)
			_speechAction = new SpeechResponse(*response);
		else if (item)
			_speechAction = new SpeechItem(*item);
		else if (action)
			_speechAction = new SpeechAction(*action);//must be last as it is the base class of the others
		else
			_speechAction = NULL;  
	}

	{
		delete _serialControlAction;
		_serialControlAction= NULL;

		SerialControlResponse* response = dynamic_cast<SerialControlResponse*>(simulatorInput._serialControlAction);
		SerialControlItem* item = dynamic_cast<SerialControlItem*>(simulatorInput._serialControlAction);
		SerialControlAction* action = dynamic_cast<SerialControlAction*>(simulatorInput._serialControlAction);
		if (response)
			_serialControlAction = new SerialControlResponse(*response);
		else if (item)
			_serialControlAction = new SerialControlItem(*item);
		else if (action)
			_serialControlAction = new SerialControlAction(*action);//must be last as it is the base class of the others
		else
			_serialControlAction = NULL;  
	}

	return *this;
}	
		   
bool SimulatorInput::operator ==(const SimulatorInput & simulatorInput)	const
{
	if (_type != simulatorInput._type)
		return false;

	switch (_type)
	{
	case sit_keyboard:
		if (simulatorInput._keyboardAction== NULL || 	_keyboardAction == NULL)
			return false;
		else
			return (*_keyboardAction == *simulatorInput._keyboardAction);
	case sit_speechRecognition:
		if (simulatorInput._speechAction== NULL || 	_speechAction == NULL)
			return false;
		else
			return (*_speechAction == *simulatorInput._speechAction);
	case sit_multiBellInterface:
		if (simulatorInput._multiBellAction == NULL || 	_multiBellAction == NULL)
			return false;
		else
			return (*_multiBellAction == *simulatorInput._multiBellAction);
	case sit_serial:
		if (simulatorInput._serialControlAction == NULL || 	_serialControlAction == NULL)
			return false;
		else
			return (*_serialControlAction == *simulatorInput._serialControlAction);
	default:
		ASSERT(FALSE);
	}

	return false;

}

void SimulatorInput::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  
		ar << _type;

		if (_keyboardAction == NULL)
			ar << FALSE;
		else
		{
			//we only serialise this type of item.
			KeyboardItem* item = dynamic_cast<KeyboardItem*>(_keyboardAction);
			if (item == NULL)
			{
				ASSERT(FALSE);
				ar << FALSE;
			}
			else
			{   
				ar << TRUE;
				item->Serialize(ar);
			}
		} 

		if (_multiBellAction == NULL)
			ar << FALSE;
		else
		{
			//we only serialise this type of item.
			MultiBellItem* item = dynamic_cast<MultiBellItem*>(_multiBellAction);
			if (item == NULL)
			{
				ASSERT(FALSE);
				ar << FALSE;
			}
			else
			{   
				ar << TRUE;
				item->Serialize(ar);
			}
		}    

		if (_speechAction== NULL)
			ar << FALSE;
		else
		{
			//we only serialise this type of item.
			SpeechItem* item = dynamic_cast<SpeechItem*>(_speechAction);
			if (item == NULL)
			{
				ASSERT(FALSE);
				ar << FALSE;
			}
			else
			{   
				ar << TRUE;
				item->Serialize(ar);
			}
		}    

		if (_serialControlAction == NULL)
			ar << FALSE;
		else
		{
			//we only serialise this type of item.
			SerialControlItem* item = dynamic_cast<SerialControlItem*>(_serialControlAction);
			if (item == NULL)
			{
				ASSERT(FALSE);
				ar << FALSE;
			}
			else
			{   
				ar << TRUE;
				item->Serialize(ar);
			}
		}    
	}
	else
	{
		ar >> version;  
		int temp;
		ar >> temp;
		_type = static_cast<SimulatorInputType>(temp);		

		delete _keyboardAction;
		_keyboardAction = NULL;
		BOOL haveAction;
		ar >> haveAction;
		if (haveAction)
		{
			KeyboardItem* item = new KeyboardItem;
			item->Serialize(ar);
			_keyboardAction = item;
		}

		delete _multiBellAction;
		_multiBellAction = NULL;
		ar >> haveAction;
		if (haveAction)
		{
			MultiBellItem* item = new MultiBellItem;
			item->Serialize(ar);
			_multiBellAction = item;
		}
		
		delete _speechAction;
		_speechAction = NULL;
		ar >> haveAction;
		if (haveAction)
		{
			SpeechItem* item = new SpeechItem;
			item->Serialize(ar);
			_speechAction = item;
		}

		delete _serialControlAction;
		_serialControlAction = NULL;
		ar >> haveAction;
		if (haveAction)
		{
			SerialControlItem* item = new SerialControlItem;
			item->Serialize(ar);
			_serialControlAction = item;
		}

	}	
}


void SimulatorInput::setSimulatorInputType(SimulatorInputType type)
{
	_type = type;
}

SimulatorInputType SimulatorInput::getSimulatorInputType() const
{
	return _type;
}

void SimulatorInput::setKeyboard(KeyboardAction* action)
{
	ASSERT(action);
	if (action)
	{
		_type = sit_keyboard;
		
		delete _keyboardAction;
		_keyboardAction = NULL;
		_keyboardAction = action;
	}			
}

bool SimulatorInput::isKeyboard() const
{
	return (_type == sit_keyboard);
}

void SimulatorInput::setSpeech(SpeechAction* action)
{	
	ASSERT(action);
	if (action)
	{
		_type = sit_speechRecognition;
		
		delete _speechAction;
		_speechAction = NULL;
		_speechAction = action;
	}
}

SpeechAction* SimulatorInput::getSpeech(void)  const
{
	return _speechAction;
}

KeyboardAction* SimulatorInput::getKeyboard() const
{
	return _keyboardAction;
}

bool SimulatorInput::isSpeech() const
{
	return (_type == sit_speechRecognition);
}

void SimulatorInput::setMultiBellInterface(MultiBellAction* action)
{	
	ASSERT(action);
	if (action)
	{
		_type = sit_multiBellInterface;
		
		delete _multiBellAction;
		_multiBellAction = NULL;
		_multiBellAction = action;
	}
}

MultiBellAction* SimulatorInput::getMultiBellInterface(void) const
{
	return _multiBellAction;
}

bool SimulatorInput::isMultiBellInterface() const
{
	return (_type == sit_multiBellInterface);
}

//we take controle of this object
void SimulatorInput::setSerialControl(SerialControlAction* action)
{	
	ASSERT(action);
	if (action)
	{
		_type = sit_serial;
		
		delete _serialControlAction;
		_serialControlAction = NULL;
		_serialControlAction = action;
	}
}

bool SimulatorInput::isSerialControl() const
{
	return (_type == sit_serial);
}

SerialControlAction* SimulatorInput::getSerialControl(void) const
{
	return _serialControlAction;
}


CString SimulatorInput::getDescription(SimulatorInputDescriptionType version)   const
{                
	CString str;
	switch (_type)
	{
	case sit_keyboard:
		if (!isValid())
			str = "Choose Key Press";
		else
		{
			if (getKeyboard() != NULL)
			{
				if (version == sidt_short)
					str.Format("%s", getKeyboard()->getDescription());
				else if (version == sidt_long)
					str.Format("Key %s", getKeyboard()->getDescription());
				else if (version == sidt_long_base)
					str.Format("Key %s", getKeyboard()->KeyboardAction::getDescription());
			}
			else
			{
				ASSERT(FALSE);
				str = "'Error'";
			}
		}
		break;
	case sit_speechRecognition:
		if (!isValid())
			str = "Choose Microphone Word";
		else
		{
			if (getSpeech() != NULL)
			{
				if (version == sidt_short)
    				str.Format("%s", getSpeech()->getDescription());
				else if (version == sidt_long)
    				str.Format("Word %s", getSpeech()->getDescription());
				else if (version == sidt_long_base)
					str.Format("Word %s", getSpeech()->SpeechAction::getDescription());
			}
			else
			{
				ASSERT(FALSE);
				str = "'Error'";
			}
		}
		break;
	case sit_multiBellInterface:
		if (!isValid())
			str = "Choose Channel";
		else
		{
			if (getMultiBellInterface() != NULL)
			{
				if (version == sidt_short)
					str.Format("%s", getMultiBellInterface()->getDescription());
				else if (version == sidt_long)
					str.Format("Channel %s", getMultiBellInterface()->getDescription());
				else if (version == sidt_long_base)
					str.Format("Channel %s", getMultiBellInterface()->MultiBellAction::getDescription());
			}
			else
			{
				ASSERT(FALSE);
				str = "'Error'";
			}
		}
		break;
	case sit_serial:
		if (!isValid())
			str = "Choose Port";
		else
		{
			if (getSerialControl() != NULL)
			{
				if (version == sidt_short)
					str.Format("%s", getSerialControl()->getDescription());
				else if (version == sidt_long)
					str.Format("%s",getSerialControl()->getDescription());
				else if (version == sidt_long_base)
					str.Format("%s",getSerialControl()->SerialControlAction::getDescription());
			}
			else
			{
				ASSERT(FALSE);
				str = "'Error'";
			}
		}
		break;

	default:
		break;			
	}	

	return str;
}

bool SimulatorInput::isInput() const
{
	return (_type != sit_none);
}

bool SimulatorInput::isValid() const
{
	switch (_type)
	{
	case sit_none:
		return true;
	case sit_keyboard:
		if (getKeyboard() == NULL)
			return FALSE;
		else
			return getKeyboard()->isValid();
		break;
	case sit_speechRecognition:
		if (getSpeech() == NULL)
			return FALSE;
		else
			return getSpeech()->isValid();
		break;
	case sit_multiBellInterface:
		if (getMultiBellInterface() == NULL)
			return FALSE;
		else
			return getMultiBellInterface()->isValid();
		break;
	case sit_serial:
		if (getSerialControl() == NULL)
			return FALSE;
		else
			return getSerialControl()->isValid();
		break;
	default:
		break;			
	}	

	return false;	
}

bool SimulatorInput::isValidResponse() const
{
	switch (_type)
	{
	case sit_keyboard: 
		{
			KeyboardResponse* response = dynamic_cast<KeyboardResponse*>(_keyboardAction);
			ASSERT(response);
			if (response)
				return response->isValid();
		}
		return false;
	case sit_speechRecognition:
		{
			SpeechResponse* response = dynamic_cast<SpeechResponse*>(_speechAction);
			ASSERT(response);
			if (response)
				return response->isValid();
		}
		return false;
	case sit_multiBellInterface:
		{
			MultiBellResponse* response = dynamic_cast<MultiBellResponse*>(_multiBellAction);
			ASSERT(response);
			if (response)
				return response->isValid();
		}
		return false;
	case sit_serial:
		{
			SerialControlResponse* response = dynamic_cast<SerialControlResponse*>(_serialControlAction);
			ASSERT(response);
			if (response)
				return response->isValid();
		}
		return false;
	default:
		ASSERT(FALSE);
		return false;
		break;			
	}			
}

void SimulatorInput::edit(CWnd* wnd)
{
	switch (_type)
	{
	case sit_keyboard: 
		{
			KeyboardItem* item = dynamic_cast<KeyboardItem*>(_keyboardAction);
			if (!item)
			{
				_keyboardAction = new KeyboardItem();	
				item = dynamic_cast<KeyboardItem*>(_keyboardAction);
			}

			KeyboardItem* returnItem = new KeyboardItem(keyboardManager()->captureKey(wnd, *item));
			setKeyboard(returnItem);
		}
		break;
	case sit_speechRecognition:
		{
			SpeechItem* item = dynamic_cast<SpeechItem*>(_speechAction);
			if (!item)
			{
				_speechAction = new SpeechItem();	
				item = dynamic_cast<SpeechItem*>(_speechAction);
				item->setWord("");
			}

			SpeechItem* returnItem = new SpeechItem(speechManager()->captureSpeech(wnd, *item));
			setSpeech(returnItem);
		}
		break;
	case sit_multiBellInterface:
		{
			MultiBellItem* item = dynamic_cast<MultiBellItem*>(_multiBellAction);
			if (!item)
			{
				_multiBellAction = new MultiBellItem();	
				item = dynamic_cast<MultiBellItem*>(_multiBellAction);
				item->setChannel(0);
			}

			MultiBellItem* returnItem = new MultiBellItem(multiBellInterfaceManager()->captureMultiBellInterface(wnd, *item));
			setMultiBellInterface(returnItem);
		}
		break;
	case sit_serial:
		{
			SerialControlItem* item = dynamic_cast<SerialControlItem*>(_serialControlAction);
			if (!item)
			{
				_serialControlAction = new SerialControlItem();	
				item = dynamic_cast<SerialControlItem*>(_serialControlAction);
			}

			SerialControlItem* returnItem = new SerialControlItem(serialControlManager()->captureSerialControl(wnd, *item));
			setSerialControl(returnItem);
		}
	break;
	default:
		break;			
	}	
}


CString SimulatorInput::getTypeText()const 
{
	switch (_type)
	{
	case sit_keyboard:
		return "Key";
	case sit_multiBellInterface:
		return "MBI";
	case sit_serial:
		return "Serial";
	case sit_speechRecognition:
		return "Speech";
	default:
		ASSERT(FALSE);
		return "";    
	}
}
