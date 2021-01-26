#include "StdAfx.h"
#include ".\playrequestsimulator.h"
#include "MethodSimulator.h"

PlayRequestSimulator::PlayRequestSimulator(MethodSimulator &method) :
PlayRequest(method), 
_originalMethod(method)
{
}

PlayRequestSimulator::~PlayRequestSimulator(void)
{
}

bool PlayRequestSimulator::getAnotherRow(Stroke stroke)
{
	Row* row = _originalMethod.getAnotherRow(stroke);

	if (row)
	{
		Add(new RowExtra(*row, 0));//we do not use lead length on simulator - OK to be 0
		return true;
	}

	return false;
}

int PlayRequestSimulator::getStartChangeRepeat(Tower* /*tower*/) const
{
	return 0;
}

int PlayRequestSimulator::getEndChangeRepeat(Tower* /*tower*/) const
{
	return 0;
}