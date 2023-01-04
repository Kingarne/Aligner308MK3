// $Id: AlignerDoc.cpp,v 1.26 2014-07-11 09:29:30 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "AlignerDoc.h"
#include "UserMessages.h"
#include "Util.h"
#include "PortSetup.h"
#include "SystemConfigurationView.h"
#include "CreatePolynomial.h"
//#include "History.h"
#include "LiveDataViewA202.h"
#include "SystemSetup.h"
#include "LiveDataView.h"
#include "Database.h"
//#include "../AlignerReport/AlignerReport.h"
#include "SetupLiveDataGraphDlg.h"
#include "DAUCalibrationWizard.h"
#include "TiltOffsetCalibration.h"
#include "GainAzimuthCalibration.h"
#include "AdapterCalibration.h"
#include "TheoAdapterCalibration.h"
#include ".\alignerdoc.h"
#include "ComSettingsDlg.h"
#include "ReportManager.h"
#include "ProjectOpenCalibDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAlignerDoc, CDocument)
 
BEGIN_MESSAGE_MAP(CAlignerDoc, CDocument)
  ON_COMMAND(ID_CALIBRATION_SETUP, OnCalibrationSetup)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_SETUP, OnUpdateCalibrationSetup)
  ON_COMMAND(ID_ALIGNMENT_SETUP, OnAlignmentSetup)
  ON_UPDATE_COMMAND_UI(ID_ALIGNMENT_SETUP, OnUpdateAlignmentSetup)
//   ON_COMMAND(ID_CALIBRATIONDATA_EXPORT, OnCalibDataExport)
//   ON_UPDATE_COMMAND_UI(ID_CALIBRATIONDATA_EXPORT, OnUpdateCalibDataExport)
  ON_COMMAND(ID_CALIBRATIONDATA_IMPORT, OnCalibDataImport)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATIONDATA_IMPORT, OnUpdateCalibDataImport)
//   ON_COMMAND(ID_SETUP_COMPORTS, OnSetupComports)
//   ON_UPDATE_COMMAND_UI(ID_SETUP_COMPORTS, OnUpdateSetupComports)
  ON_COMMAND(ID_SETUP_A202_EDIT_CHANNELS, OnSetupA202EditChannels)
  ON_UPDATE_COMMAND_UI(ID_SETUP_A202_EDIT_CHANNELS, OnUpdateSetupA202EditChannels)
  ON_COMMAND(ID_CALIBRATION_CREATETEMPERATUREMODEL, OnCalibrationCreatetemperaturemodel)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_CREATETEMPERATUREMODEL, OnUpdateCalibrationCreatetemperaturemodel)
  ON_COMMAND(ID_PROJECT_CALIBRATION, OnProjectCalibration)
  ON_UPDATE_COMMAND_UI(ID_PROJECT_CALIBRATION, OnUpdateProjectCalibration)
  ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
  ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
  ON_COMMAND(ID_SETUP_SYSTEM, OnSetupSystem)
  ON_UPDATE_COMMAND_UI(ID_SETUP_SYSTEM, OnUpdateSetupSystem)
  ON_COMMAND(ID_SETUP_A202_LOAD_CHANNELS, OnSetupA202LoadChannels)
  ON_UPDATE_COMMAND_UI(ID_SETUP_A202_LOAD_CHANNELS, OnUpdateSetupA202LoadChannels)
  ON_COMMAND(ID_SETUP_A202_SAVE_CHANNELS, OnSetupA202SaveChannels)
  ON_UPDATE_COMMAND_UI(ID_SETUP_A202_SAVE_CHANNELS, OnUpdateSetupA202SaveChannels)
  ON_COMMAND(ID_UTILITIES_VIEWREPORT, OnUtilitiesViewreport)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_VIEWREPORT, OnUpdateUtilitiesViewreport)
  ON_COMMAND(ID_CALIBRATION_DAUOFFSET, OnCalibrationDauoffset)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_DAUOFFSET, OnUpdateCalibrationDauoffset)
  ON_COMMAND(ID_CALIBRATION_DAUGAIN, OnCalibrationDaugain)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_DAUGAIN, OnUpdateCalibrationDaugain)
  ON_COMMAND(ID_CALIBRATION_OFFSET, OnCalibrationOffset)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_OFFSET, OnUpdateCalibrationOffset)
  ON_COMMAND(ID_CALIBRATION_GAIN, OnCalibrationGain)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_GAIN, OnUpdateCalibrationGain)
  ON_COMMAND(ID_CALIBRATION_ADAPTER, OnCalibrationAdapter)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_ADAPTER, OnUpdateCalibrationAdapter)
  ON_COMMAND(ID_UTILITIES_VIEWCALIBRATIONDATA, OnUtilitiesViewcalibrationdata)
  ON_UPDATE_COMMAND_UI(ID_UTILITIES_VIEWCALIBRATIONDATA, OnUpdateUtilitiesViewcalibrationdata)
  ON_COMMAND(ID_CALIBRATION_DAILYAZIMUTHERRORTEST, OnCalibrationDailyazimutherrortest)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_DAILYAZIMUTHERRORTEST, OnUpdateCalibrationDailyazimutherrortest)
  ON_COMMAND(ID_UTILITIES_COMSETUP, OnUtilitiesComsetup)
  ON_COMMAND(ID_CALIBRATION_THEOADAPTER, &CAlignerDoc::OnCalibrationTheoadapter)
  ON_UPDATE_COMMAND_UI(ID_CALIBRATION_THEOADAPTER, &CAlignerDoc::OnUpdateCalibrationTheoadapter)
	ON_COMMAND(ID_PROJECT_OPENCALIBRATION, &CAlignerDoc::OnProjectOpencalibration)
