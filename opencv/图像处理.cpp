// http://bbs.csdn.net/topics/370159961 xiaoc1026


/************************************************************
*
*  函数名称：
*       RemoveScatterNoise()
*
*  参数：
*     HDIB    hDIB     －原图像的句柄
*
*  返回值:
*       无
*
*  功能：
*     通过对连续点长度的统计来去除离散杂点
*
*  说明：
*      只能对2值图像进行处理
****************************************************************/
void RemoveScatterNoise(HDIB hDIB)
{
	
	// 指向DIB的指针
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;	

	// 找到DIB图像象素数据区的起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	//获得图像的长度
	LONG lWidth=::DIBWidth ((char*)lpDIB);

	//获得图像的高度
	LONG lHeight=::DIBHeight ((char*)lpDIB);

	//设置判定噪声的长度阈值为15
	//即如果与考察点相连接的黑点的数目小于15则认为考察点是噪声点
	int length=15;
	
	// 循环变量
	m_lianXuShu=0;
	LONG	i;
	LONG	j;	
	LONG    k;

	// 图像每行的字节数
	LONG	lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

    
	LPSTR lpSrc;

	//开辟一块用来存放标志的内存数组
	LPBYTE lplab = new BYTE[lHeight * lWidth];

	//开辟一块用来保存离散判定结果的内存数组
	bool *lpTemp = new bool[lHeight * lWidth];

    //初始化标志数组
	for (i=0;i<lHeight*lWidth;i++)
    {

    //将所有的标志位设置为非
	lplab[i] = false;

	}

	//用来存放离散点的坐标的数组
	CPoint lab[21];
   
	//为循环变量赋初始值
	k=0;

	//扫描整个图像

	//逐行扫描
	for(i =0;i<lHeight;i++)
	{  
       
	   //逐行扫描
		for(j=0;j<lWidth;j++)
			{	
				//先把标志位置false
				for(k=0;k<m_lianXuShu;k++)
				lplab[lab[k].y * lWidth + lab[k].x] = false;

				//连续数置0
				m_lianXuShu =0;

			    //进行离散性判断
			    lpTemp[i*lWidth+j] = DeleteScaterJudge(lpDIBBits,(WORD)lLineBytes,lplab,lWidth,lHeight,j,i,lab,length);

			}
	}
			
	//扫描整个图像，把离散点填充成白色

	//逐行扫描
	for(i = 0;i<lHeight;i++)
	{

      //逐列扫描
		for(j=0;j<lWidth;j++)
		{       
			    //查看标志位,如果为非则将此点设为白点
				if(lpTemp[i*lWidth+j] == false)
				{	
                   //指向第i行第j个象素的指针
					lpSrc=(char*)lpDIBBits + lLineBytes * i + j;

					//将此象素设为白点
					*lpSrc=BYTE(255);
				}
			}
	}

	//解除锁定
	::GlobalUnlock ((HGLOBAL)hDIB);

}



/*****************************************************************
*
*  函数名称 
*       DeleteScaterJudge()
*
*  参数：
*     LPSTR   lpDIBBits      －指向象素起始位置的指针
*	  WORD    lLineBytes     －图像每行的字节数
*     LPBYTE  lplab          －标志位数组
*     int     lWidth         －图像的宽度
*	  int     lHeight        －图像的高度
*     int     x              －当前点的横坐标
*	  int     y              －当前点的纵坐标 
*     CPoint  lab[]          －存放议考察过的连续点坐标
*     int     lianXuShu      －离散点的判定长度
*
*  返回值：
*     Bool                   －是离散点返回false 不是离散点返回true
*	 
*  功能：
*     利用递归算法统计连续点的个数，通过阈值来判定是否为离散点
*
*  说明：
*     只能对2值图像进行处理
******************************************************************/     


bool DeleteScaterJudge(LPSTR lpDIBBits, WORD lLineBytes, LPBYTE lplab, int lWidth, int lHeight, int x, int y, CPoint lab[], int lianXuShu)
{
	
	//如果连续长度满足要求，说明不是离散点，返回
	if(m_lianXuShu>=lianXuShu)
		return TRUE;

	//长度加一
	m_lianXuShu++;

	//设定访问标志
	lplab[lWidth * y +x] = true;
	
	//保存访问点坐标
	lab[m_lianXuShu-1].x = x;
	lab[m_lianXuShu-1].y = y;

	//象素的灰度值
	int gray;
  
	//指向象素的指针
	LPSTR lpSrc;

	//长度判定
    //如果连续长度满足要求，说明不是离散点，返回
	if(m_lianXuShu>=lianXuShu)
		return TRUE;
	
	//下面进入递归
	else
	{	
		//考察上下左右以及左上、右上、左下、右下八个方向
		//如果是黑色点，则调用函数自身进行递归

		//考察下面点
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y-1) + x;

		//传递灰度值
		gray=*lpSrc;

		//如果点在图像内、颜色为黑色并且没有被访问过
		if(y-1 >=0 && gray == 0 && lplab[(y-1)*lWidth+x] == false)

		//进行递归处理		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x,y-1,lab,lianXuShu);

		//判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
		return TRUE;
		
		//左下点
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y-1) + x-1;

        //传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(y-1 >=0 &&  x-1 >=0 && gray== 0 && lplab[(y-1)*lWidth+x-1] == false)

      	//进行递归处理		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x-1,y-1,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
		return TRUE;
		
		//左边
		
		lpSrc=(char*)lpDIBBits + lLineBytes * y + x-1;

		//传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(x-1 >=0 &&  gray== 0 && lplab[y*lWidth+x-1] == false)

        //进行递归处理		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x-1,y,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		
		//左上
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y+1) + x-1;

		//传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(y+1 <lHeight && x-1 >= 0 && gray == 0 && lplab[(y+1)*lWidth+x-1] == false)

		//进行递归处理
		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x-1,y+1,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		
		//上面
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y+1) + x;

        //传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(y+1 < lHeight && gray == 0 && lplab[(y+1)*lWidth+x] == false)

        //进行递归处理
		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x,y+1,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		
		//右上
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y+1) + x+1;
        
		//传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(y+1 <lHeight && x+1 <lWidth &&  gray == 0 && lplab[(y+1)*lWidth+x+1] == false)

        //进行递归处理
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x+1,y+1,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		
		//右边
	
		lpSrc=(char*)lpDIBBits + lLineBytes * y + x+1;

        //传递灰度值
		gray=*lpSrc;

		//如果点在图像内、颜色为黑色并且没有被访问过
		if(x+1 <lWidth && gray==0 && lplab[y*lWidth+x+1] == false)

        //进行递归处理		
		DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x+1,y,lab,lianXuShu);

        //判断长度

		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
		
		//右下
		
		lpSrc=(char*)lpDIBBits + lLineBytes * (y-1) + x+1;

        //传递灰度值
		gray=*lpSrc;

        //如果点在图像内、颜色为黑色并且没有被访问过
		if(y-1 >=0 && x+1 <lWidth && gray == 0 && lplab[(y-1)*lWidth+x+1] == false)

       //进行递归处理		
	   DeleteScaterJudge(lpDIBBits,lLineBytes,lplab,lWidth,lHeight,x+1,y-1,lab,lianXuShu);

        //判断长度
		//如果连续长度满足要求，说明不是离散点，返回
		if(m_lianXuShu>=lianXuShu)
			return TRUE;
	}
	

	//如果递归结束，返回false，说明是离散点
	return FALSE;

}
