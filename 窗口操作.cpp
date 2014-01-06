窗口操作
根据标题或者类名查找窗口
HWND hMainHwnd = ::FindWindow(nullptr, _T("网上股票交易系统5.0"));
if (!hMainHwnd)
{
    MessageBox(_T("请打开同花顺 网上股票交易系统5.0"));
    return;
}
 
遍历指定窗口的子窗口
hwnd = ::GetWindow(hMainHwnd, GW_CHILD);
while (hwnd)
{
    HWND hButton = ::GetDlgItem(hwnd, 0x06);
    if (hButton)
    {
        ::PostMessage(hButton, WM_LBUTTONDOWN, 0, 0);
        ::PostMessage(hButton, WM_LBUTTONUP, 0, 0);
        break;
    }
    hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
}
 
遍历所有窗口
EnumWindows的实现过程
BOOL EnumWindows(
  WNDENUMPROC lpEnumFunc,  // callback function
  LPARAM lParam            // application-defined value
)
{
    BOOL bContinue;
    HWND hwnd = GetNextWindow( NULL, GW_HWNDNEXT );
    for( ;hwnd != NULL ; )
    {
        bContinue = lpEnumFunc( hwnd, lParam );
        if( !bContinue ) return TRUE;
		hwnd = GetNextWindow( hwnd, GW_HWNDNEXT ); 
    }
    return TRUE;
}
签名：jmcooler