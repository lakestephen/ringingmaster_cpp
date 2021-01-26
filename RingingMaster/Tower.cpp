// Tower.cpp: implementation of the Tower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "Tower.h"	
#include "GenericPropDlg.h"
#include "PlayBuffer.h"
#include "KeyboardManager.h"
#include "TowerManager.h"
#include "SimulatorInput.h"
#include "SerialControlItem.h"
#include "MultiBellItem.h"
#include "SpeechItem.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
									


const int Tower::_maxSpeed = 650;
const int Tower::_minSpeed = 50;
const int speedIncrement = 1;

const double Tower::_maxFreq = 500.0;
const double Tower::_minFreq = 1.0;
const double frequencyIncrement = .50;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
	
Tower::Tower() :
_name("Tower"),
_number(8),
_speed(180),
_openHandstrokeGap(1.0),
_extraCoverForOddBells(TRUE),
_coverCount(0),
_quality(0), 
_smallerRingType(sr_back),
_startChangeRepeat(3),
_endChangeRepeat(2),
_use3D(TRUE),
_overallVolume(-12),//in Db
_overallFrequency(100),//in percent 
_firstSimulatedBell(0), 
_clockwise(true), 		
_addMethodShortcutsAsKeyboardInputs(true), 
_addMethodNamesAsMicrophoneInputs(true)


{
	setupDefaultTTSTypes();

	//set up the images
	RGBQUAD quad;  
	quad.rgbRed = 255;
	quad.rgbGreen = 255;
	quad.rgbBlue = 255;

	for (int i=0;i<=spt_back_right;i++)
	{
		_bellImages[i].SetTransIndex(0);
		_bellImages[i].SetTransColor(quad);
	}

	_bellFontInfo._size = 120;
	_bellFontInfo._name = "Arial";
	_bellFontInfo._color = RGB(128,128,128);

	_bellFontInfo._weight = FW_BOLD;
}

Tower::Tower(const Tower &tower)
{
	operator = (tower);
}

Tower::~Tower()
{
	deleteTowerTTSItems();
}

void Tower::copyNonRestartChangesForPlayManager(const Tower& tower)
{
	_name = tower._name;

	deleteTowerTTSItems();
	for (int i=0;i<tower._towerTTSItems.GetSize();i++)
	{
		TowerTTSItem* item = new TowerTTSItem(*tower._towerTTSItems.GetAt(i));
		_towerTTSItems.Add(item);
	}

	for (int i=0;i<MAXBELLS;i++)
		_strikingErrors[i] = tower._strikingErrors[i];
}

//NOTE: only used for play manager to determine if the tower needs to be reloaded
bool Tower::hasChangedForPlayManager(const Tower &tower, bool& needToRestart)
{
	//this is the stuff that can change, 
	// but does NEEDS a restart of the play thread
	needToRestart = true;

	if (_number				!= tower._number)				return true;
	if (_extraCoverForOddBells != tower._extraCoverForOddBells) return true;
	if (_coverCount			!= tower._coverCount)			return true;
	if (_smallerRingType	!= tower._smallerRingType)		return true;
	if (_startChangeRepeat	!= tower._startChangeRepeat)	return true;
	if (_endChangeRepeat	!= tower._endChangeRepeat)		return true;
	if (_use3D				!= tower._use3D)				return true;
	if (_overallVolume		!= tower._overallVolume)		return true;
	if (_overallFrequency   != tower._overallFrequency)		return true;
	if (_speed				!= tower._speed)				return true; 
	if (_openHandstrokeGap	!= tower._openHandstrokeGap)	return true; 
	if (_quality			!= tower._quality)				return true; 

	for (int i=0;i<MAXBELLS;i++)
		if (_files[i] != tower._files[i]) return true;

	if (_towerTTSItems.GetSize()!= tower._towerTTSItems.GetSize()) return true;

	//this is the stuff that can change, 
	// but DOES NOT NEED a restart of the play thread
	//See Tower::copyNonRestartChanges
	needToRestart = false;

	if (_name				!= tower._name)					return true;

	for (int i=0;i<tower._towerTTSItems.GetSize();i++)
	{
		if (*_towerTTSItems.GetAt(i) != *tower._towerTTSItems.GetAt(i))
			return true;
	}

	for (int i=0;i<MAXBELLS;i++)
		if (_strikingErrors[i] !=  tower._strikingErrors[i]) return true;


	return false;
}	  

Tower& Tower::operator =(const Tower &tower)
{
	_name			= tower._name;
	_number			= tower._number;
	_speed			= tower._speed;
	_openHandstrokeGap = tower._openHandstrokeGap;
	_extraCoverForOddBells			= tower._extraCoverForOddBells;
	_coverCount = tower._coverCount;
	_quality		= tower._quality;
	_smallerRingType = tower._smallerRingType;
	_startChangeRepeat = tower._startChangeRepeat;
	_endChangeRepeat = tower._endChangeRepeat;
	_use3D			= tower._use3D;
	_overallVolume  = tower._overallVolume;
	_overallFrequency = tower._overallFrequency;

	for (int i=0;i<MAXBELLS;i++)
		_files[i] = tower._files[i];
	
	deleteTowerTTSItems();
	for (int i=0;i<tower._towerTTSItems.GetSize();i++)
	{
		TowerTTSItem* item = new TowerTTSItem(*tower._towerTTSItems.GetAt(i));
		_towerTTSItems.Add(item);
	}

	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
		getAction((SimulatorActionType)i) = tower.getAction((SimulatorActionType)i);

	for (int i=0;i<MAXBELLS;i++)
		_strikingErrors[i] = tower._strikingErrors[i];

	_firstSimulatedBell = tower._firstSimulatedBell;
	_clockwise = tower._clockwise;
	_pictureBaseDir = tower._pictureBaseDir;
	_bellFontInfo = tower._bellFontInfo; 
	_addMethodShortcutsAsKeyboardInputs = tower._addMethodShortcutsAsKeyboardInputs;
	_addMethodNamesAsMicrophoneInputs = tower._addMethodNamesAsMicrophoneInputs;

	return (*this);
}


