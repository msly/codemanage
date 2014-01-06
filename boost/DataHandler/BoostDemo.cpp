// BoostDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataHandler/DataHandler.h"
#include <iostream>

// 包含Windows.h，仅为使用Sleep函数
#include <Windows.h>

// Demo程序，示范如何使用CWnQueue, CWnEvent, CWnLock, CWnScopedLock等类的使用方法
// CWnLock, CWnScopedLock的使用方法，可以参考CWnQueue类的实现
// CWnQueue, CWnEvent的使用，可参考CDataHandler及其继承类的使用

class CDemoDataHandler : public CDataHandler<int>
{
public:
	CDemoDataHandler(void){}
	~CDemoDataHandler(){}
protected:
	// 处理数据的函数，可在运行时绑定
	virtual void DataFunc(int& t)
	{
		std::cout << t << std::endl;
	}
};

// 从CDemoDataHandler的使用中可以看出，
// 此数据处理器使用非常简单，一般仅需要实现DataFunc函数，就实现了数据的接收与处理分线程机制
// 即：数据的接收在一个线程中，数据的处理在另外一个线程
// 这对于大量并发数据的情况下（例如证券行业）尤其有效。
// 它可以保证接收的及时性，而不会导致由于数据处理环节的拥塞引起的行情数据丢失
// 此机制已经应用在《上海万得资讯》的Feedhandler数据服务器上
int _tmain(int argc, _TCHAR* argv[])
{
	CDataHandler<int>* pDataHandler = new CDemoDataHandler;

	int val = 0;
	while(true)
	{
		pDataHandler->Put(++val);

		// 等待一秒
		// Sleep为windows下函数，此处仅为示范使用
		// 可以修改为boost的sleep方法，做到跨平台
		Sleep(10);
	}

	return 0;
}