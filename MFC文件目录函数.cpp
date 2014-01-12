文件对话框
TCHAR szPath[MAX_PATH] = {0};
CString str;

BROWSEINFO bi;
bi.hwndOwner = m_hWnd;
bi.pidlRoot  = NULL;
bi.pszDisplayName = szPath;
bi.lpszTitle = _T("请选择目录");
bi.ulFlags   = 0;
bi.lpfn      = NULL;
bi.lParam    = 0;
bi.iImage    = 0;

LPITEMIDLIST lp = SHBrowseForFolder(&bi);
if (lp && SHGetPathFromIDList(lp, szPath))
{
    str = szPath;
}
 
遍历文件
CFileFind find;
BOOL bContinue = find.FindFile(strPath+_T("\\*.txt"));
while (bContinue)
{
    bContinue = find.FindNextFile();
    if (find.IsDots() || find.IsDirectory())
        continue;
    strFileName = find.GetFileName();
}

获取文件创建时间
unsigned long GetFileNameCreateTime(const char* strFileName)
{
    struct stat stBuff;
    if( stat(strFileName, &stBuff) == 0 )
    {
    return stBuff.st_mtime;//stBuff.st_ctime;
    }

    return 0;
}
 
查看文件是否存在
/* ACCESS.C: This example uses _access to check the
 * file named "ACCESS.C" to see if it exists and if
 * writing is allowed.
 */
 
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
 
void main( void )
{
    /* Check for existence */
    if( (_access( "ACCESS.C", 0 )) != -1 )
    {
        printf( "File ACCESS.C exists\n" );
        /* Check for write permission */
        if( (_access( "ACCESS.C", 2 )) != -1 )
            printf( "File ACCESS.C has write permission\n" );
        if( (_access( "ACCESS.C", 4 )) != -1 )
            printf( "File ACCESS.C has read permission\n" );
        if( (_access( "ACCESS.C", 6 )) != -1 )
            printf( "File ACCESS.C has read and write permission\n" );
    }
    else
    {
        printf("File ACCESS.C does not exists\n");
    }
}
 

获取exe路径

CString strCurPath;
GetModuleFileName( AfxGetInstanceHandle(), strCurPath.GetBuffer(_MAX_PATH), sizeof(char)*_MAX_PATH );
strCurPath.ReleaseBuffer();
int nIndex = strCurPath.ReverseFind('\\');
if(nIndex != -1)
{
    strCurPath = strCurPath.Left(nIndex+1);
}
return strCurPath;


获取文件大小
struct stat file_info;
/* get the file size of the local file */ 
if(stat(LOCAL_FILE, &file_info)) 
{
    printf("Couldnt open '%s': %s\n", LOCAL_FILE, strerror(errno));
    return 1;
}
fsize = (curl_off_t)file_info.st_size;