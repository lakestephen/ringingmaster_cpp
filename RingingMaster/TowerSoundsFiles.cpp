// TowerSoundsFiles.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "TowerSoundsFiles.h"
#include "TowerSoundsGeneral.h"
#include "Tower.h"
#include "PlayManager.h"
#include "TowerAdvancedDlg.h"
#include "UserMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFiles dialog


TowerSoundsFiles::TowerSoundsFiles(Tower& internalTower)
	: CBCGPPropertyPage(TowerSoundsFiles::IDD),
	_internalTower(internalTower),
	_inited(false), 
	_testBell(-1),
	_testStroke(hand)
{
	//{{AFX_DATA_INIT(TowerSoundsFiles)
	//}}AFX_DATA_INIT

	_bell[0] = &_bell0;
	_bell[1] = &_bell1;
	_bell[2] = &_bell2;
	_bell[3] = &_bell3;
	_bell[4] = &_bell4;
	_bell[5] = &_bell5;
	_bell[6] = &_bell6;
	_bell[7] = &_bell7;

	_stroke[0] = &_stroke0;
	_stroke[1] = &_stroke1;
	_stroke[2] = &_stroke2;
	_stroke[3] = &_stroke3;
	_stroke[4] = &_stroke4;
	_stroke[5] = &_stroke5;
	_stroke[6] = &_stroke6;
	_stroke[7] = &_stroke7;

	_details[0] = &_details0;
	_details[1] = &_details1;
	_details[2] = &_details2;
	_details[3] = &_details3;
	_details[4] = &_details4;
	_details[5] = &_details5;
	_details[6] = &_details6;
	_details[7] = &_details7;

	_test[0] = &_test0;
	_test[1] = &_test1;
	_test[2] = &_test2;
	_test[3] = &_test3;
	_test[4] = &_test4;
	_test[5] = &_test5;
	_test[6] = &_test6;
	_test[7] = &_test7;

	_edit[0] = &_edit0;
	_edit[1] = &_edit1;
	_edit[2] = &_edit2;
	_edit[3] = &_edit3;
	_edit[4] = &_edit4;
	_edit[5] = &_edit5;
	_edit[6] = &_edit6;
	_edit[7] = &_edit7;

	_path[0] = &_path0;
	_path[1] = &_path1;
	_path[2] = &_path2;
	_path[3] = &_path3;
	_path[4] = &_path4;
	_path[5] = &_path5;
	_path[6] = &_path6;
	_path[7] = &_path7;

}


