// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
else if( uMsg == WM_NCACTIVATE ) 
{
	if( !::IsIconic(m_hWnd) ) 
    {                
    	return (wParam == 0) ? TRUE : FALSE;
    }
}
else if( uMsg == WM_NCCALCSIZE ) 
{            
	return 0;
}
else if( uMsg == WM_NCPAINT ) 
{            
	return 0;
}

MFC时间操作
#define _SECOND ((ULONGLONG) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

void CTestClass::ChangeTime(SYSTEMTIME& sysTime)
{ 
    GetLocalTime(&sysTime);

    FILETIME ft={0}; 
    SystemTimeToFileTime(&sysTime, &ft); 

    ULONGLONG qwResult;
    // Copy the time into a quadword.
    qwResult = (((ULONGLONG) ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
    // minus 2 hours.
    qwResult -= 2 * _HOUR;
    // Copy the result back into the FILETIME structure.
    ft.dwLowDateTime  = (DWORD) (qwResult & 0xFFFFFFFF );
    ft.dwHighDateTime = (DWORD) (qwResult >> 32 );

    FileTimeToSystemTime(&ft, &sysTime);
} 
