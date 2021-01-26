#pragma once


class CDocManagerEx :
	public CDocManager
{
	DECLARE_DYNAMIC(CDocManagerEx)

public:
	CDocManagerEx(void);
	virtual ~CDocManagerEx(void);

	virtual void RegisterShellFileTypes(BOOL bCompat);
};
