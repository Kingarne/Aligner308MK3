// $Id: Digital.cpp,v 1.17 2014-07-11 09:29:31 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "StdAfx.h"
#include "Digital.h"
#include "lib_crc.h"
#include "ccitt_crc.h"
#include <algorithm>
#include <sstream>

BOOL Digital::m_checkMINSCRC = FALSE;
BOOL Digital::m_checkSigma40CRC = FALSE;

Digital::Digital( const CString &name, BOOL isAligner202 ) : Syncro( name )
{
	m_syncroType = DigChTypeProUnused;
	SetGearing( 1.0 ) ;
	SetRefVoltageA202( 0 ) ;
	m_isAligner202 = isAligner202 ;
	m_CRCError = FALSE;
	m_receivedFrames = 0;
	m_dataReceptionOK = TRUE;
    m_statusCandidate = DS_OK;
    Registry reg;
    m_dir = reg.GetStringValue("Directory", "").TrimRight("\\");        
    m_dumpFileNum = 1;
    m_dumpFile.Format("%s\\MinsData%d.txt",m_dir,m_dumpFileNum);

	/*char t[] = ":F -0049 0236 24253  0000  0002  0043 -0015 114837.00";
	vector<char> v(t, t+sizeof(t));
	double r,p,h;
	ParseMSI(v, r, p, h);
	TRACE("r:%.2f, p:%.2f, h:%.2f", r, p, h);*/
}

Digital::~Digital( void )
{

}

void Digital::SetDataDeg( double roll, double pitch, double heading, int status )
{
    if(status == DS_OK)
    {
        m_rollCandidate =  roll;
        m_pitchCandidate = pitch;
        m_headingCandidate = heading;
    }
}

void Digital::SetData( double roll, double pitch, double heading, int status )
{
    if(status == DS_OK)
    {
         if(theApp.IsAligner308Enabled())
         {
            Rotate( roll, pitch, -(M_PI * GetNominalAzimuth() / 180.0) , roll, pitch ) ;
            m_rollCandidate =  - ( M_PI * roll ) / 180 ;
            m_pitchCandidate = - ( M_PI * pitch ) / 180 ;
			m_headingCandidate = heading;// -(M_PI * heading * 2) / 180;
        }
        else
        {
            Rotate( roll, pitch, (M_PI * GetNominalAzimuth() / 180.0) , roll, pitch ) ;
            m_rollCandidate =  ( M_PI * roll ) / 180 ;
            m_pitchCandidate = ( M_PI * pitch ) / 180 ;
            m_headingCandidate = ( M_PI * heading*2 ) / 180 ;    
        }
    }
    m_statusCandidate = status;
}

BOOL Digital::SetCandidate()
{
    m_roll.Set( m_rollCandidate ) ;
    m_pitch.Set( m_pitchCandidate ) ;
    m_heading.Set( m_headingCandidate ) ;
    m_status = m_statusCandidate;
    return TRUE ;
}

CString Digital::GetDigTypeText( )
{
    CString typeText ;
    switch (m_syncroType)
    {
        case DigChTypeProUnused:        typeText = _T("Unused");        break ;
        case DigChTypeProSeapath:       typeText = _T("Seapath");       break ;
        case DigChTypeProSigma40_03:    typeText = _T("Sigma40-03");    break ;
        case DigChTypeProSigma40_NMEA:  typeText = _T("Sigma40-NMEA");    break ;
        case DigChTypeProSigma40_01:    typeText = _T("Sigma40-01");    break ;
		case DigChTypeProSigma40_50:    typeText = _T("Sigma40-50");    break ;
        case DigChTypeProMins:          typeText = _T("MINS");          break ;        
        case DigChTypeProIXSEA:         typeText = _T("IXSEA");          break ;        
        case DigChTypeProMSI:           typeText = _T("MSI");          break ;      
		case DigChTypeProPL40:          typeText = _T("PL-41");          break ; 
		case DigChTypeProMinsNMEA:      typeText = _T("Sperry-NMEA");    break;
		case DigChTypeProSperryMk39M3:  typeText = _T("Sperry-MK39M3");    break;
		case DigChTypeProSigma40_ICD:  typeText = _T("Sigma40-ICD");    break;
		default:ASSERT(0); break ;
    }
    return typeText ;
}


