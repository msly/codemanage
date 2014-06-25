// http://bbs.csdn.net/topics/390583897

//GDI与DX截屏API操作
LPDIRECTDRAW        lpDD       = NULL;
LPDIRECTDRAWSURFACE lpDDSPrime = NULL;
LPDIRECTDRAWSURFACE lpDDSBack  = NULL;
LPDIRECTDRAWSURFACE lpDDSGdi   = NULL;
LPDIRECTDRAWSURFACE lpSurf     = NULL;

DDSURFACEDESC DDSdesc;
BOOL m_b24=TRUE;
//rfbServerInitMsg m_scrinfo;
RECT    m_bmrect;

struct _BMInfo {
    BITMAPINFO bmi       ;
    BOOL       truecolour;
    RGBQUAD    cmap[256] ;
} m_bminfo; // 用来保存位图信息的结构

int DX_Init() {// DirectX初始化。返回当前表面获取一张屏幕位图的存储空间大小
    HRESULT hr;

    // 初始化directX
    hr = DirectDrawCreate(0, &lpDD, 0);
    if (FAILED(hr)) return FALSE;

    hr = lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
    if (FAILED(hr)) return FALSE;

    ZeroMemory(&DDSdesc, sizeof(DDSdesc));
    DDSdesc.dwSize  = sizeof(DDSdesc);
    DDSdesc.dwFlags = DDSD_CAPS;
    DDSdesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hr = lpDD->CreateSurface(&DDSdesc, &lpDDSPrime, 0);
    if (FAILED(hr)) return FALSE;

    hr = lpDD->GetGDISurface(&lpDDSGdi);
    if (FAILED(hr)) return FALSE;

    ZeroMemory(&DDSdesc, sizeof(DDSdesc));
    DDSdesc.dwSize  = sizeof(DDSdesc);
    DDSdesc.dwFlags = DDSD_ALL;
    hr = lpDDSPrime->GetSurfaceDesc(&DDSdesc);
    if (FAILED(hr)) return FALSE;

    // 初始化位图信息
    if ((DDSdesc.dwFlags & DDSD_WIDTH) && (DDSdesc.dwFlags & DDSD_HEIGHT)) {
        m_bmrect.left = m_bmrect.top = 0;
        m_bmrect.right = DDSdesc.dwWidth;
        m_bmrect.bottom = DDSdesc.dwHeight;
    } else return FALSE;

    m_bminfo.bmi.bmiHeader.biCompression = BI_RGB;//BI_BITFIELDS;
    m_bminfo.bmi.bmiHeader.biBitCount = DDSdesc.ddpfPixelFormat.dwRGBBitCount;

    // m_bminfo.truecolour = DDSdesc.ddpfPixelFormat.dwFlags & DDPF_RGB;
    if (m_bminfo.bmi.bmiHeader.biBitCount > 8)
        m_bminfo.truecolour = TRUE;
    else
        m_bminfo.truecolour = FALSE;

    ZeroMemory(&DDSdesc, sizeof(DDSdesc));
    DDSdesc.dwSize = sizeof(DDSdesc);
    DDSdesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    DDSdesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    DDSdesc.dwHeight = m_bmrect.bottom - m_bmrect.top;
    DDSdesc.dwWidth  = m_bmrect.right  - m_bmrect.left;
    hr = lpDD->CreateSurface(&DDSdesc, &lpDDSBack, 0);
    if (FAILED(hr)) return FALSE;
//  hr = lpDDSPrime->QueryInterface( IID_IDirectDrawSurface3, (LPVOID *)&lpSurf);
//  if (FAILED(hr)) return FALSE;

    switch (m_bminfo.bmi.bmiHeader.biBitCount) {
    case 32:
    case 24:
       // Update the bitmapinfo header
       m_b24 = TRUE;
       m_bminfo.bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
       m_bminfo.bmi.bmiHeader.biWidth = 1024;
       m_bminfo.bmi.bmiHeader.biHeight = 768;
       m_bminfo.bmi.bmiHeader.biPlanes = 1;
//     m_bminfo.bmi.bmiHeader.biBitCount = 24;
       m_bminfo.bmi.bmiHeader.biCompression = BI_RGB;
       m_bminfo.bmi.bmiHeader.biSizeImage = abs((m_bminfo.bmi.bmiHeader.biWidth * m_bminfo.bmi.bmiHeader.biHeight * m_bminfo.bmi.bmiHeader.biBitCount)/8);
       m_bminfo.bmi.bmiHeader.biXPelsPerMeter = (1024*1000)/1024;
       m_bminfo.bmi.bmiHeader.biYPelsPerMeter = (768*1000)/768;
       m_bminfo.bmi.bmiHeader.biClrUsed   = 0;
       m_bminfo.bmi.bmiHeader.biClrImportant = 0;
       break;
    }

    return m_bminfo.bmi.bmiHeader.biSizeImage;
}

