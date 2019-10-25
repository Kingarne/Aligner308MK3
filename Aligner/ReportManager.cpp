#include "StdAfx.h"
#include "ReportManager.h"


//#import "..\ReporterLib\ReporterLib\bin\Debug\ReporterLib.tlb" raw_interfaces_only
#import "..\ReporterLib\ReporterLib\bin\Release\ReporterLib.tlb" raw_interfaces_only
using namespace ReporterLib;

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
	BSTR str = L"I am a happy BSTR";
	reporter->OpenReport(projectId, measId, &l);
	//c2.Add(100, 100);

	return 0;
}

int ReportManager::OpenCalibrationData(int projectId)
{	
	ReporterIPtr reporter(__uuidof(Reporter));

	long l;
	reporter->OpenCalibrationData(projectId, &l);	

	return 0;
}