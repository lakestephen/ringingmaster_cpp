#include "StdAfx.h"
#include "ProofAnalysisEngine.h"
#include "ProofInput.h"
#include "Method.h"
#include "Row.h"
#include "FalseRow.h"
#include "MethodAnalysis.h"



ProofAnalysisEngine::ProofAnalysisEngine(const ProofInput& proofInput, const Method& method, MethodAnalysis& methodAnalysis): 
_met(method),
_anal(methodAnalysis),
_ip(proofInput),
_hasActuallyTerminated(false),
_hasRequestedTerminate(false)
{
	
}

ProofAnalysisEngine::~ProofAnalysisEngine(void)
{
 }

void ProofAnalysisEngine::requestEarlyTerminate()
{
	_hasRequestedTerminate = true;
}

bool ProofAnalysisEngine::hasActuallyTerminated()
{
	return _hasActuallyTerminated;
}

void ProofAnalysisEngine::doCalculate()
{

	calculate();

	_hasActuallyTerminated = true;
}

void ProofAnalysisEngine::calculate()
{
	//calculate false rows 
	if (_hasRequestedTerminate)
		return;
	
	int count = 0;
	Row** rows = getSortedRows(count);

	if (rows != NULL && count > 0)
	{
		if (!_hasRequestedTerminate)
			checkMusic(rows, count);

		//do whatever we have to 
		if (!_hasRequestedTerminate)
		{
			if (_ip.getBlockCount() <= 1)
				checkFalsenessSingleBlock(rows, count);
			else
				checkFalsenessMultiBlock(rows, count);
		}

	  delete [] rows;
	}

	//setup the display stuff
	if (!_hasRequestedTerminate)
		setupDisplay();

	_hasActuallyTerminated = true;
}


Row** ProofAnalysisEngine::getSortedRows(int& count)
{
	//set up rows
	count = _met.getRowCount();
	Row** rows = new Row*[count]; 
	
	//exclude the last row if identical to the first row
	if ((*_met.getFirstRow()) == (*_met.getLastRow()))
		count--;

	//get a set of row pointers into a buff. 
	if (!_hasRequestedTerminate) 
		for (int i=0;i<count;i++)
			rows[i] = _met.getRow(i);

	if (!_hasRequestedTerminate) 
		qsort((void*)rows, 
			  (size_t)count, 
			  sizeof(Row *), 
			  compareRows );

	return rows;
}

int ProofAnalysisEngine::compareRows(const void *arg1, const void *arg2)
{
   	int diff;
	for (int i=0;i<(*(Row**)arg1)->_number;i++)
	{
		diff = (*(Row**)arg1)->getBellInPlace(i)-(*(Row**)arg2)->getBellInPlace(i);
		if (diff != 0)
			return diff;
	}
	return 0;
}

void ProofAnalysisEngine::checkMusic( Row **rows, int size)
{
	if (!_hasRequestedTerminate) 
		setupMusic();

	//check all the rows.
	if (!_hasRequestedTerminate) 
		for (int i=0;i<size;i++)
			checkRowForMusic(rows[i]);


	//sort the music lists into real row order
	if (!_hasRequestedTerminate) 
	{
		for (int i=0;i<_anal._musicResults.GetSize();i++)
		{
			qsort((void*)_anal._musicResults.GetAt(i)->GetData(), 
				(size_t)_anal._musicResults.GetAt(i)->GetSize(), 
				sizeof(MusicResult *), 
				RowPosition::compare);
		}
	}
}

void ProofAnalysisEngine::checkFalsenessSingleBlock(Row** sortedRows, int size)
{
	FalseRow* curArr = NULL;
	
	if (!_hasRequestedTerminate)
	{
		for (int i=0;i<size-1;i++)
		{
			if (*sortedRows[i]== *sortedRows[i+1])
			{
				if (!curArr)
				{//we are at the start of a sequence of false rows
					curArr = new FalseRow(sortedRows[i]);
					_anal._falseRows.Add(curArr);
					RowPosition* rowPosition = new RowPosition;
					rowPosition->setRow(&_met, sortedRows[i]->getRowIndex());
					curArr->Add(rowPosition);
					(sortedRows[i])->setFalse();
					_anal._falseRowCount++;
				}

				RowPosition* rowPosition = new RowPosition;
				rowPosition->setRow(&_met,(sortedRows[i+1])->getRowIndex());
				curArr->Add(rowPosition);
				(sortedRows[i+1])->setFalse();
				_anal._falseRowCount++;
			}
			else
			{	//we have come to the end of a sequence of false rows
				curArr=NULL;
			}
		}
	}


	if (!_hasRequestedTerminate) 
	{	
		//now sort each falseness RowPosition list
		for (int i=0;i<_anal._falseRows.GetSize();i++)
		{
			qsort((void*)_anal._falseRows.GetAt(i)->GetData(), 
				  (size_t)_anal._falseRows.GetAt(i)->GetSize(), 
				  sizeof(MultiBlockFalseRow *), 
				  RowPosition::compare);
		}
	}
}

