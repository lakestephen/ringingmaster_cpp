#include "StdAfx.h"
#include ".\touchmainproplist.h"
#include "TouchDoc.h"
#include "ButtonProp.h"
#include "UserMessages.h"
#include "TouchPropListExpanded.h"
#include "SimulatorManager.h"
#include "Row.h"

TouchMainPropList::TouchMainPropList(ViewType viewType) :
_viewType(viewType)
{
	ASSERT((_viewType == vt_touch_main)||(_viewType == vt_touch_blue_line)||(_viewType == vt_touch_simulator));
  
	simulatorManager()->addEventListener(this);
}

TouchMainPropList::~TouchMainPropList(void)
{
	simulatorManager()->removeEventListener(this);
}


BEGIN_MESSAGE_MAP(TouchMainPropList, PropListBase)
	//{{AFX_MSG_MAP(TouchMainPropList)
	ON_MESSAGE(BCG_BTN_PRESS, OnBCGBtnPress)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_TOUCH_MAIN_DELETE, OnTouchMainDelete)
	ON_COMMAND(ID_TOUCH_MAIN_ADD, OnTouchMainAdd)
	ON_COMMAND(ID_TOUCH_MAIN_CRIB, OnTouchMainCrib)
	ON_COMMAND(ID_ADD_EXPLORER_NOTATION, OnAddExplorerNotation)
	ON_COMMAND(ID_ADD_FIND_NOTATION, OnAddFindNotation)	
	ON_UPDATE_COMMAND_UI(ID_TOUCH_MAIN_DELETE, OnUpdateTouchMainDelete)
	ON_UPDATE_COMMAND_UI(ID_TOUCH_MAIN_ADD, OnUpdateTouchMainAdd)
	ON_UPDATE_COMMAND_UI(ID_TOUCH_MAIN_CRIB, OnUpdateTouchMainCrib)
END_MESSAGE_MAP()

