// $Id: DAUData.cpp,v 1.5 2014-10-16 08:46:56 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"

#include "DAUData.h"

#pragma optimize( "", off )

BOOL DAUData::CheckChecksum( void ) const
{
    unsigned __int16 crc = 0xFFFFU ;
    const unsigned __int16 *pData = reinterpret_cast<const unsigned __int16 *>(&(this -> m_mainBoard)) ;
    unsigned __int16 items = reinterpret_cast<const unsigned __int16 *>(&(this -> m_checksum)) - pData  ;
    for (int i = 0 ; i < items ; i++)
    {
        unsigned __int16 data = pData [i] ;
        for (int i = 0 ; i < 8 * sizeof(unsigned __int16) ; i++)
        {
            bool xor = (0 != (crc & (1 << (8 * sizeof(unsigned __int16) - 1)))) ;
            crc <<= 1 ;
            if (0 != (data & 1))
            {
                crc |= 1 ;
            }
            data >>= 1 ;
            if (xor)
            {
                crc ^= 0x1009 ;
            }
        }
    }
    return crc == m_checksum ;
}

DAUDataPro::DAUDataPro()
{
    
}

void DAUDataPro::Init()
{
    CString str ;
    str.LoadString(IDS_UNUSED);
    m_digChMap[str] = DigChTypeProUnused;

    str.LoadString(IDS_SEAPATH) ;
    m_digChMap[str] = DigChTypeProSeapath;

    str.LoadString(IDS_SIGMA_40_03) ;
    m_digChMap[str] = DigChTypeProSigma40_03;

	str.LoadString(IDS_SIGMA_40_50) ;
	m_digChMap[str] = DigChTypeProSigma40_50;

    str.LoadString(IDS_SIGMA_40_OSLO) ;                   
    m_digChMap[str] = DigChTypeProSigma40_NMEA;

    str.LoadString(IDS_SIGMA_40_01) ;
    m_digChMap[str] = DigChTypeProSigma40_01;

    str.LoadString(IDS_MINS) ;
    m_digChMap[str] = DigChTypeProMins;    
    
    str.LoadString(IDS_IXSEA) ;
    m_digChMap[str] = DigChTypeProIXSEA;    

	str.LoadString(IDS_MSI) ;
    m_digChMap[str] = DigChTypeProMSI;    

	str.LoadString(IDS_PL40) ;
	m_digChMap[str] = DigChTypeProPL40;
	
	str.LoadString(IDS_MINS_NMEA);
	m_digChMap[str] = DigChTypeProMinsNMEA;
	
	str.LoadString(IDS_SPERRY_MK39M3);
	m_digChMap[str] = DigChTypeProSperryMk39M3;
		
  str.LoadString(IDS_SIGMA40_ICD);
  m_digChMap[str] = DigChTypeProSigma40_ICD;

  str.LoadString(IDS_SPERRY_NMEA);
  m_digChMap[str] = DigChTypeProSperryNMEA;

  m_digInterfaceMap[DigChTypeProUnused] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSeapath] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSigma40_03] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSigma40_NMEA] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSigma40_01] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSigma40_50] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProMins] = DigChSerialTypeProHdlc;
  m_digInterfaceMap[DigChTypeProIXSEA] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProPL40] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProMinsNMEA] = DigChSerialTypeProUart;
	m_digInterfaceMap[DigChTypeProSperryMk39M3] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSigma40_ICD] = DigChSerialTypeProUart;
  m_digInterfaceMap[DigChTypeProSperryNMEA] = DigChSerialTypeProUart;
}


