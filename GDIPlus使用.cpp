#include <windows.h>
#include "WinApplication.h"
#include <cassert>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

using namespace std;

typedef union
{
    ARGB Color;
    struct 
    {
        BYTE Blue;
        BYTE Green;
        BYTE Red;
        BYTE Alpha;
    };
}ARGBQuad, *PARGBQuad;

// 图像数据灰度化 二值化
void GrayAnd2Value(BitmapData *data, BYTE threshold)
{
    PARGBQuad p = (PARGBQuad)data->Scan0;
    int offset = data->Stride - data->Width * sizeof(ARGBQuad);

    for (UINT y=0; y<data->Height; ++y, p += offset)
    {
        for (UINT x=0; x<data->Width; ++x,++p)
        {
            if ( ((p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8) < threshold )
                p->Color &= 0xff000000;
            else
                p->Color |= 0x00ffffff;
        }
    }
}

void ShowImage(HDC hdc)
{
    Graphics graphics(hdc);
    Gdiplus::Bitmap bitmap(L"c:\\code.jpg");
    graphics.DrawImage(&bitmap, 10, 10);

    ColorMatrix clrMatrix = {
        0.3f,  0.3f,  0.3f,  0.0f, 0.0f,
        0.59f, 0.59f, 0.59f, 0.0f, 0.0f,
        0.11f, 0.11f, 0.11f, 0.0f, 0.0f,
        0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.0f,  0.0f,  0.0f,  0.0f, 1.0f
    };
    ImageAttributes ImgAttr;
    ImgAttr.SetColorMatrix(&clrMatrix);
    ImgAttr.SetThreshold(0.5);
    Graphics *g = Graphics::FromImage(&bitmap);
    g->DrawImage(&bitmap, GpRect(0, 0, bitmap.GetWidth(), bitmap.GetHeight()), 
        0, 0, bitmap.GetWidth(), bitmap.GetHeight(), UnitPixel, &ImgAttr);
    graphics.DrawImage(&bitmap, 10 + bitmap.GetWidth() + 2, 10);
}

void OnPaint(HDC hdc)
{
    Graphics graphics(hdc);

    Bitmap b(L"code.jpg");
    Bitmap* b2;

    INT iWidth = b.GetWidth();
    INT iHeight = b.GetHeight();

    Rect rect(0,0,iWidth,iHeight);
    b2 = b.Clone(rect,PixelFormat24bppRGB); 


    BitmapData bmData;
    BitmapData bmData2;

    b.LockBits(&rect,ImageLockModeRead | ImageLockModeWrite,
        PixelFormat24bppRGB,&bmData);
    b2->LockBits(&rect,ImageLockModeRead |ImageLockModeWrite,
        PixelFormat24bppRGB,&bmData2);

    int stride = bmData.Stride;

    unsigned char * p = (unsigned char *)bmData.Scan0;
    unsigned char * p2 = (unsigned char *)bmData2.Scan0;


    int nOffset = stride - iWidth*3; 
    int nWidth = iWidth * 3;

    int nPixel = 0, nPixelMax = 0;

    p += stride;
    p2 += stride;
    int nThreshold = 0;

    for(int y=1;y < (iHeight-1);++y)
    {
        p += 3;
        p2 += 3;

        for(int x=3; x < (nWidth-3); ++x )
        {  			

            nPixelMax = abs((p2 - stride + 3)[0] - (p2+stride-3)[0]);
            nPixel = abs((p2 + stride + 3)[0] - (p2 - stride - 3)[0]);
            if (nPixel>nPixelMax)
                nPixelMax = nPixel;

            nPixel = abs((p2 - stride)[0] - (p2 + stride)[0]);
            if (nPixel>nPixelMax)
                nPixelMax = nPixel;

            nPixel = abs((p2+3)[0] - (p2 - 3)[0]);
            if (nPixel>nPixelMax)
                nPixelMax = nPixel;

            if (nPixelMax < nThreshold) 
                nPixelMax = 0;							

            p[0] = (byte) nPixelMax;

            ++ p;
            ++ p2;

        }

        p += 3 + nOffset;
        p2 += 3 + nOffset;
    }


    b.UnlockBits(&bmData);
    b2->UnlockBits(&bmData2);

    graphics.DrawImage(b2,0,0,iWidth,iHeight);
    graphics.DrawImage(&b, iWidth+10, 0, iWidth, iHeight); 
}

void OnPaint2(HDC hdc)
{
    Graphics graphics(hdc);
    //Gdiplus::Bitmap bitmap(L"C:\\code.jpg");
    //Gdiplus::Bitmap bitmap(L"C:\\validCode.jpg");
    Gdiplus::Bitmap bitmap(L"code.jpg");
    graphics.DrawImage(&bitmap, 10, 10);

    // 灰度化 二值化
    BitmapData bitmapData;
    Gdiplus::Rect r(0, 0, bitmap.GetWidth(), bitmap.GetHeight());
    bitmap.LockBits(&r, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
    PARGBQuad p = (PARGBQuad)bitmapData.Scan0;
    int offset = bitmapData.Stride - bitmapData.Width * sizeof(ARGBQuad);
    assert(offset == 0);
    int *zft = new int[bitmap.GetWidth()]();
    for (UINT y=0; y<bitmapData.Height; ++y, p += offset)
    {
        for (UINT x=0; x<bitmapData.Width; ++x,++p)
        {
            if ( ((p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8) < 128 )
            {
                p->Color &= 0xff000000;
                ++zft[x];
            }
            else
            {
                p->Color |= 0x00ffffff;
            }
        }
    }
    bitmap.UnlockBits(&bitmapData);

    // 输出直方图
    Pen pen(Color(255, 0, 0, 255));
    for (int i=0; i<bitmap.GetWidth(); ++i)
    {
        graphics.DrawLine(&pen, 10+i, 10 + 2*bitmap.GetHeight(), 10+i, 10 + 2*bitmap.GetHeight() - zft[i]);
    }


    // 分割字符   
    bool bPair = false;
    vector<int> vecSpilt;
    for (int y=0; y<bitmap.GetHeight(); ++y)
    {
        vecSpilt.clear();
        bPair = false;
        for (int x=0; x<bitmap.GetWidth(); ++x)
        {
            if (!bPair && zft[x] > y)
            {
                vecSpilt.push_back(x);
                bPair = true;
            }
            else if (bPair && zft[x] <= y)
            {
                if (x - vecSpilt.back() < 3)
                    break;
                vecSpilt.push_back(x);
                bPair = false;
            }
            else if (bPair && x==bitmap.GetWidth()-1 && zft[x] > y)
            {
                vecSpilt.push_back(x);
            }
        }
        if (vecSpilt.size() == 8)
        {
            for (int i=0; i<vecSpilt.size(); i+=2)
            {
                graphics.DrawImage(&bitmap, GpRect(10 + vecSpilt[i], 10 + 2 * bitmap.GetHeight() + 4,  vecSpilt[i+1] - vecSpilt[i], bitmap.GetHeight()),
                    vecSpilt[i], 0, vecSpilt[i+1] - vecSpilt[i], bitmap.GetHeight(), UnitPixel);
                // 垂直扫描
                bitmap.LockBits(&r, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
                PARGBQuad p = (PARGBQuad)bitmapData.Scan0;
                bool bFind = false;
                for (size_t vY = 0; vY < bitmap.GetHeight() && !bFind; ++vY)
                {
                    for (size_t hX = vecSpilt[i]; hX < vecSpilt[i+1]; ++hX)
                    {
                        if (p[vY*bitmapData.Width + hX].Red == 0)
                        {
                            graphics.DrawLine(&pen, 10 + vecSpilt[i], 10 + 3 * bitmap.GetHeight() + 4 - vY - 1, 10 + vecSpilt[i+1], 10 + 3 * bitmap.GetHeight() + 4 - vY - 1);
                            bFind = true;
                            break;
                        }
                    }
                }
                bFind = false;
                for (size_t vY = bitmap.GetHeight()-1; vY >= 0 && !bFind; --vY)
                {
                    for (size_t hX = vecSpilt[i]; hX < vecSpilt[i+1]; ++hX)
                    {
                        if (p[vY*bitmapData.Width + hX].Red == 0)
                        {
                            graphics.DrawLine(&pen, 10 + vecSpilt[i], 10 + 3 * bitmap.GetHeight() + 4 - vY - 2, 10 + vecSpilt[i+1], 10 + 3 * bitmap.GetHeight() + 4 - vY - 2);
                            bFind = true;
                            break;
                        }
                    }
                }
                bitmap.UnlockBits(&bitmapData);
                graphics.DrawLine(&pen, 10 + vecSpilt[i], 10 + 2 * bitmap.GetHeight() + 4,  10 + vecSpilt[i], 10 + 3 * bitmap.GetHeight() + 4);
                graphics.DrawLine(&pen, 10 + vecSpilt[i+1], 10 + 2 * bitmap.GetHeight() + 4,  10 + vecSpilt[i+1], 10 + 3 * bitmap.GetHeight() + 4);
            }

            break;
        }
    }

    graphics.DrawImage(&bitmap,10 + bitmapData.Width + 2, 10);

    delete []zft;
}

void OnCommand(WORD notifyCode, WORD itemID, HWND ctlHandle)
{
    switch (notifyCode)
    {
    case WM_LBUTTONDOWN:
        MessageBox(ctlHandle, "ok", "ok", MB_OK);
        break;
    default:
        break;
    }
}

CWinApplication *g_pWinApp;

void OnLButtonDown()
{
    string strUrl = "https://dynamic.12306.cn/otsweb/passCodeAction.do?rand=sjrand";
    time_t now = time(0);
    ostringstream os;
    os << strUrl << "&" << now;
    strUrl = os.str();
    os.str("");
    os << "code" << now << ".jpg";
    
    //LRESULT hr = URLDownloadToFile(NULL, strUrl.c_str(), "code.jpg", 0, NULL);
    //Gdiplus::Rect r(0, 0, 400, 300);
    //if (hr == S_OK) //INET_E_DOWNLOAD_FAILURE
    //    InvalidateRect(g_pWinApp->GetHwnd(), (LPRECT)&r, true);

    //strUrl = "http://www.gsao.fudan.edu.cn/addon/achieve/achievementdoc/validateimage.jsp?siteId=3&pageId=0&rand1367913776683";
    string strFile = "code.jpg";//os.str();
    HINTERNET hNet = InternetOpen("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)", 
        PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
    HINTERNET hUrlFile = InternetOpenUrl(hNet, strUrl.c_str(), NULL, 0, 
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
    char buffer[10240] = {0};
    DWORD dwBytes = 0;

    if (InternetReadFile(hUrlFile, buffer, sizeof(buffer), &dwBytes) && dwBytes > 0)
    {
        ofstream fout(strFile.c_str(), ios::binary);
        if (fout)
        {
            while (dwBytes > 0)
            {
                fout.write(buffer, dwBytes);
                InternetReadFile(hUrlFile, buffer, sizeof(buffer), &dwBytes);
            }
            fout.close();
            Gdiplus::Rect r(0, 0, 400, 300);
            InvalidateRect(g_pWinApp->GetHwnd(), (LPRECT)&r, true);
        }  
    } 

    InternetCloseHandle(hUrlFile);
    InternetCloseHandle(hNet);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT iCmdShow)
{
    CWinApplication WinApp(hInstance, iCmdShow);
    g_pWinApp = &WinApp;
    PaintProc       = OnPaint;
    CommandProc     = OnCommand;
    LButtonDownProc = OnLButtonDown;
    WinApp.Run();
    return 0;
} // WinMain
