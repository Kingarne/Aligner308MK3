// $Id: CalibInfo.cpp,v 1.2 2014-02-04 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "CalibInfo.h"
#include "DAU.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CalibrationInfo::CalibrationInfo()
{
    
}


void CalibrationInfo::AddChannel(CString ch)
{
    if(ch!="")
        m_channels.push_back(ch);
}

void CalibrationInfo::AddSensor(CString se)
{
    if(se!="")
        m_sensors.push_back(se);
}

void CalibrationInfo::AddAdapter(CString ad)
{
    if(ad!="")
        m_adapters.push_back(ad);
}

BOOL CalibrationInfo::CalibrationTimeValid(DBTIMESTAMP time)
{
    COleDateTime t(time);
    COleDateTime calibTime(m_time);
    
   
    COleDateTimeSpan ts = calibTime-t; 

    TRACE("time:%d\n",ts.GetDays());   
    if(ts.GetDays() > CAL_TIME_LIMIT)
        return FALSE;

   
    return TRUE;
}

CString CalibrationInfo::GetInfo()
{
    CString info, str;

    int id = DAU::GetDAU().GetDAUDBId();
    
    vector<CString>::iterator chIter;
    for(chIter=m_channels.begin() ; chIter!=m_channels.end() ; chIter++)
    {
        str="";
        DBTIMESTAMP time;
        if(DBInterface::Instance()->GetChannelCalibTime(*chIter, id, time))
        {
            if(!CalibrationTimeValid(time))
            {
                str.Format("Expired calibration for channel: %s\r\n",*chIter);
            }
            TRACE("Ch time:%d,%d\n",time.year,time.second);                                    
        }
        else
        {
            str.Format("Channel %s is not calibrated\r\n",*chIter);
        }
        info += str;       
    }

    vector<CString>::iterator seIter;
    for(seIter=m_sensors.begin() ; seIter!=m_sensors.end() ; seIter++)
    {
        str="";
        DBTIMESTAMP time;
        if(DBInterface::Instance()->GetSensorCalibTime(*seIter, time))
        {
            if(!CalibrationTimeValid(time))
            {
                str.Format("Expired calibration for sensor S/N: %s\r\n",*seIter);
            }
            TRACE("Ch time:%d,%d\n",time.year,time.second);                                    
        }
        else
        {
            str.Format("Sensor S/N %s is not calibrated\r\n",*seIter);
        }
        info += str;      
    }

    vector<CString>::iterator adIter;
    for(adIter=m_adapters.begin() ; adIter!=m_adapters.end() ; adIter++)
    {
        str="";
        DBTIMESTAMP time;
        if(DBInterface::Instance()->GetAdapterCalibTime(*adIter, time))
        {
            if(!CalibrationTimeValid(time))
            {
                str.Format("Expired calibration for adapter S/N: %s\r\n",*adIter);
            }
            TRACE("Ch time:%d,%d\n",time.year,time.second);                                    
        }
        else
        {
            str.Format("Adapter S/N %s is not calibrated\r\n",*adIter);
        }
        info += str;      
            
    }

    if(info.GetLength() > 254)
    {
        info = info.Left(245) + "...more";
    }


    return info;
}

