// Scale.h: interface for the Scale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALE_H__3E89D123_9041_11D7_B6C3_000255162CF0__INCLUDED_)
#define AFX_SCALE_H__3E89D123_9041_11D7_B6C3_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ScaleType
{
	s_2_500,
	s_5_000,
	s_10_000,
	s_25_000,
	s_50_000,
	s_100_000,
	s_200_000,
	s_250_000,
	s_500_000,
	s_1000_000,
	s_2000_000,
	s_4000_000,
	s_none, 

	//NOTE for future releases, always add to the end.
};


typedef CArray <ScaleType, ScaleType> ScaleTypes;


class Scale  
{
public:
	Scale() {};
	virtual ~Scale() {};

};

#endif // !defined(AFX_SCALE_H__3E89D123_9041_11D7_B6C3_000255162CF0__INCLUDED_)
