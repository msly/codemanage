findContours函数，这个函数的原型为：
void findContours(InputOutputArray image, OutputArrayOfArrays contours, OutputArray hierar-
chy, int mode, int method, Point offset=Point())
参数说明
输入图像image必须为一个2值单通道图像
contours参数为检测的轮廓数组，每一个轮廓用一个point类型的vector表示
hiararchy参数和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
mode表示轮廓的检索模式
CV_RETR_EXTERNAL表示只检测外轮廓
CV_RETR_LIST检测的轮廓不建立等级关系
CV_RETR_CCOMP建立两个等级的轮廓，上面的一层为外边界，里面的一层为内孔的边界信息。如果内孔内还有一个连通物体，这个物体的边界也在顶层。
CV_RETR_TREE建立一个等级树结构的轮廓。具体参考contours.c这个demo
method为轮廓的近似办法
CV_CHAIN_APPROX_NONE存储所有的轮廓点，相邻的两个点的像素位置差不超过1，即max（abs（x1-x2），abs（y2-y1））==1
CV_CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
CV_CHAIN_APPROX_TC89_L1，CV_CHAIN_APPROX_TC89_KCOS使用teh-Chinl chain 近似算法
offset表示代表轮廓点的偏移量，可以设置为任意值。对ROI图像中找出的轮廓，并要在整个图像中进行分析时，这个参数还是很有用的。
findContours后会对输入的2值图像改变，所以如果不想改变该2值图像，需创建新mat来存放，findContours后的轮廓信息contours可能过于复杂不平滑，可以用approxPolyDP函数对该多边形曲线做适当近似
contourArea函数可以得到当前轮廓包含区域的大小，方便轮廓的筛选



findContours
findContours经常与drawContours配合使用，用来将轮廓绘制出来。
第一个参数image表示目标图像，
第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，
第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，
第四个参数color为轮廓的颜色，
第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，
第六个参数lineType为线型，
第七个参数为轮廓结构信息，
第八个参数为maxLevel



对图像的不规则区域设置ROI

OpenCV自带的函数cvSetImageROI( IplImage* image, CvRect rect )只能设置矩形的敏感区域，而实际图像处理中遇到的处理对象都是非矩形的不规则形状，此时用cvSetImageROI( IplImage* image, CvRect rect )显然达不到目的。我们可以用以下操作进行代替。

//----------------------------------------------------------

方法一：

cvCopy(src,dst,mask);

                                 //mask与src,dst通道数可以不一样。

方法二：
       cvXor(src,mask,dst);

                                 //三者通道数必须一样。

//----------------------------------------------------------

用上面语句就可以将不规则区域图像抠出来（不改变图像的大小），进而做相应的处理。然而如何生成不规则区域的mask图像呢？

 

不规则区域的mask图像的生成（假设目标是实现对一不规则物体区域设置ROI）

步骤：

1.     提取物体的最外围轮廓contour，使之为一连通域。

//----------------------------------------------------------

cvFindContours( gray, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL,                            CV_CHAIN_APPROX_SIMPLE);

//----------------------------------------------------------

 

2.     对轮廓连通域进行颜色填充。

//----------------------------------------------------------

方法一：

获取轮廓内的任意一点作为cvFloodFill操作的起始种子点：

   CvRect s;

   CvPoint pt;

   s=cvBoundingRect(contour);

   pt = cvPoint(s.x+s.width/2,s.y+s.height/2);//-------可以用其他方式获得连通域的一个内点作为起始种子点

 

对轮廓连通域进行颜色填充：

   cvFloodFill(gray, pt ,cvScalarAll(255));

 

方法二：

将轮廓内部填充为白色，其他区域为黑色：

cvDrawContours(gray, contour,  CV_RGB(255,255,255), CV_RGB(255,255 ,255),  -1, CV_FILLED, 8);
 

//----------------------------------------------------------

3.     此时获得的图像gray即为不规则区域的mask。
