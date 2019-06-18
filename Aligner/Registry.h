#pragma once

#pragma warning (disable: 4786)
#include <vector>

//Path to Registry key for application settings storage
const char REGKEY_PATH[] = "SOFTWARE\\Schill Reglerteknik AB\\";

//Interface to the registry. Uses 
//HKEY_LOCAL_MACHINE/Software/NOW/ to 
//store and get values.

class Registry 
{
public:
 
  Registry();
  virtual ~Registry();

  bool KeyExists(const CString& strKeyName);

  //Get a value from the registry. If the key doesn´t 
  //exist, it is created and the default value is stored in 
  //the key.
   int GetIntegerValue(const CString & strKeyName, int nDefaultValue);

  //Set a value in the registry. If the key doesn´t 
  //exist, it is created.
  int SetIntegerValue(const CString& strKeyName, int nValue);

  //Get a value from the registry. If the key doesn´t 
  //exist, it is created and the default value is stored in 
  //the key.
  CString GetStringValue(const CString & strKeyName, const CString & strDefaultValue);

  //Set a value in the registry. If the key doesn´t 
  //exist, it is created.
  int SetStringValue(const CString& strKeyName, const CString& strValue);

  //Get a value from the registry. If the key doesn´t 
  //exist, it is created and the default value is stored in 
  //the key.  
  std::vector<CString> GetMultiStringValue(const CString& strKeyName, std::vector<CString> &defaultValue    );
	
  std::vector<BYTE> GetBinaryValue(const CString& strKeyName, std::vector<BYTE>& defaultValue);
  	
  int SetBinaryValue(const CString& strKeyName, std::vector<BYTE>& value);
    
private:
  //Get the complete key name.  
   CString GetKeyPath( CString & strKeyName);
public:  
  int SetMultiStringValue(const CString& strKeyName, std::vector<CString>& value);

  static HKEY m_rootKey;
  static CString m_regPath;    
  static CString m_strAppName;

};

