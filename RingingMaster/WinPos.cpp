// WinPos.cpp: implementation of the WinPos class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinPos.h"
#include "RingingMaster.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WinPos::WinPos()
{

}

WinPos::~WinPos()
{

}


void WinPos::Save(CWnd *win, CString token)
{
	if ((win)&&(::IsWindow(win->m_hWnd))) 
	{
		CRect rWindow;
		win->GetWindowRect(&rWindow);
		AfxGetApp()->WriteProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Top", rWindow.top);	
		AfxGetApp()->WriteProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Left", rWindow.left); 
		AfxGetApp()->WriteProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Width", rWindow.Width());	
		AfxGetApp()->WriteProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Height", rWindow.Height()); 
	}
	
}

void WinPos::Load(CWnd *win, CString token, bool includeSize)
{
	if ((win)&&(::IsWindow(win->m_hWnd))) 
	{	CRect rWindow, rNewWindow;
		win->GetWindowRect(&rWindow);
		rNewWindow.top = AfxGetApp()->GetProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos",  token + " Position Top", -1);
		rNewWindow.left = AfxGetApp()->GetProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Left", -1);
		
		if (includeSize)
		{
			rNewWindow.bottom = rNewWindow.top + AfxGetApp()->GetProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Height", -1);
			rNewWindow.right = rNewWindow.left + AfxGetApp()->GetProfileInt(RingingMasterApp::_RegistryBase + "\\WinPos", token + " Position Width", -1);
		} 
		else
		{
			rNewWindow.bottom = rNewWindow.top + rWindow.Height();
			rNewWindow.right = rNewWindow.left + rWindow.Width();
		}

		if ((rNewWindow.top != -1)&&
			(rNewWindow.top > 0)&&
			(rNewWindow.top < 3000)&&
			(rNewWindow.left > 0)&&
			(rNewWindow.left < 3000)&&
			(rNewWindow.right > 0)&&
			(rNewWindow.right < 3000)&&
			(rNewWindow.bottom > 0)&&
			(rNewWindow.bottom < 3000))
		{
			win->MoveWindow( rNewWindow);
		}
	}
}
