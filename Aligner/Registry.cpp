
#include "stdafx.h"
#include "Registry.h"


/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

CString Registry::m_regPath = REGKEY_PATH;    
CString Registry::m_strAppName = "";
//HKEY Registry::m_rootKey = HKEY_LOCAL_MACHINE;
HKEY Registry::m_rootKey = HKEY_CURRENT_USER;

Registry::Registry()
{
}


Registry::~Registry() 
{
}


bool Registry::KeyExists(const CString& strKeyName)
{
    const int BUF_SIZE = 1024;
    DWORD nBufLen = BUF_SIZE;
    char aczRegistryString[BUF_SIZE];
    CString strKey = strKeyName;
    
    CString strKeyPath = GetKeyPath(strKey);

    HKEY hKey;
    int nResult;
    if((nResult = ::RegOpenKeyEx(m_rootKey, strKeyPath, 0, KEY_READ, &hKey)) == ERROR_SUCCESS)
    {
        nResult = ::RegQueryValueEx(hKey, strKey, NULL, NULL, (LPBYTE)aczRegistryString, &nBufLen);
    }
    

    if(hKey != 0)
        ::RegCloseKey(hKey);

    return (nResult == ERROR_SUCCESS);
}

//Get a value from the registry. If the key doesn´t 
//exist, it is created and the default value is stored in 
//the key.

CString Registry::GetStringValue(const CString& strKeyName, const CString& strDefaultValue)
{
  HKEY hKey;
  DWORD nDisposition;
  const int BUF_SIZE = 1024;
  DWORD nBufLen = BUF_SIZE;
  char aczRegistryString[BUF_SIZE];
  int nResult;
  CString strRetString;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nResult = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_READ | KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {

    //Error, return the default value
    strRetString = strDefaultValue;
  }

  //Try to read the value
  else if ((nResult = ::RegQueryValueEx(hKey, 
                                        strKey, 
                                        NULL, 
                                        NULL,
                                        (LPBYTE)aczRegistryString, 
                                        &nBufLen)) != ERROR_SUCCESS)
  {

    //Write default value to key
    strcpy(aczRegistryString, strDefaultValue);
    nBufLen = strlen(aczRegistryString) + 1;

    ::RegSetValueEx(hKey,                       // handle to key
                    strKey,                     // value name
                    0,                          // reserved
                    REG_SZ,                     // value type
                    (LPBYTE)aczRegistryString,  // value data
                    nBufLen);                   // size of value data                    

    //Return the default value
    strRetString = strDefaultValue;
  }

  //Everything went OK, return the value from Registry
  else
  {

    //Read value, return it
    strRetString = CString(aczRegistryString);
  }

  //Close the registry
  ::RegCloseKey(hKey);

  return(strRetString);
}


//Set a value in the registry. If the key doesn´t 
//exist, it is created.
int Registry::SetStringValue(const CString& strKeyName, const CString& strValue)
{
  HKEY hKey;
  DWORD nDisposition;
  const int BUF_SIZE = 1024;
  DWORD nBufLen = BUF_SIZE;
  char aczRegistryString[BUF_SIZE];
  int nRetVal;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //Write string to key
  strcpy(aczRegistryString, strValue);
  nBufLen = strlen(aczRegistryString) + 1;

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nRetVal = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {
    //Error
  }
  //Try to write the value
  else if ((nRetVal = ::RegSetValueEx(hKey,       // handle to key
                      strKey,                     // value name
                      0,                          // reserved
                      REG_SZ,                     // value type
                      (LPBYTE)aczRegistryString,  // value data
                      nBufLen)) != ERROR_SUCCESS) // size of value data                    
  {
    //Error
  }
  else
  {

    //Return success
    nRetVal = ERROR_SUCCESS;
  }

  //Close the registry
  ::RegCloseKey(hKey);

  return(nRetVal);
}


