// MatrixGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixGridCtrl.h"
#include "MatrixEntityBase.h"
#include "MatrixItemBase.h"
#include "MatrixManager.h"
#include "MatrixFilterSort.h"
#include "GotoRowDlg.h"


static BCGP_GRID_COLOR_DATA theme = 
{
	-1,	// Grid background color
	-1,	// Grid foreground color
	-1,	// Header foreground color

	{	// Even rows colors:
		RGB (246, 250, 253), -1, -1, 0, -1
	},

	{	// Odd rows colors:
		-1, -1, -1, 0, -1
	},

	{	// Group colors:
		RGB (210, 230, 249), RGB (16, 37, 127), RGB (228, 241, 251), 90, -1
	},

	{	// Selected group colors:
		RGB (250, 199, 97), RGB (0, 0, 0), -1, 0, RGB (192, 192, 192)
	},

	{	// Selected rows colors:
		RGB (255, 229, 153), -1, -1, 0, RGB (192, 192, 192)
	},

	{	// Header item colors:
//		RGB (154, 194, 237), RGB (16, 37, 127), RGB (189, 214, 242), 90, RGB (136, 176, 228)
			-1, -1, -1, 0, -1
	},

	{	// Selected header item colors:
		-1, -1, -1, 0, -1
	},

	{	// Left offset colors:
		RGB (207, 221, 240), -1, RGB (227, 234, 244), 90, RGB (136, 176, 228)
	},

	-1,	// Grid horizontal line text
	-1,	// Grid vertical line text
};

const UINT MatrixGridCtrl::_findDialogMessage = RegisterWindowMessage(FINDMSGSTRING);



// MatrixGridCtrl

IMPLEMENT_DYNAMIC(MatrixGridCtrl, CBCGPGridCtrl)

MatrixGridCtrl::MatrixGridCtrl(MatrixEntityBase* entityBase) :
_matrixEntity(entityBase),
_pFindDialog(NULL)
{
	ASSERT(_matrixEntity);
	matrixManager()->addEventListener(this);
}

MatrixGridCtrl::~MatrixGridCtrl()
{
	matrixManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(MatrixGridCtrl, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SHOW_ALL, &MatrixGridCtrl::OnShowAll)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL, &MatrixGridCtrl::OnUpdateShowAll)
	ON_COMMAND(ID_INVERT_SELECTION, &MatrixGridCtrl::OnInvertSelection)
	ON_COMMAND(ID_MEMORY_ASSIGN, &MatrixGridCtrl::OnMemoryAssign)
	ON_COMMAND(ID_MAIN_ASSIGN, &MatrixGridCtrl::OnMainAssign)	
	ON_COMMAND(ID_MEMORY_SWAP, &MatrixGridCtrl::OnMemorySwap)
	ON_COMMAND(ID_MEMORY_OR, &MatrixGridCtrl::OnMemoryOr)
	ON_COMMAND(ID_MEMORY_MINUS, &MatrixGridCtrl::OnMemoryMinus)
	ON_COMMAND(ID_MEMORY_AND, &MatrixGridCtrl::OnMemoryAnd)
	ON_COMMAND(ID_MEMORY_ADD_ITEM, &MatrixGridCtrl::OnMemoryAddItem)
	ON_COMMAND(ID_MEMORY_REMOVE_ITEM, &MatrixGridCtrl::OnMemoryRemoveItem)
	ON_UPDATE_COMMAND_UI(ID_MEMORY_ADD_ITEM, &MatrixGridCtrl::OnUpdateMemoryAddItem)
	ON_UPDATE_COMMAND_UI(ID_MEMORY_REMOVE_ITEM, &MatrixGridCtrl::OnUpdateMemoryRemoveItem)
	ON_COMMAND(ID_EDIT_GOTO, OnEditGoto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTO, OnUpdateEditGoto)
    ON_REGISTERED_MESSAGE(_findDialogMessage, OnFindDialogMessage)
	ON_COMMAND(ID_EDIT_FIND_PREVIOUS, OnEditFindPrevious)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)

	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

BOOL CALLBACK MatrixGridCtrl::GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT (pdi != NULL);
	MatrixGridCtrl* pGridCtrl = (MatrixGridCtrl*) lp;
	ASSERT(pGridCtrl);
	return pGridCtrl->gridCallback(pdi);
}