END_MESSAGE_MAP()

CAlignerDoc::CAlignerDoc( void )
{
    m_initialSetup = TRUE ;
    m_probeError = FALSE ;
    m_XMLHandler.m_pDoc = this;
}

CAlignerDoc::~CAlignerDoc( void )
{
  // Empty
}

BOOL CAlignerDoc::OnNewDocument( void )
{
    if (!CDocument::OnNewDocument())
    {
		    return FALSE ;
    }
	
    if (m_initialSetup)
    {        
        m_initialSetup = FALSE ;
        return TRUE ;
    }
    
	NewProject(SYSTEM_SETUP_MODE_ALIGNMENT);
   
    return TRUE ;
}

int CAlignerDoc::NewProject(int mode)
{
	if (!OnSetupSystem(mode))
	{
		SetModifiedFlag(FALSE);
		return FALSE;
	}

	DAU::GetDAU().Clear();
	DAU::GetDAU().SetSelected(FALSE);
	DAU::GetDAU().EnableErrorEvent(FALSE);

	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT)
	{
		string directory = SysSetup->GetProjectPath();

		if (!::CreateDirectory(directory.c_str(), NULL))
		{
			int err = GetLastError();
			if (err == ERROR_ALREADY_EXISTS)
			{
				::AfxMessageBox(_T("Project already exist, enter different project name"));
			}
			else
			{
				::AfxMessageBox(_T("Unable to create project directory"));
			}
			OnSetupSystem(SYSTEM_SETUP_NO_MODE, FALSE);
			SetModifiedFlag(FALSE);
			return TRUE;
		}

		directory = SysSetup->GetProjectPath() + _T("\\Images");
		if (!::CreateDirectory(directory.c_str(), NULL))
		{
			::AfxMessageBox(_T("Unable to create project image directory"));
			OnSetupSystem(SYSTEM_SETUP_NO_MODE, FALSE);
			SetModifiedFlag(FALSE);
			return TRUE;
		}
		SetCurrentDirectory(SysSetup->GetProjectPath());
	}

	CString fileName = SysSetup->GetProjectPath() + _T("\\") + SysSetup->GetProjectName();

	SetPathName(fileName);

	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT)
	{
		DBInterface::Instance()->InsertProject(SysSetup->GetProject());
	}
	else if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_CALIBRATION)
	{
		DBInterface::Instance()->InsertProjectCalibration(SysSetup->GetProject());
	}

	CSetupLiveDataGraphDlg::m_TextFileDir = DEFAULT_FILE_DIR;
	CGraphView::m_GraphFileDir = DEFAULT_FILE_DIR;

	return TRUE;
}

void CAlignerDoc::OnProjectCalibration(void)
{
	int result = NewProject(SYSTEM_SETUP_MODE_CALIBRATION);
	
	if (result == TRUE)
	{
		theApp.RenameDocument(_T("Calibration"));
	}
	else
	{
		theApp.RenameDocument(_T(""));
	}
}

void CAlignerDoc::OnProjectOpencalibration()
{
	ProjectOpenCalibDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SystemSetup::Instance()->SetProject(dlg.m_selectedProj);

		OpenConfig();
		SetPathName(dlg.m_selectedProj.m_projectName);


		TRACE("Proj: %s\n", dlg.m_selectedProj.m_projectName);

	}
}


void CAlignerDoc::LoadSensorCalibration()
{
	for (int i = 0; i < DAU::GetDAU().GetSensorCount(); i++)
	{
		DAU::GetDAU().GetSensor(i)->LoadCalibration();
	}
}



BOOL CAlignerDoc::OpenConfig()
{
	if (SysSetup->GetProjectID() < 1)
		return false;

	DAU::GetDAU().Clear();
	CString str;
	//OnSetupSystem(SYSTEM_SETUP_MODE_ALIGNMENT, FALSE);
	
	if (m_XMLHandler.ParseConfig(SysSetup->GetConfigXML()))
	{

	}
	
	static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView)->ShowAll(SW_SHOW);

	CSetupLiveDataGraphDlg::m_TextFileDir = DEFAULT_FILE_DIR;
	CGraphView::m_GraphFileDir = DEFAULT_FILE_DIR;
	return TRUE;
}

void CAlignerDoc::ReOpenDocument()
{
	CString docName = GetPathName();
	if(docName != "")
		OnOpenDocument(docName);

}

BOOL CAlignerDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
 /*   m_probeError = FALSE;
    DAU::GetDAU().Close() ;
    CString str;
    OnSetupSystem( SYSTEM_SETUP_MODE_ALIGNMENT, FALSE ) ;
          
    if(m_XMLHandler.Load(lpszPathName))
    {
        static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView) -> ShowAll( SW_SHOW ) ;
    }
    else
    {
        if (!CDocument::OnOpenDocument(lpszPathName))
        {
            OnSetupSystem( SYSTEM_SETUP_NO_MODE, FALSE ) ;
            return TRUE ;
        }  
    }
    m_ChannelSetupFileDir = DEFAULT_FILE_DIR ;
    CSetupLiveDataGraphDlg::m_TextFileDir = DEFAULT_FILE_DIR ;
    CGraphView::m_GraphFileDir = DEFAULT_FILE_DIR ;*/
    return TRUE ;
}

