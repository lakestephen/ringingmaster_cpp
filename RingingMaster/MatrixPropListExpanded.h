#pragma once

enum PropertyTypeMatrix
{
	ptm_userRoot,
	ptm_userCurrent,
	ptm_filterRoot,
	ptm_filterItem,
};

class MatrixPropListExpanded
{
public:
	MatrixPropListExpanded(void);
	~MatrixPropListExpanded(void);

	PropertyTypeMatrix _focusType; 
	int _scrollPos;
	int _columnPos;

	BOOL _users;
	BOOL _filters;

	BOOL _loaded;
};
