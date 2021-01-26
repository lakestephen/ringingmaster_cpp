// Notation.cpp: implementation of the Notation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notation.h"
#include "Lead.h"
#include "Method.h"
#include "NotationLite.h"
#include "AllTheWorkBell.h"
#include "CribDoc.h"
#include "ProofManager.h"
#include "ProofInput.h"
#include "Row.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Notation::Notation() :
NotationPersist(),
_spliceRowCount(0),
_allTheWork(NULL)
{

}

Notation::Notation(const Notation & notation) :
NotationPersist(notation),
_spliceRowCount(0),
_allTheWork(NULL)
{
    prepare();
/*	
	//setup calls
    calculateLeadHead();
    setDefaultCalls();
    //setup call pos lead
    setDefaultCallPosLead();
    //setup call pos method
    setDefaultCallPosMethod();
    //setup call splice position
    setDefaultSplicePosition();
    //setup call lead lines
    setDefaultLeadLines();
    //setup call course head
    setDefaultCourseHeadPos();*/

}

Notation::~Notation()
{
    for (int i=0;i<_plainLead.GetSize();i++)
        delete _plainLead.GetAt(i);
    _plainLead.RemoveAll();

	delete _allTheWork;
}

void Notation::operator = (const Notation & notation)
{
	NotationPersist::operator =(notation);
}

void Notation::operator =(const NotationLite& lite)
{
    NotationPersist::operator = (lite);
    //setup calls
    calculateLeadHead();
    setDefaultCalls();
    //setup call pos lead
    setDefaultCallPosLead();
    //setup call pos method
    setDefaultCallPosMethod();
    //setup call splice position
    setDefaultSplicePosition();
    //setup call lead lines
    setDefaultLeadLines();
    //setup call course head
    setDefaultCourseHeadPos();

}

CString Notation::getName(bool showNumber, bool showSpliceLetter) const
{
    return _name + 
		   ((showNumber)?(" " + GlobalFunctions::numberName(_number)):"") +
		   ((showSpliceLetter&&getSpliceLetter() != "")?(" (" + getSpliceLetter() + ")"):"");
}

int Notation::getNumber() const
{
    return _number;
}

BOOL Notation::isSymmetric()
{
    return _symmetric;
}

void Notation::create(BOOL call,
                      CString name,
                      int number,
                      bool symmetric,
                      CString notation,
                      CString leadEnd,
                      CString spliceLetter,
                      bool bInit)
{
    init(bInit);
    _call = call;

    //check the number is within limits
    if ((number<3)||(number>MAXBELLS))  return ;
    _number = number;

    //record name
    setName(name);

    //record symetry
    _symmetric = symmetric;

    //check notation
    _notation = validateNotation(notation);

    //check leadEnd
    if (_symmetric)
        _leadEnd = validateNotation(leadEnd);
    else
        _leadEnd.Empty();

    //record name
    setSpliceLetter(spliceLetter);

    //setup calls
    calculateLeadHead();
    setDefaultCalls();

    //setup call pos lead
    setDefaultCallPosLead();

    //setup call pos method
    setDefaultCallPosMethod();

    //setup call splice
    setDefaultSplicePosition();

    //setup call pos method
    setDefaultLeadLines();

    //setup call course head
    setDefaultCourseHeadPos();


    return;
}

CString Notation::getNotation(bool showSymmetry, SeperationStyle seperationStyle) const
{
    if (seperationStyle == SEP_NONE)
        seperationStyle = globalDataManager()->getSeperationStyle();

    return ((showSymmetry)?((_symmetric )?"& ":"+ "):"") +
             getDisplayString(_notation, seperationStyle);
}

CString Notation::getLeadEnd()
{
    return getDisplayString(_leadEnd, globalDataManager()->getSeperationStyle());
}

CString Notation::getDisplayString(const CString &str, SeperationStyle seperationStyle) const
{
    CString displayStr = str;
    //sort seperators
    if (seperationStyle == SEP_COMPACT)
    {
        while (displayStr.Replace("-.","-"));
        while (displayStr.Replace(".-","-"));
    }

    while (displayStr.Replace('-', globalDataManager()->getAllChangeChar()));

    return displayStr;
}

int Notation::getChangesPerPlainLead() const
{
    return _plainLead.GetSize();
}

CString Notation::validateNotation(CString not)
{

    //force the global chars.
    char allChange = globalDataManager()->getAllChangeChar();
    SeperationStyle seperatioStyle = globalDataManager()->getSeperationStyle();
    globalDataManager()->setSeperationStyle(SEP_VERBOSE);
    globalDataManager()->setAllChangeChar('-');

    //tidy up the notation
    not.TrimLeft();
    not.TrimRight();
    not.MakeUpper();

    while (not.Replace("  ", " "));
    while (not.Replace(". ", "."));
    while (not.Replace(" .", "."));
    while (not.Replace(" " , "."));
    while (not.Replace("x" , "-"));
    while (not.Replace("X" , "-"));
    while (not.Replace("--" , "-"));

    int pos = 0;

    //add dots to x boundarys
    if (not.GetLength() >1)
    {
        while ((pos = not.Find('-', pos)) != -1)
        {
            if (pos == 0)
            {
                if (not[pos+1] != '.')
                {
                    not.Insert(1,'.');
                }
            }
            else if (pos == not.GetLength()-1)
            {
                if(not[pos-1] != '.')
                {
                    not.Insert(not.GetLength()-1, '.');
                    pos ++;
                }
            }
            else
            {
                if (not[pos-1] != '.')
                {
                    not.Insert(pos, '.');
                    pos ++;
                }
                if (not[pos+1] != '.')
                {
                    not.Insert(pos+1, '.');
                }
            }
        pos++;
        }
    }

    while (not.Replace("-.-" , "-"));

    //chop up and check
    CString str;
    for(int i=0, j=0;i<not.GetLength();j++)
    {
        if ((j>=not.GetLength())||(not[j] == '.'))
        {
            str += validateRow(not.Mid(i, j-i));
            i=j;
            i++;
            str += ".";
        }
    }

    while (str.Replace("..", "."));

    while ((str.GetLength() > 0)&&(str[0] == '.'))
        str = str.Right(str.GetLength()-1);

    while ((str.GetLength() > 0)&&(str[str.GetLength()-1] == '.'))
        str = str.Left(str.GetLength()-1);

    //swap the global chars back
    globalDataManager()->setSeperationStyle(seperatioStyle);
    globalDataManager()->setAllChangeChar(allChange);

    return str;
}


