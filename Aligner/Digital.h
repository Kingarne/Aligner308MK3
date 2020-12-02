// $Id: Digital.h,v 1.14 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#define DS_OK       0
#define DS_CRC_ERR  1

#include "SerialPort.h"
#include "Syncro.h"

struct HDLCFrameData 
{
    unsigned char Address;					// Don't care
    unsigned char MsgID;
    //char status;
    unsigned char DRU_Malfunction	:1;		// => Error : DRU malfunction
    unsigned char Simulation		:1;		// => Warning : Simulation data
    unsigned char Spare				:1;		
    unsigned char EM_Log_Available	:1;		
    unsigned char GPS_Available		:1;		
    unsigned char AttitudeValid		:1;		
    unsigned char Navigation		:1;		
    unsigned char Align				:1;		// OBS! MSb !!
    long HeadingAngle;				
    long RollAngle;
    long PitchAngle;
    short HeadingAngleRate;			
    short RollAngleRate;			
    short PitchAngleRate;			
};

class Digital : public Syncro
{
public:
    Digital( const CString &name, BOOL isAligner202 ) ;
    virtual ~Digital( void ) ;
public:
    void SetData( double roll, double pitch, double heading=0.0f, int status = DS_OK) ;
    void SetDataDeg( double roll, double pitch, double heading=0.0f, int status = DS_OK) ;
    BOOL SetCandidate();
  
  	void SetSyncroType( DigChTypePro type, bool force=false ) ;
    DigChTypePro GetSyncroType( void ) const {return m_syncroType;}
    CString GetDigTypeText( );

    UINT HandleDataFrame(DAUFrame &frame);
	BOOL CheckIfDataReceived();

	DigChTypePro m_syncroType ;
	BOOL m_CRCError;  
	int m_receivedFrames;
	BOOL m_dataReceptionOK;
	static BOOL m_checkMINSCRC;
    static BOOL m_checkSigma40CRC;    

private:
  
    void HandleSigma40_50(DAUFrame &frame);
	void HandleSigma40_03(DAUFrame &frame);
    void HandleSigma40_01(DAUFrame &frame);
    void HandleSigma40_NMEA(DAUFrame &frame );
    void HandleMINS(DAUFrame &frame );    
	void HandleMINSNMEA(DAUFrame &frame);
	void HandleIXSEA(DAUFrame &frame);
	void HandleMSI(DAUFrame &frame );
	void HandlePL40(DAUFrame &frame );
	void HandleSperryMk39M3(DAUFrame &frame);

    BOOL ParseNMEA(vector<char>& frame, double& roll, double& pitch, double& heading, BOOL& checksumOK);
	BOOL ParseMINSNMEA(vector<char>& frame, double& roll, double& pitch, double& heading, BOOL& checksumOK);
    BOOL ParseMSI(vector<char>& frame, double& roll, double& pitch, double& heading);
	void DumpHDLCFrame(HDLCFrameData *frameData);
    void DumpDigFrame(BYTE* pData, int len, int crc);

private:
     	
	double m_rollCandidate ;
	double m_pitchCandidate ;
    double m_headingCandidate ;
    int m_statusCandidate;
    BOOL m_isAligner202 ;
    CString m_dir;

    CString m_dumpFile;
    int m_dumpFileNum;
} ;


