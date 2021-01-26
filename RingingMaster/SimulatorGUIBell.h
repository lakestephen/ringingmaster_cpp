#pragma once

class SimulatorGUIBell
{
public:
	SimulatorGUIBell(void);
	~SimulatorGUIBell(void);

	void setBellRect(const CRect& rect);
	const CRect& getBellRect() const;
	void setText(CString text, bool leftAlign);
	bool isLeftAlign() const;
	const CString& getText() const;

protected:	
	CRect _bellRect;
	CString _text;
	bool _leftAlign;

};
