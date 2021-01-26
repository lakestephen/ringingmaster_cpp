// MatrixFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ringingmaster.h"
#include "MatrixFilterDlg.h"

#include "MatrixEntityBase.h"
#include "UserMessages.h"

#include "WinPos.h"



// MatrixFilterDlg dialog


MatrixFilterDlg::MatrixFilterDlg(MatrixFilterSort& originalFilterSort, bool editing, CWnd* pParent )
	: ResizeDialog(MatrixFilterDlg::IDD, pParent),
	_originalFilterSort(originalFilterSort),//save the reference
	_filterSort(originalFilterSort),//make a copy to work on
	_tabCtrl(_filterSort),  //pass the reference on
	_descCtrl(_filterSort), //pass the reference on
	_editing(editing)
{
	_name = _filterSort.getName(true);

	if (_name.IsEmpty())
		_name = "New Filter";
}

MatrixFilterDlg::~MatrixFilterDlg()
{       

}

void MatrixFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UP, _up);
	DDX_Control(pDX, IDC_DOWN, _down);
	DDX_Control(pDX, IDC_REMOVE, _remove);
	DDX_Control(pDX, IDC_MATRIX_FILTER_NAME, _nameCtrl);
	DDX_Text(pDX, IDC_MATRIX_FILTER_NAME, _name);
}


BEGIN_MESSAGE_MAP(MatrixFilterDlg, ResizeDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_MATRIX_FILTER_UPDATE, OnMatrixFilterUpdate) 
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDC_DOWN, OnBnClickedDown)
	ON_BN_CLICKED(IDC_UP, OnBnClickedUp)
	ON_UPDATE_COMMAND_UI(IDC_UP, OnUpdateUp)		   	
	ON_UPDATE_COMMAND_UI(IDC_DOWN, OnUpdateDown)	
	ON_UPDATE_COMMAND_UI(IDC_REMOVE, OnUpdateRemove)	
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_EN_CHANGE(IDC_MATRIX_FILTER_NAME, OnEnChangeMatrixFilterName)
END_MESSAGE_MAP()


LRESULT MatrixFilterDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}


// MatrixFilterDlg message handlers

BOOL MatrixFilterDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();

	CRect clientRect;
	GetClientRect(&clientRect);


	//create the tabs, and all the windows it contains.
	CRect rectTab(11,10,clientRect.right - 10, 200);
	if (!_tabCtrl.Create (rectTab, this))
	{
		return FALSE;
	}

	//createthe description window
	CRect rectDesc(11,210,clientRect.right - 40, clientRect.bottom - 85);
	if (!_descCtrl.Create (rectDesc, this))
	{
		return FALSE;
	}

	_up.SetImage(IDB_UP);
	_up.m_bDrawFocus = FALSE;
	_up.SetTooltip("Move Item Up");
	_up.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_down.SetImage(IDB_DOWN);
	_down.m_bDrawFocus = FALSE;
	_down.SetTooltip("Move Item Down");
	_down.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_remove.SetImage(IDB_DELETE);
	_remove.m_bDrawFocus = FALSE;
	_remove.SetTooltip("Remove Item");
	_remove.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	AddControl(IDC_TAB, resize, none);
	AddControl(IDC_MATRIX_FILTER_DESCRIP, resize, resize);
	AddControl(IDC_MATRIX_FILTER_NAME, resize, repos);  	
	AddControl(IDC_UP, repos, none);  	
	AddControl(IDC_DOWN, repos, none);  	
	AddControl(IDC_REMOVE, repos, none);  	
	AddControl(IDOK, repos_center, repos);  	
	AddControl(IDCANCEL, repos_center, repos);  	

	CString section;
	section.Format("MatrixFilterDlg%d", (int)_filterSort.getMatrixEntityToken());

	WinPos::Load(this, section, true) ;

	update();

	SetWindowText((_editing?"Edit ":"Add ") + _filterSort.getEntityName() + " Filter: " + _name);

	return TRUE;
}

void MatrixFilterDlg::OnEnChangeMatrixFilterName()
{
	if (::IsWindow(_nameCtrl.m_hWnd))
	{
		CString name;
		_nameCtrl.GetWindowText(name);
		SetWindowText((_editing?"Edit ":"Add ") + _filterSort.getEntityName() + " Filter: " + name );
	}
}

void MatrixFilterDlg::OnDestroy()
{
	CString section;
	section.Format("MatrixFilterDlg%d", (int)_filterSort.getMatrixEntityToken());

	WinPos::Save(this, section);

	ResizeDialog::OnDestroy();
}

LRESULT MatrixFilterDlg::OnMatrixFilterUpdate(WPARAM wParam, LPARAM lParam)
{
	update();
	return 0;
}

void MatrixFilterDlg::update()
{
	if (::IsWindow(_descCtrl.m_hWnd))
		_descCtrl.update();
}

void MatrixFilterDlg::OnBnClickedDown()
{
	if ((_descCtrl.getSelectedSort()>=0)&&(_filterSort.moveSortDown(_descCtrl.getSelectedSort())))
	{
		_descCtrl.setSelectedSort(_descCtrl.getSelectedSort() + 1);			
	}      
	else if ((_descCtrl.getSelectedColumn()>=0)&&(_filterSort.moveColumnDown(_descCtrl.getSelectedColumn())))
	{
		_descCtrl.setSelectedColumn(_descCtrl.getSelectedColumn() + 1);			
	}   	  		
}

