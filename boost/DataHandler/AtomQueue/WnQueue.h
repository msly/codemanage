#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10
// 线程安全的双端队列，可用于多线程中传递数据，数据类型包括二进制流、简单类型、对象等。

#include <deque>
#include "Synchronous/WnLock.h"
#include "Synchronous/WnScopedLock.h"
#include <boost/make_shared.hpp>

// 缓冲区双端队列
// 线程安全类
template<class T> class CWnQueue : public std::deque<T>
{
public:
  CWnQueue()
	{
		m_ptr_mutex = boost::make_shared<CWnLock>();
	}
  ~CWnQueue()
	{
	}

public:
	void push(T& t)
  {
    CWnScopedLock lock(*m_ptr_mutex);
    __super::push_back(t);
  }

	bool get(T& t)
  {
    CWnScopedLock lock(*m_ptr_mutex);
    if(!__super::empty())
    {
      t = __super::front();
      __super::pop_front(); 
      return true;
    }
    return false;
  }

	void clear()
  {
    CWnScopedLock lock(*m_ptr_mutex);
    __super::clear();
  }

protected:
	// 使用智能指针，避免浅拷贝引起的内存提前释放而导致程序崩溃
	boost::shared_ptr<CWnLock> m_ptr_mutex;
};