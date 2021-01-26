// PropList.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"

#include "PropItem.h"
#include "PropList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropList

PropList::PropList() :
_deviderMove(FALSE),
_columnCount(2),
_showColumnHeads(false),
_buttImg(IDB_PROP_BUT),
_buttImgHot(0),
_buttToolTip("")
{
	_devider[0] = 0;
	_devider[1] = 0;  

	_columnStyle[0] = DT_END_ELLIPSIS;
	_columnStyle[1] = DT_END_ELLIPSIS;
	_columnStyle[2] = DT_END_ELLIPSIS;
}						

PropList::~PropList()
{

}


BEGIN_MESSAGE_MAP(PropList, CCJListBox)
	//{{AFX_MSG_MAP(PropList)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(AAA_PROPCMBBOX, OnKillfocusCmbBox)
	ON_CBN_SELCHANGE(AAA_PROPCMBBOX, OnSelchangeCmbBox)
	ON_EN_KILLFOCUS(AAA_PROPEDITBOX, OnKillfocusEditBox)
	ON_EN_CHANGE(AAA_PROPEDITBOX, OnChangeEditBox)
	ON_BN_CLICKED(AAA_PROPBTNCTRL, OnButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropList message handlers

BOOL PropList::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CCJListBox::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style |= LBS_OWNERDRAWFIXED;

	return TRUE;
}

void PropList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if ((lpMeasureItemStruct->itemID == 0)&&(_showColumnHeads))
		lpMeasureItemStruct->itemHeight = 15; //pixels
	else
		lpMeasureItemStruct->itemHeight = 18; //pixels		  
}


void PropList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	//setup 
	CDC dc;
	dc.Attach(lpDIS->hDC);	
	CRect rcAll = lpDIS->rcItem;
	UINT item = lpDIS->itemID;

	//setup the devider
	switch (_columnCount)
	{
	case 1:
		_devider[0] = rcAll.Width();
		_devider[1] = rcAll.Width();
		break;
	case 2:
		{
			if (_devider[0] <= 0)
				_devider[0] = rcAll.Width() / 2;
			if (_devider[0] > rcAll.Width() - 20)
				_devider[0] = rcAll.Width() - 20;
			_devider[1] = rcAll.Width();
			break;
		}
	case 3:
		{
			if (_devider[0] == 0)
			{
				_devider[0] = rcAll.Width() / 3;
				_devider[1] = (rcAll.Width()*2) / 3;
			}
			if (_devider[1] < 40)
				_devider[1] = 40;
			if (_devider[1] > rcAll.Width() - 20)
				_devider[1] = rcAll.Width() - 20;
			if (_devider[0] >= _devider[1] -20)
				_devider[0] = _devider[1] - 20;
			break;
		}
	}


	//setup rects
	CRect rect[3];

	rect[0] = rcAll;
	rect[1] = rcAll;
	rect[2] = rcAll;

	rect[2].left  = _devider[1]+1;
	rect[1].right = _devider[1];
	rect[1].left  = _devider[0]+1;
	rect[0].right = _devider[0];

	//draw them
	if (item != (UINT) -1)
	{
		//get the PropItem for the current row
		PropItem* pItem = (PropItem*) GetItemDataPtr(item);

		CPen pen;
		CPen * oldPen;

		if ((item == 0)&&(_showColumnHeads))
		{
			for (int i=0;i<3;i++)
			{
				dc.DrawEdge(rect[i] + CRect(-1,-1,1,0) ,BDR_RAISEDOUTER, BF_RECT|BF_MIDDLE);
			}
			dc.SetTextColor(GetSysColor((lpDIS->itemState == ODS_DISABLED)?COLOR_GRAYTEXT:COLOR_WINDOWTEXT));
			pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
			oldPen = dc.SelectObject(&pen);

			//right line
			dc.MoveTo(rcAll.right-1, rcAll.top);
			dc.LineTo(rcAll.right-1, rcAll.bottom);

		}
		else 
		{
			//draw two rectangles, one for each row column
			UINT state = lpDIS->itemState;

			if ((state == ODS_SELECTED)&&(pItem->_type == PT_STATIC))
				state = ODS_DEFAULT;

			if (pItem->_type == PT_STATIC_GREY)
				state = ODS_DISABLED;

			switch (state)
			{
			case ODS_SELECTED:
				dc.FillSolidRect(rcAll,GetSysColor(COLOR_HIGHLIGHT));
				dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				break;
/*			case ODS_FOCUS:
				dc.FillSolidRect(rcAll,GetSysColor(COLOR_BTNFACE));
				dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
				break;
*/			case ODS_DISABLED:
				dc.FillSolidRect(rcAll,::GetSysColor(COLOR_WINDOW));
				dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
				break;
			default:
				dc.FillSolidRect(rect[0],::GetSysColor(COLOR_WINDOW));
				dc.FillSolidRect(rect[1],pItem->_backgroundColor);
				dc.FillSolidRect(rect[2],pItem->_backgroundColor);
				dc.SetTextColor(pItem->_textColor);
				break;
			}

			pen.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DLIGHT));
			oldPen = dc.SelectObject(&pen);
		}


		//underline 
		dc.MoveTo(rcAll.left,  rcAll.bottom - 1);
		dc.LineTo(rcAll.right, rcAll.bottom - 1);

		//devider 0
		dc.MoveTo(_devider[0], rcAll.top);
		dc.LineTo(_devider[0], rcAll.bottom);
		//devider 1
		dc.MoveTo(_devider[1], rcAll.top);
		dc.LineTo(_devider[1], rcAll.bottom);

		dc.SetBkMode(TRANSPARENT);
		//text 1

		dc.SelectObject(oldPen);  
		
		for (int i=0;i<3;i++)
		{
			dc.DrawText(pItem->_col[i],
						CRect(rect[i].left+3, rect[i].top+1,rect[i].right-3,rect[i].bottom-1),
						DT_LEFT|DT_SINGLELINE|DT_VCENTER| DT_NOPREFIX | _columnStyle[i]);
		}

	} 
	dc.Detach();
}

