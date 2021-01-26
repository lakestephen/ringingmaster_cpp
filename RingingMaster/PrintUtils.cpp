// PrintUtils.cpp: implementation of the PrintUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PrintUtils.h"

#include "CPage.h"


double PrintUtils::_AnalysisMargin = 1.8;
double PrintUtils::_AnalysisIndent = 0.2;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrintUtils::PrintUtils()
{

}

PrintUtils::~PrintUtils()
{

}

void PrintUtils::printFootnote(CPage *page, CString pagesStr, const double margin)
{
	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont=page->SetFont("Arial");

	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	

	int lineWidth = page->ConvertToMappedUnits(.01,VERTRES); //guess at the width of 2 pixels
	double linePos = margin - .15;
	page->Line(page->getLengthInches() - linePos, column,page->getLengthInches() - linePos, 
							page->getWidthinches(), lineWidth);
	
	double textPos = margin - .20;
	page->m_PrtDesc._doPrintClip = false;
	CString sTitle;
	sTitle.LoadString(AFX_IDS_APP_TITLE);
	sTitle += "®";
	page->Print(page->getLengthInches() - textPos, column, TEXT_BOLD, 8, sTitle);
	
	CString user = app()->GetProfileString("MainFrame","User Name", "");

	if (user.IsEmpty())
		user = "Unregistered";
	
	CTime time(CTime::GetCurrentTime());	//does not need to be COleDateTime

	CString date = time.Format("%#c");
	CString output;
	output.Format("(Registered to %s)   %s", user, date);

	page->m_PrtDesc._doPrintClip = false;
	page->Print(page->getLengthInches() - textPos, page->GetNextLogicalColumn(TRUE, TRUE), 0, 8, output);

	//have we got room for the page number?
	double width = page->getTextWidth(pagesStr);
	double position = page->GetNextLogicalColumn(TRUE, TRUE);
	double pageWidth = page->getWidthinches();

	if ((pageWidth - position) > width)
	{
		page->m_PrtDesc._doPrintClip = false;
		page->Print(page->getLengthInches() - textPos, 0.0, TEXT_RIGHT, 8, pagesStr);
	}

	page->SetFont(oldFont);
	page->SetColor(oldColor);
}


void PrintUtils::printShareware(CPage *page)
{
	//do the shareware stuff
	if (mainFrame()->verifyKey() == FALSE)		
	{
		for (int i=0;i<9; i+=3)
		{
			CString oldFont = page->SetFont("Arial");
			COLORREF oldColor = page->SetColor(RGB(0,0,0));
			page->PrintRotatedText(0.3 + i, 0.3, 17.0,17.0,TEXT_NOCLIP,26,"Printed by Ringing Master ",3300);
			page->PrintRotatedText(1.3 + i, 0.3, 17.0,17.0,TEXT_NOCLIP,26,"*******UNREGISTERED*******",3300);
			page->SetFont(oldFont);
			page->SetColor(oldColor);
		}
	}
}


double PrintUtils::printTitle(CPage *page, CString title, double nextRow, bool mainTitle) 
{
	//title is the section name
	
	COLORREF oldColor = page->SetColor(RGB(0,0,0));
	CString oldFont = page->SetFont("Arial");

	const double column = (double)((double)(app()->GetProfileInt("Print", "Touch Left", 1000))/2540.0);	


	nextRow = page->Print(nextRow, column,TEXT_BOLD, (mainTitle)?16:12, title);

	if (!mainTitle)
	{
		int lineWidth = page->ConvertToMappedUnits(0.01,VERTRES); //guess at the width of 2 pixels
		page->Line(nextRow, column,nextRow, 
								page->getWidthinches(), lineWidth);
		nextRow += .02;  
	}				   

	page->SetFont(oldFont);
	page->SetColor(oldColor);

	return nextRow + .08;
}	  


double PrintUtils::getFootnoteHeight()
{
	return 0.35;
}
