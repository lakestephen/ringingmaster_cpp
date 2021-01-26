// GlobalFunctions.cpp: implementation of the GlobalFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "GlobalFunctions.h"
#include "GlobalDataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GlobalFunctions::GlobalFunctions()
{

}

GlobalFunctions::~GlobalFunctions()
{

}



/***************************************************************/
CString GlobalFunctions::numberName(int Number)		
{
	CString strName;
	switch (Number)
	{
	case 3:		strName = "Singles";			break;
	case 4:		strName = "Minimus";		  	break;
	case 5:		strName = "Doubles";		   	break;
	case 6:		strName = "Minor";				break;
	case 7:		strName = "Triples";			break;
	case 8:		strName = "Major";				break;
	case 9:		strName = "Caters";				break;
	case 10:	strName = "Royal";				break;
	case 11:	strName = "Cinques";			break;
	case 12:	strName = "Maximus";			break;
	case 13:	strName = "Sextuples";			break;
	case 14:	strName = "Fourteen";			break;
	case 15:	strName = "Septuples";			break;
	case 16:	strName = "Sixteen";			break;
	case 17:	strName = "Seventeen";			break;
	case 18:	strName = "Eighteen";			break;
	case 19:	strName = "Ninteen";			break;
	case 20:	strName = "Twenty";				break;
	case 21:	strName = "Twenty-one";			break;
	case 22:	strName = "Twenty-two";			break;
	case 23:	strName = "Twenty-three";		break;
	case 24:	strName = "Twenty-four";		break;
	case 25:	strName = "Twenty-five";		break;
	case 26:	strName = "Twenty-six";			break;
	case 27:	strName = "Twenty-seven";		break;
	case 28:	strName = "Twenty-eight";		break;
	case 29:	strName = "Twenty-nine";		break;
	case 30:	strName = "Thirty";				break;
	case 31:	strName = "Thirty-one";			break;
	case 32:	strName = "Thirty-two";			break;
	case 33:	strName = "Thirty-three";		break;
	case 34:	strName = "Thirty-four";		break;
	case 35:	strName = "Thirty-five";		break;
	case 36:	strName = "Thirty-six";			break;
	default:	strName = "";					break;
	}
	return strName;
}

/******************************************************************/


int GlobalFunctions::charToBellNumbers(char ch)              
{
	int iNumber;
	switch(ch)
	{
	case '1':		iNumber = 1;		break;
	case '2':		iNumber = 2;		break;
	case '3':		iNumber = 3;		break;
	case '4':     	iNumber = 4;		break;
	case '5':		iNumber = 5;		break;
	case '6':		iNumber = 6;		break;
	case '7':		iNumber = 7;		break;
	case '8':		iNumber = 8;		break;
	case '9':		iNumber = 9;		break;
	case '0':		iNumber = 10;		break;
	case 'e':
	case 'E':		iNumber = 11;		break;
	case 't':
	case 'T':		iNumber = 12;		break;
	case 'a':
	case 'A':		iNumber = 13;		break;
	case 'b':
	case 'B':		iNumber = 14;		break;
	case 'c':
	case 'C':		iNumber = 15;		break;
	case 'd':
	case 'D':		iNumber = 16;		break;
	case 'f':
	case 'F':		iNumber = 17;		break;
	case 'g':
	case 'G':		iNumber = 18;		break;
	case 'h':
	case 'H':		iNumber = 19;		break;
	case 'j':
	case 'J':		iNumber = 20;		break;
	case 'k':
	case 'K':		iNumber = 21;		break;
	case 'l':
	case 'L':		iNumber = 22;		break;
	case 'm':
	case 'M':		iNumber = 23; 		break;
	case 'n':
	case 'N':		iNumber = 24; 		break;
	case 'p':
	case 'P':		iNumber = 25; 		break;
	case 'q':
	case 'Q':		iNumber = 26; 		break;
	case 'r':
	case 'R':		iNumber = 27; 		break;
	case 's':
	case 'S':		iNumber = 28; 		break;
	case 'u':
	case 'U':		iNumber = 29; 		break;
	case 'v':
	case 'V':		iNumber = 30; 		break;
	case 'x':
	case 'X':
	case '-': 		iNumber = ALLCHANGE;	break;
	case ' ':
	case '.':		iNumber = SEPERATOR;	break;
	default: 		iNumber = BADDATA;
	}
	return iNumber;
}

