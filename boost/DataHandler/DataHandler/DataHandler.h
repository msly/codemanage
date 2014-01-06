#pragma once

// Author: Huangzhidan | great3779@sina.com 
// copyright: @Wind Information Co., Ltd (Wind Info) ShangHai
// Create time: 2011-09-10

// 一个可用于线程间传递数据的类。此类的优势在于：
// 1. 跨平台
// 2. 将线程通信间比较难的线程安全、信号通知等机制均封装在对象中
// 3. 由于数据的传递完全依靠事件通知，因此数据的流转效率以及吞吐量均非常高（已经使用在公司海外股票FeedHandler上，吞吐量轻松突破每秒500,000个包）
// 4. 接口简单，使用非常方便（可参考BoostDemo程序）

// 使用方法
// CDataHandler是一个基类，使用时定义子类对其进行继承。
// 继承类重写DataThread和DataFunc方法（一般情况下仅需重写DataFunc方法即可）

#include "AtomQueue/WnQueue.h"
#include "Synchronous/WnEvent.h"
#include <boost/thread.hpp>

// a pure virtual function
template <class T> class CDataHandler
{
public:
	CDataHandler(void) {Start();}
	virtual ~CDataHandler() {}

public:
	// 单个通知接口（一般不需调用）
	void NotifyOne() {m_event.notify_one();}

	// 全部通知接口（一般不需调用）
	void NotifyAll() {m_event.notify_all();}

	// 推入流数据
	void Put(T& t)
	{
		m_record_set.push(t);

		// 发送通知信号
		m_event.notify_one();
	}

	// 获取缓冲区buffer size的接口
	int BufferSize() { return m_record_set.size(); }

protected:
	// 处理数据的线程，可在运行时绑定
	virtual void DataThread()
	{
		while(true)
		{
			m_event.wait();
			while(!m_record_set.empty())
			{
				T t;
				if(m_record_set.get(t))
				{
					DataFunc(t);
				}
			}
		}
	}

	// 处理数据的函数，可在运行时绑定
	virtual void DataFunc(T& t) {}

// 以下为内部函数
protected:
	// 开始运行
	void Start()
	{
		boost::thread t(&CDataHandler::DataThread, this);
	}

protected:
	// 流数据集
	CWnQueue<T> m_record_set;

	// 信号
	CWnEvent m_event;	
};
