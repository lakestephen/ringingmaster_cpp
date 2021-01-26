// Tower.h: interface for the Tower class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Stroke.h"
#include "TowerTTSItem.h"
#include "TowerBell.h"
#include "SimulatorAction.h"
#include "SimulatorActionMap.h"
#include "StrikingErrors.h"


#define TOWER_RANDOM_DEVIATION_MAX 2.0

enum SmallerRingType 
{
	sr_front,
	sr_back
	
};

enum TowerValidity
{
	tv_ok = true, 
	tv_wavProblem,
	tv_bellInputProblem,
	tv_commandInputProblem,
	tv_callInputProblem,
	tv_methodInputProblem,
	tv_simulatorDisplay,
};

enum SimulatorPictureType
{
	spt_hand_left,	
	spt_back_left,	
	spt_hand_right,	
	spt_back_right,//used to size arrays	
};
enum SimulatorSetupType
{
	sst_home, 
	sst_tower_serial_one_reflector, 
	sst_tower_serial_two_reflector, 
	sst_tower_mbi_one_reflector, 
	sst_tower_mbi_two_reflector, 
};

class Tower  
{
public:
	BOOL getAddMethodShortcutsAsKeyboardInputs();
	void setAddMethodShortcutsAsKeyboardInputs(BOOL addMethodShortcutsAsKeyboardInputs);

	BOOL getAddMethodNamesAsMicrophoneInputs();
	void setAddMethodNamesAsMicrophoneInputs(BOOL addMethodNamesAsMicrophoneInputs);

	TowerValidity getTowerValidityType(SimulatorActionType type);
	void setSimulatorDefaults(SimulatorSetupType type, int port, bool setupSpeech);
	void setUpListeningTest(int bellsAffected, bool allowFllowOn, bool lockHandAndBack, int maxNumber, double maxPercent);
	Stroke getSimulatorRoundsStartStroke();
	int getDelay(int bellToPlay, Stroke stroke, bool& mute);
	bool isMute(int bellToPlay);
	StrikingErrors& getStrikingError(int bell);
	void setAllStrikingOn(BOOL on);
	void resetAllStriking();
	BOOL getClockwise();
	void setClockwise(BOOL clockwise);
	CxImage& getBellImage(SimulatorPictureType type);
	CFont& getBellFont();
	void editBellFont(CWnd* pWnd);
	const FontInfo& getBellFontInfo() const;
	void rebuildSimulatorGUIComponents();
	CString getPicturePath(SimulatorPictureType type) const;
	const CString& getPictureBaseDir();
	void setPictureBaseDir(const CString& baseDir);
	void setupForEditing();
	void cleanupAfterEditing();
	int getFirstSimulatedBell();
	void setFirstSimulatedBell(int firstSimulatedBell);
	SimulatorAction& getAction(SimulatorActionType action) const;
	int getBellInputCount();
	void addAction(SimulatorActionType action);
	void resetOverallFrequency();
	void resetOverallVolume();
	BOOL placeUserAtCenterOfGravityWouldChange();
	void placeUserAtCenterOfGravity();
	void moveUser(double xMeters, double yMeters, double zMeters);
	bool arrangerBellsInCircle(int numberAtBottom, double radius,  bool clockwise, bool userInMiddle);
	double getOverallVolume();
	bool setOverallVolume(double overallVolume);
	double getOverallFrequency();
	bool setOverallFrequency(double overallFrequency);
	BOOL getUse3D();
	void setUse3D(BOOL use3D);
	bool createFromTemplate();
	void removeAllFiles();
	void showMethodInfo(int bell, Stroke stroke, CWnd* pWnd);
	bool editPath(int bell, Stroke stroke, CWnd* pWnd);
	TowerTTSItem* getTowerTTSItem(int index);
	int getTowerTTSItemCount();
	CString getTTSString(TowerTTSTypes type, CString token = CString(""));
	static bool CreateTower(CString treble, Tower& tower, CString& msg);
	TowerValidity isValid(CString& msg);
	bool isAllowableClash(SimulatorActionType type1, SimulatorActionType type2);
	int getNumberInclCover(int initNumber);
	TowerBellPersist& getTowerBellPersist(int bell);
	TowerBell getTowerBell(int bell, Stroke stroke);
	void setFile(int index, CString fileName, Stroke stroke);
	bool canDecrementSpeed();
	bool canIncrementSpeed();
	bool canDecrementOverallFrequency();
	bool canIncrementOverallFrequency();
	bool decrementSpeed();
	bool incrementSpeed();
	bool decrementOverallFrequency();
	bool incrementOverallFrequency();
	void setSpeed(int speed) {_speed = speed;}
	void setName(CString name) {_name = name;}
	void setOpenHandstrokeGap(double openHandstrokeGap) {_openHandstrokeGap = openHandstrokeGap;}