/******************************************************************/
// converts ints to the char equiv of bell numbers
char GlobalFunctions::bellNumbersToChar(int number)
{
char str;
switch(number)
	{
	case 1:			str = '1';	break;
	case 2:			str = '2';	break;
	case 3:			str = '3';	break;
	case 4:			str = '4';	break;
	case 5:			str = '5';	break;
	case 6:			str = '6';	break;
	case 7:			str = '7';	break;
	case 8:			str = '8';	break;
	case 9:			str = '9';	break;
	case 10: 		str = '0';	break;
	case 11: 		str = 'E';	break;
	case 12: 		str = 'T';	break;
	case 13: 		str = 'A';	break;
	case 14: 		str = 'B';	break;
	case 15: 		str = 'C';	break;
	case 16: 		str = 'D';	break;
	case 17: 		str = 'F';	break;
	case 18: 		str = 'G';	break;
	case 19: 		str = 'H';	break;
	case 20: 		str = 'J';	break;
	case 21: 		str = 'K';	break;
	case 22: 		str = 'L';	break;
	case 23: 		str = 'M';	break;
	case 24: 		str = 'N';	break;
	case 25: 		str = 'P';	break;
	case 26: 		str = 'Q';	break;
	case 27: 		str = 'R';	break;
	case 28: 		str = 'S';	break;
	case 29: 		str = 'U';	break;
	case 30: 		str = 'V';	break;
	case ALLCHANGE: str = globalDataManager()->getAllChangeChar(); break;      
	case SEPERATOR:	str = '.';  break;      
	default: 		str = ' ';
	}
return str;
}

/********************************************************/

CString GlobalFunctions::capitalString(CString strString, bool allowRemoval)
{
	if (!strString.IsEmpty())
	{
		strString.MakeLower();
		int i;
		if (allowRemoval)
		{
			strString.TrimLeft();
			strString.TrimRight();

			while ((i = strString.FindOneOf("#%;./\\")) != -1)
				strString.Delete(i,1);
		}

		while(strString.Replace("  ", " "));
		
		for (i=0;i<strString.GetLength();i++)
		{
			if ((i==0)||
				((i>0)&&(strString.GetAt(i-1) == ' ')))
			{
				CString upper(strString.GetAt(i));
				upper.MakeUpper();
				strString.SetAt(i,upper.GetAt(0));
			}
		}
	}
	return strString;
}

/********************************************************/

CString GlobalFunctions::titleString(CString strString)
{
	if (!strString.IsEmpty())
		{
		strString.MakeLower();
		strString.TrimLeft();
		strString.TrimRight();
		int i;
		while ((i = strString.FindOneOf("#%;./\\")) != -1)
			strString.Delete(i,1);
		while(strString.Replace("  ", " "));

		CString Left = strString.Left(1);
		Left.MakeUpper();
		if (strString.GetLength() > 0)
			strString.SetAt(0,Left.GetAt(0));
		}
	return strString;
}

/********************************************************/
bool GlobalFunctions::isEven(int num)
{
	/*int test = num/2;
	test = test*2;
	if (test - num == 0)
		return TRUE;
	else
		return FALSE;
	  */
	return (num%2 == 0);
}

/********************************************************/

void GlobalFunctions::InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo)
{
	int nOldMode = pDC->SetROP2(R2_NOT);
	
	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo);

	pDC->SetROP2(nOldMode);
}

