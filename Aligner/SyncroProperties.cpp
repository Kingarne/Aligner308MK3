// $Id: SyncroProperties.cpp,v 1.10 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002
 
#include "stdafx.h"
#include "Aligner.h"
#include "SyncroProperties.h"
#include "Defines.h"
#include "DAU.h"
#include "SyncroSheet.h"
#include ".\syncroproperties.h"
#include "SystemSetup.h"

IMPLEMENT_DYNAMIC(SyncroProperties, CPropertyPage)

SyncroProperties::SyncroProperties()
	: CPropertyPage(SyncroProperties::IDD)
  , m_nominalAzimuth(0),m_buddy("")
{
    m_resolution = RESOLUTION_NOT_USED;
    
    m_typeMap[UnitType::Unused] = "Unused";
    m_typeMap[UnitType::Fixed] = "Connected";
    m_typeMap[UnitType::Fine] = "Fine";
    m_typeMap[UnitType::Coarse] = "Coarse";
    m_typeMap[UnitType::Composite] = "Composite";
}


SyncroProperties::~SyncroProperties( void )
{
}

void SyncroProperties::DoDataExchange( CDataExchange *pDX )
{
    CPropertyPage::DoDataExchange( pDX ) ;
      
    if( ( m_tabName != C1 ) && ( m_tabName != C2 ) )
    {
        DDX_Control( pDX, IDC_SYNCRO_GEARING, m_gearingControl ) ;
        if ( pDX -> m_bSaveAndValidate )
        {
            CString gearingText ;
            m_gearingControl.GetWindowText( gearingText ) ;
            m_gearing = atof( gearingText ) ;
        }
    }
    
    DDX_Control( pDX, IDC_SYNCRO_TYPE, m_typeControl ) ;  
    DDX_Control( pDX, IDC_SYNCRO_NOMINAL_AZIMUTH, m_nominalAzimuthControl ) ;
    DDX_Text( pDX, IDC_SYNCRO_NOMINAL_AZIMUTH, m_nominalAzimuth ) ;
    DDV_MinMaxDouble( pDX, m_nominalAzimuth, -180, 180 ) ;
    DDX_Control(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetectionControl ) ;
    DDX_Check(pDX, IDC_OVERRANGE_TRAP, m_overrangeDetection) ;
    DDX_Control(pDX, IDC_RESOLUTION_TITLE, m_resTitle);
    DDX_Control(pDX, IDC_TYPE_TITLE, m_typeTitle);
    DDX_Control(pDX, IDC_STATION_TITLE, m_stationTitle);
    DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzTitle);
    DDX_Control(pDX,  IDC_GEARING_TITLE, m_gearingTitle);  
    DDX_Control(pDX, IDC_HIGH_SEA_COMP, m_highSeaCompControl ) ;


    if(theApp.IsAligner202Enabled())
    {        
        DDX_Control( pDX, IDC_SYNCRO_DESCRIPTION, m_descriptionControl ) ;
        DDX_Text( pDX, IDC_SYNCRO_DESCRIPTION, m_description ) ;
        DDX_Control( pDX, IDC_SYNCRO_RES_COMBO, m_syncroResControl ) ;
        
    }
    else
    {
        DDX_Control(pDX,  IDC_STATION_COMBO, m_stationControl);  
        if (pDX->m_bSaveAndValidate && m_type && m_stationControl.GetCurSel() == CB_ERR)
        {
            AfxMessageBox( IDS_MUSTSETSTATION ) ;
            pDX->Fail() ;
        }
    }
}

void SyncroProperties::SetResolution()
{
    
}

void SyncroProperties::HideGearingItems( void )
{
  GetDlgItem( IDC_SYNCRO_GEARING )->ShowWindow( SW_HIDE );
  GetDlgItem( IDC_GEARING_TITLE)->ShowWindow( SW_HIDE );
}


SyncroProperties* SyncroProperties::GetSyncroPage(CString title)
{
    SyncroSheet* pParent;    
    pParent = static_cast<SyncroSheet*>(GetParent());
    int noOfPages = pParent->GetPageCount();
    SyncroProperties* pSync;
    CString pageName;

    for( int i=0; i<noOfPages; i++ )
    {
        pSync = dynamic_cast<SyncroProperties*>(pParent->GetPage( i ));
        if(pSync)
        {
            pageName = (CString)pSync->GetPSP().pszTitle;
            if(pageName == title)
            {
                return pSync;
            }
        }
    }

    return NULL;
}