	void setSmallerRingType(SmallerRingType smallerRingType) {_smallerRingType = smallerRingType;}
	void setExtraCoverForOddBells(BOOL extraCoverForOddBells) {_extraCoverForOddBells = extraCoverForOddBells;}
	BOOL getExtraCoverForOddBells() {return _extraCoverForOddBells;}

	void setCoverCount(int coverCount) {_coverCount= coverCount;}
	BOOL getCoverCount() {return _coverCount;}

	void setStartChangeRepeat(int startChangeRepeat) {_startChangeRepeat = startChangeRepeat;}
	void setEndChangeRepeat(int endChangeRepeat) {_endChangeRepeat = endChangeRepeat;}
	
	void setNumber(int number, bool silent = true);

	CString getDisplayName();
	Tower(const Tower& tower);
	void Serialize(CArchive& ar);

	void deserialize_v1_v2(CArchive& ar);//upto version 2.01
	void deserialize_v3(CArchive& ar);//version 3.01 onwards

	Tower& operator =(const Tower &tower);	
	bool hasChangedForPlayManager(const Tower &tower, bool& needToRestart);	
	void copyNonRestartChangesForPlayManager(const Tower& tower);
	Tower();
	virtual ~Tower();

	CString getName()						{return _name;}
	int getNumber()							{return _number;}
	int getSpeed()							{return _speed;}
	double getOpenHandstrokeGap()			{return _openHandstrokeGap;}
	SmallerRingType getSmallerRingType()	{return _smallerRingType;}
	int getStartChangeRepeat()				{return _startChangeRepeat;}
	int getEndChangeRepeat()				{return _endChangeRepeat;}
	
	int getOffset(int numberIncludingCover);
	bool isExistingPath(int number) ;

	static const int _maxSpeed;
	static const int _minSpeed;

	static const double _maxFreq;
	static const double _minFreq;

protected	:
	
	void setupSpeechControls();
	void setupSerialBells(int reflectorCount, int port);
	SimulatorInput* setupSimulatorinputSerial(int reflectorCount, int port, PortControlItem portControlItem);
	void setupKeyboardBells();
	void setupKeyboardControls();
	void setupMBIBells(int reflectorCount);

	void deleteTowerTTSItems();
	void setupDefaultTTSTypes();

	CString _name;
	int _number;
	int _speed;
	double _openHandstrokeGap;
	int _coverCount;
	BOOL _extraCoverForOddBells;
	int _quality;   //this is obsolete, BUT I see there may be a need for it in the future
	SmallerRingType  _smallerRingType;
	TowerBellPersist _files[MAXBELLS];

	int _startChangeRepeat;
	int _endChangeRepeat;

	TowerTTSItems _towerTTSItems;

	BOOL  _use3D;
	double _overallVolume;
	double _overallFrequency;

	//simulator profile stuff
	mutable SimulatorAction _simulatorAction[SIMULATOR_ACTION_COUNT+1];   
	int _firstSimulatedBell;
	CString _pictureBaseDir;
	FontInfo _bellFontInfo;
	CFont _bellFont;
	CxImage _bellImages[spt_back_right+1];
	BOOL _clockwise;
	BOOL _addMethodShortcutsAsKeyboardInputs;
	BOOL _addMethodNamesAsMicrophoneInputs;

	StrikingErrors _strikingErrors[MAXBELLS];

};
