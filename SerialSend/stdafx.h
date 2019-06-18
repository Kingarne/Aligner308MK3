#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS
//#pragma pack(push, 8)
#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>

#include <afxdtctl.h>

//#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
//#endif
//#pragma pack(pop)

#include <list>
#include <set>
#include <vector>

using namespace std ;