class Tracer
{
public:
  ~Tracer( void ) {
    ::OutputDebugString( m_text ) ;
  }
public:
  CString &operator()( void ) {
    return m_text ;
  }
private:
  CString m_text ;
} ;

void CAlignerDoc::DeleteContents( void )
{
//  Tracer()().Format( _T("Vector size is -> %d\n"), m_projectMeasurments.size()) ;
//  m_projectMeasurments.clear() ;
  __super::DeleteContents() ;
}

void CAlignerDoc::StoreChSetup(CArchive& ar, int ver)
{
    map<CString, Sensor*>& sens = DAU::GetDAU().m_sensorMap;
    map<CString, Sensor*>::iterator iter;
    ar << sens.size();
    for(iter = sens.begin();iter!= sens.end();iter++)
    {
        TRACE("store:%s\n",iter->first);
        Sensor* pSensor = iter->second;
        ar << pSensor->GetName();
        ar << pSensor->GetOffset();
        ar << pSensor->GetDefaultScale();
        ar << pSensor->GetSerialNumber();
        ar << pSensor->GetAdapterSerialNumber();
        ar << pSensor->GetNominalAzimuth();
        ar << (int)pSensor->GetType();
        ar << (int)pSensor->GetOverrangeDetection();
        ar << (int)pSensor->IsSelected();   
        ar << pSensor->m_description;
    }

    map<CString, Syncro*>& sync = DAU::GetDAU().m_syncroMap;
    map<CString, Syncro*>::iterator it;
    ar << sync.size();
    int isDigital=0;//, digitalType;
    for(it = sync.begin();it!= sync.end();it++)
    {
        TRACE("store:%s\n",it->first);
        Syncro* pSyncro = it->second;
        Digital* pDig = dynamic_cast<Digital*>(pSyncro);
        isDigital = (pDig != NULL) ? 1 : 0;        
        DigChTypePro digitalType = (isDigital == 1) ? pDig->m_syncroType : DigChTypeProUnused;

        ar << pSyncro->GetName();
        ar << pSyncro->GetOffset();        
        ar << (int)pSyncro->GetType();
        ar << (int)pSyncro->GetOverrangeDetection();
        ar << (int)pSyncro->IsSelected();   
        ar << pSyncro->GetNominalAzimuth();
        ar << pSyncro->GetGearing();     
        ar << pSyncro->m_description;
        ar << isDigital;
        ar << (int)digitalType;
        if(ver >= 2)
        {
            ar << pSyncro->GetResolution();
        }

    }
}