//this should ONLY be called from validateNotation
CString Notation::validateRow(CString row)
{
    Ints data;
    int i, iSwap, prevPlace;
    BOOL change;
    CString out;

    //convert to bell numbers in an array
    row.Replace("." , "");
    for (i=0;i<row.GetLength();i++)
    {
        int bellNum = GlobalFunctions::charToBellNumbers(row[i]);
        if (bellNum != BADDATA)
            data.Add(bellNum);
    }

    //check for ALLCHANGE
    for (i=0;i<data.GetSize();i++)
    {
        if (data[i] == ALLCHANGE)
        {
            if (GlobalFunctions::isEven(_number))
            {
                return CString("-");
            }
            else
            {
                data.RemoveAt(i);
            }
        }
    }

    //sort
    do
    {
        change = FALSE;
        for (i = 0;i<data.GetUpperBound();i++)
        {
            if (data[i] == data[i+1])
            {
                data.RemoveAt(i);
                i--;
            }
            else if (data[i]  > data[i+1])
            {
                iSwap = data[i];
                data[i] = data[i+1];
                data[i+1] = iSwap;
                change = TRUE;
            }
        }
    }while(change);

    //check for places made higher than number of bells
    for (i=0;i<data.GetSize();i++)
    {
        if ((data[i] > _number)&&(data[i] <= MAXBELLS))
        {
            data.RemoveAt(i);
            i--;
        }
    }

    //check positions can be made
    for (prevPlace = 0,i=0;i<data.GetSize();i++)
    {
        if (GlobalFunctions::isEven(data[i] - prevPlace))
        {
            if ((i==0)&&(data[i] != 1))
                data.InsertAt(0,1);
            else
                data.RemoveAt(i);

            prevPlace = 0;
            i = -1;
        }
        else
        {
            prevPlace = data[i];
        }

        if ((i == data.GetUpperBound()  )&& //last element in data
            (data.GetSize() < _number   )&& //not fill - i.e. 8 places in an 8 bell method
            (!GlobalFunctions::isEven(_number - prevPlace))) //mismatch
        {
            data.Add(_number);
            prevPlace = 0;
            i = -1;
        }
    };

    //recreate the string
    for (i=0;i<data.GetSize();i++)
    {
        out += GlobalFunctions::bellNumbersToChar(data[i]);
    }

    return out;

}

//at this point we assume good checked data
void Notation::prepare()
{
    int i,j;

    init(false);

    for(i=0, j=0;i<_notation.GetLength();j++)
    {
        if ((j>=_notation.GetLength())||(_notation[j] == '.'))
        {
            Ints * rowArr = getRowArray(_notation.Mid(i, j-i));
            if (rowArr)
                _plainLead.Add(rowArr);
            i=j;
            i++;
        }
    }

    if (_symmetric)
    {
        int halfway = _plainLead.GetUpperBound()-1;
        for (i=halfway;i>=0;i--)
        {
            _plainLead.Add(copyRowArray(_plainLead[i]));
        }
    }

    if (!_leadEnd.IsEmpty())
    {
        for(int i=0, j=0;i<_leadEnd.GetLength();j++)
        {
            if ((j>=_leadEnd.GetLength())||(_leadEnd[j] == '.'))
            {
                Ints * rowArr = getRowArray(_leadEnd.Mid(i, j-i));
                if (rowArr)
                    _plainLead.Add(rowArr);
                i=j;
                i++;
            }
        }
    }
    else
    {
        if ((_plainLead.GetSize() > 1)&&(_symmetric))
        {
            delete _plainLead.GetAt(_plainLead.GetUpperBound());
            _plainLead.RemoveAt(_plainLead.GetUpperBound());
        }
    }
}

Ints * Notation::getRowArray(CString row)
{
    if (row.IsEmpty()) return NULL;

    Ints *rowArr = new Ints();

    row.Replace("." , "");

    for (int i=0;i<row.GetLength();i++)
    {
        rowArr->Add(GlobalFunctions::charToBellNumbers(row[i]));
    }
    return rowArr;
}

Ints * Notation::copyRowArray(Ints *rowArr)
{
    Ints *newRowArr = new Ints();

    for (int i=0;i<rowArr->GetSize();i++)
    {
        newRowArr->Add(rowArr->GetAt(i));
    }

    return newRowArr;
}

Ints * Notation::getRow(int row) const
{
    if ((row < 0)||(row>_plainLead.GetUpperBound()))
        return NULL;

    return _plainLead.GetAt(row);
}

CString Notation::getRowStr(int row)
{
    if (row > _plainLead.GetUpperBound()) return "";
    Ints * arr = getRow(row);

    if (!arr) return "";

    CString str("");
    CString rowStr("");

    for (int i=0;i<arr->GetSize();i++)
    {
        rowStr += GlobalFunctions::bellNumbersToChar(arr->GetAt(i));
    }

    return rowStr;
}

void Notation::init(bool deep)
{
    for (int i=0;i<_plainLead.GetSize();i++)
    {
        delete _plainLead.GetAt(i);
    }
    _plainLead.RemoveAll();

    if (deep)
        NotationPersist::init();
}

bool Notation::checkForSymmetry(bool takeAction)
{
    if (_symmetric) return false;

    //convert to an array of strings

    CStringArray strArr;

    for(int i=0, j=0;i<_notation.GetLength();j++)
    {
        if ((j>=_notation.GetLength())||(_notation[j] == '.'))
        {
            strArr.Add(_notation.Mid(i, j-i));
            i=j;
            i++;
        }
    }

    CString notation;

    if (GlobalFunctions::isEven(strArr.GetSize()))
    {
        //must be symmetric with a lead end
        for (int i=0;i<(strArr.GetSize()/2)-1;i++)
        {
            if (strArr[i] != strArr[strArr.GetSize() - i - 2]) return false;

            notation += strArr[i] + ".";
        }

        notation += strArr[(strArr.GetSize()/2)-1] ;

        //we are symmetric,
        if  (takeAction)
        {
            _symmetric = true;

            //check notation
            _notation = validateNotation(notation);

            //check leadEnd
            _leadEnd = validateNotation(strArr[strArr.GetUpperBound()]);
        }
    }
    else
    {
        //must be symmetric without a lead end
        for (int i=0;i<((strArr.GetSize()-1)/2);i++)
        {
            if (strArr[i] != strArr[strArr.GetSize() - i - 1]) return false;

            notation += strArr[i] + ".";
        }

        notation += strArr[((strArr.GetSize()-1)/2)] ;

        //we are symmetric,
        if  (takeAction)
        {
            _symmetric = true;

            //check notation
            _notation = validateNotation(notation);

            //check leadEnd
            _leadEnd = "";
        }
    }

    return true;
}

bool Notation::contains(int row, int place)
{
    Ints * intArr = getRow(row);

    for (int i=0;i<intArr->GetSize();i++)
    {
        if (intArr->GetAt(i) == place)
            return true;
    }
    return false;
}

void Notation::setName(CString name)
{
    _name = GlobalFunctions::capitalString(name);
}


CString Notation::getSpliceLetter() const
{
    return _spliceLetter;
}

void Notation::setSpliceLetter(CString spliceLetter)
{
    _spliceLetter = spliceLetter;
}

void Notation::convertToAsym()
{
    if (!_symmetric) return;

    CString asymStr = _notation;
    bool pastHalfLead = false;

    for(int i=_notation.GetLength()-1;i>=0;i--)
    {
        if (_notation[i] == '.')
            pastHalfLead = true;

        if (pastHalfLead)
            asymStr += _notation[i];
    }

    asymStr += _leadEnd;

    _symmetric = false;

    //check notation
    _notation = validateNotation(asymStr);
}

