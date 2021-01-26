#pragma once

class MultiBellAction
{
public:
	MultiBellAction(int channel);
	MultiBellAction();
	~MultiBellAction(void);

	virtual CString getDescription()const;

	bool operator ==(const MultiBellAction & multiBellAction)	const;
	virtual void Serialize(CArchive &ar);

	int getChannel() const;
	void setChannel(int channel);
	
	bool isValid() const ;

protected:
	int _channel;

};

template<>
	inline UINT AFXAPI HashKey(const MultiBellAction& key)
{
	//create a hash of the values
	return key.getChannel();
}