int CAlignerDoc::LoadChSetup(CArchive& ar, int ver)
{    
    Sensor* pSysSens;
    Syncro* pSysSync;    
    map<CString, Sensor*> sensors;   
    map<CString, Syncro*> syncros;   
    BOOL updateConfig = TRUE;

    //Do sensors
    CString name, serial, adaptSerial, desc;
    int type, overrange, selected, offset, resolution=3;    
    int numSens=0;
    double defultScale, nomAz, gearing;
    try
    {
        ar >> numSens;    
        if(numSens != DAU::GetDAU().GetSensorCount())
		{
			throw "Mismatch1";
		}

        for(int i=0;i<numSens;i++)
        {
            ar >> name;
            ar >> offset;
            ar >> defultScale;
            ar >> serial;
            ar >> adaptSerial;
            ar >> nomAz;
            ar >> type;
            ar >> overrange;
            ar >> selected;
            ar >> desc;
            TRACE("load:%s, %d\n",name, offset);
            if((pSysSens = DAU::GetDAU().GetSensor(name)) == NULL || pSysSens->GetOffset() != offset)
            {
                throw "Mismatch2";        
            }
            Sensor* pSens = new Sensor(name);        
            pSens->SetDefaultScale(defultScale);
            pSens->SetOffset(offset);
            pSens->SetSerialNumber(serial);
            pSens->SetAdapterSerialNumber(adaptSerial);
            pSens->SetNominalAzimuth(nomAz);
            pSens->SetType((UnitType::Types)type);
            pSens->SetOverrangeDetection(overrange);
            pSens->SetIsSelected(selected);            
            pSens->m_description = desc;
            sensors[name] = pSens;
        }

        //Do syncros and digital
        int numSync=0;
        int isDigital = 0, digitalType;
        ar >> numSync;
        if(numSync != DAU::GetDAU().GetSyncroCount())
		{
			throw "Mismatch3";
		}

        for(int i=0;i<numSync;i++)
        {
            ar >> name;
            ar >> offset;
            ar >> type;
            ar >> overrange;
            ar >> selected;            
            ar >> nomAz;
            ar >> gearing;
            ar >> desc;
            ar >> isDigital;
            ar >> digitalType;    
            if(ver >= 2)
            {
                ar >> resolution;
            }
            if((pSysSync = DAU::GetDAU().GetSyncro(name)) == NULL || pSysSync->GetOffset() != offset)
            {
                throw "Mismatch4";        
            }
            
            Syncro* pSync;
            if(isDigital)
            {
                pSync = new Digital(name, FALSE);
                ((Digital*)pSync)->SetSyncroType((DigChTypePro)digitalType);                
            }
            else
            {
                pSync = new Syncro(name);
            }       
            pSync->SetOffset(offset);
            pSync->SetType((UnitType::Types)type);
            pSync->SetOverrangeDetection(overrange);
            pSync->SetIsSelected(selected);     
            pSync->SetGearing(gearing);
            pSync->SetResolution(resolution);
            pSync->m_description = desc;
            syncros[name] = pSync;
            TRACE("load:%s, %d, %d\n",name, offset, isDigital);
        }
    }
    catch(char* pStr)
    {
        TRACE("Exception:%s\n",pStr);
        updateConfig = FALSE;
    }

    if(updateConfig)
    {        
        map<CString, Sensor*>::iterator iter;
        for(iter=sensors.begin();iter!=sensors.end();iter++)
        {
            Sensor* pSens = DAU::GetDAU().GetSensor(iter->first);
            if(pSens != NULL)
            {
            //    *pSens = *iter->second;   				
				pSens->SetOffset(iter->second->GetOffset());
				pSens->SetSerialNumber(iter->second->GetSerialNumber());
				pSens->SetAdapterSerialNumber(iter->second->GetAdapterSerialNumber());				
				pSens->SetType((UnitType::Types)iter->second->GetType());
				pSens->SetOverrangeDetection(iter->second->GetOverrangeDetection());
				pSens->SetIsSelected(iter->second->IsSelected());            
				pSens->SetNominalAzimuth(iter->second->GetNominalAzimuth());
				pSens->m_description = iter->second->m_description;
				pSens->LoadCalibration();
			}
        }
        map<CString, Syncro*>::iterator it;
        for(it=syncros.begin();it!=syncros.end();it++)
        {
            Syncro* pSync = DAU::GetDAU().GetSyncro(it->first);
            if(pSync != NULL)
            {
                //*pSync= *it->second;
				pSync->SetOffset(it->second->GetOffset());
				pSync->SetType((UnitType::Types)it->second->GetType());
				pSync->SetOverrangeDetection(it->second->GetOverrangeDetection());
				pSync->SetIsSelected(it->second->IsSelected());     				
				pSync->SetGearing(it->second->GetGearing());
                pSync->SetResolution(it->second->GetResolution());
                pSync->m_description = it->second->m_description;

                Digital* pDig = dynamic_cast<Digital*>(pSync);
                Digital* pLoadDig = dynamic_cast<Digital*>(it->second);                
                if(pDig != NULL && pLoadDig != NULL)
                {            
                    pDig->SetSyncroType(pLoadDig->m_syncroType);   
                }                
            }
        }
        //map<CString, Sensor*>& sensMap = DAU::GetDAU().m_sensorMap;            
    }

    //Delete
    map<CString, Syncro*>::iterator syncI; 
    map<CString, Sensor*>::iterator sensI; 
    for(syncI=syncros.begin();syncI!=syncros.end();syncI++)
    {
        if(syncI->second != NULL)
            delete syncI->second;
    }
    for(sensI=sensors.begin();sensI!=sensors.end();sensI++)
    {
        if(sensI->second != NULL)
            delete sensI->second;
    }

    DAU::GetDAU().UpdateUARTProtocoll();
    DAU::GetDAU().SetResolutionsToDAU();

    return 1;
}

void CAlignerDoc::Serialize( CArchive &archive )
{     
	/*
    if (archive.IsStoring())
    {                        
        archive << FILE_FORMAT_VERSION;
        archive << SysSetup->GetProjectName() ;
        archive << SysSetup->GetOperatorName() ;
        archive << SysSetup->GetShipName() ;
        archive << SysSetup->GetPlace() ;
        archive << SysSetup->GetLatitude();
        archive << SysSetup->GetImageFileIndex(); 
        archive << m_projectMeasurments.size() ;
        for (std::set<LONG>::iterator i = m_projectMeasurments.begin() ; i != m_projectMeasurments.end() ; i++)
        {
            archive << *i ;
        }
        archive << SysSetup->GetUnits() ;
        StoreChSetup(archive, FILE_FORMAT_VERSION);                
    }
    else
    {               
        CString text ;
        int version;
        archive >> version;
        archive >> text ;
        SysSetup->SetProjectName( text ) ;
        archive >> text ;
        SysSetup->SetOperatorName( text ) ;
        archive >> text ;
        SysSetup->SetShipName( text ) ;
        archive >> text ;
        SysSetup->SetPlace( text ) ;
        double latitude;
        archive >> latitude ;
        SysSetup->SetLatitude( latitude ) ;
        long index;
        archive >> index ;
        SysSetup->SetImageFileIndex( index ) ;
        std::set<LONG>::size_type size ;
        archive >> size ;
        for ( std::set<LONG>::size_type i = 0 ; i < size ; i++)
        {
            LONG value ;
            archive >> value ;
            m_projectMeasurments.insert( value ) ;
        }
        int UnitTypes ;
        archive >> UnitTypes ;
        SysSetup->SetUnits( UnitTypes ) ;
  	    LoadChSetup(archive, version);
        static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView) -> ShowAll( SW_SHOW ) ;
              
    }
    
    */
}



#ifdef _DEBUG
void CAlignerDoc::AssertValid( void ) const
{
	CDocument::AssertValid() ;
}

