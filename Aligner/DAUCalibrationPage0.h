#pragma once

class DAUCalibrationPage0 : public CPropertyPage
{
	DECLARE_DYNAMIC(DAUCalibrationPage0)
	DECLARE_MESSAGE_MAP()
public:
	DAUCalibrationPage0( CWnd *pParent = NULL ) ;
	virtual ~DAUCalibrationPage0( void ) ;
private:
  enum { IDD = IDD_DAU_CALIBRATION_PAGE_0 } ;
protected:
	virtual void DoDataExchange( CDataExchange *pDX ) ;
  virtual BOOL OnInitDialog( void ) ;
  virtual LRESULT OnWizardNext( void ) ;
private:
  afx_msg void OnBnClickedSelect( void ) ;
  afx_msg void OnBnClickedDeselect( void ) ;
  afx_msg void OnBnClickedSelectAll( void ) ;
  afx_msg void OnBnClickedDeselectAll( void ) ;
  afx_msg void OnLbnSelchangeAvailable( void ) ;
  afx_msg void OnLbnSelchangeSelected( void ) ;
private:
  void HandleSelectionChange( void ) ;
private:
  CListBox m_availableControl ;
  CListBox m_selectedControl ;
  CButton m_selectControl ;
  CButton m_deselectControl ;
  CButton m_selectAllControl ;
  CButton m_deselectAllControl ;
} ;
