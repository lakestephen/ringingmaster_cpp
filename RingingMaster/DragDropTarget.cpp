

#include "stdafx.h"
#include "RingingMaster.h"
#include "DragDropTarget.h"

#include "MatrixManager.h"


CMap<HWND, HWND, DragDropTarget*, DragDropTarget*> DragDropTarget::_windowMap;

DragDropTarget::DragDropTarget(UINT dragBMP, int width) :
_dragging(false), 
_draggingRequest(false)
{
	// Create a drag image
	_draggingImage.Create(dragBMP, width, 1, RGB(255,0,255)); 

}

DragDropTarget::~DragDropTarget()
{
}

void DragDropTarget::registerNonCWndWindow(HWND hWnd)
{
	_windowMap.SetAt(hWnd, this);
}

void DragDropTarget::OnLButtonDown(CPoint point)
{
	CWnd* pWnd = dynamic_cast<CWnd*>(this);

	if (!pWnd)
		return;

	_mousedOverTargets.RemoveAll();

	if (canStartDrag())
	{
		_draggingRequest = true;
		_dragging = false;
		pWnd->SetCapture ();
	}
}

bool DragDropTarget::OnLButtonUp(CPoint point)
{
	CWnd* pWnd = dynamic_cast<CWnd*>(this);

	if (!pWnd)
		return false;

    ReleaseCapture ();


	if (_dragging)
	{
        //end drag image
		_draggingImage.DragLeave (pWnd->GetDesktopWindow ());
        _draggingImage.EndDrag ();


        CPoint pt(point);    //get our current mouse coordinates
        pWnd->ClientToScreen(&pt); //convert to screen coordinates

		_dragDropTargetInfo._screenCoordinates = pt;

		DragDropTarget* target = getTarget(pWnd, pt);
                 
		if ((target) && 
			(target != this)&&
			(target->canAcceptDrop(&_dragDropTargetInfo)))
		{
			target->performDrop(&_dragDropTargetInfo);
		}    

		//iterate through all the affected windows, and allow them to perform GUI clean up.
		POSITION pos = _mousedOverTargets.GetStartPosition();
		while(pos)
		{
			int count = 0;
			DragDropTarget* dragDropTarget = NULL;
			_mousedOverTargets.GetNextAssoc(pos, dragDropTarget, count);

			ASSERT(dragDropTarget);

			if (dragDropTarget)
				dragDropTarget->dragFinished(&_dragDropTargetInfo);
		}   
	}

	_dragDropTargetInfo._valid = false;
	_dragDropTargetInfo._items.RemoveAll();

	bool wasDragging = _dragging;

	_dragging = false;
	_draggingRequest = false;

	return wasDragging;
}

void DragDropTarget::OnMouseMove(CPoint point)
{

	CWnd* pWnd = dynamic_cast<CWnd*>(this);

	if (!pWnd)
		return;

	if (_draggingRequest)
	{
		//get the drag information
		_dragDropTargetInfo._valid = false;
		_dragDropTargetInfo._items.RemoveAll();
		getDragInfo(&_dragDropTargetInfo);

		if (_dragDropTargetInfo._valid)
		{
			_dragging = true;

			_draggingImage.BeginDrag(0, CPoint(-10, -10));
			CPoint screenPoint(point);
			pWnd->ClientToScreen(&screenPoint);
			_draggingImage.DragEnter(pWnd->GetDesktopWindow(), screenPoint);	
		}
	}    

	if (_dragging)
    {
        // Move the drag image
        CPoint pt(point);    //get our current mouse coordinates
        pWnd->ClientToScreen(&pt); //convert to screen coordinates
		_draggingImage.DragMove(pt); //move the drag image to those coordinates
        // Unlock window updates (this allows the dragging image to be shown 
                // smoothly)
        _draggingImage.DragShowNolock(false);

		_dragDropTargetInfo._screenCoordinates = pt;

		DragDropTarget* target = getTarget(pWnd, pt);
                 
		if ((target) && 
			(target != this)&&
			(doCanAcceptDrop(target)))
		{
			//we are over a drag drop window. 
			SetCursor(app()->LoadStandardCursor(IDC_ARROW));
		}
		else
			SetCursor(LoadCursor(NULL, IDC_NO));


		_draggingImage.DragShowNolock(true);

	}
}

DragDropTarget* DragDropTarget::getTarget(CWnd* pWnd, CPoint point)
{
	CWnd* pDropWnd = pWnd->WindowFromPoint (point);
		
	DragDropTarget* target = NULL;

	//first attempt to get it from the static map.
	HWND hWnd  = pDropWnd->m_hWnd;
	if (_windowMap.Lookup(hWnd, target))
		return target;

	//now attempt a cast
	//NOTE: The BCG code does not have the RTTI compile options switched on. 
	// Thus we get class cast exceptions. This try..catch is easier to maintain than 
	// switching on RTTI on in bcg each release.
	try {

		if (target = dynamic_cast<DragDropTarget*>(pDropWnd))
			return target;
	}
	catch(...)
	{

	}	
	return target;
}

bool DragDropTarget::doCanAcceptDrop(DragDropTarget* target)
{
	//test to see if we have had this target before.
	_dragDropTargetInfo._count = -1;          
	//get the count - if this is the first time for this target, then count will stay as the -1 default
	_mousedOverTargets.Lookup(target, _dragDropTargetInfo._count);
	//increment the count, and record away
	_mousedOverTargets.SetAt(target, ++_dragDropTargetInfo._count);

	return target->canAcceptDrop(&_dragDropTargetInfo);			
}
