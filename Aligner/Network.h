// $Id: Network.h,v 1.1 2014-04-22 09:16:58 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#define FEAT_ANALYSIS 0x01
#define FEAT_TRACKING 0x02

class Features
{	
public:

	static BOOL Can(int feature){return ((m_featureKey & feature)==feature);}
	static BOOL CheckNetwork( void ) ;
	static BOOL CheckCPU( );
	static BOOL CheckString(CString str );
	static BOOL CheckFeatures() ;
	static UINT m_featureKey;
private:
	
	static BOOL CheckAdapter( PIP_ADAPTER_INFO pAdapter ) ;
	static BOOL CheckAdapterTrippleDES( PIP_ADAPTER_INFO pAdapter ) ;	
	
};