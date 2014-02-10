CBitmap从文件中加载图片
HBITMAP bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),strFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);


//============================================================
双缓冲实现
CDC *m_pDC;
CDC MemDC;
CBitmap MemBitmap;
CBitmap *pOldbitmap;

MemDC.CreateCompatibleDC(m_pDC);                          // 创建内存兼容设备上下文
MemBitmap.CreateCompatibleBitmap(m_pDC,xRange,yRange);    // 创建内存兼容画布，大小由逻辑范围决定
pOldbitmap = MemDC.SelectObject(&MemBitmap);              // 将画布选入内存设备上下文

// 对内存中的画布填充背景颜色，否则是默认的黑色
MemDC.FillSolidRect(0,0,xRange,yRange,RGB(123,213,132)); 

// 画图操作，如画一条对角直线
MemDC.MoveTo(0,0);
MemDC.LineTo(xRange*0.9,yRange*0.9);

// 将内存中的画图区域拷贝到界面的控件区域上去
// 第1和第2个参数若是0时，则从物理坐标的(rect.left,rect.bottom)点上开始按上述指定的方向贴图
m_pDC->BitBlt(0,0,xRange,yRange,&MemDC,0,0,SRCCOPY);



//============================================================
CImage类显示图片
注意：VC6.0中不支持CImage
包含头文件：atlimage.h

CImage *pImage = new CImage();
pImage->Load(_T("test.jpg"));
pImage->Draw(*pDC, 0, 0);
delete pImage;