void Digital::SetSyncroType( DigChTypePro type, bool force ) 
{
    if(m_syncroType == type && !force)
        return;

    m_syncroType = type ;
     
    DigDauChPro interfaceCh = DAU::GetDAU().m_dauDataPro.GetDigDauChPro( GetName() ); 
    DigChSerialTypePro interfaceType = DAU::GetDAU().m_dauDataPro.GetDigChSerialTypePro( m_syncroType ); 
    
    //DAU::GetDAU().m_comThr.SetDigInterface(interfaceCh, interfaceType);
    DAU::GetDAU().m_comThr.SetDigInterfaceSettings(interfaceCh, interfaceType, m_syncroType);

}




static double ToINSAWS( char firstByte, char secondByte, char thirdByte )
{
  int angle = ((firstByte >> 1) & 0x7F) + 0x80 * ((secondByte >> 1) & 0x7F) + 0x4000 * ((thirdByte >> 1) & 0x7F) ;
  double result = (360.0 / 65536.0) * angle ;
  if (180.0 < result)
  {
    result -= 360 ;
  }
  return result ;
}

void Shift( BYTE *pBuffer, UINT &length, UINT shift )
{
  for (UINT i = 0 ; i + shift < length ; i++)
  {
    pBuffer [i] = pBuffer [i+shift] ;
  }
  length -= shift ;
}


static double ToSigma40_180(BYTE firstByte, BYTE secondByte, double scale)
{
	union {
		unsigned __int8  byte[2];
		unsigned __int16 angle;
	} angleUnion;
	angleUnion.byte[0] = (unsigned __int8)secondByte;
	angleUnion.byte[1] = (unsigned __int8)firstByte;
	return scale * 360.0 * ((double)angleUnion.angle) / 65536.0;
}

static double ToSigma40_90( BYTE firstByte, BYTE secondByte, double scale )
{
  union {
    __int8  byte [2] ;
    __int16 angle ;
  } angleUnion ;
  angleUnion.byte [0] = (__int8)secondByte ;
  angleUnion.byte [1] = (__int8)firstByte ;
  return scale * 180.0 * ((double)angleUnion.angle) / 65536.0 ;
}

static double ToSigma40_24bit( BYTE firstByte, BYTE secondByte, BYTE thirdByte, double scale )
{
	union {
		__int8  byte [4] ;
		__int32 angle ;
	} angleUnion ;

	int sign = firstByte & 0x80 ? -1 : 1;
//	firstByte &= ~0x80; // clear sign
	if(sign < 0)
	{
		//Invert 
		firstByte = ~firstByte;
		secondByte = ~secondByte;
		thirdByte = ~thirdByte;
	}


	angleUnion.byte [0] = (__int8)thirdByte;
	angleUnion.byte [1] = (__int8)secondByte ;
	angleUnion.byte [2] = (__int8)firstByte ;
	angleUnion.byte [3] = (__int8)0 ;
	
	angleUnion.angle = (angleUnion.angle+1)*sign;

	return scale * 180.0 * ((double)angleUnion.angle) / 16777216.0 ;
}


double RollFilter[5] ;
double PitchFilter[5] ;
bool FilteringOK(double RollIn, double PitchIn)
{
	int i ;
	int NoOfRollExtremes, NoOfPitchExtremes ;
	double MaxDiff = 0.1 ;

	// Shift new values into Filter
	for (i=3 ; i>=0 ; i--)
	{
		RollFilter[i+1] = RollFilter[i] ;
		PitchFilter[i+1] = PitchFilter[i] ;
	}
	RollFilter[0] = RollIn ;
	PitchFilter[0] = PitchIn ;
	// Test Roll
	NoOfRollExtremes = 0 ;
	NoOfPitchExtremes = 0 ;
	for (i=4 ; i>=1 ; i--)
	{
		if (abs(RollFilter[0] - RollFilter[i]) > MaxDiff)
			NoOfRollExtremes++ ;
		if (abs(PitchFilter[0] - PitchFilter[i]) > MaxDiff)
			NoOfPitchExtremes++ ;
	}
	if ((NoOfRollExtremes >= 2) || (NoOfPitchExtremes >= 2))
		return false ;
	else
		return true ;
}