void Notation::calculateLeadHead()
{
    if (_call == TRUE) return;

    //create a plain lead
	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
    proofInput.setLeadLimit(1);
	Method met;
	proofManager()->requestProof(proofInput, met);

    prepare();

	if (met.getLeadCount() > 0)
    {
		const Lead& lead = met.getLead(0);
        _leadHead = lead.GetAt(lead.GetUpperBound())->getAsText();
        CString leadEnd("");
        Ints* row = NULL;
        bool invert = false;

        if (getLeadEnd().IsEmpty())
        {
            row = getRow(0);
            if (_symmetric)
                invert = true;
        }
        else
        {
            row = getRow(getChangesPerPlainLead()-1);
        }

        if (row)
            for (int i=0;i<row->GetSize();i++)
                leadEnd += GlobalFunctions::bellNumbersToChar(row->GetAt(i));

        _leadHead = GlobalFunctions::getLeadHeadCode(_leadHead, getNumber(), leadEnd, invert);
    }
    else
    {
        _leadHead = "";
    }
    setDefaultCalls();
}

void Notation::Serialize(CArchive &ar)
{
    int version = 1;

    if (ar.IsStoring())
    {
        ar << version;

        NotationPersist::Serialize(ar);
    }
    else //loading
    {
        init();

        ar >> version;

        NotationPersist::Serialize(ar);
        //setup calls
        calculateLeadHead();
        setDefaultCalls();
        //setup call pos lead
        setDefaultCallPosLead();
        //setup call pos method
        setDefaultCallPosMethod();
        //setup call splice 
        setDefaultSplicePosition();
        //setup call lead lines
        setDefaultLeadLines();
       //setup call course head
        setDefaultCourseHeadPos();

    }
}


/*return types
    CE_OK,
    CE_OK_DEFAULT,
    CE_DUPLICATE_NAME,
    CE_DUPLICATE_NOTATION
    CE_INVALID_NAME
    CE_INVALID_NOTATION
*/
CallError Notation::addCall(CString nameShorthand, CString nameLonghand, CString notation)
{
    if (nameShorthand.IsEmpty()) return CE_INVALID_SHORTHAND_NAME;
    if (nameLonghand.IsEmpty()) return CE_INVALID_LONGHAND_NAME;
    //check valid call;
    Notation not;
    not.create(true, "",_number, false, notation);
    notation = not.getNotation();
    if (notation.GetLength() == 0) return CE_INVALID_NOTATION;

    //check unique name
    for(int i=0;i<_calls.GetSize();i++)
    {
        if(_calls[i]->_nameShorthand == nameShorthand) return CE_DUPLICATE_SHORTHAND_NAME;
        if(_calls[i]->_nameLonghand == nameShorthand) return CE_DUPLICATE_SHORTHAND_NAME;
        if(_calls[i]->_nameShorthand == nameLonghand) return CE_DUPLICATE_LONGHAND_NAME;
        if(_calls[i]->_nameLonghand == nameLonghand) return CE_DUPLICATE_LONGHAND_NAME;
        if(_calls[i]->_notation == notation) return CE_DUPLICATE_NOTATION;
    }

    //create & add
    NotationCall * call = new NotationCall;

    call->_nameShorthand = nameShorthand;
    call->_nameLonghand = nameLonghand;
    call->_notation = notation;

    _calls.Add(call);

    sortCalls();   

	setDefaultCallPosMethod();

    return (_defaultCalls)?	CE_OK_DEFAULT:CE_OK;
}

CString Notation::getAddCallErrorText(CallError err)
{
    switch (err)
    {
    case CE_OK:
        return "Call Added";
    case CE_OK_DEFAULT:
        return "Error, the call will be lost - Contact stephen@ringingmaster.co.uk";
    case CE_DUPLICATE_SHORTHAND_NAME:
        return "Another call has the same call";
    case CE_DUPLICATE_LONGHAND_NAME:
        return "Another call has the same name";
    case CE_DUPLICATE_NOTATION:
        return "Another call has the same notation";
    case CE_INVALID_SHORTHAND_NAME:
        return "Please supply a call";
    case CE_INVALID_LONGHAND_NAME:
        return "Please supply a name";
    case CE_INVALID_NOTATION:
        return "The notation is invalid";
    }
    return "Error";
}

void Notation::addCallDefault(bool nearCall)
{
	//first delete all calls that of of the correct name
	deleteCall(findCall("-"));

	if (_number > 5)
		deleteCall(findCall("s"));
	
	doAddCallDefault(nearCall);
}

void Notation::doAddCallDefault(bool nearCall)
{
	if (nearCall)
	{									   
        //add 4th place bob
        addCall("-", "Bob", "14");

		setStandardCall("-");

        if (_number > 5)
            //add a 1234 single
            addCall("s", "Single", "1234");
	}
	else
    {
        CString not;

        //add n-2 place bob
        not.Format("1%c", GlobalFunctions::bellNumbersToChar(_number - 2));
        addCall("-", "Bob", not);

		setStandardCall("-");

        if (_number > 5)
        {
            //add a n-2,n-1,n single
            not.Format("1%c%c%c", GlobalFunctions::bellNumbersToChar(_number - 2),
                                  GlobalFunctions::bellNumbersToChar(_number - 1),
                                  GlobalFunctions::bellNumbersToChar(_number ));
            addCall("s", "Single", not);
        }
    }
}


void Notation::setDefaultCalls(BOOL state)
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultCalls;

    if ((_defaultCalls == TRUE)||(state == TRUE))
    {
        //remove all calls;
        for (int i=0;i<_calls.GetSize();i++)
            delete _calls.GetAt(i);
        _calls.RemoveAll();
    }

    _defaultCalls = state;

    //calculate default calls
    if (_defaultCalls == TRUE)
    {
        //can we calculate?
		bool nearCall = true; //by default we add near place calls
	    if ((_leadHead.GetLength() ==1)||(_leadHead.GetLength() ==2))
		{

			if (_leadHead.FindOneOf("m") != -1)
			{
				if (!app()->GetProfileInt("MainFrame", 
									"Near Place Type M", TRUE))
					nearCall = false;

			}
			else if (_leadHead.FindOneOf("ghjklrs") != -1) //extream place
			{
				nearCall = false;
			}
		}

		doAddCallDefault(nearCall);
	}
	setDefaultCallPosMethod();
}

int Notation::findCall(CString name)
{
    for(int i=0;i<_calls.GetSize();i++)
    {
        if(_calls[i]->_nameShorthand == name) return i;
        if(_calls[i]->getDisplayName() == name) return i;
    }

    return -1;
}

NotationCall * Notation::getCall(int index)
{
    if ((index < 0)||(index > _calls.GetSize())) return NULL;

    return _calls[index];
}

int Notation::getCallCount()
{
    return _calls.GetSize();
}

BOOL Notation::getDefaultCalls()
{
    return _defaultCalls;
}

bool Notation::deleteCall(int index)
{
    if ((index < 0)||(index > _calls.GetSize())) return false;

    delete _calls.GetAt(index);
    _calls.RemoveAt(index);

	setDefaultCallPosMethod();

    return true;
}

CallError Notation::changeCall(int index, CString nameShorthand, CString nameLonghand, CString notation)
{
    //create a copy
    NotationCall * pCall = getCall(index);
    if (!pCall) return CE_ERROR;
    NotationCall call = * pCall;

    //remove call
    deleteCall(index);

    //add new call
    CallError ret = addCall(nameShorthand, nameLonghand, notation);

    if ((ret != CE_OK)&&(ret != CE_OK_DEFAULT))
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addCall(call._nameShorthand, call._nameLonghand, call._notation);
    }

    //we will be at the back of the list!
    return ret;
}

CString Notation::getNotes()
{
    return _notes;
}

void Notation::setNotes(CString &notes)
{
    _notes = notes;
}