void ProofAnalysisEngine::checkFalsenessMultiBlock(Row** sortedRows, int size)
{	
	_anal._requestedBlockCount = _ip.getBlockCount();

	MultiBlockFalseRows mbFalseRows;

	// in multi block we just record all common blocks
	// we also include the blocks with only one change
	MultiBlockFalseRow* curArr = new MultiBlockFalseRow(sortedRows[0]);	
	MultiBlockFalseRowPair* multiBlockFalseRowPair = new MultiBlockFalseRowPair(0);
	multiBlockFalseRowPair->setRow(&_met, (sortedRows[0])->getRowIndex());
	curArr->Add(multiBlockFalseRowPair);
	mbFalseRows.Add(curArr);

	if (!_hasRequestedTerminate) 
	{	
		for (int i=0;i<size-1;i++)
		{
		
			if (*sortedRows[i] != *sortedRows[i+1])
			{
				curArr = new MultiBlockFalseRow(sortedRows[i+1]);

				MultiBlockFalseRowPair* multiBlockFalseRowPair = new MultiBlockFalseRowPair(i+1);
				multiBlockFalseRowPair->setRow(&_met, (sortedRows[i+1])->getRowIndex());

				curArr->Add(multiBlockFalseRowPair);
				mbFalseRows.Add(curArr);
			}
			else
			{	//must be more than one of this change
				MultiBlockFalseRowPair* multiBlockFalseRowPair = new MultiBlockFalseRowPair(i+1);
				multiBlockFalseRowPair->setRow(&_met, (sortedRows[i+1])->getRowIndex());

				curArr->Add(multiBlockFalseRowPair);
				(sortedRows[i])->setCommon();
				(sortedRows[i+1])->setCommon();
			}
		}							
	}

	int biggestCommonArray = 0;
	int smallestCommonArray = INT_MAX; //silly large number
	int extentSize = 1;

	//now sort each common RowPosition list
	if (!_hasRequestedTerminate) 
	{	
		for (int i=0;i<mbFalseRows.GetSize();i++)
		{
			curArr = mbFalseRows.GetAt(i);
			qsort((void*)curArr->GetData(), 
				  (size_t)curArr->GetSize(), 
				  sizeof(MultiBlockFalseRow *), 
				  RowPosition::compare);

			if (curArr->GetSize() > biggestCommonArray)
				biggestCommonArray = curArr->GetSize();
			if (curArr->GetSize() < smallestCommonArray)
				smallestCommonArray = curArr->GetSize();
		}
									
		//get the number of rows in an extent
		for (int j=2;j<=_met.getNumber();j++)
			extentSize *= j;

		if (extentSize != mbFalseRows.GetSize())
			smallestCommonArray = 0;
	}

	if (!_hasRequestedTerminate)
	{
		//note the blocks - this code is to go into the analyss screen
		_anal._completeBlocks = smallestCommonArray;
		_anal._partialBlocks  = biggestCommonArray - smallestCommonArray;

		//now go through all the false rows and check for falseness against the 
		//smallest number of complete blocks.
		int falseChanges = 0;

		for (int i=0;i<=biggestCommonArray;i++)
			_anal._blockRowCounts.Add(0);

		_anal._blockRowCounts[0] = extentSize - mbFalseRows.GetSize();

		for (int i=0;i<mbFalseRows.GetSize();i++)
		{
			curArr = mbFalseRows.GetAt(i);
			
			if ((curArr->GetSize() != smallestCommonArray) &&
				(curArr->GetSize() != smallestCommonArray+1))
			{
				for(int j=0;j<curArr->GetSize();j++)
				{
					(sortedRows[curArr->GetAt(j)->getSortedIndex()])->setFalse();			
					_anal._falseRowCount++;
				}
				falseChanges++;
			}
			else
				curArr->setMultiBlockTrue();

			_anal._blockRowCounts[curArr->GetSize()]++;
		}
	}

	//now create the actual false row list from the special Multi one
	//at this stage we can exclude ones of size one
	_anal._commonRowCount = _met.getRowCount();

	for (int i=0;i<mbFalseRows.GetSize();i++)
	{
		MultiBlockFalseRow* mbFalseRow = mbFalseRows.GetAt(i);
		if (mbFalseRow->GetSize() > 1)
			_anal._falseRows.Add(new FalseRow(*mbFalseRow));
		else
			_anal._commonRowCount--;
		delete mbFalseRow;
	}			 
}