void TowerSoundsFiles::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TowerSoundsFiles)
	DDX_Control(pDX, IDC_REMOVE_ALL, _removeAll);
	DDX_Control(pDX, IDC_TEMPLATE_BUTT, _templateBtn);
	DDX_Control(pDX, IDC_PATH0, _path0);
	DDX_Control(pDX, IDC_PATH1, _path1);
	DDX_Control(pDX, IDC_PATH2, _path2);
	DDX_Control(pDX, IDC_PATH3, _path3);
	DDX_Control(pDX, IDC_PATH4, _path4);
	DDX_Control(pDX, IDC_PATH5, _path5);
	DDX_Control(pDX, IDC_PATH6, _path6);
	DDX_Control(pDX, IDC_PATH7, _path7);
	DDX_Control(pDX, IDC_EDIT0, _edit0);
	DDX_Control(pDX, IDC_EDIT1, _edit1);
	DDX_Control(pDX, IDC_EDIT2, _edit2);
	DDX_Control(pDX, IDC_EDIT3, _edit3);
	DDX_Control(pDX, IDC_EDIT4, _edit4);
	DDX_Control(pDX, IDC_EDIT5, _edit5);
	DDX_Control(pDX, IDC_EDIT6, _edit6);		   
	DDX_Control(pDX, IDC_EDIT7, _edit7);
	DDX_Control(pDX, IDC_TEST0, _test0);
	DDX_Control(pDX, IDC_TEST1, _test1);
	DDX_Control(pDX, IDC_TEST2, _test2);
	DDX_Control(pDX, IDC_TEST3, _test3);
	DDX_Control(pDX, IDC_TEST4, _test4);				   
	DDX_Control(pDX, IDC_TEST5, _test5);
	DDX_Control(pDX, IDC_TEST6, _test6);
	DDX_Control(pDX, IDC_TEST7, _test7);
	DDX_Control(pDX, IDC_SCROLLBAR1, _sbLines);
	DDX_Control(pDX, IDC_DETAILS0, _details0);
	DDX_Control(pDX, IDC_DETAILS1, _details1);
	DDX_Control(pDX, IDC_DETAILS2, _details2);							 
	DDX_Control(pDX, IDC_DETAILS3, _details3);
	DDX_Control(pDX, IDC_DETAILS4, _details4);
	DDX_Control(pDX, IDC_DETAILS5, _details5);
	DDX_Control(pDX, IDC_DETAILS6, _details6);
	DDX_Control(pDX, IDC_DETAILS7, _details7);
	DDX_Text(pDX, IDC_STATIC_BELL0, _bell0);
	DDX_Text(pDX, IDC_STATIC_BELL1, _bell1);
	DDX_Text(pDX, IDC_STATIC_BELL2, _bell2);
	DDX_Text(pDX, IDC_STATIC_BELL3, _bell3);
	DDX_Text(pDX, IDC_STATIC_BELL4, _bell4);
	DDX_Text(pDX, IDC_STATIC_BELL5, _bell5);
	DDX_Text(pDX, IDC_STATIC_BELL6, _bell6);
	DDX_Text(pDX, IDC_STATIC_BELL7, _bell7);
	DDX_Text(pDX, IDC_STATIC_STROKE0, _stroke0);
	DDX_Text(pDX, IDC_STATIC_STROKE1, _stroke1);
	DDX_Text(pDX, IDC_STATIC_STROKE2, _stroke2);
	DDX_Text(pDX, IDC_STATIC_STROKE3, _stroke3);
	DDX_Text(pDX, IDC_STATIC_STROKE4, _stroke4);
	DDX_Text(pDX, IDC_STATIC_STROKE5, _stroke5);
	DDX_Text(pDX, IDC_STATIC_STROKE6, _stroke6);
	DDX_Text(pDX, IDC_STATIC_STROKE7, _stroke7);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(TowerSoundsFiles, CDialog)
	//{{AFX_MSG_MAP(TowerSoundsFiles)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DETAILS0, OnDetails0)
	ON_BN_CLICKED(IDC_DETAILS1, OnDetails1)
	ON_BN_CLICKED(IDC_DETAILS2, OnDetails2)
	ON_BN_CLICKED(IDC_DETAILS3, OnDetails3)
	ON_BN_CLICKED(IDC_DETAILS4, OnDetails4)
	ON_BN_CLICKED(IDC_DETAILS5, OnDetails5)
	ON_BN_CLICKED(IDC_DETAILS6, OnDetails6)
	ON_BN_CLICKED(IDC_DETAILS7, OnDetails7)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PATH0, OnPath0)
	ON_BN_CLICKED(IDC_PATH1, OnPath1)
	ON_BN_CLICKED(IDC_PATH2, OnPath2)
	ON_BN_CLICKED(IDC_PATH3, OnPath3)
	ON_BN_CLICKED(IDC_PATH4, OnPath4)
	ON_BN_CLICKED(IDC_PATH5, OnPath5)
	ON_BN_CLICKED(IDC_PATH6, OnPath6)
	ON_BN_CLICKED(IDC_PATH7, OnPath7)

	ON_BN_CLICKED(IDC_TEMPLATE_BUTT, OnTemplateButt)
	ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
	
	ON_BN_CLICKED(IDC_EDIT0, OnEdit0)
	ON_BN_CLICKED(IDC_EDIT1, OnEdit1)
	ON_BN_CLICKED(IDC_EDIT2, OnEdit2)
	ON_BN_CLICKED(IDC_EDIT3, OnEdit3)
	ON_BN_CLICKED(IDC_EDIT4, OnEdit4)
	ON_BN_CLICKED(IDC_EDIT5, OnEdit5)
	ON_BN_CLICKED(IDC_EDIT6, OnEdit6)
	ON_BN_CLICKED(IDC_EDIT7, OnEdit7)

	ON_BN_CLICKED(IDC_TEST0, OnTest0)
	ON_BN_CLICKED(IDC_TEST1, OnTest1)
	ON_BN_CLICKED(IDC_TEST2, OnTest2)
	ON_BN_CLICKED(IDC_TEST3, OnTest3)
	ON_BN_CLICKED(IDC_TEST4, OnTest4)
	ON_BN_CLICKED(IDC_TEST5, OnTest5)
	ON_BN_CLICKED(IDC_TEST6, OnTest6)
	ON_BN_CLICKED(IDC_TEST7, OnTest7)

	ON_WM_TIMER()
	ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TowerSoundsFiles message handlers

