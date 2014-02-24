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


遍历文件夹
void RecursiveFindFile(CString& strPath)
{
   CFileFind ff;
   BOOL bFind = ff.FindFile(strPath + "\*.*");

   while(bFind)
   {
      bFind = ff.FindNextFile();
      if (ff.IsDots() || ff.IsSystem() || ff.IsHidden())
         continue;
      
      if (ff.IsDirectory())
      {
         RecursiveFindFile(ff.GetFilePath());
      }
      else
      {
         CString strFileName = ff.GetFileName();
         AfxMessageBox(strFileName);
      }
   }
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


实现文件拖放的一种简洁方法
本文要介绍的这种方法只须调用一个WINDOWS API函数DragQueryFile即可实现文件的拖放操作，而且完全克服了上述3点不足。下面先介绍一下这个函数。 
---- DragQueryFile原型为： 

UINT DragQueryFile (HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINTcch)
---- 其中hDrop是指向拖动文件结构的句柄； 
---- iFile 是以0开始的被托动文件的序号，因为一次可能拖动多个文件。当此参数 
---- 设为0xFFFFFFFF，则函数返回拖动文件的数量； 
---- lpszFile 是指向文件名的缓冲区； 
---- cch 是文件名缓冲区的大小，即文件名的字符数。 
---- 明确了该函数的参数后，编程就是一件很简单的事情了。以下给出一个完整的实例，
程序运行后弹出一个对话框，可以拖动任意数量的文件或目录到上面，
松开鼠标后，程序先显示拖放文件的数量，然后将拖放的文件名全部显示在一个List Box控件中。
具体步骤如下： 

建立一个基于对话框的工程drop，然后其它选项全部用缺省值。 
选中IDD_DROP_DIALOG对话框中Extended Styles的Accept files属性。 
给IDD_DROP_DIALOG对话框添加一个List Box 控件IDC_LIST1。 
在ClassWizard中给CdropDlg类添加成员变量m_list，类型为ClistBox。 
给CdropDlg类中增加处理WINDOWS消息WM_DROPFILES的函数，
接受系统给出的默认名字OnDropFiles，然后输入以下代码：
void CDropDlg::OnDropFiles(HDROP hDropInfo) 
{char *lpszFileName=new char[512],cFileCount[10];
 int nFileCount,i;
 nFileCount=::DragQueryFile 
   (hDropInfo,0xFFFFFFFF,NULL,512);
 ::AfxMessageBox(itoa(nFileCount,cFileCount,10));
 for (i=0;i< nFileCount;i++)
 {
  UINT nChars=::DragQueryFile
          (hDropInfo,i,&lpszFileName[0],512);
  CString str(&lpszFileName[0],nChars);
  m_List.AddString(str);  // 若对于CEdit，用   m_edit1.SetWindowText(str);

 }
 ::DragFinish (hDropInfo);   //释放内存
 i=0;
 delete []lpszFileName;
// CDialog::OnDropFiles(hDropInfo);     此语句注释掉
}

---- 注意程序中的::DragFinish (hDropInfo);语句是必不可少的，它用于释放WINDOWS为处理文件拖放而分配的内存。

WM_DROPFILES消息如果找不到,就先到向导的最后一项class info右下的message filter选windows