// StrikeDoc.h : interface of the StrikeDoc class
//

#pragma once
#include "StrikingData.h"
#include "PlayManagerEventListener.h"

class StrikeDoc : public CDocument, public PlayManagerEventListener
{
protected: // create from serialization only
	StrikeDoc();
	DECLARE_DYNCREATE(StrikeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	void setInitialData(const StrikingData& strikingData);
	const StrikingData& getStrikingData();

	void playManager_notifyRowPositionChangedFromSeperateThread(const RowPositionChangedPackage& rowPositionChangedPackage);
	void doGoto(int row,  bool quiet);
	int getSelectedRow() {return _selectedRow;}

// Implementation
public:
	virtual ~StrikeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

	void setUpdatePoint(CString name, bool setModifyed = true);

	StrikingData _strikingData;

	int _selectedRow;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
};


