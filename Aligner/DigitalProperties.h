// $Id: DigitalProperties.h,v 1.6 2013/04/02 08:28:15 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "resource.h"
#include "DAU.h"
#include "ColorStatic.h"

class DigitalProperties : public CPropertyPage
{
public:
    DigitalProperties( void ) ;
    virtual ~DigitalProperties( void ) ;

    enum { IDD = IDD_DIGITAL_PROPERTYPAGE };

    int m_type ;
    double m_nominalAzimuth ;
    CString m_description ;
    DigChTypePro m_DigChTypePro;        

    CString m_buddy;
    Digital* m_pDigital;

protected:
	
	virtual void DoDataExchange( CDataExchange *pDX ) ;
    afx_msg void OnBnClickedDigChChange( void ) ;
    virtual BOOL OnInitDialog( void ) ;
    afx_msg void OnCbnSelchangeDigitalType( void ) ;
    afx_msg void OnCbnSelchangeStation();
    void UpdateHSCState();

    CComboBox m_typeControl ;
    CComboBox m_stationControl ;
    CEdit m_nominalAzimuthControl ;
    CEdit m_descriptionControl ;
    CButton m_highSeaCompControl;

    virtual void OnOK();
    void SetupStations();
    
    ColorStatic m_typeTitle;
    ColorStatic m_stationTitle;
    ColorStatic m_nominalAzTitle;

    virtual BOOL OnSetActive();

    DECLARE_DYNAMIC(DigitalProperties)
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedHighSeaComp();
} ;
