#include "stdafx.h"
#include "DataCompensator.h"
#include "DAU.h"
#include "Util.h"
#include "math.h"

double DervFilter::Add(double val)
{    
    if(m_first)
    {
        m_U = val;
        m_first=FALSE;
        return 0.0f;
    }
    double Y = m_K*(val-m_U);
    m_U = m_U + m_ts*Y;
    return Y;
}

DataCompensator::DataCompensator()
{
	//temp
	m_rollComp = 0.0f;
	m_pitchComp = 0.0f;
    m_active = FALSE;
    m_enable = TRUE;        
    m_pSyncroRef = NO_REF; 
    m_pSensorRef1 = NO_REF;
    m_pSensorRef2 = NO_REF;
    m_maxDiff = 0.04;
    
}

void DataCompensator::SetEnable(BOOL b)
{
    for(int i=0;i<DAU::GetDAU().GetSensorCount();i++)
    {
        Sensor* pSensor = DAU::GetDAU().GetSensor(i);
        pSensor->SetCentrifugPitchComp(0.0f);
        pSensor->SetCentrifugRollComp(0.0f);         
    }
    m_enable = b;
}

DAUValues DataCompensator::GetDAUData(DAU *pDau )
{
	DAUValues newValues( *pDau) ;
	
    if(!m_active || !m_enable)
        return newValues;

    CalcCompensation();

    //newValues.CompensateSensorData(m_rollComp, m_pitchComp);
	

	return newValues;
}


BOOL DataCompensator::Init()
{
    if(!m_enable)
        return FALSE;

    for(int i=0;i<DAU::GetDAU().GetSensorCount();i++)
    {
        Sensor* pSensor = DAU::GetDAU().GetSensor(i);
        pSensor->SetCentrifugPitchComp(0.0f);
        pSensor->SetCentrifugRollComp(0.0f);         
    }

    m_pSyncroRef = DAU::GetDAU().GetHighSeaCompGyro();            
    if(!m_pSyncroRef)
    {
        m_active = FALSE;
        return FALSE;
    }
    
    
    m_active = TRUE;
    m_pSensorRef1 = m_pSensorRef2 = NO_REF;
    
    m_pitchFilter.SetPeriod(DAU::GetDAU().GetPeriod());
    m_pitchFilter.Reset();
    m_rollFilter.SetPeriod(DAU::GetDAU().GetPeriod());
    m_rollFilter.Reset();

    //Decide which source(s) to be reference.
//     if(syncros.size() > 0)
//     {
//         //We have a syncro as ref.
//         m_pSyncroRef = DAU::GetDAU().GetSyncro(CONVERT_ARRAY_INDEX_TO_SYNCRO_CHANNEL(syncros[0]));
//         TRACE("Syncro Ref:%s\n",m_pSyncroRef->GetName());
//         m_active = TRUE;
//     }
//     else
//     {
//         //We need two sensors as ref.
//         if(sensors.size() <= 1)
//         {
//             return FALSE;
//         }
//                 
//         double minZparallax=DBL_MAX, maxZparallax = DBL_MIN;
//         for(iter=sources.begin();iter!=sources.end();iter++)
//         {
//             Sensor* pSensor = DAU::GetDAU().GetSensor(CONVERT_ARRAY_INDEX_TO_SENSOR_CHANNEL(*iter));    
//             double z = pSensor->GetParallaxData().z;
//             if( z < minZparallax)
//             {
//                 m_pSensorRef1 = pSensor;             
//                 minZparallax = z;
//             }
// 
//             if( z > maxZparallax)
//             {
//                 m_pSensorRef2 = pSensor;                         
//                 maxZparallax = z;
//             }
//         }
// 
//         if(m_pSensorRef1 == m_pSensorRef2 || (maxZparallax - minZparallax) < 0.1f)
//         {
//             m_pSensorRef1 = m_pSensorRef2 = NO_REF;
//             return FALSE;
//         }
//     }


  

    //     
//     Syncro* pRefSyncro = DAU::GetDAU().GetFirstSelectedSyncro();
//     if(pRefSyncro)
//         TRACE("pRef:%d",pRefSyncro->GetSerialNumber());

    return TRUE;
}

void DataCompensator::CalcCompensation()
{
    if(m_pSyncroRef != NO_REF)
    {
        double P = m_pitchFilter.Add(m_pSyncroRef->GetUnfilteredPitch());
        double P2 = pow(P, 2);
        double R = m_rollFilter.Add(m_pSyncroRef->GetUnfilteredRoll());
        double R2 = pow(R, 2);

        //TRACE("P2:%.2f, R2:%.2f\n",P2,R2);

        for(int i=0;i<DAU::GetDAU().GetSensorCount();i++)
        {
            Sensor* pSensor = DAU::GetDAU().GetSensor(i);
            pSensor->SetCentrifugPitchComp(-P2);
            pSensor->SetCentrifugRollComp(R2);            
       
            if(P2 > m_maxDiff || R2 > m_maxDiff)
            {
                DAU::GetDAU().SendInvalidGyroDataMsg();
            }

            TRACE("R2:%.3f, P2:%.3f\n",P2, R2);
//             if(pSensor->GetName() == "J3")
//                 TRACE("orgP:%.5f, orgR:%.5f, compP:%.5f, compR:%.5f, diffP:%f, diffR:%f\n",values.GetSensor(i).GetPitch(),values.GetSensor(i).GetRoll(),
//                     pitchCompVal, rollCompVal,values.GetSensor(i).GetPitch()-pitchCompVal,values.GetSensor(i).GetRoll()-rollCompVal);
       
            //values.GetSensor(i).SetPitch(pitchCompVal);
            //values.GetSensor(i).SetRoll(rollCompVal);

        }

    }


}



