// Undo.cpp: implementation of the Undo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Undo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor
Undo::Undo(int undoLevels) : 
_undoLevels(undoLevels),
_chkPt(0)
{

} 
// Destructor
Undo::~Undo() 
{
	// Clear undo list
	POSITION pos = _undoList.GetHeadPosition(); 
	CMemFile  *nextFile = NULL;
	while(pos) 
	{
		nextFile = (CMemFile *) _undoList.GetNext(pos);
		delete nextFile;
		nextFile = NULL;
	}
	_undoList.RemoveAll();	

	// Clear redo list
	ClearRedoList();
}

// Remove contents of the redo list
void Undo::ClearRedoList()
{
	// Clear redo list
	POSITION pos = _redoList.GetHeadPosition(); 
	CMemFile* nextFile = NULL;
	while(pos) 
	{
		nextFile = (CMemFile *) _redoList.GetNext(pos);
		delete nextFile;
		nextFile = NULL;
	}
	_redoList.RemoveAll();	
}


// Checks undo availability, may be used to enable menus
BOOL Undo::CanUndo() 
{
	return (_undoList.GetCount() > 1);
}

// Checks redo availability, may be used to enable menus
BOOL Undo::CanRedo() 
{
	return (_redoList.GetCount() > 0);
}

// Adds state to the beginning of undo list		
void Undo::AddUndo(CMemFile* file) 
{
	// Remove old state if there are more than max allowed
	if (_undoList.GetCount() > _undoLevels) 
	{
		CMemFile* pFile = (CMemFile *) _undoList.RemoveTail();
		delete pFile;
		pFile = NULL;
	}
	// Add new state to head of undo list
	_undoList.AddHead(file);
}

// Saves current object into CMemFile instance
void Undo::Store(CMemFile* file) 
{
	file->SeekToBegin();
	CArchive ar(file, CArchive::store);
	Serialize(ar); 
	ar.Close();
}

// Loads CMemfile instance to current object
void Undo::Load(CMemFile* file) 
{
	DeleteContents(); 
	file->SeekToBegin();
	CArchive ar(file, CArchive::load);
	Serialize(ar); 
	ar.Close();
}

// Save current object state to Undo list
void Undo::CheckPoint(CString name) 
{
	if (_chkPt <= 0) 
	{
		CMemFile* file = new CMemFile();
		if (!name.IsEmpty()) file->SetFilePath(name);
		Store(file);
		AddUndo(file);
		ClearRedoList();
	}
}

void Undo::EnableCheckPoint()
{
	if (_chkPt > 0) 
	{
		_chkPt--;
	}
}

void Undo::DisableCheckPoint()
{
	_chkPt++;
}

// Place CMemFile instnace on Redo list
void Undo::AddRedo(CMemFile *file) 
{
	// Move state to head of redo list
	_redoList.AddHead(file);
}

// Perform an Undo command
void Undo::DoUndo(int count) 
{
	CWaitCursor wait;
	
	if (CanUndo()&& (count > 0)) 
	{
		// Remember that the head of the undo list
		// is the current state. So we just move that
		// to the Redo list and load then previous state.
		CMemFile *pFile = NULL;
		while (CanUndo() && count >0)
		{
			count--;
			pFile= (CMemFile *) _undoList.GetHead();
			_undoList.RemoveHead();
			AddRedo(pFile);
		}
		pFile = (CMemFile *)_undoList.GetHead();
		Load(pFile);
	}
}

//Perform a Redo Command
void Undo::DoRedo(int count) 
{
	CWaitCursor wait;
	if (CanRedo()&& (count > 0 )) 
	{
		CMemFile *pFile = NULL;
		while (CanRedo() && count >0)
		{
			count--;
			pFile = (CMemFile *) _redoList.GetHead() ;
			_redoList.RemoveHead();
			AddUndo(pFile);
		}
		Load(pFile);
	}
}

CString Undo::UndoWhat()
{
	ASSERT(CanUndo());
	return static_cast<CMemFile *>(_undoList.GetHead())->GetFilePath();
}

CString Undo::RedoWhat()
{
	ASSERT(CanRedo());
	return static_cast<CMemFile *>(_redoList.GetHead())->GetFilePath();
}

void Undo::getUndoWhatList(CStringList& lstActions)
{
	// Clear undo list
	POSITION pos = _undoList.GetHeadPosition(); 
	CMemFile  *nextFile = NULL;
	while(pos) 
	{
		nextFile = (CMemFile *) _undoList.GetNext(pos);

		if (nextFile) 
			lstActions.AddTail(nextFile->GetFilePath());
	}

	//remove the last, as it is the initial state
	if (lstActions.GetCount() > 0)
		lstActions.RemoveTail();
}

void Undo::getRedoWhatList(CStringList& lstActions)
{
	// Clear undo list
	POSITION pos = _redoList.GetHeadPosition(); 
	CMemFile  *nextFile = NULL;
	while(pos) 
	{
		nextFile = (CMemFile *) _redoList.GetNext(pos);

		if (nextFile) 
			lstActions.AddTail (nextFile->GetFilePath());
	}
}