void SyncroProperties::CheckCompositeState( CString compositePageTitle )
{
    int type1, type2;
    SyncroProperties* pSyncA, *pSyncB;
    CString chA = (compositePageTitle == C1) ? A1 : A2;
    CString chB = (compositePageTitle == C1) ? B1 : B2;

    pSyncA = GetSyncroPage(chA);
    pSyncB = GetSyncroPage(chB);    
    if(pSyncA && pSyncB)
    {
        type1 = pSyncA->m_type;
        type2 = pSyncB->m_type;
        if(type1 <= 0 || type2 <= 0 || type1==type2)
        {
            SyncroProperties* pComp = GetSyncroPage(compositePageTitle);
            if(pComp)
            {
                pComp->m_type = 0;
            }
        }
    }    
}


BOOL SyncroProperties::IsAAndBChannelsConnected( CString &compositePageTitle )
{
    int type1, type2;
    SyncroProperties* pSyncA, *pSyncB;
    CString chA = (compositePageTitle == C1) ? A1 : A2;
    CString chB = (compositePageTitle == C1) ? B1 : B2;
    
    pSyncA = GetSyncroPage(chA);
    pSyncB = GetSyncroPage(chB);    
    if(pSyncA && pSyncB)
    {
        type1 = pSyncA->m_type;
        type2 = pSyncB->m_type;
        if(type1 <= 0 || type2 <= 0 || type1==type2)
        {
            return FALSE;
        }
    }

    return TRUE;
}

void SyncroProperties::EnableChannelC( BOOL enable )
{
	if( enable == TRUE )
	{
		GetDlgItem( IDC_SYNCRO_TYPE )->EnableWindow( TRUE );
	}
	else
	{
		m_typeControl.SetCurSel( 0 );
		GetDlgItem( IDC_SYNCRO_TYPE )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYNCRO_DESCRIPTION )->EnableWindow( FALSE );
	}
  UpdateData( TRUE );
}

BEGIN_MESSAGE_MAP(SyncroProperties, CPropertyPage)
    ON_CBN_SELCHANGE(IDC_SYNCRO_TYPE, OnCbnSelchangeSyncroType)
    ON_CBN_SELCHANGE(IDC_STATION_COMBO, OnCbnSelchangeStation)
    ON_CBN_SELCHANGE(IDC_SYNCRO_RES_COMBO, OnCbnSelchangeSyncroRes)    
    ON_BN_CLICKED(IDC_HIGH_SEA_COMP, OnBnClickedHighSeaComp)
END_MESSAGE_MAP()

static char *sm_gearing [] = {
  "36.0", "30.0", "24.0", "20.0", "18.0", "16.0", "12.0",
  "10.0", "8.0", "6.0", "5.0", "4.0", "3.0", "2.0", "1.8", "1.0"
} ;

static void SetBoxText( char **pTextPointers, CComboBox *pBox, int items )
{
  for (int i = 0 ; i < items ; i++)
  {
    pBox -> AddString( pTextPointers [i] ) ;
  }
}

void SyncroProperties::SetupStations()
{
    m_stationControl.ResetContent();

    vector<CString> stations;
    DBInterface::Instance()->GetStationsOnShip(stations, SysSetup->GetShipName());
    vector<CString>::iterator iter;
    for(iter=stations.begin();iter!=stations.end();iter++)
    {
        int index = m_stationControl.AddString(*iter) ;
    }

    m_stationControl.SelectString(0, m_description);    
}

void SyncroProperties::OnCbnSelchangeSyncroRes()
{
    CString str;
    int index;
    if((index = m_syncroResControl.GetCurSel()) == CB_ERR)
        return;
    
    m_resolution = m_syncroResControl.GetItemData(index);

}

void SyncroProperties::OnCbnSelchangeStation()
{
    CString str;
    int index;
    if((index = m_stationControl.GetCurSel()) == CB_ERR)
        return;
	 
    m_stationControl.GetLBText(index, str);
    m_description = str;

};

