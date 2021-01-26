// RecordAnalysisTowerTower.cpp: implementation of the RecordAnalysisTowerTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "RecordAnalysisTowerTower.h"
#include "RecordManager.h"
#include "MapManager.h"
#include "RecordTower.h"
#include "PropItem.h"
#include "WeightDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RecordAnalysisTowerTower::RecordAnalysisTowerTower()  : 
_tower(NULL)
{

}

RecordAnalysisTowerTower::~RecordAnalysisTowerTower()
{

}


BEGIN_MESSAGE_MAP(RecordAnalysisTowerTower, PropList)
	//{{AFX_MSG_MAP(RecordAnalysisTowerTower)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_POPUP_PRINT_ANALYSIS, OnPopupPrintAnalysis)
	ON_UPDATE_COMMAND_UI(ID_POPUP_PRINT_ANALYSIS, OnUpdatePopupPrintAnalysis)
	ON_COMMAND(ID_LAUNCH_WEB_PAGE, OnLaunchWebPage)
	ON_UPDATE_COMMAND_UI(ID_LAUNCH_WEB_PAGE, OnUpdateLaunchWebPage)
	ON_COMMAND(ID_POPUP_SHOWMAP, OnShowMap)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWMAP, OnUpdateShowMap)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL RecordAnalysisTowerTower::Create(CWnd* parent, CRect rect)
{
	BOOL result = PropList::Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
								LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_HASSTRINGS, 
								rect,
								parent,
								AAA_ANALYSIS_TOWER_TOWER);	
	_devider[0] = 140;	
	
	if (result)
		SetFont(&app()->_fontStd);

	return  result;
}

void RecordAnalysisTowerTower::doUpdate(RecordTower* tower)
{	
	if (!::IsWindow(m_hWnd))
		return;

	_tower = tower;

	int topIndex = GetTopIndex();

	deleteAllPropItems();
	
	if (_tower == NULL)
		return;

	/* todo 	
	const AvailableFields& selected = recordManager()->getSelectedFields(vt_tower); //these MUST be tower as they are displaying tower tables.
	const AvailableFields& available = recordManager()->getAvailableFields(vt_tower);
	AvailableFields others;

	for (int i=0;i<available.GetSize();i++)
	{
		bool needToAdd = true;
		for (int j=0;j<selected.GetSize();j++)
		{
			if (selected.GetAt(j) == available.GetAt(i))
			{
				needToAdd = false;
				break;
			}
		}

		if (needToAdd)
			others.Add(available.GetAt(i));
	}

	//sort the others #
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<others.GetUpperBound();i++)
		{
			if (recordManager()->getAvailableFieldsName(others[i]) > recordManager()->getAvailableFieldsName(others[i+1]))
			{
				changed = true;
				AvailableField temp    = others.GetAt(i);
				others.SetAt(i,others.GetAt(i+1));
				others.SetAt(i+1, temp);
			}
		}
	}



	for (int i=0;i<selected.GetSize();i++)
		addLine(selected.GetAt(i), tower);    

	for (int i=0;i<others.GetSize();i++)
		addLine(others.GetAt(i), tower, RGB(240,240,240));    

	SetTopIndex(topIndex);
*/
	Invalidate();
}


void RecordAnalysisTowerTower::addLine(AvailableField field, RecordTower* tower, COLORREF color)
{   /* todo
	PropItem* pItem = new PropItem(recordManager()->getAvailableFieldsPropType(field));

	AddPropItem(pItem);

	pItem->_col[0] =  recordManager()->getAvailableFieldsName(field);;
	pItem->_col[1] =  recordManager()->getAvailableFieldsString(field, tower, NULL, true);
	pItem->_list =    recordManager()->getAvailableFieldsPropList(field);
	pItem->_data1 =   field;
	pItem->_data2 =   (long)tower;

	if (color != -1)
		pItem->_backgroundColor = color;

	//set red if modifyed
	if (recordManager()->getAvailableFieldsModifyed(field, tower, NULL))
		pItem->_textColor = RGB(255,100,100);
		*/
}

	   
void RecordAnalysisTowerTower::doKillFocusCmbBox(PropItem* pItem)
{
	doHandleAction(pItem);
}

void RecordAnalysisTowerTower::doKillFocusEditBox(PropItem* pItem)
{	   
	doHandleAction(pItem);
}

void RecordAnalysisTowerTower::doButton(PropItem *pItem)
{
	doHandleAction(pItem);
}

