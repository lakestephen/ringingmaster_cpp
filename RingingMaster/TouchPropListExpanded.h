#pragma once


enum PropertyTypeTouch     
{
	//cant use 0 as the prop list uses 0 as its default data item.
	ptt_setupRoot=1,
	ptt_setupAdvancedRoot,
	ptt_title,
	ptt_author,
	ptt_numOfBells,
	ptt_callFrom,
	ptt_firstSimulatedBell,
	ptt_coverBellCount,
	ptt_method,
	ptt_callType,
	ptt_plainLeadToken,
	ptt_wrapCalls,
	ptt_checkingType,
	ptt_blockCount,
	
	ptt_startRoot,
	ptt_startWithChange,
	ptt_startAtRow,
	ptt_startAtStroke,
	ptt_startNotation,

	ptt_endRoot, 
	ptt_endWithChange,
	ptt_endRowLimit,
	ptt_endLeadLimit,
	ptt_endPartLimit,
	ptt_endCircleLimit,

	ptt_methodRoot,

	ptt_definitionRoot,

	ptt_musicRoot,

	ptt_styleRoot, 
	ptt_styleTouchFontTitle,
	ptt_styleTouchFontAuthor,
	ptt_styleTouchFontMain,
	ptt_styleTouchFontNotes,
  	ptt_styleBlueLine,
	
	ptt_notesRoot,
	ptt_notesShow, 
	ptt_notesEdit, 

	//carefuly note how this works!!!!!!
	
	ptt_styleTouchColorItem = TypeGapSize, 

	ptt_methodItem = ptt_styleTouchColorItem + TypeGapSize,

	ptt_definitionItem = ptt_methodItem + TypeGapSize,

	ptt_musicItem = ptt_definitionItem + TypeGapSize,


};

class TouchPropListExpanded
{
public:
	TouchPropListExpanded(void);
	~TouchPropListExpanded(void);

	PropertyTypeTouch  _focusType; //not serialised
	int _scrollPos; //not serialised
	int _columnPos; //not serialised

	BOOL _mainSetup;
	BOOL _mainSetupAdvanced;
	BOOL _mainStart;
	BOOL _mainEnd;
	BOOL _mainMethod;
	BOOL _mainDefinition;
	BOOL _mainMusic;
	BOOL _mainStyle;
	BOOL _mainNotes;

	BOOL _loaded;
};
