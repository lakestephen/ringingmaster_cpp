// CalledChangeEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "CalledChangeEditorDlg.h"
#include "GlobalFunctions.h"
#include "OptionsDlg.h"		  
#include "Method.h"
#include "Lead.h"
#include "Tower.h"
#include "TowerManager.h"
#include "UserMessages.h"
#include "MethodPosition.h"
#include "MethodDlg.h"
#include "RowChooserDlg.h"
#include "WinPos.h"
#include "PlayManager.h"

#include "OptionsCalledChanges.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CalledChangeEditorDlg dialog


CalledChangeEditorDlg::CalledChangeEditorDlg(CWnd* pParent )
	: ResizeDialog(CalledChangeEditorDlg::IDD, pParent),
	_number(6),
	_method(NULL),
	_inited(false)
{
	//{{AFX_DATA_INIT(CalledChangeEditorDlg)
	//}}AFX_DATA_INIT

	_seekRow = app()->GetProfileString(OptionsCalledChanges::CallChangeSection, "Seek Row", "");
	_number = app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Number", 6);

}

CalledChangeEditorDlg::~CalledChangeEditorDlg()
{
	app()->WriteProfileString(OptionsCalledChanges::CallChangeSection, "Seek Row", _seekRow );
	app()->WriteProfileInt(OptionsCalledChanges::CallChangeSection, "Number", _number );

	delete _method;
}

void CalledChangeEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CalledChangeEditorDlg)
	DDX_Control(pDX, IDC_MESSAGE, _messageCtrl);
	DDX_Control(pDX, IDC_CC_STYLE, _style);
	DDX_Control(pDX, IDC_CC_SEEK, _seek);
	DDX_Control(pDX, IDC_CC_REWIND, _rewind);
	DDX_Control(pDX, IDC_CC_COPY, _copy);
	DDX_Control(pDX, IDC_NUMBER, _numberCombo);
	DDX_Control(pDX, ID_EDIT_TOWER_DETAILS, _towerDetails);
	DDX_Control(pDX, ID_TOWER_SOUND_OPTIONS, _towerOptions);
	DDX_Control(pDX, IDC_OPTIONS_TOWER_COMBO, _comboTower);
	DDX_Control(pDX, IDC_CC_UNDO, _undo);
	DDX_Control(pDX, IDC_CC_STOP, _stop);
	DDX_Control(pDX, IDC_CC_PLAY_LIVE, _playLive);
	DDX_Control(pDX, IDC_CC_STATR_AGAIN, _startAgain);
	DDX_Control(pDX, IDC_CC_OPTIONS, _options);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CalledChangeEditorDlg, ResizeDialog)
	//{{AFX_MSG_MAP(CalledChangeEditorDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_CC_OPTIONS, OnCcOptions)
	ON_BN_CLICKED(IDC_CC_OPTIONS, OnCcOptions)
	ON_BN_CLICKED(IDC_CC_STATR_AGAIN, OnCcStatrAgain)
	ON_BN_CLICKED(IDC_CC_PLAY_LIVE, OnCcPlayLive)
	ON_BN_CLICKED(IDC_CC_STOP, OnCcStop)
	ON_BN_CLICKED(IDC_CC_UNDO, OnCcUndo)
	ON_CBN_SELCHANGE(IDC_OPTIONS_TOWER_COMBO, OnSelchangeOptionsTowerCombo)
	ON_CBN_SELCHANGE(IDC_NUMBER, OnSelchangeNumber)
	ON_BN_CLICKED(IDC_CC_REWIND, OnCcRewind)
	ON_BN_CLICKED(IDC_CC_SEEK, OnCcSeek)
	ON_BN_CLICKED(IDC_CC_COPY, OnCcCopy)
	ON_BN_DOUBLECLICKED(IDC_CC_UNDO, OnCcUndo)
	ON_BN_CLICKED(IDC_CC_STYLE, OnCcStyle)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SET_METHOD_POS,OnSetMethodPos) 
	ON_UPDATE_COMMAND_UI(IDC_CC_PLAY_LIVE, OnUpdateCcPlayLive)
	ON_UPDATE_COMMAND_UI(IDC_CC_STOP, OnUpdateCcStop)

	ON_UPDATE_COMMAND_UI(IDC_CC_UNDO, OnUpdateCcUndo)
	ON_UPDATE_COMMAND_UI(IDC_CC_STATR_AGAIN, OnUpdateCcStartAgain)

	ON_UPDATE_COMMAND_UI(IDC_OPTIONS_TOWER_COMBO, OnUpdateOptionsTowerCombo)

	ON_UPDATE_COMMAND_UI(IDC_CC_REWIND, OnUpdateCcRewind)

	ON_UPDATE_COMMAND_UI_RANGE(IDC_CALLED_CAHNGE_BASE, IDC_CALLED_CAHNGE_BASE+MAXBELLS, OnUpdateSwap)

	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CALLED_CAHNGE_BASE, IDC_CALLED_CAHNGE_BASE+MAXBELLS, OnSwap)
	ON_CONTROL_RANGE(BN_DOUBLECLICKED, IDC_CALLED_CAHNGE_BASE, IDC_CALLED_CAHNGE_BASE+MAXBELLS, OnSwap)
	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CalledChangeEditorDlg message handlers

