
#pragma once

#include "SaveableObject.h"


class RecordTower;

enum BellsGo
{
	bgt_wow,
	bgt_excellent,
	bgt_good,
	bgt_average,
	bgt_poor,
	bgt_veryPoor,
	bgt_unringable,
	bgt_none = 1000
};

typedef CArray<BellsGo, BellsGo> BellsGos;


enum BellsSound
{
	bst_sweet,
	bst_excellent,
	bst_good,
	bst_average,
	bst_poor,
	bst_veryPoor,
	bst_tinCans,
	bst_none = 1000
};

typedef CArray<BellsSound, BellsSound> BellsSounds;

class RecordGrab : public SaveableObject 
{
public:
	void dummyFunction(){};//todo remove

	RecordTower* getTower();
	int getSecondGrabIndex();
	void setSecondGrabIndex(int secondGrabIndex = -1);
	CString getDisplayName();
	void setDayOrder(int dayOrder);
	int getDayOrder();
	void bellSoundFillComboBox(CComboBox &comboBox);
	void bellGoFillComboBox(CComboBox& comboBox);
	BellsSound getBellsSound();
	BellsGo getBellsGo();	   
	static CString getBellsGoName(BellsGo type);
	static CString getBellsSoundName(BellsSound type);
	void setGrabIndex(int index);
	CString getGrabIndexAsString();
	int getGrabIndex();
	CString getBellsSoundAsString();
	CString getBellsGoAsString();
	COleDateTime getDateOle();
	CString getComments();
	CString getCommentAsSingleLine();
	CString getDateAsString(bool shortDate = true); //all the get...AsString() methods should be removed. Use the matrix code to format.
	void setTowerIndex(int towerIndex);
	int getTowerIndex();

	void setComments(CString comments);
	void setBellsGo(BellsGo bellsGo);
	void setBellsSound(BellsSound bellsSound);
	void setDateOle(COleDateTime dateOle);
	RecordGrab();

	virtual ~RecordGrab();

	void Serialize(CArchive& ar);

protected:

	void deserialize_v1(CArchive& ar);
	void deserialize_v2(CArchive& ar);


	COleDateTime _dateOle;
	BellsGo _bellsGo;
	BellsSound _bellsSound;
	CString _comments;
	int _towerIndex;
	int _dayOrder;
	
	//unserialised values
	int _index;
	int _secondGrabIndex;

};	 



