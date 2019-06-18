// $Id:
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once



class CalibrationInfo
{
public:
    CalibrationInfo();
    void SetCalibrationTime(DBTIMESTAMP time){m_time=time;}
    void AddChannel(CString ch);
    void AddSensor(CString se);
    void AddAdapter(CString ad);

    CString GetInfo();

protected:
    
    DBTIMESTAMP m_time;    
    vector<CString> m_channels;
    vector<CString> m_sensors;
    vector<CString> m_adapters;

    int m_timeLimitDays;

    BOOL CalibrationTimeValid(DBTIMESTAMP time);

};

