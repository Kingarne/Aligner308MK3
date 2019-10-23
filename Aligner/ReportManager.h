#pragma once



class ReportManager
{
public:
	ReportManager();
	~ReportManager();

	int OpenReport(int projectID, int measId);
	int OpenCalibrationData(int projectID);
};