BOOL MatrixGridCtrl::gridCallback(BCGPGRID_DISPINFO* pdi)
{
	ASSERT(_matrixEntity);

	//are we being requested a column?
	if (pdi->item.nCol == -1)
	{
		pdi->item.iImage= (int)0;		
	}
	else
	{
		//text
		CString text = _matrixEntity->getText(pdi->item.nRow, pdi->item.nCol);
		if (!text.IsEmpty())
			pdi->item.varValue = (LPCTSTR) text;

		//text color
		COLORREF textColor = _matrixEntity->getTextColor(pdi->item.nRow, pdi->item.nCol);
		if (textColor != UINT_MAX)
			pdi->item.clrText = textColor;
		
		//image
		MatrixGridImages image = _matrixEntity->getImage(pdi->item.nRow, pdi->item.nCol);
		if (image != mgi_none)
			pdi->item.iImage= (int)image;
	}
	return TRUE;

}

int MatrixGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!_matrixEntity)
		return -1;

	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableVirtualMode (GridCallback, (LPARAM) this);
	SetSingleSel(FALSE);
//	EnableMarkSortedColumn(TRUE, FALSE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_HIDE_ITEMS | BCGP_GRID_HEADER_SORT);
	SetColorTheme (theme);
	SetReadOnly(TRUE);
	SetWholeRowSel(TRUE);
	
	_imageList.Create(IDB_MATRIX_GRID, 16, 1, RGB(192,192,192));
	SetImageList(&_imageList);
	SetHeaderImageList(&_imageList);

	for (int i=0;i<_matrixEntity->getColumnCount();i++)
	{
		MatrixItemBase* item = _matrixEntity->getColumn(i);
		InsertColumn(i, item->getName(), item->getDefaultWidth(),(int)item->getHeaderImageIndex());
	}

	update();
	return 0;
}

void MatrixGridCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	ScreenToClient(&point);

	if (m_rectHeader.PtInRect (point))
		ShowColumnsChooser();
	else
	{
		ASSERT(_matrixEntity);

		if (_matrixEntity)
		{
			CBCGPGridRow* clickRow = HitTest(point);
			int clickRowId = -1;
			if (clickRow)
				clickRowId = clickRow->GetRowId();
			
			CArray<long, long> ids;
			getSelectedRowIds(ids);
			
			_matrixEntity->contextClick(ids, clickRowId, this);  
		}  
	}
}

void MatrixGridCtrl::getSelectedRowIds(CArray<long, long>& ids )
{
	CMap<long, long, long, long> idMap;

	//this is a list of selected areas. each one has a range of rows and columns.
	for (POSITION pos = m_lstSel.GetHeadPosition (); pos != NULL;)
	{
		CBCGPGridRange* pSelRange = m_lstSel.GetNext (pos);
		ASSERT (pSelRange != NULL);

		for (int i=pSelRange->m_nTop;i<= pSelRange->m_nBottom;i++)
		{
			idMap.SetAt(i,i);
		}
	}

	//put the map items into an array
	CPoint pt;
	CMap<long, long, long, long>::CPair* pCurVal;

	pCurVal= idMap.PGetFirstAssoc( );
	while (pCurVal != NULL)
	{
		ids.Add(pCurVal->key);
		pCurVal= idMap.PGetNextAssoc(pCurVal);
	}

	//now sort
	qsort((void*)ids.GetData(), 
		  (size_t)ids.GetSize(), 
		  sizeof(long), 
		  compareLong);	
}


int MatrixGridCtrl::compareLong(const void *arg1, const void *arg2)
{	
	return (*(long*)arg1) - (*(long*)arg2);
}



SaveableObject* MatrixGridCtrl::getActiveItem()
{
	if (m_idActive.m_nRow != -1)
	{
		return _matrixEntity->getEntityFromRow(m_idActive.m_nRow);
	}
  
	return NULL;
}


void MatrixGridCtrl::applyFilterSort(MatrixFilterSort * filterSort)
{
	//first see if we are of the correct type.
	ASSERT(_matrixEntity);
	ASSERT(filterSort);
	ASSERT(_matrixEntity->getMatrixEntityToken() == filterSort->getMatrixEntityToken());
	
	if (_matrixEntity && filterSort && _matrixEntity->getMatrixEntityToken() == filterSort->getMatrixEntityToken())
	{
		_matrixEntity->applyFilterSort(filterSort);
		update();
	}
}

void MatrixGridCtrl::update()
{				   
	ASSERT(_matrixEntity);

	if (_matrixEntity)
	{
		SetVirtualRows(_matrixEntity->getRowCount());
	}
	else
	{
		//no entity
		SetVirtualRows(0); 
	}	  
}

void MatrixGridCtrl::OnShowAll()
{
	_matrixEntity->resetIdList();
	_matrixEntity->applyDefaultSort();
	update();
}

