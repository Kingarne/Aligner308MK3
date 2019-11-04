// $Id: SystemSetup.cpp,v 1.18 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "SystemSetup.h"
#include "DAU.h"
#include "Aligner.h"
#include <sstream>
#include "SystemSetupDlg.h"
#include "SystemSetupCalibDlg.h"

SystemSetup* SystemSetup::m_ptInstance = NULL;
// SystemSetup::Data SystemSetup::m_proj ;
// map<int, ValidSyncroInfo> SystemSetup::m_validSyncroTypes;
// int SystemSetup::m_featureMask = 0 ;
// int SystemSetup::m_currentChannelSetup = 0 ;
// int SystemSetup::m_regChannelMask = 0;
// long SystemSetup::m_imageFileIndex = 0;
// CString SystemSetup::m_projectPath = "";

/**
 *  @brief Wrapper to handle the System Setup dialog.
 *
 *  SystemSetupDialog is a (mostly) wizard generated class to handle interaction
 *  with the System Setup menu accessed via the main application menu.
 */

SystemSetup::SystemSetup( void )
{
    SetImageFileIndex(0); 
    m_refVoltageMap[A1] = 26.0f;
    m_refVoltageMap[B1] = 26.0f;
    m_refVoltageMap[C1] = 26.0f;
    m_refVoltageMap[A2] = 115.0f;
    m_refVoltageMap[B2] = 115.0f;
    m_refVoltageMap[C2] = 115.0f;
    
} 

SystemSetup::~SystemSetup(void)
{
    if(m_ptInstance != NULL)
        delete m_ptInstance;
}


SystemSetup* SystemSetup::Instance()
{	
    if (m_ptInstance != NULL)
    {
        return(m_ptInstance);
    }

    if ((m_ptInstance = new SystemSetup) == NULL)
    {
        return NULL;
    }

    return m_ptInstance;
}

BOOL SystemSetup::DoModal(int mode)
{
	int result = IDCANCEL;
	if (mode == SYSTEM_SETUP_MODE_ALIGNMENT)
	{
		SystemSetupDialog dialog(m_proj);
		dialog.m_folder = m_projectPath;
		if ((result = dialog.DoModal()) == IDOK)
		{
			m_proj = dialog.m_proj;
			m_projectPath = dialog.m_folder;
		}
	}
	else if (mode == SYSTEM_SETUP_MODE_CALIBRATION)
	{
		SystemSetupCalibDialog dialog(m_proj);
		dialog.m_folder = m_projectPath;
		if ((result = dialog.DoModal()) == IDOK)
		{
			m_proj = dialog.m_proj;
			m_projectPath = dialog.m_folder;
		}
	}

	if (result == IDOK)
	{
		m_proj.m_dauSerial = DAU::GetDAU().m_mainBoardRegsPro.m_dauSerialNo;
			
		SaveToRegistry();
		SaveLatitudeToRegistry();
		if (GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT)
		{
			SaveUnitsToRegistry();
		}
		SaveProjectPathToRegistry();
	}
  
	return result == IDOK ;
}

int SystemSetup::GetNumMeasurements()
{
	return m_proj.m_numMeasurements;
}

bool SystemSetup::IsOpen()
{
	return (m_proj.m_projectID > 0);
}

void SystemSetup::LoadLatitudeFromRegistry( void )
{
    Registry reg;
    CString lat = reg.GetStringValue("Latitude", "59.0");
    m_proj.m_latitude = atof(lat);
}

void SystemSetup::LoadUnitsFromRegistry( void )
{
  Registry reg;
  int registryValue = reg.GetIntegerValue("Units", 0);

  SetUnits( registryValue ) ;
}

void SystemSetup::LoadSignFromRegistry(void)
{
	Registry reg;
	CString signdefText = reg.GetStringValue("SignDef", "0");//::AfxGetApp() -> GetProfileString( SIGNDEF_REGISTER_SECTION, SIGNDEF_REGISTER_NAME, SIGNDEF_DEFAULT_VALUE ) ;
	int def = atoi(signdefText) ? -1 : 1;
	SetSignDef(def);
}


void SystemSetup::LoadProjectFromRegistry()
{
	LoadDigitalSyncroTypesFromRegistry();
	LoadChannelsFromRegistry();
	LoadProjectPathFromRegistry();
	LoadLatitudeFromRegistry();
	LoadUnitsFromRegistry();
	LoadFromRegistry();
	LoadSignFromRegistry();
}

void SystemSetup::LoadFromRegistry() 
{
    Registry reg;
    CString str = reg.GetStringValue("Project\\Operator", "");
    SetOperatorName(str);
    str = reg.GetStringValue("Project\\Place", "");
    SetPlace(str);
    str = reg.GetStringValue("Project\\Ship", "");
	Ship ship;
	if (DBInterface::Instance()->GetShip(str, ship))
	{
		m_proj.m_shipName = str;
		m_proj.m_shipClass = ship.m_classID;
		m_proj.m_shipID = ship.m_ID;
	}

	

}

