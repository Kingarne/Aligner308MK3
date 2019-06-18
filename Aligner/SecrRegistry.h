#pragma once

#pragma warning (disable: 4786)
#include <vector>

//Path to SecretRegistry key for application settings storage
const char SECREGKEY_PATH[] = "SOFTWARE\\ASch\\";

//Interface to the SecretRegistry. Uses 
//HKEY_LOCAL_MACHINE/Software/NOW/ to 
//store and get values.

class SecretRegistry 
{
public:
 
  SecretRegistry();
  virtual ~SecretRegistry();

  bool KeyExists(const CString& strKeyName);

  //Get a value from the SecretRegistry. If the key doesn´t 
  //exist, it is created and the default value is stored in 
  //the key.
   int GetIntegerValue(const CString & strKeyName, int nDefaultValue);

  //Set a value in the SecretRegistry. If the key doesn´t 
  //exist, it is created.
  int SetIntegerValue(const CString& strKeyName, int nValue);

  //Get a value from the SecretRegistry. If the key doesn´t 
  //exist, it is created and the default value is stored in 
  //the key.
  CString GetStringValue(const CString & strKeyName, const CString & strDefaultValue);

  //Set a value in the SecretRegistry. If the key doesn´t 
  //exist, it is created.
  int SetStringValue(const CString& strKeyName, const CString& strValue);

  //Get a value from the SecretRegistry. If the key doesn´t 
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

  static CString m_regPath;    
  static CString m_strAppName;

};

