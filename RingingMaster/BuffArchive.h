// BuffArchive.h: interface for the BuffArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFARCHIVE_H__BCC5D627_84C1_4D2C_8797_467FC550A520__INCLUDED_)
#define AFX_BUFFARCHIVE_H__BCC5D627_84C1_4D2C_8797_467FC550A520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BuffArchive : public CArchive
{
public:
	BYTE* getData(int& length);
	void setData(BYTE*, int length);

	BuffArchive(UINT nMode);
	virtual ~BuffArchive();

};

#endif // !defined(AFX_BUFFARCHIVE_H__BCC5D627_84C1_4D2C_8797_467FC550A520__INCLUDED_)
