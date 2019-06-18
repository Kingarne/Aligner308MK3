#pragma once


// AdapterCalibrationPage10 dialog

class AdapterCalibrationPage10 : public CPropertyPage
{
	DECLARE_DYNAMIC(AdapterCalibrationPage10)

public:
	AdapterCalibrationPage10();
	virtual ~AdapterCalibrationPage10();

// Dialog Data
	enum { IDD = IDD_ADAPTER_CALIBRATION_PAGE_10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnSetActive();
};
