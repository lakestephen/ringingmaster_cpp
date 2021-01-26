// NotationExplorerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "NotationExplorerDlg.h"
#include "ExplorerFolder.h"
#include "ImportLog.h"
#include "SearchDlg.h"

#include "Notation.h"
#include "MethodManager.h"
#include "WinPos.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const UINT TIMER_DEVIDER = 1000;



/////////////////////////////////////////////////////////////////////////////
// NotationExplorerDlg dialog


NotationExplorerDlg::NotationExplorerDlg(CWnd* pParent, NotationLites& selection, bool databaseMaintainance, bool filterState, int filterNumber, bool import)
: ResizeDialog(NotationExplorerDlg::IDD, pParent),
_devider(-1),
_deviding(false),
_selection(selection),
_import(import),
_eventWaitDlg(NULL),
_filterState(filterState),
_filterNumber(filterNumber),
_databaseMaintainance(databaseMaintainance),
_flattenedState(false)
{
	_list.setParent(this);
	_tree.setParent(this);

	_flattenedState = (app()->GetProfileInt("Explorer", "Flattened", 0) == TRUE);		

	//{{AFX_DATA_INIT(NotationExplorerDlg)
	_status = _T("");
	//}}AFX_DATA_INIT
}

NotationExplorerDlg::~NotationExplorerDlg()
{
	delete _eventWaitDlg;

	app()->WriteProfileInt("Explorer", "Flattened", _flattenedState );

}

void NotationExplorerDlg::DoDataExchange(CDataExchange* pDX)
{
	ResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NotationExplorerDlg)
	DDX_Control(pDX, IDC_EDIT_CUT, _cutBtn);
	DDX_Control(pDX, IDC_EDIT_PASTE, _pasteBtn);
	DDX_Control(pDX, IDC_EDIT_COPY, _copyBtn);
	DDX_Control(pDX, IDC_FLATTEN_BUTT, _flattenBtn);
	DDX_Control(pDX, IDC_EXPLORE_STATIS, _statusCtrl);
	DDX_Control(pDX, IDOK, _ok);
	DDX_Control(pDX, IDCANCEL, _cancel);
	DDX_Control(pDX, IDC_EXPLORER_FIND, _search);
	DDX_Control(pDX, IDC_FILTER_COMBO, _filterCombo);
	DDX_Control(pDX, IDC_FILTER_BUTT, _filterOnOff);
	DDX_Control(pDX, IDC_TREE, _tree);
	DDX_Text(pDX, IDC_EXPLORE_STATIS, _status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NotationExplorerDlg, ResizeDialog)
	//{{AFX_MSG_MAP(NotationExplorerDlg)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FILTER_BUTT, OnFilterButt)
	ON_CBN_SELCHANGE(IDC_FILTER_COMBO, OnSelchangeFilterCombo)
	ON_BN_CLICKED(IDC_EXPLORER_FIND, OnExplorerFind)
	ON_BN_CLICKED(IDC_FLATTEN_BUTT, OnFlattenButt)
	ON_BN_CLICKED(IDC_EDIT_COPY, OnEditCopy)
	ON_BN_CLICKED(IDC_EDIT_CUT, OnEditCut)
	ON_BN_CLICKED(IDC_EDIT_PASTE, OnEditPaste)

	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_PASTE, OnUpdateEditPaste)

END_MESSAGE_MAP()

LRESULT NotationExplorerDlg::OnKickIdle(WPARAM /*wParam*/, LPARAM /*lParam*/)	
{
	updateStatus();
	//this operates the ON_UPDATE_COMMAND_UI macro whenever there is Idle time.
	UpdateDialogControls(this, TRUE);
	return 0;	
}
/////////////////////////////////////////////////////////////////////////////
// NotationExplorerDlg message handlers