void Tower::Serialize(CArchive &ar)
{
	int version = 3;
	if (ar.IsStoring())
	{
		//version 1 not kept.
		ar << version;
		ar << _name;
		ar << _number;
		ar << _speed;
		ar << _extraCoverForOddBells;
		ar << _quality;
		ar << _smallerRingType;

		for (int i=0;i<MAXBELLS;i++)
			_files[i].Serialize(ar);

		//new for version 2 / 2.01 

		ar << _startChangeRepeat;

		/***************new in v 3.01 / v3 ***************************/
		
		ar << _towerTTSItems.GetSize();

		for (int i=0;i<_towerTTSItems.GetSize();i++)
		{
			_towerTTSItems.GetAt(i)->Serialize(ar);
		}

		ar << _endChangeRepeat;
		ar << _use3D;
		ar << _overallVolume;
		ar << _overallFrequency;
		ar << _openHandstrokeGap;
		ar << _coverCount;

		for (int i=0;i<MAXBELLS;i++)
			_strikingErrors[i].Serialize(ar);

		ar << SIMULATOR_ACTION_COUNT;
		for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
			getAction((SimulatorActionType)i).Serialize(ar);

		ar << _firstSimulatedBell;
		ar << _clockwise;
		ar << _pictureBaseDir;
		_bellFontInfo.Serialize(ar);
		
		ar << _addMethodShortcutsAsKeyboardInputs;
		ar << _addMethodNamesAsMicrophoneInputs;

	}
	else
	{
		ar >> version;

		TRACE("Deserialising Tower Version %d \r\n", version);

		if ((version == 1)||(version == 2))
			deserialize_v1_v2(ar);//upto version 2.01 
		else if (version == 3)
			deserialize_v3(ar);	 //version 3.01 on 
		else
			AfxThrowArchiveException(CArchiveException::endOfFile);	   

	}
}	
	
	
void Tower::deserialize_v1_v2(CArchive &ar)
{	
	ar >> _name;
	ar >> _number;
	ar >> _speed;
	ar >> _extraCoverForOddBells;
	ar >> _quality;
	int smallerRingType ;
	ar >> smallerRingType;
	_smallerRingType = static_cast<SmallerRingType>(smallerRingType);
	int lead;
	ar >> lead;
	if (lead == 0) // l_open
		_openHandstrokeGap = 1.0;
	else if (lead == 1) // l_cartwheel
		_openHandstrokeGap = 0.0;

	CString file;
	for (int i=0;i<MAXBELLS;i++)
	{
		ar >> file;
		_files[i].getTowerBellItem(hand).setFile(file);
	}
	for (int i=0;i<MAXBELLS;i++)
	{
		ar >> file;
		_files[i].getTowerBellItem(back).setFile(file);
	}


	//new for version 2 / 2.01  

	ar >> _startChangeRepeat;  

	//use default towerTTSItems

	arrangerBellsInCircle(0,2.5, TRUE, TRUE);

	//create a default set of simulator stuff
	setSimulatorDefaults(sst_home,0, true);
}

void Tower::deserialize_v3(CArchive &ar)
{	
	ar >> _name;
	ar >> _number;
	ar >> _speed;
	ar >> _extraCoverForOddBells;
	ar >> _quality;
	int smallerRingType ;
	ar >> smallerRingType;
	_smallerRingType = static_cast<SmallerRingType>(smallerRingType);

	for (int i=0;i<MAXBELLS;i++)
		_files[i].Serialize(ar);;

	ar >> _startChangeRepeat;  

	/***************new in v 3.01 / v3 ***************************/

	deleteTowerTTSItems();

	int size;
	ar >> size;
	for (int i=0;i<size;i++)
	{
		TowerTTSItem* item = new TowerTTSItem();
		item->Serialize(ar);

		_towerTTSItems.Add(item);
	}

	ar >> _endChangeRepeat;  
	ar >> _use3D;
	ar >> _overallVolume;
	ar >> _overallFrequency;
	ar >> _openHandstrokeGap;
	ar >> _coverCount;

	for (int i=0;i<MAXBELLS;i++)
		_strikingErrors[i].Serialize(ar);


	size = 0;
	ar >> size;
	for (int i=0;i<size;i++)
		getAction((SimulatorActionType)i).Serialize(ar);

	ar >> _firstSimulatedBell;
	ar >> _clockwise;
	ar >> _pictureBaseDir;
	_bellFontInfo.Serialize(ar);	

	ar >> _addMethodShortcutsAsKeyboardInputs;
	ar >> _addMethodNamesAsMicrophoneInputs;
}					   

