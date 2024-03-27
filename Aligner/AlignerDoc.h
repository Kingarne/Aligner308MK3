// $Id: AlignerDoc.h,v 1.8 2014-07-11 09:29:30 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#include "XMLHandler.h"

#define FILE_FORMAT_VERSION           2
#define SERIALIZE_SETUP               1
#define SERIALIZE_SETUP_VALIDATION    0x77f75a58 //magic number

class CAlignerDoc : public CDocument
{
  DECLARE_MESSAGE_MAP()
  DECLARE_DYNCREATE(CAlignerDoc)

    friend class XMLHandler;
protected:
  CAlignerDoc( void ) ;
public:
  virtual BOOL OnNewDocument( void ) ;
  virtual void Serialize( CArchive &ar ) ;
  /*virtual void Serialize( CArchive &ar, int mode ) ;*/
public:
  virtual ~CAlignerDoc( void ) ;
public:
  virtual void DeleteContents( void ) ;
public:
  //void AddMeasurment( LONG measurmentID ) ;
  //void RemoveMeasurment( LONG measurmentID ) ;
  void SaveProject( void ) ;
  void ReOpenDocument();
  void LoadSensorCalibration();
protected:
  BOOL GetCanSetupAlign( void ) const ;
  BOOL GetCanSetupCalibrate( void ) const ;
  BOOL GetCanSetupA202Channels( void ) const ;
  BOOL OnSetupSystem( int mode, BOOL showDialog = TRUE )  ;
protected:
  afx_msg void OnSetupSystem( void ) ;
  afx_msg void OnUpdateSetupSystem( CCmdUI *pCmdUI ) ; 
  afx_msg void OnCalibrationSetup( void )  ;
  afx_msg void OnUpdateCalibrationSetup( CCmdUI *pCmdUI ) ; 
//   afx_msg void OnCalibDataExport( void )  ;
//   afx_msg void OnUpdateCalibDataExport( CCmdUI *pCmdUI ) ; 
  afx_msg void OnCalibDataImport( void )  ;
  afx_msg void OnUpdateCalibDataImport( CCmdUI *pCmdUI ) ; 
  afx_msg void OnAlignmentSetup( void )  ;
  afx_msg void OnUpdateAlignmentSetup( CCmdUI *pCmdUI ) ; 
//   afx_msg void OnSetupComports( void ) ;
//   afx_msg void OnUpdateSetupComports( CCmdUI *pCmdUI ) ;
  afx_msg void OnSetupA202EditChannels( void ) ;
  afx_msg void OnUpdateSetupA202EditChannels( CCmdUI *pCmdUI ) ;
  afx_msg void OnSetupA202LoadChannels( void ) ;
  afx_msg void OnUpdateSetupA202LoadChannels( CCmdUI *pCmdUI ) ;
  afx_msg void OnSetupA202SaveChannels( void ) ;
  afx_msg void OnUpdateSetupA202SaveChannels( CCmdUI *pCmdUI ) ;
  afx_msg void OnCalibrationCreatetemperaturemodel( void ) ;
  afx_msg void OnUpdateCalibrationCreatetemperaturemodel( CCmdUI *pCmdUI ) ;
  afx_msg void OnProjectCalibration( void ) ;
  afx_msg void OnUpdateProjectCalibration( CCmdUI *pCmdUI ) ;
  afx_msg void OnUpdateFileOpen( CCmdUI *pCmdUI ) ;
  afx_msg void OnUpdateFileNew( CCmdUI *pCmdUI ) ;
  afx_msg void OnUpdateFileSave( CCmdUI *pCmdUI ) ;
  afx_msg void OnUtilitiesViewreport( void ) ;
  afx_msg void OnUpdateUtilitiesViewreport( CCmdUI *pCmdUI ) ; 
  afx_msg void OnCalibrationDauoffset( void ) ;
  afx_msg void OnUpdateCalibrationDauoffset( CCmdUI *pCmdUI ) ;
  afx_msg void OnCalibrationDaugain( void ) ;
  afx_msg void OnUpdateCalibrationDaugain( CCmdUI *pCmdUI ) ;
  afx_msg void OnCalibrationOffset( void ) ;
  afx_msg void OnUpdateCalibrationOffset( CCmdUI *pCmdUI ) ;
  afx_msg void OnCalibrationGain( void ) ;
  afx_msg void OnUpdateCalibrationGain( CCmdUI *pCmdUI ) ;
  afx_msg void OnCalibrationAdapter( void ) ;
  afx_msg void OnUpdateCalibrationAdapter( CCmdUI *pCmdUI ) ;
  afx_msg void OnUtilitiesViewcalibrationdata();
  afx_msg void OnUpdateUtilitiesViewcalibrationdata(CCmdUI *pCmdUI);
   afx_msg void OnUpdateCalibrationDailyazimutherrortest(CCmdUI *pCmdUI);

  void StoreChSetup(CArchive& archive, int ver);
  int LoadChSetup(CArchive& archive, int ver);
  int NewProject(int mode);

public:
	afx_msg void OnCalibrationDailyazimutherrortest();

  BOOL m_probeError;
private:
  BOOL m_initialSetup ;
  XMLHandler m_XMLHandler;

#ifdef _DEBUG
  virtual void AssertValid( void ) const ;
  virtual void Dump( CDumpContext& dc ) const ;
#endif
public:
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  afx_msg void OnUtilitiesComsetup();
  afx_msg void OnCalibrationTheoadapter();
  afx_msg void OnUpdateCalibrationTheoadapter(CCmdUI *pCmdUI);

  BOOL OpenConfig();
  afx_msg void OnProjectOpencalibration();
  afx_msg void OnUtilitiesFixtheoadaptercalibration();
  afx_msg void OnUpdateUtilitiesFixtheoadaptercalibration(CCmdUI* pCmdUI);
} ;
