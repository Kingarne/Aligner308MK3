// $Id: Setup.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class CSetup
{
public:
  CSetup(void) ;
  ~CSetup(void) ;
public:
  BOOL IsConnected( CString channelName );
  void SetNotConnected( CString channelName );
  void SetConnected( CString channelName );
} ;