CString Tower::getDisplayName()
{
	CString str;
	
	str.Format("%s (%d)", _name, _number);

	return str;

}

//speed == 50 - 750 ms


bool Tower::incrementSpeed()
{	
	if ((_speed - speedIncrement) > _minSpeed)
	{
		_speed -= speedIncrement;
		return true;
	}
	return false;	
}

bool Tower::decrementSpeed()
{
	if ((_speed + speedIncrement) < _maxSpeed)
	{
		_speed += speedIncrement;
		return true;
	}
	return false;	
}

bool Tower::canIncrementSpeed()
{
	if ((_speed - speedIncrement) > _minSpeed)
	{
		return true;
	}
	return false;	
}

bool Tower::canDecrementSpeed()
{
	if ((_speed + speedIncrement) < _maxSpeed)
	{
		return true;
	}
	return false;	
}

//frequency 1% and 500%


bool Tower::decrementOverallFrequency()
{	
	if ((_overallFrequency - frequencyIncrement) > _minFreq)
	{
		_overallFrequency -= frequencyIncrement;
		return true;
	}
	return false;	
}

bool Tower::incrementOverallFrequency()
{
	if ((_overallFrequency + frequencyIncrement) < _maxFreq)
	{
		_overallFrequency += frequencyIncrement;
		return true;
	}
	return false;	
}

bool Tower::canIncrementOverallFrequency()
{								  
	if ((_overallFrequency - frequencyIncrement) > _minFreq)
	{
		return true;
	}
	return false;	
}

bool Tower::canDecrementOverallFrequency()
{
	if ((_overallFrequency + frequencyIncrement) < _maxFreq)
	{
		return true;
	}
	return false;	
}

void Tower::setFile(int index, CString fileName, Stroke stroke)
{
	ASSERT(index < MAXBELLS);

	if(index < MAXBELLS)
		_files[index].getTowerBellItem(stroke).setFile(fileName);
}

int Tower::getOffset(int numberIncludingCover)
{
	if ((numberIncludingCover < _number)&&(_smallerRingType == sr_back))
		return _number - numberIncludingCover;

	return 0;
}

TowerBell Tower::getTowerBell(int bell, Stroke stroke)
{
	return TowerBell(getTowerBellPersist(bell), stroke, _overallVolume, _overallFrequency, _use3D);
}

TowerBellPersist& Tower::getTowerBellPersist(int bell)
{
	ASSERT(bell < MAXBELLS);

	return _files[bell];
}

int Tower::getNumberInclCover(int initNumber)
{
	//add on the odd bell cover
	if (_extraCoverForOddBells&&
		!GlobalFunctions::isEven(initNumber))
		initNumber++;

	//now add extra covers
	initNumber += _coverCount;

	return min( MAXBELLS, initNumber);
}

TowerValidity Tower::isValid(CString& msg)
{
	TowerValidity valid = tv_ok;
	CString tmp;

	for (int i=0;i<_number;i++)
	{
		if (!getTowerBellPersist(i).getTowerBellItem(hand).isValid())
		{
			valid = tv_wavProblem;
			CString str;
			str.Format(" %d hand is not a valid .wav file\r\n", i+1);
			tmp+= str;
		}

		if (!getTowerBellPersist(i).getTowerBellItem(back).isValid())
		{
			valid = tv_wavProblem;
			CString str;
			str.Format(" %d back is not a valid .wav file\r\n", i+1);
			tmp+= str;
		}
	}

	if (valid == tv_wavProblem)
	{
		msg += "Not all the bells have valid sound files attached. \nThere is a problem with the following bells:\r\n";
		msg += tmp;
		msg += "\r\n";
		return valid;
	}


	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
	{
		if (!getAction((SimulatorActionType)i).isValid())
		{
			CString temp;
			temp.Format("Please choose a valid simulator input for %s or select no input\r\n",getSimulatorActionTypeName((SimulatorActionType)(MAXBELLS + i)));
			msg += temp;
			valid = getTowerValidityType((SimulatorActionType)i);
		}
	}

	//make sure that we do not have the same input for two bells.
	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
	{
		SimulatorAction& bell1 = getAction((SimulatorActionType)i);

		for (int j=0;j<bell1.getInputCount();j++)
		{
			SimulatorInput* input1 = bell1.getInput(j);

			for (int k=i;k<SIMULATOR_ACTION_COUNT;k++)
			{
				SimulatorAction& bell2 = getAction((SimulatorActionType)k);

				for (int l=((k==i)?j+1:0);l<bell2.getInputCount();l++)
				{
					SimulatorInput* input2 = bell2.getInput(l);

					if ((input1->isInput())&&
						(input2->isInput())&&
						(*input1 == *input2)&&
						!isAllowableClash((SimulatorActionType)i, (SimulatorActionType)k))
					{
						CString temp;
						temp.Format("The input '%s' is used for '%s' and '%s'\r\n", 
								input1->getDescription(sidt_long_base),
								getSimulatorActionTypeName((SimulatorActionType)i),
								getSimulatorActionTypeName((SimulatorActionType)k));
						msg += temp;
						valid = getTowerValidityType((SimulatorActionType)i);
					}
				}
			}
		}
	}
		  
	if (_firstSimulatedBell > _number)
	{
		CString temp;
		temp.Format("Please choose a first simulated bell that is less than the number of bells.\r\n");
		msg += temp;
		valid = tv_simulatorDisplay;
	}

	CFileFind fileFind;
	if (!fileFind.FindFile(_pictureBaseDir + "."))
	{
		CString temp;
		temp.Format("The 'Images Root Directory' is not valid\r\n");
		msg += temp;
		valid = tv_simulatorDisplay;
	}

	for (int i=spt_hand_left;i<=spt_back_right;i++)
	{
		CString path = getPicturePath((SimulatorPictureType)i);
		
		CFileFind fileFindPath;
		if (!fileFindPath.FindFile(path))
		{
			CString temp;
			temp.Format("Image %s is missing.\r\n", path);
			msg += temp;
			valid = tv_simulatorDisplay;
		}
	}    

	return valid;
}

