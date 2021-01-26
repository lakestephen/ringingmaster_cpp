// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "SearchDlg.h"
#include "NotationResultsDlg.h"
#include "MethodManager.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SearchDlg dialog


SearchDlg::SearchDlg(NotationLites& selection, int number, bool noSelect)
: CDialog(SearchDlg::IDD, NULL),
_advanced(FALSE),
_selection(selection),
_number(number),
_noSelect(noSelect)
{
	//{{AFX_DATA_INIT(SearchDlg)
	_location = FALSE;
	_CCC = FALSE;
	_dateTower = FALSE;
	_dateHand = FALSE;
	_LE = FALSE;
	_MUG = FALSE;
	_notation = FALSE;
	_RW_Ref = FALSE;
	_PMM = FALSE;
	_shorthand = FALSE;
	_TDMM = FALSE;
	_LH = FALSE;
	_searchStr = _T("");
	_nameMethod = FALSE;
	//}}AFX_DATA_INIT

}


SearchDlg::~SearchDlg()	
{
}


void SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SearchDlg)
	DDX_Control(pDX, IDC_FIND_WHAT, _findWhat);
	DDX_Control(pDX, IDC_NUMBER, _numberCtrl);
	DDX_Control(pDX, IDC_LINE_STATIC, _lineStatic);
	DDX_Control(pDX, ID_ADVANCED, _advancedBtn);
	DDX_Control(pDX, IDC_METHOD_NAME, _name);
	DDX_Check(pDX, IDC_LOCATION, _location);
	DDX_Check(pDX, IDC_CCC, _CCC);
	DDX_Check(pDX, IDC_DATE_TOWER, _dateTower);
	DDX_Check(pDX, IDC_DATEHAND, _dateHand);
	DDX_Check(pDX, IDC_LE, _LE);
	DDX_Check(pDX, IDC_MUG, _MUG);
	DDX_Check(pDX, IDC_NOTATION, _notation);
	DDX_Check(pDX, IDC_RW_REF, _RW_Ref);
	DDX_Check(pDX, IDC_PMM, _PMM);
	DDX_Check(pDX, IDC_SHORTHAND, _shorthand);
	DDX_Check(pDX, IDC_TDMM, _TDMM);
	DDX_Check(pDX, IDC_LH, _LH);
	DDX_CBString(pDX, IDC_METHOD_NAME, _searchStr);
	DDX_Check(pDX, IDC_NAME, _nameMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SearchDlg, CDialog)
	//{{AFX_MSG_MAP(SearchDlg)
	ON_BN_CLICKED(ID_ADVANCED, OnAdvanced)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SearchDlg message handlers

BOOL SearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_CCC        = app()->GetProfileInt("Search", "CCC",			FALSE);
	_dateHand   = app()->GetProfileInt("Search", "DateHand",	FALSE);
	_dateTower  = app()->GetProfileInt("Search", "DateTower",	FALSE);
	_LE         = app()->GetProfileInt("Search", "LE",			FALSE);
	_LH 	    = app()->GetProfileInt("Search", "LH",			FALSE);
	_notation   = app()->GetProfileInt("Search", "Notation",	FALSE);
	_location   = app()->GetProfileInt("Search", "Locations",	TRUE);
	_MUG        = app()->GetProfileInt("Search", "MUG",			FALSE);
	_PMM        = app()->GetProfileInt("Search", "PMM",			FALSE);
	_RW_Ref     = app()->GetProfileInt("Search", "RW_Ref",		FALSE);
	_shorthand  = app()->GetProfileInt("Search", "Shorthand",	FALSE);
	_TDMM       = app()->GetProfileInt("Search", "TDMM",		FALSE);
	_nameMethod	= app()->GetProfileInt("Search", "Name",		TRUE);

	UpdateData(FALSE);

	if ((_number <3)||(_number > MAXBELLS))
	{
		_numberCtrl.AddString("[Any Number...]");

		for (int i=MINBELLS;i<=MAXBELLS;i++)
		{
			_numberCtrl.AddString(GlobalFunctions::numberName(i));
		}

		int selection = app()->GetProfileInt("Search", "Number", 0 );
		_numberCtrl.SetCurSel(selection);

	}
	else
	{
		_numberCtrl.AddString(GlobalFunctions::numberName(_number));
		_numberCtrl.SetCurSel(0);
		_numberCtrl.EnableWindow(FALSE);
	}

   
	_name.LoadHistory("SearchDlg"); //must be after UpdateData(FALSE);

	_advanced   = app()->GetProfileInt("Search", "Advanced", FALSE);

	WinPos::Load(this, "SearchDlg", false);
	
	setDialogSize();

	return FALSE;//TRUE;  // return TRUE unless you set the focus to a control

}



void SearchDlg::OnOK() 
{
	UpdateData(TRUE);

	_name.SaveHistory();
	
	app()->WriteProfileInt("Search", "CCC",		 _CCC      );
	app()->WriteProfileInt("Search", "DateHand", _dateHand );
	app()->WriteProfileInt("Search", "DateTower",_dateTower);
	app()->WriteProfileInt("Search", "LE",		 _LE       );
	app()->WriteProfileInt("Search", "LH",		 _LH 	  );
	app()->WriteProfileInt("Search", "Notation", _notation );
	app()->WriteProfileInt("Search", "Locations",_location );
	app()->WriteProfileInt("Search", "MUG",		 _MUG      );
	app()->WriteProfileInt("Search", "PMM",		 _PMM      );
	app()->WriteProfileInt("Search", "RW_Ref",	 _RW_Ref   );
	app()->WriteProfileInt("Search", "Shorthand",_shorthand);
	app()->WriteProfileInt("Search", "TDMM",	 _TDMM     );
	app()->WriteProfileInt("Search", "Name",	 _nameMethod);

	if ((_number <3)||(_number > MAXBELLS))
	{
	    app()->WriteProfileInt("Search", "Number", _numberCtrl.GetCurSel());
	}
	
	app()->WriteProfileInt("Search", "Advanced", _advanced);

	//do the search
	doSearch();

	//have we got any results?	
	if (_selection.GetSize() == 0)
	{
		AfxMessageBox("No Matches Found");
	}
	else
	{
		NotationResultsDlg dlg(_selection, this,  _noSelect);
		if (dlg.DoModal() == IDOK)
    		CDialog::OnOK();
		else
			_selection.RemoveAll();
	}
}

