一句话删除注册表键及其子键cntrump
网上的代码都是递归查询注册表然后逐个删除。
其实也可以一个 API 就能实现。
 
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
SHDeleteKeyW(HKEY_CURRENT_USER, L"Software\\Google\\Google");
如果想不依赖 shlwapi.dll ，那只能用递归实现了。
使用方法和 SHDeleteKeyW 一样。
LONG WINAPI DeleteKey(HKEY hKey, LPCWSTR pszSubKey)
{
  HKEY hSubKey;
  LONG ret = RegOpenKeyExW(hKey, pszSubKey, 0, KEY_READ|KEY_WRITE, &hSubKey);
 
  if (ERROR_SUCCESS != ret)
    return GetLastError();
 
  WCHAR wzKeyName[MAX_PATH] = {0};
  DWORD dwKeyNameLen;
  FILETIME ft;
  DWORD dwSubKeys;
 
  ret = RegQueryInfoKeyW(hSubKey, NULL, NULL, NULL, &dwSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, &ft);
 
  if (ERROR_SUCCESS != ret)
  {
    RegCloseKey(hSubKey);
    return GetLastError();
  }
 
  // 递归进入每一个子键
  for (DWORD i = 0; i < dwSubKeys; i++)
  {
    dwKeyNameLen = MAX_PATH;
    ret = RegEnumKeyExW(hSubKey, 0, wzKeyName, &dwKeyNameLen, NULL, NULL, NULL, &ft);
 
    do
    {
      // 如果存在子键则递归进入
      if (ERROR_SUCCESS == ret)
      {
        DeleteKey(hSubKey, wzKeyName);
        RegDeleteKeyW(hSubKey, wzKeyName);
      }
 
      dwKeyNameLen = MAX_PATH;
      ret = RegEnumKeyExW(hSubKey, 0, wzKeyName, &dwKeyNameLen, NULL, NULL, NULL, &ft);
    } while (ERROR_SUCCESS == ret);
 
    // 如果不存在子键则删除
    if (ERROR_NO_MORE_ITEMS == ret)
    {
      RegDeleteKeyW(hSubKey, wzKeyName);
    }
  }
 
  RegCloseKey(hSubKey);
  RegDeleteKeyW(hKey, pszSubKey);
 
  return GetLastError();
}