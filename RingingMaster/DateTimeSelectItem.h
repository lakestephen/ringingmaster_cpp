#pragma once

enum DateTimeSelectFormatType
{
	dtsft_date,
	dtsft_time,							  
	dtsft_date_time,
};          

enum DateTimeSelectType
{
	dtst_greater,
	dtst_less,
	dtst_between,
	dtst_exact,
};

class DateTimeSelectItem
{
	public:
	DateTimeSelectItem();

	void Serialize(CArchive& ar);

	DateTimeSelectType _type;
	DateTimeSelectFormatType _format;
	COleDateTime _dateTime1;
	COleDateTime _dateTime2;
};
