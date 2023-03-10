#include "StdAfx.h"
#include "XMLHandler.h"
#include "AlignerDoc.h"

XMLHandler::XMLHandler(void)
{
}

XMLHandler::~XMLHandler(void)
{
}

bool XMLHandler::Store(CString file)
{
    TiXmlDocument doc;
    //XmlSerializer xmlSerializer;

    TiXmlElement* profile = new TiXmlElement("File");
    profile->SetAttribute("Version", XML_VERSION);
    doc.LinkEndChild(profile);

    TiXmlElement* project = new TiXmlElement("Project");

    AddAttrib(project, "Name", SysSetup->GetProjectName());
    AddAttrib(project, "Operator", SysSetup->GetOperatorName());
    AddAttrib(project, "Ship", SysSetup->GetShipName());
    AddAttrib(project, "Location", SysSetup->GetPlace());
    AddAttrib(project, "Lat", SysSetup->GetLatitude());
    AddAttrib(project, "ImgIndx", SysSetup->GetImageFileIndex());
    AddAttrib(project, "Unit", SysSetup->GetUnits());    
    StoreMeasurements(project);
    doc.LinkEndChild(project);

    TiXmlElement* channels = new TiXmlElement("Channels");
    StoreChannels(channels);
    doc.LinkEndChild(channels);

    // Save the serialized data
    if(!doc.SaveFile(file))
    {
        CString s("Could not save xml file.\n");
        s+=doc.ErrorDesc(); // Append the error description from TinyXML for added clarity
        AfxMessageBox(s,MB_OK|MB_ICONEXCLAMATION,0);	
        return false;
    }

    m_activeFile = file;

    return true;
}


TiXmlElement* XMLHandler::GetProjectParam(TiXmlElement* node, CString name)
{
    TiXmlElement* pProjItem = node->FirstChildElement("Param");
    while(pProjItem )
    {        
        const char* p = pProjItem->Attribute(name);
        if(p)
            return pProjItem;

        pProjItem  = pProjItem->NextSiblingElement("Param");		
    }

    return NULL;
}


bool XMLHandler::Load(CString file)
{
	try
	{
		TiXmlDocument doc;		
		if(!doc.LoadFile(file))
		{
			CString s("Malformed project file.\n");
			s+=doc.ErrorDesc(); // Append the error description from TinyXML for added clarity
			throw XmlSerializerException(s, __FILE__, __LINE__);
		}
		
		m_activeFile = file;
		
        TiXmlHandle hFile = doc.FirstChildElement("File");
        if( !hFile.Node())
            throw XmlSerializerException("LoadProjectAsXml could not find Project node",__FILE__,__LINE__);

        //	Check that it is the latest version we are trying to load
        int version;
        hFile.Element()->Attribute("Version", &version);
        m_version = version;

        TiXmlElement* hProject = doc.FirstChildElement("Project");
              
		if(!hProject)
		{
			CString s("Malformed project file.\nCould not find Project tag.");
			throw XmlSerializerException(s, __FILE__, __LINE__);
		}

        int iVal;
        double fVal;
        TiXmlElement* pEl;
        CString elName;
        elName = "Name";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            SysSetup->SetProjectName(pEl->Attribute(elName));
        }
        elName = "Operator";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            SysSetup->SetOperatorName(pEl->Attribute(elName));
        }
        elName = "Location";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            SysSetup->SetPlace(pEl->Attribute(elName));
        }
        elName = "Ship";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            SysSetup->SetShipName(pEl->Attribute(elName));
        }
        elName = "Lat";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            pEl->QueryDoubleAttribute(elName, &fVal);
            SysSetup->SetLatitude(fVal);
        }
        elName = "ImgIndx";        
        if((pEl = GetProjectParam(hProject, elName)) != NULL)        
        {
            pEl->QueryIntAttribute(elName, &iVal);
            SysSetup->SetImageFileIndex(iVal);
        }
		elName = "Unit";
		if ((pEl = GetProjectParam(hProject, elName)) != NULL)
		{
			pEl->QueryIntAttribute(elName, &iVal);
			SysSetup->SetUnits(iVal);
		}
        TiXmlElement* pMeasurements = hProject->FirstChildElement("Measurements");
        if(pMeasurements)
        {
            LoadMeasurements(pMeasurements);
        }

        TiXmlElement* pChannels = doc.FirstChildElement("Channels");
        if(pChannels)
        {
            LoadChannels(pChannels);            
        } 

        DAU::GetDAU().UpdateUARTProtocoll();
        DAU::GetDAU().SetResolutionsToDAU();

    }
    catch(XmlSerializerException e)
	{
		return false;
	}
    
    return true;
}

//TiXmlDocument doc;
//XmlSerializer xmlSerializer;


