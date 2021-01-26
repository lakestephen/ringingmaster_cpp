#include "StdAfx.h"
#include "MatrixManagerColumnFactory.h"
#include "MatrixConstants.h"
#include "MatrixItemBase.h"

MatrixManagerColumnFactory* MatrixManagerColumnFactory::_TheInstance = NULL;

MatrixManagerColumnFactory* MatrixManagerColumnFactory::getInstance()
{
	if (_TheInstance == NULL)
		_TheInstance = new MatrixManagerColumnFactory();
	return _TheInstance;
}

void MatrixManagerColumnFactory::Destroy()
{
	MatrixManagerColumnFactory* theInstance = _TheInstance;
	_TheInstance = NULL;
	delete theInstance;
}

MatrixManagerColumnFactory::MatrixManagerColumnFactory(void)
{
}

MatrixManagerColumnFactory::~MatrixManagerColumnFactory(void)
{
	deleteColumnFactory();
}


MatrixItemBase* MatrixManagerColumnFactory::createColumnUsingFactory(MatrixItemToken token)
{
	MatrixItemBase* matrixItemBase = NULL;

	if (_columnFactoryMap.Lookup(token, matrixItemBase) && matrixItemBase!= NULL)
		return matrixItemBase->clone();

	ASSERT(FALSE);

	return NULL;       		
}

void MatrixManagerColumnFactory::addColumnToFactory(MatrixItemBase* matrixItemBase)
{
	ASSERT(matrixItemBase);

	if (matrixItemBase)
	{
		MatrixItemToken token = matrixItemBase->getToken();

#ifdef _DEBUG
		//have we already got one?
		MatrixItemBase* temp;
		ASSERT(!_columnFactoryMap.Lookup(token, temp));
#endif

		_columnFactoryMap.SetAt(token, matrixItemBase);
	}
}
						
void MatrixManagerColumnFactory::deleteColumnFactory()
{

	POSITION pos = _columnFactoryMap.GetStartPosition();
	MatrixItemToken  nKey;
	MatrixItemBase* nValue;
	while (pos != NULL)
	{
		_columnFactoryMap.GetNextAssoc(pos, nKey, nValue );

		if (nValue)
			delete nValue;
		
		VERIFY(_columnFactoryMap.RemoveKey( nKey ));
	}   
}