void TestCRC()
{
	unsigned char test[] = {
	0x03,
	0x00,
	0x60,
	0x87,
	0x9A,
	0x20,
	0x00,
	0xFF,
	0x60,
	0x80,
	0x00,
	0xFF,
	0xE4,
	0x60,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x45,//crc1
	0xA7};//crc2

	int len = sizeof(test);
	unsigned short crc_ccitt_ffff = 0xffff;
	for(int i=0;i<len-2;i++)
		crc_ccitt_ffff = update_crc_ccitt(  crc_ccitt_ffff, test[i]);

	unsigned char crc1 = (crc_ccitt_ffff & 0xff00) >> 8;
	unsigned char crc2 = (crc_ccitt_ffff & 0x00ff);

	if(crc1 == test[len-2] && crc2 == test[len-1])
	{
		TRACE("CRC OK!\n");

	}
	else
	{
		TRACE("CRC FAIL!\n");
	}
}

BOOL Digital::ParseNMEA(vector<char>& frame, double& roll, double& pitch, double& heading, BOOL& checksumOK)
{
    std::vector<char>::iterator pos = find(frame.begin(), frame.end(), '*');
    if(pos == frame.end())
        return FALSE;

    CString checkSumStr;
    checkSumStr += *(pos+1);
    checkSumStr += *(pos+2);

    unsigned int x;   
    std::stringstream ss;
    ss << std::hex << checkSumStr;
    ss >> x;

    std::vector<char>::iterator p1 = pos+1;
    std::vector<char>::iterator p2 = pos+2;
    frame.erase(pos, pos+3);
    //frame.push_back('*');
    //calc checksum
    BYTE checkSum=0;
    for(int i=1;i<frame.size();i++)
    {
        checkSum ^= frame[i];
      //  TRACE("%c",frame[i]);
    }

    checksumOK = m_checkSigma40CRC ? (x == checkSum) : TRUE;
    if(!checksumOK)
    {
        return TRUE;
    }

    vector<CString> field;
    CString str;
    std::vector<char>::iterator start = frame.begin();
    pos = find(frame.begin(), frame.end(), ',');
    while(pos != frame.end())
    {    
        str = "";
        std::vector<char>::iterator it;
        for(it = start;it!=pos;it++)
        {
            str += *it;
        }

        start = pos+1;
        pos = find(start, frame.end(), ',');               
        field.push_back(str);
    }

    str = "";
    std::vector<char>::iterator it;
    for(it = start;it!=frame.end();it++)
    {
        str += *it;
    }
    field.push_back(str);

    //     vector<CString>::iterator fIt;
    //     for(fIt = field.begin();fIt!=field.end();fIt++)
    //     {
    //         TRACE("field:%s\n",*fIt);
    //     }

    if(field.size() != 5)
        return FALSE;

    if(field[1] != "GPatt")
        return FALSE;


    pitch = atof(field[3]);
    roll = atof(field[4]);

    return TRUE;
}
/*
Matz stole this gyro 
BOOL Digital::ParseMINSNMEA(vector<char>& frame, double& roll, double& pitch, double& heading, BOOL& checksumOK)
{
	std::vector<char>::iterator pos = find(frame.begin(), frame.end(), '*');
	if (pos == frame.end())
		return FALSE;

	CString checkSumStr;
	checkSumStr += *(pos + 1);
	checkSumStr += *(pos + 2);

	unsigned int x;
	std::stringstream ss;
	ss << std::hex << checkSumStr;
	ss >> x;

	std::vector<char>::iterator p1 = pos + 1;
	std::vector<char>::iterator p2 = pos + 2;
	frame.erase(pos, pos + 3);
	//frame.push_back('*');
	//calc checksum
	BYTE checkSum = 0;
	for (int i = 1; i < frame.size(); i++)
	{
		checkSum ^= frame[i];
		//  TRACE("%c",frame[i]);
	}

	checksumOK = m_checkMINSCRC ? (x == checkSum) : TRUE;
	if (!checksumOK)
	{
		return TRUE;
	}

	vector<CString> field;
	CString str;
	std::vector<char>::iterator start = frame.begin();
	pos = find(frame.begin(), frame.end(), ',');
	while (pos != frame.end())
	{
		str = "";
		std::vector<char>::iterator it;
		for (it = start; it != pos; it++)
		{
			str += *it;
		}

		start = pos + 1;
		pos = find(start, frame.end(), ',');
		field.push_back(str);
	}

	str = "";
	std::vector<char>::iterator it;
	for (it = start; it != frame.end(); it++)
	{
		str += *it;
	}
	field.push_back(str);

	//     vector<CString>::iterator fIt;
	//     for(fIt = field.begin();fIt!=field.end();fIt++)
	//     {
	//         TRACE("field:%s\n",*fIt);
	//     }

	//if (field.size() != 10)
	//	return FALSE;

	if (field[0] != "$PANZHRP")
		return FALSE;


	heading = atof(field[2]);
	roll = atof(field[3]);
	pitch = atof(field[4]);

	return TRUE;
}
*/

