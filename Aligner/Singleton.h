#pragma once

#include "afxmt.h"

class Singleton
{
public:
    Singleton();
    ~Singleton(void);

    int Create(CString name);

    CMutex* m_pMutex;
};
