// GlobalFunctions.h: interface for the GlobalFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALFUNCTIONS_H__E96C1035_5C28_11D6_B5A8_000255162CF0__INCLUDED_)
#define AFX_GLOBALFUNCTIONS_H__E96C1035_5C28_11D6_B5A8_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GlobalFunctions  
{
public:
	static CString getDodgePullNumber(int number);
	GlobalFunctions();
	virtual ~GlobalFunctions();		

	static CString numberName(int Number);
	static int charToBellNumbers(char ch);
	static char bellNumbersToChar(int number);
	static CString titleString(CString strString);
	static CString capitalString(CString strString, bool allowRemoval = true);
	static bool isEven(int num);
	static void InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo);
	static CString getLeadHeadCode(const CString& leadHead, int number, const CString& leadEnd, bool invert);
	static bool isIligalChar(const CString& check, CString& msg);
	static CString getPlaceName(int number, int tenor);
	static CString getNumberName(int number);
	static void negotiateSpace(CWnd* wnd, CWnd* parent);
	static void negotiateSpace(CWnd* wnd, CPoint requestPoint);

	static CString getWeightStringWithApprox(double weightKG, bool approx, bool full = false);
	static CString getWeightString(double weightKG, bool full = false);
	static void getWeightAsCWT(double KG, int& cwt, int& quarters, int& pounds);
	static void getWeightAsKG(int cwt, int quarters, int pounds, double& KG);

	static CString getDistanceString(double distanceKM, bool doConversion = true);

	static double KMtoMiles(double KM);
	static double milesToKM(double miles);

	static CString ColorToRTF(COLORREF color);
	static CString TextToRTF(int fontNumber, int size, COLORREF color, CString text);

	static void convertAngleToXY(double angle, double rad, double &X, double &Y);
	static void normaliseAngle(double& angle);

	static CString getPealTimeText(int numberOfBells, int intervalMillis, 
								double percentLead, bool includeNumber = false, int changeCount = PEAL_LENGTH);

};

#endif // !defined(AFX_GLOBALFUNCTIONS_H__E96C1035_5C28_11D6_B5A8_000255162CF0__INCLUDED_)
