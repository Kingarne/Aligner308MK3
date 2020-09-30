#include "StdAfx.h"
#include "ReportManager.h"


//#import "..\ReporterLib\ReporterLib\bin\Debug\ReporterLib.tlb" raw_interfaces_only
#import "..\ReporterLib\ReporterLib\bin\Release\ReporterLib.tlb" raw_interfaces_only
using namespace ReporterLib;

CString ReportManager::dbPath="";

ReportManager::ReportManager()
{
	CoInitialize(NULL);	
}


ReportManager::~ReportManager()
{
}


int ReportManager::OpenReport(int projectId, int measId)
{
	
	//Class1Ptr ic(__uuidof(Class2));
	ReporterIPtr reporter(__uuidof(Reporter));

	long l;
	//ic->Add(0,0,&l);
	//Class2 c2;
	//char* str = "I am a happy BSTR";
	TRACE("%s\n",ReportManager::dbPath);
	//int len = strlen(str);
	reporter->SetDBPath((unsigned char*)(LPCTSTR)dbPath, dbPath.GetLength());
	reporter->OpenReport(projectId, measId, &l);
	//c2.Add(100, 100);

	return l;
}

int ReportManager::OpenCalibrationData(int projectId)
{	
	ReporterIPtr reporter(__uuidof(Reporter));

	long l;
	reporter->SetDBPath((unsigned char*)(LPCTSTR)dbPath, dbPath.GetLength());
	reporter->OpenCalibrationData(projectId, &l);	

	return 0;
}