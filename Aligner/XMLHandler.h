#pragma once


#include "tinyxml.h"

#define XML_VERSION 1

class XmlSerializerException
{
public:
	std::string m_sError;

	XmlSerializerException(char const* msg)
	{
		m_sError=msg;
	}
	XmlSerializerException(char const* msg, char const* file, long line)
	{
		m_sError=msg;
#ifdef DEBUG
		char buff[256];
		m_sError+="\n";
		m_sError+="\n";
		m_sError+="Debug info:\n";
		m_sError+="File: ";
		m_sError+=file;
		m_sError+="\n";
		m_sError+="Line: ";
		m_sError+=ltoa(line,buff, 10);
#endif
	}
};
class CAlignerDoc;

class XMLHandler
{
public:
	XMLHandler(void);
	~XMLHandler(void);

	bool Load(CString file);
	bool Store(CString file);	
	bool ParseConfig(CString text);

    CAlignerDoc* m_pDoc;

protected:

    TiXmlElement* GetChannel(TiXmlElement* node, CString name);
    TiXmlElement* GetProjectParam(TiXmlElement* node, CString name);
    
    bool AddChannel(TiXmlElement* node, Sensor* pSensro);
    bool AddChannel(TiXmlElement* node, Syncro* pSyncro);
    bool LoadChannels(TiXmlElement* node);
    bool StoreChannels(TiXmlElement* node);
    bool StoreMeasurements(TiXmlElement* node);
    bool LoadMeasurements(TiXmlElement* node);
    bool AddAttrib(TiXmlElement* node, CString name, CString str);
    bool AddAttrib(TiXmlElement* node, CString name, int val);
    bool AddAttrib(TiXmlElement* node, CString name, double val);

// 	void AddParamGroup(ParamGroup* pGroup, TiXmlElement* node);
// 	BOOL StoreParamXML(NParameter* param, TiXmlElement* node);
// 
// 	void ReadGroup(TiXmlElement* pNode, ParamGroup* pg);
// 	void ReadParam(TiXmlElement* pNode, NParameter* pParam);

	int m_version;
	CString m_activeFile;
};