bool XMLHandler::GetConfigXML(CString& xml)
{
	TiXmlDocument doc;
	TiXmlElement* channels = new TiXmlElement("Channels");
	StoreChannels(channels);
	doc.LinkEndChild(channels);
	
	// Declare a printer    
	TiXmlPrinter printer;

	// attach it to the document you want to convert in to a std::string 
	doc.Accept(&printer);

	// Create a std::string and copy your document data in to the string    
	xml = printer.CStr();

	return true;
}

bool XMLHandler::ParseConfig(CString text)
{
	DAU::GetDAU().SetSelected(FALSE);

	TiXmlDocument doc;
	doc.Parse(text);
	if(doc.Error())
	{				
		return false;
	}

	TiXmlElement* pChannels = doc.FirstChildElement("Channels");
	if (pChannels)
	{
		LoadChannels(pChannels);
	}

	DAU::GetDAU().UpdateUARTProtocoll();
	DAU::GetDAU().SetResolutionsToDAU();
	return true;
}


bool XMLHandler::AddAttrib(TiXmlElement* node, CString name, CString str)
{
    TiXmlElement* attrib = new TiXmlElement("Param");
    attrib->SetAttribute(name, str);
    node->LinkEndChild(attrib);
    return true;
}

bool XMLHandler::AddAttrib(TiXmlElement* node, CString name, int val)
{
    TiXmlElement* attrib = new TiXmlElement("Param");
    attrib->SetAttribute(name, val);
    node->LinkEndChild(attrib);
    return true;
}

bool XMLHandler::AddAttrib(TiXmlElement* node, CString name, double val)
{
    TiXmlElement* attrib = new TiXmlElement("Param");
    attrib->SetDoubleAttribute(name, val);
    node->LinkEndChild(attrib);
    return true;
}

bool XMLHandler::LoadMeasurements(TiXmlElement* node)
{
   /* m_pDoc->m_projectMeasurments.clear();
    TiXmlElement* pMeas = node->FirstChildElement("Measurement");

    while(pMeas)
    {
        int iVal=0;
        int res = pMeas->QueryIntAttribute("Item", &iVal);           
        if(res != TIXML_NO_ATTRIBUTE)
        {            
            m_pDoc->m_projectMeasurments.insert(iVal);
        }

        pMeas = pMeas->NextSiblingElement("Measurement");		
    }
	*/
    return true;
}

bool XMLHandler::StoreMeasurements(TiXmlElement* node)
{
 /*   TiXmlElement* measGroup = new TiXmlElement("Measurements");        
    
    for (std::set<LONG>::iterator i = m_pDoc->m_projectMeasurments.begin() ; i != m_pDoc->m_projectMeasurments.end() ; i++)
    {
        TiXmlElement* meas = new TiXmlElement("Measurement");        
        meas->SetAttribute("Item", *i);
        //archive << *i ;
        measGroup->LinkEndChild(meas);
    }
    
    node->LinkEndChild(measGroup);
   */ 
    return true;
}

bool XMLHandler::AddChannel(TiXmlElement* node, Sensor* pSensor)
{
    TiXmlElement* attrib = new TiXmlElement("Sensor");
    
    attrib->SetAttribute("Name", pSensor->GetName());
    attrib->SetAttribute("Offset", pSensor->GetOffset());
    attrib->SetDoubleAttribute("DefScale", pSensor->GetDefaultScale()*10000.0f);    
    attrib->SetAttribute("Serial", pSensor->GetSerialNumber());    
    attrib->SetAttribute("AdapterSerial", pSensor->GetAdapterDesc());    
    attrib->SetDoubleAttribute("NomAz", pSensor->GetNominalAzimuth());  
    attrib->SetAttribute("Type", (int)pSensor->GetType());  
    attrib->SetAttribute("ORDetect", (int)pSensor->GetOverrangeDetection());  
    attrib->SetAttribute("Selected", (int)pSensor->IsSelected());  
    attrib->SetAttribute("Desc", pSensor->m_description);  
   
    node->LinkEndChild(attrib);
    return true;
}

bool XMLHandler::AddChannel(TiXmlElement* node, Syncro* pSyncro)
{    
    Digital* pDig = dynamic_cast<Digital*>(pSyncro);
    int isDigital = (pDig != NULL) ? 1 : 0;        
    DigChTypePro digitalType = (isDigital == 1) ? pDig->m_syncroType : DigChTypeProUnused;
    
    TiXmlElement* attrib = new TiXmlElement("Syncro");
    attrib->SetAttribute("Name", pSyncro->GetName());
    attrib->SetAttribute("Offset", pSyncro->GetOffset());
    attrib->SetAttribute("Type", (int)pSyncro->GetType());
    attrib->SetAttribute("ORDetect", (int)pSyncro->GetOverrangeDetection());
    attrib->SetAttribute("Selected", (int)pSyncro->IsSelected());
    attrib->SetAttribute("NomAz", pSyncro->GetNominalAzimuth());
    attrib->SetAttribute("Gearing", pSyncro->GetGearing());
    attrib->SetAttribute("Desc", pSyncro->m_description);
    attrib->SetAttribute("IsDigital", isDigital);
    attrib->SetAttribute("DigType", (int)digitalType);
    attrib->SetAttribute("Resolution", (int)pSyncro->GetResolution());
    attrib->SetAttribute("HSeaComp", pSyncro->GetHighSeaComp()?1:0);

    node->LinkEndChild(attrib);
    return true;
}


