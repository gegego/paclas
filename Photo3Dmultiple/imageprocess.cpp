#include "imageprocess.h"
#include <iostream>
#include <cmath>
#include <numeric>

using namespace std;
#define C1 (float) (0.01 * 255 * 0.01  * 255)
#define C2 (float) (0.03 * 255 * 0.03  * 255)

imageprocess::imageprocess()
{

}

std::vector<uchar> imageprocess::getYMiddleLine(QImage spec)
{
    Mat img = QtOcv::image2Mat(spec, CV_8UC1);
    std::vector<uchar> result(img.rows);

    for (int i=0; i<img.rows; ++i)
        result[i] = img.at<uchar>(i,img.cols/2);

    return result;
}

std::vector<uchar> imageprocess::getXMiddleLine(QImage spec)
{
    Mat img = QtOcv::image2Mat(spec, CV_8UC1);
    const uchar* p = img.ptr<uchar>(img.rows/2);
//    std::cout<<img<<endl;

//    for (int i=0; i<img.cols; ++i)
     std::vector<uchar> result(p, p+img.cols);
//    result.insert(result.end(), img.ptr<uchar>(img.rows/2), img.ptr<uchar>(img.rows/2)+img.cols);

//    for ( int i: result)
//      std::cout << i << ' ';
    return result;
}

QImage imageprocess::FilterLowPass(QImage org, int size)
{
    Mat img = QtOcv::image2Mat(org, CV_8UC1);
    m_Image = img.clone();
    Mat complex = computeDFT(img);
    m_Complex = complex.clone();
//    m_Mask = createNormalFilterMask(m_Complex.size(), m_Complex.cols/2, m_Complex.rows/2, size, true, false);

    m_Mask = createButterworthBandpass(complex.size(), complex.cols/2, complex.rows/2, size, size/2);

    computeMergeSpectra();

    return computeInverseResult();
}

QImage imageprocess::EqHist(QImage org)
{
    Mat img = QtOcv::image2Mat(org, CV_8UC1);

    equalizeHist(img, img);

    QImage result = QtOcv::mat2Image(img);

    return result;
}

int imageprocess::computeBrushDifference(QImage brushPatch){
    Mat img = QtOcv::image2Mat(brushPatch, CV_8UC1);
    int count_wihte = 0;
    int count_black = 0;

    vector<vector<Point>> contours;
    findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i)
    {
        // Check area
        if (contourArea(contours[i]) < 10) continue;

        count_wihte++;
    }
    img = Mat::ones(img.size(), CV_8UC1) - img;
    findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i)
    {
        // Check area
        if (contourArea(contours[i]) < 10) continue;

        count_black++;
    }

    return count_wihte>count_black?count_wihte : count_black;
}

double imageprocess::pearsoncoeff(vector<uchar> X, vector<uchar> Y)
{
    double result;
    std::vector<double> x_1 = minus_corr(X, mean_corr(X));
    std::vector<double> y_1 = minus_corr(Y, mean_corr(Y));
    std::vector<double> x_y = mult_corr(x_1, y_1);
    result = sum_corr(x_y) / (X.size()*stdev_corr(X)* stdev_corr(Y));

    return result;
}

int imageprocess::getSpectraLoopCount(std::vector<uchar> X)
{
    int result = 0;

//    double sum = accumulate(X.begin(), X.end(), 0.0);
//    double mean = sum / X.size();
    int min = *min_element(X.begin(), X.end());
//    bool down = X[0] < mean;
    bool start = false;
    int periodlenght = 0;
    for(int i =0; i< X.size()/2; i++ )
    {
        if(X[i] < min + 20 && !start)
        {
            start = true;
            periodlenght = 0;
        }
        if (X[i] < min + 20 && start)
        {
            if (periodlenght > 10)
            {
                break;
            }
        }
        if(start){
            periodlenght++;
        }

    }

    return periodlenght;
//    return 0;
}

QImage imageprocess::computeSpectra(QImage org) {
    Mat img = QtOcv::image2Mat(org, CV_8UC1);

    m_Image = img.clone();
    Mat complex = computeDFT(img);
    m_Complex = complex;

    m_filterMaxSize = img.rows < img.cols? img.rows/2: img.cols/2;

    Mat magI = updateMag(complex);

//    double th = computeThresholdValue(magI, 2000);
    threshold(magI, magI, 0.6, 1, 0);

    QImage result = QtOcv::mat2Image(magI);
    return result;
}