void SystemSetup::SaveToRegistry() 
{
    Registry reg;
    reg.SetStringValue("Project\\Operator", m_proj.m_operatorName);
    reg.SetStringValue("Project\\Place", m_proj.m_location);
    reg.SetStringValue("Project\\Ship", m_proj.m_shipName);    
}

void SystemSetup::LoadChannelsFromRegistry( void )
{
    Registry reg;
    m_regChannelMask = atoi(reg.GetStringValue("Setup\\ChannelMask", ""));        
}

void SystemSetup::LoadDigitalSyncroTypesFromRegistry( void )
{
  Registry reg;
  m_featureMask = atoi(reg.GetStringValue("Setup\\FeatureMask", ""));
        
  DAU::GetDAU().SetEnableSperryMeasurement( m_featureMask & FEATURE_MASK_SPERRY_MARK_29 ? TRUE : FALSE ) ;
  DAU::GetDAU().SetEnableAzimuthMeasurement( m_featureMask & FEATURE_MASK_HIGH_PERFORMANCE ? TRUE : FALSE ) ;

  CString str;
  ValidSyncroInfo info;
  m_validSyncroTypes.clear();
  if( IsDigitalSyncroTypeEnabled( FEATURE_MASK_NMEA ) == TRUE )
  {
      str.LoadString( IDS_SEAPATH );
      info.name = str;
      info.type = DigChTypeProSeapath;      
      info.serialType = DigChSerialTypeProUart;
      m_validSyncroTypes[info.type] = info;//.push_back(info);
  }
  if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_SIGMA_40_ID3 ) == TRUE )
  {
      str.LoadString( IDS_SIGMA_40_STD );
      info.name = str;
      info.type = DigChTypeProSigma40_03; 
      info.serialType = DigChSerialTypeProUart;
      info.bits = 8;
      info.parity = 1;
      info.rate = 38400;
      m_validSyncroTypes[info.type] = info;
  }
  if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_SIGMA_40_NMEA ) == TRUE )
  {
      str.LoadString( IDS_SIGMA_40_NMEA );
      info.name = str;
      info.type = DigChTypeProSigma40_NMEA;  
      info.serialType = DigChSerialTypeProUart;
      info.bits = 8;
      info.parity = 0;
      info.rate = 57600;
      m_validSyncroTypes[info.type] = info;
  }

  if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_SIGMA_40_ID1 ) == TRUE )
  {
      str.LoadString( IDS_SIGMA_40_01 );
      info.name = str;
      info.type = DigChTypeProSigma40_01;    
      info.serialType = DigChSerialTypeProUart;
      info.bits = 8;
      info.parity = 1;
      info.rate = 38400;
      m_validSyncroTypes[info.type] = info;
  }

  if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_MINS ) == TRUE )
  {
      str.LoadString( IDS_MINS );
      info.name = str;
      info.type = DigChTypeProMins;            
      info.serialType = DigChSerialTypeProHdlc;
      m_validSyncroTypes[info.type] = info;
  }

  if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_IXSEA ) == TRUE )
  {
      str.LoadString( IDS_IXSEA );
      info.name = str;
      info.type = DigChTypeProIXSEA;            
      info.serialType = DigChSerialTypeProUart;
      info.bits = 8;
      info.parity = 1;
      info.rate = 115200;
      m_validSyncroTypes[info.type] = info;
  }
   if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_MSI ) == TRUE )
  {
      str.LoadString( IDS_MSI );
      info.name = str;
      info.type = DigChTypeProMSI;            
      info.serialType = DigChSerialTypeProUart;
      info.bits = 8;
      info.parity = 1;
      info.rate = 38400;
      m_validSyncroTypes[info.type] = info;
  }
   if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_PL40 ) == TRUE )
   {
	   str.LoadString( IDS_PL40 );
	   info.name = str;
	   info.type = DigChTypeProPL40;            
	   info.serialType = DigChSerialTypeProUart;
	   info.bits = 8;
	   info.parity = 1;
	   info.rate = 115200;
	   m_validSyncroTypes[info.type] = info;
   }
   if( SystemSetup::IsDigitalSyncroTypeEnabled( FEATURE_MASK_SIGMA_40_ID50 ) == TRUE )
   {
	   str.LoadString( IDS_SIGMA_40_50 );
	   info.name = str;
	   info.type = DigChTypeProSigma40_50;            
	   info.serialType = DigChSerialTypeProUart;
	   info.bits = 8;
	   info.parity = 1;
	   info.rate = 38400;
	   m_validSyncroTypes[info.type] = info;
   }
   if (SystemSetup::IsDigitalSyncroTypeEnabled(FEATURE_MASK_MINS_NMEA) == TRUE)
   {
	   str.LoadString(IDS_MINS_NMEA);
	   info.name = str;
	   info.type = DigChTypeProMinsNMEA;
	   info.serialType = DigChSerialTypeProUart;
	   info.bits = 8;
	   info.parity = 1;
	   info.rate = 38400;
	   m_validSyncroTypes[info.type] = info;
   }

    ReadDigitalSettings();

	Digital::m_checkMINSCRC = (reg.GetIntegerValue("Setup\\CheckMINSCRC", 0) != 0);
	Digital::m_checkSigma40CRC = (reg.GetIntegerValue("Setup\\CheckSigma40CRC", 1) != 0);	
}


