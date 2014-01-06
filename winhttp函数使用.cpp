刷百度空间的访问量cntrump
int _tmain(int argc, _TCHAR* argv[])
{
    for (;;)
    {
        HINTERNET hInternet = InternetOpenW(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        HINTERNET hConnect = InternetConnectW(hInternet, L"hi.baidu.com", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, 
            INTERNET_SERVICE_HTTP, INTERNET_FLAG_ASYNC, 0);
 
        LPCWSTR pwzType[] = {L"image/gif", NULL};
 
        HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", L"cntrump", HTTP_VERSIONW, NULL, pwzType,
                            INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_RELOAD, 0);
 
        BOOL bSend = HttpSendRequestW(hRequest, NULL, 0, NULL, 0);
        CHAR szBuffer[MAX_PATH+1];
        DWORD dwReaded = 0;
 
        do
        {
            InternetReadFile(hRequest, szBuffer, sizeof(szBuffer) - 1, &dwReaded);
        }
        while(dwReaded);
 
        Sleep(2000);
 
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
    }
 
return 0;
}



使用 WinHttp 实现文件下载cntrump
#include "stdafx.h"
 
typedef struct _URL_INFO
{
    WCHAR szScheme[512];
    WCHAR szHostName[512];
    WCHAR szUserName[512];
    WCHAR szPassword[512];
    WCHAR szUrlPath[512];
    WCHAR szExtraInfo[512];
}URL_INFO, *PURL_INFO;
 
 
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd )
{
    URL_INFO url_info = {0};
    URL_COMPONENTSW url = {0};
    url.dwStructSize = sizeof(url);
    url.lpszExtraInfo = url_info.szExtraInfo;
    url.lpszHostName = url_info.szHostName;
    url.lpszPassword = url_info.szPassword;
    url.lpszScheme = url_info.szScheme;
    url.lpszUrlPath = url_info.szUrlPath;
    url.lpszUserName = url_info.szUserName;
    url.dwExtraInfoLength = 
    url.dwHostNameLength = 
    url.dwPasswordLength = 
    url.dwSchemeLength = 
    url.dwUrlPathLength = 
    url.dwUserNameLength = 512;
     
    WinHttpCrackUrl(L"http://www.pediy.com/tools/Debuggers/ollydbg/OllyICE.rar", 0, ICU_ESCAPE, &url);
     
        // 创建一个会话
    HINTERNET hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
    BYTE *pBuffer = NULL;
    DWORD dwReadBytes, dwSizeDW = sizeof(dwSizeDW), dwContentSize, dwIndex = 0;
        // 创建一个连接
    HINTERNET hConnect = WinHttpConnect(hSession, url.lpszHostName, url.nPort, 0);
        // 创建一个请求，先查询内容的大小
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"HEAD", url.lpszUrlPath, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH);
        WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        WinHttpReceiveResponse(hRequest, 0);
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH|WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwContentSize, &dwSizeDW, &dwIndex);
        WinHttpCloseHandle(hRequest);
     
        // 创建一个请求，获取数据
        hRequest = WinHttpOpenRequest(hConnect, L"GET", url.lpszUrlPath, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH);
        WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        WinHttpReceiveResponse(hRequest, 0);
        pBuffer = new BYTE[dwContentSize];
        ZeroMemory(pBuffer, dwContentSize);
    WinHttpReadData(hRequest, pBuffer, dwContentSize, &dwReadBytes);
     
        HANDLE hFile = CreateFileW(L"c:\\ollydbg.rar", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(hFile, pBuffer, dwContentSize, &dwReadBytes, NULL);
        CloseHandle(hFile);
     
        delete pBuffer;
     
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
     
    return 0;
}

简单下载文件
HINTERNET hNet = InternetOpen("FILENAME", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
HINTERNET hUrlFile = InternetOpenUrl(hNet, strUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
char buffer[400 * 1024] = {0};
DWORD dwBytes = 0;
if (InternetReadFile(hUrlFile, buffer, sizeof(buffer), &dwBytes) && dwBytes > 0)
{
    ofstream fout(strFileName, ios::binary);
    if (fout)
    {
        while (dwBytes > 0)
        {
            fout.write(buffer, dwBytes);
            InternetReadFile(hUrlFile, buffer, sizeof(buffer), &dwBytes);
        }
        fout.close();
        InternetCloseHandle(hUrlFile);
        InternetCloseHandle(hNet);
    }
}
