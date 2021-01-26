#pragma once


class SimulatorInput;

class SimulatorInputSelectorWnd : public CWnd
{
	DECLARE_DYNAMIC(SimulatorInputSelectorWnd)

public:
	SimulatorInputSelectorWnd();
	virtual ~SimulatorInputSelectorWnd();

protected:
	DECLARE_MESSAGE_MAP()

	bool _showAddBtn;
	CComboBox _combo;
	CBCGPButton _optionButton;
	CBCGPButton _addButton;
	int _id;

	SimulatorInput* _input;
	CString _text;
	COLORREF _textColor;
	bool _drawLine;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeCmbBox();


	virtual BOOL Create(const CPoint topLeft, CWnd* pParentWnd, UINT nID);

	void update(void);
	void setSimulatorInput(int id, SimulatorInput* input, CString text, COLORREF textColor, bool showAddBtn, bool drawLine);
	void setActiveIcon();
	afx_msg void OnPaint();
	afx_msg void OnPressOptionBtn();
	afx_msg void OnPressAddBtn();
	afx_msg void OnTimer(UINT nIDEvent);
};


