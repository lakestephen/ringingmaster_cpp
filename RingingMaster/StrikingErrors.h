#pragma once


class StrikingErrors
{
public:
	StrikingErrors(void);
	virtual ~StrikingErrors(void);

	void Serialize(CArchive &ar);
	
	bool operator !=(const StrikingErrors &strikingErrors);
	bool operator ==(const StrikingErrors &strikingErrors);


	void setHandstrokeLateness(double handstrokeLateness);
	void setBackstrokeLateness(double backstrokeLateness);
	void setHandstrokeQuality(double handstrokeQuality);
	void setBackstrokeQuality(double backstrokeQuality);
	void setFollowPreviousBellError(BOOL followPreviousBellError);
	void setQuiet(BOOL quiet);
	void setUseStrikingErrors(BOOL useStrikingErrors);
	void setLockHandAndBack(BOOL lockHandAndBack);

	double getHandstrokeLateness();
	double getBackstrokeLateness();
	double getHandstrokeQuality();
	double getBackstrokeQuality();
	BOOL getFollowPreviousBellError();
	BOOL getQuiet();
	BOOL getUseStrikingErrors();
	BOOL getLockHandAndBack();

	void reset();

protected:
	BOOL _useStrikingErrors;
	BOOL _quiet;
	double _handstrokeLateness; //-100% to +100%
	double _backstrokeLateness; 
	double _handstrokeQuality;//0 to + 100%
	double _backstrokeQuality;
	BOOL _followPreviousBellError;
	BOOL _lockHandAndBack;
};
