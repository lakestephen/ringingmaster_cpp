// DoveExtractorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DoveExtractor.h"
#include "DoveExtractorDlg.h"
#include "DoveTowerPersistExtra.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoveExtractorDlg dialog

CDoveExtractorDlg::CDoveExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoveExtractorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDoveExtractorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDoveExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoveExtractorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDoveExtractorDlg, CDialog)
	//{{AFX_MSG_MAP(CDoveExtractorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXTRACT, OnExtract)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoveExtractorDlg message handlers

BOOL CDoveExtractorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDoveExtractorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDoveExtractorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDoveExtractorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDoveExtractorDlg::OnExtract() 
{
	CString fileName = AfxGetApp()->GetProfileString("Settings", "fileName", "");
	CFileDialog dlg(TRUE, "txt", fileName , NULL, 
					"Text Files (*.txt)|*.txt|All Files (*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		fileName = dlg.GetPathName();
		AfxGetApp()->WriteProfileString("Settings", "fileName", fileName);

		char ch[1000000];
		CString buffer;

		//read the file in
		CFile file(fileName, CFile::modeRead);
		int count = file.Read(ch, 1000000);
		if (count > 999900) AfxMessageBox("Load problem: Over 1Mb");
		buffer = ch;
		buffer = buffer.Left(count);
		file.Close();

		CWaitCursor wait;

		//split up the strings into tower lines
		CStringArray lines;
		CString extracted;
		int start = 0;		
		for (int i=0;i<buffer.GetLength();i++)
		{
			//index through each char
			if (buffer[i] == 10)
			{
				extracted = buffer.Mid(start, i-start);
				start = i;
				while (extracted.Remove(13));
				while (extracted.Remove(10));
				lines.Add(extracted);
				//TRACE("START" + extracted + "END\n");
			}
		}

		DoveTowerPersistExtras doveTowerPersists;

		//now split each line into parts
		for (int i=1;i<lines.GetSize();i++) //ignore the first index line
		{				  
			DoveTowerPersistExtra* doveTowerPersist = new DoveTowerPersistExtra();
			doveTowerPersist->init();
			doveTowerPersists.Add(doveTowerPersist);

			CString strGridRef;

			CString line = lines.GetAt(i);
			int index = 0;
			start = 0;
			for (int j=0;j<line.GetLength();j++)
			{
				//index through each char
				if ((line[j] == '\\')||(j == line.GetLength()-1))
				{
					extracted = line.Mid(start, j-start+1);
					while (extracted.Remove('\\'));
					start = j;

					switch(index++)
					{
					case 0: //Index
						doveTowerPersist->_strIndex = extracted;			
						break;	
					case 1: //ExpRef
						strGridRef = extracted;
						break;	
					case 2: //NG
						if (strGridRef.IsEmpty())
							strGridRef = extracted;
						break;	
					case 3: //FULL
						doveTowerPersist->_county = extracted;		
						break;	
					case 4: //Place
						doveTowerPersist->_name = extracted;			
						break;	
					case 5: //Dedicn
						doveTowerPersist->_dedication = extracted;	
						break;	
					case 6: //Postcode
						doveTowerPersist->_postCode = extracted;		
						break;	
					case 7: //GF
						doveTowerPersist->_groundFloor = (extracted == "G");	
						break;	
					case 8: //Bells
						doveTowerPersist->_number = atoi(extracted);		
						break;	
					case 9: //cwt
						doveTowerPersist->_cwt = atoi(extracted);			
						break;	
					case 10: //qr
						doveTowerPersist->_quarters = atoi(extracted);		
						break;	
					case 11: //lb
						doveTowerPersist->_lb = atoi(extracted);			
						break;	
					case 12: //App
						doveTowerPersist->_approx = (extracted == "approx");		
						break;	
					case 13: //Note
						{
							if(extracted.IsEmpty()) //no entry
								doveTowerPersist->_note = n_none;

							else if(extracted == "AF")
								doveTowerPersist->_note = n_a_flat;
							else if(extracted == "A")
								doveTowerPersist->_note = n_a;
							else if(extracted == "A#")
								doveTowerPersist->_note = n_a_sharp;

							else if(extracted == "BF")
								doveTowerPersist->_note = n_b_flat;
							else if(extracted == "B")
								doveTowerPersist->_note = n_b;
							else if(extracted == "B#")
								doveTowerPersist->_note = n_b_sharp;

							else if(extracted == "CF")
								doveTowerPersist->_note = n_c_flat;
							else if(extracted == "C")
								doveTowerPersist->_note = n_c;
							else if(extracted == "C#")
								doveTowerPersist->_note = n_c_sharp;

							else if(extracted == "DF")
								doveTowerPersist->_note = n_d_flat;
							else if(extracted == "D")
								doveTowerPersist->_note = n_d;
							else if(extracted == "D#")
								doveTowerPersist->_note = n_d_sharp;

							else if(extracted == "EF")
								doveTowerPersist->_note = n_e_flat;
							else if(extracted == "E")
								doveTowerPersist->_note = n_e;
							else if(extracted == "E#")
								doveTowerPersist->_note = n_e_sharp;

							else if(extracted == "FF")
								doveTowerPersist->_note = n_f_flat;
							else if(extracted == "F")
								doveTowerPersist->_note = n_f;
							else if(extracted == "F#")
								doveTowerPersist->_note = n_f_sharp;

							else if(extracted == "GF")
								doveTowerPersist->_note = n_g_flat;
							else if(extracted == "G")
								doveTowerPersist->_note = n_g;
							else if(extracted == "G#")
								doveTowerPersist->_note = n_g_sharp;
							else
								ASSERT(FALSE);
							break;	
						}
					case 14: //Prac
						{
							//Sunday is the index 1
							//Saturday is 7


							if(extracted.IsEmpty())
								doveTowerPersist->_night = now_none;
							else
							{								
								if (extracted.Find("-") != -1)
									doveTowerPersist->_night = now_sun_minus;
								else if (extracted.Find("+") != -1)
									doveTowerPersist->_night = now_sun_plus;
								else 
									doveTowerPersist->_night = now_sun;

								while (extracted.Remove('+'));
								while (extracted.Remove('-'));

								//left with just number 
								doveTowerPersist->_night = 
										(Night) (doveTowerPersist->_night + ((atoi(extracted)-1)*3)  );
							}
						}
						break;	
					case 15: //UR
						doveTowerPersist->_unringable = (extracted == "Unringable");   
						break;	
					case 16: //WebPage
						doveTowerPersist->_webPage = extracted;		
						break;	
					case 17: //CountryCode
						{
							if(extracted.IsEmpty()) //no entry
								doveTowerPersist->_country = c_england;
							else if(extracted == "1")
								doveTowerPersist->_country = c_wales;
							else if(extracted == "A")
								doveTowerPersist->_country = c_scotland;
							else if(extracted == "U")
								doveTowerPersist->_country = c_usa;
							else if(extracted == "D")
								doveTowerPersist->_country = c_australia;
							else if(extracted == "5")
								doveTowerPersist->_country = c_chanel_isl;
							else if(extracted == "E")
								doveTowerPersist->_country = c_new_zealand;
							else if(extracted == "B")
								doveTowerPersist->_country = c_ireland;
							else if(extracted == "C")
								doveTowerPersist->_country = c_south_africa;
							else if(extracted == "K")
								doveTowerPersist->_country = c_kenya;
							else if(extracted == "T")
								doveTowerPersist->_country = c_pakistan;
							else if(extracted == "Z")
								doveTowerPersist->_country = c_zimbabwe;
							else if(extracted == "F")
								doveTowerPersist->_country = c_canada;
							else if(extracted == "4")
								doveTowerPersist->_country = c_isle_of_man;
							else if(extracted == "P")
								doveTowerPersist->_country = c_spain;
							else if(extracted == "S")
								doveTowerPersist->_country = c_india;
							else
								ASSERT(FALSE);
						}
						break;	
					default: 
						ASSERT(FALSE);
					}
				}
			}

			if ((doveTowerPersist->_country == c_england)||
				(doveTowerPersist->_country == c_wales)||
				(doveTowerPersist->_country == c_scotland)||
				(doveTowerPersist->_country == c_isle_of_man))
			{
				ASSERT("This has been removed from teh project!!!! - use source control to reinstate");
				//doveTowerPersist->_gridRef.set10Numeric(strGridRef);
			}
			else
			{
				doveTowerPersist->_gridRef.setNonOSRef(strGridRef);
			}
		}

		//now serialise to file

		CFile fileOut;
		CString path = "towers.dat";
		if(!fileOut.Open(path, CFile::modeWrite | CFile::modeCreate ))
		{
			CString msg;
			msg.Format("ERROR: Unable to open %s for save", path);
			AfxMessageBox(msg);
			TRACE("********SAVE:Data File not found********\n");
			return;
		}

		fileOut.SetLength(0);

		CArchive ar(&fileOut, CArchive::store);


		//set the metric weight

		for(int i=0;i<doveTowerPersists.GetSize();i++)
		{
			doveTowerPersists.GetAt(i)->_index = i+1; //do not use 0
			doveTowerPersists.GetAt(i)->calculateWeight();
		}

		//archive the doveTowers
		ar << doveTowerPersists.GetSize();
		for(int i=0;i<doveTowerPersists.GetSize();i++)
		{
			doveTowerPersists.GetAt(i)->Serialize(ar);
			delete doveTowerPersists.GetAt(i);
		}

		//close down
		ar.Flush();
		fileOut.Close();

	}
}