BOOL Digital::ParseMINSNMEA(vector<char>& frame, double& roll, double& pitch, double& heading, BOOL& checksumOK)
{
	std::vector<char>::iterator pos = find(frame.begin(), frame.end(), '*');
	if (pos == frame.end())
		return FALSE;

	CString checkSumStr;
	checkSumStr += *(pos + 1);
	checkSumStr += *(pos + 2);

	unsigned int x;
	std::stringstream ss;
	ss << std::hex << checkSumStr;
	ss >> x;

	std::vector<char>::iterator p1 = pos + 1;
	std::vector<char>::iterator p2 = pos + 2;
	frame.erase(pos, pos + 3);
	//frame.push_back('*');
	//calc checksum
	BYTE checkSum = 0;
	BYTE test = 0;
	for (int i = 0; i < frame.size(); i++)
	{
		test = frame[i];
		checkSum ^= frame[i];
		//  TRACE("%c",frame[i]);
	}

	checksumOK = m_checkMINSCRC ? (x == checkSum) : TRUE;
	if (!checksumOK)
	{
		return TRUE;
	}

	vector<CString> field;
	CString str;
	std::vector<char>::iterator start = frame.begin();
	pos = find(frame.begin(), frame.end(), ',');
	while (pos != frame.end())
	{
		str = "";
		std::vector<char>::iterator it;
		for (it = start; it != pos; it++)
		{
			str += *it;
		}

		start = pos + 1;
		pos = find(start, frame.end(), ',');
		field.push_back(str);
	}

	str = "";
	std::vector<char>::iterator it;
	for (it = start; it != frame.end(); it++)
	{
		str += *it;
	}
	field.push_back(str);

	//     vector<CString>::iterator fIt;
	//     for(fIt = field.begin();fIt!=field.end();fIt++)
	//     {
	//         TRACE("field:%s\n",*fIt);
	//     }

	//if (field.size() != 10)
	//	return FALSE;

	//Matz why is $ cut by HandleMINSNMEAUARTData???
	if (field[0] != "INXDR")
		return FALSE;


	heading = atof(field[2]);
	roll = atof(field[6])*-1;
	pitch = atof(field[10])*-1;

	return TRUE;
}

BOOL Digital::ParseMSI(vector<char>& frame, double& roll, double& pitch, double& heading)
{
	roll = 0;
	pitch = 0;
	heading = 0;
	std::stringstream ss(string(frame.begin(), frame.end()));

	int val=0;
	char c;
	ss >> c; //:
	ss >> c; //status (F)
	ss >> val; //Roll	
	roll = ((double)val)/100;
	val = 0;
	ss >> val; //Pitch	
	pitch = ((double)val)/100;
	val = 0;
	ss >> val; //Heading
	heading = ((double)val)/100;


    return TRUE;
}


//Fix!!!
void Digital::HandleSigma40_NMEA(DAUFrame &frame )
{    
    vector<char> vec;
    vec.insert (vec.begin(), frame.HdlcMsg, frame.HdlcMsg+frame.length);
    
    double roll=0, pitch=0, heading=0;
    BOOL checksumOK=FALSE;    
    //TRACE("r:%f, p:%f\n",roll,pitch);

    if(ParseNMEA(vec, roll, pitch, heading, checksumOK))
    {
        if(checksumOK)
        {
            SetData( roll, pitch, heading ) ;					
        }
        else
        {
            //Checksum incorrect
            SetData( 0, 0, 0, DS_CRC_ERR ) ;
            m_CRCError = TRUE;
        }
    }
}

void Digital::HandleMINSNMEA(DAUFrame &frame)
{
	vector<char> vec;
	vec.insert(vec.begin(), frame.HdlcMsg, frame.HdlcMsg + frame.length);

	double roll = 0, pitch = 0, heading = 0;
	BOOL checksumOK = FALSE;
	//TRACE("r:%f, p:%f\n",roll,pitch);

	if (ParseMINSNMEA(vec, roll, pitch, heading, checksumOK))
	{
		if (checksumOK)
		{
			SetData(roll, pitch, heading);
		}
		else
		{
			//Checksum incorrect
			SetData(0, 0, 0, DS_CRC_ERR);
			m_CRCError = TRUE;
		}
	}
}


