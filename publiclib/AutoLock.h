// AutoLock.h: interface for the AutoLock class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__

#include "CritSec.h"

class PUBLICLIB_EXPORT AutoLock
{
public:
	AutoLock(CritSec*pAVCritSec);
	~AutoLock();
protected:
    CritSec * m_pAVCritSec;
};

#endif 