void MatrixGridCtrl::OnUpdateShowAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(_matrixEntity->getIdCount() != _matrixEntity->getUnfilteredIdCount());
	pCmdUI->SetText("&Show All " + _matrixEntity->getName());
}

void MatrixGridCtrl::OnInvertSelection()
{
	_matrixEntity->invertSelection();
	update();
}

MatrixEntityBase* MatrixGridCtrl::getEntitity()
{
	return _matrixEntity;
}

bool MatrixGridCtrl::getEntitityToken(MatrixEntityToken& token)
{
	if (_matrixEntity)
	{
		token = _matrixEntity->getMatrixEntityToken();
		return true;
	}

	ASSERT(FALSE);
	return false;
}

void MatrixGridCtrl::OnMemoryAssign()
{
	_matrixEntity->memoryAssign();
	update();
}

void MatrixGridCtrl::OnMainAssign()
{
	_matrixEntity->mainAssign();
	update();
}

void MatrixGridCtrl::OnMemorySwap()
{
	_matrixEntity->memorySwap();
	update();
}

void MatrixGridCtrl::OnMemoryOr()
{
	_matrixEntity->memoryOr();
	update();
}

void MatrixGridCtrl::OnMemoryMinus()
{
	_matrixEntity->memoryMinus();
	update();
}

void MatrixGridCtrl::OnMemoryAnd()
{
	_matrixEntity->memoryAnd();
	update();
}

void MatrixGridCtrl::OnMemoryAddItem()
{
	/* todo
	//build the list of filtered towers
	RecordTowerMap tempTowers;
	for (int i=0;i<recordManager()->getFilteredTowerCount();i++)
		tempTowers.SetAt(recordManager()->getFilteredTower(i)->getIndex(), recordManager()->getFilteredTower(i));

	RecordTowers recordTowers;
	for (int i=0;i<recordManager()->getAllTowersCount();i++)
	{
		RecordTower* temp;
		RecordTower* recordTower = recordManager()->getAllTower(i);
		if (!tempTowers.Lookup(recordTower->getIndex(), temp))
		{
			recordTowers.Add(recordTower);
		}
	}


	CPoint point;
	GetCursorPos(&point);
	RecordChooseTowerDlg dlg(mainFrame(), point, ((recordTowers.GetSize() > 0)?recordTowers.GetAt(0):NULL), &recordTowers);
	dlg.DoModal();

	if (dlg.getRecordTower())
	{
		recordManager()->addTowerToFilteredList(dlg.getRecordTower());
		recordManager()->setUpdatePoint("Add Tower To Filter");
	}	   */


	//todo use addItemToFilter in the entity
}

void MatrixGridCtrl::OnUpdateMemoryAddItem(CCmdUI *pCmdUI)
{
	if (_matrixEntity)
	{
		pCmdUI->SetText("&Add " + _matrixEntity->getName() + " to Filter");
		pCmdUI->Enable(_matrixEntity->getIdCount() != _matrixEntity->getUnfilteredIdCount());	
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void MatrixGridCtrl::OnMemoryRemoveItem()
{
	// TODO
/*		RecordTower* recordTower = NULL;

	if (_list.getSelectedIndex() != -1)
		recordTower = recordManager()->getFilteredTower(_list.getSelectedIndex());

	if (recordTower == NULL)
	{
		CPoint point;
		GetCursorPos(&point);

		//build the list of filtered towers
		RecordTowers towers;
		for (int i=0;i<recordManager()->getFilteredTowerCount();i++)
			towers.Add(recordManager()->getFilteredTower(i));


		RecordChooseTowerDlg dlg(mainFrame(), point, ((towers.GetSize() > 0)?towers.GetAt(0):NULL), &towers);
		dlg.DoModal();

		recordTower = dlg.getRecordTower();
	}

	if (recordTower)
	{
		recordManager()->removeTowerFromFilteredList(recordTower);
		recordManager()->setUpdatePoint("Remove Tower From Filter");
		recordManager()->resetAllTowerSortColumns();
	}	*/   
}

void MatrixGridCtrl::OnUpdateMemoryRemoveItem(CCmdUI *pCmdUI)
{
	if (_matrixEntity)
		pCmdUI->SetText("&Remove " + _matrixEntity->getName() + " from Filter");

	pCmdUI->Enable(GetCurSel() != 0);
}

void MatrixGridCtrl::Sort(int nColumn, BOOL bAscending , BOOL bAdd )
{
	CWaitCursor wait;
	_matrixEntity->sortListCol(nColumn, bAscending);
	
	__super::Sort(nColumn, bAscending, bAdd);

	update();
}


BOOL MatrixGridCtrl::OnDrawItem (CDC* pDC, CBCGPGridRow* pItem)
{
	//this override it to allow the selection arrow to appear on the left of the selectyion. 
	CBCGPGridRow* savedRow = m_pSelRow;
	CBCGPGridRow* selectedRow = GetCurSel();

	if (m_pSelRow == 0 & selectedRow != 0)
		m_pSelRow = selectedRow;
	
	if (__super::OnDrawItem(pDC, pItem) == FALSE)
		return FALSE;
	
	m_pSelRow = savedRow;

}


void MatrixGridCtrl::OnEditGoto() 
{
	GotoRowDlg dlg(this, _matrixEntity->getIdCount()-1);
	if(dlg.DoModal() == IDOK)
	{
		SetCurSel(CBCGPGridItemID(dlg._row-1));
		EnsureVisible(GetCurSel());
	}		  	
}

void MatrixGridCtrl::OnUpdateEditGoto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_matrixEntity->getIdCount() >1)	;
}


