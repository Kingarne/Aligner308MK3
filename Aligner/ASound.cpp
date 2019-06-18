// $Id: ASound.cpp,v 1.2 2009/11/20 16:11:38 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#include "stdafx.h"
#include "ASound.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ASound::ASound()
{
  m_bUseBeep = FALSE;
    

  strcpy(NiceSoundPath,"ding.wav");
  strcpy(ErrorSoundPath,"chord.wav");
}

ASound::~ASound()
{

}

void ASound::PlayNiceSound()
{
  if(m_bUseBeep)
  {
    PlayNiceSoundBeep();      
  } 
  else{
    PlayNiceSoundWav();
  }
}

void ASound::PlayErrorSound()
{
  if(m_bUseBeep)
  {
    PlayErrorSoundBeep();      
  }
  else
  {
    PlayErrorSoundWav();
  }
}

void ASound::PlayNiceSoundBeep()
{
  Beep(800,200);
  Beep(1000,500);
}

void ASound::PlayNiceSoundWav()
{
  PlaySound(NiceSoundPath, NULL, SND_FILENAME);
}

void ASound::PlayErrorSoundBeep()
{
  Beep(400,800);
}

void ASound::PlayErrorSoundWav()
{
  PlaySound(ErrorSoundPath, NULL, SND_FILENAME);
}

void ASound::SetNiceSound(char *Path)
{
  strcpy(NiceSoundPath,Path);
}

void ASound::SetErrorSound(char *Path)
{
  strcpy(ErrorSoundPath,Path);
}