void TouchMainPropList::initPropList()
{
	__super::initPropList();
	
	//-----------------------------------------
	//setup
	//-----------------------------------------
	CBCGPProp* setup = new CBCGPProp (_T("Setup"), ptt_setupRoot);
	AddProperty (setup);
	setup->SetDescription("The main settings for the touch.");

	if (_viewType != vt_touch_simulator)
	{
		setup->AddSubItem(new CBCGPProp (_T("Title"),			(_variant_t) "",	_T("Edit the name of the touch."), ptt_title));
		setup->AddSubItem(new CBCGPProp (_T("Author"),			(_variant_t) "",	_T("Edit the author of the touch."),ptt_author));
	}
	setup->AddSubItem(new CBCGPProp (_T("Number Of bells"), (_variant_t) "",	_T("You can alter the number of bells once the touch has been created, but methods and music detection must be removed."), ptt_numOfBells));
	if (_viewType != vt_touch_simulator)
	{
		setup->AddSubItem( new CBCGPProp (_T("Call From"),		(long)4,			_T("Set the bell to call from. This is the bell that the call positions refer to."), ptt_callFrom));
	}
	if (_viewType == vt_touch_simulator)
	{
		setup->AddSubItem( new CBCGPProp (_T("First Simulated Bell"),		(long)4,			_T("Set the first simulated bell. This is the bell that will map to Bell Input 1."), ptt_firstSimulatedBell));
		setup->AddSubItem( new CBCGPProp (_T("Cover Bell Count"),		(long)4,			_T("Set the number of cover bells."), ptt_coverBellCount));
	}
	setup->AddSubItem( new CBCGPProp (_T("Active Method"),	(_variant_t) "",	_T("Choose the method that you wish to use for your touch or set Spliced."), ptt_method));

	if (_viewType != vt_touch_simulator)
	{
		setup->AddSubItem( new CBCGPProp (_T("Call Type"),		(_variant_t) "",	_T("Choose between Course Based (WBMH style) and Lead Based (-ps-p- style)."), ptt_callType));
		setup->AddSubItem( new CBCGPProp (_T("Plain Lead Token"), (_variant_t) "",	_T("Choose a letter or sequence of letters that will represent a plain lead."), ptt_plainLeadToken));
	}


	setup->Expand(FALSE);

	//-----------------------------------------
	// advanced setup
	//-----------------------------------------

	if ((_viewType == vt_touch_main)||(_viewType == vt_touch_blue_line))
	{    
		CBCGPProp* setupAdvanced = new CBCGPProp (_T("Advanced Setup"), ptt_setupAdvancedRoot);
		AddProperty (setupAdvanced);
		setupAdvanced->SetDescription("The advanced settings for the touch.");

		if (_viewType != vt_touch_simulator)
		{
			setupAdvanced->AddSubItem( new CBCGPProp (_T("Wrap Calls"), (_variant_t) true,	    _T("Set to false to prevent the calls being made in a second and subsequent parts when all the calls have been made. Normally set True."), ptt_wrapCalls));
			setupAdvanced->AddSubItem(new CBCGPProp (_T("Checking Type"),	(_variant_t) "",	_T(""), ptt_checkingType));
			setupAdvanced->AddSubItem(new CBCGPProp (_T("Block Count"),		(long)2 ,			_T(""), ptt_blockCount));
		}

		setupAdvanced->Expand(FALSE);
	}

	//-----------------------------------------
	//start
	//-----------------------------------------
	CBCGPProp* start = new CBCGPProp (_T("Start"), ptt_startRoot);
	AddProperty (start);
	start->SetDescription("All the settings to do with starting a touch.");

	start->AddSubItem (new ButtonProp (_T("Start With Change"), (_variant_t) "",	_T("Enter the change to start the touch with. This is usually rounds."),ptt_startWithChange, IDB_RESET));
	
	if (globalDataManager()->isStartAtRowZero())
		start->AddSubItem (new ButtonProp (_T("Start At Row"),	(long) 0,				_T("Enter the row of a plain course to start on. Set 2  for a snap start. Usually set to 0."),ptt_startAtRow, IDB_VIS_ROW));
	else
		start->AddSubItem (new ButtonProp (_T("Start At Row"),	(long) 1,				_T("Enter the row of a plain course to start on. Set 3  for a snap start. Usually set to 1."),ptt_startAtRow, IDB_VIS_ROW));

	start->AddSubItem (new CBCGPProp (_T("Start At Stroke"), (_variant_t) "",		_T("Set the stroke of the first change (Not the first Row.)"),ptt_startAtStroke));
	start->AddSubItem (new ButtonProp (_T("Start Notation"), (_variant_t) "",		_T("Enter some notation to start the touch with. This will form the first lead of the first part. Usually blank."),ptt_startNotation, IDB_NOTATION));
	start->Expand(FALSE);

	//-----------------------------------------
	//Termination
	//-----------------------------------------
	CBCGPProp* termination = new CBCGPProp (_T("Termination"), ptt_endRoot);
	AddProperty (termination);
	termination->SetDescription("All the settings to do with terminating a touch");

	termination->AddSubItem (new ButtonProp (_T("End With Change"), (_variant_t) "",_T("Enter the change to terminate the touch with. This is usually rounds."), ptt_endWithChange, IDB_RESET));
	if (_viewType != vt_touch_simulator)
	{
		termination->AddSubItem (new CBCGPProp (_T("Row Limit"), (_variant_t) "",_T("Enter the number of rows to terminate the touch at if the end change is not reached."), ptt_endRowLimit));
		termination->AddSubItem (new ButtonProp (_T("Lead Limit"), (_variant_t) "",_T("Enter the number of leads to terminate the touch at if the end change is not reached."), ptt_endLeadLimit, IDB_NO_LIMIT));
		termination->AddSubItem (new ButtonProp (_T("Part Limit"), (_variant_t) "",_T("Enter the number of parts to terminate the touch at if the end change is not reached."), ptt_endPartLimit, IDB_NO_LIMIT));
		termination->AddSubItem (new ButtonProp (_T("Circular Touch Limit"), (_variant_t) "",_T("Enter the number of circular course lengths to terminate the touch. Default is 2."),ptt_endCircleLimit, IDB_NO_LIMIT));
	}
	termination->Expand(FALSE);

	//-----------------------------------------
	//Methods 
	//-----------------------------------------

	CBCGPProp* methods = new CBCGPProp (_T("Methods"), ptt_methodRoot);
	AddProperty (methods);
	methods->SetDescription("Manage the methods for the touch.");
	methods->Expand(FALSE);

	//-----------------------------------------
	//Definitions
	//-----------------------------------------

	if (_viewType != vt_touch_simulator)
	{
		CBCGPProp* definitions = new CBCGPProp (_T("Definitions"), ptt_definitionRoot);
		AddProperty (definitions);
		definitions->SetDescription("Manage the touches definitions. Definitions allow you to group and reuse a sequence of calls / splices. ");
		definitions->Expand(FALSE);
	}	
	//-----------------------------------------
	//Music
	//-----------------------------------------

	if (_viewType != vt_touch_simulator)
	{
		CBCGPProp* music = new CBCGPProp (_T("Music"), ptt_musicRoot);
		AddProperty (music);
		music->SetDescription("Manage the touches music analysis. Music allows you define your own checks for music." );
		music->Expand(FALSE);
	}

	//-----------------------------------------
	//Style
	//-----------------------------------------

	if ((_viewType == vt_touch_main)||(_viewType == vt_touch_blue_line))
	{
		CBCGPProp* style = new CBCGPProp (_T("Style"), ptt_styleRoot);
		AddProperty (style);
		style->SetDescription("Manage the styles. ");;

		if (_viewType == vt_touch_main)
		{
			style->AddSubItem (new ButtonProp (_T("Title Font"), (_variant_t) "", _T("Choose a font for the title."),ptt_styleTouchFontTitle));
			style->AddSubItem (new ButtonProp (_T("Author Font"), (_variant_t) "", _T("Choose a font for the author."),ptt_styleTouchFontAuthor));
			style->AddSubItem (new ButtonProp (_T("Main Font"), (_variant_t) "", _T("Choose a font for the main part of the touch."),ptt_styleTouchFontMain));
			style->AddSubItem (new ButtonProp (_T("Notes Font"), (_variant_t) "",  _T("Choose a font for the notes."), ptt_styleTouchFontNotes));



			//some color??
			for (int i=0;i<tsc_highest_val;i++) 
			{
				CBCGPColorProp* pColorProp = new CBCGPColorProp    (_T(TouchStyle::getColorName((TouchStyleColorType)i) + " Colour"), 
																	RGB (0, 0, 0),
																	NULL, 
																	_T("Choose Color for " + TouchStyle::getColorName((TouchStyleColorType)i)),
																	ptt_styleTouchColorItem + i);
				pColorProp->EnableOtherButton (_T("Other..."));
				style->AddSubItem (pColorProp);
			}
		}
		else if (_viewType == vt_touch_blue_line)
		{
			style->AddSubItem (new ButtonProp (_T("Blue Line Style"), (_variant_t) "", _T("Launch the blue line editor ."),ptt_styleBlueLine, IDB_STYLE, FALSE));
		}

		style->Expand(FALSE);
	}


	//-----------------------------------------
	//notes
	//-----------------------------------------
	if (_viewType == vt_touch_main)
	{
		CBCGPProp* notes = new CBCGPProp (_T("Notes"), ptt_notesRoot);
		AddProperty (notes);
		notes->SetDescription("Edit notes for the touch.");

		notes->AddSubItem (new CBCGPProp (_T("Show Notes"), (_variant_t) false, _T("Set to true to show notes on the touch."), ptt_notesShow));
		notes->AddSubItem (new ButtonProp (_T("Edit Notes"), (_variant_t) "", _T("Edit the notes."), ptt_notesEdit, IDB_NOTES));
		notes->Expand(FALSE);	
	}

	
}