CString GlobalFunctions::getLeadHeadCode(const CString &leadHead, int number, const CString& leadEnd, bool invert)
{
	if (leadEnd.IsEmpty())
		return leadHead;
	
	bool bNear;

	CString numberStr ;
	numberStr = bellNumbersToChar(number);

	if (GlobalFunctions::isEven(number))
	{
		if (leadEnd.Find(numberStr) != -1)
			bNear = false;
		else 
			bNear = true;
	}
	else
	{
		if (leadEnd.Find(numberStr) != -1)
			bNear = true;
		else
			bNear = false;
	}

	if (invert) bNear = !bNear;
	
	switch (number)
	{
	case 5:
		{
			if (leadHead == "12534") return bNear ? "a":"g";
			if (leadHead == "12453") return bNear ? "f":"m";
			if (leadHead == "13524") return bNear ? "p":"r";
			if (leadHead == "14253") return bNear ? "q":"s";
			break;
		}
	case 6:
		{
			if (leadHead == "135264") return bNear ? "a":"g";
			if (leadHead == "156342") return bNear ? "b":"h";
			if (leadHead == "164523") return bNear ? "e":"l";
			if (leadHead == "142635") return bNear ? "f":"m";
			if (leadHead == "125364") return bNear ? "p":"r";
			if (leadHead == "124635") return bNear ? "q":"s";
			break;
		}
	case 7:
		{
			if (leadHead == "1253746") return bNear ? "a":"g";
			if (leadHead == "1275634") return bNear ? "b":"h";
			if (leadHead == "1267453") return bNear ? "e":"l";
			if (leadHead == "1246375") return bNear ? "f":"m";
			if (leadHead == "1352746") return bNear ? "p":"r";
			if (leadHead == "1426375") return bNear ? "q":"s";
			break;
		}
	case 8:
		{
			if (leadHead == "13527486") return bNear ? "a":"g";
			if (leadHead == "15738264") return bNear ? "b":"h";
			if (leadHead == "17856342") return bNear ? "c":"j";
			if (leadHead == "18674523") return bNear ? "d":"k";
			if (leadHead == "16482735") return bNear ? "e":"l";
			if (leadHead == "14263857") return bNear ? "f":"m";
			if (leadHead == "12537486") return bNear ? "p":"r";
			if (leadHead == "12463857") return bNear ? "q":"s";
			break;
		}
	case 9:
		{
			if (leadHead == "125374968") return bNear ? "a":"g";
			if (leadHead == "127593846") return bNear ? "b":"h";
			if (leadHead == "129785634") return bNear ? "c":"j";
			if (leadHead == "128967453") return bNear ? "d":"k";
			if (leadHead == "126849375") return bNear ? "e":"l";
			if (leadHead == "124638597") return bNear ? "f":"m";
			if (leadHead == "135274968") return bNear ? "p":"r";
			if (leadHead == "179583624") return bNear ? "p1":"r1";
			if (leadHead == "186947253") return bNear ? "q1":"s1";
			if (leadHead == "142638597") return bNear ? "q":"s";
			break;
		}
	case 10:
		{
			if (leadHead == "1352749608") return bNear ? "a":"g";
			if (leadHead == "1573920486") return bNear ? "b":"h";
			if (leadHead == "1907856342") return bNear ? "c1":"j1";
			if (leadHead == "1089674523") return bNear ? "d1":"k1";
			if (leadHead == "1648203957") return bNear ? "e":"m";
			if (leadHead == "1426385079") return bNear ? "f":"n";
			if (leadHead == "1253749608") return bNear ? "p":"r";
			if (leadHead == "1297058364") return bNear ? "p1":"r1";
			if (leadHead == "1280694735") return bNear ? "q1":"s1";
			if (leadHead == "1246385079") return bNear ? "q":"s";
			break;
		}
	case 11:
		{
			if (leadHead == "12537496E80") return bNear ? "a":"g";
			if (leadHead == "127593E4068") return bNear ? "b":"h";
			if (leadHead == "12E90785634") return bNear ? "c1":"j1";
			if (leadHead == "120E8967453") return bNear ? "d1":"k1";
			if (leadHead == "1268403E597") return bNear ? "e":"l";
			if (leadHead == "124638507E9") return bNear ? "f":"m";
			if (leadHead == "13527496E80") return bNear ? "p":"r";
			if (leadHead == "1795E302846") return bNear ? "p1":"r1";
			if (leadHead == "18604E29375") return bNear ? "q1":"s1";
			if (leadHead == "142638507E9") return bNear ? "q":"s";
			break;
		}
	case 12:
		{
			if (leadHead == "13527496E8T0") return bNear ? "a":"g";
			if (leadHead == "157392E4T608") return bNear ? "b":"h";
			if (leadHead == "1795E3T20486") return bNear ? "c":"j";
			if (leadHead == "19E7T5038264") return bNear ? "c1":"j1";
			if (leadHead == "1ET907856342") return bNear ? "c2":"j2";
			if (leadHead == "1T0E89674523") return bNear ? "d2":"k2";
			if (leadHead == "108T6E492735") return bNear ? "d1":"k1";
			if (leadHead == "108T6E492735") return bNear ? "d":"k";
			if (leadHead == "1648203T5E79") return bNear ? "e":"l";
			if (leadHead == "142638507T9E") return bNear ? "f":"m";
			if (leadHead == "12537496E8T0") return bNear ? "p":"r";
			if (leadHead == "1297E5T30486") return bNear ? "p1":"r1";
			if (leadHead == "12806T4E3957") return bNear ? "q1":"s1";
			if (leadHead == "124638507T9E") return bNear ? "q":"s";
			break;
		}
	case 13:
		{
			if (leadHead == "12537496E8A0T") return bNear ? "a":"g";
			if (leadHead == "127593E4A6T80") return bNear ? "b":"h";
			if (leadHead == "1297E5A3T4068") return bNear ? "c":"j";
			if (leadHead == "12E9A7T503846") return bNear ? "c1":"j1";
			if (leadHead == "12AET90785634") return bNear ? "c2":"j2";
			if (leadHead == "12TA0E8967453") return bNear ? "d2":"k2";
			if (leadHead == "120T8A6E49375") return bNear ? "d1":"k1";
			if (leadHead == "12806T4A3E597") return bNear ? "d":"k";
			if (leadHead == "1268403T5A7E9") return bNear ? "e":"l";
			if (leadHead == "124638507T9AE") return bNear ? "f":"m";
			if (leadHead == "13527496E8A0T") return bNear ? "p":"r";
			if (leadHead == "1EA9T70583624") return bNear ? "p2":"r2";
			if (leadHead == "1T0A8E6947253") return bNear ? "q2":"s2";
			if (leadHead == "142638507T9AE") return bNear ? "q":"s";
			break;
		}
	case 14:
		{
			if (leadHead == "13527496E8A0BT") return bNear ? "a":"g";
			if (leadHead == "157392E4A6B8T0") return bNear ? "b":"h";
			if (leadHead == "1795E3A2B4T608") return bNear ? "c":"j";
			if (leadHead == "19E7A5B3T20486") return bNear ? "c1":"j1";
			if (leadHead == "1EA9B7T5038264") return bNear ? "c2":"j2";
			if (leadHead == "1ABET907856342") return bNear ? "c3":"j3";
			if (leadHead == "1BTA0E89674523") return bNear ? "d3":"k3";
			if (leadHead == "1T0B8A6E492735") return bNear ? "d2":"k2";
			if (leadHead == "108T6B4A2E3957") return bNear ? "d1":"k1";
			if (leadHead == "18604T2B3A5E79") return bNear ? "d":"k";
			if (leadHead == "1648203T5B7A9E") return bNear ? "e":"l";
			if (leadHead == "142638507T9BEA") return bNear ? "f":"m";
			if (leadHead == "12537496E8A0BT") return bNear ? "p":"r";
			if (leadHead == "12AEB9T7058364") return bNear ? "p2":"r2";
			if (leadHead == "12TB0A8E694735") return bNear ? "q2":"s2";
			if (leadHead == "124638507T9BEA") return bNear ? "q":"s";
			break;
		}
	case 15:
		{
			if (leadHead == "12537496E8A0CTB") return bNear ? "a":"g";
			if (leadHead == "127593E4A6C8B0T") return bNear ? "b":"h";
			if (leadHead == "1297E5A3C4B6T80") return bNear ? "c":"j";
			if (leadHead == "12E9A7C5B3T4068") return bNear ? "c1":"j1";
			if (leadHead == "12AEC9B7T503846") return bNear ? "c2":"j2";
			if (leadHead == "12CABET90785634") return bNear ? "c3":"j3";
			if (leadHead == "12BCTA0E8967453") return bNear ? "d3":"k3";
			if (leadHead == "12TB0C8A6E49375") return bNear ? "d2":"k2";
			if (leadHead == "120T8B6C4A3E597") return bNear ? "d1":"k1";
			if (leadHead == "12806T4B3C5A7E9") return bNear ? "d":"k";
			if (leadHead == "1268403T5B7C9AE") return bNear ? "e":"l";
			if (leadHead == "124638507T9BECA") return bNear ? "f":"m";
			if (leadHead == "13527496E8A0CTB") return bNear ? "p":"r";
			if (leadHead == "1795E3A2C4B6T80") return bNear ? "p1":"r1";
			if (leadHead == "1EA9C7B5T302846") return bNear ? "p2":"r2";
			if (leadHead == "1T0B8C6A4E29375") return bNear ? "q2":"s2";
			if (leadHead == "18604T2B3C5A7E9") return bNear ? "q1":"s1";
			if (leadHead == "142638507T9BECA") return bNear ? "q":"s";
			break;
		}
	case 16:
		{
			if (leadHead == "13527496E8A0CTDB") return bNear ? "a":"g";
			if (leadHead == "157392E4A6C8D0BT") return bNear ? "b":"h";
			if (leadHead == "19E7A5C3D2B4T608") return bNear ? "c1":"j1";
			if (leadHead == "1CDABET907856342") return bNear ? "c4":"j4";

			if (leadHead == "1DBCTA0E89674523") return bNear ? "d4":"k4";
			if (leadHead == "108T6B4D2C3A5E79") return bNear ? "d1":"k1";
			if (leadHead == "1648203T5B7D9CEA") return bNear ? "e":"l";
			if (leadHead == "142638507T9BEDAC") return bNear ? "f":"m";

			if (leadHead == "12537496E8A0CTDB") return bNear ? "p":"r";
			if (leadHead == "1297E5A3C4D6B8T0") return bNear ? "p1":"r1";
			if (leadHead == "12AEC9D7B5T30486") return bNear ? "p2":"r2";
			if (leadHead == "12TB0D8C6A4E3957") return bNear ? "q2":"s2";
			if (leadHead == "12806T4B3D5C7A9E") return bNear ? "q1":"s1";
			if (leadHead == "124638507T9BEDAC") return bNear ? "q":"s";
			break;
		}
	case 17:
		{
			if (leadHead == "12537496E8A0CTFBD") return bNear ? "a":"g";
			if (leadHead == "127593E4A6C8F0DTB") return bNear ? "b":"h";
			if (leadHead == "12E9A7C5F3D4B6T80") return bNear ? "c1":"j1";
			if (leadHead == "12FCDABET90785634") return bNear ? "c4":"j4";

			if (leadHead == "12DFBCTA0E8967453") return bNear ? "d4":"k4";
			if (leadHead == "120T8B6D4F3C5A7E9") return bNear ? "d1":"k1";
			if (leadHead == "1268403T5B7D9FECA") return bNear ? "e":"l";
			if (leadHead == "124638507T9BEDAFC") return bNear ? "f":"m";

			if (leadHead == "13527496E8A0CTFBD") return bNear ? "p":"r";
			if (leadHead == "1795E3A2C4F6D8B0T") return bNear ? "p1":"r1";
			if (leadHead == "1EA9C7F5D3B2T4068") return bNear ? "p2":"r2";
			if (leadHead == "1CFADEB9T70583624") return bNear ? "p3":"r3";
			if (leadHead == "1DBFTC0A8E6947253") return bNear ? "q3":"s3";
			if (leadHead == "1T0B8D6F4C2A3E597") return bNear ? "q2":"s2";
			if (leadHead == "18604T2B3D5F7C9AE") return bNear ? "q1":"s1";
			if (leadHead == "142638507T9BEDAFC") return bNear ? "q":"s";
			break;
		}
	case 18:
		{
			if (leadHead == "13527496E8A0CTFBGD") return bNear ? "a":"g";
			if (leadHead == "157392E4A6C8F0GTDB") return bNear ? "b":"h";
			if (leadHead == "1795E3A2C4F6G8D0BT") return bNear ? "c":"j";
			if (leadHead == "19E7A5C3F2G4D6B8T0") return bNear ? "c1":"j1";
			if (leadHead == "1EA9C7F5G3D2B4T608") return bNear ? "c2":"j2";
			if (leadHead == "1ACEF9G7D5B3T20486") return bNear ? "c3":"j3";
			if (leadHead == "1CFAGED9B7T5038264") return bNear ? "c4":"j4";
			if (leadHead == "1FGCDABET907856342") return bNear ? "c5":"j5";

			if (leadHead == "1GDFBCTA0E89674523") return bNear ? "d5":"k5";
			if (leadHead == "1DBGTF0C8A6E492735") return bNear ? "d4":"k4";
			if (leadHead == "1BTD0G8F6C4A2E3957") return bNear ? "d3":"k3";
			if (leadHead == "1T0B8D6G4F2C3A5E79") return bNear ? "d2":"k2";
			if (leadHead == "108T6B4D2G3F5C7A9E") return bNear ? "d1":"k1";
			if (leadHead == "18604T2B3D5G7F9CEA") return bNear ? "d":"k";
			if (leadHead == "1648203T5B7D9GEFAC") return bNear ? "e":"l";
			if (leadHead == "142638507T9BEDAGCF") return bNear ? "f":"m";

			if (leadHead == "12537496E8A0CTFBGD") return bNear ? "p":"r";
			if (leadHead == "1297E5A3C4F6G8D0BT") return bNear ? "p1":"r1";
			if (leadHead == "12AEC9F7G5D3B4T608") return bNear ? "p2":"r2";
			if (leadHead == "12FCGADEB9T7058364") return bNear ? "p3":"r3";

			if (leadHead == "12DGBFTC0A8E694735") return bNear ? "q3":"s3";
			if (leadHead == "12TB0D8G6F4C3A5E79") return bNear ? "q2":"s2";
			if (leadHead == "12806T4B3D5G7F9CEA") return bNear ? "q1":"s1";
			if (leadHead == "124638507T9BEDAGCF") return bNear ? "q":"s";
			break;
		}
	case 19:
		{
			if (leadHead == "12537496E8A0CTFBHDG") return bNear ? "a":"g";
			if (leadHead == "127593E4A6C8F0HTGBD") return bNear ? "b":"h";
			if (leadHead == "1297E5A3C4F6H8G0DTB") return bNear ? "c":"j";
			if (leadHead == "12E9A7C5F3H4G6D8B0T") return bNear ? "c1":"j1";
			if (leadHead == "12AEC9F7H5G3D4B6T80") return bNear ? "c2":"j2";
			if (leadHead == "12CAFEH9G7D5B3T4068") return bNear ? "c3":"j3";
			if (leadHead == "12FCHAGED9B7T503846") return bNear ? "c4":"j4";
			if (leadHead == "12HFGCDABET90785634") return bNear ? "c5":"j5";

			if (leadHead == "12GHDFBCTA0E8967453") return bNear ? "d5":"k5";
			if (leadHead == "12DGBHTF0C8A6E49375") return bNear ? "d4":"k4";
			if (leadHead == "12BDTG0H8F6C4A3E597") return bNear ? "d3":"k3";
			if (leadHead == "12TB0D8G6H4F3C5A7E9") return bNear ? "d2":"k2";
			if (leadHead == "120T8B6D4G3H5F7C9AE") return bNear ? "d1":"k1";
			if (leadHead == "12806T4B3D5G7H9FECA") return bNear ? "d":"k";
			if (leadHead == "1268403T5B7D9GEHAFC") return bNear ? "e":"l";
			if (leadHead == "124638507T9BEDAGCHF") return bNear ? "f":"m";

			if (leadHead == "13527496E8A0CTFBHDG") return bNear ? "p":"r";
			if (leadHead == "1EA9C7F5H3G2D4B6T80") return bNear ? "p2":"r2";
			if (leadHead == "1CFAHEG9D7B5T302846") return bNear ? "p3":"r3";

			if (leadHead == "1DBGTH0F8C6A4E29375") return bNear ? "q3":"s3";
			if (leadHead == "1T0B8D6G4H2F3C5A7E9") return bNear ? "q2":"s2";
			if (leadHead == "142638507T9BEDAGCHF") return bNear ? "q":"s";
			break;
		}
	case 20:
		{
			if (leadHead == "13527496E8A0CTFBHDJG") return bNear ? "a":"g";
			if (leadHead == "157392E4A6C8F0HTJBGD") return bNear ? "b":"h";
			if (leadHead == "1795E3A2C4F6H8J0GTDB") return bNear ? "c":"j";
			if (leadHead == "19E7A5C3F2H4J6G8D0BT") return bNear ? "c1":"j1";
			if (leadHead == "1EA9C7F5H3J2G4D6B8T0") return bNear ? "c2":"j2";
			if (leadHead == "1ACEF9H7J5G3D2B4T608") return bNear ? "c3":"j3";
			if (leadHead == "1CFAHEJ9G7D5B3T20486") return bNear ? "c4":"j4";
			if (leadHead == "1FHCJAGED9B7T5038264") return bNear ? "c5":"j5";
			if (leadHead == "1HJFGCDABET907856342") return bNear ? "c6":"j6";

			if (leadHead == "1JGHDFBCTA0E89674523") return bNear ? "d6":"k6";
			if (leadHead == "1GDJBHTF0C8A6E492735") return bNear ? "d5":"k5";
			if (leadHead == "1DBGTJ0H8F6C4A2E3957") return bNear ? "d4":"k4";
			if (leadHead == "1BTD0G8J6H4F2C3A5E79") return bNear ? "d3":"k3";
			if (leadHead == "1T0B8D6G4J2H3F5C7A9E") return bNear ? "d2":"k2";
			if (leadHead == "108T6B4D2G3J5H7F9CEA") return bNear ? "d1":"k1";
			if (leadHead == "18604T2B3D5G7J9HEFAC") return bNear ? "d":"k";
			if (leadHead == "1648203T5B7D9GEJAHCF") return bNear ? "e":"l";
			if (leadHead == "142638507T9BEDAGCJFH") return bNear ? "f":"m";

			if (leadHead == "12537496E8A0CTFBHDJG") return bNear ? "p":"r";
			if (leadHead == "12AEC9F7H5J3G4D6B8T0") return bNear ? "p2":"r2";
			if (leadHead == "12FCHAJEG9D7B5T30486") return bNear ? "p3":"r3";

			if (leadHead == "12DGBJTH0F8C6A4E3957") return bNear ? "q3":"s3";
			if (leadHead == "12TB0D8G6J4H3F5C7A9E") return bNear ? "q2":"s2";
			if (leadHead == "124638507T9BEDAGCJFH") return bNear ? "q":"s";
			break;
		}
	case 21:
		{
			if (leadHead == "12537496E8A0CTFBHDKGJ") return bNear ? "a":"g";
			if (leadHead == "127593E4A6C8F0HTKBJDG") return bNear ? "b":"h";
			if (leadHead == "1297E5A3C4F6H8K0JTGBD") return bNear ? "c":"j";
			if (leadHead == "12E9A7C5F3H4K6J8G0DTB") return bNear ? "c1":"j1";
			if (leadHead == "12AEC9F7H5K3J4G6D8B0T") return bNear ? "c2":"j2";
			if (leadHead == "12CAFEH9K7J5G3D4B6T80") return bNear ? "c3":"j3";
			if (leadHead == "12FCHAKEJ9G7D5B3T4068") return bNear ? "c4":"j4";
			if (leadHead == "12HFKCJAGED9B7T503846") return bNear ? "c5":"j5";
			if (leadHead == "12KHJFGCDABET90785634") return bNear ? "c6":"j6";

			if (leadHead == "12JKGHDFBCTA0E8967453") return bNear ? "d6":"k6";
			if (leadHead == "12GJDKBHTF0C8A6E49375") return bNear ? "d5":"k5";
			if (leadHead == "12DGBJTK0H8F6C4A3E597") return bNear ? "d4":"k4";
			if (leadHead == "12TB0D8G6J4K3H5F7C9AE") return bNear ? "d3":"k3";
			if (leadHead == "12TB0D8G6J4K3H5F7C9AE") return bNear ? "d2":"k2";
			if (leadHead == "120T8B6D4G3J5K7H9FECA") return bNear ? "d1":"k1";
			if (leadHead == "12806T4B3D5G7J9KEHAFC") return bNear ? "d":"k";
			if (leadHead == "1268403T5B7D9GEJAKCHF") return bNear ? "e":"l";
			if (leadHead == "124638507T9BEDAGCJFKH") return bNear ? "f":"m";

			if (leadHead == "13527496E8A0CTFBHDKGJ") return bNear ? "p":"r";
			if (leadHead == "1795E3A2C4F6H8K0JTGBD") return bNear ? "p1":"r1";
			if (leadHead == "1CFAHEK9J7G5D3B2T4068") return bNear ? "p3":"r3";
			if (leadHead == "1HKFJCGADEB9T70583624") return bNear ? "p4":"r4";

			if (leadHead == "1JGKDHBFTC0A8E6947253") return bNear ? "q4":"s4";
			if (leadHead == "1DBGTJ0K8H6F4C2A3E597") return bNear ? "q3":"s3";
			if (leadHead == "18604T2B3D5G7J9KEHAFC") return bNear ? "q1":"s1";
			if (leadHead == "142638507T9BEDAGCJFKH") return bNear ? "q":"s";
			break;
		}
	case 22:
		{
			if (leadHead == "13527496E8A0CTFBHDKGLJ") return bNear ? "a":"g";
			if (leadHead == "157392E4A6C8F0HTKBLDJG") return bNear ? "b":"h";
			if (leadHead == "19E7A5C3F2H4K6L8J0GTDB") return bNear ? "c1":"j1";
			if (leadHead == "1EA9C7F5H3K2L4J6G8D0BT") return bNear ? "c2":"j2";
			if (leadHead == "1FHCKALEJ9G7D5B3T20486") return bNear ? "c5":"j5";
			if (leadHead == "1KLHJFGCDABET907856342") return bNear ? "c7":"j7";

			if (leadHead == "1LJKGHDFBCTA0E89674523") return bNear ? "d7":"k7";
			if (leadHead == "1GDJBLTK0H8F6C4A2E3957") return bNear ? "d5":"k5";
			if (leadHead == "1T0B8D6G4J2L3K5H7F9CEA") return bNear ? "d2":"k2";
			if (leadHead == "108T6B4D2G3J5L7K9HEFAC") return bNear ? "d1":"k1";
			if (leadHead == "1648203T5B7D9GEJALCKFH") return bNear ? "e":"l";
			if (leadHead == "142638507T9BEDAGCJFLHK") return bNear ? "f":"m";

			if (leadHead == "12537496E8A0CTFBHDKGLJ") return bNear ? "p":"r";
			if (leadHead == "1297E5A3C4F6H8K0LTJBGD") return bNear ? "p1":"r1";
			if (leadHead == "12FCHAKEL9J7G5D3B4T608") return bNear ? "p3":"r3";
			if (leadHead == "12KHLFJCGADEB9T7058364") return bNear ? "p4":"r4";

			if (leadHead == "12JLGKDHBFTC0A8E694735") return bNear ? "q4":"s4";
			if (leadHead == "12DGBJTL0K8H6F4C3A5E79") return bNear ? "q3":"s3";
			if (leadHead == "12806T4B3D5G7J9LEKAHCF") return bNear ? "q1":"s1";
			if (leadHead == "124638507T9BEDAGCJFLHK") return bNear ? "q":"s";
			break;
		}
	}

	return leadHead;
}