void RecordAnalysisTowerTower::doHandleAction(PropItem* pItem)
{	

	RecordTower* tower = (RecordTower*)pItem->_data2;

	if (!tower)
	{
		ASSERT(FALSE);
		return;
	}

	switch (pItem->_data1) //available field
	{
   /*  todo		//tower fields
	case af_tower_gridMapRef:
		{
			MapGridRef mapGridRef;

			mapGridRef.setGridRef(pItem->_col[1]);

			if (mapGridRef.isValidOSGridRef())
			{
				tower->setMapRef(mapGridRef); 
			}
			else if (mapGridRef.isValidNonOSGridRef())
			{

				if ((tower->getCountry() == c_england)||
					(tower->getCountry() == c_wales)||
					(tower->getCountry() == c_scotland)||
					(tower->getCountry() == c_isle_of_man))
				{

					if (AfxMessageBox("This is not a valid OS grid ref (format NN123123). \nDo you want to set this value?", MB_YESNO) == IDYES)
						tower->setMapRef(mapGridRef); 
				}
				else
				{
					tower->setMapRef(mapGridRef); 
				}

			}
			else
			{
				AfxMessageBox("Please enter a valid os grid ref.");
			}
		}
		break;
	case af_tower_county:		
		tower->setCounty(pItem->_col[1]);
		break;
	case af_tower_name:			
		tower->setName(pItem->_col[1]);
		break;
	case af_tower_dedication:	
		tower->setDedication(pItem->_col[1]);
		break;
	case af_tower_postCode:		
		tower->setPostCode(pItem->_col[1]);
		break;
	case af_tower_groundFloor:	
		tower->setGroundFloor(pItem->_col[1] == "Ground Floor");
		break;
	case af_tower_number:		
		tower->setNumber(atoi(pItem->_col[1]));
		break;
	case af_tower_weight:
		{
			WeightDlg dlg(tower->getWeight(), TRUE, tower->getApprox(), this);

			if (dlg.DoModal() == IDOK)
			{
				tower->setWeight(dlg.getWeightInKG());
				tower->setApprox(dlg._approx);
			}	
		}
		break;
	case af_tower_note:			
		{
			for (int i=n_a_flat;i<=n_g_sharp;i++)
				if (pItem->_col[1] == recordManager()->getNoteAsString((Note)i))
				{
					tower->setNote((Note)i);
					break;
				}
			if (pItem->_col[1] == recordManager()->getNoteAsString(n_none, true))
				tower->setNote(n_none);
		}
		break;
	case af_tower_night:	
		{
			for (int i=now_sun;i<=now_sat_minus;i++)
				if (pItem->_col[1] == recordManager()->getNightAsString((Night)i))
				{
					tower->setNight((Night)i);
					break;
				}
			if (pItem->_col[1] == recordManager()->getNightAsString(now_none, true))
				tower->setNight(now_none);
		}
		break;
	case af_tower_unringable:	
		tower->setUnringable(pItem->_col[1] == "Unringable");
		break;
	case af_tower_webPage:		
		tower->setWebPage(pItem->_col[1]);
		break;
	case af_tower_country:	
		{
			for (int i=c_england;i<=c_india;i++)
				if (pItem->_col[1] == recordManager()->getCountryAsString((Country)i))
				{
					tower->setCountry((Country)i);
					break;
				}
			if (pItem->_col[1] == recordManager()->getCountryAsString(c_none, true))
				tower->setCountry(c_none);
		}
		break;
//	case af_tower_distanceFromHome:		//static
//		return "Distance From Home Tower";
//	case af_tower_direction:			//static
//		return "Direction From Home Tower";
//	case af_tower_grabCount:			//static
//		return "Grab Count";
	case af_tower_comments:
		tower->setComment(pItem->_col[1]);
		break;

/*		//grab fields
	case af_grab_number:			
		return "No";
	case af_grab_date:			
		return "Date";
	case af_grab_bellsGo:		
		return "Bells Go";
	case af_grab_bellsSound:		
		return "Bells Sound";
	case af_grab_comments:
		return "Comments (Grab)"; */

	default:
		break;
	}
	
	recordManager()->setUpdatePoint("Edit " + pItem->_col[0] + " for " + tower->getName());
}


void RecordAnalysisTowerTower::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	ViewType viewType = app()->getViewType();
	if (viewType == vt_grab || viewType == vt_tower || viewType == vt_map)		
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
		
		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_TOWER_ANALYSIS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		pPopupMenu->Create(this, point.x, point.y, pPopup->Detach (), FALSE, TRUE);	
	}
}

void RecordAnalysisTowerTower::OnEditCopy() 
{
	if(_tower && OpenClipboard())
	{
		EmptyClipboard();

		CString strSource; // todo migrate to matrix = recordManager()->getCopyData(_tower);
		
		HGLOBAL strBuff;
		char * buffer;
		
		//str
		strBuff = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+1);
		buffer = (char*)GlobalLock(strBuff);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(strBuff);
		SetClipboardData(CF_TEXT,strBuff);

		CloseClipboard();
	}

}

void RecordAnalysisTowerTower::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_tower != NULL);	
}

void RecordAnalysisTowerTower::OnPopupPrintAnalysis() 
{
	CDocument* pDoc = (CDocument*)recordManager()->getTowerDoc();

	if (!pDoc) 
		pDoc = (CDocument*)recordManager()->getGrabDoc();

	if ((pDoc)&&(_tower))
	{
		POSITION pos = pDoc->GetFirstViewPosition();

		CView* view = pDoc->GetNextView(pos);

		if (view)
		{
			recordManager()->setPrintTowerRef(_tower);
			::PostMessage(view->m_hWnd,WM_COMMAND, ID_POPUP_PRINT_ANALYSIS,0);	
		}  
	}	   	
}

void RecordAnalysisTowerTower::OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_tower != NULL);	
}

void RecordAnalysisTowerTower::OnLaunchWebPage() 
{
	if ((_tower)&&(!_tower->getWebAddress().IsEmpty()))
		app()->launchBrowser(_tower->getWebAddress());
}

void RecordAnalysisTowerTower::OnUpdateLaunchWebPage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((_tower)&&(!_tower->getWebAddress().IsEmpty()));
}


void RecordAnalysisTowerTower::OnShowMap() 
{
	CWaitCursor wait;

	if (_tower)
		mapManager()->showTowerMapDoc(_tower);
}

void RecordAnalysisTowerTower::OnUpdateShowMap(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(mapManager()->isValid()&&
					_tower&&
					_tower->getMapRef().isValidOSGridRef());		
}


