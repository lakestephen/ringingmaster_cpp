// StyleManager.cpp: implementation of the StyleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"
#include "StyleManager.h"
#include "RegArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StyleManager::StyleManager() :
_activeStyle(0)
{

}

StyleManager::~StyleManager()
{

}
	

void StyleManager::load()
{
	CString strToken, strEntry ;	
	
	int count = app()->GetProfileInt("Style", "Count", 0);
	_activeStyle = app()->GetProfileInt("Style", "Active", 0);
	
	if (_activeStyle >= count) _activeStyle = 0;

	//add a default list
	if (count < 1) 
	{
		Style style;
		style.setDefault();
		_styleArray.Add(style);
	}

	for (int i=0;i<count;i++)
	{
		strEntry.Format("Style %d", i);
		RegArchive ar (CArchive::load, "Style", strEntry);
		Style style;
		if (ar.isValid()) style.Serialize(ar);
		_styleArray.Add(style);
	}
}


void StyleManager::save()
{
	CString strToken, strEntry ;	

	int count = _styleArray.GetSize();
	
	app()->WriteProfileInt("Style", "Count", count);
	app()->WriteProfileInt("Style", "Active", _activeStyle);
	
	for (int i=0;i<count;i++)
	{
		strEntry.Format("Style %d", i);
		RegArchive ar(CArchive::store,"Style",strEntry);   
		_styleArray[i].Serialize(ar);
	}
}


BOOL StyleManager::isStyleUnique(CString strName)
{
	//check for unique name
	for (int i=0;i<_styleArray.GetSize();i++)
	{
		if (strName == _styleArray[i]._name)
			return false;
	}
	
	return true;
}

int StyleManager::getStyleIndexFromName(CString strName)
{
	//check for unique name
	for (int i=0;i<_styleArray.GetSize();i++)
	{
		if (strName == _styleArray[i]._name)
			return i;
	}
	
	return -1;
}


Style* StyleManager::getStyle(int index)
{
	if (index == -1) index = _activeStyle;
	if (index >= _styleArray.GetSize()) return NULL;
		
	return &(_styleArray[index]);
}


int StyleManager::getStyleCount()
{
	return _styleArray.GetSize();
}

void StyleManager::setupStyleMenu (CMenu* menu)
{
	if (menu) 
	{
	
		//---------------------------------------------------------------
		// Replace style dummy items to dynamic style names:
		//---------------------------------------------------------------
		for (int i = 0; i < (int) menu->GetMenuItemCount ();)
		{
			UINT uiCmd = menu->GetMenuItemID (i);

			if (uiCmd >= ID_STYLE_MENU_00 && 
				uiCmd <= ID_STYLE_MENU_09)
			{
				//-------------------------------------------------------------------
				// "User toolbar" item. First check that style number 'x' is exist:
				//-------------------------------------------------------------------
				Style* style = getStyle(uiCmd - ID_STYLE_MENU_00);
				if (style != NULL)
				{
					CString name;
					name.Format("&%d ",uiCmd - ID_STYLE_MENU_00 + 1); 
					name += style->_name;
					menu->ModifyMenu (i ++, MF_BYPOSITION | MF_STRING, uiCmd, name);
				}
				else
				{
					menu->DeleteMenu (i, MF_BYPOSITION);
				}
			}
			//remove more item if necedssary
			else if (uiCmd == ID_STYLE_MENU_MORE)
			{
				if (getStyleCount() < 10) 
				{
					menu->DeleteMenu (i, MF_BYPOSITION);
				}
				else 
				{
					i++;
				}
			}
			else	// Not "user toolbar" item, move next
			{
				i ++;
			}
		}
	}
}


int StyleManager::getActiveStyleIndex()
{
	return _activeStyle;
}

void StyleManager::setActiveStyleIndex(int index)
{
	_activeStyle = index;
}						   

void StyleManager::addStyle(Style style)
{
	_styleArray.Add(style);
}

void StyleManager::removeStyle(int index)
{
	_styleArray.RemoveAt(index);
}

void StyleManager::setAt(int index, Style style)
{
	_styleArray.SetAt(index,style);
}
