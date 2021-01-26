// OptionsMethod.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "OptionsMethod.h"
#include "StyleManager.h"  
#include "GlobalDataManager.h"
#include "GenericEditDlg.h"
#include "StyleEditDlg.h"
#include ".\optionsmethod.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptionsMethod dialog


OptionsMethod::OptionsMethod()
	: CBCGPPropertyPage(OptionsMethod::IDD, NULL)
	, _startAtRow0(0)
{
	//{{AFX_DATA_INIT(OptionsMethod)
	_allChange = -1;
	_seperator = -1;
	_rowNoType = -1;
	_saveStyleWithTouch = FALSE;
	//}}AFX_DATA_INIT
}


void OptionsMethod::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptionsMethod)
	DDX_Control(pDX, IDC_OPTIONS_LINES_EDIT, _editBtn);
	DDX_Control(pDX, IDC_OPTIONS_LINES_DEL, _delBtn);
	DDX_Control(pDX, IDC_OPTIONS_LINES_ADD, _addBtn);
	DDX_Control(pDX, IDC_SEPERATOR, _seperatorCombo);
	DDX_Control(pDX, IDC_ALL_CHANGE, _allChangeCombo);
	DDX_Control(pDX, IDC_OPTIONS_LINES_STYLE_COMBO, _comboStyle);
	DDX_CBIndex(pDX, IDC_ALL_CHANGE, _allChange);
	DDX_CBIndex(pDX, IDC_SEPERATOR, _seperator);
	DDX_Radio(pDX, IDC_ROW_NO_TYPE, _rowNoType);
	DDX_Check(pDX, IDC_SAVE_STYLE_TOUCH, _saveStyleWithTouch);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_START_AT_ROW0, _startAtRow0);
}


BEGIN_MESSAGE_MAP(OptionsMethod, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(OptionsMethod)
	ON_BN_CLICKED(IDC_OPTIONS_LINES_ADD, OnOptionsMethodAdd)
	ON_BN_CLICKED(IDC_OPTIONS_LINES_DEL, OnOptionsMethodDel)
	ON_BN_CLICKED(IDC_OPTIONS_LINES_EDIT, OnOptionsMethodEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
   
BOOL OptionsMethod::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	_addBtn.SetImage(IDB_NEW);
	_addBtn.m_bDrawFocus = FALSE;
	_addBtn.SetTooltip("Add Style");
	_addBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_delBtn.SetImage(IDB_DELETE);
	_delBtn.m_bDrawFocus = FALSE;
	_delBtn.SetTooltip("Delete Style");
	_delBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_editBtn.SetImage(IDB_DOT_DOT_DOT);
	_editBtn.m_bDrawFocus = FALSE;
	_editBtn.SetTooltip("Edit Style");
	_editBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	
	for (int i=0;i<styleManager()->getStyleCount();i++)
	{
		_comboStyle.AddString(styleManager()->getStyle(i)->_name);
	}
	//set the active style
	_comboStyle.SetCurSel(styleManager()->getActiveStyleIndex());

	char allChange = globalDataManager()->getAllChangeChar();
	switch (allChange)
	{
	case 'x':
		_allChange = 0;
		break;
	case 'X':
		_allChange = 1;
		break;
	case '-':
		_allChange = 2;
		break;		
	default:
		_allChange = -1;
		break;

	}

	SeperationStyle seperationStyle = globalDataManager()->getSeperationStyle();
	switch (seperationStyle)
	{
	case SEP_VERBOSE:
		_seperator = 0;
		break;
	case SEP_COMPACT:
		_seperator = 1;
		break;		
	default:
		_seperator = -1;
	}

	_rowNoType = globalDataManager()->getRowType();

	_saveStyleWithTouch= app()->GetProfileInt("Style", 
									"Save With Method", TRUE);

	_startAtRow0 = globalDataManager()->isStartAtRowZero()?0:1;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void OptionsMethod::OnOptionsMethodAdd() 
{
	//get the name of the new style
	GenericEditDlg dlg(NULL, "New Style", "Enter style name", "Enter style name" );

	do 
	{
		if (dlg.DoModal() == IDCANCEL) return;
		dlg._strGenericEdit = GlobalFunctions::titleString(dlg._strGenericEdit);
		if (!styleManager()->isStyleUnique(dlg._strGenericEdit))
		{
			CString str;
			AfxMessageBox(dlg._strGenericEdit + " is not unique. Choose another name");
		}
		else
		{
			//create a new style using currently selected as default
			Style style(*styleManager()->getStyle(_comboStyle.GetCurSel()));
			style._name = dlg._strGenericEdit;

			//edit 
			StyleEditDlg styleEditDlg(style, this, MAXBELLS,  slt_edit, style._name);
			if (styleEditDlg.DoModal() == IDOK)
			{
				//add to list
				styleManager()->addStyle(style);
				//add to combo box	 
				_comboStyle.AddString(dlg._strGenericEdit);
				_comboStyle.SelectString(-1,dlg._strGenericEdit);
			}
			return;
		}
	}while(true);
}

void OptionsMethod::OnOptionsMethodDel() 
{
	int iSel = _comboStyle.GetCurSel();

	//stops the default style being deleted
	if (iSel == 0)
	{
		AfxMessageBox("You cant delete the default style");
		return;
	}

	if (iSel != CB_ERR) 
	{
		//remove from the array
		styleManager()->removeStyle(iSel);	
		//remove from combo box
		_comboStyle.DeleteString(iSel);
		//select the next item in the combo box, 
		//or the previous if sel was the last on the list
		if (iSel >= _comboStyle.GetCount()) 
			iSel = _comboStyle.GetCount() -1; 
		_comboStyle.SetCurSel(iSel);		
	}
	
}

void OptionsMethod::OnOptionsMethodEdit() 
{
	int iSel = _comboStyle.GetCurSel();
	 
	//stops the default style being edeted
	if (iSel == 0)
	{
		AfxMessageBox("You cant edit the default style. Press the Add \nbutton to create a custom style based on the selected style");
		return;
	}
	
	StyleEditDlg styleEditDlg(*styleManager()->getStyle(iSel), this, MAXBELLS, slt_edit,
		styleManager()->getStyle(iSel)->_name);	
	
	styleEditDlg.DoModal();

	//as style name could have changed, we need to repopulate the combo box
	_comboStyle.ResetContent();
	for (int i=0;i<styleManager()->getStyleCount();i++)
	{
		_comboStyle.AddString(styleManager()->getStyle(i)->_name);
	}
	_comboStyle.SetCurSel(iSel);

}



BOOL OptionsMethod::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		styleManager()->setActiveStyleIndex(_comboStyle.GetCurSel());

		char allChange;
		switch(_allChange)
		{
		case 0:
			allChange = 'x';
			break;
		case 1:
			allChange = 'X';
			break;
		default:
		case 2:
			allChange = '-';
			break;
		}

		globalDataManager()->setAllChangeChar(allChange);

		SeperationStyle seperationStyle;
		switch(_seperator)
		{
		case 1:
			seperationStyle = SEP_COMPACT;
			break;
		case 0:
		default:
			seperationStyle = SEP_VERBOSE;
			break;
		}

		globalDataManager()->setSeperationStyle(seperationStyle);

		globalDataManager()->setRowType((RowType)_rowNoType);
		globalDataManager()->setStartAtRowZero(_startAtRow0 == 0);

		app()->WriteProfileInt("Style", 
								"Save With Method", _saveStyleWithTouch);	
		return TRUE;
	}
	return FALSE;
}
