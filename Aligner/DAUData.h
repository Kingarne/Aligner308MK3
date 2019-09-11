// $Id: DAUData.h,v 1.8 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002
#include <limits>
#pragma once
#pragma pack(push)
#pragma pack(1)

#define Sigma40_03Ident    0x03
#define Sigma40_01Ident    0x01
#define Sigma40_50Ident    0x50
#define Sigma40_51Ident    0x51

#define FRAME_TYPE_SENSOR 1
#define FRAME_TYPE_HDLC_A 2
#define FRAME_TYPE_HDLC_B 3
#define FRAME_TYPE_UART_A 4
#define FRAME_TYPE_UART_B 5
#define FRAME_TYPE_REG    7

#define FQ_MODE_1220    0x00
#define FQ_MODE_305     0x01
#define FQ_MODE_76      0x02
#define FQ_MODE_19      0x03

//***		BaudRate = 19 660 800 / (UartDiv + 1)	; Values below = UartDiv	***//
//typedef enum UartBaudRate {UBR300 = 65535, UBR600 = 32767, UBR1200 = 16383, UBR2400 = 8191, UBR9600 = 2047, UBR19200 = 1023, UBR38400 = 511, UBR114400 = 171, UBR115200 = 170} ;

class MainBoardData
{
public:
	WORD m_identityData : 8 ;
	WORD m_acqusitionInProgress : 1 ;
	WORD m_resetPerformed : 1 ;
	WORD m_fifoHalfFull : 1 ;
	WORD m_fifoEmpty : 1 ;
	WORD m_sdError : 1 ;
	WORD m_temperatureChannel : 3 ;
} ;

class SensorData
{
public:
	__int16 m_roll ;
	__int16 m_pitch ;
//    SensorData( void ): m_roll(0), m_pitch(0){ }
} ;
struct SensorDataPro																			//	A308Pro
{
	__int16 m_roll ;
	__int16 m_pitch ;
} ;

class TemperatureData
{
public:
  union {
  __int16 m_temperature ;
  __int8 m_old [2] ;
  } ;
public:
  TemperatureData( void ) {
    m_temperature = 0 ;
  }
  TemperatureData( __int16 temperature ) {
    m_temperature = temperature ;
  }
} ;
struct TemperatureDataPro																//	A308Pro
{
  union {
  __int16 m_temperature ;
  __int8 m_old [2] ;
  } ;
} ;

#define DEFAULT_TEMPERATURE_SCALE 1.48E-3
#define DEFAULT_TEMPERATURE_OFFSET 20.0

struct TemperatureCalibrationData
{
private:
  double m_temperatureScale ;
  double m_temperatureOffset ;
public:
  TemperatureCalibrationData( void ) {
    m_temperatureScale = DEFAULT_TEMPERATURE_SCALE ;
    m_temperatureOffset = DEFAULT_TEMPERATURE_OFFSET ;
  }
public:
  double operator()( __int16 data ) const {
    return m_temperatureOffset - m_temperatureScale * data ;
  }
} ;

class SyncroData
{
public:
	__int16 m_roll ;
	__int16 m_pitch ;
	__int16 m_heading ;
public:
  SyncroData( void ) {
    m_roll = 0 ;
    m_pitch = 0 ;
    m_heading = 0;
  }
} ;

struct SyncroDataPro																			//	A308Pro
{
	__int16 m_roll ;
	__int16 m_pitch ;
} ;

/**
*  @brief Encapsulation of DAU low level protocol.
*
*  DAUData encapsulates the serial connection between the DAU mainboard and the host PC.
*
*  @note Do not modify, add or remove members in this structure. They are directly
*        related to the low level application protocol of the Data Accusition Unit (DAU).
*/
class DAUData
{
#if defined(DAU_SIMULATION)
public:
  DAUData( void ) ;
#endif
public:
  BOOL CheckChecksum( void ) const ;
  void ByteSwap( void ) ;
  void InvertSensorData( int protocolVersion ) ;
  void Trace( void ) const ;    
public:
  MainBoardData m_mainBoard ;
	SyncroData m_syncro [2] ;
	SensorData m_sensor [7] ;
  TemperatureData m_temperature ;
	short m_testVoltage ;
	WORD m_checksum ;
} ;

#if defined(DAU_SIMULATION)
inline
DAUData::DAUData( void ) {
  memset( this, 0, sizeof(DAUData) ) ;
  m_sensor [0].m_pitch = 32000 ;
  m_sensor [0].m_roll = 31000 ;
  m_sensor [1].m_pitch = 33000 ;
  m_sensor [1].m_roll = 32000 ;
}
#endif