LRESULT CalledChangeEditorDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}

BOOL CalledChangeEditorDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();


	//get the position of the place holder static item
	CRect drawRect;
	GetDlgItem(IDC_CALLED_CHANGE_LIST_DUMMY)->GetWindowRect(&drawRect);
	ScreenToClient(&drawRect);

	//create the method window
	_methodList.Create(WS_EX_CLIENTEDGE, "CallChange Wnd", WS_TABSTOP, drawRect, 
								this, IDC_CALLED_CHANGE_LIST, _method);
									  
	AddControl(IDC_CALLED_CHANGE_LIST, resize, resize);	 
	AddControl(IDC_SEP1, resize, none);	 
	AddControl(IDC_SEP2, resize, none);	 
	AddControl(IDC_MESSAGE, resize, repos);	 
	


	for (int i=0;i<MAXBELLS;i++)
	{
		_buttons[i].Create("", WS_CHILD | WS_VISIBLE, CRect(0,0,100,100), this, IDC_CALLED_CAHNGE_BASE + i);
		_buttons[i].SetImage(IDB_SWAP);
		_buttons[i].m_bDrawFocus = FALSE;
		_buttons[i].m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	}

	_options.SetImage(IDB_OPTIONS_CALLCHANGE);
	_options.m_bDrawFocus = FALSE;
	_options.SetTooltip("Called Changes Options");
	_options.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_startAgain.SetImage(IDB_DELETE);
	_startAgain.m_bDrawFocus = FALSE;
	_startAgain.SetTooltip("Remove all changes and start again");
	_startAgain.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	
	_playLive.SetImage(IDB_PLAY);
	_playLive.m_bDrawFocus = FALSE;
	_playLive.SetTooltip("Play the change live.");
	_playLive.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_stop.SetImage(IDB_STOP);
	_stop.m_bDrawFocus = FALSE;
	_stop.SetTooltip("Stop the playing.");
	_stop.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_undo.SetImage(IDB_UNDO);
	_undo.m_bDrawFocus = FALSE;
	_undo.SetTooltip("Undo the last change");
	_undo.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_towerOptions.SetImage(IDB_OPTIONS_TOWER_SOUNDS);
	_towerOptions.m_bDrawFocus = FALSE;
	_towerOptions.SetTooltip("Sound Profile Options");
	_towerOptions.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_towerDetails.SetImage(IDB_TOWER_DETAILS);
	_towerDetails.m_bDrawFocus = FALSE;
	_towerDetails.SetTooltip("Edit Sound Profile");
	_towerDetails.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_style.SetImage(IDB_STYLE);
	_style.m_bDrawFocus = FALSE;
	_style.SetTooltip("Edit Style");
	_style.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_copy.SetImage(IDB_COPY);
	_copy.m_bDrawFocus = FALSE;
	_copy.SetTooltip("Copy");
	_copy.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_rewind.SetImage(IDB_ROUNDS);
	_rewind.m_bDrawFocus = FALSE;
	_rewind.SetTooltip("Rewind to Rounds");
	_rewind.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	_seek.SetImage(IDB_SEEK);
	_seek.m_bDrawFocus = FALSE;
	_seek.SetTooltip("Seek Change");
	_seek.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

	WinPos::Load(this, "CalledChangeEditorDlg", true); 

	//fill the tower combo
	towerManager()->addEventListener(this);

	//fill the number combo
	_numberCombo.ResetContent();
	int limit = MAXBELLS;
	if (mainFrame()->verifyKey() == FALSE)
	{
		limit = 6; 
	}
	for (int i=MINBELLS;i<=limit;i++)
	{
		_numberCombo.AddString(GlobalFunctions::numberName(i));
	}
	_numberCombo.SetCurSel(_number - 3);
	
	doCcStatrAgain(false);

	_inited = true;

	return TRUE;  // return TRUE unless you set the focus to a control

}