int PropList::AddPropItem(PropItem* pItem)
{
	int nIndex = AddString(_T(""));
	SetItemDataPtr(nIndex,pItem);
	return nIndex;
}

int PropList::insertPropItem(int index, PropItem *pItem)
{
	int nIndex = InsertString(index, _T(""));
	SetItemDataPtr(nIndex,pItem);
	return nIndex;
}

void PropList::OnSelchange() 
{
	CRect rect;
	CString lBoxSelText;

	GetItemRect(GetCurSel(),rect);
	if (_columnCount == 2)
		rect.left = _devider[0] +1;
	else if (_columnCount == 3)
		rect.left = _devider[1] +1;

	rect.bottom += 2;

	if (_button)
		_button.ShowWindow(SW_HIDE);

	PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());

	if ((_showColumnHeads)&&(GetCurSel() == 0)) return;

	if (pItem->_type == PT_STATIC)
	{
		return;
	}
	else if (pItem->_type == PT_STATIC_GREY)
	{
		return;
	}
	else if (pItem->_type == PT_COMBO)
	{
		//display the combo box.  If the combo box has already been
		//created then simply move it to the new location, else create it
		_nLastBox = 0;
		if (_combo)
			_combo.MoveWindow(rect);
		else
		{	
			rect.bottom += 100;
			_combo.Create(CBS_DROPDOWNLIST |  CBS_NOINTEGRALHEIGHT | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL ,
							rect,this,AAA_PROPCMBBOX);
			_combo.SetFont(&app()->_fontStd);
		}

		//add the choices for this particular property
		CString list = pItem->_list;
		lBoxSelText = pItem->_col[1];
		
		_combo.ResetContent();

		int i,i2;
		i=0;
		while ((i2=list.Find('|',i)) != -1)
		{
			_combo.AddString(list.Mid(i,i2-i));
			i=i2+1;
		}

		_combo.ShowWindow(SW_SHOW);
		_combo.SetFocus();

		//jump to the property's current value in the combo box
		int j = _combo.FindStringExact(0,lBoxSelText);
		if (j != CB_ERR)
			_combo.SetCurSel(j);
		else
			_combo.SetCurSel(0);
	}
	else if (pItem->_type==PT_EDIT)
	{
		//display edit box
		_nLastBox = 1;
		rect.bottom -= 3;
		if (_edit)
			_edit.MoveWindow(rect);
		else
		{	
			_edit.Create(ES_LEFT | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER,
							rect,this,AAA_PROPEDITBOX);
			_edit.SetFont(&app()->_fontStd);
		}

		lBoxSelText = pItem->_col[1];

		_edit.ShowWindow(SW_SHOW);
		_edit.SetFocus();
		//set the text in the edit box to the property's current value
		_edit.SetWindowText(lBoxSelText);
	}
	else
	{
		//displays a button
		_nLastBox = 2;

		if (rect.Width() > 20)
			rect.left = rect.right - 20;
		rect.bottom -= 3;

		if (_button)
			_button.MoveWindow(rect);
		else
		{	
			_button.Create("",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
							rect,this,AAA_PROPBTNCTRL);
			_button.SetImage(_buttImg, _buttImgHot);
			_button.m_bDrawFocus = FALSE;
			_button.SetTooltip(_buttToolTip);

		}

		_button.ShowWindow(SW_SHOW);
		_button.SetFocus();
	}
}