void TouchMainPropList::updateMethodList(TouchDoc * pTouchDoc)
{
	ASSERT(pTouchDoc);

	//first check if we have the correct number of items.
	CBCGPProp* methodRoot = FindItemByData(ptt_methodRoot);

	ASSERT(methodRoot);

	if (methodRoot)
	{
		//do we have too many?
		if ( pTouchDoc->getNotations().GetSize() < methodRoot->GetSubItemsCount())
		{
			//as there is not a remove item in BCG, we have to rebuild compleatly
			RemoveAll();
			//now rebuild
			initPropList();
			//get the methodRoot again as it may have changed 
			methodRoot = FindItemByData(ptt_methodRoot);
		}

		int index = methodRoot->GetSubItemsCount() + ptt_methodItem;
		while ( pTouchDoc->getNotations().GetSize() > methodRoot->GetSubItemsCount())
		{
			//we have had one added
			CString str;
			str.Format("<dummy %d>", index);
			methodRoot->AddSubItem (new ButtonProp( str, (_variant_t) "", str, index, IDB_NOTATION));
			index ++;
		}

		//now update the items
		for (int i=0;i<	pTouchDoc->getNotations().GetSize();i++)
		{												   
			CBCGPProp* item = FindItemByData(ptt_methodItem + i);
			ASSERT(item);

			if (item)
			{
				Notation* notation = pTouchDoc->getNotations().GetAt(i);
				
				//name
				item->SetName(notation->getName(false, true));

				//options
				item->RemoveAllOptions();
				if (pTouchDoc->getNotations().GetSize() > 1)
					item->AddOption(Inactive);
				item->AddOption(Active);
				if (pTouchDoc->getNotations().GetSize() > 1)
					item->AddOption(Spliced);

				//information box,and selection
				if (pTouchDoc->isSpliced())
				{
					item->SetDescription("Spliced - all methods available for use");
					item->SetValue((_variant_t)Spliced);
				}
				else 
				{
					if (pTouchDoc->getActiveNotation() == pTouchDoc->getNotations().GetAt(i))
					{
						item->SetDescription("Active method - all calls in the touch relate to " + pTouchDoc->getNotations().GetAt(i)->getName(true) + ".");
						item->SetValue((_variant_t)Active);
					}
					else
					{
						item->SetDescription("Inactive method - not used.");
						item->SetValue((_variant_t)Inactive);
					}
				}    

				item->AllowEdit(FALSE);
			}   
		}   
		
	}           
}

void TouchMainPropList::updateDefinitionsList(TouchDoc * pTouchDoc)
{
	ASSERT(pTouchDoc);

	//first check if we have the correct number of items.
	CBCGPProp* definitionRoot = FindItemByData(ptt_definitionRoot);

	ASSERT(definitionRoot);

	if (definitionRoot)
	{
		//do we have too many?
		if ( pTouchDoc->getDefinitions().GetSize() < definitionRoot->GetSubItemsCount())
		{
			//as there is not a remove item in BCG, we have to rebuild compleatly
			RemoveAll();
			//now rebuild
			initPropList();
			//get the definitionRoot again as it may have 
			definitionRoot = FindItemByData(ptt_definitionRoot);

			ASSERT(definitionRoot);
		}

		int index = definitionRoot->GetSubItemsCount() + ptt_definitionItem;
		while ( pTouchDoc->getDefinitions().GetSize() > definitionRoot->GetSubItemsCount())
		{
			CString str;
			str.Format("<dummy %d>", index);
			definitionRoot->AddSubItem (new ButtonProp( str, (_variant_t) "", str, index));
			index ++;
		}

		//now update the items
		for (int i=0;i<	pTouchDoc->getDefinitions().GetSize();i++)
		{												   
			CBCGPProp* item = FindItemByData(ptt_definitionItem + i);
			ASSERT(item);

			if (item)
			{
				Definition* definition = pTouchDoc->getDefinitions().GetAt(i);
				
				//name
				item->SetName(definition->getName(true));

				//information box,and selection			 
				item->SetDescription((pTouchDoc->getDefinitions().GetAt(i)->getType()==dt_call)?
					"Call type - can't be used for splice definitions":
					"Splice type - can't be used for call definitions");
				item->SetValue((_variant_t)pTouchDoc->getDefinitions().GetAt(i)->getDefinition());
			}   
		}   
	}           
}

void TouchMainPropList::updateMusicList(TouchDoc * pTouchDoc)
{
	ASSERT(pTouchDoc);

	//first check if we have the correct number of items.
	CBCGPProp* musicRoot = FindItemByData(ptt_musicRoot);

	ASSERT(musicRoot);

	if (musicRoot)
	{
		//do we have too many?
		int displayableSize = 0;
		for (int i=0;i<pTouchDoc->getMusicRules().GetSize();i++)
		{
			if (pTouchDoc->getMusicRules().GetAt(i)->getType() == MRT_USER) 
				displayableSize++;    
		}

		if ( displayableSize < musicRoot->GetSubItemsCount())
		{
			//as there is not a remove item in BCG, we have to rebuild compleatly
			RemoveAll();
			//now rebuild
			initPropList();
			//get the musicRoot again as it may have 
			musicRoot = FindItemByData(ptt_musicRoot);

			ASSERT(musicRoot);
		}

		int index = musicRoot->GetSubItemsCount() + ptt_musicItem;
		while (displayableSize > musicRoot->GetSubItemsCount())
		{
			CString str;
			str.Format("<dummy %d>", index);
			musicRoot->AddSubItem (new ButtonProp( str, (_variant_t) "", str, index));
			index ++;
		}

		//now update the items
		int j=0;
		for (int i=0;i<	pTouchDoc->getMusicRules().GetSize();i++)
		{												   
			MusicRule* music = pTouchDoc->getMusicRules().GetAt(i);
			ASSERT(music);

			if (music && music->getType() == MRT_USER)
			{
				CBCGPProp* item = FindItemByData(ptt_musicItem + j);
				ASSERT(item);

				if (item)
				{
					
					//name
					item->SetName(music->getName());
												 
					//information box,and selection			 
					item->SetDescription(music->getDisplayString());
					item->SetValue((_variant_t)music->getRule());
					j++	;
				}   
			}
		}   
	}           
}

