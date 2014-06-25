//http://blog.csdn.net/quarryman/article/details/6427886


#include <cv.h>
#include <highgui.h>
#pragma comment( lib, "cv.lib" )
#pragma comment( lib, "cxcore.lib" )
#pragma comment( lib, "highgui.lib" )
int main()
{	
	IplImage * src=cvLoadImage("lena.jpg",0);
//	cvSmooth(src,src,CV_BLUR,3,3,0,0);
	cvThreshold(src,src,50,255,CV_THRESH_BINARY_INV);
	IplImage* paintx=cvCreateImage( cvGetSize(src),IPL_DEPTH_8U, 1 );
	IplImage* painty=cvCreateImage( cvGetSize(src),IPL_DEPTH_8U, 1 );
	cvZero(paintx);
	cvZero(painty);
	int* v=new int[src->width];
	int* h=new int[src->height];
	memset(v,0,src->width*4);
	memset(h,0,src->height*4);
	
	int x,y;
	CvScalar s,t;
	for(x=0;x<src->width;x++)
	{
		for(y=0;y<src->height;y++)
		{
			s=cvGet2D(src,y,x);			
			if(s.val[0]==0)
				v[x]++;					
		}		
	}
	
	for(x=0;x<src->width;x++)
	{
		for(y=0;y<v[x];y++)
		{		
			t.val[0]=255;
			cvSet2D(paintx,y,x,t);		
		}		
	}
	
	for(y=0;y<src->height;y++)
	{
		for(x=0;x<src->width;x++)
		{
			s=cvGet2D(src,y,x);			
			if(s.val[0]==0)
				h[y]++;		
		}	
	}
	for(y=0;y<src->height;y++)
	{
		for(x=0;x<h[y];x++)
		{			
			t.val[0]=255;
			cvSet2D(painty,y,x,t);			
		}		
	}
	cvNamedWindow("二值图像",1);
	cvNamedWindow("垂直积分投影",1);
	cvNamedWindow("水平积分投影",1);
	cvShowImage("二值图像",src);
	cvShowImage("垂直积分投影",paintx);
	cvShowImage("水平积分投影",painty);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&paintx);
	cvReleaseImage(&painty);
	return 0;
}