TowerValidity Tower::getTowerValidityType(SimulatorActionType type)
{
	if (type >=SIMULATOR_ACTION_FIRST_BELL && type <= SIMULATOR_ACTION_LAST_BELL)
		return tv_bellInputProblem;
	if (type >=SIMULATOR_ACTION_FIRST_ACTION && type <= SIMULATOR_ACTION_LAST_ACTION)
		return tv_commandInputProblem;
	if (type >=SIMULATOR_ACTION_FIRST_CALL && type <= SIMULATOR_ACTION_LAST_CALL)
		return tv_callInputProblem;
	if (type >=SIMULATOR_ACTION_FIRST_METHOD && type <= SIMULATOR_ACTION_LAST_METHOD)
		return tv_methodInputProblem;

	ASSERT(FALSE);
	return tv_bellInputProblem;
}

bool Tower::isAllowableClash(SimulatorActionType type1,  SimulatorActionType type2)
{
	//not interested in stuff greater than the number of bells
	//they will be caught if and when the number of bells is raised
	if (type1 >= (SimulatorActionType)_number && type1 <= sat_bell29)
		return true;
	if (type2 >= (SimulatorActionType)_number && type2 <= sat_bell29)
		return true;

	//to2do any more allowable clashes. Must check the lowest value first

	if (type1 == sat_goNext && type2 == sat_stand)
		return true;

	if (type1 == sat_goNext && type2 == sat_callPlain)
		return true;
	if (type1 == sat_goNext && type2 == sat_callBob)
		return true;
	if (type1 == sat_goNext && type2 == sat_callX)
		return true;
	if (type1 == sat_goNext && type2 == sat_callY)
		return true;
	if (type1 == sat_goNext && type2 == sat_callZ)
		return true;

	return false;
}

//This static function is used to auto populate the tower 
bool Tower::CreateTower(CString treble, Tower& tower, CString& msg)
{
	if (treble.IsEmpty())
	{
		msg = "Please choose a treble";
		return false;
	}

	CFileFind fileFind;

	if (fileFind.FindFile(treble))
	{
		fileFind.FindNextFile();

		//get the file details
		CString name = fileFind.GetFileName(); //with extention
		CString path = fileFind.GetFilePath();
		path.Replace(name, "");
		CString title = fileFind.GetFileTitle();//this is without extention
		CString extention = name;
		extention.Replace(title, "");
		CString token = title;

		//have we got leading zero?
		if (title.Find("01") != -1)
		{
			int pos = title.Find("01");
			token.Replace("01", "");
			for (int i=0;i<MAXBELLS;i++)
			{
				CString newTitle = token;
				CString number;
				number.Format("%s%d", ((i<9)?"0":""),i+1);
				newTitle.Insert(pos, number);
				tower.setFile(i, path + newTitle + extention, hand);
				tower.setFile(i, path + newTitle + extention, back);
			}
		}
		else if (title.Find("1") != -1)
		{
			int pos = title.Find("1");
			token.Replace("1", "");
			for (int i=0;i<MAXBELLS;i++)
			{
				CString newTitle = token;
				CString number;
				number.Format("%d", i+1);
				newTitle.Insert(pos, number);
				tower.setFile(i, path + newTitle + extention, hand);
				tower.setFile(i, path + newTitle + extention, back);
				tower.setNumber(max(i+1,3));
			}
		}
		else
		{
			msg = "The treble does not have a 1 in the name. You will have to add the files manually";
			return false;
		}	

		//check files valid
		for (int i=0;i<MAXBELLS;i++)
		{
			CFileFind findHand, findBack;
			bool ok = true;
			if (!findHand.FindFile(tower.getTowerBellPersist(i).getTowerBellItem(hand).getFile()))
			{
				ok = false;
				tower.setFile(i, "", hand);
			}
			if (!findBack.FindFile(tower.getTowerBellPersist(i).getTowerBellItem(back).getFile()))
			{
				ok = false;
				tower.setFile(i, "", back);
			}					   

			if (ok)
				tower.setNumber(max(i+1,3));
		}
		
		//if we are doing a virgin setup, lets be helpfull and
		//set the tower name, and the number
		if (tower._name == "Tower")
		{
			token.Replace("_", " ");
			tower._name  = GlobalFunctions::titleString(token);

			tower.arrangerBellsInCircle(0,2.5, TRUE, TRUE);
		}


		tower.setSimulatorDefaults(sst_home, 0, true);


		return true;
	}
	else
	{
		msg = "Please choose a valid .wav file";
		return false;
	}
}