//this is the callback handler
LRESULT MatrixGridCtrl::OnFindDialogMessage(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    ASSERT(_pFindDialog != NULL);
	
	

    // If the FR_DIALOGTERM flag is set,
    // invalidate the handle identifying the dialog box.
    if (_pFindDialog->IsTerminating())
    {
        _pFindDialog = NULL;
        return 0;
    }

    // If the FR_FINDNEXT flag is set,
    // call the application-defined search routine
    // to search for the requested string.
    if(_pFindDialog->FindNext())
    {
        //read data from dialog
        CString findName = _pFindDialog->GetFindString();
		_searchStr = findName;
        bool bSearchDown = _pFindDialog->SearchDown() == TRUE;

        //with given name do search
        doFind(findName, bSearchDown);
    }

    return 0;
}

							
void MatrixGridCtrl::doFind(CString findText, bool searchDown)
{
	CWaitCursor wait;

	if (_matrixEntity->getIdCount() <=1)
	{
		AfxMessageBox("There are not enough " + _matrixEntity->getName() + " in the filter");
		return;
	}

	CString originalText = findText;

	findText.MakeLower();

	if(searchDown)
	{
		int startPos = 0;
		CBCGPGridRow* curSel = GetCurSel();
		if (curSel)
			startPos = curSel->GetRowId();

		startPos++;

		if (startPos >= _matrixEntity->getIdCount())
			startPos = 0;

		//startPos Onward
		for (int i=startPos;i<_matrixEntity->getIdCount();i++)
		{
			if (doFindText(findText, i))
				return;
		}
		//roll round
		for (int i=0;i<startPos;i++)
		{
			if (doFindText(findText, i))
				return;
		}
	}
	else
	{
		//if there is no selection, then set to  end
		int startPos = _matrixEntity->getIdCount()-1;

		CBCGPGridRow* curSel = GetCurSel();
		if (curSel)
			startPos = curSel->GetRowId();			

		startPos--;

		if (startPos < 0)
			startPos = _matrixEntity->getIdCount()-1;


		for (int i=startPos;i>=0;i--)
		{
			if (doFindText(findText, i))
				return;
		}
		for (int i=_matrixEntity->getIdCount()-1;i>startPos;i--)
		{
			if (doFindText(findText, i))
				return;
		}
	}

	AfxMessageBox(originalText + " not found");
		 
}


bool MatrixGridCtrl::doFindText(const CString& findText, int index)
{	 
	CBCGPGridRow* row = GetRow(index);

	if (row == NULL)
		return false;

	for (int i=0;i<row->GetItemCount();i++)
	{
		CBCGPGridItem* item = row->GetItem(i);
		CString testString = item->GetValue();
		testString.MakeLower();

		if (testString.Find(findText) != -1)
		{
			//select the item
			SetCurSel(row);
			EnsureVisible(row);
			return true;
		}
	}					   
			
	return false;
}

void MatrixGridCtrl::OnEditFind() 
{
	if (_pFindDialog == NULL)
	{
		_pFindDialog = new CFindReplaceDialog;
		_pFindDialog->Create( TRUE, _searchStr, NULL, FR_DOWN | FR_HIDEWHOLEWORD | FR_HIDEMATCHCASE, this);	
	}
	else
	{
		_pFindDialog->SetFocus();
	}
}	   

void MatrixGridCtrl::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((_pFindDialog != NULL)?1:0);	

}

void MatrixGridCtrl::OnEditFindPrevious() 
{
	if (!_searchStr.IsEmpty())
		doFind(_searchStr, true);	

}							 

