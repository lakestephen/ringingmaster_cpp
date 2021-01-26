// ImageDoc.cpp : implementation of the ImageDoc class
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "ImageDoc.h"
#include "ImageFormats.h"
#include "ImageView.h"

#include "PrintData.h"
#include ".\imagedoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImageDoc




IMPLEMENT_DYNCREATE(ImageDoc, CDocument)

BEGIN_MESSAGE_MAP(ImageDoc, CDocument)
	//{{AFX_MSG_MAP(ImageDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_IMAGE_ZOOMIN, OnImageZoomin)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ZOOMIN, OnUpdateImageZoomin)
	ON_COMMAND(ID_IMAGE_ZOOMOUT, OnImageZoomout)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ZOOMOUT, OnUpdateImageZoomout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ImageDoc construction/destruction

ImageDoc::ImageDoc():
_image(NULL), 
m_ZoomFactor(1)
{

}

ImageDoc::~ImageDoc()
{
	delete _image;
	_image = NULL;
}

BOOL ImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// ImageDoc serialization

void ImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}	
}

/////////////////////////////////////////////////////////////////////////////
// ImageDoc diagnostics

#ifdef _DEBUG
void ImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void ImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ImageDoc commands

BOOL ImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	   
	//extract the path
	char szDrive[200];
	char szDir[200];
	char szFileName[200];
	char szExt[200];

	_splitpath(lpszPathName, szDrive, szDir, szFileName, szExt);


	ENUM_CXIMAGE_FORMATS type = RingingMasterApp::GetImageType(szExt);

	if (type == CXIMAGE_FORMAT_UNKNOWN)
		return FALSE;
	
	delete _image;
	_image = new CxImage(lpszPathName, type);

	if (!_image->IsValid()){
		AfxMessageBox(_image->GetLastError());
		delete _image;
		_image = NULL;
		return FALSE;
	}

	UpdateAllViews(NULL);

	return TRUE;
}
	/*

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	//multiple images (TIFF/ICO)
	if (image->GetNumFrames()>1){
		CString s;
		s.Format("File with %d images. Read all?",image->GetNumFrames());
		if (AfxMessageBox(s,MB_OKCANCEL)==IDOK){
			
			int j; // points to the document name
			for(j=strlen(filename)-1;j>=0;j--){
				if (filename[j]=='\\'){	j++; break;	}
			}
			// create the documents for the other images
			for(int i=1;i<image->GetNumFrames();i++){
				CDemoDoc *NewDoc=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL);
				if (NewDoc)	{
					CxImage *newImage = new CxImage();
					newImage->SetFrame(i);
					newImage->Load(filename,type);
					NewDoc->image = newImage;
					CString s;
					s.Format("%s (%d)",filename.Mid(j),i+1);
					NewDoc->SetTitle(s);
					NewDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
				}
			}
		}
	}

	// EXIF jpegs
	if (image->GetType() == CXIMAGE_FORMAT_JPG){
		FILE* hfile = fopen(filename,"rb");
		m_exif.DecodeExif(hfile);
		fclose(hfile);
	}

	return TRUE;
*/

CxImage* ImageDoc::getImage()
{
	return _image;
}


BOOL ImageDoc::OnPreparePrinting(CPrintInfo* pInfo, CView* view)
{
	TRACE("OnPreparePrinting\r\n");
	
	
	//a bit of jiggery to force the standard print dialog not to show.
	// see DoPreparePrinting for information
	BOOL holdDirectState = pInfo->m_bDirect;
	if (!_showPrintSetupDlg)
	{
		pInfo->m_bDirect = TRUE;
	}	

	BOOL ret = view->DoPreparePrinting(pInfo); //this is where we get the print dialog

	//go back st the previous state
	pInfo->m_bDirect = holdDirectState ;

	return ret;
}

void ImageDoc::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnBeginPrinting\r\n");

	PrintData* printData = new PrintData;
	pInfo->m_lpUserData = printData;
}

void ImageDoc::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	//only do stuff if we are printing / preview
	if ((pInfo)&&(pDC->IsPrinting()))
	{
		PrintData* printData = (PrintData*)pInfo->m_lpUserData;

		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= printData->_numPages );
		TRACE("ImageDoc::OnPrepareDC  curPage = %d  numPages = %d pInfo->m_bContinuePrinting = %s\r\n",pInfo->m_nCurPage, printData->_numPages,  pInfo->m_bContinuePrinting?"TRUE":"FALSE");
	}
}	 