CString Tower::getTTSString(TowerTTSTypes type, CString token)
{
	for (int i=0;i<_towerTTSItems.GetSize();i++)
	{
		if ((_towerTTSItems.GetAt(i)->_active) &&
			(_towerTTSItems.GetAt(i)->_type == type))
		{
			CString str = _towerTTSItems.GetAt(i)->_string;
			str.Replace("<>", token);
			return str;
		}
	}

	return ""; 
}

void Tower::setupDefaultTTSTypes()
{
	deleteTowerTTSItems();

	_towerTTSItems.Add(new TowerTTSItem(tttst_start));
	_towerTTSItems.Add(new TowerTTSItem(tttst_go));
	_towerTTSItems.Add(new TowerTTSItem(tttst_thatsAll));
	_towerTTSItems.Add(new TowerTTSItem(tttst_rounds));
	_towerTTSItems.Add(new TowerTTSItem(tttst_stand));
	_towerTTSItems.Add(new TowerTTSItem(tttst_call));
	_towerTTSItems.Add(new TowerTTSItem(tttst_splice));
	_towerTTSItems.Add(new TowerTTSItem(tttst_part));
	
}

int Tower::getTowerTTSItemCount()
{
	return _towerTTSItems.GetSize();
}

TowerTTSItem* Tower::getTowerTTSItem(int index)
{
	ASSERT(index >= 0 && index <	_towerTTSItems.GetSize());
	if (index >= 0 && index <	_towerTTSItems.GetSize())
		return 	_towerTTSItems.GetAt(index);

	return NULL;
}

void Tower::deleteTowerTTSItems()
{

	for (int i=0;i<_towerTTSItems.GetSize();i++)
		delete _towerTTSItems.GetAt(i);

	_towerTTSItems.RemoveAll();
}



bool Tower::editPath(int bell, Stroke stroke, CWnd* pWnd)
{
	CFileDialog dlg(TRUE, "wav", getTowerBellPersist(bell).getTowerBellItem(stroke).getFile(), NULL, 
	"Wave Files (*.wav)|*.wav|All Files (*.*)|*.*|", pWnd);

	if (dlg.DoModal() == IDOK)
	{
		setFile(bell, dlg.GetPathName(), stroke);
		return true;
	}
	return false;
}

void Tower::showMethodInfo(int bell, Stroke stroke, CWnd *pWnd)
{
	//get the path
	const TowerBell& item = getTowerBell(bell, stroke);
	CString path = item.getFile();
	
	//check that teh path is valid
	GenericPropDlg dlg(pWnd, path.IsEmpty()?"Wav File":path, "Tower");

	Riff riff;
	CString file;
	if (!riff.readRiffHeader(path, file))
	{
		dlg.addProp("Status", "Invalid File");
	}
	else
	{
		dlg.addProp("File", file);

		CString str;

		str.Format("%d Bytes", riff.riffHead.dwSize + 8);
		dlg.addProp("File Length", str);

		str.Format("%d", riff.wave.fmtDetails.wChnls);
		dlg.addProp("Channels", str);

		str.Format("%d Hz", riff.wave.fmtDetails.dwSRate);
		dlg.addProp("Original Sample Rate", str );

		if (item.isSettingFrequency() )
		{
			str.Format("%d Hz", item.getFrequency());
			dlg.addProp("Override Sample Rate", str );
		}
  
		str.Format("%d", riff.wave.fmtDetails.BitsPerSample);
		dlg.addProp("Bits per Sample", str);

		str.Format("%d", riff.wave.dataHead.dwSize);
		dlg.addProp("Number of Samples", str);

		str.Format("%d ms", ((riff.wave.dataHead.dwSize *1000 )/ riff.wave.fmtDetails.dwSRate));
		dlg.addProp("Sample Length", str);
	}

	dlg.DoModal();
}

void Tower::removeAllFiles()
{
	for (int i=0;i<MAXBELLS;i++)
	{
		setFile(i, "", back);
		setFile(i, "", hand);
	}
}

bool Tower::createFromTemplate()
{
	CString msg;
	CFileFind fileFind;
	Tower tower = *this;
					  
	CString initialFile = getTowerBellPersist(0).getTowerBellItem(hand).getFile();
	if (initialFile.IsEmpty())
		initialFile = app()->_rootPath + "Bells\\*.*";

	CFileDialog dlg(TRUE, "wav", initialFile, NULL, 
		"Wave Files (*.wav)|*.wav|All Files (*.*)|*.*|", NULL);

	dlg.m_ofn.lpstrTitle = "Choose the treble.";
					  
	while (dlg.DoModal() == IDOK)
	{
		if (Tower::CreateTower(dlg.GetPathName(), tower, msg))
		{
			//we have a valid tower - do we overwrite
			if ((isExistingPath(tower.getNumber())&&
			    (AfxMessageBox("This will overwrite the other bells paths. Do you want to continue?", MB_YESNO|MB_ICONQUESTION) != IDYES)))
				return false;
	
			*this = tower;

			return true;
		}
		else 
			AfxMessageBox(msg);
	}		 

	return false;


}

