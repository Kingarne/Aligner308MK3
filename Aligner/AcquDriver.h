// $Id: AcquDriver.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002
//Matz was here

#pragma once

#define MAIN_BOARD_ARRAY_INDEX 0

#include "Defines.h"
#include "GlobalData.h"
#include "DAU.h"

// SourceStatus in struct AcquDriverInParam
#define ACQU_IDLE         1
#define ACQU_GET_DATA     2
#define ACQU_DATA_READY   3

/**
 *  This is the typedef of the struct that serves as Measure <--> ACQUDRIVER input interface.
 *  Note that some of the parameters might be changed by ACQUDRIVER.
 */
typedef struct
{
  DAU *pSource ;
  int SourceStatus ;   // changed in ACQUDRIVER()
} AcquDriverInParam ;

/**
 *  This is the typedef of the struct that serves as Measure <--> ACQUDRIVER output interface.
 *  All sensor/syncro/temperature parameters returned from the DAU are returned in
 *  a struct of this type.
 */
typedef struct
{
  double AbsRoll [SIZE_OF_ARRAYS] ;
  double AbsPitch [SIZE_OF_ARRAYS] ;
  double TempT [SIZE_OF_ARRAYS] ;
  short RollRawData [SIZE_OF_ARRAYS] ;
  short PitchRawData [SIZE_OF_ARRAYS] ;
} AcquDriverOutParam ;

class CAcquDriver : public CWnd
{
	DECLARE_DYNAMIC(CAcquDriver)
	DECLARE_MESSAGE_MAP()
public:
	CAcquDriver( void ) ;
	virtual ~CAcquDriver( void ) ;
public:
  void ACQUDRIVER( void ) ;
  AcquDriverInParam m_InParam ;
  AcquDriverOutParam m_OutParam ;
} ;