void SearchDlg::OnAdvanced() 
{
	_advanced = !_advanced;
	setDialogSize();
}

void SearchDlg::setDialogSize()
{
	int length;
	if (_advanced) 
		length = 259;
	else
		length = 133;

	CRect rect;
	GetWindowRect(&rect);

	// update window position and display
	SetWindowPos(&wndTop, 
		0,
		0,
		rect.Width( ), 
		length, 
		SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE);

	_advancedBtn.SetWindowText(_advanced?"&Advanced <<":"&Advanced >>");
	_lineStatic.ShowWindow(_advanced?SW_SHOW:SW_HIDE);
	_findWhat.SetWindowText(_advanced?"Find What?":"Find What in Method?");

}

BOOL SearchDlg::DestroyWindow() 
{
	WinPos::Save(this, "SearchDlg");
		
	return CDialog::DestroyWindow();
}


void SearchDlg::doSearch()
{
	//get the name.
	CString searchStr = _searchStr;
	
	//get the notation string
	searchStr.MakeLower();
	CString metName, metNameNum, location;	

	//trim
	searchStr.TrimLeft();
	searchStr.TrimRight();

	//search for number
	int searchNumber = _number;
	
	//if not restricted to a specific number, use the selected number
	if ((searchNumber<3)||(searchNumber> MAXBELLS))
	{
		searchNumber = _numberCtrl.GetCurSel();
		//are we [Any Number...]??
		if (searchNumber != 0)
		{
			//copnvert to bell number
			searchNumber += 2;
		}
	}		


	if (!searchStr.IsEmpty())
	{
		//load the methods (if necessary)
		methodManager()->loadExplorerData();
		
		//get a list of the methods				   		
		for (int i=0;i<methodManager()->getMethodCount();i++)
		{
			if (_advanced) 
			{
				if (doAdvancedSearch(methodManager()->getMethod(i), searchStr, searchNumber))
					_selection.Add(methodManager()->getMethod(i));
			}
			else
			{
				if (doNormalSearch(methodManager()->getMethod(i), searchStr, searchNumber))
					_selection.Add(methodManager()->getMethod(i));
			}
		}
	}

}

bool SearchDlg::doNormalSearch(NotationLite *notation, CString& searchStr, int searchNumber)
{

	if ((searchNumber == 0)||(notation->getNumber() ==  searchNumber))
	{
		if (doIndividualSearch(searchStr, notation->getName(false)))
			return true;

		if (doIndividualSearch(searchStr, notation->getName(true)))
			return true;
	}

	return false;
}

bool SearchDlg::doAdvancedSearch(NotationLite *notation, CString& searchStr, int searchNumber)
{
	//do normal search
	if ((searchNumber == 0)||(notation->getNumber() ==  searchNumber))
	{
		//normal search		
		if ((_nameMethod == TRUE)&&
			((doIndividualSearch(searchStr, notation->getName(false)))||
		    (doIndividualSearch(searchStr, notation->getName(true )))))
			return true;

		//location
		if ((_location == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_place))))
			return true;

		//CCC
		if ((_CCC == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_CCC))))
			return true;

		//Date Tower
		if ((_dateTower == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_towerBells))))
			return true;

		//Date Hand
		if ((_dateHand == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_handbells))))
			return true;

		//MUG
		if ((_MUG == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_MUG))))
			return true;

		//RW_Ref
		if ((_RW_Ref == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_RW))))
			return true;

		//PMM
		if ((_PMM == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_PMM))))
			return true;

		//TDMM
		if ((_TDMM == TRUE)&&
			(doIndividualSearch(searchStr, notation->getCCInfo(cc_TDMM))))
			return true;

		//shorthand
		if ((_shorthand == TRUE)&&
			(doIndividualSearch(searchStr, notation->getSpliceLetter())))
			return true;
		
		//we are dealing with notations here
		//lets not do it unless we have to!
		if ((_notation == TRUE)||
			(_LE == TRUE)||
			(_LH == TRUE))
		{
			while (searchStr.Replace("  ", " "));
			while (searchStr.Replace(". ", "."));
			while (searchStr.Replace(" .", "."));
			while (searchStr.Replace(" " , "."));
			while (searchStr.Replace("x" , "-"));
			while (searchStr.Replace("X" , "-"));
			while (searchStr.Replace("--" , "-"));

			//notation
			if ((_notation == TRUE)&&
				(doIndividualSearch(searchStr, notation->getNotation())))
				return true;

			//notation
			if ((_LE == TRUE)&&
				(doIndividualSearch(searchStr, notation->getLeadEnd())))
				return true;

			//notation
			if ((_LH == TRUE)&&
				(doIndividualSearch(searchStr, notation->getLeadHead())))
				return true;
		}
	}

	return false;
}

bool SearchDlg::doIndividualSearch(CString &searchStr, CString searchableStr)
{
	searchableStr.MakeLower();
	return (searchableStr.Find(searchStr) != -1);
}
