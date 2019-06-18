// $Id: DigitalProperties.cpp,v 1.12 2014-07-11 09:29:32 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Resource.h"
#include "DigitalProperties.h"
#include "SerialPort.h"
#include "Aligner.h"
#include "SystemSetup.h"
#include "DAU.h"
#include ".\digitalproperties.h"

//*******************************************************************************//
//***														DigitalProperties				A308Pro								***//
//*******************************************************************************//

IMPLEMENT_DYNAMIC(DigitalProperties, CPropertyPage)

DigitalProperties::DigitalProperties( void ) : CPropertyPage(DigitalProperties::IDD),
m_type(0), m_nominalAzimuth(0), m_description(_T("")), m_pDigital(NULL), m_DigChTypePro(DigChTypeProUnused),m_buddy("")
{
  // Empty
}

DigitalProperties::~DigitalProperties( void )
{
  // Empty
}

void DigitalProperties::DoDataExchange( CDataExchange *pDX )
{
    CPropertyPage::DoDataExchange( pDX ) ;
    DDX_Control(pDX, IDC_DIGITAL_TYPE, m_typeControl) ;
    DDX_CBIndex(pDX, IDC_DIGITAL_TYPE, m_type) ;
    DDX_Control(pDX, IDC_DIGITAL_NOMINAL_AZIMUTH, m_nominalAzimuthControl) ;
    DDX_Text(pDX, IDC_DIGITAL_NOMINAL_AZIMUTH, m_nominalAzimuth) ;
    DDV_MinMaxDouble(pDX, m_nominalAzimuth, -180, 180) ;
    DDX_Control(pDX, IDC_DIGITAL_DESCRIPTION, m_descriptionControl);   
    DDX_Control(pDX, IDC_TYPE_TITLE, m_typeTitle);
    DDX_Control(pDX, IDC_STATION_TITLE, m_stationTitle);
    DDX_Control(pDX, IDC_NOMINAL_AZIMUTH_TITLE, m_nominalAzTitle);
    DDX_Control(pDX, IDC_STATION_COMBO, m_stationControl) ;     
    DDX_Control(pDX, IDC_HIGH_SEA_COMP, m_highSeaCompControl);   
    if (pDX->m_bSaveAndValidate && m_DigChTypePro != DigChTypeProUnused && m_stationControl.GetCurSel() == CB_ERR && theApp.IsAligner202Enabled()==FALSE)
    {
        AfxMessageBox( IDS_MUSTSETSTATION ) ;
        pDX->Fail() ;
    }

    if(theApp.IsAligner202Enabled())
    {
        DDX_Text(pDX, IDC_DIGITAL_DESCRIPTION, m_description);
    }

}

BEGIN_MESSAGE_MAP(DigitalProperties, CPropertyPage)  
    ON_CBN_SELCHANGE(IDC_DIGITAL_TYPE, OnCbnSelchangeDigitalType)
    ON_CBN_SELCHANGE(IDC_STATION_COMBO, OnCbnSelchangeStation)    
    ON_BN_CLICKED(IDC_HIGH_SEA_COMP, OnBnClickedHighSeaComp)
END_MESSAGE_MAP()


BOOL DigitalProperties::OnInitDialog( void )
{
    CPropertyPage::OnInitDialog() ;
    CString str;

    //m_description = m_pDigital->GetUnitTypeDescription() ;
    //m_nominalAzimuth = m_pDigital->GetNominalAzimuth() ;
    UpdateData(FALSE);

    if( theApp.IsAligner202Enabled() == TRUE )
    {
        
        GetDlgItem( IDC_STATION_TITLE)->SetWindowText("Description");
        m_stationControl.ShowWindow(SW_HIDE);
        m_highSeaCompControl.ShowWindow(SW_HIDE);
        m_descriptionControl.ShowWindow(SW_SHOW);
    }
    else
    { 
        SetupStations();
    }
    TRACE("Type: %d, %d\n",m_pDigital->GetSyncroType(), m_type);
    BOOL validType = FALSE;
    m_typeControl.ResetContent();
    int index = 0;
    int comboIndex = 0;
    str.LoadString( IDS_UNUSED );
    m_typeControl.AddString( str );
    m_typeControl.SetItemData( index++, 0 );
    //m_DigChTypePro = m_pDigital->GetSyncroType();

    map<int, ValidSyncroInfo>::iterator iter;
    for(iter = SysSetup->m_validSyncroTypes.begin(); iter != SysSetup->m_validSyncroTypes.end();iter++)
    {
        m_typeControl.AddString( iter->second.name );
        m_typeControl.SetItemData( index++, iter->second.type );
        if( m_DigChTypePro == iter->second.type )
        {
            comboIndex = index - 1;
            validType = TRUE;
        }
    }

    TRACE("combo index1:%d\n",comboIndex);
	if( validType == FALSE )
    {
        comboIndex = 0;
        m_type = 0;
    }
    TRACE("combo index2:%d\n",comboIndex);
    m_typeControl.SetCurSel( comboIndex );
  
    OnCbnSelchangeDigitalType() ;
    

    return TRUE ;
}

