#pragma once

class AutoLock
{
    public:

    AutoLock(CRITICAL_SECTION* pLock){m_pLock = pLock;EnterCriticalSection(m_pLock);}
    ~AutoLock(){;LeaveCriticalSection(m_pLock);}

protected:

    CRITICAL_SECTION* m_pLock;
};
  