void PropList::OnKillfocusCmbBox() 
{
	PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
	doKillFocusCmbBox(pItem);

	_combo.ShowWindow(SW_HIDE);

	Invalidate();
}

void PropList::doKillFocusCmbBox(PropItem* /*pItem*/)
{
	//virtual in base class
}

void PropList::OnKillfocusEditBox()
{
	PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
	doKillFocusEditBox(pItem);

	_edit.ShowWindow(SW_HIDE);

	Invalidate();
}

void PropList::doKillFocusEditBox(PropItem* /*pItem*/)
{
	//virtual in base class
}

void PropList::OnSelchangeCmbBox()
{
	CString selStr;
	if (_combo)
	{
		_combo.GetLBText(_combo.GetCurSel(),selStr);
		PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
		pItem->_col[1] = selStr;
		DoSelchangeCmbBox(pItem);
	}
}

void PropList::DoSelchangeCmbBox(PropItem* /*pItem*/)
{
	//virtual in base class
}

void PropList::OnChangeEditBox()
{
	CString newStr;
	if (_edit)
	{
		_edit.GetWindowText(newStr);
		PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
		pItem->_col[1] = newStr;
		DoChangeEditBox(pItem);
	}
}

void PropList::DoChangeEditBox(PropItem* /*pItem*/)
{
	//virtual in base class

}

void PropList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CCJListBox::OnLButtonDblClk(nFlags, point);

	if ((GetCurSel() == LB_ERR)||
		((_showColumnHeads)&&
		(GetCurSel() == 0)))
		return;
	
	PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
	
	if (pItem->_type == PT_BUTTON)
		doButton(pItem);
	
	Invalidate();
	
}

void PropList::doButton(PropItem * pItem)
{
	if (::IsWindow(GetParent()->m_hWnd))
		::PostMessage(GetParent()->m_hWnd, WM_PROP_BUTTON,NULL, (LPARAM)pItem); 
	//virtual in base class


}

void PropList::OnButton()
{
	if ((GetCurSel() == LB_ERR)||
		((_showColumnHeads)&&(GetCurSel() == 0)))
		return;
	
	PropItem* pItem = (PropItem*) GetItemDataPtr(GetCurSel());
	
	doButton(pItem);
	
	Invalidate();
}

void PropList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (_deviderMove != FALSE)
	{
		//if columns were being resized then this indicates
		//that mouse is up so resizing is done.  Need to redraw
		//columns to reflect their new widths.
		
		//if mouse was captured then release it
		if (GetCapture()==this)
			::ReleaseCapture();

		::ClipCursor(NULL);

		CClientDC dc(this);
		GlobalFunctions::InvertLine(&dc,CPoint(point.x,_nDivTop),CPoint(point.x,_nDivBtm));
		//set the divider position to the new value
		if (_deviderMove == 1) 
			_devider[0] = point.x;
		else if (_deviderMove == 2) 
			_devider[1] = point.x;

		_deviderMove = FALSE;
		//redraw
		Invalidate();
	}
	else
	{
		_deviderMove = FALSE;
		CCJListBox::OnLButtonUp(nFlags, point);
	}
}

void PropList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (((_columnCount >= 2)&&(point.x >= _devider[0]-5) && (point.x <= _devider[0]+5))||
		((_columnCount >= 3)&&(point.x >= _devider[1]-5) && (point.x <= _devider[1]+5)))
	{
		if ((_columnCount >= 2)&&(point.x >= _devider[0]-5) && (point.x <= _devider[0]+5))
			_deviderMove = 1;			
		if ((_columnCount >= 3)&&(point.x >= _devider[1]-5) && (point.x <= _devider[1]+5))
			_deviderMove = 2;			

		//if mouse clicked on divider line, then start resizing
		SetCursor(app()->LoadStandardCursor(IDC_SIZEWE));

		CRect windowRect;
		GetWindowRect(windowRect);
		windowRect.left += 20; windowRect.right -= 20;
		//do not let mouse leave the list box boundary
		::ClipCursor(windowRect);
		
		if (_combo)
			_combo.ShowWindow(SW_HIDE);
		if (_edit)
			_edit.ShowWindow(SW_HIDE);

		CRect clientRect;
		GetClientRect(clientRect);


		_nDivTop = clientRect.top;
		_nDivBtm = clientRect.bottom;
		_nOldDivX = point.x;

		CClientDC dc(this);
		GlobalFunctions::InvertLine(&dc,CPoint(_nOldDivX,_nDivTop),CPoint(_nOldDivX,_nDivBtm));

		//capture the mouse
		SetCapture();
	}
	else
	{
		_deviderMove = FALSE;
		CCJListBox::OnLButtonDown(nFlags, point);
	}
}

