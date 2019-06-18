// $Id: stdafx.h,v 1.1 2009/10/14 08:10:46 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <_vcclrit.h>
#include <string>

extern "C" __declspec(dllexport) void __stdcall DllEnsureInit(void)
{
   // Do nothing else here. If you need extra initialization steps,
   // create static objects with constructors that perform 
   // initialization.
   __crt_dll_initialize() ;
   // Do nothing else here.
}

extern "C" __declspec(dllexport) void __stdcall DllForceTerm(void)
{
   // Do nothing else here. If you need extra terminate steps, 
   // use atexit.
   __crt_dll_terminate() ;
   // Do nothing else here.
}

//	using namespace System ;	//	stlo , commented

