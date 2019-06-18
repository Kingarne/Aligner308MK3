#pragma  once


#define NO_REF NULL

class DAU;

class DervFilter
{
public:
    DervFilter(){m_U=0;m_ts=1.0f/30.0f;m_K=10.0f;}
    void SetPeriod(double ts){m_ts = ts;}    
    void Reset(){m_U=0.0f;m_first=TRUE;}
    double Add(double val);
    double GetK(){return m_K;}
    void SetK(double k){m_K = k;}
protected:
    double m_U;
    double m_ts;
    double m_K;    
    BOOL m_first;
};

class DataCompensator
{
public:
	DataCompensator();

	DAUValues GetDAUData(DAU *pDau);
    BOOL Init();
    void Deactivate(){m_active=FALSE;}
    BOOL GetActivate(){return m_active;}
    BOOL GetEnable(){return m_enable;}
    void SetEnable(BOOL b);
    void SetK(double k){m_rollFilter.SetK(k);m_pitchFilter.SetK(k);}
    void SetMaxDataDiff(double maxDiff){m_maxDiff=maxDiff;}

    DervFilter m_rollFilter;
    DervFilter m_pitchFilter;
protected:

	double m_rollComp;
	double m_pitchComp;
    BOOL m_active;
    BOOL m_enable;    

    Syncro* m_pSyncroRef;
    Sensor* m_pSensorRef1;
    Sensor* m_pSensorRef2;
   
    double m_maxDiff;     
    void CalcCompensation();
};