BOOL NotationExplorerDlg::OnInitDialog() 
{
	ResizeDialog::OnInitDialog();
					
	//create the list
	_list.Create(LVS_REPORT | 
                    LVS_SHOWSELALWAYS | LVS_EDITLABELS | LVS_OWNERDATA | LVS_SINGLESEL |
                    WS_VISIBLE | WS_TABSTOP, CRect(0,0,100,100), this, IDC_LIST);
	_list.ModifyStyleEx (0,WS_EX_CLIENTEDGE );



	//fill the combo box
	for (int i=MINBELLS;i<=MAXBELLS;i++)
	{
		_filterCombo.AddString(GlobalFunctions::numberName(i));
	}

	_filterCombo.SetCurSel(_filterNumber - 3);

	if (_databaseMaintainance) 
	{
		_ok.ShowWindow(SW_HIDE);
		CRect rect;

		_cancel.GetWindowRect(&rect);
		ScreenToClient(rect);
		rect.OffsetRect(-30, 0);
		_cancel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
		_cancel.GetWindowRect(&rect);
		_cancel.SetWindowText("Close");

	}
	else
		AddControl(IDOK, repos_center, repos);

	AddControl(IDC_EXPLORE_STATIS, resize, repos);
	AddControl(IDCANCEL, repos_center, repos);

	_search.m_bDrawFocus = FALSE;
	_search.SetTooltip("Search entire database");
	_search.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_search.SetImage(IDB_FIND);
	_search.ShowWindow(SW_SHOW);

	_copyBtn.m_bDrawFocus = FALSE;
	_copyBtn.SetTooltip("Copy");
	_copyBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_copyBtn.SetImage(IDB_COPY);
	
	_cutBtn.m_bDrawFocus = FALSE;
	_cutBtn.SetTooltip("Copy");
	_cutBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_cutBtn.SetImage(IDB_CUT);

	_pasteBtn.m_bDrawFocus = FALSE;
	_pasteBtn.SetTooltip("Paste");
	_pasteBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	_pasteBtn.SetImage(IDB_PASTE);

	_flattenBtn.m_bDrawFocus = FALSE;
	_flattenBtn.SetTooltip("Flatten the hirachy");
	_flattenBtn.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	updateFlattenImage();

	_filterOnOff.m_bDrawFocus = FALSE;
	_filterOnOff.SetTooltip("Filter methods on number of bells");
	_filterOnOff.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	updateFilterImage();

	WinPos::Load(this, "NotationExplorerDlg", true);

	_devider = app()->GetProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos","NotationExplorerDlg Devider", -1);

	CRect rect;
	GetWindowRect(&rect);
	rect.InflateRect(1,1);
	MoveWindow(rect);
	
	_list.init();
	_tree.init();

	if (!_import)
		_tree.loadDirectory();

	_tree.openFolder((long)app()->GetProfileInt("Explorer", "Last Explorer Folder", 0));		
	
	if (_import)
		doImport();


	return TRUE;  // return TRUE unless you set the focus to a control

}

void NotationExplorerDlg::OnSize(UINT nType, int cx, int cy) 
{
	ResizeDialog::OnSize(nType, cx, cy);
	
	if (_devider == -1)
		_devider = cx/3;
	if (_devider > cx - 30)
		_devider = cx - 30;
	if (_devider < 30)
		_devider = 30;

	const int top = 24;
	const int bottom = cy - top - 48;

	if (IsWindow(_list))
		_list.MoveWindow(_devider+2,top,cx - _devider - 1,bottom);
	if (IsWindow(_tree))
		_tree.MoveWindow(0,top,_devider -2,bottom);	 
	if (IsWindow(_statusCtrl))
	    _statusCtrl.InvalidateRect(NULL);

}

BOOL NotationExplorerDlg::DestroyWindow() 
{
	app()->WriteProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos","NotationExplorerDlg Devider", _devider);

	WinPos::Save(this, "NotationExplorerDlg");
		
	
	return ResizeDialog::DestroyWindow();
}			    


BOOL NotationExplorerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rect;
	GetClientRect(&rect);

//	TRACE("point.x %d, _devider %d\n", point.x, _devider);
	
	if (((point.x > _devider - 3)&&
		 (point.x < _devider + 3)&&
		 (point.y > 31)&&
		 (point.y < rect.bottom - 20))||
		 (_deviding))
	{
		SetCursor(app()->LoadStandardCursor(IDC_SIZEWE)); 
		return TRUE;
	}
	else
		return ResizeDialog::OnSetCursor(pWnd, nHitTest, message) ; 
}


void NotationExplorerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(&rect);
	
	if ((point.x > _devider - 3)&&
		(point.x < _devider + 3)&&
		(point.y > 31)&&
		(point.y < rect.bottom - 20))
	{
		_deviding = true;
		SetTimer(TIMER_DEVIDER,5,0);

		//if mouse clicked on divider line, then start resizing
		ClientToScreen(&rect);
		rect.left += 30; 
		rect.right -= 30;
		rect.top += 35; 
		rect.bottom -= 25;

		//do not let mouse leave the list box boundary
		::ClipCursor(rect);
	}

	ResizeDialog::OnLButtonDown(nFlags, point);
}

void NotationExplorerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	TRACE("Left Btn Up \r\n");
	_deviding = false;	
	KillTimer(TIMER_DEVIDER);
	::ClipCursor(NULL);
	Invalidate();

	ResizeDialog::OnLButtonUp(nFlags, point);
}

void NotationExplorerDlg::OnTimer(UINT nIDEvent) 
{
	if ((_deviding)&&(nIDEvent == TIMER_DEVIDER))
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);		
		_devider = point.x;
		CRect rect;
		GetClientRect(&rect);
		OnSize(0,rect.Width(),rect.Height());		
	}
	
	ResizeDialog::OnTimer(nIDEvent);
}
	
void NotationExplorerDlg::OnDestroy() 
{
	_tree.saveDirectory(); 

	_list.SaveColumnWidths();
	if (_tree.GetSelectedItem() != NULL)
		app()->WriteProfileInt("Explorer", "Last Explorer Folder", 
			_tree.GetItemData(_tree.GetSelectedItem()));

	ResizeDialog::OnDestroy();
	
}

void NotationExplorerDlg::makeSelection()
{
	POSITION pos = _list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
//		TRACE0("No items were selected!\n");
	}
	else
	{
		int childMethodCount = _list.getChildFoldersCount();
		while (pos)
		{
			int nItem = _list.GetNextSelectedItem(pos);
			
			NotationLite* not = _list._liteList[nItem - childMethodCount];
			if (not != NULL)
			{
				NotationLite* sel = new NotationLite;
				*sel = *not;
				_selection.Add(sel);					
			}
		}
	}
	EndDialog(IDOK);
}

void NotationExplorerDlg::deleteMethodsInFolder(unsigned long folderID)
{
	for (int i=0;i<methodManager()->getMethodCount();i++)
	{
		if (methodManager()->getMethod(i)->getFolderID() == folderID)
		{
			delete methodManager()->getMethod(i);
			methodManager()->removeMethod(i);
			i--;
		}		
	}	
}


