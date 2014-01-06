#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// 封装了boost的mutex，能跨平台使用。

#include <boost/thread.hpp>

class CWnLock
{
public:
	CWnLock(void) {}
	~CWnLock(void) {}
public:
	virtual void lock() {m_lock.lock();}
	virtual void unlock() {m_lock.unlock();}
protected:
  boost::mutex m_lock;
};