void DigitalProperties::SetupStations()
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

void DigitalProperties::OnCbnSelchangeStation()
{
    CString str;
    int index;
    if((index = m_stationControl.GetCurSel()) == CB_ERR)
        return;

    m_stationControl.GetLBText(index, str);
    m_description = str;

};

//*******************************************************************************//
//***											OnCbnSelchangeDigitalType				A308Pro							***//
//***	----------------------------------------------------------------------- ***//
//***		New																																		***//
//***		Purpose : To set a new ch type (Unused, Seapath..Mins) and save it		***//
//***			temporarily until OK btn																						***//
//*******************************************************************************//
void DigitalProperties::OnCbnSelchangeDigitalType( void )
{	
    BOOL selected =  m_typeControl.GetCurSel() != 0;
    m_nominalAzimuthControl.EnableWindow(selected) ;
	m_stationControl.EnableWindow(selected) ;
    m_highSeaCompControl.EnableWindow(selected) ;

    if( theApp.IsAligner202Enabled())
    {
        m_descriptionControl.EnableWindow(selected) ;        
    }
    else
    {
        if(!selected && (DAU::GetDAU().m_activeHighSeaGyro == m_pDigital->GetName()))
        {
            DAU::GetDAU().m_activeHighSeaGyro = "";
            m_highSeaCompControl.SetCheck(FALSE);
            UpdateHSCState();
        }
    }

    //***   Dig ch D1 or D2 ?   ***//
    CString StrDigCh ;
    StrDigCh = this->GetPSP().pszTitle ;
    int ArrIx = DAU::GetDAU().m_dauDataPro.GetDigDauChPro( StrDigCh ) ;
    //***   Dig ch type ?   ***//
    CString StrDigChType ;
    m_typeControl.GetLBText(m_typeControl.GetCurSel(), StrDigChType) ;

    //***   Set temp DigCh type   ***//
    m_DigChTypePro = DAU::GetDAU().m_dauDataPro.GetDigChTypePro( StrDigChType ) ;

    if(m_buddy != "")
    {
        if(m_DigChTypePro != DigChTypeProUnused)
            DAU::GetDAU().AddBuddyUsedChannel(StrDigCh);
        else
            DAU::GetDAU().RemoveBuddyUsedChannel(StrDigCh);
    }
}

void DigitalProperties::OnOK()
{
      
    m_pDigital->SetSyncroType(m_DigChTypePro, true);
  

    CPropertyPage::OnOK();
}



BOOL DigitalProperties::OnSetActive()
{
    BOOL IsBuddyActive = DAU::GetDAU().IsBuddyChannelUsed(m_buddy);
        
    m_typeControl.EnableWindow(!IsBuddyActive);
        
    m_highSeaCompControl.SetCheck(DAU::GetDAU().m_activeHighSeaGyro == m_pDigital->GetName());
    UpdateHSCState();

    return CPropertyPage::OnSetActive();
}

void DigitalProperties::UpdateHSCState()
{
    CWnd* pWnd = GetDlgItem(IDC_HSC_INFO);
    pWnd->ShowWindow(m_highSeaCompControl.GetCheck() ? SW_SHOW : SW_HIDE);
}


void DigitalProperties::OnBnClickedHighSeaComp()
{   
    DAU::GetDAU().m_activeHighSeaGyro = m_highSeaCompControl.GetCheck() ? m_pDigital->GetName() : "";    
    UpdateHSCState();    
}
