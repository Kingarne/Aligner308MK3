// $Id: ASound.h,v 1.1 2009/10/14 08:08:14 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ASound  
{
public:
	ASound();
	virtual ~ASound();

  void PlayNiceSound();
  void PlayErrorSound();
  void UseBeep(BOOL b=TRUE){m_bUseBeep = b;}
  void SetNiceSound(char *Path);
  void SetErrorSound(char *Path);

private:

  BOOL m_bUseBeep;
  char NiceSoundPath[MAX_PATH];
  char ErrorSoundPath[MAX_PATH];

  void PlayNiceSoundBeep();
  void PlayNiceSoundWav();
  void PlayErrorSoundBeep();
  void PlayErrorSoundWav();
  

};
