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
	ReporterIPtr reporter(__uuidof(Reporter));

	Registry reg;
	CString path;
	CString regPath = reg.GetKeyPath(path);	

	long l;
	//TRACE("%s\n",ReportManager::dbPath);
	
	reporter->SetRegPath((unsigned char*)(LPCTSTR)regPath, regPath.GetLength());
	reporter->SetDBPath((unsigned char*)(LPCTSTR)dbPath, dbPath.GetLength());
	reporter->OpenReport(projectId, measId, &l);
	
	return l;
}

int ReportManager::OpenCalibrationData(int projectId)
{	
	ReporterIPtr reporter(__uuidof(Reporter));
	
	Registry reg;
	CString path;
	CString regPath = reg.GetKeyPath(path);

	long l;
	reporter->SetRegPath((unsigned char*)(LPCTSTR)regPath, regPath.GetLength());
	reporter->SetDBPath((unsigned char*)(LPCTSTR)dbPath, dbPath.GetLength());
	reporter->OpenCalibrationData(projectId, &l);	

	return 0;
}