const int numberTop= 66;
const int buttonTop = numberTop + 12;
const int buttonSpacing = 22;
const int buttonGap = 0;
const int buttonHeight = 19;
const int buttonLeftOffset = 19;
const int numberLeftOffset = buttonLeftOffset - 5;

void CalledChangeEditorDlg::setupSwapButtons()
{


	for (int i=0;i<MAXBELLS;i++)
	{
		//show
		_buttons[i].ShowWindow((i < _number-1)?SW_SHOW:SW_HIDE);

		//position
		if (IsWindow(_buttons[i].m_hWnd))
		_buttons[i].SetWindowPos (	NULL,
								(buttonSpacing * i) + buttonLeftOffset , 
								buttonTop, 
								buttonSpacing - buttonGap, 
								buttonHeight,
								SWP_NOACTIVATE | SWP_NOZORDER); 	
		//enable
		_buttons[i].EnableWindow(FALSE);

	}	
}



void CalledChangeEditorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	paintNumbers(&dc);
	
}

void CalledChangeEditorDlg::paintNumbers(CDC *pDC)
{
	CFont* oldFont = pDC->SelectObject(&app()->_fontStd);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

/*	pDC->Draw3dRect(CRect(  11,
							numberTop-5,
							buttonLeftOffset+ ((_number -1)* buttonSpacing) + 15 ,
							buttonTop + buttonHeight + 5), 
					GetSysColor(COLOR_3DSHADOW), 
					GetSysColor(COLOR_3DLIGHT));
  */
	CString str;
		
	Row * row = getLastRow();

	if (row)
	{
		for (int i=0;i<row->getNumber();i++)
		{
			pDC->TextOut(numberLeftOffset + (i*buttonSpacing), 
						numberTop, 
						CString(GlobalFunctions::bellNumbersToChar(row->getBellInPlace(i))));		
		}
	}


	pDC->SelectObject(oldFont);

}

void CalledChangeEditorDlg::setToolTips()
{
	CString str;

	Row* row = getLastRow();
	
	for (int i=0;i<MAXBELLS-1;i++)
	{
		if ((row)&&(i < row->getNumber()-1))
		{	
			Row* newRow = new Row(*row);
			newRow->swap(i);
			str = getCallText(row, newRow, FALSE, FALSE);			
			delete newRow;
		}
		else
			str = "";

		_buttons[i].SetTooltip(str);
	}
}


void CalledChangeEditorDlg::OnDestroy() 
{
	towerManager()->removeEventListener(this);
	
	WinPos::Save(this, "CalledChangeEditorDlg");

	ResizeDialog::OnDestroy();

}