void TouchMainPropList::simulatorManager_notifyProfileChange()
{
	if (_viewType == vt_touch_simulator && app()->getViewType() == vt_touch_simulator)
		mainFrame_notifyUpdate(vt_touch_simulator);
}

void TouchMainPropList::mainFrame_notifyUpdate(ViewType viewType)
{
	__super::mainFrame_notifyUpdate(viewType);
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if (((viewType == vt_touch_main)||(viewType == vt_touch_blue_line)||(viewType == vt_touch_simulator))&&(pTouchDoc))
	{
		//-----------------------------------------
		//deal with the dynamic lists
		//-----------------------------------------
		updateMethodList(pTouchDoc);
		if (_viewType != vt_touch_simulator)
		{
			updateDefinitionsList(pTouchDoc);
			updateMusicList(pTouchDoc);
		}

		//-----------------------------------------
		//setup
		//-----------------------------------------
		CBCGPProp* title = FindItemByData(ptt_title);
		if (title)
			title->SetValue((_variant_t)pTouchDoc->getName());

		CBCGPProp* author = FindItemByData(ptt_author);
		if (author)
			author->SetValue((_variant_t)pTouchDoc->getAuthor());

		CBCGPProp* noOfBells = FindItemByData(ptt_numOfBells);
		if (noOfBells)
		{
			noOfBells->RemoveAllOptions();
			CString selected;
			for (int i=MINBELLS;i<MAXBELLS;i++)
			{
				CString str;
				str.Format("%s (%d)",GlobalFunctions::numberName(i),i);
				
				if (i==pTouchDoc->getNumber())
					selected = str;

				noOfBells->AddOption(str);
			}
			noOfBells->SetValue((_variant_t)selected);
			noOfBells->AllowEdit(FALSE);
		}        

		CBCGPProp* callFrom = FindItemByData(ptt_callFrom);
		if (callFrom)
		{
			callFrom->SetValue((long)pTouchDoc->getCallFrom());   
			callFrom->EnableSpinControl (TRUE, 1, pTouchDoc->getNumber());
		}  
		
		CBCGPProp* firstSimulatedBell = FindItemByData(ptt_firstSimulatedBell);
		if (firstSimulatedBell)
		{
			firstSimulatedBell->SetValue((long)(simulatorManager()->getFirstSimulatedBell()+1));
			firstSimulatedBell->EnableSpinControl (TRUE, 1, simulatorManager()->getNumber());
		}  

		CBCGPProp* coverBellCount = FindItemByData(ptt_coverBellCount);
		if (firstSimulatedBell)
		{
			coverBellCount->SetValue((long)simulatorManager()->getCoverBellCount());
			coverBellCount->EnableSpinControl (TRUE, 0, MAXBELLS);
		}  

		CBCGPProp* method = FindItemByData(ptt_method);
		if (method)
		{
			method->RemoveAllOptions();

			if (pTouchDoc->getNotations().GetSize() > 1)
				method->AddOption(Spliced);

			for (int i=0;i<pTouchDoc->getNotations().GetSize();i++)
				method->AddOption(pTouchDoc->getNotations().GetAt(i)->getName(true));

			method->SetValue((_variant_t)pTouchDoc->getActiveNotationString());   

			method->AllowEdit (FALSE);
		}   

		CBCGPProp* callType = FindItemByData(ptt_callType);
		if (callType)
		{
			callType->AddOption (LeadBased);
			callType->AddOption (CourseBased);
			callType->SetValue((_variant_t)(pTouchDoc->isCourseBased()?CourseBased:LeadBased));	
			callType->AllowEdit(FALSE);
		}

		CBCGPProp* plainLeadToken = FindItemByData(ptt_plainLeadToken);
		if (plainLeadToken)	  //to2do dynamically remove this when bcg allows removal
		{
			plainLeadToken->SetValue((_variant_t)pTouchDoc->getPlainLeadToken());
			plainLeadToken->Enable (!pTouchDoc->isCourseBased());
		}	

		CBCGPProp* wrapCalls = FindItemByData(ptt_wrapCalls);
		if (wrapCalls) 
		{
			wrapCalls->SetValue((_variant_t) (pTouchDoc->getWrapCalls() == TRUE));	
		}

		CBCGPProp* checkingType = FindItemByData(ptt_checkingType);
		if (checkingType)   	  //to2do dynamically remove this when bcg allows removal
		{
			checkingType->AddOption (SingleBlock);
			checkingType->AddOption (MultiBlock);

			checkingType->SetValue((_variant_t)(pTouchDoc->isMultiBlock()?MultiBlock:SingleBlock));
			checkingType->AllowEdit(FALSE);

			checkingType->Enable(pTouchDoc->getNumber()<=8);

			checkingType->SetDescription((pTouchDoc->getNumber()<=8)?
				"Choose from single block or multi block . In multi block the end change can appear several times.":
					"Multi Block not available on touches over 8 bells.");
		}		

		CBCGPProp* blockCount = FindItemByData(ptt_blockCount);
		if (blockCount)		//to2do dynamically remove this when bcg allows removal
		{
			blockCount->SetValue((long)pTouchDoc->getBlockCount());   
			blockCount->EnableSpinControl (TRUE, 1, 10000);

			blockCount->Enable(pTouchDoc->getNumber()<=8 && pTouchDoc->getBlockCount() > 1);

			blockCount->SetDescription((pTouchDoc->getNumber()<=8)?
				"Set the number of times that the end change (usually rounds) appears before the touch terminates.":
					"Multi Block not available on touches over 8 bells.");			
		}  


		ButtonProp* startWithChange = dynamic_cast<ButtonProp*>(FindItemByData(ptt_startWithChange));
		if (startWithChange) 
		{
			Row row(pTouchDoc->getNumber()); //check for rounds
			if ((pTouchDoc->getStartRow() == row.getAsText())||
				(pTouchDoc->getStartRow().IsEmpty()))
			{
				startWithChange->SetValue("Rounds");
				startWithChange->setHasButton(FALSE);
			}
			else
			{
				startWithChange->SetValue((_variant_t)pTouchDoc->getStartRow());
				startWithChange->setHasButton(TRUE);
			}
		}

		CBCGPProp* startAtRow = FindItemByData(ptt_startAtRow);
		if (startAtRow) 
		{
			startAtRow->EnableSpinControl (TRUE, (globalDataManager()->isStartAtRowZero()?0:1), pTouchDoc->getFirstLeadRowCount() - 1 + (globalDataManager()->isStartAtRowZero()?0:1));
			startAtRow->SetValue((long)pTouchDoc->getStartAtRow() + (globalDataManager()->isStartAtRowZero()?0:1));
		}
					  
		CBCGPProp* startAtStroke = FindItemByData(ptt_startAtStroke);
		if (startAtStroke) 
		{
			startAtStroke->AddOption(Handstroke);
			startAtStroke->AddOption(Backstroke);
			startAtStroke->SetValue((_variant_t)((pTouchDoc->getStartStroke() == hand)?Handstroke:Backstroke));	
			startAtStroke->AllowEdit(FALSE);
		}				

		CBCGPProp* startNotation = FindItemByData(ptt_startNotation);
		if (startNotation) 
		{
			startNotation->SetValue((_variant_t) pTouchDoc->getStartNotation().getNotation());	
		}	

		ButtonProp* endWithChange = dynamic_cast<ButtonProp*>(FindItemByData(ptt_endWithChange));
		if (endWithChange) 
		{
			Row row(pTouchDoc->getNumber()); //check for rounds
			if ((pTouchDoc->getEndRow() == row.getAsText())||
				(pTouchDoc->getEndRow().IsEmpty()))
			{
				endWithChange->SetValue("Rounds");
				endWithChange->setHasButton(FALSE);
			}
			else
			{
				endWithChange->SetValue((_variant_t)pTouchDoc->getEndRow());
				endWithChange->setHasButton(TRUE);
			}
		}

		CBCGPProp* endRowLimit = FindItemByData(ptt_endRowLimit);
		if (endRowLimit) 
		{
			CString str;
			str.Format("%d",pTouchDoc->getRowCountMax());
			endRowLimit->SetValue((_variant_t) str);	
		}	

		ButtonProp* endLeadLimit = dynamic_cast<ButtonProp*>(FindItemByData(ptt_endLeadLimit));
		if (endLeadLimit) 
		{
			CString str;
			if (pTouchDoc->getLeadCountMax() > 0)
			{
				str.Format("%d",pTouchDoc->getLeadCountMax());
				endLeadLimit->setHasButton(TRUE);
			}
			else 
			{
				str = "No Limit";
				endLeadLimit->setHasButton(FALSE);
			}

			endLeadLimit->SetValue((_variant_t)str);   
		}

		ButtonProp* endPartLimit = dynamic_cast<ButtonProp*>(FindItemByData(ptt_endPartLimit));
		if (endPartLimit) 
		{
			CString str;
			if (pTouchDoc->getPartCountMax() > 0)
			{
				str.Format("%d",pTouchDoc->getPartCountMax());
				endPartLimit->setHasButton(TRUE);
			}
			else 
			{
				str = "No Limit";
				endPartLimit->setHasButton(FALSE);
			}

			endPartLimit->SetValue((_variant_t)str);   
		}

		ButtonProp* endCircleLimit = dynamic_cast<ButtonProp*>(FindItemByData(ptt_endCircleLimit));
		if (endCircleLimit) 
		{
			CString str;
			if (pTouchDoc->getCircularTouchMax() > 0)
			{
				str.Format("%d",pTouchDoc->getCircularTouchMax());
				endCircleLimit->setHasButton(TRUE);
			}
			else 
			{
				str = "No Limit";
				endCircleLimit->setHasButton(FALSE);
			}

			endCircleLimit->SetValue((_variant_t)str);   
		}
				   
		CBCGPProp* notesShow = FindItemByData(ptt_notesShow);
		if (notesShow) 
		{
			notesShow->SetValue((_variant_t) (pTouchDoc->isShowNotes() == TRUE));	
		}	         

		CBCGPProp* notesEdit = FindItemByData(ptt_notesEdit);
		if (notesEdit) 
		{
			notesEdit->SetValue((_variant_t) pTouchDoc->getNotes());	
		}	         

		//font 
		ButtonProp* styleTouchFontTitle = dynamic_cast<ButtonProp*>(FindItemByData(ptt_styleTouchFontTitle));
		if (styleTouchFontTitle) 
		{
			styleTouchFontTitle->SetValue((_variant_t)pTouchDoc->getFontTitle().getDisplayName());
			styleTouchFontTitle->AllowEdit(FALSE);
		}	        
		
		ButtonProp* styleTouchFontAuthor = dynamic_cast<ButtonProp*>(FindItemByData(ptt_styleTouchFontAuthor));
		if (styleTouchFontAuthor) 
		{
			styleTouchFontAuthor->SetValue((_variant_t)pTouchDoc->getFontAuthor().getDisplayName());
			styleTouchFontAuthor->AllowEdit(FALSE);
		}	
			 
		ButtonProp* styleTouchFontMain = dynamic_cast<ButtonProp*>(FindItemByData(ptt_styleTouchFontMain));
		if (styleTouchFontMain) 
		{
			styleTouchFontMain->SetValue((_variant_t)pTouchDoc->getFontMain().getDisplayName());
			styleTouchFontMain->AllowEdit(FALSE);
		}	        

		ButtonProp* styleTouchFontNotes = dynamic_cast<ButtonProp*>(FindItemByData(ptt_styleTouchFontNotes));
		if (styleTouchFontNotes) 
		{
			styleTouchFontNotes->SetValue((_variant_t)pTouchDoc->getFontNotes().getDisplayName());
			styleTouchFontNotes->AllowEdit(FALSE);
		}	         

		//touch colors
		for (int i=0;i<tsc_highest_val;i++) 
		{								 
			CBCGPColorProp* color = (CBCGPColorProp*)FindItemByData(ptt_styleTouchColorItem+i);
			if (color != NULL)
			{
				color->SetColor(pTouchDoc->getStyleColor((TouchStyleColorType)i));
			}
		}

		//load the expand status
		loadExpandStatus();
	}
}

