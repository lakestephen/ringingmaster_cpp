#include "StdAfx.h"
#include ".\Cribproplist.h"

#include "ButtonProp.h"
#include "UserMessages.h"
#include "CribDoc.h"

CribPropList::CribPropList(void)
{
}

CribPropList::~CribPropList(void)
{
}

BEGIN_MESSAGE_MAP(CribPropList, PropListBase)
	//{{AFX_MSG_MAP(CribPropList)
	ON_MESSAGE(BCG_BTN_PRESS, OnBCGBtnPress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CribPropList::initPropList()
{
	__super::initPropList();
	
	//-----------------------------------------
	//general
	//-----------------------------------------
	CBCGPProp* generalRoot = new CBCGPProp (_T("General"), ptc_generalRoot);
	AddProperty (generalRoot);
	generalRoot->SetDescription("General crib settings.");
	generalRoot->AddSubItem(new CBCGPProp (_T("Title"),	(_variant_t) "",	_T("Edit the title."), ptc_titleEdit));
	generalRoot->AddSubItem(new CBCGPProp (_T("Crib From"),	(long) 1,	_T("Set the bell to crib from."), ptc_cribFrom));
	generalRoot->Expand(TRUE);

	//-----------------------------------------
	//method
	//-----------------------------------------
	CBCGPProp* methodRoot = new CBCGPProp (_T("Method"), ptc_methodRoot);
	AddProperty (methodRoot);
	methodRoot->SetDescription("Method to crib.");
	methodRoot->AddSubItem(new ButtonProp (_T("Notation"),	(_variant_t) "",	_T("Edit the crib method."), ptc_methodItem, IDB_NOTATION, FALSE));
	methodRoot->Expand(TRUE);

	//-----------------------------------------
	//method
	//-----------------------------------------
	CBCGPProp* styleRoot = new CBCGPProp (_T("Styles"), ptc_styleRoot);
	AddProperty (styleRoot);
	styleRoot->SetDescription("Manage the styles.");
	styleRoot->AddSubItem (new ButtonProp (_T("Crib Font"), (_variant_t) "",  _T("Choose a font for the crib."), ptc_styleCribFont));
	styleRoot->AddSubItem (new ButtonProp (_T("Notes Font"), (_variant_t) "",  _T("Choose a font for the notes."), ptc_styleCribNotesFont));
	styleRoot->AddSubItem (new ButtonProp (_T("Blue Line Style"), (_variant_t) "", _T("Launch the blue line editor ."),ptc_styleBlueLine, IDB_STYLE, FALSE));
	styleRoot->Expand(FALSE);

	//-----------------------------------------
	//method
	//-----------------------------------------
	CBCGPProp* notes = new CBCGPProp (_T("Notes"), ptc_notesRoot);
	AddProperty (notes);
	notes->SetDescription("Edit notes for the crib.");

	notes->AddSubItem (new CBCGPProp (_T("Show Notes"), (_variant_t) false, _T("Set to true to show notes on the crib."), ptc_notesShow));
	notes->AddSubItem (new ButtonProp (_T("Edit Notes"), (_variant_t) "", _T("Edit the notes."), ptc_notesEdit, IDB_NOTES));
	notes->Expand(FALSE);	

}

void CribPropList::mainFrame_notifyUpdate(ViewType viewType)
{
	__super::mainFrame_notifyUpdate(viewType);

	CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();
  
	if (viewType == vt_crib)
	{            
		//-----------------------------------------
		//general
		//-----------------------------------------
		CBCGPProp* titleEdit = FindItemByData(ptc_titleEdit);
		if (titleEdit)
		{
			titleEdit->SetValue((_variant_t)pCribDoc->getTitle());
		}

		CBCGPProp* cribFrom = FindItemByData(ptc_cribFrom);
		if (cribFrom)
		{
			cribFrom->SetValue((long)pCribDoc->getCribNumber());   
			cribFrom->EnableSpinControl (TRUE, 1, pCribDoc->getNumber());
		}  

		CBCGPProp* methodItem = FindItemByData(ptc_methodItem);
		if (methodItem)
		{
			const Notation& notation = pCribDoc->getNotation();
			CString name = notation.getName(true);
			methodItem->SetName(name);
		}

		ButtonProp* styleCribFont = dynamic_cast<ButtonProp*>(FindItemByData(ptc_styleCribFont));
		if (styleCribFont) 
		{
			styleCribFont->SetValue((_variant_t)pCribDoc->getCribFont().getDisplayName());
			styleCribFont->AllowEdit(FALSE);
		}			  

		ButtonProp* styleCribNotesFont = dynamic_cast<ButtonProp*>(FindItemByData(ptc_styleCribNotesFont));
		if (styleCribNotesFont) 
		{
			styleCribNotesFont->SetValue((_variant_t)pCribDoc->getCribNotesFont().getDisplayName());
			styleCribNotesFont->AllowEdit(FALSE);
		}			  

		CBCGPProp* notesShow = FindItemByData(ptc_notesShow);
		if (notesShow) 
		{
			notesShow->SetValue((_variant_t) (pCribDoc->getShowNotes() == TRUE));	
		}	         

		CBCGPProp* notesEdit = FindItemByData(ptc_notesEdit);
		if (notesEdit) 
		{
			notesEdit->SetValue((_variant_t) pCribDoc->getNotes());	
		}	         



		//load the expand status
		loadExpandStatus();
	}	 
}

void CribPropList::saveExpandStatus()
{
	//only one instance, so do nothing 
}

void CribPropList::loadExpandStatus()
{
	//only one instance, so do nothing 
}

void CribPropList::OnPropertyChanged (CBCGPProp* pProp) const
{
	CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();

	if (pProp && pCribDoc)
	{
		switch (pProp->GetData())
		{
		case ptc_titleEdit:
			pCribDoc->setTitle(pProp->GetValue());
			break;

		case ptc_cribFrom:
			pCribDoc->setCribNumber(pProp->GetValue());
			break;  

		case ptc_notesShow:
			pCribDoc->setShowNotes((bool)pProp->GetValue()); 
			break;	  

		case ptc_notesEdit:
			pCribDoc->setNotes(pProp->GetValue());
			break;

		default:
			ASSERT(FALSE);
		case ptc_styleCribFont:
		case ptc_styleCribNotesFont:

			break;
		}
	}
}

										 
LRESULT CribPropList::OnBCGBtnPress(WPARAM /*wParam*/, LPARAM lParam)
{
	CribDoc* pCribDoc = mainFrame()->getActiveCribDoc();
	CBCGPProp* pProp = reinterpret_cast<CBCGPProp*>(lParam);

	if ((pProp)&&(pCribDoc))
	{
		switch (pProp->GetData())
		{
		case ptc_methodItem:
			pCribDoc->OnPopupEditmethod();
			break;

		case ptc_styleBlueLine:
			::PostMessage(mainFrame()->m_hWnd, WM_COMMAND, ID_TOOLS_STYLE,0);
			break;

		case ptc_styleCribFont:
			{
				FontInfo fontInfo(pCribDoc->getCribFont());
				fontInfo.editFont(this, true);
				pCribDoc->setCribFont(fontInfo);
			}
			break;				

		case ptc_styleCribNotesFont:
			{
				FontInfo fontInfo(pCribDoc->getCribNotesFont());
				fontInfo.editFont(this, true);
				pCribDoc->setCribNotesFont(fontInfo);
			}
			break;	

		case ptc_notesEdit:
			{
				CPoint point;
				GetCursorPos(&point);
				pCribDoc->editNotes(point);
			}
			break;
		}
	}	
	Invalidate();	 
	
	return 0;
}




