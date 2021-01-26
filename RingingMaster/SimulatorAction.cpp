// SimulatorAction.cpp: implementation of the SimulatorAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "SimulatorAction.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimulatorAction::SimulatorAction() 
{

}

SimulatorAction::~SimulatorAction()
{
	reset();
}

SimulatorAction& SimulatorAction::operator =(const SimulatorAction & simulatorAction)
{
	reset();
	
	for (int i=0;i<simulatorAction._inputs.GetSize();i++)
	{
		SimulatorInput* action = new SimulatorInput;
		*action = *simulatorAction._inputs[i];
		_inputs.Add(action);
	}

	return *this;
}

void SimulatorAction::clean(int nonInputCount)
{
	for (int i=0;i<_inputs.GetSize();i++)
	{
		if (!_inputs.GetAt(i)->isInput())
		{
			if (nonInputCount > 0)
			{
				nonInputCount--;
			}
			else
			{
				delete _inputs.GetAt(i);
				_inputs.RemoveAt(i--);
			}
		}
	}
}
  
void SimulatorAction::reset()
{
	for (int i=0;i<_inputs.GetSize();i++)
		delete _inputs.GetAt(i);

	_inputs.RemoveAll();	
}



void SimulatorAction::Serialize(CArchive &ar)
{
	int version = 1;
	if (ar.IsStoring())
	{
		ar << version; 
		ar << _inputs.GetSize();

		for (int i=0;i<_inputs.GetSize();i++)
			_inputs.GetAt(i)->Serialize(ar);

	}
	else
	{
		reset();
		ar >> version;  
		int size;
		ar >> size;
		for (int i=0;i<size;i++)
		{
			SimulatorInput* action = new SimulatorInput();
			action->Serialize(ar);
			_inputs.Add(action);
		}
	}
}

int SimulatorAction::addInput(SimulatorInput* input)
{
	_inputs.Add(input);

	return _inputs.GetSize();
}

int SimulatorAction::getInputCount() const 
{
	return _inputs.GetSize();
}

SimulatorInput* SimulatorAction::getInput(int index) const 
{
	ASSERT(index >=0 && index < _inputs.GetSize());
	
	if (index >=0 && index < _inputs.GetSize())
		return _inputs.GetAt(index);

	return NULL;
}

int SimulatorAction::getNoInputCount()
{
	int count = 0;
	for (int i=0;i<_inputs.GetSize();i++)
	{
		if (!_inputs.GetAt(i)->isInput())
			count++;
	}

	return count;
}       

int SimulatorAction::isValid()
{
	for (int i=0;i<_inputs.GetSize();i++)
	{
		if (!_inputs.GetAt(i)->isValid())
			return false;
	}

	return true;
}