/*	return vals
    CPLE_OK,
    CPLE_OK_DEFAULT,
    CPLE_DUPLICATE_POS,
    CPLE_INVALID_POS,
    CPLE_ERROR
*/
CallPosLeadError Notation::addCallPosLead(int pos)
{
    prepare();

    if((pos < 0)||(pos > getChangesPerPlainLead()))
        return CPLE_INVALID_POS;

    //check unique pos
    for(int i=0;i<_callPosLead.GetSize();i++)
    {
        if(_callPosLead[i] == pos) return CPLE_DUPLICATE_POS;
    }

    _callPosLead.Add(pos);

    sortCallPosLead();

	setDefaultCallPosMethod();

    return (_defaultCallPosLead)?	CPLE_OK_DEFAULT:CPLE_OK;
}

CString Notation::getAddCallPosLeadErrorText(CallPosLeadError err)
{
    switch (err)
    {
    case CPLE_OK:
        return "Position Added";
    case CPLE_OK_DEFAULT:
        return "Error, the position will be lost - Contact stephen@ringingmaster.co.uk";
    case CPLE_DUPLICATE_POS:
        return "The position already exists";
    case CPLE_INVALID_POS:
        return "This is not a valid position";
    }
    return "Error";
}

void Notation::addCallPosLeadDefault()
{
    prepare();
    addCallPosLead(getChangesPerPlainLead()-1);
}

void Notation::setDefaultCallPosLead(BOOL state)
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultCallPosLead;

    if ((_defaultCallPosLead == TRUE)||(state == TRUE))
    {
        //remove all call positions
        _callPosLead.RemoveAll();
    }

    _defaultCallPosLead = state;

    //calculate default calls
    if (_defaultCallPosLead == TRUE)
    {
        prepare();
        addCallPosLead(getChangesPerPlainLead()-1);
    }

	setDefaultCallPosMethod();
}

BOOL Notation::getDefaultCallPosLead()
{
    return _defaultCallPosLead;
}

int Notation::findCallPosLead(int pos)
{
    for(int i=0;i<_callPosLead.GetSize();i++)
    {
        if(_callPosLead[i] == pos) return i;
    }

    return -1;
}

int Notation::getCallPosLead(int index)
{
    if ((index < 0)||(index > _callPosLead.GetSize())) return -1;

    return _callPosLead[index];
}

int Notation::getCallPosLeadCount()
{
    return _callPosLead.GetSize();
}


bool Notation::deleteCallPosLead(int index)
{
    if ((index < 0)||(index > _callPosLead.GetSize())) return false;

    _callPosLead.RemoveAt(index);

	setDefaultCallPosMethod();

    return true;
}

CallPosLeadError Notation::changeCallPosLead(int index, int pos)
{
    //create a copy
    int oldPos = getCallPosLead(index);
    if (oldPos == -1) return CPLE_ERROR;

    //remove call
    deleteCallPosLead(index);

    //add new call
    CallPosLeadError ret = addCallPosLead(pos);

    if ((ret != CPLE_OK)&&(ret != CPLE_OK_DEFAULT))
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addCallPosLead(oldPos);
    }

    //we will be at the back of the list!
    return ret;
}


//lead lines stuff

/*
LLE_OK,
LLE_OK_DEFAULT,
LLE_DUPLICATE_LINE,
LLE_INVALID_LINE,
LLE_ERROR
*/

LeadLinesError Notation::addLeadLine(int leadLine)
{
    prepare();

    if((leadLine < 0)||(leadLine > getChangesPerPlainLead()))
        return LLE_INVALID_LINE;

    //check unique leadLine
    for(int i=0;i<_leadLines.GetSize();i++)
    {
        if(_leadLines[i] == leadLine) return LLE_DUPLICATE_LINE;
    }

    _leadLines.Add(leadLine);

    sortLeadLine();

    return (_defaultLeadLines)?	LLE_OK_DEFAULT:LLE_OK;
}

CString Notation::getAddLeadLineErrorText(LeadLinesError err)
{
    switch (err)
    {
    case LLE_OK:
        return "Lead line added";
    case LLE_OK_DEFAULT:
        return "Error, the lead line will be lost - Contact stephen@ringingmaster.co.uk";
    case LLE_DUPLICATE_LINE:
        return "The lead line already exists";
    case LLE_INVALID_LINE:
        return "This is not a valid lead line";
    }
    return "Error";
}

void Notation::addLeadLineDefault()
{
	doSetDefaultLeadLine();
}

void Notation::setDefaultLeadLines(BOOL state)
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultLeadLines;

    if ((_defaultLeadLines == TRUE)||(state == TRUE))
    {
        //remove all lead lines
        _leadLines.RemoveAll();
    }

    _defaultLeadLines = state;

    //calculate default calls
    if (_defaultLeadLines == TRUE)
    {
		doSetDefaultLeadLine();
    }
}


void Notation::doSetDefaultLeadLine()
{
	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
    proofInput.setLeadLimit(1);
	Method met;
	proofManager()->requestProof(proofInput, met);

    int afterRow;

	if ((met.getLeadCount() > 0)&&(met.getLead(0).getHuntCount() == 1))
		afterRow = met.getLead(0).GetUpperBound()- 1;
    else
        afterRow = 0;

    addLeadLine(afterRow);
}

int Notation::findLeadLine(int leadLine)
{
    for(int i=0;i<_leadLines.GetSize();i++)
    {
        if(_leadLines[i] == leadLine) return i;
    }

    return -1;

}

int Notation::getLeadLine(int index)
{
    if ((index < 0)||(index > _leadLines.GetSize())) return -1;

    return _leadLines[index];
}

int Notation::getLeadLineCount()
{
    return _leadLines.GetSize();
}

BOOL Notation::getDefaultLeadLines()
{
    return _defaultLeadLines;
}

bool Notation::deleteLeadLine(int index)
{
    if ((index < 0)||(index > _leadLines.GetSize())) return false;

    _leadLines.RemoveAt(index);
    return true;
}

LeadLinesError Notation::changeLeadLine(int index, int leadLine)
{
        //create a copy
    int oldLeadLine = getLeadLine(index);
    if (oldLeadLine == -1) return LLE_ERROR;

    //remove call
    deleteLeadLine(index);

    //add new call
    LeadLinesError ret = addLeadLine(leadLine);

    if ((ret != LLE_OK)&&(ret != LLE_OK_DEFAULT))
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addLeadLine(oldLeadLine);
    }

    //we will be at the back of the list!
    return ret;
}


//call pos method stuff
/*
    CPME_OK,
    CPME_OK_DEFAULT,
    CPME_DUPLICATE_POS,
    CPME_DUPLICATE_NAME,
    CPME_INVALID_LEAD,
    CPME_INVALID_POS,
    CPME_INVALID_NAME,
    CPME_ERROR
*/