inline
void DAUData::ByteSwap( void ) {
  BYTE *pData = (BYTE *)this ;
  for (int i = 0 ; i < sizeof(DAUData) ; i += sizeof(WORD)) {
    BYTE temporary = pData [i] ;
    pData [i] = pData [i+1] ;
    pData [i+1] = temporary ;
  }
}

inline
static void InvertInteger( __int16 &word )
{
  if (-32768 == word)   {
    word = 32767 ;
  } else {
    word = -word ;
  }
}

inline void InvertByte( __int8 &byte )
{
  //if (-128 == byte) {																			//	For what it's worth		080925
  //  byte = 128 ;
  if (-128 == byte) {
    byte = 127 ;
  } else {
    byte = -byte ;
  }
}

inline
void DAUData::InvertSensorData( int protocolVersion ) {
  for (int i = 0 ; i < (sizeof(m_sensor)) / sizeof(m_sensor [0]) ; i++) {
    InvertInteger( m_sensor [i].m_pitch ) ;
    InvertInteger( m_sensor [i].m_roll ) ;
  }
  if (0 != protocolVersion)
  {
    InvertInteger( m_temperature.m_temperature ) ;
  }
  else
  {
    InvertByte( m_temperature.m_old [0] ) ;
  }
  InvertInteger( m_testVoltage ) ;
}

inline
void DAUData::Trace( void ) const {
  TRACE("Syncro [0]  : {%d,%d}\n", m_syncro [0].m_pitch, m_syncro [0].m_roll) ;
  TRACE("Sensor [0]  : {%d,%d}\n", m_sensor [0].m_pitch, m_sensor [0].m_roll) ;
  TRACE("Sensor [0]  : {%d,%d}\n", m_sensor [6].m_pitch, m_sensor [6].m_roll) ;
}

//*******************************************************************************//
//***														MainBoardRegsPro				A308Pro								***//
//*** ----------------------------------------------------------------------- ***//
//***	class																																		***//
//*******************************************************************************//
class MainBoardRegsPro															//	A308Pro		Largely extended
{
public:
	struct
	{
	UCHAR					m_dauVersion : 8 ;									//	New
	UCHAR					m_dauSerialNo : 8 ;									//	Dau serial number
	//***   CTRL_REG   ***//	
//	union {
		UCHAR		m_dataAcqEnable : 1 ;
//		UCHAR		m_acqusitionInProgress : 1 ;
//	} ;
	UCHAR			m_reservedForTest : 1 ;
	UCHAR			m_dummy0 : 2 ;
	UCHAR			m_SamplingSpeed : 2 ;							//	00 = 61 Hz, 01 = 1 Hz, 10 = 15.3 Hz, 11 = 31 kHz
//	union {
		UCHAR		m_dauReset : 1 ;
//		UCHAR		m_dauResetPerformed : 1 ;
//	} ;
	UCHAR			m_usbTxOn : 1 ;
	//***   TEST_BYTE   ***//
	UCHAR					m_testByte : 8 ;										//	Earlier : m_identityData
	//***   SerialCtrl_A   ***//
	UCHAR			m_serialCtrlA_HdlcUartSel : 1 ;					//	Select HDLC or UART Rx , HDLC = 1
	UCHAR			m_serialCtrlA_Enable : 1 ;
	UCHAR			m_serialCtrlA_Dummy0 : 2 ;
	UCHAR			m_serialCtrlA_HdlcBitrate : 2 ;					//	00 = 614,4 kb/s, 01 = 307,2 kb/s, 10 = 153,6 kb/s, 11 = 76,8 kb/s
	UCHAR			m_serialCtrlA_Dummy1 : 2 ;
	//***   UartCtrl_A   ***//
	UCHAR			m_uartCtrlA_m_noOfDataBits : 2 ;				//	00 = 5, 01 = 6, 10 = 7, 11 = 8 data bits
	UCHAR			m_uartCtrlA_m_parityEven : 1 ;
	UCHAR			m_uartCtrlA_m_parityOn : 1 ;
	UCHAR			m_uartCtrlA_m_rxTransientFilter : 1 ;
	UCHAR			m_uartCtrlA_m_fifoThreshold : 2 ;				//	00 = 1, 01 = 8, 10 = 16, 11 = 64 bytes ; DAU send threshold
	UCHAR			m_uartCtrlA_ParityError : 1 ;
	//***   UartBaudRate_A   ***//
	WORD			m_uartCtrlA_BaudRate ;												//	See enum UartBaudRate
	//***   SerialCtrl_B   ***//
	UCHAR			m_serialCtrlB_HdlcUartSel : 1 ;					//	Select HDLC or UART Rx , HDLC = 1
	UCHAR			m_serialCtrlB_Enable : 1 ;
	UCHAR			m_serialCtrlB_Dummy0 : 2 ;
	UCHAR			m_serialCtrlB_HdlcBitrate : 2 ;					//	00 = 614,4 kb/s, 01 = 307,2 kb/s, 10 = 153,6 kb/s, 11 = 76,8 kb/s
	UCHAR			m_serialCtrlB_Dummy1 : 2 ;
	//***   UartCtrl_B   ***//
	UCHAR			m_uartCtrlB_m_noOfDataBits : 2 ;				//	00 = 5, 01 = 6, 10 = 7, 11 = 8 data bits
	UCHAR			m_uartCtrlB_m_parityEven : 1 ;
	UCHAR			m_uartCtrlB_m_parityOn : 1 ;
	UCHAR			m_uartCtrlB_m_rxTransientFilter : 1 ;
	UCHAR			m_uartCtrlB_m_fifoThreshold : 2 ;				//	00 = 1, 01 = 8, 10 = 16, 11 = 64 bytes ; DAU send threshold
	UCHAR			m_uartCtrlB_ParityError : 1 ;
	//***   UartBaudRate_B   ***//
	WORD			m_uartCtrlB_BaudRate ;												//	See enum UartBaudRate
	//***   TestVoltage (DACData)   ***//
	short					m_testVoltage ;
	//***   SyncroResolution   ***//
	UCHAR			m_syncroResolutionD2J2Pitch : 2 ;				//	00 = 10, 01 = 12, 10 = 14, 11 = 16 bits
	UCHAR			m_syncroResolutionD2J2Roll : 2 ;
	UCHAR			m_syncroResolutionD1J1Pitch : 2 ;
	UCHAR			m_syncroResolutionD1J1Roll : 2 ;
	//***   Filler (MainBoardData = 16 bytes)   ***//
	UCHAR					m_dummy2 ;															//	Sum = 16 Bytes
	//UCHAR					m_checksum ;														//	Word changed to byte
	}	;	// end struct
} ;

