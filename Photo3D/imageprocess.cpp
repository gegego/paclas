#include "imageprocess.h"
#include <iostream>

using namespace std;

imageprocess::imageprocess()
{

}

QImage imageprocess::computeSpectra(QImage org) {
    Mat img = QtOcv::image2Mat(org, CV_8UC1);
//    imshow("org", img);
//    equalizeHist(img, img); //equalize the histogram

    m_Image = img.clone();
    Mat complex = computeDFT(img);
//    imshow("complex", complex);

    m_Complex = complex;

    m_filterMaxSize = img.rows < img.cols? img.rows/2: img.cols/2;

    Mat magI = updateMag(complex);

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
            mask = createNormalBandpass(complex.size(), complex.cols/2, complex.rows/2, size, size*2/3);
        else
            mask = createNormalFilterMask(complex.size(), complex.cols/2, complex.rows/2, size, true, invert);
//            mask = createGaborFilterMask(complex.size());
//            mask = createGaborFilterMask(complex.size());
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
    case 3:
        Mat complex = m_Complex.clone();
        Mat magI = updateMag(complex);
        if(type == 0)
            threshold(magI, magI, (float)size/10, 1, 3);
        else
            threshold(magI, magI, (float)size/10, 1, 4);

        mask = magI;
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


QImage imageprocess::computeMergeSpectra() {
//    filter2D(src_f, dest, CV_32F, m_Mask);

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

int imageprocess::computeBrushDifference(QImage brushPatch){
    Mat img = QtOcv::image2Mat(brushPatch, CV_8UC1);
    int count = 0;
    vector<vector<Point>> contours;
    findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i)
    {
        // Check area
        if (contourArea(contours[i]) < 10) continue;

        count++;
    }
    img = Mat::ones(img.size(), CV_8UC1) - img;
    findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i)
    {
        // Check area
        if (contourArea(contours[i]) < 10) continue;

        count++;
    }
    return count;
}

int imageprocess::computeSpecSize(QImage org, int level){
    Mat spec = QtOcv::image2Mat(org, CV_8UC1);

    Mat complex = computeDFT(spec);
//    imshow("complex", complex);
    Mat magI = updateMag(complex);
    threshold(magI, magI, 0.6, 1, 0);
    Mat magFiltered;

    vector<int> result;
    int radius = magI.cols/2;
    CvPoint2D32f center;
    center.x = magI.cols/2;
    center.y = magI.rows/2;
//    int r = magI.size().height()/8;

    double magSum = sum(magI)[0];
    cout<<magSum<<endl;

    for(int i=0; i< level; i++)
    {
        int r = radius - i*(radius/level);
        Mat Mon_mask(magI.size(), CV_8UC1, Scalar(0,0,0));
        circle(Mon_mask, center, r, Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
        bitwise_not(Mon_mask,Mon_mask);// commenté ou pas = RP ou DML

        magI.copyTo(magFiltered, Mon_mask);

        double rate = sum(magFiltered)[0]/magSum;
        cout<<rate<<endl;
        if(rate > 0.08){
            cout<<level-i<<endl;
            return level-i;
        }
    }

    return 0;
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

    return magI;
}

Mat imageprocess::computeDFT(Mat image) {
    // http://opencv.itseez.com/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
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

Mat imageprocess::createGaborFilterMask(Size mask_size){
    Mat mask = cv::Mat::zeros(mask_size, CV_32F);
    double sig = 30, lm = 20, gm = 0.1, ps = 0.2;
    double theta = 0;

    cout<<mask.size()<<endl;
    cv::Rect cut(0, 0, mask_size.width, mask_size.height);
    mask = cv::getGaborKernel(mask_size, sig, theta, lm, gm, ps, CV_32F);
    mask = mask(cut);
    cout<<mask.size()<<endl;


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