//*******************************************************************************//
//***																GetDigDauChPro				A308Pro							***//
//*** ----------------------------------------------------------------------- ***//
//***	Get DigDauChPro from string "D1", "D2"																	***//
//*******************************************************************************//
DigDauChPro	DAUDataPro::GetDigDauChPro( CString StrIn ) 
{	
	if (StrIn.Compare(D1Name()) == 0)
		return DigDauChProB;
	else
		return DigDauChProA;
}
//*******************************************************************************//
//***													GetDigChSerialTypePro				A308Pro							***//
//*** ----------------------------------------------------------------------- ***//
//***	Get DigChSerialTypePro from DigChTypePro															***//
//*******************************************************************************//
DigChSerialTypePro DAUDataPro::GetDigChSerialTypePro( DigChTypePro DigChTypeProIn ) 
{
    map<DigChTypePro, DigChSerialTypePro>::iterator iter;
    if((iter = m_digInterfaceMap.find(DigChTypeProIn)) != m_digInterfaceMap.end())
    {
        return iter->second;
    }
    else
        return DigChSerialTypeProUart;     
}
//*******************************************************************************//
//***															  GetDigChTypePro				A308Pro							***//
//*** ----------------------------------------------------------------------- ***//
//***	Get DigChTypePro from string																						***//
//*******************************************************************************//
DigChTypePro DAUDataPro::GetDigChTypePro( CString StrIn ) 
{
    map<CString, DigChTypePro>::iterator iter;
    if((iter = m_digChMap.find(StrIn)) != m_digChMap.end())
    {
        return iter->second;
    }
    else
        return DigChTypeProUnused;
       
}
//*******************************************************************************//
//***															GetParityPro				A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***	Get ParityPro from string																								***//
//*******************************************************************************//
ParityPro	DAUDataPro::GetParityPro( CString StrIn )
{
	CString Str1 ;
	Str1 = "None" ;
	if (StrIn.Compare(Str1) == 0) {
		return ParityNone ;
	}
	Str1 = "Even" ;
	if (StrIn.Compare(Str1) == 0) {
		return ParityEven ;
	}
	Str1 = "Odd" ;
	if (StrIn.Compare(Str1) == 0) {
		return ParityOdd ;
	}
    return ParityNone;
}
//*******************************************************************************//
//***															  DAUDataPro				A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***	Create area																															***//
//*******************************************************************************//
//int DAUDataPro::i1 = 0 ;
//DAUDataPro DAUDataPro::m_dauDataPro ;
//DAUDataPro m_dauDataPro ;
//DAUDataPro m_dauDataProTemp ;
//*******************************************************************************//
//***															CheckChecksumPro		A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***	Returns True on correct checksum																				***//
//*******************************************************************************//
BOOL DAUDataPro::CheckChecksumPro( void ) const
{
UCHAR crc_array[256] = { 
0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 
0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 
0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 
0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 
0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 
0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 
0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 
0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 
0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 
0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 
0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 
0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 
0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 
0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 
0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 
0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 
0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35, 
}; 

		UCHAR crc = 0xFF ;
//    UCHAR *pData = &m_msgLen ;										//	Compiler error : const UHAR <> UCHAR
//    unsigned __int16 items = m_dauMsgPro.m_msgLen + 2 ;	//	m_msgLen = payload but crc includes m_msgLen and m_msgType
//		UCHAR *pData = DAU::GetDAU().m_dauDataPro.m_RxBuf ;		 
		UCHAR *pData ;
		pData = &DAU::GetDAU().m_dauDataPro.m_dauMsgPro.m_msgHeader0 ;		 
		pData += 2 ;																					//	Start crc at m_msgLen 
		UCHAR items = *pData + 2 ;														//	Payload + m_msgLen and m_msgType = m_msgLen + 2
    for (int i = 0 ; i < items ; i++)
    {
      UCHAR data = *(pData + i) ;
			UCHAR ix = data ^ crc ;
			crc = crc_array[ix] ; 
    }
		UCHAR checksum = *(pData + items) ;
//		return crc == m_checksum ;
		return crc == checksum ;



//unsigned char crc_table(int data)
//{
//int i = (data ^ crc) & 0xff; 
//crc = crc_array[(data ^ crc)&0xff]; 
//return crc;
//} 



};