void CAlignerDoc::Dump( CDumpContext &dc ) const
{
	CDocument::Dump( dc ) ;
}
#endif //_DEBUG

BOOL CAlignerDoc::OnSetupSystem( int mode, BOOL showDialog )
{
	BOOL creationStatus = TRUE ;
	
	if (showDialog && !SysSetup->DoModal(mode))
	{
		return FALSE;
	}

	SysSetup->SetMode(mode);
 	if (!SysSetup->IsValid() && theApp.m_DAUPresent /*&& !theApp.IsAligner202Enabled()*/)
	{
		::AfxMessageBox( _T("Software mismatch!") ) ;
		DAU::GetDAU().Clear();
		static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView)->ShowAll(SW_HIDE);
		creationStatus = FALSE;

		SysSetup->SetMode( SYSTEM_SETUP_NO_MODE ) ;
	}

	g_AlignerData.Init();
	DAU::GetDAU().SetOverrangeDetection(TRUE);

	if (SysSetup->GetMode() == SYSTEM_SETUP_MODE_CALIBRATION )
	{
		ResetAllNomAz();	
	}

	static_cast<SystemConfigurationView *>(theApp.m_pSystemConfigurationView)->ShowGeneralConfiguration(SW_SHOW);	
	UpdateAllViews( NULL ) ;

	return creationStatus ;
}

void CAlignerDoc::OnUpdateSetupSystem( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable((theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
}

BOOL CAlignerDoc::GetCanSetupCalibrate( void ) const
{
  if (theApp.IsLiveDataViewActivated())
  {
    return FALSE ;
  }
  return DAU::GetDAU().IsOpen() && (0 < SysSetup->GetOperatorName().GetLength()) && (SYSTEM_SETUP_MODE_CALIBRATION == SysSetup->GetMode()) ;
}

void CAlignerDoc::OnCalibrationSetup( void )
{
    if(DAU::GetDAU().ConfigSensors())
    {
		CString xml;
		m_XMLHandler.GetConfigXML(xml);
		SysSetup->UpdateConfig(xml);
    }
    static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
    UpdateAllViews( NULL ) ;
}

void CAlignerDoc::OnUpdateCalibrationSetup( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanSetupCalibrate() ) ;
}

BOOL CAlignerDoc::GetCanSetupAlign( void ) const
{
  if (theApp.IsLiveDataViewActivated())
  {
    return FALSE ;
  }
  return DAU::GetDAU().IsOpen() && (0 < SysSetup->GetShipName().GetLength()) && (0 < SysSetup->GetOperatorName().GetLength()) && (SysSetup->GetMode() == SYSTEM_SETUP_MODE_ALIGNMENT) ;
}

void CAlignerDoc::OnAlignmentSetup( void )
{
    BOOL result = DAU::GetDAU().ConfigSensors( FALSE ) ;
    if(result)
    {
		CString xml;
		m_XMLHandler.GetConfigXML(xml);
		SysSetup->UpdateConfig(xml);
		

		
		//SaveProject();
    }
    InitGlobalSensorStrings( result ) ;
	static_cast<CLiveDataView *>(theApp.m_pLiveDataView) -> SetShowOnlySelected( FALSE ) ;
	static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
	static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSyncroConfiguration( SW_SHOW );
  static_cast<SystemConfigurationView*>(theApp.m_pSystemConfigurationView)->ShowInfo(SW_SHOW);
    UpdateAllViews( NULL ) ;
}

void CAlignerDoc::OnUpdateAlignmentSetup( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanSetupAlign() ) ;
}
int Test1 ;
// void CAlignerDoc::OnCalibDataExport( void )
// {
// 	CString DbFile, DbExportFile ;
// 	CString StrMsgOK, StrMsgErr, StrCaption ;
// 	CString ExePath ; //, ExeFile ;
// 
// 	ExePath = SysSetup->GetExecutablePath() ;
// //	ExeFile = ExePath + "Supplement\\VersionDbMerge.exe" ;
// //	MergeRunningFile = ExePath + "MergeRunning.txt" ;
// 
// 
// //	DbFile = "C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\\Aligner308.mdb" ;
// 	DbFile = ExePath + "Aligner308.mdb" ;
// //	DbExportFile = "C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\\Aligner308CalibExport.mdb" ;
// 	DbExportFile = ExePath + "Aligner308CalibExport.mdb" ;
// 	StrMsgOK = "Calibration data exported\n\nPath : C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\nFilename : Aligner308CalibExport.mdb" ;
// 	StrMsgErr = "Calibration data export failed" ;
// 	StrCaption = "Export Calibration data" ;
// 
// 	if (CopyFile(DbFile, DbExportFile, False))
// 		AfxMessageBox(StrMsgOK, MB_ICONINFORMATION|MB_OK, 0) ;
// 	else
// 		AfxMessageBox(StrMsgErr, MB_ICONERROR|MB_OK, 0) ;
// }
// 
// void CAlignerDoc::OnUpdateCalibDataExport( CCmdUI *pCmdUI )
// {
//   Test1 = 1 ;
// //  pCmdUI -> Enable( GetCanSetupAlign() ) ;
// }

