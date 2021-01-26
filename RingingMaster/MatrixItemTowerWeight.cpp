#include "StdAfx.h"
#include "MatrixItemTowerWeight.h"
#include "RecordTower.h"
#include "resource.h"
#include "WeightSelectDlg.h"
#include "MatrixManagerColumnFactory.h"

AUTO_REG(MatrixItemTowerWeight, MatrixManagerColumnFactory::getInstance()->addColumnToFactory( new MatrixItemTowerWeight););

MatrixItemTowerWeight::MatrixItemTowerWeight()
{
}

MatrixItemTowerWeight::~MatrixItemTowerWeight(void)
{
}

MatrixItemBase* MatrixItemTowerWeight::clone() const 
{
	return new MatrixItemTowerWeight(*this);
} 

MatrixItemToken MatrixItemTowerWeight::getToken() const
{
	return mit_tower_weight;
}

CString MatrixItemTowerWeight::getItemName() const
{
	return "Weight";
}


bool MatrixItemTowerWeight::editFilterCondition(CWnd* parent)
{
	if (!_filterEditedAndReadyToUse)
	{
		_doubleSelectItem._double1 = 600.0;
		_doubleSelectItem._double2 = 1000.0;
	}

	WeightSelectDlg dlg(parent, _doubleSelectItem,  "Filter on " + getName());
  
	bool ret = false;
	if (dlg.DoModal() == IDOK)
	{
		ret = true;
		_filterEditedAndReadyToUse = TRUE;
	}
					  
	return ret;
}      


double MatrixItemTowerWeight::getDouble(SaveableObject* translatedEntity)
{
	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	ASSERT(recordTower);
	if (recordTower)
	{
		return (double)recordTower->getWeight();
	}

	return 0;
}

CString MatrixItemTowerWeight::getTextFromDouble(double doubleVal, SaveableObject* translatedEntity)
{
	if (doubleVal == 0.0)
		return "";

	

	RecordTower* recordTower = dynamic_cast<RecordTower*>(translatedEntity);
	if (recordTower)
	{
		return GlobalFunctions::getWeightStringWithApprox(doubleVal, recordTower->getApprox());
	}
	else
	{
		return GlobalFunctions::getWeightString(doubleVal, false);
	}


	return "";
}

 
int MatrixItemTowerWeight::getDefaultWidth()
{
	return 80;
}
