#include "StdAfx.h"
#include ".\singleton.h"

Singleton::Singleton(void):m_pMutex(NULL)
{
}

Singleton::~Singleton(void)
{
   if(m_pMutex != NULL)
       delete m_pMutex;
}


int Singleton::Create(CString name)
{
    m_pMutex = new CMutex(FALSE, name);
    int result = GetLastError();

    if(result != ERROR_SUCCESS)
    {
        ::AfxMessageBox("Aligner308 already open.");
    }
    
    TRACE("Result:%d\n",result);

    return (result == ERROR_SUCCESS);
}