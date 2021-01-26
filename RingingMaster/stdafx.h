// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				
#define WINVER 0x0500		// Allow use of features specific to 2000 and later
#endif


#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0500	// Allow use of features specific to 2000 and later	
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target IE 5.0 or later.
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS



#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxinet.h>
#include <afxhtml.h>
#include <afxmt.h>

#pragma warning(push,2)
#include <mmsystem.h>
#pragma warning(pop)

#include <dos.h>
#include <vector>
#include <algorithm>
#include <direct.h>
#include <math.h>
#include <tchar.h>
#include <locale.h>
#include <assert.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <winreg.h>
#include <wininet.h>

//direct input stuff
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//direct sound stuff
#define DIRECTSOUND_VERSION 0x0800
#include <dsound.h>

//speech stuff
#include <sphelper.h>

//ximage stuff -  causes the .exp and .lib files to be created
#include <ximage.h>

// BCGPro Control Bar
#include <BCGCBProInc.h>

#if _BCGCBPRO_VERSION_ != 0x90530
	_BCGCBPRO_VERSION_ error
	//Please link with verison 9.53 of BCG.
	//See the output window to view the linked verison
	
	//Specific project includes are set in the follownig locations of both 
	// the debug and release project.
	//[Project Properties]-[C/C++]- [General] - [Additional Include Directories]
	//[Project Properties]-[Linker]- [General] - [Additional Library Directories]
	//[Project Properties]-[Resources]- [General] - [Additional Include Directories]
#endif

//ringingmaster specific stuff

const CString DEFAULT_CALL_TOKEN = "DEFAULT_CALL_TOKEN";

typedef CArray<int, int> Ints;
typedef CArray<Ints*, Ints*> Intss;
typedef CArray<unsigned long, unsigned long> UnsignedLongs;
typedef CMap<CString, LPCTSTR, int, int> StringMap;
typedef CMap<DWORD, DWORD, int, int> DWORDIntMap;

#define MAXBELLS 30
#define MINBELLS 3

#define PEAL_LENGTH 5040

#define BADDATA   11111
#define ALLCHANGE 22222	 //i.e. an X, an x or a -  (was 50)
#define SEPERATOR 33333	//i.e. a dot or a space	 (was 51)
#define NOBELL	  44444

const int TypeGapSize = 1000;

const double PI = acos((double)0) *2;
const double PI2 = PI * 2;
const double HALF_PI = PI/2;
const double segment16 = PI2 / 16;
const double segment32 = PI2 / 32;

//ringingmaster global includes
#include "TipDlg.h"
#include "GlobalFunctions.h"



//registering class - allows code to be called on startup. usefull for registering classes for factory work
#define AUTO_REG(classname, code) \
struct AUTO_REG_##classname \
	{ \
	AUTO_REG_##classname() \
		{ \
			code \
		} \
	} reg_##classname; 


