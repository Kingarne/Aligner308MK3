// $Id: SystemSetup.cpp,v 1.18 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "SystemSetup.h"
#include "DAU.h"
#include "Aligner.h"
#include <sstream>

SystemSetup* SystemSetup::m_ptInstance = NULL;
// SystemSetup::Data SystemSetup::m_data ;
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
class SystemSetupDialog : public CDialog
{
public:
    SystemSetupDialog( SystemSetup::Data &dataSource, CWnd *pParent = NULL ) ;
	virtual ~SystemSetupDialog( void ) ;

    SystemSetup::Data m_data ;
    WhiteBackgroundEdit m_date ;

protected:
	enum { IDD = IDD_SYSTEM_SETUP } ;

	virtual void DoDataExchange( CDataExchange *pDX ) ;
    virtual BOOL OnInitDialog( void ) ;
    void HandleNewMode( void ) ;
	void HandleTextChanged( void ) ;

    afx_msg void OnCbnSelchangeSystemSetupDialogMode( void ) ;
    afx_msg void OnCbnSelchangeShipName( void ) ;    
    afx_msg void OnTextChanged( void ) ;
    afx_msg void OnBnClickedBrowseButton( void ) ;


    DECLARE_MESSAGE_MAP()
    DECLARE_DYNAMIC(SystemSetupDialog)

    void UpdateShips();
    CComboBox m_shipCombo;
} ;

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

BOOL SystemSetup::DoModal( void )
{
    LoadLatitudeFromRegistry() ;
    LoadUnitsFromRegistry() ;
    LoadFromRegistry() ;
    SystemSetupDialog dialog( m_data ) ;
    if (IDOK == dialog.DoModal())
    {
    m_data = dialog.m_data ;
    SaveToRegistry() ;
    SaveLatitudeToRegistry() ;
    if (SYSTEM_SETUP_MODE_ALIGNMENT == GetMode())
    {
      SaveUnitsToRegistry() ;
    }		
		SaveProjectPathToRegistry() ;			
		return TRUE ;
  }
  
  return FALSE ;
}


void SystemSetup::LoadLatitudeFromRegistry( void )
{
    Registry reg;
    CString lat = reg.GetStringValue("Latitude", "59.0");
    m_data.m_latitude = atof(lat);
}

void SystemSetup::LoadUnitsFromRegistry( void )
{
  Registry reg;
  int registryValue = reg.GetIntegerValue("Units", 0);

  SetUnits( registryValue ) ;
}

void SystemSetup::LoadFromRegistry() 
{
    Registry reg;
    CString str = reg.GetStringValue("Project\\Operator", "");
    SetOperatorName(str);
    str = reg.GetStringValue("Project\\Place", "");
    SetPlace(str);
    str = reg.GetStringValue("Project\\Ship", "");
    SetShipName(str);

}

void SystemSetup::SaveToRegistry() 
{
    Registry reg;
    reg.SetStringValue("Project\\Operator", m_data.m_operatorName);
    reg.SetStringValue("Project\\Place", m_data.m_place);
    reg.SetStringValue("Project\\Ship", m_data.m_shipName);    
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
  double latitude = m_data.m_latitude ;
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
}

void SystemSetup::SaveProjectPathToRegistry( void )
{
  Registry reg;
  reg.SetStringValue("ProjectPath", m_projectPath);//  ::AfxGetApp() -> WriteProfileString( PROJECT_PATH_REGISTER_SECTION, PROJECT_PATH_REGISTER_NAME, m_projectPath ) ;
}

CString SystemSetup::GetProjectPath( void )
{
    return m_projectPath + _T("\\") + m_data.m_projectName ;
}

CString SystemSetup::GetProjectPath2( void )
{
    return m_projectPath ;
}

CString SystemSetup::GetProjectName( void )
{
    return m_data.m_projectName ;
}

CString SystemSetup::GetOperatorName( void )
{
  return m_data.m_operatorName ;
}

CString SystemSetup::GetShipName( void )
{
  return m_data.m_shipName ;
}

double SystemSetup::GetLatitude( void )
{
  return m_data.m_latitude ;
}

int SystemSetup::GetMode( void )
{
  return m_data.m_mode ;
}