bool GlobalFunctions::isIligalChar(const CString& check, CString& msg)
{
	int index = check.FindOneOf("\"[]{}().?~");
	if (index != -1)
	{
		msg.Format("contains illigal character: '%c'", check[index]);
		return true;
	}
	return false;
}

//number is 0 based, tenor is 1 based.
CString GlobalFunctions::getPlaceName(int number, int tenor)
{
	if (number+1 == tenor) return "the back";
	
	switch (number) 
	{
	case 0:	
		return "the front";
	case 1:	
		return "seconds";
	case 2:	
		return "thirds";
	case 3:	
		return "fourths";
	case 4:	
		return "fifths";
	case 5:	
		return "sixths";
	case 6:	
		return "sevenths";
	case 7:	
		return "eighths";
	case 8:	
		return "ninths";
	case 9:	
		return "tenths";
	}

	CString str;
	str.Format("%d place", number +1);
	return str;
}

CString GlobalFunctions::getNumberName(int number)
{
	switch (number) 
	{
	case 0:	
		return "no";
	case 1:	
		return "one";
	case 2:	
		return "two";
	case 3:	
		return "three";
	case 4:	
		return "four";
	case 5:	
		return "five";
	case 6:	
		return "six";
	case 7:	
		return "seven";
	case 8:	
		return "eight";
	case 9:	
		return "nine";
	case 10:	
		return "ten";
	case 11:	
		return "eleven";
	case 12:	
		return "twelve";
	}

	CString str;
	str.Format("%d", number);
	return str;
}