bool Tower::isExistingPath(int number) 
{

	for (int i=0;i<number;i++)
		if ((!getTowerBellPersist(i).getTowerBellItem(hand).getFile().IsEmpty())||
			(!getTowerBellPersist(i).getTowerBellItem(back).getFile().IsEmpty()))
			return true;

	return false;
}

void Tower::setUse3D(BOOL use3D)
{
	_use3D = use3D;
}

BOOL Tower::getUse3D()
{
	return _use3D;
}

bool Tower::setOverallVolume(double overallVolume)
{
	if ((overallVolume >= -40)||(overallVolume <=0))
	{
		_overallVolume = overallVolume;
		return true;
	}

	return false;
}

double Tower::getOverallVolume()
{
	return _overallVolume;
}
void Tower::resetOverallVolume()
{
	_overallVolume = -12;//Db
}

bool Tower::setOverallFrequency(double overallFrequency)
{
	if ((overallFrequency >= _minFreq)||(overallFrequency <=_maxFreq))
	{
		_overallFrequency = overallFrequency;
		return true;
	}

	return false;
}

double Tower::getOverallFrequency()
{
	return _overallFrequency;
}

void Tower::resetOverallFrequency()
{
	_overallFrequency = 100;//%
}


bool Tower::arrangerBellsInCircle(int numberAtBottom, double radius,  bool clockwise, bool userInMiddle)
{
	ASSERT((numberAtBottom  >= 0)&&(numberAtBottom < _number));

	if ((numberAtBottom  >= 0)&&(numberAtBottom < _number))
	{
		//find the angle in rad
		double segment = PI2/_number;
		double pos = PI;
		double offset = (numberAtBottom * segment);

		if (clockwise)
			pos -= offset;
		else
			pos += offset;

		//calculate angles - this allows an error to bomb out.
		for (int i=0;i<_number;i++)
		{
			double X, Y;
			TowerBellPersist& item = getTowerBellPersist(i);

			GlobalFunctions::convertAngleToXY(pos, radius, X, Y);

			if (!userInMiddle)
				Y+=radius;

			item.setPosX(X);
			item.setPosZ(Y);	

//			ASSERT(pos < PI2);

			if (clockwise)
				pos += segment;
			else
				pos -= segment;
		}

		return true;
	}


	return false;
}



void Tower::setNumber(int number, bool silent) 
{
	if (number > MAXBELLS)
		number = MAXBELLS;
	if (number < 3)
		number = 3;

	if (_number != number)
	{
		_number = number;	 

		if ((silent)||
			(getUse3D()&&AfxMessageBox("As you are changing the number of bells, would you like to reset the 3D sound positions?", MB_YESNO) == IDYES))
		{
			arrangerBellsInCircle(0,2.5, TRUE, TRUE);
		}
	}
}

void Tower::moveUser(double xMeters, double yMeters, double zMeters)
{
	for (int i=0;i<_number;i++)
	{
		TowerBellPersist& item = getTowerBellPersist(i);

		double X = item.getPosX() + xMeters;
		double Y = item.getPosY() + yMeters;
		double Z = item.getPosZ() + zMeters;

		item.setPosX(X);
		item.setPosY(Y);
		item.setPosZ(Z);
	}
}

const double precision = 10e-10;

BOOL Tower::placeUserAtCenterOfGravityWouldChange()
{
	double X = 0;
	double Y = 0;
	double Z = 0;

	for (int i=0;i<_number;i++)
	{
		TowerBellPersist& item = getTowerBellPersist(i);

		X -= item.getPosX(); 
		Y -= item.getPosY();
		Z -= item.getPosZ();
	}

	X/=_number;
	Y/=_number;
	Z/=_number;

	return ((fabs(X) > precision)||
			(fabs(Y) > precision)||
			(fabs(Z) > precision));
}

void Tower::placeUserAtCenterOfGravity()
{
	double X = 0;
	double Y = 0;
	double Z = 0;

	for (int i=0;i<_number;i++)
	{
		TowerBellPersist& item = getTowerBellPersist(i);

		X -= item.getPosX(); 
		Y -= item.getPosY();
		Z -= item.getPosZ();
	}

	X/=_number;
	Y/=_number;
	Z/=_number;

	moveUser(X,Y,Z);
}

void Tower::setSimulatorDefaults(SimulatorSetupType type, int port, bool setupSpeech)
{
	//basic setup.
    _pictureBaseDir = app()->_rootPath + "Pictures\\bells\\";

	//reset  all the actions

	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
		getAction((SimulatorActionType)i).reset();      

	switch (type)
	{
	case sst_home:
		setupKeyboardBells();
		setupKeyboardControls();
		break;
	case sst_tower_mbi_one_reflector:
		setupMBIBells(1);
		setupKeyboardControls();
		break;
	case sst_tower_mbi_two_reflector:
		setupMBIBells(2);
		setupKeyboardControls();
		break;
	case sst_tower_serial_one_reflector:
		setupSerialBells(1,port);
		setupKeyboardControls();
		break;
	case sst_tower_serial_two_reflector:
		setupSerialBells(2,port);
		setupKeyboardControls();
		break;
	}   

	if (setupSpeech)
	{
		setupSpeechControls();
	}
}

void Tower::setupKeyboardControls()
{
	SimulatorInput* input;

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('R'))));
	getAction(sat_startRingingRounds).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan(' '))));
	getAction(sat_goNext).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('S'))));
	getAction(sat_stand).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan(' '))));
	getAction(sat_callBob).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('C'))));
	getAction(sat_callSingle).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('V'))));
	getAction(sat_callPlain).addInput(input);
}

