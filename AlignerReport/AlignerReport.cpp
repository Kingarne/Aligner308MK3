// $Id: AlignerReport.cpp,v 1.5 2014-07-11 09:30:52 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"

#include "AlignerReport.h"
#include "comdef.h"

#using <mscorlib.dll>

#using "System.dll" 
#using "System.Windows.Forms.dll" 
#using "System.Drawing.dll" 
#using "CrystalDecisions.Windows.Forms.dll"
#using "CrystalDecisions.CrystalReports.Engine.dll"
#using "CrystalDecisions.Shared.dll"
 
//***   stlo , lines below commented 090416
using namespace System ;
using namespace System::IO ;
using namespace System::ComponentModel ;
using namespace System::Drawing ;
using namespace System::Collections ;
using namespace System::Windows::Forms ;
using namespace System::Diagnostics;
using namespace CrystalDecisions::Windows::Forms ;
using namespace CrystalDecisions::CrystalReports::Engine ;
using namespace CrystalDecisions::Shared ;


#if defined(MessageBox)
#undef MessageBox
#endif

#if defined(GetCurrentDirectory)
#undef GetCurrentDirectory
#endif

#if defined(new)
#undef new
#endif

__gc class ReportForm : public System::Windows::Forms::Form
{
private:
	static Hashtable *reportTable = new Hashtable ;
public:
    static void SetReportDirectory( std::string &directory ) ;
	static void CloseReport( LONG id ) ;
	static void UpdateReport( LONG id ) ;
	static BOOL IsOpen( LONG id ) ;
//	static String* GetExeFileName( void ) ;
private:
	CrystalReportViewer *pViewer ;
	ReportDocument *pDocument ;
	String *reportSource ;
	LONG m_id ;
    static String *reportDirectory ;
public:
	ReportForm( LONG id, String *reportFile )
	{       
        m_id = id ;
		Name = "ReportForm" ;
		Text = "Result Table" ;
        reportSource = String::Concat( String::Copy( reportDirectory ), reportFile ) ;
		reportTable -> Add( __box(m_id), this ) ;		
        pViewer = new CrystalReportViewer() ;
//        Beep(1000,100);
// 		Trace::WriteLine(S"Create report");
//         Trace::WriteLine(reportSource);
        pDocument = new ReportDocument() ;
		pDocument->Load( reportSource ) ;
		pDocument->SetParameterValue( "historyID", __box( m_id ) ) ;
		SuspendLayout() ;
		__hook( &CrystalDecisions::Windows::Forms::CrystalReportViewer::ReportRefresh, pViewer, &ReportForm::ReportWasRefreshed ) ;
		pViewer -> ShowRefreshButton = FALSE ;
		pViewer -> ActiveViewIndex = -1 ;
		pViewer -> Dock = System::Windows::Forms::DockStyle::Fill ;
		pViewer -> Name = "ReportViewer" ;
		pViewer -> ReportSource = pDocument ;
		pViewer -> Size = System::Drawing::Size(100,100);//( 292, 273 ) ;
		pViewer -> TabIndex = 0 ;
		WindowState = System::Windows::Forms::FormWindowState::Normal ;
		AutoScaleBaseSize = System::Drawing::Size( 5, 13 ) ;
		ClientSize = System::Drawing::Size( 700, 750 ) ;
		Controls -> Add( pViewer ) ;
//		Trace::WriteLine(S"Hook");
        __hook( &System::Windows::Forms::Form::Closed, this, &ReportForm::RemoveReport ) ;
        PageMargins margins = pDocument -> PrintOptions -> PageMargins ;
        margins.bottomMargin = 4 * margins.bottomMargin ;
        margins.leftMargin = 4 * margins.leftMargin ;
        margins.topMargin = 4 * margins.topMargin ;
        pDocument -> PrintOptions -> ApplyPageMargins( margins ) ;	
        StartPosition = FormStartPosition::Manual;
	    Location = Point(0,0);
        ResumeLayout( false ) ;
        TopMost = true;
//        Trace::WriteLine(S"Done");
    }
	ReportForm( String *reportFile )
	{
        //Beep(440,50);	
        Name = "ReportForm" ;
		Text = "Result Table" ;
        reportSource = String::Concat( String::Copy( reportDirectory ), reportFile ) ;
        pViewer = new CrystalReportViewer() ;
		pDocument = new ReportDocument() ;
		pDocument -> Load( reportSource ) ;
		SuspendLayout() ;
		pViewer -> ShowRefreshButton = FALSE ;
		pViewer -> ActiveViewIndex = -1 ;
		pViewer -> Dock = System::Windows::Forms::DockStyle::Fill ;
		pViewer -> Name = "ReportViewer" ;
		pViewer -> ReportSource = pDocument ;
		pViewer -> Size = System::Drawing::Size( 292, 273 ) ;
		pViewer -> TabIndex = 0 ;        
		WindowState = System::Windows::Forms::FormWindowState::Normal ;
		AutoScaleBaseSize = System::Drawing::Size( 5, 13 ) ;
		ClientSize = System::Drawing::Size( 390, 520 ) ;
		Controls -> Add( pViewer ) ;
        pDocument -> ExportOptions -> ExportFormatType = ExportFormatType::WordForWindows ;
        pDocument -> ExportOptions -> ExportDestinationType = ExportDestinationType::DiskFile ;
        DiskFileDestinationOptions *pDiskOptions = new DiskFileDestinationOptions ;
        pDiskOptions -> DiskFileName = String::Concat( Directory::GetCurrentDirectory(), L"Report.doc" ) ;
        pDocument -> ExportOptions -> DestinationOptions = pDiskOptions ;
        PageMargins margins = pDocument -> PrintOptions -> PageMargins ;
        margins.bottomMargin = 4 * margins.bottomMargin ;
        margins.leftMargin = 3 * margins.leftMargin ;
        margins.topMargin = 4 * margins.topMargin ;
        pDocument -> PrintOptions -> ApplyPageMargins( margins ) ;
        ResumeLayout( false ) ;
//         Trace::WriteLine(S"Create report(2)");
// 	    Trace::WriteLine(reportSource);
        pViewer -> RefreshReport() ;
    }

public:
	void UpdateReport( void )
	{
		pDocument -> Load( reportSource ) ;
		pDocument -> SetParameterValue( "historyID", __box( m_id ) ) ;
		pViewer -> RefreshReport() ;
		Invalidate() ;
	}

private:
	void ReportWasRefreshed( Object *sender, ViewerEventArgs *e )
	{
		try
		{
			ParameterDiscreteValue *pValue = __try_cast<ParameterDiscreteValue *>(pViewer -> ParameterFieldInfo -> Item [ "historyID" ] -> CurrentValues -> Item [0]) ;
			pDocument -> Refresh() ;
			e -> Handled = true ;
		}
		catch (...)
		{
		}
	} ;

private:
	void RemoveReport( Object *sender, EventArgs *e )
	{

		try
		{
			reportTable -> Remove( __box(m_id) ) ;   
		}
		catch (...)
		{
			// Empty
		}
	}
} ;

