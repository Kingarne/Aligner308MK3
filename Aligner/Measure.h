// $Id: Measure.h,v 1.2 2011/10/13 10:09:12 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "AcquDriver.h"
#include "GraphView.h"
#include "Util.h"

#define SPERRY_K        164
#define TETA            0.9
#define NO_OF_TAO_IN_AUTOSCALE_X_AXIS   16 //length of autoscaled x-axis is fixed: NO_OF_TAO_IN_AUTOSCALE_X_AXIS * Tao

#define MEASURE_MODE_ON  0
#define MEASURE_MODE_OFF 1

// CMeasure class

class CMeasure :
  public CWnd
{
public:
  CMeasure(void);
  ~CMeasure(void);
  void SetGraphPointer( CGraphView *pGraph );
  void Measure(void);
  void AddData(DAUValues data);
  MeasureInParam m_InParam ; // not changed in Measure()
  MeasureOutParam m_OutParam ;
  //CAcquDriver m_AcquDriver ;
  DAU *m_pSource ;
  CGraphView *m_pGraph ;
  int m_mode;   

private:
  CString m_Text, m_MsgCaption ;
  void GetNextData(AcquDriverOutParam& param);
    
  deque<AcquDriverOutParam> m_dauData;  
} ;
