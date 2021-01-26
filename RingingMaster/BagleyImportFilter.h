#pragma once

class StrikingData;

struct StrikeItem
{
	int _bellNumber;
	DWORD _timestamp;
	CString _comment;
};




class BagleyImportFilter
{
public:
	BagleyImportFilter(void);
public:
	~BagleyImportFilter(void);

	bool loadFile(CWnd* pWnd, StrikingData& strikingData, CString& fileName);

protected:

	bool openFile(CString& fileContents, const CString& path);
	bool extractRows(const CString& fileContents, CStringArray& rows);
	bool getFileName(CString& path, CString& fileName, CWnd* pWnd);
	bool splitStrikes(const CStringArray& rows, CArray <StrikeItem, StrikeItem>& strikeItems);
	CString calculateStats(const CArray <StrikeItem, StrikeItem>& strikeItems);
	bool calculateStrikingData(const CArray <StrikeItem, StrikeItem>& strikeItems, StrikingData& strikingData);
};
