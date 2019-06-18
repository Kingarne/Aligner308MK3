#pragma once
#include "McbDES2.hpp"

void McbDecrypt(CString& key1, CString& key2, CString& cryptogram, CString& plainText);
int GetKey(int t1, int t2);