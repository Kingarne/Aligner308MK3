// $Id: stdafx.h,v 1.5 2014-10-16 08:46:56 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once


#ifndef WIN32
#define WIN32
#endif

#ifndef _WINDOWS
#define _WINDOWS
#endif

#ifndef _ATL_ATTRIBUTES
#define _ATL_ATTRIBUTES
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#define _USE_MATH_DEFINES

// Do not change the order of apperence for the include files.
// The declaration (and definition) of classes and methods depend on
// the exact order of inclusion.

#include <afxwin.h>      
#include <afxoledb.h>    
#include <atlcomtime.h>  
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#include <afxcmn.h>
#include <afxmt.h>
#include <afxpriv.h>

#include <atlbase.h>
#include <atlplus.h>
#include <atlconv.h>

#ifndef _USE_MATH_DEFINES 
#define _USE_MATH_DEFINES 
#endif
#include <math.h>
#include <stdlib.h>
#include <direct.h>
#include <wchar.h>
#include <float.h>
#include <io.h>

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>

#include <shlobj.h>

#include <afx.h>
#include <atldbcli.h>
#include <afxdlgs.h>
#include <afxdhtml.h>
#include <afxctl.h>

#include <odbcinst.h> 
#include <iphlpapi.h>

//Staffan
#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include <signal.h>
                  
#define TrueDBGridVer TrueDBGrid80

//#import "C:\Program\ComponentOne\Chart7\Bin\olch2x7.ocx" no_implementation, rename( "EOF", "MyEOF" )
#import "C:\Windows\System32\olch2x8.ocx" no_implementation, rename( "EOF", "MyEOF" )
#import "C:\Windows\system32\tdbg8.ocx" no_implementation, rename( "EOF", "MyEOF" )	

using namespace std ;
using namespace C1Chart2D8 ;

#pragma warning (disable : 4706 )
#pragma warning (disable : 4239 )

#if defined(_DEBUG) & !defined(DAU_SIMULATION) & defined(DAU_SIMULATION_PRO)
  #define DAU_SIMULATION
#endif

#if !defined(BUILD_ALIGNER_308) && !defined(BUILD_ALIGNER_202)
#error Must explicitly specify target executable
#endif


#include "GraphView.h"
#include "Registry.h"
#include "NStopWatch.h"
#include "UserMessages.h"
#include "DBInterface.h"
#include "SystemSetup.h"
#include <afxwin.h>
#include <afxwin.h>

#define _PI 3.1415f
#define ToMRad(a) (a*_PI/180.0f*1000.0f)
#define ToRad(a) (a*_PI/180.0f)
