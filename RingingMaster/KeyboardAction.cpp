#include "StdAfx.h"
#include "KeyboardManager.h"
#include "KeyboardAction.h"

KeyboardAction::KeyboardAction(DWORD scancode) :
_scancode(scancode)
{
}

KeyboardAction::KeyboardAction() :
_scancode(KeyboardManager::_NoKey)
{
}

KeyboardAction::~KeyboardAction(void)
{
}

CString KeyboardAction::getDescription() const
{
	return KeyboardManager::scan2String(_scancode);
}

int KeyboardAction::getScancode()   const
{
	return _scancode;
}

void KeyboardAction::setScancode(DWORD scancode)
{
	_scancode = scancode;
}

bool KeyboardAction::operator ==(const KeyboardAction & multiBellAction) const
{
	return (_scancode == multiBellAction._scancode);
}

void KeyboardAction::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version;  

		ar << _scancode;
	}
	else
	{
		ar >> version;  

		ar >> _scancode;
	}
}

bool KeyboardAction::isValid() const
{
	return (_scancode > DIK_ESCAPE && _scancode <= DIK_MEDIASELECT);
}