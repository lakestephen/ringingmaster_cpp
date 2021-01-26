// NotationPlain.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationPlain.h"

#include "Notation.h"
#include "NotationDlg.h"
#include "MethodEditDlg.h"
#include "NotationExplorerDlg.h"
#include "NotationResultsDlg.h"
#include "MethodManager.h"
#include ".\notationplain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NotationPlain property page


NotationPlain::NotationPlain(Notation &notation, BOOL fixNumber) : 
CBCGPPropertyPage(NotationPlain::IDD), 
_notation(notation),
_fixNumber(fixNumber)
{
	//{{AFX_DATA_INIT(NotationPlain)
	_leadEndStatic = _T("");
	_notationStatic = _T("");
	_splice = _T("");
	_not =		_T("");
	_asym =		FALSE;
	_leadEnd =	_T("");
	_name =		_T("");
	_splice =	_T("");
	//}}AFX_DATA_INIT

}

NotationPlain::~NotationPlain()
{
}

void NotationPlain::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationPlain)
	DDX_Control(pDX, IDC_METHOD_EXPLORER, _methodExplorer);
	DDX_Control(pDX, IDC_VISUAL_EDIT, _visualEdit);
	DDX_Control(pDX, IDC_SEARCH_NOTATION, _searchNotation);
	DDX_Control(pDX, IDC_SEARCH_NAME, _searchName);
	DDX_Control(pDX, IDC_NOTATION_STATIC, _notationStaticCtrl);
	DDX_Control(pDX, IDC_NOTATION, _notationCtrl);
	DDX_Control(pDX, IDC_LEAD_END, _leadEndCtrl);
	DDX_Control(pDX, IDC_LEAD_END_TITLE_STATIC, _leadEndTitleStaticCtrl);
	DDX_Control(pDX, IDC_LEAD_END_STATCI, _leadEndStaticCtrl);
	DDX_Control(pDX, IDC_NUMBER, _number);
	DDX_Text(pDX, IDC_LEAD_END, _leadEnd);
	DDX_Text(pDX, IDC_LEAD_END_STATCI, _leadEndStatic);
	DDX_Text(pDX, IDC_NAME, _name);
	DDX_Text(pDX, IDC_NOTATION_STATIC, _notationStatic);
	DDX_Text(pDX, IDC_SPLICE, _splice);
	DDX_Text(pDX, IDC_NOTATION, _not);
	DDX_Check(pDX, IDC_ASYM, _asym);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(NotationPlain, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(NotationPlain)
	ON_BN_CLICKED(IDC_ASYM, OnAsym)
	ON_BN_CLICKED(IDC_METHOD_EXPLORER, OnMethodExplorer)
	ON_BN_CLICKED(IDC_SEARCH_NOTATION, OnSearchNotation)
	ON_BN_CLICKED(IDC_SEARCH_NAME, OnSearchName)
	ON_BN_CLICKED(IDC_VISUAL_EDIT, OnVisualEdit)
	ON_EN_CHANGE(IDC_LEAD_END, verify)
	ON_EN_CHANGE(IDC_NOTATION, verify)
	ON_CBN_SELCHANGE(IDC_NUMBER, verify)
	ON_EN_KILLFOCUS(IDC_NAME, OnKillfocusName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NotationPlain message handlers


BOOL NotationPlain::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	EnableToolTips();

	//fill the combo box
	int limit = MAXBELLS;

	if (mainFrame()->verifyKey() == FALSE)
		limit = 6;

	for (int i=MINBELLS;i<=limit;i++)
	{
		_number.AddString(GlobalFunctions::numberName(i));
	}

	//set up rects for asym
	_notationCtrl.GetWindowRect(&_notRect[0]);
	_notationStaticCtrl.GetWindowRect(&_notStaticRect[0]);
	_leadEndCtrl.GetWindowRect(&_notRect[1]);
	_leadEndStaticCtrl.GetWindowRect(&_notStaticRect[1]);
	_notRect[1].left = _notRect[0].left;
	_notStaticRect[1].left = _notStaticRect[0].left;

	ScreenToClient(&_notRect[0]);
	ScreenToClient(&_notRect[1]);
	ScreenToClient(&_notStaticRect[0]);
	ScreenToClient(&_notStaticRect[1]);

	_methodExplorer.SetImage(IDB_EXPLORE);
	_methodExplorer.m_bDrawFocus = FALSE;
	_methodExplorer.SetTooltip("Method Explorer");
	_methodExplorer.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_visualEdit.SetImage(IDB_NOTATION);
	_visualEdit.m_bDrawFocus = FALSE;
	_visualEdit.SetTooltip("Visual Notation Editor");
	_visualEdit.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_searchNotation.SetImage(IDB_FIND);
	_searchNotation.m_bDrawFocus = FALSE;
	_searchNotation.SetTooltip("Search Method Database For This Notation");
	_searchNotation.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_searchName.SetImage(IDB_FIND);
	_searchName.m_bDrawFocus = FALSE;
	_searchName.SetTooltip("Search Method Database For this Number, and Name (or partial name) and Location");
	_searchName.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;


	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationPlain::OnAsym() 
{
	UpdateData();
	_leadEndCtrl.ShowWindow((_asym)?SW_HIDE:SW_SHOW);
	_leadEndStaticCtrl.ShowWindow((_asym)?SW_HIDE:SW_SHOW);
	_leadEndTitleStaticCtrl.ShowWindow((_asym)?SW_HIDE:SW_SHOW);
		
	_notationCtrl.MoveWindow(_notRect[(_asym)?1:0]);
	_notationStaticCtrl.MoveWindow(_notStaticRect[(_asym)?1:0]);	
}

void NotationPlain::verify()
{
	CWaitCursor wait;
 	UpdateData();
	//get from the edit
	
	_notation.create(false,
					 _name, 
					 _number.GetCurSel() + 3,  
					 !_asym,
					 _not,
					 _leadEnd,
					 _splice,
					 false);

	//set the static
	_notationStatic = _notation.getNotation();
	_leadEndStatic  = _notation.getLeadEnd(); 

	((NotationDlg*)GetParent())->setConstantText(_notation.getName(true));


   	UpdateData(FALSE);					 
}


void NotationPlain::OnMethodExplorer() 
{
	NotationLites notations;
	NotationExplorerDlg dlg(this, notations, false, true, _notation.getNumber());

	if (dlg.DoModal() == IDOK)
	{
		if (notations.GetSize() > 0)
		{
			NotationLite* lite = notations.GetAt(0);

			//check for correct number
			if (lite->getNumber() != _notation.getNumber())
			{
				CString msg;
				msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
					lite->getName(true), GlobalFunctions::numberName(_notation.getNumber()));
				AfxMessageBox(msg);
			}
			else
			{
				_notation = *lite;

				//set the static
				_not   = _notation.getNotation();
				_leadEnd = _notation.getLeadEnd(); 
				_name = _notation.getName();
				_splice = _notation.getSpliceLetter();

   				UpdateData(FALSE);		
				verify();
			}
		} 	
	}

	for (int i=0;i<notations.GetSize();i++)
		delete notations.GetAt(i);
}

void NotationPlain::OnSearchName() 
{
	//get the notation string
	UpdateData();
	CString strName = _name;
	strName.MakeLower();
	CString metName, metNameNum;

	NotationLites lites;
	
	if (!strName.IsEmpty())
	{
		//load the methods (if necessary)
		methodManager()->loadExplorerData();

		//get a list of the methods

		
		for (int i=0;i<methodManager()->getMethodCount();i++)
		{
			metName = methodManager()->getMethod(i)->getName(true);
			metName .MakeLower();
			metNameNum = methodManager()->getMethod(i)->getName(false);
			metNameNum .MakeLower();

			if ((methodManager()->getMethod(i)->getNumber() ==  _notation.getNumber())&&
				((metName.Find(strName) != -1)||
				 (metNameNum.Find(strName) != -1)))
				lites.Add(methodManager()->getMethod(i));
		}

	}

	if (lites.GetSize() == 0)
	{
		AfxMessageBox("No Matches Found for Number and Name");
	}
	else
	{
		NotationResultsDlg dlg(lites, this);
		if (dlg.DoModal() == IDOK)
		{
			if (lites.GetSize() > 0)
				{
					NotationLite* lite = lites.GetAt(0);

					//check for correct number
					if (lite->getNumber() != _notation.getNumber())
					{
						CString msg;
						msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
							lite->getName(true), GlobalFunctions::numberName(_notation.getNumber()));
						AfxMessageBox(msg);
					}
					else
					{
						_notation = *lite;

						//set the static
						_not   = _notation.getNotation();
						_leadEnd = _notation.getLeadEnd(); 
						_name = _notation.getName();
						_splice = _notation.getSpliceLetter();

   						UpdateData(FALSE);		
						verify();
					}
			}
		} 	
	}
	for (int i=0;i<lites.GetSize();i++)
		delete lites.GetAt(i);	
}