void NotationExplorerDlg::doImport()
{
	//create the import log
	//get file name
	CString fileName ;
	fileName = app()->_rootPath;

	//Init the Filedialog
	CFileDialog dlg(TRUE, "TXT", fileName, OFN_HIDEREADONLY |OFN_ALLOWMULTISELECT);

	//declare my own buffer to get around the 256 char limit
	CString strFileName;
	//set a limit -in character-
	int maxChar = 5000;
	//give dialog a hint
	dlg.m_ofn.lpstrFile = strFileName.GetBuffer(maxChar);
	dlg.m_ofn.nMaxFile = maxChar;
	//show dialog
	if(dlg.DoModal()  != IDOK)
	{
		strFileName.ReleaseBuffer();
		return;
	}
	else
	{
		strFileName.ReleaseBuffer();
		EndDialog(IDOK);	
	}

	CWaitCursor wait;

	addString("Start Import");
	
	_columnNames.Add("Name");
	_columnNames.Add("Notation");
	_columnNames.Add("hl");			//half
	_columnNames.Add("le");			//end
	_columnNames.Add("lh");			//head
	_columnNames.Add("Tower bells");
	_columnNames.Add("Place");
	_columnNames.Add("Handbells");
	_columnNames.Add("CCC");
	_columnNames.Add("PMM");
	_columnNames.Add("TDMM");
	_columnNames.Add("MUG");
	_columnNames.Add("RW ref");

	//clear out the old methods and directorys
	for(int i=0;i<methodManager()->getMethodCount();i++)
	{
		delete methodManager()->getMethod(i);
	}
	methodManager()->removeAllMethods();
	_list.DeleteAllItems();
	_tree.DeleteAllItems();

	_tree.createBlankCCDir();

	DWORD start = timeGetTime();

	_eventWaitDlg = new EventWaitDlg(this);

	POSITION pos = dlg.GetStartPosition();

	int fileCount = 0;

	while (pos != NULL)
	{
		fileCount++;
		
		fileName = dlg.GetNextPathName(pos);

		//load file into string
		CString originalBuff;
		load(originalBuff, fileName);

		CString msg;
		msg.Format("Importing: %s", fileName);
		addString(msg);

		//split up string into seperate CStrings
		CStringArray lines;
		splitString(originalBuff, lines);

		//ignore the first line - copyright
		lines.RemoveAt(0);
		CString str;

		//create the sections from the strings
		MetSections sections;
		extract(lines, sections, fileName);

		//add the methods to the explorer
		addSections(sections);

		//clean up 
		for (int i=0;i<sections.GetSize();i++)
			delete sections.GetAt(i);

	}
	
	//finished
	Beep(100,100);
	addString("End Import");

	CString notationsAdded;
	notationsAdded.Format("Converted %d methods, in %d directories, int %d files, in %d seconds", methodManager()->getMethodCount(), _tree.GetCount(), fileCount, (timeGetTime() - start)/1000);
	addString(notationsAdded);

	delete _eventWaitDlg;
	_eventWaitDlg = NULL;

	ImportLog importLog(_logStrings, this);
	importLog.DoModal();

	EndDialog(IDOK);	
}

bool NotationExplorerDlg::splitString(CString &originalBuff, CStringArray &lines)
{
	int start = 0;
	CString extracted;
	int i;
	for (i=0;i<originalBuff.GetLength();i++)
	{
		//index through each char
		if (originalBuff[i] == 10)
		{
			
			extracted = originalBuff.Mid(start, i-start);
			start = i;
			while (extracted.Remove(13));
			while (extracted.Remove(10));
			lines.Add(extracted);
			//TRACE("START" + extracted + "END\n");
		}	
	}

	ASSERT (start > i -2);
	return true;
}

void NotationExplorerDlg::load(CString& originalBuff, CString fileName)
{
	char ch[1000000];

	CFile file(fileName, CFile::modeRead);
	int count = file.Read(ch, 1000000);
	if (count > 999900) AfxMessageBox("Load problem: Over 1Mb");
	originalBuff = ch;
	originalBuff = originalBuff.Left(count);
	file.Close();
}

void NotationExplorerDlg::extract(CStringArray& lines, MetSections& sections, CString fileName)
{
	LOOKINGFOR lookingFor = EM_TYPE;

	MetSection* section = NULL;
	CString longName;

	for (int i=0;i<lines.GetSize();i++)
	{				  
		CString line = lines.GetAt(i);
		switch (lookingFor)
		{
		case EM_TYPE:
			{
				if (!line.IsEmpty())
				{
					section = new MetSection;
					sections.Add(section);
					section->_type = line;
					//addString(line);
					//addString("Section: " + line);
					TRACE("Section: " + line + "\r\n");
					section->_fileName = fileName;
					lookingFor = EM_HEAD;
				}
			}
			break;
		case EM_HEAD:
			{
				if (line.Mid(0,4) == " No.")
				{
					//find column starts
					for (int j=0;j<_columnNames.GetSize();j++)
					{
						int column = line.Find(_columnNames.GetAt(j));
						if (column != -1)
						{
							section->_columnNames.Add(_columnNames.GetAt(j));
							section->_columnNum.Add(column);
							
							/*CString str;
							str.Format("%d %s",column, _columnNames.GetAt(j));							
							addString(str);*/
						}
					}

					section->_columnNum.Add(line.GetLength()); //the last col

					lookingFor = EM_METH;
				}						 
				else 
				{
					//cope with trebles path being on the next line
					CString lineCopy = line;
					lineCopy.TrimLeft(' ');
					if ((lineCopy.GetLength() >0)&&
						(lineCopy[0] == '('))
						section->_type += line;
				}
			}
			break;
		case EM_METH:
			{
				if (!line.IsEmpty())
				{
					lookingFor = EM_END;
					i--; //process again in the next section
				}
			}
			break;
		case EM_END:
			{
				if (line.IsEmpty())
				{
					lookingFor = EM_TYPE ;
				}
				else
				{
					bool success = fillMet(line, *section);
					if (!success)
					{
						longName = line;
					}
					else 
					{
						//if we have a name from the previous line, and no name for this methjod, we have a split line method
						if (!longName.IsEmpty())
						{
							if (section->getStr(section->_mets.GetUpperBound(), "Name").IsEmpty())
								section->setStr(section->_mets.GetUpperBound(), "Name", longName);
							else
								addString("**MethodError: " + longName);
						}
						
						longName.Empty();
					}
				}
			}
			break;
		}
	}

}	 



