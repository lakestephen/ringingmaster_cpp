// HistoryObj.h: interface for the HistoryObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORYOBJ_H__0037BF76_9B32_11D7_B6D0_000255162CF0__INCLUDED_)
#define AFX_HISTORYOBJ_H__0037BF76_9B32_11D7_B6D0_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class HistoryObj  
{
public:
	HistoryObj(const CString& strTitle, const CString& strURL, const UINT uiCommand);
	virtual ~HistoryObj();

	LPCTSTR GetTitle () const
	{
		return m_strTitle;
	}

	LPCTSTR GetURL () const
	{
		return m_strURL;
	}

	UINT GetCommand () const
	{
		return m_uiCommand;
	}

protected:
	const CString	m_strTitle;
	const CString	m_strURL;
	const UINT		m_uiCommand;
};


#endif // !defined(AFX_HISTORYOBJ_H__0037BF76_9B32_11D7_B6D0_000255162CF0__INCLUDED_)