CString GlobalFunctions::getDodgePullNumber(int number)
{
	switch (number) 
	{
	case 1:	
		return "";
	case 2:	
		return "double ";
	case 3:	
		return "triple ";
	}

	CString str;
	str.Format("%d pull ", number);
	return str;

}

void GlobalFunctions::negotiateSpace(CWnd* wnd, CWnd* parent)
{
	ASSERT(!parent);
	if (!parent)
		return;

	CRect parentRect;
	parent->GetWindowRect(&parentRect);

	negotiateSpace(wnd, parentRect.TopLeft());

}

void GlobalFunctions::negotiateSpace(CWnd* wnd, CPoint requestPoint)
{
	ASSERT(wnd);
	if (!wnd)
		return;

	CRect thisRect;
	wnd->GetWindowRect(&thisRect);
	CRect newRect(requestPoint.x + 10, 
				  requestPoint.y + 15, 
				  requestPoint.x + 10 + thisRect.Width(),
				  requestPoint.y + 15 + thisRect.Height());

	CRect screen;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &screen, SPIF_UPDATEINIFILE);
		
	CPoint scrOffset;
	scrOffset.x = screen.right - newRect.right - 8;
	scrOffset.y = screen.bottom - newRect.bottom - 8;

	if (scrOffset.x > 0) scrOffset.x = 0;
	if (scrOffset.y > 0) scrOffset.y = 0;

	newRect.OffsetRect(scrOffset);

	wnd->MoveWindow(&newRect);

}

