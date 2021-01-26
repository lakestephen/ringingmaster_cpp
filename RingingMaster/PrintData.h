// PrintData.h: interface for the PrintData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTDATA_H__7AFA3761_01E7_11D6_BB7F_B77D3323864F__INCLUDED_)
#define AFX_PRINTDATA_H__7AFA3761_01E7_11D6_BB7F_B77D3323864F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MethodWndPrint;

class PrintData  
{
public:
	unsigned int _numPages;
	MethodWndPrint* _methodWndPrt;
	PrintData();
	virtual ~PrintData();

};

#endif // !defined(AFX_PRINTDATA_H__7AFA3761_01E7_11D6_BB7F_B77D3323864F__INCLUDED_)