void SystemSetup::SetMode( int mode )
{
	m_data.m_mode = mode ;
}

CString SystemSetup::GetPlace( void )
{
  return m_data.m_place ;
}

int SystemSetup::GetUnits( void )
{
  return m_data.m_units ;
}

CString SystemSetup::GetUnitText()
{
    return (m_data.m_units == UNIT_MRAD) ? "mrad" : "arcmin";
}

BOOL SystemSetup::GetCanConfigure( void )
{
  return SYSTEM_SETUP_NO_MODE != m_data.m_mode ;
}

void SystemSetup::SetUnits( int units )
{
	m_data.m_units = units;
}

void SystemSetup::SetImageFileIndex( const long index )
{
	m_imageFileIndex = index ;
}

long SystemSetup::GetImageFileIndex( void )
{
	return m_imageFileIndex ;
}

CString SystemSetup::GetNewImageFileIndexString( void )
{
    m_imageFileIndex++;
    CString str;
    str.Format( _T("%d"), m_imageFileIndex );
	return str ;
}

CString SystemSetup::GetExecutablePath( void ) 
{
	Registry reg;
    CString S1 ;
	S1 = reg.GetStringValue("Directory", "");//AfxGetApp() -> GetProfileString("", "Directory", "") ;
	return S1 ;
}


IMPLEMENT_DYNAMIC(SystemSetupDialog, CDialog)

SystemSetupDialog::SystemSetupDialog( SystemSetup::Data &dataSource, CWnd *pParent )	: CDialog(SystemSetupDialog::IDD, pParent), m_data( dataSource )
{
  // Empty
}

SystemSetupDialog::~SystemSetupDialog( void )
{
  // Empty
}

void SystemSetupDialog::DoDataExchange( CDataExchange *pDX )
{
    CDialog::DoDataExchange( pDX ) ;
    if (pDX -> m_bSaveAndValidate)
    {
        CString text ;
        GetDlgItem( IDC_SYSTEM_SETUP_DIRECTORY ) -> GetWindowText( text ) ;
        SysSetup->SetProjectPath( text ) ;
    }
    else
    {
        SetDlgItemText( IDC_SYSTEM_SETUP_DIRECTORY, SysSetup->GetProjectPath2() ) ;
    }
    DDX_Text(pDX, IDC_SYSTEM_SETUP_PROJECT, m_data.m_projectName);
    DDV_MaxChars(pDX, m_data.m_projectName, 32);
    DDX_Text(pDX, IDC_SYSTEM_SETUP_NAME, m_data.m_operatorName);
    DDV_MaxChars(pDX, m_data.m_operatorName, 32);
    if(theApp.IsAligner202Enabled())
    {
        DDX_Text(pDX, IDC_SYSTEM_MEAS_OBJECT, m_data.m_shipName);
        DDV_MaxChars(pDX, m_data.m_shipName, 32);
    }
    DDX_Text(pDX, IDC_EDIT_LATITUDE, m_data.m_latitude);
    DDV_MinMaxDouble(pDX, m_data.m_latitude, -80, 80);
    DDX_CBIndex(pDX, IDC_SYSTEM_SETUP_MODE, m_data.m_mode);
    DDX_Control(pDX, IDC_SYSTEM_SETUP_DATE, m_date);
    DDX_Text(pDX, IDC_SYSTEM_SETUP_PLACE, m_data.m_place);
    DDX_CBIndex(pDX, IDC_SYSTEM_SETUP_UNITS, m_data.m_units) ;
    DDX_Control(pDX, IDC_SHIP_COMBO, m_shipCombo);
}

BEGIN_MESSAGE_MAP(SystemSetupDialog, CDialog)
    ON_CBN_SELCHANGE(IDC_SYSTEM_SETUP_MODE, OnCbnSelchangeSystemSetupDialogMode)
    ON_EN_CHANGE(IDC_SYSTEM_SETUP_NAME, OnTextChanged)
    ON_EN_CHANGE(IDC_SYSTEM_SETUP_PLACE, OnTextChanged)
    ON_EN_CHANGE(IDC_SYSTEM_SETUP_SHIP, OnTextChanged)
    ON_EN_CHANGE(IDC_SYSTEM_SETUP_PROJECT, OnTextChanged)
    ON_BN_CLICKED(IDC_SYSTEM_SETUP_BROWSE, OnBnClickedBrowseButton)
    ON_CBN_SELCHANGE(IDC_SHIP_COMBO, OnCbnSelchangeShipName)