CString GlobalFunctions::getWeightStringWithApprox(double weightKG, bool approx, bool full)
{
	return getWeightString(weightKG, full) +  
			(approx?" (Approx)":"");
}


CString GlobalFunctions::getWeightString(double weightKG, bool full)
{
	int weightSwitch = app()->GetProfileInt("Tower Database", 
									"Weight", 0);
	
	CString str;
	
	if (weightSwitch == 0)
	{
		int pounds, cwt, quarters;
		getWeightAsCWT(weightKG, cwt, quarters, pounds);

		if (full)
			str.Format("%dcwt %dqr %dlb",cwt, quarters, pounds);
		else
			str.Format("%d-%d-%d",cwt, quarters, pounds);
	}
	else
	{
		str.Format("%.1f kg",weightKG);
	}

	return str;
}


void GlobalFunctions::getWeightAsKG(int cwt, int quarters, int pounds, double& KG)
{
	pounds += (cwt * 112);
	pounds += (quarters * 28);

	KG = ((double)pounds / 2.205);
}

void GlobalFunctions::getWeightAsCWT(double KG, int& cwt, int& quarters, int& pounds)
{
	pounds = (int)(KG * 2.205);

	cwt = pounds / 112;
	pounds -= (cwt*112);

	quarters = pounds / 28;

	pounds -= (quarters*28);
}