void TouchMainPropList::saveExpandStatus()
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	if (pTouchDoc)
	{		 
		TouchPropListExpanded& touchPropListExpanded = pTouchDoc->getTouchPropListExpanded();

		if (touchPropListExpanded._loaded == TRUE)
		{
		
			CBCGPProp* root = FindItemByData(ptt_setupRoot);
			if (root)
				touchPropListExpanded._mainSetup = root->IsExpanded();

			root = FindItemByData(ptt_setupAdvancedRoot);
			if (root)
				touchPropListExpanded._mainSetupAdvanced = root->IsExpanded();

			root = FindItemByData(ptt_startRoot);
			if (root)
				touchPropListExpanded._mainStart = root->IsExpanded();
			
			root = FindItemByData(ptt_endRoot);
			if (root)
				touchPropListExpanded._mainEnd = root->IsExpanded();

			root = FindItemByData(ptt_methodRoot);
			if (root)
				touchPropListExpanded._mainMethod = root->IsExpanded();
			
			root = FindItemByData(ptt_definitionRoot);
			if (root)
				touchPropListExpanded._mainDefinition = root->IsExpanded();

			root = FindItemByData(ptt_musicRoot);
			if (root)
				touchPropListExpanded._mainMusic = root->IsExpanded();

			root = FindItemByData(ptt_styleRoot);
			if (root)
				touchPropListExpanded._mainStyle = root->IsExpanded();

			root = FindItemByData(ptt_notesRoot);
			if (root)
				touchPropListExpanded._mainNotes= root->IsExpanded();

			touchPropListExpanded._scrollPos = m_nVertScrollOffset;
			touchPropListExpanded._columnPos = m_nLeftColumnWidth;

			CBCGPProp* focusItem = GetCurSel();
			if (focusItem)
				touchPropListExpanded._focusType = static_cast<PropertyTypeTouch>(focusItem->GetData());
		}
	}    
}