void MatrixGridCtrl::OnDestroy()
{
	if (_pFindDialog != NULL)
		destroyFindWindow();

	__super::OnDestroy();
}

void MatrixGridCtrl::destroyFindWindow()
{
	//remove the find dialog box.
	if (_pFindDialog != NULL)
	{
		_pFindDialog->EndDialog(0);
		_pFindDialog->DestroyWindow();
		_pFindDialog = NULL;
	}		
}


void MatrixGridCtrl::OnEditCopy() 
{
	
	if(OpenClipboard())
	{
		EmptyClipboard();

		CString strSource ;
		CArray<long, long> ids;
		getSelectedRowIds(ids);

		for (int i=0;i<ids.GetSize();i++)
		{
			CBCGPGridRow* row = GetRow(ids.GetAt(i));
			if (row)
			{
				for (int j=0;j<row->GetItemCount();j++)
				{
					CString itemTxt = row->GetItem(j)->GetValue();
					strSource += itemTxt;
					if (j < row->GetItemCount()-1)
						strSource += ", ";
				}

				strSource += "\r\n";
			}
		}

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

void MatrixGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCurSel() != NULL);	
}

void MatrixGridCtrl::matrixManager_entityAdded(MatrixEntityToken token, long id)
{
	if (_matrixEntity && _matrixEntity->getMatrixEntityToken() == token)
	{
		_matrixEntity->addItemToFilter(id, true);
		_matrixEntity->addItemToFilter(id, false);

		update();

		// now try and select it.
		int rowId = _matrixEntity->getRowFromID(id);
		if (rowId != -1)
		{
			CBCGPGridRow* row = GetRow(rowId);
			if (row)
			{
				SetCurSel(row);
				EnsureVisible(row);
			}
		}	
	}
}


void MatrixGridCtrl::matrixManager_entityRemoved(MatrixEntityToken token, long id)
{
	if (_matrixEntity && _matrixEntity->getMatrixEntityToken() == token)
	{
		_matrixEntity->removeItemFromFilter(id, true);
		_matrixEntity->removeItemFromFilter(id, false);

		SetCurSel(NULL, FALSE);
		update();
	}
}

void MatrixGridCtrl::matrixManager_refresh()
{
	CBCGPGridItemID id;
	GetCurSel(id);
	m_CachedItems.CleanUpCache ();
	SetCurSel(id);
	AdjustLayout();		   
}

void MatrixGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	//go through the list of items, looking at the sort criteria, and move to trhat row.
	TRACE("%c\r\n", nChar);

	int sortCol = m_Columns.GetSortColumn();

	if (isalnum(nChar)&&
		(sortCol != -1)&&
		(sortCol < GetColumnCount()))
	{
		CString testStr;
		_pressedString += ((char)nChar);
		int length = _pressedString.GetLength();
		SetTimer(AAA_TIMER_LIST_SCROLL, 600, NULL);
		TRACE("Finding Row: " + _pressedString + " flags: %d\r\n", nFlags); 

		//get the list of items being shown
		int i;
		int rowCount = _matrixEntity->getRowCount();
		for (i=0;i<rowCount;i++)
		{
			CString testStr = _matrixEntity->getText(i, sortCol);
			 
			testStr = testStr.Left(length);

			int compResult = testStr.CompareNoCase(_pressedString);

			//TRACE(" _pressedString = %s, testStr = %s, compResult = %d\r\n", _pressedString, testStr, compResult);

			if (compResult == 0)
			{
				SetCurSel(i);
				EnsureVisible(GetCurSel());
				TRACE("Select Row: %d \r\n", i);
				return;
			}
		}	  
	}  

	__super::OnChar(nChar, nRepCnt, nFlags);
}

void MatrixGridCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == AAA_TIMER_LIST_SCROLL)
	{
		_pressedString.Empty();
		KillTimer(AAA_TIMER_LIST_SCROLL);
		TRACE("Clearing Row: " + _pressedString + "\r\n"); 
	}						  
	
	__super::OnTimer(nIDEvent);
}

void MatrixGridCtrl::selectEntityFromId(int entityId)
{
	int rowId = _matrixEntity->getRowFromID(entityId);
	if (rowId != 0)
	{
		SetCurSel(rowId);
		EnsureVisible(GetCurSel());
	}
}
void MatrixGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
	CBCGPGridRow* selectedRow = GetCurSel();
	if (selectedRow)
		_matrixEntity->doubleClick(selectedRow->GetRowId(), this);

	__super::OnLButtonDblClk(nFlags, point);
}
