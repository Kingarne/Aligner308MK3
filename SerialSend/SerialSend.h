#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class Application : public CWinApp
{
	DECLARE_MESSAGE_MAP()
public:
	Application( void ) ;
public:
	virtual BOOL InitInstance( void ) ;
} ;

extern Application theApp ;