void Tower::setupSpeechControls()
{
	SimulatorInput* input;

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("rounds")));
	getAction(sat_startRingingRounds).addInput(input);

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("go")));
	getAction(sat_goNext).addInput(input);

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("stand")));
	getAction(sat_stand).addInput(input);

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("bob")));
	getAction(sat_callBob).addInput(input);

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("single")));
	getAction(sat_callSingle).addInput(input);

	input = new SimulatorInput();
	input->setSpeech(new SpeechItem(SpeechAction("no call")));
	getAction(sat_callPlain).addInput(input);
}
void Tower::setupMBIBells(int reflectorCount)
{
	for (int i=0;i<12;i++)
	{
		SimulatorInput* input = new SimulatorInput();
		MultiBellItem* item = new MultiBellItem(MultiBellAction(i));
		SimulatorEventData& data = item->getSimulatorEventData();
		if (reflectorCount == 1)
		{
			data.setEventSequenceType(est_1of1);
			data.setDelay(150);
		}
		else
		{
			data.setEventSequenceType(est_1of2);
			data.setDelay(10);
		}          

		input->setMultiBellInterface(item);

		getAction((SimulatorActionType)i).addInput(input);		
	}
}

void Tower::setupSerialBells(int reflectorCount, int port)
{
	getAction(sat_bell0).addInput(setupSimulatorinputSerial(reflectorCount, port, pci_cts));

	getAction(sat_bell1).addInput(setupSimulatorinputSerial(reflectorCount, port, pci_dsr));

	getAction(sat_bell2).addInput(setupSimulatorinputSerial(reflectorCount, port, pci_rlsd));

	getAction(sat_bell3).addInput(setupSimulatorinputSerial(reflectorCount, port, pci_ring));
}


SimulatorInput* Tower::setupSimulatorinputSerial(int reflectorCount, int port, PortControlItem portControlItem)
{
	SimulatorInput* input = new SimulatorInput();
	SerialControlItem* item = new SerialControlItem(SerialControlAction(port, portControlItem));
	SimulatorEventData& data = item->getSimulatorEventData();
	if (reflectorCount == 1)
	{
		data.setEventSequenceType(est_1of1);
		data.setDelay(150);
	}
	else
	{
		data.setEventSequenceType(est_1of2);
		data.setDelay(10);
	}          

	input->setSerialControl(item);

	return input;
}


void Tower::setupKeyboardBells()
{
	SimulatorInput* input;

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('I'))));
	getAction(sat_bell0).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('J'))));
	getAction(sat_bell1).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('D'))));
	getAction(sat_bell2).addInput(input);

	input = new SimulatorInput();
	input->setKeyboard(new KeyboardItem(KeyboardAction(KeyboardManager::char2scan('E'))));
	getAction(sat_bell3).addInput(input);
}


 void Tower::addAction(SimulatorActionType action)
 {
	getAction(action).addInput(new SimulatorInput());
 }

int Tower::getBellInputCount()
{
	int count = 0;
	for (int i=0;i<MAXBELLS;i++)
	{
		count += getAction((SimulatorActionType)i).getInputCount();
	}

	return count;
}

SimulatorAction& Tower::getAction(SimulatorActionType action) const
{
	return _simulatorAction[action];
}
												
void Tower::setupForEditing()
{
	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
	{
		if (getAction((SimulatorActionType)i).getInputCount()== 0)
		{
			getAction((SimulatorActionType)i).addInput(new SimulatorInput());
		}
	}
}

void Tower::cleanupAfterEditing()
{
	for (int i=0;i<SIMULATOR_ACTION_COUNT;i++)
	{
		getAction((SimulatorActionType)i).clean();
	}
}

int Tower::getFirstSimulatedBell()
{
	return _firstSimulatedBell;
}

void Tower::setFirstSimulatedBell(int firstSimulatedBell)
{
	_firstSimulatedBell = firstSimulatedBell;
}

void Tower::setClockwise(BOOL clockwise)
{
	_clockwise = clockwise;
}

BOOL Tower::getClockwise()
{
	return _clockwise;
}

const CString& Tower::getPictureBaseDir()
{
	return _pictureBaseDir;
}

CString Tower::getPicturePath(SimulatorPictureType type) const
{
	//tod2o allow the use of things other than bitmaps.   can return const ref???
	CString str(_pictureBaseDir);
	switch (type)
	{
	case spt_hand_left:
		str += "HandLeft.bmp"; 
		break;
	case spt_back_left:
		str += "BackLeft.bmp";
		break;
	case spt_hand_right:
		str += "HandRight.bmp";
		break;
	case spt_back_right:
		str += "BackRight.bmp";
		break;	
	default:
		ASSERT(FALSE);
		break;
	}

	return str;
}

void Tower::setPictureBaseDir(const CString& baseDir)
{
	_pictureBaseDir = baseDir;
}

void Tower::editBellFont(CWnd* pWnd)
{
	_bellFontInfo.editFont(pWnd, true);
}

const FontInfo& Tower::getBellFontInfo() const
{
	return _bellFontInfo;
}

CFont& Tower::getBellFont()
{
	return _bellFont;
}