void TowerSoundsFiles::update()
{
	//set up the scroll bar
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(ScrollInfo);     // size of this structure
	ScrollInfo.fMask = SIF_ALL;                 // parameters to set
	ScrollInfo.nMin = 0;                        // minimum scrolling position
	ScrollInfo.nMax = (_internalTower.getNumber()*2) - 7;	// maximum scrolling position
	ScrollInfo.nPage = 2;						// the page size of the scroll box
	ScrollInfo.nPos = 0;						// initial position of the scroll box
	ScrollInfo.nTrackPos = 0;                   // immediate position of a scroll box that the user is dragging
	_sbLines.SetScrollInfo(&ScrollInfo);

	_sbLines.SetScrollPos(0);

	//set up all other ctrls	
	dataToCtrls();
	updateIcons();
}
	

void TowerSoundsFiles::updateIcons()
{
	for (int i=0;i<PROFILE_BELL_SOUND_CTRL_COUNT;i++)
	{
		//test icon
		if (::IsWindow(_test[i]->m_hWnd))
		{
			_test[i]->SetCheck((rowToBell(i) == _testBell)&&(rowToStroke(i) == _testStroke));
		}

		//edit icon
		if (::IsWindow(_edit[i]->m_hWnd))
		{
			//get the bell item
			TowerBellPersist& towerBellPersist = _internalTower.getTowerBellPersist(rowToBell(i));
			bool advancedModifyed = (towerBellPersist.getTowerBellItem(rowToStroke(i)).isSettingFrequencyOverride()||
									 towerBellPersist.getTowerBellItem(rowToStroke(i)).isSettingVolumeOverride());


			_edit[i]->SetImage((advancedModifyed)?IDB_ADVANCED_RED:IDB_ADVANCED_BLUE);
			_edit[i]->Invalidate();

			if (advancedModifyed)
			{
				CString str;
				str.Format("Volume: %.3fDb Frequency: %.3f%%", 
					towerBellPersist.getTowerBellItem(rowToStroke(i)).getVolumeOverride(), 
					towerBellPersist.getTowerBellItem(rowToStroke(i)).getFrequencyOverride() );
				_edit[i]->SetTooltip(str);
			}
			else
				_edit[i]->SetTooltip("Advanced Edit the playing details.");
		}
	}

}

BOOL TowerSoundsFiles::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	_templateBtn.m_bDrawFocus = FALSE;
	_templateBtn.SetTooltip("Select the treble, and the program will automatically fill in the other files.");
	_templateBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_templateBtn.SetImage(IDB_TOWER_TREBLE);

	_removeAll.m_bDrawFocus = FALSE;
	_removeAll.SetTooltip("Remove all wave files");
	_removeAll.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_removeAll.SetImage(IDB_DELETE);

	for (int i=0;i<PROFILE_BELL_SOUND_CTRL_COUNT;i++)
	{
		_details[i]->SetImage(IDB_INFO);
		_details[i]->m_bDrawFocus = FALSE;
		_details[i]->SetTooltip("See the wave file details for this bell.");
		_details[i]->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		_test[i]->SetImage (IDB_BELL);
		_test[i]->SetCheckedImage (IDB_BELL_AND_TICK);
		_test[i]->m_bDrawFocus = FALSE;
		_test[i]->SetTooltip("Test play this bell.");
		_test[i]->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		_edit[i]->m_bDrawFocus = FALSE;
		_edit[i]->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;

		_path[i]->SetImage(IDB_DOT_DOT_DOT);
		_path[i]->m_bDrawFocus = FALSE;
		_path[i]->SetTooltip("Edit the file path.");
		_path[i]->m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	}

	update();



	_inited = true;	 
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void TowerSoundsFiles::dataToCtrls()
{
	for (int i=0;i<PROFILE_BELL_SOUND_CTRL_COUNT;i++)
	{
		CString str;
		str.Format("%d", rowToBell(i)+1);
		*_bell[i] = str;

		str.Format("%s", (rowToStroke(i) == hand)?"hand":"back");
		*_stroke[i] = str;
	}	 

	UpdateData(FALSE);	  	

	InvalidateRect(CRect(65, 55, 285, 240));
}


int TowerSoundsFiles::rowToBell(int row)
{
	return ((_sbLines.GetScrollPos() + row)/2);
}

Stroke TowerSoundsFiles::rowToStroke(int row)
{
	return GlobalFunctions::isEven(_sbLines.GetScrollPos() + row)? hand : back;
}	
																		 
void TowerSoundsFiles::ctrlsToData()
{
	UpdateData();

}


void TowerSoundsFiles::OnChangeVal() 
{
	if (_inited)
		ctrlsToData();	
}