//*******************************************************************************//
//***														SdcAdcMsgDataPro				A308Pro								***//
//*** ----------------------------------------------------------------------- ***//
//***	class																																		***//
//*******************************************************************************//
class SdcAdcMsgDataPro															//	A308Pro		Largely extended
{
public:
	//SdcMsgDataPro( void) ;
	WORD								m_uartParityErrorB : 1 ;
	WORD								m_uartParityErrorA : 1 ;
	WORD								m_serialFifoHalfFullB : 1 ;		//	UART or HDLC
	WORD								m_serialFifoHalfFullA : 1 ;
	WORD								m_usbTxFifoHalfFull : 1 ;
	WORD								m_acqFifoFrameLost : 1 ;
	WORD								m_hdlcFrameLostB : 1 ;
	WORD								m_hdlcFrameLostA : 1 ;

	WORD								m_statusDummy : 4 ;
	WORD								m_statusSdcError : 1 ;
	WORD								m_statusTempChannel : 3 ;

//  	SyncroData		m_syncro [2] ;								//	4 Byte * 2 = 8
//  	SensorData		m_sensor [7] ;								//	4 Byte * 7 = 28
//  	TemperatureData m_temperature ;							//	2 Byte
 	SyncroDataPro				m_syncro [2] ;								//	4 Byte * 2 = 8
 	SensorDataPro				m_sensor [7] ;								//	4 Byte * 7 = 28
 	TemperatureDataPro      	m_temperature ;								//	2 Byte
	short						m_testVoltage ;								//	2 Bytes													Sum = 42 Bytes
    unsigned short						m_testSetVoltage ;							//	2 Bytes													Sum = 42 Bytes    
    UCHAR						m_checksum ;								//	Word changed to byte
} ;
//inline
//SdcMsgDataPro::SdcMsgDataPro( void ) {
////	empty
//}

//class MainBoardDataProSetup														//	A308Pro		New class
//{
//public:
////  MainBoardDataProSetup( void ) ;
//	CString SettingDigChType ;
//	CString SettingDigCh ;
//	void SetDigChParams( CString BitRate , CString DataBits , CString Parity ) ;
//};
//*******************************************************************************//
//***															  DAUDataPro				A308Pro									***//
//*** ----------------------------------------------------------------------- ***//
//***	class																																		***//
//*******************************************************************************//
#define ConstMsgHeader0Pro 0xAA
#define ConstMsgHeader1Pro 0x55
#define ConstMsgTypeProSdcAdc 0x01
#define ConstMsgTypeProHdlcA 0x02
#define ConstMsgTypeProHdlcB 0x03
#define ConstMsgTypeProUartA 0x04
#define ConstMsgTypeProUartB 0x05
#define ConstMsgTypeProRegs 0x07
//typedef enum MsgTypePro {MsgTypeProSdcAdc = 1, MsgTypeProHdlcA = 2, MsgTypeProHdlcB = 3, MsgTypeProUartA = 4, MsgTypeProUartB = 5, MsgProTypeRegs = 7} ;

