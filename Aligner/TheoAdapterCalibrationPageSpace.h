#pragma once


// TheoAdapterCalibrationPageSpace dialog

class TheoAdapterCalibrationPageSpace : public CPropertyPage
{
	DECLARE_DYNAMIC(TheoAdapterCalibrationPageSpace)

public:
	TheoAdapterCalibrationPageSpace();
	virtual ~TheoAdapterCalibrationPageSpace();

// Dialog Data
	enum { IDD = IDD_THEO_ADAPTER_CALIBRATION_PAGE_SPACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnSetActive();
};
