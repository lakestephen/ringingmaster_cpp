#pragma once

class SpeechAction
{
public:
	SpeechAction(const CString& word	);
	SpeechAction();
	~SpeechAction(void);

	virtual CString getDescription()const;

	bool operator ==(const SpeechAction & speechAction)	const;
	virtual void Serialize(CArchive &ar);

	const CString& getWord() const;
	void setWord(const CString& word);
	
	bool isValid() const ;

protected:
	CString _word;

};

template<>
	inline UINT AFXAPI HashKey(const SpeechAction& key)
{
	//create a hash of the values	   
	return HashKey<LPCTSTR>(key.getWord());
}
