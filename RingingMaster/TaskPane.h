// TaskPane.h : interface of the TaskPane class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once

#define DOCS_ON_TASK_PANE 6

class TaskPane : public CBCGPTasksPane
{
	friend class CTasksPaneView;

// Construction
public:
	TaskPane();
	

protected:
	int	_nDocumentsGroup;

// Operations
public:
	void UpdateMRUFilesList ();
	void UpdateToolbar ();

// Overrides
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TaskPane)
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~TaskPane();

	virtual BOOL CanFloat () const {return FALSE;}

	// Generated message map functions
protected:
	//{{AFX_MSG(TaskPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