CString GlobalFunctions::getDistanceString(double distanceKM, bool doConversion)
{
	int distanceType = app()->GetProfileInt("Tower Database", "Distance", 0);
	double distance;
	distance = (((distanceType == 0)&&(doConversion))?
		GlobalFunctions::KMtoMiles(distanceKM) :distanceKM);

	if (distance == 0.0)
		return "";

	CString str;
	str.Format("%.2f %s",distance, ((distanceType == 0)?"miles":"km"));
	return str;

}

const double milesPerKM = 0.621371;

double GlobalFunctions::KMtoMiles(double KM)
{
	return KM * milesPerKM;
}

double GlobalFunctions::milesToKM(double miles)
{
	return miles / milesPerKM;
}

CString GlobalFunctions::ColorToRTF(COLORREF color)
{
	CString str;
	str.Format("\\red%d\\green%d\\blue%d;", GetRValue(color), GetGValue(color), GetBValue(color));
	return str;
}

CString GlobalFunctions::TextToRTF(int fontNumber, int size, COLORREF color, CString text)
{
	CString str	;
	str.Format("\\cf%d\\f%d\\fs%d%s\\par", color, fontNumber, size, text);
	return str;
}
			  

void GlobalFunctions::convertAngleToXY(double angle, double rad, double &X, double &Y)
{
	GlobalFunctions::normaliseAngle(angle);

	X = sin(angle) * rad;
	Y = cos(angle) * rad;
}