void Digital::HandlePL40(DAUFrame &frame )
{    
	if(frame.HdlcMsg[0] == 0xaa && frame.HdlcMsg[1] == 0x55)
	{								
		//Calculate CRC.
// 		BYTE checksumCalc = 0 ;
// 		for (int i = 2 ; i <= 29  ; i++)
// 		{
// 			checksumCalc += frame.HdlcMsg[i] ;		
// 		}

		if (1)//checksumCalc == frame.HdlcMsg[30])
		{
			SetData( ToSigma40_90( frame.HdlcMsg[6], frame.HdlcMsg[7], 1.0 ), ToSigma40_90( frame.HdlcMsg[8], frame.HdlcMsg[9], 1.0 ) );
		}
		else
		{
			//Checksum incorrect
			SetData( 0, 0, 0, DS_CRC_ERR ) ;
			m_CRCError = TRUE;
		}

	}
	else
	{
		TRACE("Wrong interface\n");
	}
}


void CalcCRC(unsigned char*pBuf, unsigned char*pCRC, int len)
{
    BYTE checksum=0;
    for(int i=0;i<len;i++)
    {
        checksum += pBuf[i];
    }
    *pCRC = checksum;
}

void Digital::HandleSigma40_50(DAUFrame &frame )
{
	if(frame.HdlcMsg[3] == Sigma40_50Ident || frame.HdlcMsg[3] == Sigma40_51Ident)
	{
		bool checksumOK = true; 
		BYTE checksumCalc = 0 ;
		
		if(m_checkSigma40CRC)
		{
			for (int i = 0 ; i < 35  ; i++)
			{
				checksumCalc += frame.HdlcMsg[i] ;		
			}
			checksumOK = (checksumCalc == frame.HdlcMsg[35]);
		}
	
		if(checksumOK)
		{
			BYTE status2 = frame.HdlcMsg[6];
			double rollInv = (status2 & 0x08) ? -1.0f : 1.0f; 
			double pitchInv = (status2 & 0x10) ? -1.0f : 1.0f; 
			SetData( ToSigma40_24bit(frame.HdlcMsg[15], frame.HdlcMsg[16], frame.HdlcMsg[17], rollInv ), ToSigma40_24bit( frame.HdlcMsg[18], frame.HdlcMsg[19], frame.HdlcMsg[20], pitchInv )) ;
		}
		else
		{
			//Checksum incorrect.
			SetData( 0, 0, 0, DS_CRC_ERR ) ;
			m_CRCError = TRUE;
		}
	}
	else
	{
		TRACE("Wrong interface\n");
	}
}


void Digital::HandleSigma40_ICD(DAUFrame& frame)
{
	if (frame.HdlcMsg[0] == 1 && frame.HdlcMsg[1] == 0xAA)
	{
		//Calculate CRC.
		BYTE checksumCalc = 0;
		for (int i = 2; i < 18; i++)
		{
			checksumCalc += frame.HdlcMsg[i];
		}

		if (true)//checksumCalc == frame.HdlcMsg[30])
		{
			SetData(ToSigma40_90(frame.HdlcMsg[4], frame.HdlcMsg[5], 1.0), ToSigma40_90(frame.HdlcMsg[6], frame.HdlcMsg[7], 1.0), ToSigma40_180(frame.HdlcMsg[2], frame.HdlcMsg[3], 1.0));
		}
		else
		{
			//Checksum incorrect
			SetData(0, 0, 0, DS_CRC_ERR);
			m_CRCError = TRUE;
		}

	}
	else
	{
		TRACE("Wrong interface\n");
	}

}

void Digital::HandleSigma40_01(DAUFrame &frame )
{    
	if(frame.HdlcMsg[3] == Sigma40_01Ident)
	{								
		//Calculate CRC.
		BYTE checksumCalc = 0 ;
		for (int i = 2 ; i <= 29  ; i++)
		{
			checksumCalc += frame.HdlcMsg[i] ;		
		}

		if (checksumCalc == frame.HdlcMsg[30])
		{
			SetData( ToSigma40_90( frame.HdlcMsg[8], frame.HdlcMsg[9], 1.0 ), ToSigma40_90( frame.HdlcMsg[10], frame.HdlcMsg[11], 1.0 ) );
		}
		else
		{ 
			//Checksum incorrect
			SetData( 0, 0, 0, DS_CRC_ERR ) ;
			m_CRCError = TRUE;
		}

	}
	else
	{
		TRACE("Wrong interface\n");
	}
}