QImage imageprocess::getFilterById(int idxFilter, int type, int size, int order)
{
    Mat complex = m_Complex.clone();
    Mat mask;

    bool invert = type == 0 ? false : true;

    switch (idxFilter) {
    case 0:
        if(type == 2)
            mask = createNormalBandpass(complex.size(), complex.cols/2, complex.rows/2, size, size/2);
        else
            mask = createNormalFilterMask(complex.size(), complex.cols/2, complex.rows/2, size, true, invert);
        break;
    case 1:
        if(type == 2)
            mask = createGaussianBandpass(complex.size(), complex.cols/2, complex.rows/2, size, size/2);
        else
            mask = createGaussianFilterMask(complex.size(), complex.cols/2, complex.rows/2, m_filterMaxSize, size, true, invert);
        break;
    case 2:
        if(type == 2)
            mask = createButterworthBandpass(complex.size(), complex.cols/2, complex.rows/2, size, size/2);
        else
            mask = createButterworthFilterMask(complex.size(), complex.cols/2, complex.rows/2, size, order, true, invert);
        break;
    }

    m_Mask = mask;
    QImage result = QtOcv::mat2Image(mask);
    return result;
}

QImage imageprocess::changeFilterSize(int size)
{
    Mat complex = m_Complex.clone();
    Mat mask;

    mask = createNormalFilterMask(complex.size(), complex.cols/2, complex.rows/2, size, true, false);

    m_Mask = mask;
    QImage result = QtOcv::mat2Image(mask);
    return result;
}

double imageprocess::ssim(QImage img1, QImage img2, int block_size)
{
    double ssim = 0;

    Mat img_src = QtOcv::image2Mat(img1, CV_8UC1);
    Mat img_compressed = QtOcv::image2Mat(img2, CV_8UC1);

    img_src.convertTo(img_src, CV_64F);
    img_compressed.convertTo(img_compressed, CV_64F);

    int nbBlockPerHeight 	= img_src.rows / block_size;
    int nbBlockPerWidth 	= img_src.cols / block_size;

    for (int k = 0; k < nbBlockPerHeight; k++)
    {
        for (int l = 0; l < nbBlockPerWidth; l++)
        {
            int m = k * block_size;
            int n = l * block_size;

            double avg_o 	= mean(img_src(Range(k, k + block_size), Range(l, l + block_size)))[0];
            double avg_r 	= mean(img_compressed(Range(k, k + block_size), Range(l, l + block_size)))[0];
            double sigma_o 	= sigma(img_src, m, n, block_size);
            double sigma_r 	= sigma(img_compressed, m, n, block_size);
            double sigma_ro	= cov(img_src, img_compressed, m, n, block_size);

            ssim += ((2 * avg_o * avg_r + C1) * (2 * sigma_ro + C2)) / ((avg_o * avg_o + avg_r * avg_r + C1) * (sigma_o * sigma_o + sigma_r * sigma_r + C2));

        }

    }
    ssim /= nbBlockPerHeight * nbBlockPerWidth;

    std::cout<<ssim<<endl;
    return ssim;
}