void NotationPlain::OnSearchNotation() 
{
	//get the notation string
	CString strNot = _notation.getNotation(true);
	NotationLites lites;
	
	if (!strNot.IsEmpty())
	{
		//load the methods (if necessary)
		methodManager()->loadExplorerData();

		//get a list of the methods

		
		for (int i=0;i<methodManager()->getMethodCount();i++)
		{
			if ((methodManager()->getMethod(i)->getNumber() ==  _notation.getNumber())&&
				(methodManager()->getMethod(i)->getNotation(true) ==  strNot)&&
				(methodManager()->getMethod(i)->getLeadEnd() ==  _notation.getLeadEnd()))
				lites.Add(methodManager()->getMethod(i));
		}

	}

	if (lites.GetSize() == 0)
	{
		AfxMessageBox("No Matches Found for Number and Notation");
	}
	else
	{
		NotationResultsDlg dlg(lites, this);
		if (dlg.DoModal() == IDOK)
		{
			if (lites.GetSize() > 0)
				{
					NotationLite* lite = lites.GetAt(0);

					//check for correct number
					if (lite->getNumber() != _notation.getNumber())
					{
						CString msg;
						msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
							lite->getName(true), GlobalFunctions::numberName(_notation.getNumber()));
						AfxMessageBox(msg);
					}
					else
					{
						_notation = *lite;

						//set the static
						_not   = _notation.getNotation();
						_leadEnd = _notation.getLeadEnd(); 
						_name = _notation.getName();
						_splice = _notation.getSpliceLetter();

   						UpdateData(FALSE);		
						verify();
					}
			}
		} 	
	}	
	for (int i=0;i<lites.GetSize();i++)
		delete lites.GetAt(i);	

}

