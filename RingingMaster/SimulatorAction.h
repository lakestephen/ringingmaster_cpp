// SimulatorAction.h: interface for the SimulatorAction class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "SimulatorInput.h" 

class SimulatorAction  
{
public:
	void reset();
	void clean(int nonInputCount = 0);
	int getNoInputCount();
	void Serialize(CArchive &ar);
	SimulatorAction();
	virtual ~SimulatorAction();

	int isValid();


	SimulatorAction& operator =(const SimulatorAction & simulatorAction);

protected:

	SimulatorInputs _inputs;// list of inputs that triggers this call.

public:
	int addInput(SimulatorInput* action);
	SimulatorInput* getInput(int index) const;
	int getInputCount() const;
};


typedef CArray<SimulatorAction*, SimulatorAction*> SimulatorActions;