void Digital::HandleSperryMk39M3(DAUFrame &frame)
{
	if (frame.HdlcMsg[0] == 0x5A && frame.HdlcMsg[1] == 0xA5 && frame.HdlcMsg[3] == 0x1)// same as Sigma40_01Ident
	{
		BYTE checksumCalc = 0;
		for (int i = 2; i <= 29; i++)
		{
			checksumCalc += frame.HdlcMsg[i];
		}
	

		if (checksumCalc == frame.HdlcMsg[30])  //This was [31] in v3.0.8
		{
			SetData(ToSigma40_90(frame.HdlcMsg[8], frame.HdlcMsg[9], -1.0), ToSigma40_90(frame.HdlcMsg[10], frame.HdlcMsg[11], -1.0), ToSigma40_180(frame.HdlcMsg[6], frame.HdlcMsg[7], 1.0));
		}
		else
		{
			//Checksum incorrect.
			SetData(0, 0, 0, DS_CRC_ERR);
			m_CRCError = TRUE;
		}
	}
	else
	{
		TRACE("Wrong interface\n");
	}
}

void Digital::HandleSigma40_03(DAUFrame &frame )
{
    if(frame.HdlcMsg[3] == Sigma40_03Ident)
    {
		BYTE checksumCalc = 0 ;
		for (int i = 2 ; i <= 30  ; i++)
        {
			checksumCalc += frame.HdlcMsg[i] ;		
		}
        BYTE c;
       // CalcCRC(&frame.HdlcMsg[2], &c, 29);
        
//         static int test=0;
//         if(test++%1000==0)
//             checksumCalc+=1;

		if (checksumCalc == frame.HdlcMsg[31])
		{     
			SetData( ToSigma40_90(frame.HdlcMsg[11], frame.HdlcMsg[12], 1.0 ), ToSigma40_90( frame.HdlcMsg[13], frame.HdlcMsg[14], 1.0 ), ToSigma40_180(frame.HdlcMsg[9], frame.HdlcMsg[10], 1.0 ) ) ;
		}
		else
		{
			//Checksum incorrect.
            SetData( 0, 0, 0, DS_CRC_ERR ) ;
            m_CRCError = TRUE;
		}
    }
    else
    {
        TRACE("Wrong interface\n");
    }
}

void Digital::HandleIXSEA(DAUFrame &frame )
{
    if(frame.HdlcMsg[0] == 0x02)
    {
        BYTE* data = frame.HdlcMsg;        
        
        BYTE crc = data[1];
        //Calculate CRC
        for(int i=2;i<17;i++)
        {
            crc ^= data[i];
        }

        if(crc == frame.crc[0])
        {
            double heading = (data[1]-48)*100 + (data[2]-48)*10 + (data[3]-48) + (double)(data[4]-48)/10.0f + (double)(data[5]-48)/100.0f;
            double roll = (data[7]-48)*10 + (data[8]-48) + (double)(data[9]-48)/10.0f + (double)(data[10]-48)/100.0f;
            double pitch = (data[12]-48)*10 + (data[13]-48) + (double)(data[14]-48)/10.0f + (double)(data[15]-48)/100.0f;
            
            double rollSign = (data[6] == 0x20) ? 1.0f : -1.0f;
            double pitchSign = (data[11] == 0x20) ? 1.0f : -1.0f;

            roll *= rollSign;
            pitch *= pitchSign;
         
            SetData(roll, pitch);

            TRACE("HandleIXSEA: heading:%.2f, roll:%.2f, pitch:%.2f val:%d, crc:%d\n",heading, roll, pitch, crc, data[17]);
        }
        else
        {
            SetData( 0, 0, 0, DS_CRC_ERR ) ;
            m_CRCError = TRUE;
        }               
    }
    else
    {
        TRACE("Wrong interface\n");
    }
}

void Digital::HandleMSI(DAUFrame &frame )
{    
    vector<char> vec;
    vec.insert (vec.begin(), frame.HdlcMsg, frame.HdlcMsg+frame.length);
    
    double roll=0, pitch=0, heading=0;
    //TRACE("r:%f, p:%f\n",roll,pitch);

    if(ParseMSI(vec, roll, pitch, heading))
    {
        if(1)//checksumOK)
        {
            SetData( roll, pitch, heading ) ;					
        }
        else
        {
            //Checksum incorrect
            SetData( 0, 0, 0, DS_CRC_ERR ) ;
            m_CRCError = TRUE;
        }
    }
}

