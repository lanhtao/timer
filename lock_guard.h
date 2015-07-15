
#ifndef __LOCK_GUARD__
#define __LOCK_GUARD__

#include "CriticalLock.h"

class lock_guard
{
public:
	  lock_guard(CriticalLock &l) : m_lock(l) { m_lock.Lock(); }
	  ~lock_guard() { m_lock.Unlock(); }
private:
	  CriticalLock &m_lock;
};

#endif