int imageprocess::computeSpecSize(QImage org, int level){
    Mat spec = QtOcv::image2Mat(org, CV_8UC1);

    Mat complex = computeDFT(spec);
//    imshow("complex", complex);
    Mat magI = updateMag(complex);

//    double th = computeThresholdValue(magI, 2000);
    threshold(magI, magI, 0.6, 1, 0);

//    string file = "/Users/wejaq/Documents/test/temp/org.jpg";
//    imwrite(file, magI);

//    vector<int> result;
    int radius = magI.cols/2;
    CvPoint2D32f center;
    center.x = magI.cols/2;
    center.y = magI.rows/2;

    double magSum = sum(magI)[0];
//    cout<<magSum<<endl;

    for(int i=0; i< level; i++)
    {
        Mat magFiltered;
        int r = radius - i*(radius/level);
        Mat Mon_mask(magI.size(), CV_8UC1, Scalar(0,0,0));
        circle(Mon_mask, center, r, Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
        bitwise_not(Mon_mask,Mon_mask);// commenté ou pas = RP ou DML

        magI.copyTo(magFiltered, Mon_mask);

        double rate = sum(magFiltered)[0]/magSum;
//        cout<<rate<<endl;
        if(rate > 0.08){
//            cout<<level-i<<endl;
            return level-i+1;
        }
    }

    return 1;
}

double imageprocess::computeThresholdValue(Mat m, int n)
{
    vector<float> temp;
//    cout<<m<<endl;
    for(int y = 0; y < m.rows; y++)
    {
        for (int x = 0; x < m.cols; x++)
        {
            temp.push_back(m.at<float>(y,x));
        }
    }
    std::sort (temp.begin(), temp.end());
    if(temp.size() > 0)
        return (double)temp[temp.size()-2000];

    return 0.6;
}

QImage imageprocess::DeleteFocus(QImage input)
{
    Mat image = QtOcv::image2Mat(input, CV_8UC1);

    Mat src_gray;
    Mat src_blur;
    Mat dst;
    Mat abs_dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    GaussianBlur( image, src_gray, Size(3,3), 0, 0, BORDER_DEFAULT );
//    cvtColor( src_blur, src_gray, COLOR_BGR2GRAY );

    //second gradient operation
    Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( dst, abs_dst );

    Mat _out;
    threshold( abs_dst, _out, 80, 255, 0 );


    Scalar color = Scalar( 255, 255, 255);

    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = _out.begin<uchar>();
    cv::Mat_<uchar>::iterator end = _out.end<uchar>();
    for (; it != end; ++it)
      if (*it)
        points.push_back(it.pos());

    // Find the convex hull object for each contour
    vector<vector<Point> >hull(1);
    convexHull( Mat(points), hull[0], false );
    // Draw hull results
    Mat _gray;
//    cvtColor(image.clone(), _gray, COLOR_RGB2GRAY );
//    cvtColor(_gray, _gray, COLOR_GRAY2BGR);
//    Mat drawing = _gray;
    drawContours( image, hull, 0, color, CV_FILLED, 8, vector<Vec4i>(), 0, Point() );

    QImage result = QtOcv::mat2Image(image);

    return result;
}

QImage imageprocess::computeMergeSpectra() {
    Mat mask = m_Mask.clone();
    Mat complex = m_Complex.clone();

//    shift(mask);

    //apply the mask
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    Mat kernel_spec;
    planes[0] = mask; // real
    planes[1] = mask; // imaginar
    merge(planes, 2, kernel_spec);

    shift(complex);

    mulSpectrums(complex, kernel_spec, complex, DFT_ROWS); // only DFT_ROWS accepted

    shift(complex);

    //mulSpectrums(complex, kernel_spec, complex, 0);

    m_MergeComplex = complex.clone();
//    m_Complex = complex.clone();
    Mat magI = updateMag(complex);

        //cout << "M = "<< endl << " "  << magI << endl << endl;


    QImage result = QtOcv::mat2Image(magI);
    return result;
}

QImage imageprocess::computeInverseResult() {
    Mat complex = m_MergeComplex.clone();
    Mat work;
    idft(complex, work);
//  dft(complex, work, DFT_INVERSE + DFT_SCALE);
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};

    split(work, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
//    normalize(planes[0], imgOutput, 0, 1, CV_MINMAX);

//    magnitude(planes[0], planes[1], work);    // === sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    normalize(planes[0], work, 0, 1, CV_MINMAX);

    threshold(work, work, 0.5, 1, 0);

    m_ftResult = work.clone();

    QImage result = QtOcv::mat2Image(work);
    return result;
}

Mat imageprocess::updateMag(Mat complex) {

    Mat magI;
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(complex, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    magnitude(planes[0], planes[1], magI);    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

    // switch to logarithmic scale: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    shift(magI);
    normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
                                              // viewable image form (float between values 0 and 1).
//    threshold(magI, magI, 0.6, 1, 0);

//    threshold(magI, magI, 0.4, 1, THRESH_TOZERO);

//    std::cout<<magI<<endl;
    return magI;
}

Mat imageprocess::computeDFT(Mat image) {
    // http://opencv.itseez.com/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    //problem of center line
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(image), Mat::zeros(image.size(), CV_32F)};
    Mat complex;
    merge(planes, 2, complex);         // Add to the expanded another plane with zeros
    dft(complex, complex, DFT_COMPLEX_OUTPUT);  // furier transform
    return complex;
}

Mat imageprocess::createNormalFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert) {
    // Some corrections if out of bounds
    if(x < (ksize / 2)) {
        ksize = x * 2;
    }
    if(y < (ksize / 2)) {
        ksize = y * 2;
    }
    if(mask_size.width - x < ksize / 2 ) {
        ksize = (mask_size.width - x ) * 2;
    }
    if(mask_size.height - y < ksize / 2 ) {
        ksize = (mask_size.height - y) * 2;
    }

    cv::Mat mask = cv::Mat::zeros(mask_size, CV_32F);
    cv::circle(mask, cv::Point(x, y), ksize, cv::Scalar::all(1), -1, 8, 0);
//    cv::rectangle(mask, cv::Point(0, y-ksize), cv::Point(x*2, y+ksize), cv::Scalar::all(1), -1, 8, 0);
//    cv::rectangle(mask, cv::Point(x-ksize, 0), cv::Point(x+ksize, y*2), cv::Scalar::all(1), -1, 8, 0);

    //cv::circle(mask, cv::Point(x, y), ksize/2, cv::Scalar::all(0), -1, 8, 0);

    // transform mask to range 0..1
    if(normalization) {
        normalize(mask, mask, 0, 1, NORM_MINMAX);
    }

    // invert mask
    if(invert) {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }

    return mask;
}

