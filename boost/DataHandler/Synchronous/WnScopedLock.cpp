#include "WnScopedLock.h"

CWnScopedLock::CWnScopedLock(CWnLock& mutex)
{
  m_pMutex = &mutex;
  m_pMutex->lock();
}

CWnScopedLock::~CWnScopedLock(void)
{
  m_pMutex->unlock();
}