void CAlignerDoc::OnCalibDataImport( void )
{
	HWND Handle ;
	HINSTANCE HinstRes ;
//	char *ExeFile ;
	CString ExePath, ExeFile ;

	ExePath = SysSetup->GetExecutablePath() ;
	ExeFile = ExePath + "Supplement\\CalibDbMerge.exe" ;

//	ExeFile = "C:\\Program Files\\Schill Reglerteknik AB\\Aligner 308\\Supplement\\CalibDbMerge.exe" ;
	Test1 = 2 ;


	HinstRes = ShellExecute(Handle, "open", ExeFile, NULL, NULL, SW_SHOWNORMAL) ;
}

void CAlignerDoc::OnUpdateCalibDataImport( CCmdUI *pCmdUI )
{
  Test1 = 3 ;
//  pCmdUI -> Enable( GetCanSetupAlign() ) ;
}
// 
// void CAlignerDoc::OnSetupComports( void )
// {
//   if (SYSTEM_SETUP_NO_MODE == SysSetup->GetMode() ||
//       IDOK == ::AfxMessageBox( _T("This will reset channel setup"), MB_OKCANCEL))
//   {
//     DAU::GetDAU().Close() ;
//     if (IDOK == PortSetup::DoModal())
//     {
//         DAU::GetDAU().Close() ;
//         if (SYSTEM_SETUP_NO_MODE != SysSetup->GetMode())
//         {
//             OnSetupSystem( SYSTEM_SETUP_MODE_ALIGNMENT, FALSE ) ;
//         }
//         UpdateAllViews( NULL ) ;
//     }
//   }
// }
// 
// void CAlignerDoc::OnUpdateSetupComports( CCmdUI *pCmdUI )
// {
//     pCmdUI -> Enable( (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
// }

BOOL CAlignerDoc::GetCanSetupA202Channels( void ) const
{
  return (theApp.IsAligner202Enabled() == TRUE) && DAU::GetDAU().IsOpen() && (0 < SysSetup->GetShipName().GetLength()) && (0 < SysSetup->GetOperatorName().GetLength()) ;
}

void CAlignerDoc::OnSetupA202EditChannels( void )
{
  BOOL result = DAU::GetDAU().ConfigSensors( FALSE ) ;
  if(result)
  {
      SaveProject();
  }
  InitGlobalSensorStrings( TRUE ) ;
  static_cast<LiveDataViewA202 *>(theApp.m_pLiveDataView) -> SetShowOnlySelected( FALSE ) ;
  static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSyncroConfiguration( SW_SHOW );
  UpdateAllViews( NULL ) ;
}

