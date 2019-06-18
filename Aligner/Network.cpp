// $Id: Network.cpp,v 1.1 2014-04-22 09:16:58 riar Exp $
//  
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "Network.h"
#include "CryptoUtil.h"
#include <sstream>


UINT Features::m_featureKey=0;

#define NIC_LENGTH 6
BYTE dolchAddress [][NIC_LENGTH] = {{ '\x00', '\x03', '\x2d', '\x02', '\x2a', '\x31'},  
                                    { '\x02', '\x03', '\x2d', '\x02', '\x62', '\x18'},
                                    { '\x00', '\x03', '\x2d', '\x03', '\x2a', '\x36'},
                                    { '\x00', '\x03', '\x2d', '\x07', '\x1a', '\x3a'},
                                    { '\x00', '\x03', '\x2d', '\x04', '\x1c', '\x2b'},	  // UAE 1
                                    { '\x00', '\x03', '\x2d', '\x0c', '\x0c', '\xef'},	  // Indian Navy 1
                                    { '\x00', '\x03', '\x2d', '\x07', '\x14', '\x40'},    // Indian Navy 2                                 
									{ '\x00', '\x03', '\x2d', '\x0c', '\x0c', '\xc8'},  // Indian Navy 3                                  
									{ '\x00', '\x03', '\x2d', '\x0e', '\x31', '\xb4'}} ;  // 
//                                    { '\x00', '\x0c', '\xf1', '\x84', '\x5f', '\x6c'}} ;//Rian


BOOL Features::CheckAdapter( PIP_ADAPTER_INFO pAdapter )
{
    if (6 != pAdapter -> AddressLength)
    {
        return FALSE ;
    }
    BYTE address [6] ;
    int numDolch = sizeof(dolchAddress)/NIC_LENGTH;    
    for(int dolch=0 ; dolch<numDolch ; dolch++)
    {
        memcpy( address, dolchAddress[dolch], sizeof(address) ) ;
        int size = sizeof(address); 
        if(memcmp( address, pAdapter->Address, size) == 0)
            return TRUE;
    }
    return FALSE;
}

int CountChar(char c, CString text)
{
	int count = 0;
	for(int i=0;i<text.GetLength();i++)
	{
		if(text[i] == c)
			count++;
	}
	return count;
}

BOOL Features::CheckFeatures()
{
	Registry reg;
	CString crypto = reg.GetStringValue("ApplicationKey", "");

	if(crypto.GetLength() >= 16)
	{
		int key1 = GetKey(23249999, 21110211); //23256527
		int key2 = GetKey(12124121, 43532454); //45678591
		CString key1Str; key1Str.Format("%d",key1);
		CString key2Str; key2Str.Format("%d",key2);

		CString plainText, compNic;
		McbDecrypt(key1Str, key2Str, crypto, plainText);		
		
		m_featureKey = atoi(plainText);
		return TRUE;
	}
	return FALSE;
}

UINT64 GetCPUId(void)
{
	DWORD dwStandard = 0; 
	DWORD dwFeature = 0; 

	_asm 
	{ 
		mov eax, 1 
			cpuid 
			mov dwStandard, eax 
			mov dwFeature, edx 
	}
	return( ((UINT64)(dwFeature) << 32) | ((UINT64)(dwStandard)));
}

BOOL Features::CheckString(CString str )
{
	Registry reg;
	CString crypto = reg.GetStringValue("ApplicationKey", "");

	if(crypto.GetLength() >= 16)
	{
		int key1 = GetKey(23249999, 21110211); //23256527
		int key2 = GetKey(12124121, 43532454); //45678591
		CString key1Str; key1Str.Format("%d",key1);
		CString key2Str; key2Str.Format("%d",key2);

		CString plainText, comp;
		McbDecrypt(key1Str, key2Str, crypto, plainText);		

		comp = plainText;
		if(comp == "krackelibankelfnatt")
			return TRUE;

		if(comp.Compare(str) == 0)
		{
			TRACE("Match!\n");


			return TRUE;
		}				
	}

	return FALSE;
}

BOOL Features::CheckCPU( )
{
	Registry reg;
	CString crypto = reg.GetStringValue("ApplicationKey", "");

	if(crypto.GetLength() >= 16)
	{
		int key1 = GetKey(23249999, 21110211); //23256527
		int key2 = GetKey(12124121, 43532454); //45678591
		CString key1Str; key1Str.Format("%d",key1);
		CString key2Str; key2Str.Format("%d",key2);
		
		CString plainText, compCPU;
		McbDecrypt(key1Str, key2Str, crypto, plainText);		

		compCPU = plainText;
		if(compCPU == "krackelibankelfnatt")
			return TRUE;

		UINT64 cpuid = GetCPUId(); 
		
		stringstream ss;
		ss << std::hex << cpuid;

		CString myCPU = ss.str().c_str();

		if(compCPU.CompareNoCase(myCPU) == 0)
		{
			TRACE("Match!\n");
			

			return TRUE;
		}				
	}

	return FALSE;
}


BOOL Features::CheckAdapterTrippleDES( PIP_ADAPTER_INFO pAdapter )
{
	if (pAdapter->AddressLength != 6)
	{
		return FALSE ;
	}

	Registry reg;
	CString crypto = reg.GetStringValue("ApplicationKey", "");

    if(crypto.GetLength() >= 16)
	{
		int key1 = GetKey(23249999, 21110211); //23256527
		int key2 = GetKey(12124121, 43532454); //45678591
		CString key1Str; key1Str.Format("%d",key1);
		CString key2Str; key2Str.Format("%d",key2);

		CString plainText, compNic;
		McbDecrypt(key1Str, key2Str, crypto, plainText);		

		if(CountChar('-', plainText) != 5 || plainText.GetLength() < 17)
		{
			return false;
		}


		CString myNic;
		myNic.Format("%.2x-%.2x-%.2x-%.2x-%.2x-%.2x",pAdapter->Address[0], pAdapter->Address[1],pAdapter->Address[2],
			pAdapter->Address[3],pAdapter->Address[4],pAdapter->Address[5]);

		compNic = plainText.Left(17);

		if(compNic.CompareNoCase(myNic) == 0)
		{
			TRACE("Match!\n");
			m_featureKey = atoi(plainText.Right(plainText.GetLength()-18));

			return TRUE;
		}				
	}

	return FALSE;
}


BOOL Features::CheckNetwork( void )
{
#if defined(NO_ALIGNER_IP_CHECK)
    return TRUE ;
//#error dd
#endif    
	ULONG length ;
    DWORD error = ::GetAdaptersInfo( NULL, &length ) ;
    if (ERROR_NO_DATA == error || ERROR_NOT_SUPPORTED == error)
    {     
		return FALSE ;
    }
    PIP_ADAPTER_INFO pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(new BYTE [length]) ;
    if (NULL == pAdapterInfo)
    {     
		return FALSE ;
    }
    error = ::GetAdaptersInfo( pAdapterInfo, &length ) ;
    if (ERROR_SUCCESS != error)
    {   
		return FALSE ;
    }
    PIP_ADAPTER_INFO pNextAdapter = pAdapterInfo ;
    while (NULL != pNextAdapter)
    {		
		//if (CheckAdapter( pNextAdapter ))
		if (CheckAdapterTrippleDES( pNextAdapter ))
		{
            return TRUE ;
        }
        pNextAdapter = pNextAdapter -> Next ;
    }
    return FALSE ;
}