void Tower::rebuildSimulatorGUIComponents()
{
	LOGFONT lf;
	_bellFontInfo.getLogFont(&lf);
	_bellFont.DeleteObject();
	_bellFont.CreateFontIndirect(&lf);				

	for (int i=0;i<=spt_back_right;i++)
	{
		CString path = getPicturePath((SimulatorPictureType)i);
		_bellImages[i].Load(path);
	}
}

CxImage& Tower::getBellImage(SimulatorPictureType type)
{
	return _bellImages[type];
}


void Tower::setAllStrikingOn(BOOL on)
{
	for (int i=0;i<MAXBELLS;i++)
		_strikingErrors[i].setUseStrikingErrors(on);
}

void Tower::resetAllStriking()
{
	for (int i=0;i<MAXBELLS;i++)
		_strikingErrors[i].reset();
}

StrikingErrors& Tower::getStrikingError(int bell)
{
	ASSERT(bell >=0 && bell < MAXBELLS);

	if ((bell < 0) || (bell >=MAXBELLS))
		bell = 0;

	return _strikingErrors[bell];
}

bool Tower::isMute(int bellToPlay)
{
	StrikingErrors& error = getStrikingError(bellToPlay);	
	return (error.getQuiet() == TRUE);
}

double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;


	if(phase == 0) {
		do {
			srand(GetTickCount());

			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}

int Tower::getDelay(int bellToPlay, Stroke stroke, bool& mute)
{
 	//calculate the delay for the unevenness
	StrikingErrors& error = getStrikingError(bellToPlay);
	
	mute = (error.getQuiet() == TRUE);

	int delay = 0;
	static int previousDelay ;

	if (error.getUseStrikingErrors())
	{
		//
		double quality = 0.0;
		double lateness = 0.0;

		if (stroke == hand)
		{
			quality = error.getHandstrokeQuality();
			lateness = error.getHandstrokeLateness();
		}
		else
		{
			quality = error.getBackstrokeQuality();
			lateness = error.getBackstrokeLateness();
		}

		//apply following bell error
		if (error.getFollowPreviousBellError())
			delay += previousDelay;

		//apply lateness
		//lateness is +100% to -100%
		if (lateness != 0.0) 
			delay += (int)((lateness/100) * (double)_speed);	   

		//apply quality
		//quality is 0 to 100%, but we want it to be +/- 1 stroke
		if (quality > 0.0)
		{
			double random = 1000;//arbatorary high number			
			do 
			{
				random = (double)gaussrand(); //this has a std deviation of -1 , 1, wwe do not want to go outside this value
			}
			while (random < - TOWER_RANDOM_DEVIATION_MAX || random > TOWER_RANDOM_DEVIATION_MAX);

			random *= ((double)_speed / (double)TOWER_RANDOM_DEVIATION_MAX);

			TRACE("random %.2f \r\n", random);

			delay += (int)(random * ((double)quality/(double)100.0));
		}

		//check within limits
		if (delay <= (-_speed))
			delay = (-_speed) +1;
		if (delay >= _speed)
			delay = _speed -1; 
	}

	previousDelay = delay;

	//moves it on by one place
	delay += _speed;

	return delay ;
}

Stroke Tower::getSimulatorRoundsStartStroke()
{
	return hand;//to2do edit in profile. default to hand
}

void Tower::setUpListeningTest(int bellsAffected, bool allowFllowOn, bool lockHandAndBack, int maxNumber, double maxPercent)
{

	//to2do have this work like a quiz. i.e. the user checks the boxes of the bells that are wrong, and we then give a score
	resetAllStriking();
	for (int i=0;i<bellsAffected;i++)
	{
		srand(GetTickCount());

		int bell = (rand() * maxNumber) / RAND_MAX;
		StrikingErrors& error = getStrikingError(bell);	


		double valueHand = (double)(((double)rand() * maxPercent * 2) / RAND_MAX) - maxPercent;

		double valueBack = (double)(((double)rand() * maxPercent * 2) / RAND_MAX) - maxPercent;

		error.setLockHandAndBack(lockHandAndBack);

		if (lockHandAndBack)
		{
			valueBack = valueHand;
		}    

		error.setBackstrokeLateness(valueHand);
		error.setHandstrokeLateness(valueBack);
		
		if (allowFllowOn)
		{
			bool allowFollowOnBool = (((rand() * 2) / RAND_MAX) == 0);
			error.setFollowPreviousBellError(allowFollowOnBool);
		}       
	}

	towerManager()->fireUpdateTowerList();
}

BOOL Tower::getAddMethodShortcutsAsKeyboardInputs()
{
	return _addMethodShortcutsAsKeyboardInputs;
}

void Tower::setAddMethodShortcutsAsKeyboardInputs(BOOL addMethodShortcutsAsKeyboardInputs)
{
	_addMethodShortcutsAsKeyboardInputs = addMethodShortcutsAsKeyboardInputs;
}

BOOL Tower::getAddMethodNamesAsMicrophoneInputs()
{
	return _addMethodNamesAsMicrophoneInputs;
}   

void Tower::setAddMethodNamesAsMicrophoneInputs(BOOL addMethodNamesAsMicrophoneInputs)
{
	_addMethodNamesAsMicrophoneInputs = addMethodNamesAsMicrophoneInputs;
}   
