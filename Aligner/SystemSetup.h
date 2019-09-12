// $Id: SystemSetup.h,v 1.9 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "AlignerControls.h"

#define BENGT_CRYPTO_1      33
#define BENGT_CRYPTO_2      17
#define BENGT_CRYPTO_3      29
#define BENGT_CRYPTO_4      7

#define FEATURE_MASK_NMEA               0x0001
#define FEATURE_MASK_SIGMA_40_ID1       0x0002
#define FEATURE_MASK_SIGMA_40_ID3       0x0004
#define FEATURE_MASK_SIGMA_40_NMEA      0x0008
#define FEATURE_MASK_SPERRY_MARK_29     0x0010
#define FEATURE_MASK_HIGH_PERFORMANCE   0x0020
#define FEATURE_MASK_MINS			    0x0040
#define FEATURE_MASK_IXSEA			    0x0080
#define FEATURE_MASK_MSI			    0x0100
#define FEATURE_MASK_PL40			    0x0200
#define FEATURE_MASK_SIGMA_40_ID50	    0x0400
#define FEATURE_MASK_MINS_NMEA  	    0x0800

#define SYSTEM_SETUP_NO_MODE            0
#define SYSTEM_SETUP_MODE_CALIBRATION   1
#define SYSTEM_SETUP_MODE_ALIGNMENT     2

#define UNIT_MRAD   0
#define UNIT_ARCMIN 1

#define SysSetup SystemSetup::Instance()

/**
 *  @brief SystemSetup handles central setup issues.
 *
 *  SystemSetup is a static class acting namespace for application wide setup issues.
 *  ProjectData defines information that is required throughout the application,
 *  e.g., the name of the operator.
 *
 *  SystemSetup should never be instanciated, the private constructor ensures this. All
 *  usage of SystemSetup is through the static methods declared below.
 */
struct ValidSyncroInfo
{
    CString name;
    DigChTypePro type;
    DigChSerialTypePro serialType;
    int rate;
    int bits;
    int parity;
};

class ProjectData
{
public:

	ProjectData(void) : m_projectName(""), m_operatorName(""), m_shipName(""), m_latitude(0.0), m_mode(SYSTEM_SETUP_NO_MODE), m_place(""), m_units(0) 
	{

	};
	
public:
	int m_projectID;
	CString m_projectName;
	DBTIMESTAMP m_time;
	CString m_operatorName;
	CString m_shipName;
	int m_shipClass;
	CString m_place;
	double m_latitude;
	int m_units;
	int m_signDef;
	int m_mode;
};

class SystemSetup
{
private:
    SystemSetup( void );
    SystemSetup::~SystemSetup(void);
public:
	/**
	*  @brief Encapsulation of System Setup data.
	*
	*  SystemSetup::Data encapsulates dialog variables - This is done so that setup of
	*  dialog/data can be done with a single assignment.
	*/
	
public:
    static SystemSetup* Instance();

	void SetProject(ProjectData data) { m_data = data; }
	ProjectData GetProject() { return m_data; }

	BOOL DoModal( void ) ;
	void LoadFromRegistry() ;
	void SaveToRegistry() ;
	void LoadLatitudeFromRegistry( void ) ;
	void SaveLatitudeToRegistry( void ) ;
	void LoadUnitsFromRegistry( void ) ;
	void SaveUnitsToRegistry( void ) ;
	void LoadDigitalSyncroTypesFromRegistry( void ) ;
	void LoadChannelsFromRegistry( void ) ;
	BOOL IsDigitalSyncroTypeEnabled( int syncroTypeMask ) ;
	void ClearUnits( void ) ;
	void AddSyncro( int offset ) ;
	void AddSensor( int offset ) ;
	BOOL IsValid( void ) ;
	void LoadProjectPathFromRegistry( void ) ;
	void SaveProjectPathToRegistry( void ) ;
	CString GetOperatorName( void ) ;
	CString GetShipName( void ) ;
	CString GetProjectName( void ) ;
	int GetProjectID(void);
	CString GetProjectPath( void ) ;
	CString GetProjectPath2( void ) ;
	CString GetPlace( void ) ;
	void SetOperatorName( const CString name ) ;
	void SetShipName( const CString name ) ;
	void SetProjectName( const CString name ) ;
	void SetProjectPath( const CString name ) ;
	void SetPlace( const CString name ) ;
	void SetLatitude( const double latitude ) ;
	double GetLatitude( void ) ;
	double GetLatitudeCompensation( void ) ;
	int GetMode( void ) ;
	int GetUnits( void ) ;
	BOOL GetCanConfigure( void ) ;
	void SetUnits( int units ) ;
	CString GetUnitText() ;
	void SetMode( int mode ) ;
	void SetImageFileIndex( const long index ) ;
	long GetImageFileIndex( void ) ;
	CString GetNewImageFileIndexString( void ) ;
	CString GetExecutablePath( void ) ;
	map<int, ValidSyncroInfo> m_validSyncroTypes;
	double GetRefVoltage(CString name);   

private:

	ProjectData m_data ;
    CString m_projectPath ;
    int m_featureMask ;  
    int m_currentChannelSetup ;  
    int m_regChannelMask;
    long m_imageFileIndex ;
    map<CString, double> m_refVoltageMap;

    void ReadDigitalSettings();  
  
protected:

  static SystemSetup* m_ptInstance;
} ;
