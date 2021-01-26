#pragma once

#include "Method.h"	 

class ProofInput;

struct CallPreview
{
public:
	CallPreview()
	{
		  
	}

	CString _name;
	CString _notation;
	CStringArray _notationStrings;
};

typedef CArray<CallPreview*, CallPreview*>  CallPreviewArray;

class MethodPreview : public Method  
{
public:
	MethodPreview();
	virtual ~MethodPreview();
	CallPreviewArray _callPreviews;
	virtual void create(ProofInput* proofInput);

protected:
	int getNextPositionIndex(int index, int changesPerPlainLead);


};
