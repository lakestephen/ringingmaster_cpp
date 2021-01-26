// NewDocumentWizardPages.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "resource.h"
#include "NewDocumentWizardPages.h"
#include "NewDocumentWizard.h"
#include "NotationDlg.h"
#include "PropItem.h"
#include "SearchDlg.h"
#include "NotationResultsDlg.h"
#include "NotationExplorerDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNCREATE(NewDocumentWizardPage0, CPropertyPage)
IMPLEMENT_DYNCREATE(NewDocumentWizardPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(NewDocumentWizardPage2, CPropertyPage)
IMPLEMENT_DYNCREATE(NewDocumentWizardPage3, CPropertyPage)


NewDocumentWizardPage0::NewDocumentWizardPage0(ViewType viewType) : CPropertyPage(NewDocumentWizardPage0::IDD)
{
	//{{AFX_DATA_INIT(NewDocumentWizardPage0)
	//}}AFX_DATA_INIT
	switch (viewType)
	{
	case vt_touch_main:
	case vt_none:
		_documentType = 0;
		break;
	case vt_crib:
		_documentType = 1;
		break;
	}
}

NewDocumentWizardPage0::~NewDocumentWizardPage0()
{
}

void NewDocumentWizardPage0::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewDocumentWizardPage0)
	DDX_Radio(pDX, IDC_DOCUMENT_TYPE, _documentType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewDocumentWizardPage0, CPropertyPage)
	//{{AFX_MSG_MAP(NewDocumentWizardPage0)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage0 message handlers

BOOL NewDocumentWizardPage0::OnSetActive() 
{
	((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

LRESULT NewDocumentWizardPage0::OnWizardNext() 
{
	UpdateData();
	ViewType viewType;

	LRESULT page = 0;

	switch (_documentType)
	{
	case 0:
		 viewType = vt_touch_main;
		 page = IDD_NEW_METHOD_WIZ_1;
		 break;
	case 1:
		 viewType = vt_crib;
		 page = IDD_NEW_METHOD_WIZ_2;
		 break;
	default:
		ASSERT(FALSE);
		viewType = vt_touch_main;
		 break;
	}

	((NewDocumentWizard*)GetParent())->_trans->_viewType = viewType;	

	return page;

}



/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage1 property page

NewDocumentWizardPage1::NewDocumentWizardPage1() : CPropertyPage(NewDocumentWizardPage1::IDD)
{
	//{{AFX_DATA_INIT(NewDocumentWizardPage1)
	_name = _T("New Touch");
	_courseBased = 0;
	//}}AFX_DATA_INIT
}

NewDocumentWizardPage1::~NewDocumentWizardPage1()
{
}

void NewDocumentWizardPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewDocumentWizardPage1)
	DDX_Text(pDX, IDC_TOUCH_NAME, _name);
	DDX_Radio(pDX, IDC_COURSE_BASED, _courseBased);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewDocumentWizardPage1, CPropertyPage)
	//{{AFX_MSG_MAP(NewDocumentWizardPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage2 property page

NewDocumentWizardPage2::NewDocumentWizardPage2() : CPropertyPage(NewDocumentWizardPage2::IDD)
{
	//{{AFX_DATA_INIT(NewDocumentWizardPage2)
	//}}AFX_DATA_INIT
}

NewDocumentWizardPage2::~NewDocumentWizardPage2()
{
}

void NewDocumentWizardPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewDocumentWizardPage2)
	DDX_Control(pDX, IDC_FIND_METHOD, _find);
	DDX_Control(pDX, IDC_EXPLORE_METHOD, _explore);
	DDX_Control(pDX, IDC_DELETE_METHOD, _delete);
	DDX_Control(pDX, IDC_ADD_METHOD, _add);
	DDX_Control(pDX, IDC_LIST, _list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewDocumentWizardPage2, CPropertyPage)
	//{{AFX_MSG_MAP(NewDocumentWizardPage2)
	ON_BN_CLICKED(IDC_ADD_METHOD, OnAddMethod)
	ON_BN_CLICKED(IDC_DELETE_METHOD, OnDeleteMethod)
	ON_BN_CLICKED(IDC_FIND_METHOD, OnFindMethod)
	ON_BN_CLICKED(IDC_EXPLORE_METHOD, OnExploreMethod)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROP_BUTTON, OnEditNotation)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NewDocumentWizardPage3 property page

