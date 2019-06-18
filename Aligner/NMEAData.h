// $Id: NMEAData.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

class NMEAData
{
public:
  NMEAData( double heading ) {
    m_heading = heading ;
  }
public:
  double m_heading ;
} ;