void MatrixFilterDlg::OnBnClickedUp()
{
	if ((_descCtrl.getSelectedSort()>=0)&&(_filterSort.moveSortUp(_descCtrl.getSelectedSort())))
	{
		_descCtrl.setSelectedSort(_descCtrl.getSelectedSort() -1);			
	}      
	else if ((_descCtrl.getSelectedColumn()>=0)&&(_filterSort.moveColumnUp(_descCtrl.getSelectedColumn())))
	{
		_descCtrl.setSelectedColumn(_descCtrl.getSelectedColumn() -1);			
	}      
}

void MatrixFilterDlg::OnBnClickedRemove()
{
	if ((_descCtrl.getSelectedSort()>=0)&&(_filterSort.deleteSort(_descCtrl.getSelectedSort())))
	{
		_descCtrl.setSelectedSort(max(0,_descCtrl.getSelectedSort() -1));			
	}      
	else if ((_descCtrl.getSelectedColumn()>=0)&&(_filterSort.deleteColumn(_descCtrl.getSelectedColumn())))
	{
		_descCtrl.setSelectedColumn(max(0,_descCtrl.getSelectedColumn() -1));			
	}      
	else if ((_descCtrl.getSelectedFilter()>=0)&&(_filterSort.deleteFilter(_descCtrl.getSelectedFilter())))
	{
		_descCtrl.setSelectedFilter(max(0,_descCtrl.getSelectedFilter() -1));			
	}      

	_tabCtrl.update();
}

void MatrixFilterDlg::OnUpdateUp(CCmdUI* pCmdUI)  
{																						 
	pCmdUI->Enable((_descCtrl.getSelectedSort() >0)&&(_descCtrl.getSelectedSort() < _filterSort.getSortCount())||
				   (_descCtrl.getSelectedColumn() >0)&&(_descCtrl.getSelectedColumn() < _filterSort.getColumnCount()));
}

void MatrixFilterDlg::OnUpdateDown(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable((_descCtrl.getSelectedSort() >=0)&&(_descCtrl.getSelectedSort() < _filterSort.getSortCount()-1)||
				   (_descCtrl.getSelectedColumn() >=0)&&(_descCtrl.getSelectedColumn() < _filterSort.getColumnCount()-1));
}

void MatrixFilterDlg::OnUpdateRemove(CCmdUI* pCmdUI)  
{
	pCmdUI->Enable((_descCtrl.getSelectedSort()   >= 0)&&(_descCtrl.getSelectedSort()   < _filterSort.getSortCount()  )||
				   (_descCtrl.getSelectedColumn() >= 0)&&(_descCtrl.getSelectedColumn() < _filterSort.getColumnCount())||
				   (_descCtrl.getSelectedFilter() >= 0)&&(_descCtrl.getSelectedFilter() < _filterSort.getFilterCount()));
}

void MatrixFilterDlg::OnOK()
{
	UpdateData();

	if ((_filterSort.getFilterCount() == 0)&&(_filterSort.getSortCount() == 0)&&(_filterSort.getColumnCount() == 0))
	{
		AfxMessageBox("Please select a filter, a sort, or some columns.");
		return;
	}

/*	//this is commented out as we are not usingthe column feature. We may want it in the future.
	if (_filterSort.getColumnCount() == 0)
	{
		if (AfxMessageBox("This filter has no columns selected. Is this your intention?", MB_YESNO) != IDYES)
		{
			_tabCtrl.SetActiveTab(2);
			return;
		}    		
	}
	*/

	if (!_filterSort.isAllInvoked())
	{
		AfxMessageBox("Please complete all the red highlighted links in the filter description.");
		return;

	}

	if (_name.IsEmpty())
	{
		AfxMessageBox("Please choose a name for the filter");
		::SetFocus(_nameCtrl.GetSafeHwnd());
		return;
	}
	else
		_filterSort.setName(CapitalString(_name));

	if (!((matrixManager()->isFilterNameUnique(_filterSort.getMatrixEntityToken(), _filterSort.getMatrixFilterSectionToken(), _name))||
		((_editing)&&(_originalFilterSort.getName() == _name))))
	{
		AfxMessageBox("Please choose a unique filter name.");
		::SetFocus(_nameCtrl.GetSafeHwnd());
		return;
	}

	//set the original filter
	_originalFilterSort = _filterSort;
	
	ResizeDialog::OnOK();     

}


CString MatrixFilterDlg::CapitalString(CString strString)
{
	if (!strString.IsEmpty())
	{
		strString.MakeLower();
		strString.TrimLeft();
		strString.TrimRight();
		int i;
		while ((i = strString.FindOneOf("#%;./\\")) != -1)
			strString.Delete(i,1);
		while(strString.Replace("  ", " "));
		
		for (i=0;i<strString.GetLength();i++)
		{
			if ((i==0)||
				((i>0)&&(strString.GetAt(i-1) == ' ')))
			{
				CString upper(strString.GetAt(i));
				upper.MakeUpper();
				strString.SetAt(i,upper.GetAt(0));
			}
		}
	}
	return strString;
}


BOOL MatrixFilterDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (::IsWindow(_tabCtrl.m_hWnd))
		if (_tabCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;          

	return ResizeDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
