#pragma once

class RecordTower;
class RecordGrab;
class TowerDataStatic;
class TowerDataModification;

enum Night    //be carefull changing this as the extractor relys on the order.
{
	
	now_sun,
	now_sun_plus,
	now_sun_minus,

	now_mon,
	now_mon_plus,
	now_mon_minus,

	now_tue,
	now_tue_plus,
	now_tue_minus,

	now_wed,
	now_wed_plus,
	now_wed_minus,

	now_thu,
	now_thu_plus,
	now_thu_minus,

	now_fri,
	now_fri_plus,
	now_fri_minus,

	now_sat,
	now_sat_plus,
	now_sat_minus,

	now_none = 1000		//at the end to allow sorting to put nones at the ned

};

enum Country //todo remove this - to allow new countries to be added dynamically.
{	//******************** ONLY EVER ADD TO THE END OF THIS LIST ********************
	c_england,		//no entry
	c_wales,		//1
	c_scotland,		//A
	c_usa,	   		//U
	c_australia,	//D
	c_chanel_isl,	//5
	c_new_zealand,	//E
	c_ireland,		//B	   
	c_south_africa,	//C
	c_kenya,		//K
	c_pakistan,		//T
	c_zimbabwe,		//Z
	c_canada,		//F
	c_isle_of_man,	//4
	c_spain,		//P
	c_india,		//S

	c_none = 1000	//at the end to allow sorting to put nones at the ned

};

enum Note
{
	n_a_flat,
	n_a,
	n_a_sharp,

	n_b_flat,
	n_b,
	n_b_sharp,

	n_c_flat,
	n_c,
	n_c_sharp,

	n_d_flat,
	n_d,
	n_d_sharp,

	n_e_flat,
	n_e,
	n_e_sharp,

	n_f_flat,
	n_f,
	n_f_sharp,

	n_g_flat,
	n_g,
	n_g_sharp,


	n_none = 1000	//at the end to allow sorting to put nones at the ned

};


enum Direction
{
	d_n,
	d_nne,
	d_ne,
	d_ene,

	d_e,
	d_ese,
	d_se,
	d_sse,

	d_s,
	d_ssw,
	d_sw,
	d_wsw,

	d_w,
	d_wnw,
	d_nw,
	d_nnw,
	
	d_home = -1, 
	
	d_none = 1000 	//at the end to allow sorting to put nones at the ned
};

typedef CArray<Direction,Direction> Directions;
typedef CArray<Note, Note> Notes;
typedef CArray<Night, Night> Nights;
typedef CArray<Country, Country> Countrys;

typedef CArray<TowerDataStatic*, TowerDataStatic*> TowerDataStatics;
typedef CArray<TowerDataModification*, TowerDataModification*> TowerDataModifications;


typedef CArray<RecordTower*, RecordTower*> RecordTowers;
typedef CMap<int, int, RecordTower*, RecordTower*> RecordTowerMap;

typedef CArray<RecordGrab*, RecordGrab*> RecordGrabs;
typedef CMap<RecordGrab*, RecordGrab*, RecordGrab*, RecordGrab*> RecordGrabMap;



