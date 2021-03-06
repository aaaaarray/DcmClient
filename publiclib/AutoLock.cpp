// AutoLock.cpp: implementation of the AutoLock class.
//
//////////////////////////////////////////////////////////////////////
#include "AutoLock.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AutoLock::AutoLock(CritSec*pAVCritSec)
{
	if (pAVCritSec)
	{
		m_pAVCritSec = pAVCritSec;
		m_pAVCritSec->Lock();
	}
	
}

AutoLock::~AutoLock()
{
	if (m_pAVCritSec)
		m_pAVCritSec->Unlock();
}