BOOL SyncroProperties::OnInitDialog( void )
{
	m_tabName = (CString)GetPSP().pszTitle;
    m_isCompositeCh = (m_tabName == C1 || m_tabName == C2);
    //pSyncro->SetRefVoltageA202( refVoltage );

    if( theApp.IsAligner202Enabled() == TRUE )
	{
		Syncro *pSyncro1, *pSyncro2;
		pSyncro1 = DAU::GetDAU().GetSyncro( A1 );
		pSyncro2 = DAU::GetDAU().GetSyncro( A2 );
		if( (pSyncro1 == NULL) || (pSyncro2 == NULL) )
		{
			return( FALSE );
		}
		pSyncro1 = DAU::GetDAU().GetSyncro( B1 );
		pSyncro2 = DAU::GetDAU().GetSyncro( B2 );
		if( (pSyncro1 == NULL) || (pSyncro2 == NULL) )
		{
			return( FALSE );
		}
		pSyncro1 = DAU::GetDAU().GetSyncro( C1 );
		pSyncro2 = DAU::GetDAU().GetSyncro( C2 );
		if( (pSyncro1 == NULL) || (pSyncro2 == NULL) )
		{
			return( FALSE );
		}
	}

    CString controlText ;
    CPropertyPage::OnInitDialog() ;
    CString str;

    if(!m_isCompositeCh)
    {
        SetBoxText( sm_gearing, &m_gearingControl, sizeof(sm_gearing) / sizeof(char *) ) ;
        controlText.Format( "%3.1f", m_gearing ) ;
        m_gearingControl.SetWindowText( controlText ) ;
    }
  
    m_typeControl.ResetContent();
    if(theApp.IsAligner202Enabled())
    {
        m_highSeaCompControl.ShowWindow(SW_HIDE);
        GetDlgItem( IDC_SYNCRO_DESCRIPTION)->ShowWindow( SW_SHOW );                        
        GetDlgItem( IDC_STATION_COMBO)->ShowWindow( SW_HIDE );        
        m_stationTitle.SetWindowText("Description");
        m_stationTitle.ShowWindow( SW_SHOW );

        GetDlgItem( IDC_SYNCRO_NOMINAL_AZIMUTH )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_NOMINAL_AZIMUTH_TITLE )->ShowWindow( SW_HIDE );
        GetDlgItem( IDC_OVERRANGE_TRAP )->ShowWindow( SW_HIDE );        
                
        str.LoadString( IDS_SIGNAL_DESCRIPTION );
        SetDlgItemText( IDC_SYNCRO_DESCRIPTION_BOUNDARY, str );

        if( !m_isCompositeCh )
        {
            GetDlgItem( IDC_RESOLUTION_TITLE )->ShowWindow( SW_SHOW );            
            GetDlgItem( IDC_SYNCRO_RES_COMBO)->ShowWindow( SW_SHOW );            
            
            m_syncroResControl.AddString("10"); 
            m_syncroResControl.SetItemData(0,RESOLUTION_10_BIT); 
            m_syncroResControl.AddString("12"); 
            m_syncroResControl.SetItemData(1,RESOLUTION_12_BIT); 
            m_syncroResControl.AddString("14"); 
            m_syncroResControl.SetItemData(2,RESOLUTION_14_BIT); 
            m_syncroResControl.AddString("16"); 
            m_syncroResControl.SetItemData(3,RESOLUTION_16_BIT); 
            m_syncroResControl.SetCurSel(m_resolution);

            //Unused, Coarse and fine are valid types	                            
            m_typeControl.AddString( UnitType::GetUnitText(UnitType::Unused));
            m_typeControl.SetItemData(0, UnitType::Unused);
            m_typeControl.AddString( UnitType::GetUnitText(UnitType::Coarse));
            m_typeControl.SetItemData(1, UnitType::Coarse);
            m_typeControl.AddString( UnitType::GetUnitText(UnitType::Fine));
            m_typeControl.SetItemData(2, UnitType::Fine);                              
        }   
        if( m_isCompositeCh )
        {
            HideGearingItems();
            EnableChannelC( IsAAndBChannelsConnected( m_tabName ) );
            //Unused and Composite are valid types        
            m_typeControl.AddString( UnitType::GetUnitText(UnitType::Unused));
            m_typeControl.SetItemData(0, UnitType::Unused);
            m_typeControl.AddString( UnitType::GetUnitText(UnitType::Composite));
            m_typeControl.SetItemData(1, UnitType::Composite);        
        }
    }
    else
    {
        m_typeControl.AddString( UnitType::GetUnitText(UnitType::Unused));
        m_typeControl.SetItemData(0, UnitType::Unused);
        m_typeControl.AddString("Connected");// special case, m_typeMap[UnitType::Fixed] );
        m_typeControl.SetItemData(1, UnitType::Fixed);
    
        SetupStations();
    }

    if( m_type == UnitType::Unused )
    {
        m_typeControl.SetCurSel( 0 ); //Unused
    }
    else if( m_type == UnitType::Fine )
    {
        m_typeControl.SetCurSel( 2 ); //Fine
    }
    else
    {
        m_typeControl.SetCurSel( 1 ); //Coarse or Composite
    }
  
  	OnCbnSelchangeSyncroType() ;

    return TRUE ;
}

