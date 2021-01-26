// PrintWhatAnalysis.cpp: implementation of the PrintWhatAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "PrintWhatAnalysis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrintWhatAnalysis::PrintWhatAnalysis()
{
	unSetAll();;
}

PrintWhatAnalysis::~PrintWhatAnalysis()
{

}

void PrintWhatAnalysis::setAll()
{
	_bStatus  = TRUE;	
	_bFalseness 	 = TRUE	;
	_bSplice  = TRUE	;
	_bSpliceTransitions  = TRUE	;
	_bMusic  = TRUE		  ;
	_bMultiBlock  = TRUE ;
	_bCourse  = TRUE	  ;
	_bLeadEnds = TRUE	  ;
	_bAllWork  = TRUE	;
	_bCallsMade = TRUE	;
}

void PrintWhatAnalysis::unSetAll()
{
	_bStatus  = FALSE	  ;
	_bFalseness 	 = FALSE ;
	_bSplice  = FALSE	  ;
	_bSpliceTransitions  = FALSE	  ;
	_bMusic  = FALSE	  ;
	_bMultiBlock  = FALSE  ;
	_bCourse  = FALSE	  ;	
	_bLeadEnds = FALSE;
	_bAllWork  = FALSE	  ;
	_bCallsMade = FALSE;


}

void PrintWhatAnalysis::setFromRegistry()
{
	_bFalseness= 	app()->GetProfileInt("Print", "Falseness", FALSE);	
	_bStatus =		app()->GetProfileInt("Print", "Status", TRUE);	
	_bSplice =		app()->GetProfileInt("Print", "Splice", TRUE);	
	_bSpliceTransitions =		app()->GetProfileInt("Print", "Splice Transition", TRUE);	
	_bMusic =		app()->GetProfileInt("Print", "Music", TRUE);	
	_bMultiBlock =	app()->GetProfileInt("Print", "MultiBlock", TRUE);	
	_bCourse =		app()->GetProfileInt("Print", "Course", TRUE);	
	_bLeadEnds=		app()->GetProfileInt("Print", "LeadEnds", TRUE);	
	_bAllWork =		app()->GetProfileInt("Print", "AllWork", TRUE);	
	_bCallsMade =   app()->GetProfileInt("Print", "CallsMade", TRUE);	

}

int PrintWhatAnalysis::getSectionCount()
{
	int sectionCount = 0;

	if(_bStatus) sectionCount++;
	if(_bFalseness) sectionCount++;
	if(_bSplice) sectionCount++;
	if(_bSpliceTransitions) sectionCount++;
	if(_bMusic) sectionCount++;
	if(_bMultiBlock) sectionCount++;
	if(_bCourse) sectionCount++;
	if(_bLeadEnds) sectionCount++;
	if(_bAllWork) sectionCount++;
	if(_bCallsMade) sectionCount++;

	return sectionCount;
}