CallPosMethodError Notation::addCallPosMethod(int lead, int pos, CString name, bool overwrite)
{
    if (name.IsEmpty()) return CPME_INVALID_NAME;

    //check valid call pos
	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
	Method met;
	proofManager()->requestProof(proofInput, met);


    if (lead >= met.getLeadCount()) return CPME_INVALID_LEAD; //this will prepare the notation
    if (lead < 0) return CPME_INVALID_LEAD; //this will prepare the notation
    if (pos > getChangesPerPlainLead()) return CPME_INVALID_POS;
    if (pos < 0) return CPME_INVALID_POS;
    if (findCallPosLead(pos) == -1) return CPME_INVALID_POS;


    //check unique name	&& pos
    for(int i=0;i<_callPosMethod.GetSize();i++)
    {
        if((_callPosMethod[i]->_lead == lead)&&
           (_callPosMethod[i]->_pos == pos  ))
        {
            if (overwrite)
            {
                deleteCallPosMethod(i);	//allow it to be rewritten
                break;
            }
            else
                return CPME_DUPLICATE_POS;
        }
        if (_callPosMethod[i]->_name == name)   return CPME_DUPLICATE_NAME;
    }

    //create & add
    NotationCallPosMethod * callPosMethod = new NotationCallPosMethod;

    callPosMethod->_lead  = lead;
    callPosMethod->_pos   = pos;
    callPosMethod->_name  = name;

    _callPosMethod.Add(callPosMethod);

    sortCallPosMethod();

    return (_defaultCallPosMethod)?	CPME_OK_DEFAULT:CPME_OK;
}

void Notation::addCallPosMethodDefault(bool overwrite)
{
    doAddCallPosMethodDefault(overwrite);
}

void Notation::doAddCallPosMethodDefault(bool overwrite)
{

	if (overwrite) 
    {
        //remove all calls;
        for (int i=0;i<_callPosMethod.GetSize();i++)
            delete _callPosMethod.GetAt(i);
        _callPosMethod.RemoveAll();
    }

    //calculate default calls
    if ((getCallPosLeadCount() >= 1)&&
        (hasStandardCall())&&
        (getNumber() > 4))
    {
        //as in default state, call pos LEAD only allows one position,
        //and call pos LEAD MUST be false if we have got here

        //lets create a plain course
		ProofInput proofInput(_number);
		proofInput.addNotation(*this);

		//arbetorary number of leads. Can a plain course legitimatly have this many leads? 
		//If it does, then we probably do not want to calculate these values as each bell 
		//is in each position at the lead end at least 10 times.
		proofInput.setLeadLimit(_number * 10);

		Method plainCourse;
		proofManager()->requestProof(proofInput, plainCourse);

		if (plainCourse.getTerminateReason() == TR_CHANGE) //any other terminate reason means that the plain course has not come round.
		{
			//now lets create one bobbed lead.
			ProofInput proofInput(_number);
			proofInput.addNotation(*this);
			proofInput.setLeadLimit(1);

			//default variance (omit from no parts
			Variance variance;

			proofInput.addCall(Call(PT_NONE, variance, 0, _standardCall));
			Method bobbedLead;
			proofManager()->requestProof(proofInput, bobbedLead);

			int pos = getCallPosLead(0);

			if (pos > bobbedLead.getLead(0).GetUpperBound()) return;

			if (getChangesPerPlainLead() > bobbedLead.getLead(0).GetUpperBound()) return;

			calculateCallPosMethod(		&bobbedLead, &plainCourse, pos, 1			 , "I", overwrite);
			calculateCallPosMethod(		&bobbedLead, &plainCourse, pos, 2			 , "B", overwrite);
			if (getNumber() > 5)
				calculateCallPosMethod(	&bobbedLead, &plainCourse, pos, 3			 , "F", overwrite);
			if (getNumber() > 7)
				calculateCallPosMethod(	&bobbedLead, &plainCourse, pos, 4			 , "V", overwrite);
			if (getNumber() > 6)
				calculateCallPosMethod(	&bobbedLead, &plainCourse, pos, getNumber()-3, "M", overwrite);
			calculateCallPosMethod(		&bobbedLead, &plainCourse, pos, getNumber()-2, "W", overwrite);
			calculateCallPosMethod(		&bobbedLead, &plainCourse, pos, getNumber()-1, "H", overwrite);
		}
    }
}

void Notation::calculateCallPosMethod(Method *bobbedLead, Method *plainCourse, int pos, int place, CString callPosName, bool overwrite)
{
	//get the number of the bell in the appropriate place at the end of the bobbed lead
	int bellInPos = bobbedLead->getLastRow()->getBellInPlace(place);
    //now get the 
	int posB4AffectOfBellInPos = bobbedLead->getRow(pos)->getPositionOfBell(bellInPos);
    //find the lead in the plain course that the tenor is in that place
    
    for (int i=0;i<plainCourse->getLeadCount();i++)
    {
		const Lead& lead= plainCourse->getLead(i);
        if ((pos < lead.GetSize())&&
            (lead.GetAt(pos)->getBellInPlace(posB4AffectOfBellInPos) == getNumber()))
        {
            addCallPosMethod(i,pos,callPosName, overwrite);
        }
    }	   
}

void Notation::addCallPosMethodEnum(bool overwrite)
{
    if (overwrite)
    {
        //remove all calls;
        for (int i=0;i<_callPosMethod.GetSize();i++)
            delete _callPosMethod.GetAt(i);
        _callPosMethod.RemoveAll();
    }

    //check valid call pos
	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
	Method met;
	proofManager()->requestProof(proofInput, met);

			/* todo
    //create & add
    int count = 1;
    for (int i=0;i<met.GetSize();i++)
    {
        for (int j=0;j<getCallPosLeadCount();j++)
        {
            CString str;
            str.Format("%d", count++);
            addCallPosMethod(i, getCallPosLead(j), str);
        }
    }   */
}

void Notation::setDefaultCallPosMethod(BOOL state)
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultCallPosMethod;

    if ((_defaultCallPosMethod == TRUE)||(state == TRUE))
    {
        //remove all calls;
        for (int i=0;i<_callPosMethod.GetSize();i++)
            delete _callPosMethod.GetAt(i);
        _callPosMethod.RemoveAll();
    }

    _defaultCallPosMethod = state;

    if (_defaultCallPosMethod == TRUE)
        doAddCallPosMethodDefault();
}

CallPosMethodError Notation::changeCallPosMethod(int index, int lead, int pos, CString name)
{
        //create a copy
    NotationCallPosMethod * pCallPosMethod = getCallPosMethod(index);
    if (!pCallPosMethod) return CPME_ERROR;
    NotationCallPosMethod callPosMethod = * pCallPosMethod;

    //remove call
    deleteCallPosMethod(index);

    //add new call
    CallPosMethodError ret = addCallPosMethod(lead, pos, name);

    if ((ret != CPME_OK)&&(ret != CPME_OK_DEFAULT))
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addCallPosMethod(callPosMethod._lead,
                         callPosMethod._pos,
                         callPosMethod._name);
    }

    //we will be at the back of the list!
    return ret;
}

bool Notation::deleteCallPosMethod(int index)
{
    if ((index < 0)||(index > _callPosMethod.GetSize())) return false;

    delete _callPosMethod.GetAt(index);
    _callPosMethod.RemoveAt(index);
    return true;
}

CString Notation::getAddCallPosMethodErrorText(CallPosMethodError err)
{
    switch (err)
    {
    case CPME_OK:
        return "Call position added";
    case CPME_OK_DEFAULT:
        return "Error The call position will be lost - Contact stephen@ringingmaster.co.uk";
    case CPME_DUPLICATE_POS:
        return "The position already exists";
    case CPME_DUPLICATE_NAME:
        return "Another position has the same name";
    case CPME_INVALID_LEAD:
        return "The lead is not valid";
    case CPME_INVALID_POS:
        return "The row is not available in the lead setup";
    case CPME_INVALID_NAME:
        return "Please supply a name";
    }
    return "Error";
}