bool NotationExplorerDlg::fillMet(CString &line, MetSection &section)
{
	//here we split up each method into its component parts

	if ((section._columnNum.GetSize() < 3)||
		(line.GetLength() < section._columnNum.GetAt(2)))
	{
		return false;
	}
	else
	{
		Met* met = new Met;
		section._mets.Add(met);

		for (int i=0;i<section._columnNum.GetUpperBound();i++)
		{
			int start = section._columnNum.GetAt(i);
			int end = section._columnNum.GetAt(i+1);
			CString fragment;
			if (line.GetLength() < end)
				fragment = "";
			else
				fragment = line.Mid(start, end - start);

			fragment.TrimLeft(' ');
			fragment.TrimRight(' ');
			met->_strs.Add(fragment);
		}
		return true;
	}
}

void NotationExplorerDlg::addString(CString str)
{
	_logStrings.Add(str);
	
	TRACE(str + "\r\n");

}

void NotationExplorerDlg::addSections(MetSections &sections)
{
	int unnamedCount = 0;;
	
	CString msg;
	for (int i=0;i<sections.GetSize();i++)
	{
		MetSection* section = sections.GetAt(i);
		//check we have minimum columns
		bool bName = false;
		bool bNotation = false;
		for (int k=0;k<section->_columnNames.GetSize();k++)
		{
			if (section->_columnNames.GetAt(k) == "Name")
				bName = true;
			if (section->_columnNames.GetAt(k) == "Notation")
				bNotation = true;
		}

		if (bName && bNotation && section->init())
		{
			//get the folder
			long folderID = _tree.getSectionFolderID(section);				
	
				
			for (int j=0;j<section->_mets.GetSize();j++)
			{
				//create a Notation from the data
				if (section->getStr(j, "Name").IsEmpty())
				{
					CString name;
					name.Format("Unnamed%d", ++unnamedCount);
					section->setStr(j, "Name", name);
				}

				Notation* notation = new Notation;
				notation->create(FALSE, 
								section->getStr(j, "Name") + " " + section->_strSuffex,
								section->getNumber(),
								!section->getStr(j,"hl").IsEmpty(),
								section->getStr(j, "Notation") + section->getStr(j,"hl"),
								section->getStr(j, "le"));
							
				if (notation->verify(msg))
				{
					//add the detail //do not add lead head - it is calculated
					notation->setCCInfo(cc_towerBells, section->getStr(j, "Tower bells"));
					notation->setCCInfo(cc_place	 , section->getStr(j, "Place"));
					notation->setCCInfo(cc_handbells , section->getStr(j, "Handbells"));
					notation->setCCInfo(cc_CCC		 , section->getStr(j, "CCC"));
					notation->setCCInfo(cc_PMM		 , section->getStr(j, "PMM"));
					notation->setCCInfo(cc_TDMM		 , section->getStr(j, "TDMM"));
					notation->setCCInfo(cc_MUG		 , section->getStr(j, "MUD"));
					notation->setCCInfo(cc_RW		 , section->getStr(j, "RW ref"));


					//this code is causing problems with stedman

					/*
					//perform conversion to symmetrical is poss
					if(notation->checkForSymmetry(true))
						addString("Symmetry Conversion: "  + notation->getName(true));
					*/

					//perform a check for lead end correct
					if (section->getStr(j, "lh") != notation->getLeadHead())
					{
						addString(notation->getName(true) + "  " + 
							  section->getStr(j, "lh") + "  " +  
							  notation->getLeadHead());// + "\r\n");
					}					

					//add notation
					NotationLite* lite = new NotationLite;
					*lite = *notation;
					delete notation;
					lite->setFolderID(folderID);
					methodManager()->addMethod(lite);
				}
				else
				{
					addString("**Method Error: "  + msg);
					delete notation;
				}
			}
		}
		else
		{
			msg.Format("**Section error: %s", section->_type);
			addString(msg);
		}
	}
}