void ImageDoc::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	TRACE("OnEndPrinting\r\n");

	PrintData* printData = (PrintData*)pInfo->m_lpUserData;
	delete printData;
	
	pInfo->m_lpUserData = NULL;
}

void ImageDoc::OnFilePrintPreview(CView* view) 
{
	BCGPPrintPreview (view);
}

//this is the standard print. - standard print setup
// will be used by the print button on teh 
//  print preview screen, and also external printing
//this should go direct to the print dialog, otherwise the Setup Dlg will be shown twice
void ImageDoc::OnFilePrint() 
{
	_showPrintSetupDlg = TRUE;
	//DO NOT SET _printAnalysis here it is used for both analysis and main print!!!
}

//print from toolbar -  no setup at all
void ImageDoc::OnFilePrintToolbar() 
{
	_showPrintSetupDlg = FALSE;
}

//print frm menu - using setup dialogs
void ImageDoc::OnFilePrintMenu() 
{
	_showPrintSetupDlg = TRUE;
}

void ImageDoc::OnFileSaveAs() 
{
#ifdef _DEBUG

	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT(pTemplate != NULL);

#endif //_DEBUG

	CString title;
	VERIFY(title.LoadString(AFX_IDS_SAVEFILE));

	CString strFilter;
	CString strDefault;

	//FILES
	strFilter += RingingMasterApp::GetImageFileTypes();			

	//all files
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += "|*.*||";

	
	CString fileName;

	CFileDialog dlg(FALSE, 
				"",NULL,
				OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
				strFilter,NULL);


	dlg.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);;
	dlg.m_ofn.lpstrTitle = title;

	if (dlg.DoModal() == IDOK)
	{
		fileName.ReleaseBuffer();

			   
		//extract the path
		char szDrive[200];
		char szDir[200];
		char szFileName[200];
		char szExt[200];

		_splitpath(fileName, szDrive, szDir, szFileName, szExt);

		CString extention = szExt;


		ENUM_CXIMAGE_FORMATS imageFormat = RingingMasterApp::GetImageType(extention);

		if (imageFormat != CXIMAGE_FORMAT_UNKNOWN)
		{
			saveAsImage(fileName, imageFormat);	
		}
		else 
		{
			if (!DoSave(fileName))
			{
				TRACE("Warning: File save-as failed.\n");	
			}
		}

	}
	else
	{
		fileName.ReleaseBuffer();
	}
		
}

bool ImageDoc::saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format)
{
	//file name will be a full path.
	TRACE(fileName + "\r\n");

	ASSERT(!fileName.IsEmpty());
	
	if (fileName.IsEmpty())
		return false;
	 
	//we want the rendered image
	CxImage* image = getImage();

	if (image)
	{
		if (format == CXIMAGE_FORMAT_GIF)
			image->DecreaseBpp(8, true);

		image->Save(fileName, format);
	}

	return true;
}


ImageView* ImageDoc::getImageView()
{
	POSITION viewPosition = GetFirstViewPosition();   
	ImageView* pImageView = NULL;

	while (viewPosition != NULL)   
	{
		CView* pView = GetNextView(viewPosition);      
		pImageView = dynamic_cast<ImageView*>(pView);
		if (pImageView)
			return pImageView; 
	}   
	return NULL;
}

void ImageDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);	
}


void ImageDoc::OnImageZoomin()	//to2do put zoom on the maps
{
	if (m_ZoomFactor>=16) return;
	m_ZoomFactor*=2;
	UpdateAllViews(NULL);
}

void ImageDoc::OnUpdateImageZoomin(CCmdUI *pCmdUI)
{
	if (m_ZoomFactor>=16) pCmdUI->Enable(0);
}

void ImageDoc::OnImageZoomout()
{
	if (m_ZoomFactor<=0.0625) return;
	m_ZoomFactor/=2;
	UpdateAllViews(NULL);
}

void ImageDoc::OnUpdateImageZoomout(CCmdUI *pCmdUI)
{
	if (m_ZoomFactor<=0.125) pCmdUI->Enable(0);
}



void ImageDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

	((MainFrame*) AfxGetMainWnd ())->UpdateMRUFilesList ();
}