BOOL Notation::getDefaultCallPosMethod()
{
    return _defaultCallPosMethod;
}

int Notation::getCallPosMethodCount()
{
    return _callPosMethod.GetSize();
}

NotationCallPosMethod * Notation::getCallPosMethod(int index)
{
    if ((index < 0)||(index >= _callPosMethod.GetSize())) return NULL;

    return _callPosMethod[index];
}

int Notation::findCallPosMethod(int lead, int pos)
{
    for(int i=0;i<_callPosMethod.GetSize();i++)
    {
        if((_callPosMethod[i]->_lead == lead)&&
           (_callPosMethod[i]->_pos  == pos ))
            return i;
    }

    return -1;
}

void Notation::sortCalls()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_calls.GetUpperBound();i++)
        {
            if (_calls[i]->_nameShorthand > _calls[i+1]->_nameShorthand)
            {
                changed = true;
                NotationCall * temp    = _calls.GetAt(i);
                _calls.SetAt(i,_calls.GetAt(i+1));
                _calls.SetAt(i+1, temp);
            }
        }
    }
}

void Notation::sortCallPosLead()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_callPosLead.GetUpperBound();i++)
        {
            if (_callPosLead[i] > _callPosLead[i+1])
            {
                changed = true;
                int temp    = _callPosLead.GetAt(i);
                _callPosLead.SetAt(i,_callPosLead.GetAt(i+1));
                _callPosLead.SetAt(i+1, temp);
            }
        }
    }
}

void Notation::sortSplicePositions()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_splice.GetUpperBound();i++)
        {
            if (_splice[i] > _splice[i+1])
            {
                changed = true;
                int temp    = _splice.GetAt(i);
                _splice.SetAt(i,_splice.GetAt(i+1));
                _splice.SetAt(i+1, temp);
            }
        }
    }
}


void Notation::sortCallPosMethod()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_callPosMethod.GetUpperBound();i++)
        {
            if ((_callPosMethod[i]->_lead > _callPosMethod[i+1]->_lead)||
                (_callPosMethod[i]->_lead == _callPosMethod[i+1]->_lead)&&
                (_callPosMethod[i]->_pos > _callPosMethod[i+1]->_pos))
            {
                changed = true;
                NotationCallPosMethod * temp    = _callPosMethod.GetAt(i);
                _callPosMethod.SetAt(i,_callPosMethod.GetAt(i+1));
                _callPosMethod.SetAt(i+1, temp);
            }
        }
    }
}

void Notation::sortLeadLine()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_leadLines.GetUpperBound();i++)
        {
            if (_leadLines[i] > _leadLines[i+1])
            {
                changed = true;
                int temp    = _leadLines.GetAt(i);
                _leadLines.SetAt(i,_leadLines.GetAt(i+1));
                _leadLines.SetAt(i+1, temp);
            }
        }
    }
}


    //CC info stuff
void Notation::setCCInfo(NotationCCInfoType type, CString info)
{
    //attempt to find entry
    NotationCCInfo * infoCC = NULL;
    int i;
	for (i=0;i<_info.GetSize();i++)
    {
        if (_info[i]->_type == type)
        {
            infoCC = _info[i];
            break;
        }
    }

    //delete from array if empty
    if (info.IsEmpty())
    {
        if (infoCC != NULL)
        {
            delete infoCC;
            _info.RemoveAt(i);
            return;
        }
    }

    //have we found one?
    if (infoCC == NULL)
    {
        infoCC = new NotationCCInfo();
        _info.Add(infoCC);
    }

    //populate
    infoCC->_type = type;
    infoCC->_text = info;
}

CString Notation::getCCInfo(NotationCCInfoType type)
{
    for (int i=0;i<_info.GetSize();i++)
    {
        if (_info[i]->_type == type)
        {
            return _info[i]->_text;
        }
    }
    return CString("");
}

bool Notation::verifyNonEssential(CString &msg)
{
    msg.Empty();

    CString temp;
    bool ret = true;
    int count = 1;

    prepare();

	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
	Method met;
	proofManager()->requestProof(proofInput, met);


    msg += "Please note.\n\n";

    if (getCallCount() == 0)
    {
        temp.Format("%d) No calls defined.\n", count++);
        msg += temp;
        ret = false;
    }

    if (getSplicePositionCount() == 0)
    {
        temp.Format("%d) No splices defined.\n", count++);
        msg += temp;
        ret = false;
    }


    if (getCallPosMethodCount() == 0)
    {
        temp.Format("%d) No calling positions defined.\n", count++);
        msg += temp;
        ret = false;
    }
/*	todo
    if (met.getFalseRowCount() >0)
    {
        temp.Format("%d) The plain course is false.\n", count++);
        msg += temp;
        ret = false;
    }
  */
    return ret;
}