void SystemSetup::ReadDigitalSettings()
{
    Registry reg;
    vector<CString> settings;
    CString entry;

    //Setup default
    map<int, ValidSyncroInfo>::iterator it;
    for(it = m_validSyncroTypes.begin();it!=m_validSyncroTypes.end();it++)
    {
        bool isUART = it->second.serialType == DigChSerialTypeProUart;
        entry.Format("%d %d %d %d %d %s",it->second.type, it->second.serialType, isUART ? 38400 : 2, 8, 1, it->second.name);
        settings.push_back(entry);
    }        

    settings = reg.GetMultiStringValue("Setup\\Digital", settings );
    int type, serialType, rate, bits, parity;    
    ValidSyncroInfo syncro;
    string name;
    stringstream ss;
    vector<CString>::iterator iter;
    for(iter = settings.begin() ; iter!= settings.end() ; ++iter)
    {
        TRACE("%s\n",*iter);
        std::stringstream ss((LPCTSTR)*iter, std::stringstream::in);
        
        ss >> type;
        ss >> serialType;
        ss >> rate;
        ss >> bits;
        ss >> parity;

        if(m_validSyncroTypes.find(type) != m_validSyncroTypes.end())
        {
            m_validSyncroTypes[type].rate = rate;
            m_validSyncroTypes[type].bits = bits;
            m_validSyncroTypes[type].parity = parity;
        }
      //  m_colorSettings.push_back(color);
    }
}

BOOL SystemSetup::IsDigitalSyncroTypeEnabled( int syncroTypeMask )
{
  return m_featureMask & syncroTypeMask ? TRUE : FALSE ;
}

void SystemSetup::ClearUnits( void )
{
  m_currentChannelSetup = 0 ;
}

static int Mask( int offset )
{
  int mask = 1 ;
  for (int i = 0 ; i < offset ; i++)
  {
    mask *= 2 ;
  }
  return mask ;
}

void SystemSetup::AddSyncro( int offset )
{
  m_currentChannelSetup |= 0x01<<offset;//Mask( offset + 3 ) ;
}

void SystemSetup::AddSensor( int offset )
{
  m_currentChannelSetup |= 0x01<<(offset+2) ;
}

BOOL SystemSetup::IsValid( void )
{
#ifdef R2_NO_SOFTWARE_CHECK
  return TRUE;
#else
    
    //Registry must consist with DB.
    if(m_currentChannelSetup != m_regChannelMask)
    {
        return FALSE;
    }
    
    Registry reg;
    int cryptoKey = atoi(reg.GetStringValue("Setup\\CryptoKey", ""));

    int val = abs(((m_currentChannelSetup + 33) - (m_featureMask + 101))) * 7;

    return (cryptoKey == val);
#endif
}

void SystemSetup::SaveLatitudeToRegistry( void )
{
  double latitude = m_proj.m_latitude ;
  Sensor::SetLatitudeCompensation( latitude ) ;
  Registry reg;
  CString lat;
  lat.Format("%f",latitude);
  reg.SetStringValue("Latitude", lat);
//   if (!::AfxGetApp() -> WriteProfileBinary( LATITUDE_REGISTER_SECTION, LATITUDE_REGISTER_NAME, reinterpret_cast<BYTE *>(&latitude), sizeof(latitude) ))
//   {
//     ASSERT(0) ;
//   }
}

void SystemSetup::SaveUnitsToRegistry( void )
{
  Registry reg;
  reg.SetIntegerValue("Units", GetUnits());
    //::AfxGetApp() -> WriteProfileInt( UNITS_REGISTER_SECTION, UNITS_REGISTER_NAME, GetUnits() ) ;
}

 void SystemSetup::LoadProjectPathFromRegistry( void )
{
    Registry reg;
    m_projectPath = reg.GetStringValue("ProjectPath", "C:\\").TrimRight("\\");//::AfxGetApp() -> GetProfileString( PROJECT_PATH_REGISTER_SECTION, PROJECT_PATH_REGISTER_NAME, PROJECT_PATH_DEFAULT_VALUE ) ;

	if (!::CreateDirectory(m_projectPath, NULL))
	{
		int err = GetLastError();
		if (err != ERROR_ALREADY_EXISTS)
		{
			::AfxMessageBox(_T("Unable to create project directory"));
		}		
	}
 }

