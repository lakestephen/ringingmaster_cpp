
#pragma once



enum MatrixEntityToken;
class SaveableObject;

struct DragDropTargetInfo
{
	DragDropTargetInfo() :
	_valid(false), 
	_count(0)
	{
	}

	bool _valid;
	MatrixEntityToken _dataType;
	CArray<SaveableObject*, SaveableObject*> _items;
	CPoint _screenCoordinates;
	int _count;

};

class DragDropTarget 
{

public:
	DragDropTarget(UINT dragBMP, int width);
	virtual ~DragDropTarget();

	void registerNonCWndWindow(HWND hWnd);

protected:
          
	void OnMouseMove(CPoint point);
	void OnLButtonDown(CPoint point);
	bool OnLButtonUp(CPoint point);

	bool _dragging;
	bool _draggingRequest;
	CImageList _draggingImage;
	
	DragDropTargetInfo _dragDropTargetInfo;

	virtual bool canStartDrag() = 0;
	virtual bool getDragInfo(DragDropTargetInfo* info) = 0;
	virtual bool canAcceptDrop(DragDropTargetInfo* info) = 0;
	virtual bool performDrop(DragDropTargetInfo* info) = 0;
	virtual void dragFinished(DragDropTargetInfo* info) = 0 ; //note not necessarly dropped, allows GUI's to tidy up

	DragDropTarget* getTarget(CWnd* pWnd, CPoint point);

	static CMap<HWND, HWND, DragDropTarget*, DragDropTarget*> _windowMap;

private:
	bool doCanAcceptDrop(DragDropTarget* target);
	CMap<DragDropTarget*, DragDropTarget*, int, int> _mousedOverTargets;
}
;

