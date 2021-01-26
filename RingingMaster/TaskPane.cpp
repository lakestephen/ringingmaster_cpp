// TaskPane.cpp : implementation of the TaskPane class
//

#include "stdafx.h"
#include "ringingmaster.h"
#include "TaskPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TaskPane

BEGIN_MESSAGE_MAP(TaskPane, CBCGPTasksPane)
	//{{AFX_MSG_MAP(TaskPane)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TaskPane construction/destruction

TaskPane::TaskPane() :
_nDocumentsGroup(-1)
{
}

TaskPane::~TaskPane()
{
}

/////////////////////////////////////////////////////////////////////////////
// TaskPane message handlers

int TaskPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;
						 
	SetCaption (_T("Tasks Pane"));
	SetIconsList (IDB_TASKS, 16);

	EnableNavigationToolbar (TRUE);

	EnableWrapLabels (TRUE);
	EnableOffsetCustomControls (FALSE);

	
	int nPage1Gr1 = AddGroup (_T("New Document"), FALSE, TRUE);
	AddTask (nPage1Gr1, _T("Create New Touch"), 1, ID_NEWTOUCH);
	AddTask (nPage1Gr1, _T("Create New Crib"), 2, ID_NEWCRIB);


	_nDocumentsGroup = AddGroup (_T("Open a document"));
	UpdateMRUFilesList ();
	AddSeparator(_nDocumentsGroup);
	AddTask (_nDocumentsGroup, _T("More Documents..."), 0, ID_FILE_OPEN);

	int nPage1Gr3 = AddGroup (_T("Tasks"));
	AddTask (nPage1Gr3, _T("Help Topics"), 10, ID_HELP_FINDER);
	AddSeparator(nPage1Gr3);
	AddTask (nPage1Gr3, _T("Tower Database"), 7, ID_TOWER_DATABASE);
	AddTask (nPage1Gr3, _T("Grabs"), 8, ID_GRABS);
	AddSeparator(nPage1Gr3);
	AddTask (nPage1Gr3, _T("Options"), 3, ID_TOOLS_OPTIONS);
	AddTask (nPage1Gr3, _T("Method Database"), 4, ID_TOOLS_DATABASEMAINTAINANCE);
	AddTask (nPage1Gr3, _T("Create Tower Profile"), 5, ID_NEW_TOWER_PROFILE);

	
				
	

	SetGroupTextColor(nPage1Gr1, RGB(49,106,197));
	SetGroupTextColor(_nDocumentsGroup, RGB(49,106,197));
	SetGroupTextColor(nPage1Gr3, RGB(49,106,197));
						  
	return 0;
}

void TaskPane::UpdateMRUFilesList ()
{
	AddMRUFilesList (_nDocumentsGroup, DOCS_ON_TASK_PANE);

	//update the icons on the MRU list

	POSITION pos = m_lstTaskGroups.FindIndex (_nDocumentsGroup);
	if (pos == NULL)
	{
		ASSERT (FALSE);
		return ;
	}

	CBCGPTasksGroup* pGroup = (CBCGPTasksGroup*) m_lstTaskGroups.GetAt (pos);
	ASSERT_VALID (pGroup);

	for (pos = pGroup->m_lstTasks.GetHeadPosition (); pos != NULL;)
	{
		CBCGPTask* pTask = (CBCGPTask*) pGroup->m_lstTasks.GetNext (pos);
		ASSERT_VALID (pTask);

		if (pTask->m_uiCommandID >= ID_FILE_MRU_FILE1 &&
			pTask->m_uiCommandID <= ID_FILE_MRU_FILE16)
		{
			CString name = pTask->m_strName;
			CString nameLower = name.MakeLower();
			if (nameLower.Find(".crb") != -1)
				pTask->m_nIcon = 1;
			else if (nameLower.Find(".tch") != -1)
				pTask->m_nIcon = 2;
			else if (nameLower.Find(".strk") != -1)
				pTask->m_nIcon = 9;		  
			else
			{
				if (app()->GetImageTypeFromPath(nameLower) != CXIMAGE_FORMAT_UNKNOWN)
				{
					pTask->m_nIcon = 6;
				}
			}
		}
	}
		
	AdjustScroll ();
	ReposTasks ();

	RedrawWindow ();
}

void TaskPane::UpdateToolbar ()
{
	m_wndToolBar.RedrawWindow ();
}




