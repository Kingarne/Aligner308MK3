// Created by Microsoft (R) C/C++ Compiler Version 13.10.3077
//
// c:\documents and settings\staffan\my documents\schill\copy of aligner 308_ver1.11\aligner\stdafx.mrg.h
// compiler-generated file created 05/10/06 at 15:51:56
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

// M00PRAGMA("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code



//+++ Start Injected Code
#injected_line 93 "c:\\documents and settings\\staffan\\my documents\\schill\\copy of aligner 308_ver1.11\\aligner\\stdafx.h"

#include <comdef.h>

//--- End Injected Code
// $Id: stdafx.mrg.h,v 1.1 2009/10/14 08:08:14 riar Exp $
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

#import "progid:C1Chart2D7.Control.1" no_implementation, rename( "EOF", "MyEOF" )
//#import "progid:TrueDBGrid70.TDBGrid" no_implementation, rename( "EOF", "MyEOF" )
#import "C:\WINNT\system32\tdbg7.ocx" no_implementation, rename( "EOF", "MyEOF" )
//#import "progid:TrueDBGrid80.TDBGrid" no_implementation, rename( "EOF", "MyEOF" )
#import "C:\WINNT\system32\tdbg8.ocx" no_implementation, rename( "EOF", "MyEOF" )



using namespace std ;
using namespace C1Chart2D7 ;

#pragma warning (disable : 4706 )
#pragma warning (disable : 4239 )

#if defined(_DEBUG) & !defined(DAU_SIMULATION)
  #define DAU_SIMULATION
#endif

#if !defined(BUILD_ALIGNER_308) && !defined(BUILD_ALIGNER_202)
#error Must explicitly specify target executable
#endif

#include "GraphView.h"