void GlobalFunctions::normaliseAngle(double& angle)
{
	while (angle < 0) 
		angle += PI2;

	while (angle >= PI2) 
		angle -= PI2;

	ASSERT(angle >= 0);
}

CString GlobalFunctions::getPealTimeText(int numberOfBells, int intervalMillis, 
										 double percentLead, bool includeNumber, 
										 int changeCount)
{
		//update the peal time
	CString msg;
	int timePerHandStroke = (intervalMillis * numberOfBells) + 
							(int)((double)intervalMillis * percentLead);
	int timePerBackStroke = intervalMillis * numberOfBells;
	int handstrokeChangeCount = changeCount / 2 + (GlobalFunctions::isEven(changeCount)?0:1);
	int backstrokeChangeCount = changeCount / 2;

	int overallMS = (timePerBackStroke * backstrokeChangeCount) + 
					(timePerHandStroke * handstrokeChangeCount);

	int hour = overallMS / (60*60*1000);
	int min = (overallMS / (60*1000)) - (60*hour);
	int sec = (overallMS / 1000) - (60*60*hour) - (60*min);
	
	if (includeNumber)
		msg.Format("%d: %d:%.2d:%.2d", numberOfBells, hour, min, sec);
	else
		msg.Format("%d:%.2d:%.2d", hour, min, sec);

	return msg;
}