END_MESSAGE_MAP()

void SystemSetupDialog::HandleTextChanged( void )
{
    CComboBox *pMode = static_cast<CComboBox *>(GetDlgItem( IDC_SYSTEM_SETUP_MODE )) ;
    CEdit *pProject = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_PROJECT )) ;
    CEdit *pMeasObject = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_MEAS_OBJECT )) ;
    CEdit *pName = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_NAME )) ;
    CEdit *pPlace = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_PLACE )) ;
    CButton *pOK = static_cast<CButton *>(GetDlgItem( IDOK )) ;
    CString text ;
    switch (pMode -> GetCurSel())
    {
        case SYSTEM_SETUP_NO_MODE:
            pOK -> EnableWindow( TRUE ) ;
        break ;

        case SYSTEM_SETUP_MODE_ALIGNMENT:
            pProject->GetWindowText( text ) ;
            if (0 == text.GetLength())
            {
                pOK -> EnableWindow( FALSE ) ;
                break ;
            }
            
            if( !theApp.IsAligner202Enabled())
            {
                if (m_shipCombo.GetCurSel() == CB_ERR)
                {
                    pOK->EnableWindow( FALSE ) ;
                    break ;
                }
            }
            
            
        case SYSTEM_SETUP_MODE_CALIBRATION:
            pName -> GetWindowText( text ) ;
            if (0 == text.GetLength())
            {
                pOK -> EnableWindow( FALSE ) ;
                break ;
            }
            pPlace -> GetWindowText( text ) ;
            if (0 == text.GetLength())
            {
                pOK -> EnableWindow( FALSE ) ;
                break ;
            }
            pOK -> EnableWindow( TRUE ) ;
            break ;

    default:
        ASSERT(0) ;
        break ;
    }
}

void SystemSetupDialog::HandleNewMode( void )
{
  CComboBox *pMode = static_cast<CComboBox *>(GetDlgItem( IDC_SYSTEM_SETUP_MODE )) ;
  CEdit *pProject = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_PROJECT )) ;
  CEdit *pMeasObject = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_MEAS_OBJECT )) ;
    
  CEdit *pName = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_NAME )) ;
  CEdit *pPlace = static_cast<CEdit *>(GetDlgItem( IDC_SYSTEM_SETUP_PLACE )) ;
  CComboBox *pUnits = static_cast<CComboBox *>(GetDlgItem( IDC_SYSTEM_SETUP_UNITS )) ;
  ASSERT(pMode && pMeasObject && pName && pUnits) ;
  switch (pMode -> GetCurSel())
  {
  case SYSTEM_SETUP_NO_MODE:
    pName -> SetReadOnly( TRUE ) ;
    pPlace -> SetReadOnly( TRUE ) ;
    pMeasObject -> SetReadOnly( TRUE ) ;
    m_shipCombo.EnableWindow(FALSE);
    pUnits -> SetCurSel( 0 ) ;
    pUnits -> EnableWindow( FALSE ) ;
    break ;

  case SYSTEM_SETUP_MODE_CALIBRATION:
    pProject -> SetWindowText( _T("Calibration") ) ;
    pProject -> SetReadOnly( TRUE ) ;
    pName -> SetReadOnly( FALSE ) ;
    pPlace -> SetReadOnly( FALSE ) ;
    pMeasObject -> SetReadOnly( TRUE ) ;
    m_shipCombo.EnableWindow(FALSE);
    pUnits -> SetCurSel( 0 ) ;
    pUnits -> EnableWindow( FALSE ) ;
    break ;

  case SYSTEM_SETUP_MODE_ALIGNMENT:
    pProject -> SetWindowText( _T("") ) ;
    pProject -> SetReadOnly( FALSE ) ;
    pName -> SetReadOnly( FALSE ) ;
    pPlace -> SetReadOnly( FALSE ) ;
    pMeasObject -> SetReadOnly( FALSE ) ;
    pUnits -> SetCurSel( m_data.m_units ) ;
    pUnits -> EnableWindow( TRUE ) ;
    m_shipCombo.EnableWindow(TRUE);
    break ;

  default:
    ASSERT(0) ;
    break ;
  }
  DAU::GetDAU().EnableErrorEvent( FALSE ) ;
  HandleTextChanged() ;
}

