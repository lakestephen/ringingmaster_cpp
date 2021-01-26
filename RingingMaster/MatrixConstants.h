#pragma once

#include "SaveableObjects.h"

class SaveableObject;
class MatrixItemBase;
class MatrixFilterDescriptionItem;
class MatrixRelation;
class MatrixFilterSort;
class MatrixGridCtrl;


enum MatrixGridImages
{
	mgi_none = -1, 
	mgi_blue_tick = 0,
	mgi_pink_flag,
	mgi_head_home,
	mgi_head_tick,
	mgi_green_tick
};


enum MatrixEntityToken
{
	met_tower = 0,
	met_grab
};

#define MatrixEntityTokenMax met_tower

//THESE VALUES MUST NOT CHANGE AFTER RELEASE. LOTS OF THINGS DEPEND ON THEM. 
//ALSO HO UP IN 100's TO ENSURE THE MatrixColumnImageHash WORKS.
enum MatrixItemToken
{
	mit_tower_home = 1,
	mit_tower_grabbed = 2,
	mit_tower_name = 3,
	mit_tower_county = 4,
	mit_tower_dedication = 5,
	mit_tower_number = 6,
	mit_tower_gridref = 7,
	mit_tower_postcode = 8,
	mit_tower_groundfloor = 9,
	mit_tower_weight = 10,
	mit_tower_note = 11,
	mit_tower_night = 12,
	mit_tower_unringable = 13,
	mit_tower_webaddress = 14,
	mit_tower_country = 15,
	mit_tower_distanceFromHome = 16,//todo	
	mit_tower_direction = 17,//todo	
	mit_tower_mapno = 18,	//todo	
	mit_tower_grabCount = 19,	
	mit_tower_performanceCount = 20,//todo	
	mit_tower_comments = 21,
	
	//grabs
	mit_grab_number = 50,  
	mit_grab_date = 51,	 
	mit_grab_bellsgo = 52,	 
	mit_grab_bellssound = 53,  
	mit_grab_comments = 54,	 
};

enum TranslatorFunctionItem
{
	tfi_doNothing,
};

enum MatrixFilterSectionToken
{
	mfst_user,
	mfst_stock,
};

enum MatrixCtrlBarCtrlStyle
{
	mcbcs_none,
	mcbcs_local,
	//mcbcs_mianFrame,
};


typedef SaveableObject* MATRIX_ENTITY_TRANSLATOR(SaveableObject* entity);

typedef CArray<MatrixItemBase*, MatrixItemBase*> MatrixItemBases;
typedef CArray<MatrixFilterDescriptionItem*, MatrixFilterDescriptionItem*> MatrixFilterDescriptionItems;
typedef CArray<MatrixItemToken, MatrixItemToken> MatrixItemTokens;
typedef CArray<MatrixRelation*, MatrixRelation*> MatrixRelations;
typedef CArray<MatrixFilterSort*, MatrixFilterSort*>MatrixFilterSorts;
typedef CArray<MatrixGridCtrl*, MatrixGridCtrl*>MatrixGridCtrls;