void TowerSoundsFiles::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	ctrlsToData();

	SCROLLINFO ScrollInfo;
	_sbLines.GetScrollInfo(&ScrollInfo);  // get information about the scroll
	int pos = _sbLines.GetScrollPos() ;
	switch(nSBCode)  
	{ 
    case SB_LINEDOWN:       //Scrolls one line down. 
		pos++;  
		break;
    case SB_LINEUP:         //Scrolls one line up. 
		pos--;      
		break;
    case SB_PAGEDOWN:       //Scrolls one page down. 
		pos += ScrollInfo.nPage;      
		break;
    case SB_PAGEUP:         //Scrolls one page up. 
		pos -= ScrollInfo.nPage;      
		break;
    case SB_THUMBTRACK:     //The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button. The nPos parameter indicates the position that the scroll box has been dragged to. 
		pos = nPos;      
		break;
	}
	_sbLines.SetScrollPos(pos);

	dataToCtrls();

	updateIcons();
	
	CBCGPPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
	
}

void TowerSoundsFiles::OnDetails0() 
{
	OnDetails(rowToBell(0), rowToStroke(0));		
}
void TowerSoundsFiles::OnDetails1() 
{
	OnDetails(rowToBell(1), rowToStroke(1));		
}
void TowerSoundsFiles::OnDetails2() 
{
	OnDetails(rowToBell(2), rowToStroke(2));		
}
void TowerSoundsFiles::OnDetails3() 
{
	OnDetails(rowToBell(3), rowToStroke(3));		
}
void TowerSoundsFiles::OnDetails4() 
{
	OnDetails(rowToBell(4), rowToStroke(4));		
}
void TowerSoundsFiles::OnDetails5() 
{
	OnDetails(rowToBell(5), rowToStroke(5));		
}
void TowerSoundsFiles::OnDetails6() 
{
	OnDetails(rowToBell(6), rowToStroke(6));		
}
void TowerSoundsFiles::OnDetails7() 
{
	OnDetails(rowToBell(7), rowToStroke(7));		
}
void TowerSoundsFiles::OnDetails(int bell, Stroke stroke) 
{
	_internalTower.showMethodInfo(bell, stroke, this);
}

void TowerSoundsFiles::OnPath0() 
{
	OnPath(rowToBell(0), rowToStroke(0));		
}
void TowerSoundsFiles::OnPath1() 
{
	OnPath(rowToBell(1), rowToStroke(1));		
}
void TowerSoundsFiles::OnPath2() 
{
	OnPath(rowToBell(2), rowToStroke(2));		
}
void TowerSoundsFiles::OnPath3() 
{
	OnPath(rowToBell(3), rowToStroke(3));		
}
void TowerSoundsFiles::OnPath4() 
{
	OnPath(rowToBell(4), rowToStroke(4));		
}
void TowerSoundsFiles::OnPath5() 
{
	OnPath(rowToBell(5), rowToStroke(5));		
}
void TowerSoundsFiles::OnPath6() 
{
	OnPath(rowToBell(6), rowToStroke(6));		
}
void TowerSoundsFiles::OnPath7() 
{
	OnPath(rowToBell(7), rowToStroke(7));		
}

void TowerSoundsFiles::OnPath(int bell, Stroke stroke) 
{
	_internalTower.editPath(bell, stroke, this);
	updateIcons();
	Invalidate();
}

void TowerSoundsFiles::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CFont* oldFont = dc.SelectObject(&app()->_fontStd);
	for (int i=0;i<PROFILE_BELL_SOUND_CTRL_COUNT;i++)
	{
		int y = 57 + i*21;
		dc.DrawText(_internalTower.getTowerBellPersist(rowToBell(i)).getTowerBellItem(rowToStroke(i)).getFile(),
						CRect(68,y ,285,y + 20),
						DT_LEFT|DT_SINGLELINE|DT_VCENTER| DT_NOPREFIX | DT_PATH_ELLIPSIS);
	}

	dc.SelectObject(oldFont);
}

void TowerSoundsFiles::OnTemplateButt() 
{
	UpdateData();

	if (_internalTower.createFromTemplate())
	{
		update();
		//the number may have changed, so update siblings
		VERIFY(QuerySiblings(TOWER_SOUNDS_UPDATE,0) == 0);
	}
}





void TowerSoundsFiles::OnRemoveAll() 
{
	if (AfxMessageBox("Are you sure that you want to remove all wave files?", MB_YESNO) 
		== IDYES)
	{
		_internalTower.removeAllFiles();
		update();
	}

}