double MsgAngleToDeg(long Angle)
{
    double	FloatAngle;
    long	LI ;

    LI = Angle ;
    //	Change LSB from bit 13 to bit 0 in the 32b long word
    LI = LI / pow(2.0f,13.0f) ;	// Shift right 13 positions
    //	LSB equals 90 * 2^-18 
    FloatAngle = LI ;
    FloatAngle = FloatAngle * 90 ;
    FloatAngle = FloatAngle / pow(2.0f, 18.0f) ;
    return FloatAngle;
}

long IntelStorageSwapLong(long LongIn) 
{
	long	LOut, L3, L2, L1, L0 ;

	L3 = LongIn & 0xff000000 ;
	L2 = LongIn & 0x00ff0000 ;
	L1 = LongIn & 0x0000ff00 ;
	L0 = LongIn & 0x000000ff ;
	L3 = L3 / 256 / 256 / 256 ;
	L3 = L3 & 0x000000ff ;			// mask sign
	L2 = L2 / 256 ;
	L1 = L1 * 256 ;
	L0 = L0 * 256 * 256 * 256 ;
	LOut = L3 | L2 | L1 | L0 ;
	return LOut ;
}
short IntelStorageSwapShort(short ShortIn) 
{
	short	SOut, S1, S0 ;

	S1 = ShortIn & 0xff00 ;
	S0 = ShortIn & 0x00ff ;
	S1 = S1 / 256 ;
	S1 = S1 & 0x00ff ;				// mask sign
	S0 = S0 * 256 ;
	SOut = S1 | S0 ;
	return SOut ;
}

