#include "StdAfx.h"
#include "CryptoUtil.h"

void McbDecrypt(CString& key1, CString& key2, CString& cryptogram, CString& plainText)
{    
	McbDES desDecrypt;

	desDecrypt.McbSetKey1((unsigned char*)(LPCTSTR)key1);
	desDecrypt.McbSetKey2((unsigned char*)(LPCTSTR)key2);

	desDecrypt.McbDecrypt((unsigned char*)(LPCTSTR)cryptogram, cryptogram.GetLength());

	plainText = desDecrypt.McbGetPlainText();
	TRACE("Decryption (%d) bytes: %s, \n", desDecrypt.McbGetPlainTextSize(), desDecrypt.McbGetPlainText());
}


int GetKey(int t1, int t2)
{
	return t1|t2;
}