void SystemSetup::SaveProjectPathToRegistry( void )
{
  Registry reg;
  reg.SetStringValue("ProjectPath", m_projectPath);//  ::AfxGetApp() -> WriteProfileString( PROJECT_PATH_REGISTER_SECTION, PROJECT_PATH_REGISTER_NAME, m_projectPath ) ;
}

CString SystemSetup::GetProjectPath( void )
{
    return m_projectPath + _T("\\") + m_proj.m_projectName ;
}

CString SystemSetup::GetProjectPathBase( void )
{
    return m_projectPath ;
}

int SystemSetup::GetProjectID(void)
{
	return m_proj.m_projectID;
}

CString SystemSetup::GetProjectName( void )
{
    return m_proj.m_projectName ;
}

CString SystemSetup::GetProjectTime(void)
{
	COleDateTime time(m_proj.m_time);
	return time.Format(_T("%Y-%m-%d %H:%M:%S"));
}


CString SystemSetup::GetOperatorName( void )
{
  return m_proj.m_operatorName ;
}

CString SystemSetup::GetShipName( void )
{
  return m_proj.m_shipName ;
}

double SystemSetup::GetLatitude( void )
{
  return m_proj.m_latitude ;
}

int SystemSetup::GetMode( void )
{
  return m_proj.m_mode ;
}

void SystemSetup::SetMode( int mode )
{
	m_proj.m_mode = mode ;
}

CString SystemSetup::GetPlace( void )
{
  return m_proj.m_location ;
}

int SystemSetup::GetUnits( void )
{
  return m_proj.m_unit ;
}

CString SystemSetup::GetUnitText()
{
    return (m_proj.m_unit == UNIT_MRAD) ? "mrad" : "arcmin";
}

BOOL SystemSetup::GetCanConfigure( void )
{
  return SYSTEM_SETUP_NO_MODE != m_proj.m_mode ;
}

void SystemSetup::SetUnits( int units )
{
	m_proj.m_unit = units;
}

void SystemSetup::SetSignDef( int sign )
{
	m_proj.m_signDef = sign;
	g_AlignerData.SignDef = sign;
}

int SystemSetup::GetSignDef()
{
	return m_proj.m_signDef;
}

long SystemSetup::GetImageFileIndex( void )
{
	return m_proj.m_imgIdx;
}

void SystemSetup::SetImageFileIndex(const long index)
{
	m_proj.m_imgIdx = index;
}

void SystemSetup::SetDAUSerial(int serial)
{
	m_proj.m_dauSerial = serial;
}

int SystemSetup::GetDAUSerial()
{
	return m_proj.m_dauSerial;
}

CString SystemSetup::GetDAUSerialString()
{
	CString str;
	str.Format("%03d", m_proj.m_dauSerial);
	return str;
}

CString SystemSetup::GetNewImageFileIndexString( void )
{
    m_proj.m_imgIdx++;
    CString str;
    str.Format( _T("%d"), m_proj.m_imgIdx);
	return str ;
}

CString SystemSetup::GetExecutablePath( void ) 
{
	Registry reg;
    CString S1 ;
	S1 = reg.GetStringValue("Directory", "");//AfxGetApp() -> GetProfileString("", "Directory", "") ;
	return S1 ;
}


void SystemSetup::SetOperatorName( const CString name )
{
  m_proj.m_operatorName = name ;
}

void SystemSetup::SetShipName( const CString name )
{
  m_proj.m_shipName = name ;
}

void SystemSetup::SetPlace( const CString name )
{
  m_proj.m_location = name ;
}

void SystemSetup::SetProjectID(int id)
{
	m_proj.m_projectID = id;
}

void SystemSetup::SetProjectName( const CString name )
{
  m_proj.m_projectName = name ;
}

void SystemSetup::SetProjectPath( const CString name )
{
  m_projectPath = name ;
}

void SystemSetup::SetLatitude( const double latitude )
{
    m_proj.m_latitude = latitude ;
    SaveLatitudeToRegistry() ;
}


double SystemSetup::GetRefVoltage(CString name)
{
    if(m_refVoltageMap.find(name) == m_refVoltageMap.end())
        return 0.0f;

    return m_refVoltageMap[name];

}

CString SystemSetup::GetConfigXML()
{
	return m_proj.m_config;
}

void SystemSetup::UpdateConfig(CString xml)
{	
	DBInterface::Instance()->UpdateProjectConfig(m_proj.m_projectID, xml, GetMode());
}