BOOL CaptureScreen(RECT &rect, BYTE *scrBuff, UINT scrBuffSize) {// 捕捉屏幕。rect: 区域。scrBuff: 输出缓冲。scrBuffSize: 缓冲区大小
    HRESULT hr=0;

    hr = lpDDSBack->BltFast(rect.left,rect.top,lpDDSPrime,&rect,DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
    if (FAILED(hr)) return FALSE;

    DDSURFACEDESC surfdesc;
    ZeroMemory(&surfdesc, sizeof(surfdesc));
    surfdesc.dwSize = sizeof(surfdesc);

    hr = lpDDSBack->Lock(&rect, &surfdesc, DDLOCK_READONLY | DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR /*|DDLOCK_NOSYSLOCK*/, NULL);
//  hr = lpDDSPrime->Lock(&rect, &surfdesc, DDLOCK_READONLY | DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR /*|DDLOCK_NOSYSLOCK*/, NULL);
    if (FAILED(hr)) return FALSE;

    // copy the data into our buffer
    BYTE * destbuffpos, * srcbuffpos;
//  m_scrinfo.format.bitsPerPixel = 24;
    srcbuffpos = (BYTE *) surfdesc.lpSurface;
    destbuffpos = scrBuff;

    memcpy( destbuffpos, srcbuffpos,m_bminfo.bmi.bmiHeader.biSizeImage);

    // unlock the primary surface
//  lpDDSPrime->Unlock(surfdesc.lpSurface);
    lpDDSBack->Unlock(surfdesc.lpSurface);
    return TRUE;
}

int SaveBitmapToFile(BITMAP *bitmap, LPSTR lpFileName,char *lpBuf) {
   DWORD dwWritten;
   BITMAPFILEHEADER   bmfHdr;
   BITMAPINFOHEADER   bi;
   HANDLE          fh=NULL;
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth= bitmap->bmWidth;
   bi.biHeight = bitmap->bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount      = bitmap->bmBitsPixel*8;
   bi.biCompression   = BI_RGB;
   bi.biSizeImage     = 0;
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed       = 0;
   bi.biClrImportant  = 0;
   fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
   if (fh == INVALID_HANDLE_VALUE) return FALSE;
   bmfHdr.bfType = 0x4D42; // "BM"
   bmfHdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+bitmap->bmWidth*bitmap->bmHeight*bitmap->bmBitsPixel;
   bmfHdr.bfReserved1 = 0;
   bmfHdr.bfReserved2 = 0;
   bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
   WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
   WriteFile(fh, (char *)&bi,sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
   WriteFile(fh, (char *)lpBuf,bitmap->bmWidth*bitmap->bmHeight*bitmap->bmBitsPixel, &dwWritten, NULL);
   FlushFileBuffers(fh);
   CloseHandle(fh);
   return true;
}

//(1)获取屏幕绘图设备
//(2)创建一个与屏幕绘图设备相兼容的内存绘图设备
//(2)在内存中创建一个与屏幕绘图设备相兼容的图像对象
//(3)将屏幕设备中的图像复制到内存绘图设备中
//(4)将内存图像保存到文件中
//相关函数：
//GetDIBits:按位的方式返回指定的BITMAP，并按指定的格式存储到内存中
int GetBitmapFromScreen(char *lpFileName) {
    char *lpBuf;
    HBITMAP hBitmap,hOld ;
    HDC hDC,hcDC;
    BITMAP bb;
    BITMAPINFO b;
    HANDLE hp,fh=NULL;
    DWORD dwX,dwY;

    dwX=GetSystemMetrics(SM_CXSCREEN);
    dwY=GetSystemMetrics(SM_CYSCREEN);
    hDC=GetDC(NULL);
    hcDC=CreateCompatibleDC(hDC);
    hBitmap=CreateCompatibleBitmap(hDC,dwX,dwY);
    hOld=(HBITMAP)SelectObject(hcDC,hBitmap);
    BitBlt(hcDC,0, 0,dwX,dwY, hDC, 0, 0, SRCCOPY);
    bb.bmWidth=dwX;
    bb.bmHeight =dwY;
    bb.bmPlanes = 1;
    bb.bmWidthBytes=bb.bmWidth*3;
    bb.bmBitsPixel=3;
    bb.bmType=0;
    b.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    b.bmiHeader.biWidth        =dwX;
    b.bmiHeader.biHeight       =dwY;
    b.bmiHeader.biPlanes       =1;
    b.bmiHeader.biBitCount     =3*8;
    b.bmiHeader.biCompression  =BI_RGB;
    b.bmiHeader.biSizeImage    =0;
    b.bmiHeader.biXPelsPerMeter=0;
    b.bmiHeader.biYPelsPerMeter=0;
    b.bmiHeader.biClrUsed      =0;
    b.bmiHeader.biClrImportant =0;
    b.bmiColors[0].rgbBlue     =8;
    b.bmiColors[0].rgbGreen    =8;
    b.bmiColors[0].rgbRed      =8;
    b.bmiColors[0].rgbReserved =0;
    hp=GetProcessHeap();
    lpBuf=(char *)HeapAlloc(hp,HEAP_ZERO_MEMORY,bb.bmHeight*bb.bmWidth*4);
    GetDIBits(hcDC,hBitmap,0,dwY,lpBuf,&b,DIB_RGB_COLORS);
    SaveBitmapToFile(&bb,lpFileName,lpBuf);
    ReleaseDC(NULL,hDC);
    DeleteDC(hcDC);
    DeleteObject(hBitmap);
    DeleteObject(hOld);
    HeapFree(hp,0,lpBuf);
    return true;
}

