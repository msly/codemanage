
//dll 需要一定时间初始化，所以开始启动的时候会有点慢属正常

#include <iostream>
#include <Windows.h>
#include <wininet.h> 
#pragma comment(lib, "wininet.lib ") 

int main()
{
	HINSTANCE hInst = LoadLibraryA("Sunday.dll");
	if (!hInst)
	{
		std::cout<<"无法加载 Sunday.dll!";
		getchar();
		return 0;
	}
	std::cout<<"dll 初始化完成！"<<"\n";
    std::cout<<"载入字库！（只需要在程序开头载入一次即可）"<<"\n";
	typedef int (CALLBACK* LPLoadLib)(char[],char[]);
	LPLoadLib LoadLibFromFile = (LPLoadLib)GetProcAddress(hInst, "LoadLibFromFile");
	
	
	
	int index = LoadLibFromFile ("12306.lib","123");
	
	if (index == -1)
	{
		std::cout<<"载入识别库出错。";
		getchar();
		return 0;
	}

	std::cout<<"载入识别库完成。"<<"\n";

	std::cout<<"下载图片中。。！"<<"\n";


/*
	typedef bool (CALLBACK* LPGetCode)(int,char[],char[]);
	LPGetCode GetCodeFromFile = (LPGetCode)GetProcAddress(hInst, "GetCodeFromFile");
*/
	typedef bool (CALLBACK* LPGetCodeFromBuffer)(int,char*,int,char[]);
	LPGetCodeFromBuffer GetCodeFromBuffer = (LPGetCodeFromBuffer)GetProcAddress(hInst, "GetCodeFromBuffer");

	char result[16];

	
	HANDLE pfile = CreateFile("6.png",GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

	if(pfile == INVALID_HANDLE_VALUE){
		std::cout<<"文件打开失败！";
		CloseHandle(pfile);
		return 0;
	}

	unsigned long filesize = GetFileSize (pfile, NULL);
	char* buffer = new char[filesize]; 
	unsigned long readsize;
	ReadFile(pfile,buffer,filesize,&readsize,NULL);
	CloseHandle(pfile);

	long nCount;
	nCount=GetTickCount();

	if (GetCodeFromBuffer(index,buffer,filesize,result))
		std::cout<<"识别完成 ："<<result<<" ,识别用时： "<<GetTickCount()-nCount;
	else
		std::cout<<"识别失败！";
	getchar();
	return 0;
}

