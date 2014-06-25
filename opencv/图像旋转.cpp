//http://www.cnblogs.com/easymind223/p/3330687.html

void rotate(const Mat& src, Mat& dst, float angle)
{
    CV_Assert(!src.empty());

    float radian = angle /180.0 * PI;

    int uniSize = max(src.cols, src.rows) * 2;
    int dx = (uniSize - src.cols) / 2;
    int dy = (uniSize - src.rows) / 2;

    copyMakeBorder(src, dst, dy, dy, dx, dx, BORDER_CONSTANT);

    //旋轉中心
    Point2f center(dst.cols/2, dst.rows/2);
    Mat affine_matrix = getRotationMatrix2D( center, angle, 1.0 );

    warpAffine(dst, dst, affine_matrix, dst.size());

    float sinVal = fabs(sin(radian));
    float cosVal = fabs(cos(radian));

    //旋轉后的圖像大小
    Size targetSize(src.cols * cosVal + src.rows * sinVal,
                 src.cols * sinVal + src.rows * cosVal);

    //剪掉四周边框
    int x = (dst.cols - targetSize.width) / 2;
    int y = (dst.rows - targetSize.height) / 2;

    Rect rect(x, y, targetSize.width, targetSize.height);
    dst = Mat(dst, rect);
}