void TouchMainPropList::loadExpandStatus()
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	if (pTouchDoc)
	{		 
		TouchPropListExpanded& touchPropListExpanded = pTouchDoc->getTouchPropListExpanded();
		
		CBCGPProp* root = FindItemByData(ptt_setupRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainSetup);

		root = FindItemByData(ptt_setupAdvancedRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainSetupAdvanced);

		root = FindItemByData(ptt_startRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainStart );
		
		root = FindItemByData(ptt_endRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainEnd );

		root = FindItemByData(ptt_methodRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainMethod );
		
		root = FindItemByData(ptt_definitionRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainDefinition );

		root = FindItemByData(ptt_musicRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainMusic);

		root = FindItemByData(ptt_styleRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainStyle );

		root = FindItemByData(ptt_notesRoot);
		if (root)
			root->Expand(touchPropListExpanded._mainNotes );

		if (touchPropListExpanded._columnPos > 0)
			m_nLeftColumnWidth = touchPropListExpanded._columnPos;

		__super::OnVScroll(SB_THUMBTRACK, touchPropListExpanded._scrollPos, GetScrollBarCtrl(SB_VERT));

		CBCGPProp* focusItem = FindItemByData(touchPropListExpanded._focusType);
		if (focusItem)
			SetCurSel(focusItem);

		touchPropListExpanded._loaded = TRUE;
	}  
}