typedef enum DigDauChPro {DigDauChProA, DigDauChProB, DigDauChProCount} ;
typedef enum DigChTypePro {DigChTypeProUnused, DigChTypeProSeapath, DigChTypeProSigma40_03, DigChTypeProSigma40_NMEA, DigChTypeProSigma40_01, DigChTypeProMins, DigChTypeProIXSEA, DigChTypeProMSI, DigChTypeProPL40, DigChTypeProSigma40_50, DigChTypeProMinsNMEA, DigChTypeProCount} ;
typedef enum DigChSerialTypePro {DigChSerialTypeProHdlc, DigChSerialTypeProUart, DigChSerialTypeCount} ;
typedef enum ParityPro {ParityNone, ParityEven, ParityOdd, ParityCount} ;

struct DigChSetupPro
{
	DigChTypePro				m_DigChTypePro ;
	DigChSerialTypePro	m_DigChSerialTypePro ;
	int									m_Bitrate ;
	int									m_DataBits ;									//	Only valid for Uart
	ParityPro						m_Parity ;										//	Only valid for Uart
};
//struct HdlcUartSetupPro
//{
//	ParityPro		m_parity ;
//	int					m_stopBits ;
//	int					m_bitrate ;
//};

struct DAUFrame
{
    unsigned char type;
    unsigned char length;
    unsigned char seq;
    unsigned char crc;	
	union
    {
        unsigned char HdlcMsg[1000];
        MainBoardRegsPro  m_boardRegs;
        SdcAdcMsgDataPro  sdcAdcMsgData ;
    };
};


class DAUMsgPro
{
public:
	UCHAR								m_msgHeader0	;								//	= 0xAA
	UCHAR								m_msgHeader1	;								//	= 0x55
	UCHAR								m_msgLen ;										//	After Type, before CRC => total msgLen = m_msgLen + 5
	UCHAR								m_msgType	;										//	See enum MsgType
	union {
		UCHAR							m_UartHdlcMsg[1000] ;								//	m_msgLen + 1 ; Last byte = crc ; => 255 + 1 + some extra buffer
		MainBoardRegsPro  m_mainBoardRegsPro ;
		SdcAdcMsgDataPro	m_SdcAdcMsgDataPro ;
	} ;
} ;
class DAUTxMsgPro
{
public:
	UCHAR								m_arrMsgHeader[16] ;					//	= 0xAA
	UCHAR								m_msgHeader0	;								//	= 0xAA
	UCHAR								m_msgHeader1	;								//	= 0x55
	MainBoardRegsPro		m_mainBoardRegsPro ;
} ;

class DAUDataPro
{

public:
    DAUDataPro();

// 	DigChSetupPro			m_arrDauDigChSetupPro[DigDauChProCount] ;										//	Dau DigCh setup
// 	DigChSetupPro			m_arrDauDigChSetupTempPro[DigDauChProCount] ;								//	Dau DigCh setup
// 	DigChSetupPro			m_arrDigChTypeDefaultSetupPro[DigChTypeProCount] ;					//	DigCh type default setup
	
	UCHAR m_RxBuf[1000] ;																												//	Receiver buffer
	DAUMsgPro m_dauMsgPro ;
	DAUTxMsgPro m_dauTxMsgPro ;
	BOOL CheckChecksum( void ) const ;
	DigDauChPro					GetDigDauChPro( CString StrIn ) ;												//	Dig ch A..B							A308Pro
	DigChSerialTypePro	GetDigChSerialTypePro( DigChTypePro DigChTypeProIn ) ;	//	Hdlc or Uart						A308Pro
	DigChTypePro				GetDigChTypePro( CString StrIn ) ;											//	Unused, Seapath..Mins		A308Pro
	ParityPro						GetParityPro( CString StrIn ) ;													//	Parity

    BOOL CheckChecksumPro( void ) const ;																				//	A308Pro
    void ByteSwap( void ) ;
    void InvertSensorData( int protocolVersion ) ;
    void Trace( void ) const ;
    void Init();

protected:

    map<CString, DigChTypePro> m_digChMap;
    map<DigChTypePro, DigChSerialTypePro> m_digInterfaceMap;
    
} ;

//inline
//DAUDataPro::DAUDataPro( void ) {
////	empty
//}

//DAUDataPro m_dauDataPro ;
//DAUDataPro m_dauDataProTemp ;


#pragma pack(pop)