bool Notation::verify(CString &msg)
{
    msg.Empty();

    CString temp;
    bool ret = true;
    int count = 1;

    //get the number of leads
	ProofInput proofInput(_number);
	proofInput.addNotation(*this);
	Method met;
	proofManager()->requestProof(proofInput, met);

    int leadCount = met.getLeadCount();

    msg += "Problems with the defined notation.\n\n";


    //check there is some notation
    if (getName().IsEmpty())
    {
        temp.Format("%d) There is no name entered.\n", count++);
        msg += temp;
        ret = false;
    }

    //check there is some notation
    if (getNotation().IsEmpty())
    {
        temp.Format("%d) There is no notation entered.\n", count++);
        msg += temp;
        ret = false;
    }

    //check the call
    for (int i=0;i<getCallCount();i++)
    {
        NotationCall * call = getCall(i);
        Notation not;
        not.create(true, "", getNumber(),false, call->_notation);
        if (call->_notation != not.getNotation())
        {
            temp.Format("%d) The call '%s' does not have valid notation \n        Please edit the call.\n\n",
            count++, call->_nameShorthand);
            msg += temp;
            ret = false;
        }
    }

    //check that call pos lead is less than changes per plain lead
    for (int i=0;i<getCallPosLeadCount();i++)
    {
        if (getCallPosLead(i) > getChangesPerPlainLead())
        {
            temp.Format("%d) The lead calling position '%d' is greater than the number of changes per lead (%d).\n        Change the lead calling position or add additional notation.\n\n",
                count++, getCallPosLead(i) + 1, getChangesPerPlainLead());
            msg += temp;
            ret = false;
        }
    }

    //check call pos method
    for (int i=0;i<getCallPosMethodCount();i++)
    {
        NotationCallPosMethod * pos = getCallPosMethod(i);
        bool check = false;

        //position
        for (int j=0;j<getCallPosLeadCount();j++)
        {
            if (getCallPosLead(j) == pos->_pos)
            {
                check = true;
            }
        }

        if (check == false)
        {
            temp.Format("%d) The method calling position '%s' has an invalid lead calling position defined (%d).\n      Add %d to the lead calling position, or choose a valid lead calling position.\n\n",
            count++, pos->_name, pos->_pos+1, pos->_pos+1);
            msg += temp;
            ret = false;
        }

        //lead
        if ((pos->_lead >= leadCount)||(pos->_lead < 0))
        {
            temp.Format("%d) The method calling position '%s' has an invalid lead defined (%d).\n      Change the notation or choose a valid lead.\n\n",
            count++, pos->_name, pos->_lead+1);
            msg += temp;
            ret = false;
        }
    }

    //chech call pos agregate
    for (int i=0;i<getCallPosAgregateCount();i++)
    {
        NotationCallPosAgregate* pos =  getCallPosAgregate(i);

        for (int j=0;j<getCallPosMethodCount();j++)
        {
            NotationCallPosMethod * metPos = getCallPosMethod(j);

            //check for names being duplicate with call pos method
            if ( pos->_name == metPos->_name)
            {
                temp.Format("%d) The name '%s' has been used in both method and agregate calling positions\n      Remove or rename either the '%s' method or the '%s' agregate calling position.\n\n",
                count++, pos->_name, pos->_name, pos->_name);
                msg += temp;
                ret = false;
            }
        }


        //check for the callposagregate using only callposmethod positions

        //index through all the agregate calling pos
        for (int j=0;j<pos->_callPosMethods.GetSize();j++)
        {
            bool check = false;
            CString methodPosition = pos->_callPosMethods.GetAt(j);
            //index through all the callposmethods
            for (int k=0;k<getCallPosMethodCount();k++)
            {
                NotationCallPosMethod * metPos = getCallPosMethod(k);
                if (methodPosition == metPos->_name)
                {
                    check = true;
                }
            }

            if (check == false)
            {
                temp.Format("%d) The agregate calling position '%s' has an invalid method calling position defined (%s).\n      Add %s to the method calling position, or choose a valid method calling position, or delete the agregate position.\n\n",
                count++, pos->_name, methodPosition, methodPosition);
                msg += temp;
                ret = false;
            }
        }
    }

    //check that splice position is less than changes per plain lead
    for (int i=0;i<getSplicePositionCount();i++)
    {
        if (getSplicePosition(i) > getChangesPerPlainLead())
        {
            temp.Format("%d) The splice position '%d' is greater than the number of changes per lead plus 1 (%d).\n        Change the splice position or add additional notation.\n\n",
                count++, getSplicePosition(i)+1, getChangesPerPlainLead()+1);
            msg += temp;
            ret = false;
        }
    }

    //check that course head is less than changes per plain lead
    if (getCourseHeadPos() >= getChangesPerPlainLead()+1)
    {
        temp.Format("%d) The course head position '%d' is greater than the number of changes per lead +1 (%d).\n        Change the course head position or add additional notation.\n\n",
            count++, getCourseHeadPos()+1,getChangesPerPlainLead()+1);
        msg += temp;
        ret = false;
    }


    //check lead line
    for (int i=0;i<getLeadLineCount();i++)
    {
        if (getLeadLine(i) >= getChangesPerPlainLead())
        {
            temp.Format("%d) The lead line position '%d' is greater than the number of changes per lead (%d).\n        Change the lead line position or add additional notation.\n\n",
                count++, getLeadLine(i)+1, getChangesPerPlainLead());
            msg += temp;
            ret = false;
        }
    }

    //check for iligal chars
    CString iligalMsg;
    //shorthand
    if (GlobalFunctions::isIligalChar(getSpliceLetter(), iligalMsg))
    {
        temp.Format("%d) Shorthand %s\n\n", count++, iligalMsg);
        msg += temp;
        ret = false;
    }

    //calls
    if (!getDefaultCalls())
    {
        for (int i=0;i<getCallCount();i++)
            {
            if (GlobalFunctions::isIligalChar(getCall(i)->_nameShorthand, iligalMsg))
                {
                temp.Format("%d) Call '%s' %s\n\n", count++, getCall(i)->_nameShorthand, iligalMsg);
                msg += temp;
                ret = false;
            }
        }
    }

    //call pos method
    if (!getDefaultCallPosMethod())
    {
        for (int i=0;i<getCallPosMethodCount();i++)
            {
            if (GlobalFunctions::isIligalChar(getCallPosMethod(i)->_name, iligalMsg))
                {
                temp.Format("%d) Method calling position '%s' %s \n\n", count++, getCallPosMethod(i)->_name, iligalMsg);
                msg += temp;
                ret = false;
            }
        }
    }

    //call pos agregate
    for (int i=0;i<getCallPosAgregateCount();i++)
        {
        if (GlobalFunctions::isIligalChar(getCallPosAgregate(i)->_name, iligalMsg))
            {
            temp.Format("%d) Agregate calling position '%s' %s\n\n", count++, getCallPosAgregate(i)->_name, iligalMsg);
            msg += temp;
            ret = false;
        }
    }

    return ret;
}


bool Notation::hasStandardCall()
{
    return (getStandardCall() != NULL);
}

NotationCall* Notation::getStandardCall()
{
    for (int i=0;i<getCallCount();i++)
    {
        NotationCall* call = getCall(i);
        if ((call)&&
            (call->_nameShorthand == _standardCall))
        {
            return call;
        }
    }

    return NULL;
}

void Notation::setStandardCall(const CString& str)
{
    _standardCall = str;
}



/*	return vals
    CPAE_OK,
    CPAE_DUPLICATE_AGREGATE,
    CPAE_DUPLICATE_NAME,
    CPAE_ERROR
*/
CallPosAgregateError Notation::addCallPosAgregate(NotationCallPosAgregate notationCallPosAgregate)
{
    //check unique pos
    for(int i=0;i<_callPosAgregate.GetSize();i++)
    {
        if(_callPosAgregate[i]->_name == notationCallPosAgregate._name)
            return CPAE_DUPLICATE_NAME;
        if(_callPosAgregate[i]->getDisplay() == notationCallPosAgregate.getDisplay())
            return CPAE_DUPLICATE_AGREGATE;
    }

    _callPosAgregate.Add(new NotationCallPosAgregate(notationCallPosAgregate));

    sortCallPosAgregate();

    return CPAE_OK;
}

CString Notation::getAddCallPosAgregateErrorText(CallPosAgregateError err)
{
    switch (err)
    {
    case CPAE_OK:
        return "Agregate Position Added";
    case CPAE_DUPLICATE_AGREGATE:
        return "The agregate position sequence already exists";
    case CPAE_DUPLICATE_NAME:
        return "The name already exists";
    case CPAE_ERROR:
        return "This is not a valid position";
    }
    return "Error";
}

int Notation::findCallPosAgregate(CString name)
{
    for(int i=0;i<_callPosAgregate.GetSize();i++)
    {
        if(_callPosAgregate[i]->_name == name) return i;
    }

    return -1;
}

NotationCallPosAgregate* Notation::getCallPosAgregate(int index)
{
    if ((index < 0)||(index > _callPosAgregate.GetSize())) return NULL;

    return _callPosAgregate[index];
}

int Notation::getCallPosAgregateCount()
{
    return _callPosAgregate.GetSize();
}


bool Notation::deleteCallPosAgregate(int index)
{
    if ((index < 0)||(index > _callPosAgregate.GetSize())) return false;

    _callPosAgregate.RemoveAt(index);
    return true;
}