void ReportForm::SetReportDirectory( std::string &directory )
{
    reportDirectory = new String(directory.c_str()) ;

//     Trace::Listeners->Add(new TextWriterTraceListener(Console::Out));
//     Trace::AutoFlush = true;
//     Trace::Indent();
}

void ReportForm::CloseReport( LONG id )
{
	ReportForm *pReport = __try_cast<ReportForm *>(reportTable -> Item [__box(id)]) ;
	if (NULL != pReport)
	{
		pReport -> Close() ;
	}
}

void ReportForm::UpdateReport( LONG id )
{
	ReportForm *pReport = __try_cast<ReportForm *>(reportTable -> Item [__box(id)]) ;
	if (NULL != pReport)
	{
		pReport -> UpdateReport() ;
	}
//    Trace::WriteLine(S"Update report");
}

BOOL ReportForm::IsOpen( LONG id )
{
	ReportForm *pReport = __try_cast<ReportForm *>(reportTable -> Item [__box(id)]) ;

//    Trace::WriteLine(S"Is open");

    return NULL != pReport ;
}
//String* ReportForm::GetExeFileName( void )
//{
////	char* S1 ;
////	String S1 ;
////	System::String S2 ;
////new (S1) "fw" ;
////	new String ("S1") ;
////	String* S1 = S"sdf" /* System::Windows::Forms::Application::ExecutablePath ;*/ ;
//String* S1 = System::Windows::Forms::Application::ExecutablePath ;
//return S1 ;
//}
// void name##Report::Create( LONG id, HANDLE &handle ) { \
//     try { \
//     ReportForm *pForm = new ReportForm( id, new String(reportFile) ) ; \
//     pForm -> Show() ; \
//     handle = (HANDLE)(pForm -> Handle) ; \
//     } catch (...) { \
//     } \
// } \


