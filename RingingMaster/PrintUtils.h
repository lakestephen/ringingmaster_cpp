// PrintUtils.h: interface for the PrintUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTUTILS_H__35530D54_8605_11D7_B6BB_000255162CF0__INCLUDED_)
#define AFX_PRINTUTILS_H__35530D54_8605_11D7_B6BB_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPage;

class PrintUtils  
{
public:
	static double getFootnoteHeight();
	static void printFootnote(CPage* page, CString pagesStr, const double margin);
	static void printShareware(CPage *page);
	static double printTitle(CPage *page, CString title, double nextRow, bool mainTitle);
	PrintUtils();
	virtual ~PrintUtils();

	static double _AnalysisMargin;
	static double _AnalysisIndent;


};

#endif // !defined(AFX_PRINTUTILS_H__35530D54_8605_11D7_B6BB_000255162CF0__INCLUDED_)
