#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// 封装了boost的condition_variable，使其使用方法很接近Windows的Event。其优势在于能跨平台使用。

#include "WnLock.h"
#include "WnScopedLock.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class CWnEvent
{
public:
	CWnEvent(void){}
  virtual ~CWnEvent(void){}

public:
  void wait()
	{
		CWnScopedLock scoped_lock(m_lock);
		m_condition.wait(scoped_lock);  
	}

	bool timed_wait(int nMillSec)
	{
		CWnScopedLock scoped_lock(m_lock);
		return m_condition.timed_wait(scoped_lock, boost::posix_time::millisec(nMillSec));
	}

	void notify_one() { m_condition.notify_one(); }

	void notify_all() { m_condition.notify_all(); }

private:
  CWnLock m_lock;
  boost::condition_variable_any m_condition;
};