void TowerSoundsFiles::OnEdit0() 
{
	OnEdit(rowToBell(0), rowToStroke(0));		
}
void TowerSoundsFiles::OnEdit1() 
{
	OnEdit(rowToBell(1), rowToStroke(1));		
}
void TowerSoundsFiles::OnEdit2() 
{
	OnEdit(rowToBell(2), rowToStroke(2));		
}
void TowerSoundsFiles::OnEdit3() 
{
	OnEdit(rowToBell(3), rowToStroke(3));		
}
void TowerSoundsFiles::OnEdit4() 
{
	OnEdit(rowToBell(4), rowToStroke(4));		
}
void TowerSoundsFiles::OnEdit5() 
{
	OnEdit(rowToBell(5), rowToStroke(5));		
}
void TowerSoundsFiles::OnEdit6() 
{
	OnEdit(rowToBell(6), rowToStroke(6));		
}
void TowerSoundsFiles::OnEdit7() 
{
	OnEdit(rowToBell(7), rowToStroke(7));		
}
void TowerSoundsFiles::OnEdit(int bell, Stroke stroke)  
{

	TowerBellItem& item = _internalTower.getTowerBellPersist(bell).getTowerBellItem(stroke);

	if (item.getFile().IsEmpty())
		return;

	//check to see if we have a proper wave header
	if (item.isValid())
	{
		//set the test bell
		if ((_testBell != bell)||(_testStroke != stroke))
			OnTest(bell, stroke);

		TowerAdvancedDlg dlg(this, item);
		dlg.DoModal();
		updateIcons();
	}	
	else
	{
		AfxMessageBox("Invalid wave file", MB_ICONHAND);
	}

}


void TowerSoundsFiles::OnTest0() 
{
	OnTest(rowToBell(0), rowToStroke(0));		
}
void TowerSoundsFiles::OnTest1() 
{
	OnTest(rowToBell(1), rowToStroke(1));		
}
void TowerSoundsFiles::OnTest2() 
{
	OnTest(rowToBell(2), rowToStroke(2));		
}
void TowerSoundsFiles::OnTest3() 
{
	OnTest(rowToBell(3), rowToStroke(3));		
}
void TowerSoundsFiles::OnTest4() 
{
	OnTest(rowToBell(4), rowToStroke(4));		
}
void TowerSoundsFiles::OnTest5() 
{
	OnTest(rowToBell(5), rowToStroke(5));		
}
void TowerSoundsFiles::OnTest6() 
{
	OnTest(rowToBell(6), rowToStroke(6));		
}
void TowerSoundsFiles::OnTest7() 
{
	OnTest(rowToBell(7), rowToStroke(7));		
}

void TowerSoundsFiles::OnTest(int bell, Stroke stroke, bool forceStop)  
{
	if	((forceStop)||
		((_testBell == bell)&&(_testStroke == stroke)))
	{
		//turnOff
		_testBell = -1;
		KillTimer( AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER);
	}
	else
	{
		_testBell = bell;
		_testStroke = stroke;

		int time = 2000;
		const TowerBellPersist& towerBell= _internalTower.getTowerBellPersist(_testBell);
		Riff riff;
		CString file;
		if (riff.readRiffHeader(towerBell.getTowerBellItem(_testStroke).getFile(), file))
		{
			time = ((riff.wave.dataHead.dwSize *1000 )/ riff.wave.fmtDetails.dwSRate) + 200;
		}


		OnTimer(AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER);
		SetTimer(AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER, time, NULL); 
	}

	updateIcons();
}

void TowerSoundsFiles::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	if (nIDEvent == AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER)
	{
		if (_testBell == -1)
		{
			KillTimer( AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER);
		}
		else
		{
			//get the bell
			if ((_internalTower.getTowerBell(_testBell, _testStroke).getFile().IsEmpty())||
				(!playManager()->playTowerBell(_internalTower.getTowerBell(_testBell, _testStroke))))
			{
				//turnOff
				_testBell = -1;
				KillTimer( AAA_TOWER_SOUNDS_FILES_ADVANCES_TIMER);
				Invalidate();
			}
		}
	}
}

BOOL TowerSoundsFiles::OnKillActive()
{
	OnTest(-1,hand,true);

	return CBCGPPropertyPage::OnKillActive();
}

BOOL TowerSoundsFiles::OnSetActive()
{
	update();

	return CBCGPPropertyPage::OnSetActive();
}

LRESULT TowerSoundsFiles::OnQuerySiblings( WPARAM wParam, LPARAM /*lParam */)
{
	if (wParam == TOWER_SOUNDS_UPDATE)
		update();

	return 0;
}
								   