#define IMPLEMENT_REPORT(name,reportFile) \
	void name##Report::Create( LONG id, HANDLE &handle ) { \
	ReportForm *pForm = new ReportForm( id, new String(reportFile) ) ; \
	pForm -> Show() ; \
	handle = (HANDLE)(pForm -> Handle) ; \
	} \
	void name##Report::Create( LONG id ) { \
	try { \
	ReportForm *pForm = new ReportForm( id, new String(reportFile) ) ; \
	pForm -> ShowDialog() ; \
	} catch (...) { } \
	} \
	void name##Report::Create( void ) { \
	try { \
	ReportForm *pForm = new ReportForm( new String(reportFile) ) ; \
	pForm -> ShowDialog() ; \
	} catch (...) { } \
	} \
	void name##Report::Close( LONG id ) { \
	try { \
	ReportForm::CloseReport( id ) ; \
	} catch (...) { } \
	}  \
	void name##Report::Update( LONG id ) { \
	try { \
	ReportForm::UpdateReport( id ) ; \
	} catch (...) { } \
	} \
	BOOL name##Report::IsOpen( LONG id ) { \
	try { \
	return ReportForm::IsOpen( id ) ; \
	} catch (...) {return FALSE; } \
	} 
	//String* name##Report::GetExeFileName( void ) { \
	//try { \
	//return ReportForm::GetExeFileName(  ) ; \
	//} catch (...) { } \
	//}

IMPLEMENT_REPORT(AzimuthAlignmentErrors, "AzimuthAlignError.rpt") ;
IMPLEMENT_REPORT(AzimuthVerificationBenchmark, "AzimuthVerificationBenchmark.rpt") ;
IMPLEMENT_REPORT(AzimuthVerificationGyrostab, "AzimuthVerificationGyrostab.rpt") ;
IMPLEMENT_REPORT(CommonFlatAzimuth, "CommonFlatAzimuth.rpt") ;
IMPLEMENT_REPORT(CommonFlatTilt, "CommonFlatTilt.rpt") ;
IMPLEMENT_REPORT(SensorValidationTest, "SensorValidationTest.rpt");
IMPLEMENT_REPORT(GyroPerformanceTest, "Gyroperformance.rpt") ;
IMPLEMENT_REPORT(HorizonAbsoluteMode, "HorizonAbsoluteMode.rpt") ;
IMPLEMENT_REPORT(HorizonRelativeMode, "HorizonRelativeMode.rpt") ;
IMPLEMENT_REPORT(TiltAndFlatness, "Tilt-Flatness.rpt") ;
IMPLEMENT_REPORT(TiltAndFlatnessFo, "Tilt-FlatnessFo.rpt") ;
IMPLEMENT_REPORT(TiltAlignmentErrors, "TiltAlignErrors.rpt") ;
IMPLEMENT_REPORT(LiveGraphErrors, "LiveGraphErrors.rpt") ;
IMPLEMENT_REPORT(LiveDataA202Errors, "LiveDataA202Errors.rpt") ;
IMPLEMENT_REPORT(MainReportA202, "MainReportA202.rpt") ;
IMPLEMENT_REPORT(MainReport, "MReport.rpt") ;
IMPLEMENT_REPORT(CalibrationReport, "Calibration.rpt") ;

BOOL ALIGNERREPORT_API CreateDirectoryPath( std::string &directory )
{
	BOOL result = TRUE ;
	try 
	{
		DirectoryInfo *directoryInformation = new DirectoryInfo( directory.c_str() ) ;
		if (FALSE == directoryInformation -> Exists)
		{
			directoryInformation -> Create() ;
		}
	} 
	catch (Exception *e) 
	{
		(void)e ;
		result = FALSE ;
	} 
	return result ;
}

void ALIGNERREPORT_API SetReportDirectory( std::string &directory )
{
    ReportForm::SetReportDirectory( directory ) ;
}
