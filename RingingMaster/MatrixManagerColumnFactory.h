#pragma once


class MatrixItemBase;
enum MatrixItemToken;

class MatrixManagerColumnFactory
{
public:
	static MatrixManagerColumnFactory* getInstance();

	//column factory
	void addColumnToFactory(MatrixItemBase* matrixItemBase);
	MatrixItemBase* createColumnUsingFactory(MatrixItemToken token);

	static void Destroy();
	
protected:
	MatrixManagerColumnFactory(void);
	~MatrixManagerColumnFactory(void);
	static MatrixManagerColumnFactory* _TheInstance;
	void deleteColumnFactory();

	CMap<MatrixItemToken, MatrixItemToken, MatrixItemBase*, MatrixItemBase*> _columnFactoryMap;
};
