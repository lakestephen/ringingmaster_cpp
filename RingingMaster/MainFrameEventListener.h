// MainFrameEventListener.h: interface for the MainFrameEventListener class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


enum ViewType;

class MainFrameEventListener  
{
public:
	MainFrameEventListener() {};
	virtual ~MainFrameEventListener(){};

	virtual void mainFrame_notifyUpdate(ViewType /*viewType*/){};
	virtual void mainFrame_notifyDocViewChanged(ViewType /*oldViewType*/, ViewType /*newViewType*/, CDocument* /*oldDoc*/, CDocument* /*newDoc*/) {};
	virtual void mainFrame_notifyIdle() {};

};		  

typedef CArray<MainFrameEventListener*, MainFrameEventListener*> MainFrameEventListeners;