void CAlignerDoc::OnUpdateSetupA202EditChannels( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanSetupA202Channels() && (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
}

void CAlignerDoc::OnSetupA202LoadChannels()
{
//     CFile theFile;
// 	static char BASED_CODE szFilter[] = "Channel Setup Files (*.dat)|*.dat|";
// 	CFileDialog dlg(TRUE,NULL,NULL,OFN_NOCHANGEDIR,szFilter);
//     if( m_ChannelSetupFileDir.Find(".") == 0 )
//     { // the string starts with "." 
//         char ca [MAX_PATH];
//         GetCurrentDirectory( sizeof(ca), ca );
//         m_ChannelSetupFileDir = ca;
//         m_ChannelSetupFileDir += _T("\\");
//     }
// 
//     dlg.m_ofn.lpstrInitialDir = m_ChannelSetupFileDir;
//     dlg.m_ofn.lpstrDefExt = "dat";
// 
// 	if (dlg.DoModal() == IDOK)
// 	{
//         CString oldFileDir = m_ChannelSetupFileDir;
// 		m_ChannelSetupFileDir = dlg.GetPathName();
//         m_ChannelSetupFileName = dlg.GetFileName();
// 
//         int iPathLength = m_ChannelSetupFileDir.GetLength() - m_ChannelSetupFileName.GetLength();
//         if( iPathLength >= 0 ) 
//         {
//             m_ChannelSetupFileDir.GetBufferSetLength( iPathLength );
//         }
//         else 
//         {
//             m_ChannelSetupFileDir = oldFileDir;
//         }
// 
//         if( theFile.Open( m_ChannelSetupFileDir + m_ChannelSetupFileName, CFile::modeRead, NULL ) ) 
//         {
//             CArchive archive(&theFile, CArchive::load);
//             Serialize(archive, SERIALIZE_SETUP);	      
//             archive.Close();
//             theFile.Close();	
//             InitGlobalSensorStrings( TRUE ) ;
//             static_cast<LiveDataViewA202 *>(theApp.m_pLiveDataView) -> SetShowOnlySelected( FALSE ) ;
//             static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSyncroConfiguration( SW_SHOW );
//             UpdateAllViews( NULL ) ;
//         }
//         else 
//         {
//             AfxMessageBox( IDS_COULDNT_LOAD_CHANNEL_SETUP );
//         }
//     }
}

void CAlignerDoc::OnUpdateSetupA202LoadChannels(CCmdUI *pCmdUI)
{
  pCmdUI -> Enable( GetCanSetupA202Channels() && (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
}

void CAlignerDoc::OnSetupA202SaveChannels()
{
//   CFile theFile;
// 	static char BASED_CODE szFilter[] = "Channel Setup Files (*.dat)|*.dat|";
// 	CFileDialog dlg(FALSE,_T("dat"),NULL,OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT,szFilter);
//   dlg.m_ofn.lpstrInitialDir = m_ChannelSetupFileDir;
// 
// 	if (dlg.DoModal() == IDOK)
// 	{
//     CString oldFileDir = m_ChannelSetupFileDir;
// 		m_ChannelSetupFileDir = dlg.GetPathName();
//     m_ChannelSetupFileName = dlg.GetFileName();
// 
//     int iPathLength = m_ChannelSetupFileDir.GetLength() - m_ChannelSetupFileName.GetLength();
//     if( iPathLength >= 0 ) 
//     {
//       m_ChannelSetupFileDir.GetBufferSetLength( iPathLength );
//     }
//     else 
//     {
//       m_ChannelSetupFileDir = oldFileDir;
//     }
// 
//     if( theFile.Open( m_ChannelSetupFileDir + m_ChannelSetupFileName, CFile::modeCreate | CFile::modeWrite ) ) 
//     {
//       CArchive archive(&theFile, CArchive::store);
//       Serialize(archive, SERIALIZE_SETUP);
//       archive.Close();
//       theFile.Close();	
//     }
//     else 
//     {
//       AfxMessageBox( IDS_COULDNT_SAVE_CHANNEL_SETUP );
//     }
//   }
}

void CAlignerDoc::OnUpdateSetupA202SaveChannels(CCmdUI *pCmdUI)
{
  pCmdUI -> Enable( GetCanSetupA202Channels() && (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
}

void CAlignerDoc::OnCalibrationCreatetemperaturemodel( void )
{
	CreatePolynomial dialog ;
	dialog.DoModal() ;
 
	LoadSensorCalibration();	
}

void CAlignerDoc::OnUpdateCalibrationCreatetemperaturemodel( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( GetCanSetupCalibrate() ) ;
}



void CAlignerDoc::OnUpdateProjectCalibration( CCmdUI *pCmdUI )
{  
    BOOL inConfig = FALSE;
    if(theApp.m_pSystemConfigurationView)
        inConfig = static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->IsWindowVisible( );
    
    pCmdUI -> Enable( inConfig && !IsModified() && theApp.m_DAUPresent) ;
//    pCmdUI -> SetCheck( SYSTEM_SETUP_MODE_CALIBRATION == SysSetup->GetMode() ) ;
}

void CAlignerDoc::OnUpdateFileOpen( CCmdUI *pCmdUI )
{
    BOOL inConfig = FALSE;
    if(theApp.m_pSystemConfigurationView)
        inConfig = static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->IsWindowVisible( );
    
    pCmdUI -> Enable( inConfig && !IsModified() ) ; 
}

void CAlignerDoc::OnUpdateFileNew( CCmdUI *pCmdUI )
{  
    BOOL inConfig = FALSE;
    if(theApp.m_pSystemConfigurationView)
        inConfig = static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->IsWindowVisible( );

    pCmdUI->Enable(inConfig && !IsModified() && theApp.m_DAUPresent) ;
}

void CAlignerDoc::OnUpdateFileSave( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( IsModified() ) ;
}

void CAlignerDoc::OnSetupSystem( void )
{
  OnSetupSystem( SYSTEM_SETUP_MODE_CALIBRATION ) ;
}

void CAlignerDoc::OnUtilitiesViewreport( void )
{
	ReportManager rm;
	rm.OpenReport(SysSetup->GetProjectID(), -1);

/*    DBInterface::Instance()->ClearTable("HistoryPrint");
    
    for (std::set<LONG>::const_iterator i = m_projectMeasurments.begin() ; i != m_projectMeasurments.end() ; i++)
    {
        DBInterface::Instance()->InsertHistoryPrintItem(*i);
    }
    
    CResultTable rs;
    rs.OpenMainReport();
	*/
}

void CAlignerDoc::OnUpdateUtilitiesViewreport( CCmdUI *pCmdUI )
{
  if( theApp.IsAligner202Enabled() == TRUE )
  {
    pCmdUI -> Enable( (theApp.IsLiveGraphViewActivated() == FALSE) && (theApp.IsLiveDataViewActivated() == FALSE) ) ;
  }  
  else
  {
    pCmdUI -> Enable(SysSetup->IsOpen() && /*(SysSetup->GetNumMeasurements() > 0) &&*/ SYSTEM_SETUP_MODE_CALIBRATION != SysSetup->GetMode()) ;
  }
}

/*void CAlignerDoc::AddMeasurment( LONG measurmentID )
{
  TRACE("Adding report number %d\n", measurmentID) ;
  if( measurmentID > 0 )
  {
    m_projectMeasurments.insert( measurmentID ) ;
//    SetModifiedFlag( TRUE ) ;
    m_XMLHandler.Store(m_strPathName);
    //DoSave( m_strPathName, TRUE ) ;
  }
}

void CAlignerDoc::RemoveMeasurment( LONG measurmentID )
{
  TRACE("Removing report number %d\n", measurmentID) ;
  if( measurmentID > 0 )
  {
    m_projectMeasurments.erase( measurmentID ) ;
    //SetModifiedFlag( TRUE ) ;
    m_XMLHandler.Store(m_strPathName);
    //DoSave( m_strPathName, TRUE ) ;
  }
}
*/
void CAlignerDoc::SaveProject( void )
{
    //SetModifiedFlag( TRUE ) ;
    m_XMLHandler.Store(m_strPathName);
    //DoSave( m_strPathName, TRUE ) ;
}

void CAlignerDoc::OnCalibrationDauoffset( void )
{
  //DAU::GetDAU().Start() ;
  //CDAUCalibrationWizard wizard( /*DAU_MODE::*/Offset ) ;
  //if (IDOK == wizard.DoModal())
  //{
  //  OnSetupSystem( SYSTEM_SETUP_MODE_CALIBRATION, FALSE ) ;
  //}
  //DAU::GetDAU().Stop() ;
  DAUOffsetCalibration().DoModal() ;
}

void CAlignerDoc::OnUpdateCalibrationDauoffset( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( theApp.GetCanCalibrate() && 0 == DAU::GetDAU().GetConnectedSensorCount()) ;
}

void CAlignerDoc::OnCalibrationDaugain( void )
{
  DAUScaleCalibration().DoModal() ;
}

void CAlignerDoc::OnUpdateCalibrationDaugain( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( theApp.GetCanCalibrate() && 0 == DAU::GetDAU().GetConnectedSensorCount()) ;
}

void CAlignerDoc::OnCalibrationOffset( void )
{
  TiltOffsetCalibration wizard( IDS_TILT_CALIBRATION_SETUP_TITLE ) ;
  wizard.SetSource( &DAU::GetDAU() ) ;
  wizard.DoModal() ;
}

void CAlignerDoc::OnUpdateCalibrationOffset( CCmdUI *pCmdUI )
{
	pCmdUI -> Enable( theApp.GetCanCalibrate() && 2 <= DAU::GetDAU().GetConnectedSensorCount( UnitType::Fixed )) ;
}

void CAlignerDoc::OnCalibrationGain( void )
{
  GainAzimuthCalibration wizard(  IDS_TILT_CALIBRATION_SETUP_TITLE, FALSE ) ;
  wizard.SetSource( &DAU::GetDAU() ) ;
  wizard.DoModal() ;
}

void CAlignerDoc::OnUpdateCalibrationGain( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable( theApp.GetCanCalibrate() && 2 <= DAU::GetDAU().GetConnectedSensorCount( UnitType::Fixed )) ;
}

void CAlignerDoc::OnCalibrationAdapter( void )
{
	AdapterCalibration wizard( IDS_ADAPTER_CALIBRATION_TITLE ) ;
	//if (IDOK != wizard.DoModal())
	if (ID_WIZFINISH != wizard.DoModal())
	{
	//return ;  //don't return here, continue and reload the calibration data.
				//The gun adapter data has been resetted for the selected gun adapter and must be reloaded
	}

	LoadSensorCalibration();

	//refresh the SystemConfigurationView, gun adapter data maybe changed
	static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
	UpdateAllViews( NULL ) ;
}

void CAlignerDoc::OnUpdateCalibrationAdapter( CCmdUI *pCmdUI )
{
	pCmdUI -> Enable( theApp.GetCanCalibrate() && DAU::GetDAU().GetConnectedSensorCount( UnitType::Gun )  >= 1 && DAU::GetDAU().GetConnectedSensorCount( UnitType::Fixed ) >= 1) ;
}


void CAlignerDoc::OnCalibrationTheoadapter()
{
	TheoAdapterCalibration wizard( IDS_ADAPTER_CALIBRATION_TITLE ) ;
	
	if (ID_WIZFINISH != wizard.DoModal())
	{
    //return ;  //don't return here, continue and reload the calibration data.
                //The gun adapter data has been resetted for the selected gun adapter and must be reloaded
	}
	
	LoadSensorCalibration();

	//refresh the SystemConfigurationView, gun adapter data maybe changed
	static_cast<SystemConfigurationView *>( theApp.m_pSystemConfigurationView )->ShowSensorConfiguration( SW_SHOW );
	UpdateAllViews( NULL ) ;
}


void CAlignerDoc::OnUpdateCalibrationTheoadapter(CCmdUI *pCmdUI)
{
	pCmdUI -> Enable( !theApp.GetCanCalibrate() && DAU::GetDAU().GetConnectedSensorCount( UnitType::Theo )  >= 1 && DAU::GetDAU().GetConnectedSensorCount( UnitType::Fixed ) >= 1) ;
}



void CAlignerDoc::OnUtilitiesViewcalibrationdata( void )
{
	ReportManager rm;
	rm.OpenCalibrationData(SysSetup->GetProjectID());


    //CResultTable rs;
    //rs.OpenCalibrationReport();
    //CalibrationReportReport::Create() ;
}

void CAlignerDoc::OnUpdateUtilitiesViewcalibrationdata( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable(SysSetup->IsOpen()) ;
}

void CAlignerDoc::OnCalibrationDailyazimutherrortest()
{
	CheckAndResetMeasurementSetups(SensorValidationTest);
	GainAzimuthCalibration wizard(  IDS_DAILY_AZIMUTH_ERROR_TEST_SETUP_TITLE ) ;
	wizard.SetSource( &DAU::GetDAU() ) ;
	wizard.DoModal() ;
  
	LoadSensorCalibration();
    
}

void CAlignerDoc::OnUpdateCalibrationDailyazimutherrortest( CCmdUI *pCmdUI )
{
  pCmdUI -> Enable(!theApp.IsLiveDataViewActivated() && 2 <= DAU::GetDAU().GetConnectedSensorCount() && theApp.GetCanAlign()) ;
}

void CAlignerDoc::OnUtilitiesComsetup()
{
    ComSettingsDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        DAU::GetDAU().UpdateSyncroSettings();
    }
}