void TouchMainPropList::OnPropertyChanged (CBCGPProp* pProp) const
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if ((pProp)&&(pTouchDoc))
	{
		if (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize))
		{
			//we are a method item
			if (Spliced == CString(pProp->GetValue()))
				pTouchDoc->setActiveNotation(Spliced);
			else if (Active == CString(pProp->GetValue()))
			{
				//go through the notations to find the one we want
				for (int i=0;i<pTouchDoc->getNotations().GetSize();i++)
				{
					if (pTouchDoc->getNotations().GetAt(i)->getName(false, true) == pProp->GetName())
					{
						pTouchDoc->setActiveNotation(pTouchDoc->getNotations().GetAt(i)->getName(true));
						break;
					}
				}
			}
			else if (Inactive == CString(pProp->GetValue()))
			{
				//go through the notations to find the one we want
				pTouchDoc->setActiveNotation("");
			}
		}
		else if (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem+ TypeGapSize))
		{
			pTouchDoc->editMusic(pProp->GetName(), pProp->GetDescription(), pProp->GetValue());
		}
		else if (pProp->GetData() >= ptt_styleTouchColorItem && pProp->GetData() < (ptt_styleTouchColorItem+ TypeGapSize))
		{
			pTouchDoc->setStyleColorIndirect((TouchStyleColorType)(pProp->GetData() - ptt_styleTouchColorItem), ((CBCGPColorProp*)pProp)->GetColor());
		}
		else
		{    
			switch (pProp->GetData())
			{
			case ptt_title:
				pTouchDoc->setName(pProp->GetValue());
				break;

			case ptt_author:
				pTouchDoc->setAuthor(pProp->GetValue());
				break;

			case ptt_numOfBells:
				{
					CString val(pProp->GetValue());
					CString str;
					for (int i=MINBELLS;i<MAXBELLS;i++)
					{
						str.Format("%s (%d)",GlobalFunctions::numberName(i),i);
						if (str == val)
							pTouchDoc->setNumberIndirect(i);
					}                             					
				}
				break;

			case ptt_callFrom:
				pTouchDoc->setCallFrom(pProp->GetValue());
				break;

			case ptt_firstSimulatedBell:
				simulatorManager()->setFirstSimulatedBell(((int)pProp->GetValue())-1);
				break;     

			case ptt_coverBellCount:
				simulatorManager()->setCoverBellCount(((int)pProp->GetValue()));
				break;     

			case ptt_method:
				pTouchDoc->setActiveNotation(pProp->GetValue());
				break;

			case ptt_callType:
				pTouchDoc->setCourseBased(CourseBased == CString(pProp->GetValue()));
				break;

			case ptt_plainLeadToken:
				pTouchDoc->setPlainLeadLetter(pProp->GetValue());
				break;

			case ptt_wrapCalls:
				pTouchDoc->setWrapCalls((bool)pProp->GetValue());
				break;	  

			case ptt_checkingType:
				pTouchDoc->setBlockCount((SingleBlock == CString(pProp->GetValue()))?1:2);
				break;

			case ptt_blockCount:
				pTouchDoc->setBlockCount(pProp->GetValue());
				break;

			case ptt_startWithChange:
				pTouchDoc->setStartWithChange(pProp->GetValue());
				break;

			case ptt_startAtRow:
				pTouchDoc->setStartAtRow(((int)pProp->GetValue()) - (globalDataManager()->isStartAtRowZero()?0:1));
				break;
		   
			case ptt_startAtStroke:
				pTouchDoc->OnSetStartStroke((Handstroke == CString(pProp->GetValue())?hand: back));
				break;
			 
			case ptt_startNotation:
				pTouchDoc->setStartNotation(pProp->GetValue());
				break;

			case ptt_endWithChange:
				pTouchDoc->setEndWithChange(pProp->GetValue());
				break;

			case ptt_endRowLimit:
				pTouchDoc->setRowCount(pProp->GetValue());
				break;

			case ptt_endLeadLimit:
				pTouchDoc->setLeadCount(pProp->GetValue());
				break;

			case ptt_endPartLimit:
				pTouchDoc->setPartCount(pProp->GetValue());
				break;

			case ptt_endCircleLimit:
				pTouchDoc->setCircularTouchCount(pProp->GetValue());
				break;

			case ptt_notesShow:
				pTouchDoc->setShowNotes((bool)pProp->GetValue());
				break;	  

			case ptt_notesEdit:
				pTouchDoc->setNotes(pProp->GetValue());
				break;

			default:
				ASSERT(FALSE);
			case ptt_styleTouchFontMain:
			case ptt_styleTouchFontNotes:
			case ptt_styleTouchFontTitle:
			case ptt_styleTouchFontAuthor:
				break;
			}
		}
	}
}

										 
LRESULT TouchMainPropList::OnBCGBtnPress(WPARAM /*wParam*/, LPARAM lParam)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	CBCGPProp* pProp = reinterpret_cast<CBCGPProp*>(lParam);

	if ((pProp)&&(pTouchDoc))
	{
		if (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize))
		{
			//we are a method item
			//go through the notations to find the one we want
			for (int i=0;i<pTouchDoc->getNotations().GetSize();i++)
			{
				if (pTouchDoc->getNotations().GetAt(i)->getName(false, true) == pProp->GetName())
				{
					pTouchDoc->editNotation(pTouchDoc->getNotations().GetAt(i)->getName(true));
					break;
				}
			}
		}
		else if (pProp->GetData() >= ptt_definitionItem && pProp->GetData() < (ptt_definitionItem+ TypeGapSize))
		{
			//we are a method item
			//go through the notations to find the one we want
			for (int i=0;i<pTouchDoc->getDefinitions().GetSize();i++)
			{
				if (pTouchDoc->getDefinitions().GetAt(i)->getName( true) == pProp->GetName())
				{
					pTouchDoc->editDefinition(pTouchDoc->getDefinitions().GetAt(i)->getName(true));
					break;
				}
			}
		}
		else if (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem+ TypeGapSize))
		{
			//we are a method item
			pTouchDoc->editMusic(pProp->GetName(), pProp->GetValue());
		}
		else
		{
			switch (pProp->GetData())
			{
			case ptt_startWithChange:
				pTouchDoc->setStartWithChange("Rounds");
				break;

			case ptt_startAtRow:
				pTouchDoc->onVisualSetStartAtRow();
				break;

			case ptt_startNotation:
				pTouchDoc->doEditStartNotation();
				break;

			case ptt_endWithChange:
				pTouchDoc->setEndWithChange("Rounds");
				break;

			case ptt_endLeadLimit:
				pTouchDoc->setLeadCount("");
				break;

			case ptt_endPartLimit:
				pTouchDoc->setPartCount("");
				break;

			case ptt_endCircleLimit:
				pTouchDoc->setCircularTouchCount("");
				break;

			case ptt_styleTouchFontMain:
				{
					FontInfo fontInfo(pTouchDoc->getFontMain());
					fontInfo.editFont(this);
					pTouchDoc->setFontMain(fontInfo);
				}
				break;				

			case ptt_styleTouchFontNotes:
				{
					FontInfo fontInfo(pTouchDoc->getFontNotes());
					fontInfo.editFont(this,true);
					pTouchDoc->setFontNotes(fontInfo);
				}
				break;				

			case ptt_styleTouchFontTitle:
				{
					FontInfo fontInfo(pTouchDoc->getFontTitle());
					fontInfo.editFont(this,true);
					pTouchDoc->setFontTitle(fontInfo);
				}
				break;

			case ptt_styleTouchFontAuthor:
				{
					FontInfo fontInfo(pTouchDoc->getFontAuthor());
					fontInfo.editFont(this,true);
					pTouchDoc->setFontAuthor(fontInfo);
				}
				break;	

			case ptt_styleBlueLine:
				pTouchDoc->editStyle();
				break;

			case ptt_notesEdit:
				{
					CPoint point;
					GetCursorPos(&point);
					pTouchDoc->editNotes(point);
				}
				break;

			default:
				ASSERT(FALSE);
			}
		}
	}

	Invalidate();
	
	return 0;
}



