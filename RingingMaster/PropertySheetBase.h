#pragma once

/////////////////////////////////////////////////////////////////////////////
// PropertySheetBase

class PropertySheetBase : public CBCGPPropertySheet
{
	DECLARE_DYNAMIC(PropertySheetBase)

// Construction
public:
	PropertySheetBase(CString caption, CWnd* parent, int width = 135);
	virtual ~PropertySheetBase();

	void setConstantText(CString name);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );


protected:
	BOOL setupButton(UINT id, CBCGPButton& button, CString tooltip);

	CBCGPButton _cancle;
	CBCGPButton _ok;
	CBCGPButton _apply;
	CBCGPButton _help;

	CString _caption;

};

