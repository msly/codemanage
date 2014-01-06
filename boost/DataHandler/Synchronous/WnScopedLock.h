#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// 封装了boost的mutex的scoped_lock，能跨平台使用。相对于CWnLock，其优势在于发生异常时能自动解锁，避免线程死锁。

#include "WnLock.h"

class CWnScopedLock : public CWnLock
{
public:
  CWnScopedLock(CWnLock& mutex);
  virtual ~CWnScopedLock(void);
public:
  virtual void lock() {return m_pMutex->lock();}
  virtual void unlock() {return m_pMutex->unlock();}
private:
  CWnLock* m_pMutex;
};