void Notation::sortCallPosAgregate()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i=0;i<_callPosAgregate.GetUpperBound();i++)
        {
            NotationCallPosAgregate* ag1 = _callPosAgregate.GetAt(i);
            NotationCallPosAgregate* ag2 = _callPosAgregate.GetAt(i+1);
			CString agCallPosMet1 = ag1->_callPosMethods.GetAt(0);
			CString agCallPosMet2 = ag2->_callPosMethods.GetAt(0);
			NotationCallPosMethod* CPM1 = NULL;
			for (int j=0;j<getCallPosMethodCount();j++)
			{
				if (getCallPosMethod(j)->_name == agCallPosMet1)
				{
					CPM1 = getCallPosMethod(j);
					break;
				}
			}
			if (!CPM1)
				break;

			NotationCallPosMethod* CPM2 = NULL;
			for (int j=0;j<getCallPosMethodCount();j++)
			{
				if (getCallPosMethod(j)->_name == agCallPosMet2)
				{
					CPM2 = getCallPosMethod(j);
					break;
				}
			}
			if (!CPM2)
				break;
			
	        if ((CPM1->_lead > CPM2->_lead)||
                (CPM1->_lead == CPM2->_lead)&&
                (CPM1->_pos > CPM2->_pos))
	        {
                changed = true;
                NotationCallPosAgregate* temp    = _callPosAgregate.GetAt(i);
                _callPosAgregate.SetAt(i,_callPosAgregate.GetAt(i+1));
                _callPosAgregate.SetAt(i+1, temp);
            }
        }
    }
}

CallPosAgregateError Notation::changeCallPosAgregate(int index,NotationCallPosAgregate notationCallPosAgregate )
{
        //create a copy
    NotationCallPosAgregate* pCallPosAgregate = getCallPosAgregate(index);
    if (!pCallPosAgregate) return CPAE_ERROR;
    NotationCallPosAgregate callPosAgregate = * pCallPosAgregate;

    //remove call
    deleteCallPosAgregate(index);

    //add new call
    CallPosAgregateError ret = addCallPosAgregate(notationCallPosAgregate);

    if (ret != CPAE_OK)
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addCallPosAgregate(callPosAgregate);
    }

    //we will be at the back of the list!
    return ret;
}


/*	return vals
    SPE_OK,
    SPE_OK_DEFAULT,
    SPE_DUPLICATE_POS,
    SPE_INVALID_POS,
    SPE_ERROR
*/
SpliceError Notation::addSplicePosition(int pos)
{
    prepare();

    if((pos < 0)||(pos > getChangesPerPlainLead()))
        return SPE_INVALID_POS;

    //check unique pos
    for(int i=0;i<_splice.GetSize();i++)
    {
        if(_splice[i] == pos) return SPE_DUPLICATE_POS;
    }

    _splice.Add(pos);

    sortSplicePositions();

    return (_defaultSplice)?	SPE_OK_DEFAULT:SPE_OK;
}

CString Notation::getAddSplicePositionErrorText(SpliceError err)
{
    switch (err)
    {
    case SPE_OK:
        return "Position Added";
    case SPE_OK_DEFAULT:
        return "Error, the position will be lost - Contact stephen@ringingmaster.co.uk";
    case SPE_DUPLICATE_POS:
        return "The position already exists";
    case SPE_INVALID_POS:
        return "This is not a valid position";
    }
    return "Error";
}

void Notation::addSplicePositionDefault()
{
    prepare();
    addSplicePosition(getChangesPerPlainLead());
}

void Notation::setDefaultSplicePosition(BOOL state )
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultSplice;

    if ((_defaultSplice == TRUE)||(state == TRUE))
    {
        //remove all call positions
        _splice.RemoveAll();
    }

    _defaultSplice = state;

    //calculate default calls
    if (_defaultSplice == TRUE)
    {
        prepare();
        addSplicePosition(getChangesPerPlainLead());
    }
}

BOOL Notation::getDefaultSplicePosition()
{
	return _defaultSplice;
}

int Notation::findSplicePosition(int pos)
{
    for(int i=0;i<_splice.GetSize();i++)
    {
        if(_splice[i] == pos) return i;
    }

    return -1;
}

int Notation::getSplicePosition(int index)
{
    if ((index < 0)||(index > _splice.GetSize())) return -1;

    return _splice[index];
}

int Notation::getSplicePositionCount()
{
    return _splice.GetSize();
}

bool Notation::deleteSplicePosition(int index)
{
    if ((index < 0)||(index > _splice.GetSize())) return false;

    _splice.RemoveAt(index);

    return true;
}

SpliceError Notation::changeSplicePosition(int index, int pos)
{
    //create a copy
    int oldPos = getSplicePosition(index);
    if (oldPos == -1) return SPE_ERROR;

    //remove call
    deleteSplicePosition(index);

    //add new call
    SpliceError ret = addSplicePosition(pos);

    if ((ret != SPE_OK)&&(ret != SPE_OK_DEFAULT))
    {
        //if we failed add back in the existing call. should not fail
        //if this fails we have a serious problem, we will just have to drop it!
        addSplicePosition(oldPos);
    }

    //we will be at the back of the list!
    return ret;
}



bool Notation::getCallingPosInOrder(CStringArray &strArr, BOOL homeLast)
{
	int homePos = -1;
	int i;
	
	//find home
	if (homeLast)
	{
		for (i=0;i<getCallPosMethodCount();i++)
		{
			if (getCallPosMethod(i)->_name == "H")
				homePos = i;
		}
	}

	for (i=homePos+1;i<getCallPosMethodCount();i++)
	{
		strArr.Add(getCallPosMethod(i)->_name);
	}

	for (i=0;i<=homePos;i++)
	{
		strArr.Add(getCallPosMethod(i)->_name);
	}					  

	return true;
	
}

void Notation::setDefaultCourseHeadPos(BOOL state)
{
    if (_call == TRUE) return;

    if (state == -1) //requesting a maintain state
        state = _defaultCourseHeadPos;

    if ((_defaultCourseHeadPos == TRUE)||(state == TRUE))
    {
        //remove all call positions
        _courseHeadPos = getChangesPerPlainLead();
    }

    _defaultCourseHeadPos = state;

    //calculate default calls
    if (_defaultCourseHeadPos == TRUE)
    {
        prepare();
        _courseHeadPos = getChangesPerPlainLead();
    }
}

BOOL Notation::getDefaultCourseHeadPos()
{
    return _defaultCourseHeadPos;
}

BOOL Notation::getCourseHeadPos()
{
    if (_defaultCourseHeadPos)
		return getChangesPerPlainLead();
	else
		return _courseHeadPos;
}

void Notation::setCourseHeadPos(int courseHeadPos)
{
	if (courseHeadPos > getChangesPerPlainLead())
		courseHeadPos = getChangesPerPlainLead();

	setDefaultCourseHeadPos(FALSE);
	_courseHeadPos = courseHeadPos;
}


bool Notation::isSpliceLetter(CString name)	 //todo rename - isNameMatch
{
	return ((getName(true) == name)||
			(getName(false) == name)||
			(getSpliceLetter() == name));
}

void Notation::crib()
{	
	CString msg;

	if (verify(msg))
	{
		if (!getNotation().IsEmpty())
		{
			CWaitCursor wait;
			CDocument* pDoc = app()->openDocument(vt_crib);
			CribDoc* cribDoc = dynamic_cast<CribDoc*>(pDoc);

			if (cribDoc)
			{
				cribDoc->setInitialData(*this);
			}
		}	
	}
	else
	{
		AfxMessageBox(msg);
	}		
}
