// Key.cpp: implementation of the Key class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Key Create.h"
#include "Key.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char Key::lookup[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O','P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c','d', 'e','f','g','h','i','j','k','l','m','n','o','p','q',
		'r','s','t','u','v','w','x','y','z'};



Key::Key()
{


}

Key::~Key()
{

}
	

CString Key::calculateKey(CString user, int version)
{
	CString key;

	//random component
	srand((unsigned)timeGetTime());
	int random = rand();  

	key += intToBase62String(random * 100);

	//version
	key += intToBase62String(version + random );
	
	//user
	key += intToBase62String(getUserInt(user) + random );

	//checksum
  	key += getCheckSum(key.Left(12));

	return key;
}

KeyRetType Key::verifyKeyRange(CString user, Ints * version, CString key)
{
	KeyRetType ret = KR_BAD_USER;

	for (int i=0;i<version->GetSize();i++)
	{
		ret = verifyKey(user,version->GetAt(i), key);
		if (ret == KR_OK) 
			return KR_OK;
	}
	return ret;
}

KeyRetType Key::verifyKey(CString user, int version, CString key)
{
	//check length
	if (key.GetLength() != 15)	
	{
		return KR_BAD_KEY_LENGTH;
	}

	//check checksum
	if (getCheckSum(key.Left(12)) != key.Right(3))
	{
		return KR_BAD_KEY_FORMAT;
	}

	//get rand
	int randomKey = base62StringToInt(key.Left(4))/100;

	//get version
	int versionKey = base62StringToInt(key.Mid(4,4)) - randomKey;
	if (versionKey != version) 
	{
		return KR_BAD_VERSION;
	}

	//get user
	int userKey = base62StringToInt(key.Mid(8,4)) - randomKey;
	if (userKey != getUserInt(user))
	{
		return KR_BAD_USER;
	}


	//withdrawn keys
	//todo populate
	CStringArray withdrawnKeys;  
	withdrawnKeys.Add("KdLWAHXPAHd53A5"); //user sjl

	for (int i=0;i<withdrawnKeys.GetSize();i++)
	{
		if (withdrawnKeys.GetAt(i) == key)
			return KR_BAD_KEY_WITHDRAWN;
	}


	//phew, got there
	return KR_OK;
}

/*
	KR_OK,
	KR_BAD_USER,
	KR_BAD_VERSION,
	KR_BAD_KEY_LENGTH,
	KR_BAD_KEY_FORMAT  
	KR_BAD_KEY_WITHDRAWN
*/


CString Key::getErrorString(KeyRetType error)
{
	switch (error)
	{
	case KR_BAD_KEY_LENGTH:
		return "Verification Failure: Incorect key length";
		break;
	case KR_BAD_KEY_FORMAT:
		return "Verification Failure: Incorect key format";
		break;
	case KR_BAD_VERSION:
		return "Verification Failure: Incorect version";
		break;
	case KR_BAD_USER:
		return "Verification Failure: Incorect user";
		break;
	case KR_BAD_KEY_WITHDRAWN:
		return "Verification Failure: Key Withdrawn - contact info@ringingmaster.co.uk";
		break;

		
	default:
		return "";
		break;
	}
}

CString Key::intToBase62String(int conversionInt)
{
	CString output;
	int currentValue =conversionInt;
	
	while(currentValue!=0)
	{
		output+=lookup[currentValue%62];
		currentValue/=62;
	}


	while(output.GetLength()<4)
	{
		//add an A to the end of the string if its length is less
		//than four
		output+=lookup[0];
	}

	//reverse the string
	output.MakeReverse();

	return output;
}

int Key::base62StringToInt(LPCTSTR conversionString) 
{
 	int intValue = 0;
	
		
	for(size_t i=0; i< _tcslen(conversionString); i++)
	{
		for(int j=0; j<sizeof(lookup)/sizeof(lookup[0]);j++)
		{
			if(lookup[j]==conversionString[i])
			{
				intValue=intValue*62+j;
				break;
			}
		}
	}

	return intValue;
}


int Key::getUserInt(CString user)
{ 
	int userStringInt = 0;
	while (user.GetLength() < 10) user += 'X';
	for (int i=0;i<user.GetLength();i++)
	{
		userStringInt += (user[i] * i);
	}

	return userStringInt;			  
}



CString Key::getCheckSum(CString key)
{
	int checksum = 0;
	for (int i=0;i<key.GetLength(); i++)
	{
		checksum += key[i];
	}  
	CString checksumString;
	checksumString.Format("%03X", checksum);

	return checksumString;
}