void TouchMainPropList::OnTouchMainDelete()
{
	CBCGPProp* pProp = GetCurSel();
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if ((pTouchDoc)&&(pProp))
	{
        BOOL methodItem = (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize));
		BOOL definitionItem = (pProp->GetData() >= ptt_definitionItem && pProp->GetData() < (ptt_definitionItem+ TypeGapSize));
		BOOL musicItem = (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem + TypeGapSize));

		if (methodItem)
		{
			//go through the notations to find the one we want
			for (int i=0;i<pTouchDoc->getNotations().GetSize();i++)
			{
				if (pTouchDoc->getNotations().GetAt(i)->getName(false, true) == pProp->GetName())
				{
					pTouchDoc->deleteNotation(pTouchDoc->getNotations().GetAt(i)->getName(true));
					pTouchDoc->getTouchPropListExpanded()._mainMethod = TRUE;
					break;
				}
			}
		}
		else if (definitionItem)
		{
			for (int i=0;i<pTouchDoc->getDefinitions().GetSize();i++)
			{
				if (pTouchDoc->getDefinitions().GetAt(i)->getName(true) == pProp->GetName())
				{
					pTouchDoc->deleteDefinition(pTouchDoc->getDefinitions().GetAt(i)->getName(true));
					pTouchDoc->getTouchPropListExpanded()._mainDefinition = TRUE;
					break;
				}
			}
		}
		else if (musicItem)
		{			   
			pTouchDoc->deleteMusic(pProp->GetName(), pProp->GetValue());
			pTouchDoc->getTouchPropListExpanded()._mainMusic = TRUE;
		}
	}   		
}
void TouchMainPropList::OnTouchMainAdd()
{
	CBCGPProp* pProp = GetCurSel();
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if ((pTouchDoc)&&(pProp))
	{
        BOOL methodItem = (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize));
		BOOL methodRoot = (pProp->GetData() == ptt_methodRoot);
		BOOL definitionItem = (pProp->GetData() >= ptt_definitionItem && pProp->GetData() < (ptt_definitionItem+ TypeGapSize));
		BOOL definitionRoot = (pProp->GetData() == ptt_definitionRoot);
		BOOL musicItem = (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem + TypeGapSize));
		BOOL musicRoot = (pProp->GetData() == ptt_musicRoot);

		if (methodItem || methodRoot)
		{
			pTouchDoc->OnAddNotation();
			pTouchDoc->getTouchPropListExpanded()._mainMethod = TRUE;
		}
		else if (definitionItem || definitionRoot)
		{
			pTouchDoc->OnAddDefinition();
			pTouchDoc->getTouchPropListExpanded()._mainDefinition = TRUE;
		}
		else if (musicItem || musicRoot)
		{
			pTouchDoc->OnAddMusic();
			pTouchDoc->getTouchPropListExpanded()._mainMusic = TRUE;
		}
	}
}

void TouchMainPropList::OnAddExplorerNotation()
{
	mainFrame()->PostMessage(WM_COMMAND, ID_ADD_EXPLORER_NOTATION);
}

void TouchMainPropList::OnAddFindNotation()
{
	mainFrame()->PostMessage(WM_COMMAND, ID_ADD_FIND_NOTATION);
}

void TouchMainPropList::OnTouchMainCrib()
{
	CBCGPProp* pProp = GetCurSel();
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();

	if ((pTouchDoc)&&
		(pProp)&&
		(pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize)))
	{
		//go through the notations to find the one we want
		for (int i=0;i<pTouchDoc->getNotations().GetSize();i++)
		{
			if (pTouchDoc->getNotations().GetAt(i)->getName(false, true) == pProp->GetName())
			{
				pTouchDoc->cribNotation(pTouchDoc->getNotations().GetAt(i)->getName(true));
				break;
			}
		}
	}   	
}

void TouchMainPropList::OnUpdateTouchMainDelete(CCmdUI *pCmdUI)
{
	BOOL enable = FALSE;
	CBCGPProp* pProp = GetCurSel();

	if (pProp)
	{
        BOOL methodItem = (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize));
		BOOL definitionItem = (pProp->GetData() >= ptt_definitionItem && pProp->GetData() < (ptt_definitionItem+ TypeGapSize));
		BOOL musicItem = (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem + TypeGapSize));

		enable = (methodItem || definitionItem || musicItem);
	}

	pCmdUI->Enable(enable);
}

void TouchMainPropList::OnUpdateTouchMainAdd(CCmdUI *pCmdUI)
{
	BOOL enable = FALSE;
	CBCGPProp* pProp = GetCurSel();

	if (pProp)
	{
        BOOL methodItem = (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize));
		BOOL methodRoot = (pProp->GetData() == ptt_methodRoot);
		BOOL definitionItem = (pProp->GetData() >= ptt_definitionItem && pProp->GetData() < (ptt_definitionItem+ TypeGapSize));
		BOOL definitionRoot = (pProp->GetData() == ptt_definitionRoot);
		BOOL musicItem = (pProp->GetData() >= ptt_musicItem && pProp->GetData() < (ptt_musicItem + TypeGapSize));
		BOOL musicRoot = (pProp->GetData() == ptt_musicRoot);                                                    

		enable = (methodItem || methodRoot || 
				  definitionItem || definitionRoot ||
				  musicItem || musicRoot);
	}

	pCmdUI->Enable(enable);
}

void TouchMainPropList::OnUpdateTouchMainCrib(CCmdUI *pCmdUI)
{
	BOOL enable = FALSE;
	CBCGPProp* pProp = GetCurSel();

	if (pProp)
	{
        BOOL methodItem = (pProp->GetData() >= ptt_methodItem && pProp->GetData() < (ptt_methodItem+ TypeGapSize));

		enable = methodItem;
	}

	pCmdUI->Enable(enable);
}





