#include "StdAfx.h"
#include "BagleyImportFilter.h"
#include "StrikingData.h"
#include "Row.h"



BagleyImportFilter::BagleyImportFilter(void)
{
}

BagleyImportFilter::~BagleyImportFilter(void)
{
}


bool BagleyImportFilter::loadFile(CWnd* pWnd, StrikingData& strikingData, CString& fileName)
{

	CString path;
	if (!getFileName(path, fileName, pWnd))
		return false;

	CWaitCursor wait;
	CString fileContents;
	if (!openFile(fileContents, path))
		 return false;

	CStringArray rows;
	if (!extractRows(fileContents, rows))
		return false;	  


	int fileLength = fileContents.GetLength();
	fileContents.Empty();  	
	CArray <StrikeItem, StrikeItem> strikeItems;
	if (!splitStrikes(rows, strikeItems))
		return false; 

	int rowsCount = (int)rows.GetSize();
	rows.RemoveAll();
	CString stats = calculateStats(strikeItems);

	CString str;
	str.Format("File Length %d.\r\nExtracted %d strikes\r\n%s", fileLength, rowsCount, stats);
	AfxMessageBox(str);


	if (!calculateStrikingData(strikeItems, strikingData))
		return false;
	

	return true;
	
}


bool BagleyImportFilter::getFileName(CString& path, CString& fileName, CWnd* pWnd)
{
	if (AfxMessageBox("This is a special development import tool. Do you want to continue?\n\n Issue 1.\n <Bell No.><time stamp in ms>[ascii message]<CR><LF>", MB_YESNO ) == IDYES)
	{
		CFileDialog dlg(TRUE, "wav", NULL, NULL, 
			"Bagley Timestamp Files (*.txt)|*.txt|All Files (*.*)|*.*|", pWnd);

		if (dlg.DoModal() == IDOK)
		{
			path = dlg.GetPathName();

			CFileFind find;
			if (find.FindFile(path))
			{
				find.FindNextFile();
				fileName = find.GetFileTitle();
				return true;
			}
		}
	}

	return false;
}

bool BagleyImportFilter::openFile(CString& fileContents, const CString& path)
{
	CFile file;
	if (!file.Open(path,CFile::modeRead))
	{
		return false;
	}
	
	const int size = 1024;
	BYTE* buff = new BYTE[size+1];
	UINT count = 0;
	do
	{
		memset(buff,0,size+1);
		count = file.Read(buff, size);
		fileContents += CString(buff);
									
	} while (count > 0);

	file.Close();

	//convert to CString
	delete [] buff;

	return true;

}


bool BagleyImportFilter::extractRows(const CString& fileContents, CStringArray& rows)
{
	int index = 0;

	//extract the rows
	while (true)
	{
		int newIndex = fileContents.Find("\n", index);

		if (newIndex == -1)
			break;

		CString row = fileContents.Mid(index, newIndex - index +1);

		row.Remove('\r');
		row.Remove('\n');
		
		if (row.GetLength() >= 9)
		{
			rows.Add(row);
			//TRACE("Adding Row '%s'\r\n", row);
		}
		else
			TRACE("Not Adding Row %s \r\n", row);

		index = newIndex +1;
	}

	return true;
}

bool BagleyImportFilter::splitStrikes(const CStringArray& rows, CArray <StrikeItem, StrikeItem>& strikeItems)
{
	for (int i=0;i<rows.GetSize();i++)
	{
		CString row = rows.GetAt(i);
		StrikeItem item;
		
		//number
		char number = row[0];
		CString timestamp = row.Mid(1,8);
		if (row.GetLength() > 9)
		{
			item._comment = row.Mid(9,row.GetLength()-9);
			item._comment.TrimLeft();
			item._comment.TrimRight();
		}

		item._bellNumber = GlobalFunctions::charToBellNumbers(number);
		item._timestamp = atol(timestamp);

		strikeItems.Add(item);
	}

	return true;
}


CString BagleyImportFilter::calculateStats(const CArray <StrikeItem, StrikeItem>& strikeItems)
{
	CString msg;
	CString temp;

	int values[MAXBELLS];

	for (int i=0;i<MAXBELLS;i++)
		values[i] = 0;

	for (int i=0;i<strikeItems.GetSize();i++)
	{
		StrikeItem strikeItem = strikeItems.GetAt(i);

		int bellNumber = strikeItem._bellNumber - 1;

		ASSERT(bellNumber >=0 && bellNumber < MAXBELLS);

		if (bellNumber >=0 && bellNumber < MAXBELLS)
			values[bellNumber]++;		
	}

	for (int i=0;i<MAXBELLS;i++)
	{
		if (values[i] > 0)
		{
			temp.Format("%d instances of bell %d\r\n", values[i], i+1);
			msg += temp;
		}
	}
	
	return msg;	 
}


bool BagleyImportFilter::calculateStrikingData(const CArray <StrikeItem, StrikeItem>& strikeItems, StrikingData& strikingData)
{
	if (strikeItems.GetSize() < 1)
		return false;

	int number = 0;
	for (int i=0;i<strikeItems.GetSize();i++)
	{
		StrikeItem strikeItem = strikeItems.GetAt(i);

		if (strikeItem._bellNumber > number)
			number = strikeItem._bellNumber;
	}

	strikingData.reset(number);

	Stroke stroke = hand;
	Row row(number);

	int index = 0;
	int speed = 10;


	StrikeItem strikeItem = strikeItems.GetAt(0);
	DWORD baseTimestamp = strikeItem._timestamp - 10000;
	const int maxTimestamp = 10*60*60*1000;


	while(index < strikeItems.GetSize())
	{
		StrikeItem strikeItem = strikeItems.GetAt(index);

		DWORD strikeItemCorrected = strikeItem._timestamp - baseTimestamp;

		if (index % number == 0)
		{
			if (strikeItems.GetSize() >= index + number)
			{
				DWORD endStrikeItemCorrected = strikeItems.GetAt(index + number-1)._timestamp - baseTimestamp;
				int rowTime = endStrikeItemCorrected - strikeItemCorrected;
				speed = rowTime / (number-1);
			}

			strikingData.startNewRow(stroke, strikeItemCorrected - speed, speed, row, strikeItem._bellNumber);
		}

		if (strikeItemCorrected < maxTimestamp)
			strikingData.addRealStrike(strikeItem._bellNumber-1, strikeItemCorrected);

		if (!strikeItem._comment.IsEmpty())
			strikingData.addSpeaking(strikeItem._comment,   strikeItemCorrected);


		(stroke == hand)?stroke = back:stroke = hand;
		index++;
	}

	return true;

}