void Digital::DumpDigFrame(BYTE* pData, int len, int crc)
{
	unsigned char crc1 = (crc & 0xff00) >> 8;
	unsigned char crc2 = (crc & 0x00ff);

	CString dumpFile;

	SYSTEMTIME time;
	GetLocalTime(&time);

	dumpFile.Format("%s\\DigFrameDump%2d_%2d_%2d_%2d_%2d.txt",m_dir,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	ofstream dump(dumpFile, std::ios_base::out | std::ios_base::app);
	CString str;

	for(int i=0;i<len;i++)
	{
		str.Format("%x",pData[i]);	
		dump << str << " ";
	}
	
	str.Format("%x",crc1);
	dump << "crc: " << str << " ";
	str.Format("%x",crc2);
	dump << str;
	dump << std::endl;

	dump.flush();
	dump.close();
}



void Digital::DumpHDLCFrame(HDLCFrameData *frameData)
{            
    static int num=0;
    static int startNum=0;

    ofstream dump(m_dumpFile, std::ios_base::out | std::ios_base::app);
    dump.seekp( 0, std::ios::end );
    long pos = dump.tellp();
    if(pos > 1e8)
    {
        m_dumpFileNum++;
        m_dumpFile.Format("%s\\MinsData%d.txt",m_dir,m_dumpFileNum);
        dump.close();
        dump.open(m_dumpFile, std::ios_base::out | std::ios_base::app);
        dump.seekp( 0, std::ios::end );
        pos = dump.tellp();
    }

    if(pos == 0)
    {
        //Write header
        dump << "Mins Data, rate:" << DAU::GetDAU().m_comThr.GetSamplingRate() << std::endl;     
        startNum=num;
    }
    
    SYSTEMTIME time;
    GetLocalTime(&time);
    CString str;
    str.Format("(%.2d:%.2d:%.2d,%.3d)",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
  
    //TRACE("Size:%d",sizeof(HDLCFrameData));
    HDLCFrameData data = *frameData;
    BYTE* p = (BYTE*)&data;
    data.HeadingAngle = IntelStorageSwapLong(data.HeadingAngle);
    data.RollAngle = IntelStorageSwapLong(data.RollAngle);
    data.PitchAngle = IntelStorageSwapLong(data.PitchAngle);
    data.HeadingAngleRate = IntelStorageSwapShort(data.HeadingAngleRate);
    data.RollAngleRate = IntelStorageSwapShort(data.RollAngleRate);
    data.PitchAngleRate = IntelStorageSwapShort(data.PitchAngleRate);

    long status = 0;
    status = data.DRU_Malfunction | data.Simulation << 1 | data.Spare << 2 | data.EM_Log_Available << 3 | data.GPS_Available << 4
        | data.AttitudeValid << 5 | data.Navigation << 6 | data.Align << 7;
     
    dump << num++-startNum << ";";//std::endl;
    dump << str << ";";
//     dump << (long)(data.DRU_Malfunction!=0) << ";";
//     dump << (long)(data.Simulation!=0) << ";";
//     dump << (long)(data.Spare!=0) << ";";
//     dump << (long)(data.EM_Log_Available!=0) << ";";
//     dump << (long)(data.GPS_Available!=0) << ";";
//     dump << (long)(data.AttitudeValid!=0) << ";";
//     dump << (long)(data.Navigation!=0) << ";";
//     dump << (long)(data.Align!=0) << ";";    
	dump << std::hex << status << ";";
    dump << std::hex << data.HeadingAngle << ";";    				
    dump << std::hex << data.RollAngle << ";";    
    dump << std::hex << data.PitchAngle << ";";    
    dump << std::hex << data.HeadingAngleRate << ";";    			
    dump << std::hex << data.RollAngleRate << ";";    			
    dump << std::hex << data.PitchAngleRate << ";";    			
    
    dump << std::endl;

    dump.flush();
    dump.close();


}

void Digital::HandleMINS(DAUFrame &frame )
{
    HDLCFrameData *frameData = (HDLCFrameData*)frame.HdlcMsg;

	BOOL crcErr = FALSE;
	if(m_checkMINSCRC)
	{
		int crc = crc16(frame.HdlcMsg, 21);
 		unsigned char crc1 = (crc & 0xff00) >> 8;
 		unsigned char crc2 = (crc & 0x00ff);
		
		if(crc1 != frame.HdlcMsg[21] || crc2 != frame.HdlcMsg[22] )
		{
			crcErr = TRUE;
			DumpDigFrame((BYTE*)frameData, 23, crc);
		}
	}
	 
	if(!crcErr)	
	{
		double roll = MsgAngleToDeg(IntelStorageSwapLong(frameData->RollAngle));
		double pitch = MsgAngleToDeg(IntelStorageSwapLong(frameData->PitchAngle));

		//   TRACE("Mins, Roll:%.2f, Pitch:%.2f\n", roll, pitch);
        if(g_AlignerData.debugFlag & DF_DUMP_MINS)
        {
            DumpHDLCFrame(frameData);
        }


		if (FilteringOK(roll, pitch))
		{
			SetData( roll, pitch );
		}	
	}
	else
	{
        SetData( 0, 0, 0, DS_CRC_ERR ) ;	
        m_CRCError = TRUE;	
    }
}

UINT Digital::HandleDataFrame(DAUFrame &frame)
{
	m_receivedFrames++;
	switch (m_syncroType)
    {
        case DigChTypeProSeapath:
            //HandleNMEA() ;
        break ;

		case DigChTypeProSigma40_50:
			HandleSigma40_50(frame) ;
		break ;

        case DigChTypeProSigma40_03:
            HandleSigma40_03(frame) ;
        break ;

				case DigChTypeProSigma40_ICD:
					HandleSigma40_ICD(frame);
					break;

        case DigChTypeProSigma40_NMEA:
            HandleSigma40_NMEA(frame) ;
        break ;

        case DigChTypeProSigma40_01:
            HandleSigma40_01(frame) ;
        break ;

		case DigChTypeProMinsNMEA:
			HandleMINSNMEA(frame);
			break;

        case DigChTypeProMins:
            HandleMINS(frame) ;
            break ;

        case DigChTypeProIXSEA:
            HandleIXSEA(frame) ;
            break ;

		case DigChTypeProMSI:
            HandleMSI(frame) ;
            break ;

		case DigChTypeProPL40:
			HandlePL40(frame) ;
			break ;

		case DigChTypeProSperryMk39M3:
			HandleSperryMk39M3(frame);
			break;


        case 0:
        default:
        m_receivedFrames--;
			//TRACE("No syncro type\n");
        break ;
    } 

    return 0 ;
}

BOOL Digital::CheckIfDataReceived()
{
	int numReceived = m_receivedFrames;
	m_dataReceptionOK = (m_receivedFrames > 0);
	m_receivedFrames = 0;
	
	return m_dataReceptionOK;	
}