bool XMLHandler::LoadChannels(TiXmlElement* node)
{
    int res, iVal;
    CString str;
    double fVal;
    TiXmlElement* pChItem = node->FirstChildElement("Sensor");
    while(pChItem)
    {        
        const char* p = pChItem->Attribute("Name");
        if(p)
        {
            CString name = p;
            Sensor* pSens = DAU::GetDAU().GetSensor(name);
            if(pSens)
            {                
                if(pChItem->QueryIntAttribute("Offset", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetOffset(iVal);
                }
                if(pChItem->QueryDoubleAttribute("DefScale", &fVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetDefaultScale(fVal/10000.0f);
                }
                if((p = pChItem->Attribute("Serial")) != NULL)
                {
                    str = p;    
                    pSens->SetSerialNumber(str);
                }
                if((p = pChItem->Attribute("AdapterSerial")) != NULL)
                {
                    str = p;    
                    pSens->SetAdapterDesc(str);
                }
                if(pChItem->QueryDoubleAttribute("NomAz", &fVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetNominalAzimuth(fVal);
                }
                if(pChItem->QueryIntAttribute("Type", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetType((UnitType::Types)iVal);
                }
                if(pChItem->QueryIntAttribute("ORDetect", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetOverrangeDetection(iVal);
                }
                if(pChItem->QueryIntAttribute("Selected", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSens->SetIsSelected(iVal);                    
                }


                if((p = pChItem->Attribute("Desc")) != NULL)
                {
                    str = p;    
                    pSens->m_description = str;
                }
            
                pSens->LoadCalibration();
            }     
        }
        pChItem = pChItem->NextSiblingElement("Sensor");		
    }
    
    pChItem = node->FirstChildElement("Syncro");
    while(pChItem)
    {        
        const char* p = pChItem->Attribute("Name");
        if(p)
        {
            CString name = p;            
            Syncro* pSync = DAU::GetDAU().GetSyncro(name);
            if(pSync)
            {
                if(pChItem->QueryIntAttribute("Offset", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetOffset(iVal);
                }
                if(pChItem->QueryIntAttribute("Type", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetType((UnitType::Types)iVal);
                    if(iVal != UnitType::Unused)
                    {
                        DAU::GetDAU().AddBuddyUsedChannel(name);
                    }
                }
                if(pChItem->QueryIntAttribute("ORDetect", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetOverrangeDetection(iVal);
                }               
                if(pChItem->QueryIntAttribute("Selected", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetIsSelected(iVal);                    
                }
                if(pChItem->QueryDoubleAttribute("NomAz", &fVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetNominalAzimuth(fVal);
                }
                if((p = pChItem->Attribute("Desc")) != NULL)
                {
                    str = p;    
                    pSync->m_description = str;
                }
                if(pChItem->QueryDoubleAttribute("Gearing", &fVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetGearing(fVal);
                }
                if(pChItem->QueryIntAttribute("Resolution", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetResolution(iVal);
                }
                if(pChItem->QueryIntAttribute("HSeaComp", &iVal) != TIXML_NO_ATTRIBUTE)
                {
                    pSync->SetHighSeaComp(iVal);
                    if(iVal)
                    {
                        //Store the active gyro, may be only one.                     
                        DAU::GetDAU().m_activeHighSeaGyro = pSync->GetName();
                    }
                }

                Digital* pDig = dynamic_cast<Digital*>(pSync); 
                if(pDig)
                {
                    if(pChItem->QueryIntAttribute("DigType", &iVal) != TIXML_NO_ATTRIBUTE)
                    {
                        pDig->SetSyncroType((DigChTypePro)iVal);
                    }
                }
            }     
        }
        pChItem = pChItem->NextSiblingElement("Syncro");		
    }
  
    return true;
}

bool XMLHandler::StoreChannels(TiXmlElement* node)
{
    map<CString, Sensor*>& sens = DAU::GetDAU().m_sensorMap;
    map<CString, Sensor*>::iterator iter;
    for(iter = sens.begin();iter!= sens.end();iter++)
    {
        TRACE("store:%s\n",iter->first);
        Sensor* pSensor = iter->second;
        AddChannel(node, pSensor);
    }

    map<CString, Syncro*>& sync = DAU::GetDAU().m_syncroMap;
    map<CString, Syncro*>::iterator it;
    int isDigital=0;//, digitalType;
    for(it = sync.begin();it!= sync.end();it++)
    {
        Syncro* pSyncro = it->second;
        AddChannel(node, pSyncro);
     }
    return true;
}

