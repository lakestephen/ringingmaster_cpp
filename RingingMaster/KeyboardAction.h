#pragma once

class KeyboardAction
{
public:
	KeyboardAction(DWORD scancode);
	KeyboardAction();
	~KeyboardAction(void);

	virtual CString getDescription()const;

	bool operator ==(const KeyboardAction & multiBellAction)	const;
	virtual void Serialize(CArchive &ar);

	int getScancode() const;
	void setScancode(DWORD scancode);
	
	bool isValid() const ;

protected:
	DWORD _scancode;

};

template<>
	inline UINT AFXAPI HashKey(const KeyboardAction& key)
{
	//create a hash of the values
	return key.getScancode();
}
