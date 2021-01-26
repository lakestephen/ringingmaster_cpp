
#pragma once


class PrintWhatAnalysis  
{
public:
	int getSectionCount();
	void setFromRegistry();
	void unSetAll();
	void setAll();
	PrintWhatAnalysis();
	virtual ~PrintWhatAnalysis();

	BOOL _bFalseness 	;
	BOOL _bStatus ;	
	BOOL _bSplice ;
	BOOL _bSpliceTransitions ;
	BOOL _bMusic ;	
	BOOL _bMultiBlock ;
	BOOL _bCourse ;
	BOOL _bLeadEnds;
	BOOL _bAllWork ;
	BOOL _bCallsMade;
};

