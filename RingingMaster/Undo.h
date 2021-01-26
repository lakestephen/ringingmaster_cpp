// Undo.h: interface for the Undo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDO_H__57D8B4DD_3AD8_11D5_B5C5_009027BB3286__INCLUDED_)
#define AFX_UNDO_H__57D8B4DD_3AD8_11D5_B5C5_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class Undo  
{
public:

	private:
	CObList	_undoList;		// Stores undo states
	CObList	_redoList;		// Stores redo states
	int	_undoLevels;	// Requested Undolevels 
	int	_chkPt;

	void AddUndo(CMemFile*);
	void AddRedo(CMemFile *pFile); 
	void Load(CMemFile*);
	void Store(CMemFile*);
	void ClearRedoList();
	
public:

	virtual void bulkUndo(int undoCount) = 0;
	virtual void bulkRedo(int redoCount) = 0;

	void getUndoWhatList(CStringList& lstActions);
	void getRedoWhatList(CStringList& lstActions);
	CString UndoWhat();
	CString RedoWhat();

	// Here are the hooks into the CDocument class
	virtual void Serialize(CArchive& ar) = 0;
	virtual void DeleteContents() = 0;

	// User accessable functions
	Undo(int undoLevels = 4);	// Constructor
	~Undo();			// Destructor
	BOOL CanUndo();		// Returns TRUE if can Undo
	BOOL CanRedo();		// Returns TRUE if can Redo
	void DoUndo(int count);		// Restore next Undo state
	void DoRedo(int count);		// Restore next Redo state				
	void CheckPoint(CString name);	// Save current state 
	void EnableCheckPoint();
	void DisableCheckPoint();	
};

#endif // !defined(AFX_UNDO_H__57D8B4DD_3AD8_11D5_B5C5_009027BB3286__INCLUDED_)