void NotationExplorerDlg::OnOK() 
{
	makeSelection();
}

void NotationExplorerDlg::updateStatus()
{
	int nInFolder = _list.GetItemCount() - _list.getChildFoldersCount();
	_status.Format("%d method%s in folder, %d method%s selected, %d methods in library.",
		nInFolder ,
		(nInFolder  == 1)?"":"s",
		_list.getSelectedCount(),
		(_list.getSelectedCount() == 1)?"":"s",
		methodManager()->getMethodCount());

	UpdateData(FALSE);
	
}

void NotationExplorerDlg::OnFilterButt() 
{	  	
	_filterState = !_filterState;
	updateFilterImage();
	_list.showMethods();
}

void NotationExplorerDlg::updateFilterImage()
{
	if (_filterState)
	{
		_filterOnOff.SetImage(IDB_FILTER);
		_filterCombo.ShowWindow(SW_SHOW);
	}
	else
	{
		_filterOnOff.SetImage(IDB_NO_FILTER);
		_filterCombo.ShowWindow(SW_HIDE);	
	}
}

void NotationExplorerDlg::updateFlattenImage()
{
	_flattenBtn.SetImage(_flattenedState?IDB_FLATTEN_RED:IDB_FLATTEN);
	_flattenBtn.Invalidate();

}

void NotationExplorerDlg::OnSelchangeFilterCombo() 
{
	_filterNumber = _filterCombo.GetCurSel() + 3;
	_list.showMethods();
}

void NotationExplorerDlg::OnExplorerFind() 
{
	SearchDlg dlg(_selection, _filterState?_filterNumber:-1, _databaseMaintainance);
	if ((dlg.DoModal() == IDOK)&&(!_databaseMaintainance))
		EndDialog(IDOK);
}



void NotationExplorerDlg::OnFlattenButt() 
{
	_flattenedState = !_flattenedState;	
	updateFlattenImage();
	_list.showMethods();
}

bool NotationExplorerDlg::isFlattened()
{
	return _flattenedState;

}




void NotationExplorerDlg::OnEditCut() 
{
	::PostMessage(_list.GetSafeHwnd(), WM_COMMAND, ID_EDIT_CUT,0);	
}

void NotationExplorerDlg::OnEditCopy() 
{
	::PostMessage(_list.GetSafeHwnd(), WM_COMMAND, ID_EDIT_COPY,0);
}

void NotationExplorerDlg::OnEditPaste() 
{
	::PostMessage(_list.GetSafeHwnd(), WM_COMMAND, ID_EDIT_PASTE,0);
}

void NotationExplorerDlg::OnUpdateEditCut(CCmdUI* pCmdUI)  
{
	_list.OnUpdateEditCut(pCmdUI);
}

void NotationExplorerDlg::OnUpdateEditCopy(CCmdUI* pCmdUI)  
{
	_list.OnUpdateEditCopy(pCmdUI);
}

void NotationExplorerDlg::OnUpdateEditPaste(CCmdUI* pCmdUI)  
{
	_list.OnUpdateEditPaste(pCmdUI);
}