NewDocumentWizardPage3::NewDocumentWizardPage3() : CPropertyPage(NewDocumentWizardPage3::IDD)
{
	//{{AFX_DATA_INIT(NewDocumentWizardPage3)
	//}}AFX_DATA_INIT
}

NewDocumentWizardPage3::~NewDocumentWizardPage3()
{
}

void NewDocumentWizardPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewDocumentWizardPage3)
	DDX_Control(pDX, IDC_NUMBER, _number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewDocumentWizardPage3, CPropertyPage)
	//{{AFX_MSG_MAP(NewDocumentWizardPage3)
	ON_CBN_SELCHANGE(IDC_NUMBER, OnSelchangeNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL NewDocumentWizardPage1::OnSetActive() 
{
	((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

BOOL NewDocumentWizardPage2::OnSetActive() 
{
	_add.SetImage(IDB_NEW);
	_add.m_bDrawFocus = FALSE;
	_add.SetTooltip("Add Method");
	_add.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_delete.SetImage(IDB_DELETE);
	_delete.m_bDrawFocus = FALSE;
	_delete.SetTooltip("Delete Method");
	_delete.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_find.SetImage(IDB_FIND);
	_find.m_bDrawFocus = FALSE;
	_find.SetTooltip("Find a notation in the database");
	_find.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_explore.SetImage(IDB_EXPLORE);
	_explore.m_bDrawFocus = FALSE;
	_explore.SetTooltip("Method Explorer");
	_explore.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_list.setColumnCount(2);
	_list.setColumnHead(false);
	_list.SetFont(&app()->_fontStd);
	_list.setButtonToolTip("Edit Method");
	_list._devider[0] =250; 

	update();
	
	return CPropertyPage::OnSetActive();
}

BOOL NewDocumentWizardPage3::OnSetActive() 
{

	((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	//fill the combo box
	int limit = MAXBELLS;
	if (mainFrame()->verifyKey() == FALSE)
	{
		limit = 6; 
		if (((NewDocumentWizard*)GetParent())->_trans->_number > 6)
			((NewDocumentWizard*)GetParent())->_trans->_number = 6;
	}

	int curSel = ((NewDocumentWizard*)GetParent())->_trans->_number - 3;
	
	_number.ResetContent();
	for (int i=MINBELLS;i<=limit;i++)
	{
		_number.AddString(GlobalFunctions::numberName(i));
	}
	
	_number.SetCurSel(curSel);

	return CPropertyPage::OnSetActive();
}

void NewDocumentWizardPage2::OnAddMethod() 
{
	Notation * not = new Notation();

	not->create(false, "",((NewDocumentWizard*)GetParent())->_trans->getNumber(),true,"");
	
	NotationDlg dlg(this, *not, ((NewDocumentWizard*)GetParent())->_trans->isNumberFixed(), NULL, false);

	if (dlg.DoModal() != IDCANCEL)
	{
		//check for correct number
		if ((mainFrame()->verifyKey() == FALSE)&&(not->getNumber() > 6))
		{
			CString msg;
			msg.Format("Unregistered version limited to Minor (6): Please Register\n\n Cant add %s.", 
				not->getName(true));
			AfxMessageBox(msg);
			delete not;
		}
		else if ((((NewDocumentWizard*)GetParent())->_trans->isNumberFixed())&&
		(not->getNumber() != ((NewDocumentWizard*)GetParent())->_trans->getNumber()))
		{
			CString msg;
			msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
				not->getName(true), GlobalFunctions::numberName(((NewDocumentWizard*)GetParent())->_trans->getNumber()));
			AfxMessageBox(msg);
			delete not;
		}
		else if (!((NewDocumentWizard*)GetParent())->_trans->isNotationNameUnique(not->getName(true)))
		{
			CString msg;
			msg.Format("Not adding as %s is not a unique name", 
				not->getName(true));
			AfxMessageBox(msg);
			delete not;
		}
		else if (not->getName(false).IsEmpty())
		{
			CString msg;
			msg.Format("No Name");
			AfxMessageBox(msg);
			delete not;
		}
		else if (!((NewDocumentWizard*)GetParent())->_trans->isSpliceUnique(not->getSpliceLetter()))
		{
			CString msg;
			msg.Format("%s is not a unique shorthand", 
				not->getSpliceLetter());
			AfxMessageBox(msg);
			delete not;
		}
		else
		{
			((NewDocumentWizard*)GetParent())->_trans->_notations.Add(not);
			sortNotations();
		}
	} 	
	else
	{
		delete not;
		not = NULL;
	}	
	update();
}

void NewDocumentWizardPage2::OnDeleteMethod() 
{
	//pass to the active document
	PropItem * pItem = _list.getPropItem(_list.GetCurSel());
	if (pItem)
	{
		CString name = pItem->_col[0];

		for (int i=0;i<((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize();i++)
		{
			if (((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i)->getName(true) == name)
			{
				Notation *not = ((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i);
				((NewDocumentWizard*)GetParent())->_trans->_notations.RemoveAt(i);
				delete not;
				not = NULL;
			}
		}
	}
	update();
}

LRESULT NewDocumentWizardPage1::OnWizardNext() 
{
	UpdateData();
	((NewDocumentWizard*)GetParent())->_trans->_name = _name;
	((NewDocumentWizard*)GetParent())->_trans->_courseBased = (_courseBased == 0);
	
	return CPropertyPage::OnWizardNext();
}

void NewDocumentWizardPage2::sortNotations()
{
	//now do the sort.
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (int i=0;i<((NewDocumentWizard*)GetParent())->_trans->_notations.GetUpperBound();i++)
		{
			if (((NewDocumentWizard*)GetParent())->_trans->_notations[i]->getName() > 
				((NewDocumentWizard*)GetParent())->_trans->_notations[i+1]->getName())
			{
				changed = true;
				Notation * temp    = ((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i);
				((NewDocumentWizard*)GetParent())->_trans->_notations.SetAt(i,((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i+1));
				((NewDocumentWizard*)GetParent())->_trans->_notations.SetAt(i+1, temp);
			}
		}
	}
}


void NewDocumentWizardPage2::update()
{
	_list.deleteAllPropItems();
	//we retrive the method names.
	for (int i=0;i<((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize();i++)
	{
		_list.AddPropItem(new PropItem(PT_BUTTON,
			((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i)->getName(true),
			((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i)->getSpliceLetter()));
	}

	if (((NewDocumentWizard*)GetParent())->_trans->_viewType == vt_crib)
	{
		if (((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize() > 0)
			((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
		else
			((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK);
	}
	else
	{
		if (((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize() > 0)
			((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
		else
			((NewDocumentWizard*)GetParent())->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);
	}
}

void NewDocumentWizardPage2::OnFindMethod() 
{
	int number = 0;

	if (((NewDocumentWizard*)GetParent())->_trans->isNumberFixed())
		number = ((NewDocumentWizard*)GetParent())->_trans->getNumber();

	NotationLites results ;
	SearchDlg dlg(results, number);//for any number

	if (dlg.DoModal() == IDOK)
	{
		if (results.GetSize() > 0)
		{

			for (int i=0;i<results.GetSize();i++)
			{
				NotationLite* lite = results.GetAt(i);
				Notation * not = new Notation();
				*not = *lite;
				delete lite;
				
				//check for correct number
				if ((mainFrame()->verifyKey() == FALSE)&&(not->getNumber() > 6))
				{
					CString msg;
					msg.Format("Unregistered version limited to Minor (6): Please Register\n\n Cant add %s.", 
						not->getName(true));
					AfxMessageBox(msg);
					delete not;
				}
				else if ((((NewDocumentWizard*)GetParent())->_trans->isNumberFixed())&&
					(not->getNumber() != ((NewDocumentWizard*)GetParent())->_trans->getNumber()))
				{
					CString msg;
					msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
						not->getName(true), GlobalFunctions::numberName(((NewDocumentWizard*)GetParent())->_trans->getNumber()));
					AfxMessageBox(msg);
					delete not;
				}
				else if (!((NewDocumentWizard*)GetParent())->_trans->isNotationNameUnique(not->getName(true)))
				{
					CString msg;
					msg.Format("Not adding as %s is not a unique name", 
						not->getName(true));
					AfxMessageBox(msg);
					delete not;
				}
				else if (not->getName(false).IsEmpty())
				{
					CString msg;
					msg.Format("No Name");
					AfxMessageBox(msg);
					delete not;
				}
				else if (!((NewDocumentWizard*)GetParent())->_trans->isSpliceUnique(not->getSpliceLetter()))
				{
					CString msg;
					msg.Format("%s is not a unique shorthand", 
						not->getSpliceLetter());
					AfxMessageBox(msg);
					delete not;
				}
				else
				{
					((NewDocumentWizard*)GetParent())->_trans->_notations.Add(not);
					sortNotations();
				}
			} 	
		}
	}
	update();
}

void NewDocumentWizardPage2::OnExploreMethod() 
{
	NotationLites notations;
	NotationExplorerDlg dlg(this, notations, false, (((NewDocumentWizard*)GetParent())->_trans->isNumberFixed() == TRUE), ((NewDocumentWizard*)GetParent())->_trans->getNumber());
	if (dlg.DoModal() == IDOK)
		for (int i=0;i<notations.GetSize();i++)
		{
			NotationLite* lite = notations.GetAt(i);
		 	Notation * not = new Notation();
			*not = *lite;
			delete lite;

			//check for correct number
			if ((mainFrame()->verifyKey() == FALSE)&&(not->getNumber() > 6))
			{
				CString msg;
				msg.Format("Unregistered version limited to Minor (6): Please Register\n\n Cant add %s.", 
					not->getName(true));
				AfxMessageBox(msg);
				delete not;
			}
			else if ((((NewDocumentWizard*)GetParent())->_trans->isNumberFixed())&&
			(not->getNumber() != ((NewDocumentWizard*)GetParent())->_trans->getNumber()))
			{
				CString msg;
				msg.Format("Number of bells mismatch.\n  You can't add %s to a %s touch", 
					not->getName(true), GlobalFunctions::numberName(((NewDocumentWizard*)GetParent())->_trans->getNumber()));
				AfxMessageBox(msg);
				delete not;
			}
			else if (!((NewDocumentWizard*)GetParent())->_trans->isNotationNameUnique(not->getName(true)))
			{
				CString msg;
				msg.Format("Not adding as %s is not a unique name", 
					not->getName(true));
				AfxMessageBox(msg);
				delete not;
			}
			else if (not->getName(false).IsEmpty())
			{
				CString msg;
				msg.Format("No Name");
				AfxMessageBox(msg);
				delete not;
			}
			else if (!((NewDocumentWizard*)GetParent())->_trans->isSpliceUnique(not->getSpliceLetter()))
			{
				CString msg;
				msg.Format("%s is not a unique shorthand", 
					not->getSpliceLetter());
				AfxMessageBox(msg);
				delete not;
			}
			else
			{
				((NewDocumentWizard*)GetParent())->_trans->_notations.Add(not);
				sortNotations();
			}
		} 	

	update();	
}



void NewDocumentWizardPage3::OnSelchangeNumber() 
{
	((NewDocumentWizard*)GetParent())->_trans->_number = _number.GetCurSel() + 3;
	
}

LRESULT NewDocumentWizardPage2::OnEditNotation(WPARAM /*wParam*/, LPARAM lParam)	
{
	CWaitCursor wait;

	PropItem * pItem = (PropItem*)lParam;

	if (pItem) 
	{
		Notation* not = NULL;

		//find the notaion
		for (int i=0;i<((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize();i++)
			if (((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i)->getName(true) == pItem->_col[0])
				not = ((NewDocumentWizard*)GetParent())->_trans->_notations.GetAt(i);

		if (!not) return 0;

		NotationDlg dlg(this, *not, (((NewDocumentWizard*)GetParent())->_trans->_notations.GetSize()>1), NULL, true);

		dlg.DoModal();
		sortNotations();
		update();

	}

	return 0;	
}





LRESULT NewDocumentWizardPage2::OnWizardBack() 
{
	if (((NewDocumentWizard*)GetParent())->_trans->_viewType == vt_crib)
		return IDD_NEW_METHOD_WIZ_0;
	else
		return IDD_NEW_METHOD_WIZ_1;

//	return CPropertyPage::OnWizardBack();
}