//Get a value from the registry. If the key doesn´t 
//exist, it is created and the default value is stored in 
//the key.
int Registry::GetIntegerValue(const CString& strKeyName, int nDefaultValue)
{
  HKEY hKey;
  DWORD nDisposition;
  DWORD nBufLen = 4;
  int nRegistryValue;
  int nResult;
  int nRetVal;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nResult = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_READ | KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {

    //Error, return the default value
    nRetVal = nDefaultValue;
  }

  //Try to read the value
  else if ((nResult = ::RegQueryValueEx(hKey, 
                                        strKey, 
                                        NULL, 
                                        NULL,
                                        (LPBYTE)&nRegistryValue, 
                                        &nBufLen)) != ERROR_SUCCESS)
  {

    //Write default value to key
    nRegistryValue = nDefaultValue;

    ::RegSetValueEx(hKey,                       // handle to key
                    strKey,                     // value name
                    0,                          // reserved
                    REG_DWORD,                  // value type
                    (LPBYTE)&nRegistryValue,    // value data
                    nBufLen);                   // size of value data                    

    //Return the default value
    nRetVal = nDefaultValue;
  }

  //Everything went OK, return the value from Registry
  else
  {

    //Read value, return it
    nRetVal = nRegistryValue;
  }

  //Close the registry
  ::RegCloseKey(hKey);

  return(nRetVal);
}


//Set a value in the registry. If the key doesn´t 
//exist, it is created.
//##ModelId=3EE99891036B
int Registry::SetIntegerValue(const CString& strKeyName, int nValue)
{
  HKEY hKey;
  DWORD nDisposition;
  DWORD nBufLen = 4;
  int nRetVal = -1;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nRetVal = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {
    //Error
  }
  //Try to write the value
  else if ((nRetVal = ::RegSetValueEx(hKey,       // handle to key
                      strKey,                     // value name
                      0,                          // reserved
                      REG_DWORD,                  // value type
                      (LPBYTE)&nValue,            // value data
                      nBufLen)) != ERROR_SUCCESS) // size of value data                    
  {
    //Error
  }
  else
  {

    //Return success
    nRetVal = ERROR_SUCCESS;
  }

  //Close the registry
  ::RegCloseKey(hKey);

  return(nRetVal);
}


//Get the complete key name.

CString Registry::GetKeyPath(CString& strKeyName)
{

  //The key containing the value
  CString strKeyPath;
  
  CString strSubkey = "";
  
  //Extract sub key name from key name
  int nSearchIndex = 0;
  while ((nSearchIndex = strKeyName.Find('\\', nSearchIndex)) != -1)
  {

    //Sub key path found
    strSubkey = strSubkey + strKeyName.Left(nSearchIndex + 1);

    //Remove sub path from key name
    strKeyName = strKeyName.Right(strKeyName.GetLength() - nSearchIndex - 1);
    
    nSearchIndex = 0;
  }

  if (strSubkey != "")
  {
    strKeyPath = m_regPath + m_strAppName + "\\" + strSubkey;
  }
  else
  {
    strKeyPath = m_regPath + m_strAppName + "\\";
  }
  
  return(strKeyPath);
}

std::vector<CString> Registry::GetMultiStringValue(const CString& strKeyName, std::vector<CString> &defaultValue)
{
  HKEY hKey;
  DWORD nDisposition;
  const int BUF_SIZE = 1024;
  DWORD nBufLen = BUF_SIZE;
  unsigned char *pBuffer = NULL;
  int nResult;
  std::vector<CString> tRetStrings;      
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nResult = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_READ | KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {

    //Error, return the default value
    tRetStrings = defaultValue;
  }

  //Try to read the value
  else if ((nResult = ::RegQueryValueEx(hKey, 
                                        strKey, 
                                        NULL, 
                                        NULL,
                                        NULL, 
                                        &nBufLen)) != ERROR_SUCCESS)
  {
    // Entry not in registry, create with default values.
    nResult = SetMultiStringValue(strKeyName, defaultValue);
    //Return the default value
    tRetStrings = defaultValue;
  }  
  else if(nBufLen > 0)
  {
    //Everything went OK, return the value from Registry
    pBuffer = new unsigned char[nBufLen];    

    if (pBuffer != NULL)
    {
      ::memset((unsigned char*)pBuffer, 0, nBufLen * sizeof(unsigned char));    

      if (RegQueryValueEx(hKey, strKey, NULL, NULL, pBuffer, &nBufLen) == ERROR_SUCCESS)
      {
          char *pStr = (char *)pBuffer;
          for (int i = 0; *pStr != '\0'; i++, pStr += strlen((char *)pStr) + 1)
          {
            tRetStrings.push_back(CString(pStr));
          }
      }
      delete[] pBuffer;
    }
  }

  //Close the registry
  ::RegCloseKey(hKey);

  return(tRetStrings);
}