Mat imageprocess::createGaussianFilterMask(Size mask_size, int x, int y, int ksize, int order, bool normalization, bool invert) {
    // Some corrections if out of bounds
    if(x < (ksize / 2)) {
        ksize = x * 2;
    }
    if(y < (ksize / 2)) {
        ksize = y * 2;
    }
    if(mask_size.width - x < ksize / 2 ) {
        ksize = (mask_size.width - x ) * 2;
    }
    if(mask_size.height - y < ksize / 2 ) {
        ksize = (mask_size.height - y) * 2;
    }
    ksize= ksize *2 ;
    // call openCV gaussian kernel generator
    double sigma = order;
    Mat kernelX = getGaussianKernel(ksize, sigma, CV_32F);
    Mat kernelY = getGaussianKernel(ksize, sigma, CV_32F);
    // create 2d gaus
    Mat kernel = kernelX * kernelY.t();
    // create empty mask
    Mat mask = Mat::zeros(mask_size, CV_32F);
    Mat maski = Mat::zeros(mask_size, CV_32F);

    // copy kernel to mask on x,y
    Mat pos(mask, Rect(x - ksize / 2, y - ksize / 2, ksize, ksize));
    kernel.copyTo(pos);

    // create mirrored mask
    Mat posi(maski, Rect(( mask_size.width - x) - ksize / 2, (mask_size.height - y) - ksize / 2, ksize, ksize));
    kernel.copyTo(posi);
    // add mirrored to mask
    add(mask, maski, mask);

//    cout << "M = "<< endl << " "  << mask << endl << endl;

    // transform mask to range 0..1
    if(normalization) {
        normalize(mask, mask, 0, 1, NORM_MINMAX);
    }

    // invert mask
    if(invert) {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }

    return mask;
}

Mat imageprocess::createButterworthFilterMask(Size mask_size, int x, int y, int ksize, int order, bool normalization, bool invert)
{
    // Some corrections if out of bounds
    if(x < (ksize / 2)) {
        ksize = x * 2;
    }
    if(y < (ksize / 2)) {
        ksize = y * 2;
    }
    if(mask_size.width - x < ksize / 2 ) {
        ksize = (mask_size.width - x ) * 2;
    }
    if(mask_size.height - y < ksize / 2 ) {
        ksize = (mask_size.height - y) * 2;
    }

    // based on the forumla in the IP notes (p. 130 of 2009/10 version)
    // see also HIPR2 on-line
    cv::Mat mask = cv::Mat::zeros(mask_size, CV_32F);
    Point centre = Point(mask.rows / 2, mask.cols / 2);
    double radius;

    for(int i = 0; i < mask.rows; i++)
    {
        for(int j = 0; j < mask.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            mask.at<float>(i,j) = (float)
                        ( 1 / (1 + pow((double) (radius /  ksize), (double) (2 * order))));
        }
    }

    if(normalization) {
        normalize(mask, mask, 0, 1, NORM_MINMAX);
    }

    //tmp = Mat::ones(tmp.rows, tmp.cols, CV_32F) - tmp;
    if(invert) {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }

    return mask;
}

