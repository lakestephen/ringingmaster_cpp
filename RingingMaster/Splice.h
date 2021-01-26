#pragma once


#include "Variance.h"


class Splice  
{
public:
	Splice(Variance variance, int compositionRow, CString spliceShorthand, CString spliceLonghand);
	Splice(const Splice& splice);
	virtual ~Splice();

	Variance _variance;
	int _compositionRow;
	CString _spliceShorthand;
	CString _spliceLonghand; //todo remove longhand (and hopefully shorthand) and replace with better mechanisam
};

typedef CArray<Splice*, Splice*> Splices;