void CalledChangeEditorDlg::OnCcOptions() 
{
	OptionsDisplayFilter filter(false);
	filter._calledChanges = true;

	BOOL inclTenor = app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Include Tenor", FALSE);

	//filter._tower = true;
	OptionsDlg propSheet(this, filter);
	if (propSheet.DoModal() != IDCANCEL)
	{
		if (inclTenor)
		{
			BOOL newInclTenor = app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Include Tenor", FALSE);
			if ((!newInclTenor)&&(getLastRow()->getPositionOfBell(_number) != _number-1))
			{
				if (AfxMessageBox("You have chosen to exclude the tenor. To do this your changes will need to be deleted as the tenor is out of place. \nDo you want to delete all changes?",
					MB_YESNO|MB_ICONQUESTION) == IDYES)
					doCcStatrAgain();
				else 
					app()->WriteProfileInt(OptionsCalledChanges::CallChangeSection, "Include Tenor", TRUE);	
			}
		}	
		playManager()->setPullsPerChange(app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Pulls Per Change", 2) - 1);
	}

	updateComments();
	setToolTips();
	update();
	

}

void CalledChangeEditorDlg::OnUpdateSwap( CCmdUI* pCmdUI)
{
	int buttonNo = pCmdUI->m_nID - IDC_CALLED_CAHNGE_BASE;
//	TRACE("Update Swap Btn: %d \r\n", buttonNo);
	BOOL inclTenor = app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Include Tenor", FALSE);

	pCmdUI->Enable((inclTenor)||(buttonNo != _number-2));

}

void CalledChangeEditorDlg::OnSwap( UINT nID )
{	 
	int buttonNo = nID - IDC_CALLED_CAHNGE_BASE;
//	TRACE("Swap Btn: %d \r\n", buttonNo);

	Row* row = getLastRow();

	if (row)
	{
		Row* newRow = new Row(*row);
		newRow->swap(buttonNo);
		addRow(newRow);

	}
	update();

}

void CalledChangeEditorDlg::OnCcUndo() 
{
	removeLastRow();
	updateComments();
	update();
}


void CalledChangeEditorDlg::OnCcStatrAgain() 
{
	doCcStatrAgain();
}

void CalledChangeEditorDlg::doCcStatrAgain(bool /*doStop*/) 
{   /*	todo
	delete _method;
	_method = new MethodCallChange(_number);
	getLead()->addLeadLine(0);

	//add rounds
	Lead* lead = getLead();
	if (lead)
	{
		Row* newRow = new Row(_number, back);
		newRow->setRounds();
		addRow(newRow);
		setupSwapButtons();
		setToolTips();
	}			   
	
	if (doStop)
		playManager()->OnPlayStop();
			   
	update();   */
}

Lead* CalledChangeEditorDlg::getLead()
{		/*
	if (_method == NULL)
		return NULL;
	else 
		return _method->GetAt(0);						 
	*/ //todo

	return NULL;
}

Row* CalledChangeEditorDlg::getLastRow()
{
	Lead* lead = getLead();
	if ((lead == NULL)||(lead->GetSize() == 0))
		return NULL;

	return lead->GetAt(lead->GetUpperBound());
}

void CalledChangeEditorDlg::OnUpdateCcUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(getLead()->GetSize() > 1);
}

void CalledChangeEditorDlg::OnUpdateCcStartAgain(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(getLead()->GetSize() > 1);
}

void CalledChangeEditorDlg::OnUpdateCcPlayLive(CCmdUI* pCmdUI) 
{
	Lead* lead = getLead();
	BOOL enoughRows = lead && lead->GetSize() >1;

	playManager()->OnUpdateCcPlayLive(pCmdUI, _number, enoughRows);
}

void CalledChangeEditorDlg::OnCcPlayLive() 
{
	//make a copy, and let the play thread take charge of it
	PlayRequest* request = new PlayRequest(*_method);

	request->setPullsPerChange(app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Pulls Per Change", 2) - 1);
	request->setLastRowIndefiniteRepeat(true);
	playManager()->play(request);
}

void CalledChangeEditorDlg::OnUpdateCcRewind(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((getLastRow() !=NULL)&&(!getLastRow()->isRounds()));
}

void CalledChangeEditorDlg::OnUpdateCcStop(CCmdUI* pCmdUI) 
{
	playManager()->OnUpdateStopMethod(pCmdUI);
}

void CalledChangeEditorDlg::OnCcStop() 
{
	playManager()->OnPlayStop();
}

void CalledChangeEditorDlg::OnSelchangeOptionsTowerCombo() 
{
	towerManager()->setActiveTowerIndex(_comboTower.GetCurSel());	
}

void CalledChangeEditorDlg::OnUpdateOptionsTowerCombo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(playManager()->isPlayAvailable());
}

void CalledChangeEditorDlg::towerManager_notifyUpdateTowerList()
{
//	TRACE("Listener CalledChangeEditorDlg::towerManager_notifyUpdateTowerList() \r\n");

	_comboTower.ResetContent();

	for (int i=0;i<towerManager()->getTowerCount();i++)
	{
		_comboTower.AddString(towerManager()->getTower(i)->getDisplayName());
	}
	//set the active style
	_comboTower.SetCurSel(towerManager()->getActiveTowerIndex());

	setMessage();
}