void NotationPlain::OnVisualEdit() 
{
	verify();  //this does the updateData()

	CString notStr;
	if ((_not.IsEmpty())&&
		(_leadEnd.IsEmpty()))
	{
		notStr.Empty();	
	}
	else
	{
		//create the temp notation
		Notation not;
		not.create(true, "",_notation.getNumber(), !_asym, _not, _leadEnd);
		not.convertToAsym();
		notStr = not.getNotation();
	}
		
	MethodEditDlg dlg(this, _notation.getNumber(), notStr);
	if (dlg.DoModal() == IDOK)
	{
		bool symm = !_asym;
		_notation.create(true, _name,_notation.getNumber(), false, dlg._notation, "", _splice, false);
		if (symm)
			_notation.checkForSymmetry(true);
	}

	setUpData();
	verify(); 
	OnAsym();
}

void NotationPlain::setUpData()
{
	//set up data
	_not =		_notation.getNotation();
	_asym =		!(_notation.isSymmetric());
	_leadEnd =	_notation.getLeadEnd();
	_name =		_notation.getName();		
	_splice =	_notation.getSpliceLetter();
	setNumber(_notation.getNumber());
	UpdateData(FALSE);
}

void NotationPlain::OnKillfocusName() 
{
	verify();	
}

BOOL NotationPlain::OnKillActive()
{
	if (CBCGPPropertyPage::OnKillActive())
	{
		verify();
		return TRUE;
	}

	return FALSE;
}

BOOL NotationPlain::OnSetActive()
{
	setUpData();

	_number.EnableWindow(!_fixNumber);

	verify();

	OnAsym();

	return CBCGPPropertyPage::OnSetActive();
}