void SyncroProperties::OnCbnSelchangeSyncroType( void )
{
    BOOL typeSelected = m_typeControl.GetCurSel() != 0;
    
    //m_descriptionControl.EnableWindow( m_typeControl.GetCurSel() != 0 ) ;
    m_type = m_typeControl.GetItemData(m_typeControl.GetCurSel());

    if( theApp.IsAligner202Enabled())
    {
        GetDlgItem( IDC_SYNCRO_RES_COMBO )->EnableWindow(typeSelected);
        GetDlgItem( IDC_SYNCRO_DESCRIPTION)->EnableWindow(typeSelected);               
        if( !m_isCompositeCh )
        {
            m_gearingControl.EnableWindow(typeSelected) ;
            CheckCompositeState((m_tabName == A1 || m_tabName == B1) ? C1 : C2);
        }    
    }
    else
    {
        m_nominalAzimuthControl.EnableWindow( typeSelected ) ;
        m_overrangeDetectionControl.EnableWindow(typeSelected) ;
        m_stationControl.EnableWindow(typeSelected) ;
        m_gearingControl.EnableWindow(typeSelected) ;
        m_highSeaCompControl.EnableWindow(typeSelected) ;
    
        if(!typeSelected && DAU::GetDAU().m_activeHighSeaGyro == m_tabName)
        {
            DAU::GetDAU().m_activeHighSeaGyro = "";
            m_highSeaCompControl.SetCheck(FALSE);
            UpdateHSCState();
        }
    }
        
    if(m_buddy != "" )
    {
        if(m_type != UnitType::Unused)
            DAU::GetDAU().AddBuddyUsedChannel(m_tabName);
        else
            DAU::GetDAU().RemoveBuddyUsedChannel(m_tabName);
    }
}


BOOL SyncroProperties::OnApply()
{
    
    return CPropertyPage::OnApply();
}


BOOL SyncroProperties::OnSetActive()
{
    if( theApp.IsAligner202Enabled() == TRUE )
    {
        if( m_isCompositeCh )
        {
	        EnableChannelC( IsAAndBChannelsConnected( m_tabName ) );
        }
    }
    else
    {
        TRACE("Onsetactive\n");

        BOOL IsBuddyActive = DAU::GetDAU().IsBuddyChannelUsed(m_buddy);
        m_typeControl.EnableWindow(!IsBuddyActive);
            
        m_highSeaCompControl.SetCheck(DAU::GetDAU().m_activeHighSeaGyro == m_tabName);
        UpdateHSCState();
    }
    return CPropertyPage::OnSetActive();
}

void SyncroProperties::OnOK()
{

    CPropertyPage::OnOK();
}

void SyncroProperties::UpdateHSCState()
{
    CWnd* pWnd = GetDlgItem(IDC_HSC_INFO);
    pWnd->ShowWindow(m_highSeaCompControl.GetCheck() ? SW_SHOW : SW_HIDE);
}


void SyncroProperties::OnBnClickedHighSeaComp()
{
    DAU::GetDAU().m_activeHighSeaGyro = m_highSeaCompControl.GetCheck() ? m_tabName : "";    
    UpdateHSCState();
}