int Registry::SetMultiStringValue(const CString& strKeyName, std::vector<CString>& value)
{
  HKEY hKey;
  DWORD nDisposition;
  int nRetVal;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);
  
  
  // Create buffer with values from value vector
  DWORD size = value.size();
  int nLength=0;
  // Calculate total size of strings.
  int nPos=0;
  for(nPos=0; nPos < size; nPos++)
  {
     nLength += value[nPos].GetLength()+1;    
  }
  nLength += 1; // End \0
 
  unsigned char* pValue = new unsigned char[nLength];
  if(pValue != 0)
  {    
    ::memset(pValue, 0, nLength);

    unsigned char* pDefVal = pValue;
    // Add strings to buffer \0 separated
    for(nPos=0; nPos < size; nPos++)
    {
      ::strcpy((char*)pDefVal, value[nPos]);
      pDefVal += value[nPos].GetLength() + 1;
    }
  }
  else
  {
    // No memory but ok to create empty entry.
    nLength = 0;
  }
  


  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nRetVal = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_READ | KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {

    //Error    
  }
  //Try to write the value
  else if((nRetVal = ::RegSetValueEx(hKey,                       // handle to key
                                     strKey,                     // value name
                                     0,                          // reserved
                                     REG_MULTI_SZ,               // value type                      
                                     (LPBYTE)pValue,     // value data
                                     nLength)) != ERROR_SUCCESS)     // size of value data                    
  {
    // Error
  }

  //Close the registry
  ::RegCloseKey(hKey);
  // Delete buffer memory
  delete[] pValue;
  return(nRetVal);
}

//Set a value in the registry. If the key doesn´t 
//exist, it is created.
int Registry::SetBinaryValue(const CString& strKeyName, std::vector<BYTE>& value)
{
  HKEY hKey;
  DWORD nDisposition;
  
  DWORD size = value.size();
  BYTE* registryVector = new BYTE[size];
  int nRetVal;
  CString strKey = strKeyName;
  
  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //copy vector
  std::vector<BYTE>::iterator iter;
  int i=0;
  for(iter = value.begin() ; iter != value.end() ; iter++, i++)
  {
    registryVector[i] = *iter;
  }

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nRetVal = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {
    //Error
  }
  //Try to write the value
  else if ((nRetVal = ::RegSetValueEx(hKey,       // handle to key
                      strKey,                 // value name
                      0,                          // reserved
                      REG_BINARY,                 // value type
                      (LPBYTE)registryVector,  // value data
                      size)) != ERROR_SUCCESS) // size of value data                    
  {
    //Error
  }
  else
  {

    //Return success
    nRetVal = ERROR_SUCCESS;
  }

  //Close the registry
  ::RegCloseKey(hKey);

  delete []registryVector;

  return(nRetVal);
}


//Get a value from the registry. If the key doesn´t 
//exist, it is created and the default value is stored in 
//the key.
std::vector<BYTE> Registry::GetBinaryValue(const CString& strKeyName, std::vector<BYTE>& defaultValue)
{
  HKEY hKey;
  DWORD nDisposition;
  
  DWORD size = 256;//value.size();
  BYTE* pRegistryVector = new BYTE[size];
  int nResult;
  std::vector<BYTE> retVector;
  CString strKey = strKeyName;

  //Get the name of the key containing the value
  CString strKeyPath = GetKeyPath(strKey);

  //The RegCreateKeyEx function creates the specified registry key.
  //If the key already exists, the function opens it.
  if ((nResult = ::RegCreateKeyEx(m_rootKey,
                                  strKeyPath, 
                                  0, 
                                  "", 
                                  0,
                                  KEY_READ | KEY_WRITE,
                                  NULL, 
                                  &hKey, 
                                  &nDisposition)) != ERROR_SUCCESS)
  {

    //Error, return the default value
    retVector = defaultValue;
  }

  //Try to read the value
  else if ((nResult = ::RegQueryValueEx(hKey, 
                                        strKey, 
                                        NULL, 
                                        NULL,
                                        (LPBYTE)pRegistryVector, 
                                        &size)) != ERROR_SUCCESS)
  {

    SetBinaryValue(strKeyName, defaultValue);
  
    //Return the default value
    retVector = defaultValue;
  }  
  else
  {
    //Everything went OK, return the value from Registry
    //Copy vector, return it
    for(int i=0 ; i<size ; i++)
    {
      retVector.push_back(pRegistryVector[i]); 
    }    
  }

  //Close the registry
  ::RegCloseKey(hKey);

  delete []pRegistryVector;

  return retVector;
}