void ProofAnalysisEngine::setupMusic()
{
	//create the blank shells for the results
	CString str;
	int i, j;
	MusicResult* musicResult;

	for (i=0;i<_ip.getMusicRules().GetSize();i++)
	{
		MusicRule* musicRule = _ip.getMusicRules().GetAt(i);
		if ((musicRule->getType() == MRT_CRU)&&(_met.getNumber() >=8))
		{

			CStringArray strArr;
			strArr.Add("????45");
			strArr.Add("????46");
			strArr.Add("????54");
			strArr.Add("????56");
			strArr.Add("????64");
			strArr.Add("????65");

			for (int j=0;j<strArr.GetSize();j++)
			{
				//get the string 
				CString str = strArr.GetAt(j);
				
				//fill upto the number of bells
				for (int i=6;i<_met.getNumber();i++)
				{
					str += GlobalFunctions::bellNumbersToChar(i+1);
				}

				CString displayStr = str;
				while (displayStr.Replace("?", ""));

				musicResult = new MusicResult(*musicRule, "CRU ("+ displayStr + ")");
				musicResult->setRule(str);
				_anal._musicResults.Add(musicResult);
			}
		}
		else if (musicRule->getType() == MRT_MUSICAL_CHANGES)
		{
			//reverse rounds
			str.Empty();
			for (j=_met.getNumber(); j>0;j--)
				str += GlobalFunctions::bellNumbersToChar(j);
			
			musicResult = new MusicResult(*musicRule, "Reverse rounds");
			musicResult->setRule(str);
			_anal._musicResults.Add(musicResult);


			//queens
			str.Empty();
			for (j=0; j<_met.getNumber();j+=2)
				str += GlobalFunctions::bellNumbersToChar(j+1);
			for (j=0; j<_met.getNumber();j+=2)
				str += GlobalFunctions::bellNumbersToChar(j+2);

			musicResult = new MusicResult(*musicRule, "Queens");
			musicResult->setRule(str);
			_anal._musicResults.Add(musicResult);


			//kings
			if (GlobalFunctions::isEven(_met.getNumber()))
			{
				str.Empty();
				for (j=0; j<_met.getNumber();j+=2)
					str += GlobalFunctions::bellNumbersToChar(_met.getNumber() - j -1);
				for (j=0; j<_met.getNumber();j+=2)
					str += GlobalFunctions::bellNumbersToChar(j+2);
				musicResult = new MusicResult(*musicRule,  "Kings");
				musicResult->setRule(str);
				_anal._musicResults.Add(musicResult);
			}


			//tittums
			str.Empty();
			for (j=0; j<(_met.getNumber()+1)/2;j++) //the +1 allows this to work with odd numbers of bells
			{
				str += GlobalFunctions::bellNumbersToChar(j+1);
				str += GlobalFunctions::bellNumbersToChar(j+1 + ((_met.getNumber()+1)/2));
			}
			musicResult = new MusicResult(*musicRule, "Tittums");
			musicResult->setRule(str);
			_anal._musicResults.Add(musicResult);

			//whittingtons on 6 or 8 
			if (_met.getNumber() == 6)
			{
				str = "125346";
				musicResult = new MusicResult(*musicRule, "Whittingtons");
				musicResult->setRule(str);
				_anal._musicResults.Add(musicResult);
			}
			else if (_met.getNumber() == 8)
			{
				str = "12753468";
				musicResult = new MusicResult(*musicRule, "Whittingtons");
				musicResult->setRule(str);
				_anal._musicResults.Add(musicResult);
			}
				
		}
		else if (musicRule->getType() == MRT_USER)
		{
			musicResult = new MusicResult(*musicRule);
			_anal._musicResults.Add(musicResult);
		}
	}	 
}

void ProofAnalysisEngine::checkRowForMusic(Row * row)
{
	for (int i=0;i<_anal._musicResults.GetSize();i++)
	{
		MusicResult* musicResult  = _anal._musicResults.GetAt(i);
		if (*musicResult == *row)
		{
			MusicResultRowPosition* musicResultRowPosition = new MusicResultRowPosition(row->getAsText());
			musicResultRowPosition->setRow(&_met, row->getRowIndex());
			musicResult->Add(musicResultRowPosition);
			row->setMusic(musicResult);
//			TRACE("RowFound: %s \r\n", row->getAsText());
		}
	}
}


void ProofAnalysisEngine::setupDisplay()
{
	int i,j;
	//bell to follow
	_anal._bellToFollow = _ip.getCallFrom();
	
/*	todo //name
	if (_proofInput->getName().IsEmpty())
		_proofInput->setName(getCurNot()->getName(true));

		//notation displayed in the title
	_anal._notationDisplayString = getCurNot()->getNotation(true) ;
	if (!getCurNot()->getLeadEnd().IsEmpty())
		_notationDisplayString += "  le"  + getCurNot()->getLeadEnd();
  

	
	//notation displayed down the left of the method
	if (!isSpliced()) //not spliced
	{
		CString notStr = getCurNot()->getNotation(false, SEP_VERBOSE);
		CString temp;

		for(i=0, j=0;i<notStr.GetLength();j++)
		{
			if ((j>=notStr.GetLength())||(notStr[j] == '.'))
			{
				temp = notStr.Mid(i, j-i);
				temp.Replace('-', GlobalFunctions::bellNumbersToChar(ALLCHANGE));
				_notationStrings.Add(temp);
				i=j;
				i++;
			}											
		}

		_notationSym = false;							
		if (getCurNot()->isSymmetric())
		{
			_notationSym = true;
			temp = getCurNot()->getLeadEnd();
			temp.Replace('-', GlobalFunctions::bellNumbersToChar(ALLCHANGE));
			_notationStrings.Add(temp);
		}
	}

	distributePendingComments();

	//tidy up the analysis
	completeCourseHeadMatrix();
	completeLeadEndMatrix();
	createSpliceTransitions();
			   */
}
