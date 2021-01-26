#pragma once

enum MatrixEntityToken;

class MatrixManagerEventListener  
{
public:
	MatrixManagerEventListener() {};
	~MatrixManagerEventListener() {};
	virtual void matrixManager_updateFilterList() {};
	virtual void matrixManager_entityAdded(MatrixEntityToken /*token*/, long /*id*/) {};
	virtual void matrixManager_entityRemoved(MatrixEntityToken /*token*/, long /*id*/) {};
	virtual void matrixManager_refresh() {};
};

typedef CArray<MatrixManagerEventListener*, MatrixManagerEventListener*> MatrixManagerEventListeners;