void SystemSetupDialog::OnCbnSelchangeShipName( void )
{
    CString str;
    int index;
    if((index = m_shipCombo.GetCurSel()) == CB_ERR)
        return;

    m_shipCombo.GetLBText(index, str);
    m_data.m_shipName = str;
    
    HandleTextChanged();
}

void SystemSetupDialog::OnCbnSelchangeSystemSetupDialogMode( void )
{
    HandleNewMode() ;
}

BOOL SystemSetupDialog::OnInitDialog( void )
{
    CDialog::OnInitDialog() ;

    if(theApp.IsAligner202Enabled())
    {
        GetDlgItem( IDC_LATITUDE_BOUNDARY )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_EDIT_LATITUDE )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_MODE_BOUNDARY )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_SYSTEM_SETUP_MODE )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_UNITS_BOUNDARY )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_SYSTEM_SETUP_UNITS )->ShowWindow( SW_HIDE );
        CString str;
        str.LoadString( IDS_OBJECT );
        SetDlgItemText( IDC_SHIP_BOUNDARY, str );
		CComboBox *pMode = static_cast<CComboBox *>(GetDlgItem( IDC_SYSTEM_SETUP_MODE )) ;
		pMode->SetCurSel( SYSTEM_SETUP_MODE_ALIGNMENT );
        
        m_shipCombo.ShowWindow(SW_HIDE);
        //m_data.m_shipName = "N/A";
        //UpdateShips();        
    }
    else
    {
        GetDlgItem( IDC_SYSTEM_MEAS_OBJECT )->ShowWindow( SW_HIDE );
        UpdateShips();        
    }
    m_date.SetWindowText( COleDateTime::GetCurrentTime().Format( VAR_DATEVALUEONLY ) ) ;
    HandleNewMode() ;
    return TRUE ;
}


void SystemSetupDialog::UpdateShips()
{    
    m_shipCombo.ResetContent() ;

    vector<CString> ships;
    DBInterface::Instance()->GetShips(ships); 
    vector<CString>::iterator iter;
    for(iter=ships.begin();iter!=ships.end();iter++)
    {
        int index = m_shipCombo.AddString(*iter) ;
    }

    m_shipCombo.SelectString(0, m_data.m_shipName);
}

void SystemSetupDialog::OnTextChanged( void )
{
  HandleTextChanged() ;
}

void SystemSetup::SetOperatorName( const CString name )
{
  m_data.m_operatorName = name ;
}

void SystemSetup::SetShipName( const CString name )
{
  m_data.m_shipName = name ;
}

void SystemSetup::SetPlace( const CString name )
{
  m_data.m_place = name ;
}

void SystemSetup::SetProjectName( const CString name )
{
  m_data.m_projectName = name ;
}

void SystemSetup::SetProjectPath( const CString name )
{
  m_projectPath = name ;
}

void SystemSetup::SetLatitude( const double latitude )
{
    m_data.m_latitude = latitude ;
    SaveLatitudeToRegistry() ;
}

void SystemSetupDialog::OnBnClickedBrowseButton( void )
{
    TCHAR path [MAX_PATH] ;
    TCHAR title [] = _T("Select project location") ;
    BROWSEINFO browseInfo = { m_hWnd, NULL, path, title, 0, NULL, 0, 0 } ;
    ITEMIDLIST *pItemList ;
    if (NULL != (pItemList = SHBrowseForFolder( &browseInfo )))
    {
        TCHAR buffer [MAX_PATH] ;
        if (SHGetPathFromIDList( pItemList, buffer ))
        {
            SetDlgItemText( IDC_SYSTEM_SETUP_DIRECTORY, buffer );
        }
    }
}

double SystemSetup::GetRefVoltage(CString name)
{
    if(m_refVoltageMap.find(name) == m_refVoltageMap.end())
        return 0.0f;

    return m_refVoltageMap[name];

}