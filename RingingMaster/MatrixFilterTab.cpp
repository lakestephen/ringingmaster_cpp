// MatrixFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "MatrixFilterTab.h"
#include "MatrixEntityBase.h"
#include "MatrixManager.h"
#include "MatrixFilterSort.h"
#include "MatrixItemBase.h"

#include "UserMessages.h"


// MatrixFilterDlg dialog


MatrixFilterTab::MatrixFilterTab(MatrixFilterSort& filterSort)   :
_filterSort(filterSort), 
_entitybase(NULL)
{
}

MatrixFilterTab::~MatrixFilterTab()
{ 
	delete _entitybase;
	_entitybase = NULL;
}



BEGIN_MESSAGE_MAP(MatrixFilterTab, CBCGPTabWnd)
	ON_LBN_SELCHANGE(IDC_FILTER, OnSelchangeCondition)
	ON_LBN_DBLCLK(IDC_FILTER, OnSelchangeCondition)
	ON_LBN_SELCHANGE(IDC_SORTING, OnSelchangeSorting)
	ON_LBN_DBLCLK(IDC_SORTING, OnSelchangeSorting)	 
	ON_LBN_SELCHANGE(IDC_COLUMN, OnSelchangeColumn)
	ON_LBN_DBLCLK(IDC_COLUMN, OnSelchangeColumn)	 
END_MESSAGE_MAP()


BOOL MatrixFilterTab::Create(const RECT& rect, CWnd* pParentWnd)
{
	if (!CBCGPTabWnd::Create(CBCGPTabWnd::STYLE_3D, rect, pParentWnd, IDC_TAB))
	{
		return FALSE;
	}


	//filter window
	if (!_filterCtrl.Create(LBS_SORT | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS |  LBS_NOTIFY |
						LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
						CRect(0,0,0,0), 
						this,
						IDC_FILTER)) 
	{
		return FALSE;
	}
	AddTab(&_filterCtrl, "Filter");

	//sort window
	if (!_sortCtrl.Create(LBS_SORT | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS |	LBS_NOTIFY |
						LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
						CRect(0,0,0,0), 
						this,
						IDC_SORTING)) 
	{
		return FALSE;
	}
	AddTab(&_sortCtrl, "Sort");

	//column window
	//we do not want the column stuff, but leave in in case we ever want it.
	if (!_columnCtrl.Create(LBS_SORT | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOTIFY |
						LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
						CRect(0,0,0,0), 
						this,
						IDC_COLUMN)) 
	{
		return FALSE;
	}
	//AddTab(&_columnCtrl, "Column");

	if (_font.CreateStockObject(DEFAULT_GUI_FONT))
	{
		_filterCtrl.SetFont(&_font);
		_sortCtrl.SetFont(&_font);
		_columnCtrl.SetFont(&_font);
	}

	//populate the windows with all teh columns available.
	_entitybase = matrixManager()->createEntity(_filterSort.getMatrixEntityToken());
	_entitybase->init();

	CString entityTokenName = "<" + _entitybase->getName() + ">";//the angle 

	for (int i=0;i<_entitybase->getColumnCount();i++)
	{
		MatrixItemBase * column = _entitybase->getColumn(i);

		if (column )
		{
			ASSERT(column);

			CString name = column->getName(true);

			//replace the display token with the proper name. 
			name.Replace("<>", entityTokenName );

			//filter
			int index = _filterCtrl.AddString("Where the " + name + " is specified");
			_filterCtrl.SetItemDataPtr(index, column);
			_filterCtrl.SetCheck(index, false);

			//sort
			index = _sortCtrl.AddString("Sort by "  + name);
			_sortCtrl.SetItemDataPtr(index, column);
			_sortCtrl.SetCheck(index, false);

			//column
			index = _columnCtrl.AddString(name);
			_columnCtrl.SetItemDataPtr(index, column);
			_columnCtrl.SetCheck(index, false);
		}
	}		

	update();

	return TRUE;
}

void MatrixFilterTab::OnSelchangeCondition() 
{
	//loop through all the filters, and add them to the filter 
	for (int i = 0;i<_filterCtrl.GetCount();i++)
	{
		_filterSort.setFilter(*((MatrixItemBase*)_filterCtrl.GetItemDataPtr(i)), _filterCtrl.GetCheck(i) == 1);
	}

	GetParent()->PostMessage(MSG_MATRIX_FILTER_UPDATE);
}

void MatrixFilterTab::OnSelchangeSorting() 
{
	//loop through all the sorts, and add them to the filter 
	for (int i=0;i<_sortCtrl.GetCount();i++)
	{
		_filterSort.setSort(*(MatrixItemBase*)_sortCtrl.GetItemDataPtr(i), _sortCtrl.GetCheck(i) == 1, true);
	}

	GetParent()->PostMessage(MSG_MATRIX_FILTER_UPDATE);
}

void MatrixFilterTab::OnSelchangeColumn() 
{
	//loop through all the sorts, and add them to the filter 
	for (int i=0;i<_columnCtrl.GetCount();i++)
	{
		_filterSort.setColumn(*(MatrixItemBase*)_columnCtrl.GetItemDataPtr(i), _columnCtrl.GetCheck(i) == 1, true);
	}

	GetParent()->PostMessage(MSG_MATRIX_FILTER_UPDATE);
}

BOOL MatrixFilterTab::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (::IsWindow(_columnCtrl.m_hWnd))
		if (_columnCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;          

	return CBCGPTabWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MatrixFilterTab::update()
{
	//loop through all the filter check boxes, getting there associated MatrixItemBase
	for (int i = 0;i<_filterCtrl.GetCount();i++)
	{
		MatrixItemBase* item = static_cast<MatrixItemBase*>(_filterCtrl.GetItemDataPtr(i));
		bool checked = false;

		if (item)
		{
			//now go through all the filters and set the checks.
			for (int j=0;j<_filterSort.getFilterCount();j++)
			{
				if (_filterSort.getFilter(j)->getToken() == item->getToken())
					checked = true;
			}
		}
		_filterCtrl.SetCheck(i, checked);
	}

	//loop through all the sort check boxes, getting there associated MatrixItemBase
	for (int i = 0;i<_sortCtrl.GetCount();i++)
	{
		MatrixItemBase* item = static_cast<MatrixItemBase*>(_sortCtrl.GetItemDataPtr(i));
		bool checked = false;

		if (item)
		{
			//now go through all the sorts and set the checks.
			for (int j=0;j<_filterSort.getSortCount();j++)
			{
				if (_filterSort.getSort(j)->getToken() == item->getToken())
					checked = true;
			}
		}
		_sortCtrl.SetCheck(i, checked);
	}

	//loop through all the column check boxes, getting there associated MatrixItemBase
	for (int i = 0;i<_columnCtrl.GetCount();i++)
	{
		MatrixItemBase* item = static_cast<MatrixItemBase*>(_columnCtrl.GetItemDataPtr(i));
		bool checked = false;

		if (item)
		{
			//now go through all the columns and set the checks.
			for (int j=0;j<_filterSort.getColumnCount();j++)
			{
				if (_filterSort.getColumn(j)->getToken() == item->getToken())
					checked = true;
			}
		}
		_columnCtrl.SetCheck(i, checked);
	}
}