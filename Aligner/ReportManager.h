#pragma once



class ReportManager
{
public:
	ReportManager();
	~ReportManager();

	static CString dbPath;

	int OpenReport(int projectID, int measId);
	int OpenCalibrationData(int projectID);
};

