#pragma once



// C32BitScrollView view

class CScrollView32Bit : public CScrollView
{
	DECLARE_DYNCREATE(CScrollView32Bit)

protected:
	CScrollView32Bit();           // protected constructor used by dynamic creation
	virtual ~CScrollView32Bit();

public:

	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);


	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnDraw(CDC* /*pDC*/);
};