Mat imageprocess::createNormalBandpass(Size mask_size, int x, int y, int outsize, int insize){
    Mat outside, inside;
    Mat complex = m_Complex.clone();

    outside = createNormalFilterMask(mask_size, x, y, outsize, true, false);

    inside = createNormalFilterMask(mask_size, x, y, insize, true, true);

    Mat mask = outside.mul(inside);
    return mask;
}

Mat imageprocess::createGaussianBandpass(Size mask_size, int x, int y, int outsize, int insize){
    Mat outside, inside;
    Mat complex = m_Complex.clone();

    outside = createGaussianFilterMask(mask_size, x, y, m_filterMaxSize, outsize, true, false);

    inside = createGaussianFilterMask(mask_size, x, y, m_filterMaxSize, insize, true, true);

    Mat mask = outside.mul(inside);
    return mask;
}

Mat imageprocess::createButterworthBandpass(Size mask_size, int x, int y, int outsize, int insize){
    Mat outside, inside;
    Mat complex = m_Complex.clone();

    outside = createButterworthFilterMask(mask_size, x, y, outsize, 5, true, false);

    inside = createButterworthFilterMask(mask_size, x, y, insize, 5, true, true);

    Mat mask = outside.mul(inside);
    return mask;
}

void imageprocess::shift(Mat magI) {

    // crop if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

double imageprocess::sigma(Mat & m, int i, int j, int block_size)
{
    double sd = 0;

    Mat m_tmp = m(Range(i, i + block_size), Range(j, j + block_size));
    Mat m_squared(block_size, block_size, CV_64F);

    multiply(m_tmp, m_tmp, m_squared);

    // E(x)
    double avg = mean(m_tmp)[0];
    // E(x²)
    double avg_2 = mean(m_squared)[0];


    sd = sqrt(avg_2 - avg * avg);

    return sd;
}

// Covariance
double imageprocess::cov(Mat & m1, Mat & m2, int i, int j, int block_size)
{
    Mat m3 = Mat::zeros(block_size, block_size, m1.depth());
    Mat m1_tmp = m1(Range(i, i + block_size), Range(j, j + block_size));
    Mat m2_tmp = m2(Range(i, i + block_size), Range(j, j + block_size));


    multiply(m1_tmp, m2_tmp, m3);

    double avg_ro 	= mean(m3)[0]; // E(XY)
    double avg_r 	= mean(m1_tmp)[0]; // E(X)
    double avg_o 	= mean(m2_tmp)[0]; // E(Y)


    double sd_ro = avg_ro - avg_o * avg_r; // E(XY) - E(X)E(Y)

    return sd_ro;
}

double imageprocess::sum_corr(std::vector<uchar> a)
{
    double s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += a[i];
    }
    return s;
}

double imageprocess::sum_corr(std::vector<double> a)
{
    double s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += a[i];
    }
    return s;
}

double imageprocess::mean_corr(std::vector<uchar> a)
{
    return sum_corr(a) / a.size();
}

double imageprocess::sqsum_corr(std::vector<uchar> a)
{
    double s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += pow(a[i], 2);
    }
    return s;
}

double imageprocess::stdev_corr(std::vector<uchar> nums)
{
    double N = nums.size();
    return pow(sqsum_corr(nums) / N - pow(sum_corr(nums) / N, 2), 0.5);
}

std::vector<double> imageprocess::minus_corr(std::vector<uchar> a, double b)
{
    vector<double> retvect;
    for (int i = 0; i < a.size(); i++)
    {
        retvect.push_back(a[i] - b);
    }
    return retvect;
}

std::vector<double> imageprocess::mult_corr(std::vector<double> a, std::vector<double> b)
{
    vector<double> retvect;
    for (int i = 0; i < a.size() ; i++)
    {
        retvect.push_back(a[i] * b[i]);
    }
    return retvect;
}