void CalledChangeEditorDlg::OnSelchangeNumber() 
{
	
	int number = _numberCombo.GetCurSel();

	if (number == -1) 
		return;

	number = number + 3;

	if ((number != _number)&&
	    ((getLead()->GetSize() == 1)||
		 (AfxMessageBox("Do you want to delete all changes?", MB_YESNO|MB_ICONQUESTION) == IDYES)))
	{
		_seekRow = "";
		_number = number;
		doCcStatrAgain();
	}


	WINDOWPLACEMENT wpNew;
	GetWindowPlacement ( &wpNew );
	SetWindowPlacement ( &wpNew );

	update();
	
}

void CalledChangeEditorDlg::OnCcRewind() 
{
	int originalSize = getLead()->GetUpperBound();

	if ((originalSize >= 1)&&(!getLastRow()->isRounds()))
	{
		for (int i=(originalSize-1);i>=0;i--)
		{
			Row* newRow = new Row(*getLead()->GetAt(i));
			addRow(newRow);
			if (newRow->isRounds())
				break;
		}
		update();
	}	 
			 
}

void CalledChangeEditorDlg::OnCcSeek() 
{
	RowChooserDlg dlg(this, "Seek Change", _number, _seekRow, 
		!app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Include Tenor", FALSE));
	if (dlg.DoModal() == IDOK)
	{
		_seekRow = dlg._originalRow;

		Row seekRow(_number);
		CString msg;
		//we can gaurentee that this is OK
		seekRow.setAsText(_seekRow, msg);

		//loop through the PLACES
		for (int place=0;place<_number;place++)
		{
			//get the bell that is needed inthe required place
			int activeBell = seekRow.getBellInPlace(place);
			//get the position of that bell in the last row
			int posOfActiveBell = -1;
			while ((posOfActiveBell = getLastRow()->getPositionOfBell(activeBell)) != place)
			{
				Row* newRow = new Row(*getLastRow());
				newRow->swap((posOfActiveBell < place)?posOfActiveBell:posOfActiveBell-1);
				addRow(newRow);	
			}
		}
		update();
	}
}

void CalledChangeEditorDlg::OnCcCopy() 
{
	::PostMessage(_methodList.m_hWnd,WM_COMMAND, ID_EDIT_COPY,0);	
}

void CalledChangeEditorDlg::update()
{
	_methodList.setMethod(_method);
	RowPosition rowPosition;
	rowPosition.setRow(_method, 0, getLead()->GetUpperBound());

	_methodList.scrollToRow(rowPosition);
	setMessage();
	_numberCombo.SetCurSel(_number - 3);
	Invalidate();
}


void CalledChangeEditorDlg::OnCcStyle() 
{
	::PostMessage(_methodList.m_hWnd,WM_COMMAND, ID_TOOLS_STYLE	,0);	
}

void CalledChangeEditorDlg::addRow(Row* /*newRow*/)
{	  /*
	if (_method)
	{
		//calculate stuff
		newRow->calculateInCourse();
		newRow->setRowIndex(getLead()->GetSize());

		//now add
		getLead()->Add(newRow);
	//	TRACE("Row %s\r\n", newRow->getAsText());
		
		//set comments
		RowComment* rowComment = NULL;
		if (getLead()->GetSize() > 1)
		{
			CString callText = getCallText( getLead()->GetAt(getLead()->GetUpperBound()-1),
											getLead()->GetAt(getLead()->GetUpperBound()));

			rowComment = new RowComment;
			rowComment->setRow(0,getLead()->GetUpperBound()-1);
			rowComment->setComment(callText);
			_method->addComment(rowComment);
		}

		//set tool tips;
		setToolTips();

		//tell the play thread
		RowExtra* rowExtra = new RowExtra(*newRow, 0);
		if (rowComment)
			rowExtra->Add(new RowComment(*rowComment));
		playManager()->addRow(rowExtra, _method->getUniqueId());
	}
	   */ //todo
}

bool CalledChangeEditorDlg::removeLastRow()
{
			/*
	Lead* lead = getLead();
	if ((_method)&&(lead)&&(lead->GetSize() > 1))
	{
		delete lead->GetAt(lead->GetUpperBound());
		lead->RemoveAt(lead->GetUpperBound());
	
		playManager()->removeRow(_method->getUniqueId());

		if (lead->GetSize() <= 1)
			playManager()->OnPlayStop();

		return true;
	}		*/ //todo
	return false;
}

LRESULT CalledChangeEditorDlg::OnSetMethodPos(WPARAM /*wParam*/, LPARAM lParam)
{
	//squash the message 
	MethodPosition * pos = (MethodPosition*)lParam;
	delete pos;	
	return 0;
}

CString CalledChangeEditorDlg::getCallText(Row *row1, Row *row2)
{
	return getCallText(
			row1, row2,
			app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Affect 2", FALSE),
			app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Affect 3", FALSE));
}


CString CalledChangeEditorDlg::getCallText(Row *row1, Row *row2, BOOL affect2, BOOL affect3)
{					    
	ASSERT(row1);
	ASSERT(row2);

//	TRACE("Row1 %s\r\n", row1->getAsText());
//	TRACE("Row2 %s\r\n", row2->getAsText());

	int before = -1;
	int moveDown = -1;
	int moveUp = -1;
	int after = -1;

	
	for (int i=0;i<_number;i++)
	{
		if(row1->getBellInPlace(i) == row2->getBellInPlace(i))
			before = row2->getBellInPlace(i);
		else
		{
			moveDown=row2->getBellInPlace(i);
			if (i<_number-1) 
				moveUp=row2->getBellInPlace(i+1);
			if (i<_number-2) 
				after=row2->getBellInPlace(i+2);;
			break;
		}
	}

	CString upCall = getNumberName(moveUp) + " to " +
					 getNumberName(moveDown);

	CString downCall = getNumberName(moveDown) + 
		((before==-1)?" lead":(" to " + getNumberName(before)));

	CString afterCall = ((after==-1)?
					  (getNumberName(row2->getBellInPlace(0)) + " lead from " +
					  getNumberName(moveUp)):
					 (getNumberName(after) + " to " +
					  getNumberName(moveUp)));

	//call up or down
	if (app()->GetProfileInt(OptionsCalledChanges::CallChangeSection, "Calling Type", 0) != 0)
	{
		CString temp = upCall;
		upCall = downCall;
		downCall = temp;
	}

	return upCall + 
		   (affect2?("\t" + downCall):"") + 
		   (affect3?("\t" + afterCall):"") + 
		   (row2->isRounds()?"\t rounds":"");


}

void CalledChangeEditorDlg::updateComments()
{			   /*
	getLead()->removeAllComments();

	for (int i=0;i<getLead()->GetUpperBound();i++)
	{
		CString callText = getCallText( getLead()->GetAt(i),
										getLead()->GetAt(i+1));

		RowComment* rowComment = new RowComment;
		rowComment->setRow(0,i);
		rowComment->setComment(callText);
		_method->addComment(rowComment);	
	}
	*/
}

void CalledChangeEditorDlg::OnGetMinMaxInfo(MINMAXINFO *lpMMI)
{
	ResizeDialog::OnGetMinMaxInfo(lpMMI);

	if (_inited)
	{
		int minX = (numberLeftOffset*2) + (_number*buttonSpacing);
		if (lpMMI->ptMinTrackSize.x < minX) 
			lpMMI->ptMinTrackSize.x = minX;
	}
}


CString CalledChangeEditorDlg::getNumberName(int number)
{
	if (number == 1)
		return "treble";
	if (number == _number)
		return "tenor";
	return GlobalFunctions::getNumberName(number);
}

void CalledChangeEditorDlg::setMessage()
{
	CString msg;
	//do we have a tower??
	Tower* activeTower = towerManager()->getTower();
	if ((activeTower)&&(_inited))
	{
		//update the peal time
		int pullPerChange = app()->GetProfileInt(
			OptionsCalledChanges::CallChangeSection, "Pulls Per Change", 2);

		CString strSpeed = GlobalFunctions::getPealTimeText(_number, 
											activeTower->getSpeed(),
											activeTower->getOpenHandstrokeGap(), 
											true, 
											getLead()->GetSize() * pullPerChange);


		msg.Format("Time to ring with " + strSpeed + " h:m:s");
	}																					
	
	_messageCtrl.SetWindowText(msg);

}