void PropList::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (_deviderMove != FALSE)
	{
		//move divider line to the mouse pos. if columns are
		//currently being resized
		CClientDC dc(this);
		//remove old divider line
		GlobalFunctions::InvertLine(&dc,CPoint(_nOldDivX,_nDivTop),CPoint(_nOldDivX,_nDivBtm));
		//draw new divider line
		GlobalFunctions::InvertLine(&dc,CPoint(point.x,_nDivTop),CPoint(point.x,_nDivBtm));
		_nOldDivX = point.x;
	}
	if (((_columnCount >= 2)&&(point.x >= _devider[0]-5) && (point.x <= _devider[0]+5))||
		((_columnCount >= 3)&&(point.x >= _devider[1]-5) && (point.x <= _devider[1]+5)))
		//set the cursor to a sizing cursor if the cursor is over the row divider
		SetCursor(app()->LoadStandardCursor(IDC_SIZEWE));
	else
		CCJListBox::OnMouseMove(nFlags, point);
}

void PropList::OnDestroy() 
{
	for (int i=0;i<GetCount();i++)
	{
		PropItem* pItem = (PropItem*) GetItemDataPtr(i);
		if (pItem != (PropItem*)-1)
			delete pItem;
	}
								 
	CCJListBox::OnDestroy();
}

void PropList::setColumnCount(int columnCount)
{
	_columnCount = columnCount;
	_devider[0] = 0;
	_devider[1] = 0;
	Invalidate();
}

void PropList::setColumnHead(bool head)
{
	_showColumnHeads = head;
	if (::IsWindow(m_hWnd)) 
		Invalidate();
}

void PropList::deletePropItem(int index)
{
	if ((index > 0)&&
		(index < GetCount())&&
		!((index == 0)&&(_showColumnHeads)))
	{
		PropItem* pItem = (PropItem*) GetItemDataPtr(index);
		if (pItem != (PropItem*)-1)
			delete pItem;
		
		DeleteString(index);
		if ((_button)&&(_button.m_hWnd))
			_button.ShowWindow(SW_HIDE);
	}
}

void PropList::deleteAllPropItems()
{
	for (int i=GetCount();i>=((_showColumnHeads)?1:0);i--)
	{
		PropItem* pItem = (PropItem*) GetItemDataPtr(i);
		if (pItem != (PropItem*)-1)
			delete pItem;
		
		DeleteString(i);
		if (_button) 
			_button.ShowWindow(SW_HIDE);
	}

}

PropItem * PropList::getPropItem(int index)
{ 
	if((index <0)||
	   ((index == 0)&&(_showColumnHeads))||
	   (index > GetCount()))
	   return NULL;

	return (PropItem*) GetItemDataPtr(index);
}



void PropList::OnSize(UINT nType, int cx, int cy) 
{
	CCJListBox::OnSize(nType, cx, cy);
	
	if (IsWindow(_button)) _button.ShowWindow(SW_HIDE);
	if (IsWindow(_combo)) _combo.ShowWindow(SW_HIDE);
	if (IsWindow(_edit)) _edit.ShowWindow(SW_HIDE);
}

void PropList::setButtImages(UINT _img, UINT _imgHot)
{
	_buttImg = _img;
	_buttImgHot = _imgHot;
}

void PropList::setButtonToolTip(CString tip)
{
	_buttToolTip = tip;
}

CBCGPButton* PropList::getButtonWnd()
{
	return &_button;
}

BOOL PropList::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CCJListBox::PreTranslateMessage(pMsg);
}

PropItem* PropList::findItem(const CString &col0)
{
	for (int i=0;i<GetCount();i++)
	{
		PropItem* pItem = getPropItem(i);
		if (pItem->_col[0] == col0)
			return pItem;
	}
	return NULL;
}

int PropList::findItemIndex(const CString &col0)
{
	for (int i=0;i<GetCount();i++)
	{
		PropItem* pItem = getPropItem(i);
		if (pItem->_col[0] == col0)
			return i;
	}
	return -1;
}

void PropList::setColumnHeads(CString col1, CString col2, CString col3)
{
	if((_showColumnHeads)&&
	   (GetCount() > 0))
	{
		PropItem * pItem = (PropItem*) GetItemDataPtr(0);

		pItem->_col[0] = col1;
		pItem->_col[1] = col2;
		pItem->_col[2] = col3;

		Invalidate();
	}
}

void PropList::setColumnStyle(int index, UINT style)
{
	ASSERT (index>=0&&index<3);

	_columnStyle[index] = style;

}
