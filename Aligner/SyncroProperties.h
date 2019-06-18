// $Id: SyncroProperties.h,v 1.9 2014-07-11 09:29:34 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "ColorStatic.h"

class SyncroProperties : public CPropertyPage
{
public:
	SyncroProperties( void ) ;
	virtual ~SyncroProperties( void ) ;

	enum { IDD = IDD_SYNCRO_PROPERTYPAGE };

    double m_nominalAzimuth ;
    double m_gearing ;
    CString m_buddy;  
    CString m_description ;
    BOOL m_overrangeDetection ;
    int m_type ;
    int m_resolution ;
    
protected:    
    
    virtual void DoDataExchange( CDataExchange *pDX ) ;
    void SetResolution( void ) ;
    void HideGearingItems( void ) ;
    BOOL IsAAndBChannelsConnected( CString &compositePageTitle ) ;
    void EnableChannelC( BOOL enable ) ;
    void SetupStations();
    SyncroProperties* GetSyncroPage(CString title);
    void CheckCompositeState( CString compositePageTitle );
    void UpdateHSCState();

    CComboBox m_stationControl ;
    CComboBox m_gearingControl ;
    CComboBox m_typeControl ;
    CComboBox m_syncroResControl;

    CEdit m_nominalAzimuthControl ;
    CEdit m_descriptionControl ;
    map<UnitType::Types, CString> m_typeMap;

    CButton m_overrangeDetectionControl ;    
    CButton m_highSeaCompControl;

    virtual BOOL OnInitDialog( void ) ;
    virtual BOOL OnSetActive( void ) ;
    virtual BOOL OnApply( void ) ;
    virtual void OnOK();

    afx_msg void OnCbnSelchangeSyncroType( void ) ;
    afx_msg void OnCbnSelchangeStation();
    afx_msg void OnCbnSelchangeSyncroRes();

    CString m_tabName;
    BOOL m_isCompositeCh;

    ColorStatic m_typeTitle;
    ColorStatic m_stationTitle;
    ColorStatic m_nominalAzTitle;
    ColorStatic m_gearingTitle;
    ColorStatic m_resTitle;

    DECLARE_DYNAMIC(SyncroProperties)
    DECLARE_MESSAGE_MAP()
  
public:
    afx_msg void OnBnClickedHighSeaComp();
} ;
