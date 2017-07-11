#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>
#include <iostream>

#include "cvmatandqimage.h"


using namespace cv;

class imageprocess
{
public:
    imageprocess();

    std::vector<uchar> getXMiddleLine(QImage spec);
    std::vector<uchar> getYMiddleLine(QImage spec);

    QImage computeSpectra(QImage org);
    int getfilterMaxSize() {return this->m_filterMaxSize;}

    QImage getFilterById(int idxFilter, int type, int size, int order);
    QImage changeFilterSize(int size);
    QImage changeFilterSigma(int sigma);

    QImage computeMergeSpectra();
    QImage computeInverseResult();

    double ssim(QImage img1, QImage img2, int block_size);
    double pearsoncoeff(std::vector<uchar> X, std::vector<uchar> Y);

    int getSpectraLoopCount(std::vector<uchar> X);

    QImage DeleteFocus(QImage input);
    QImage FilterLowPass(QImage org, int size);

    int computeBrushDifference(QImage brushPatch);

    QImage EqHist(QImage img); //equalize the histogram

    int computeSpecSize(QImage org, int level);

private:
    int m_filterMaxSize = 0;
    Mat m_Image;
    Mat m_Complex;
    Mat m_Mask;
    Mat m_MergeComplex;
    Mat m_ftResult;

    Mat updateMag(Mat complex);
    Mat computeDFT(Mat image);
    Mat createNormalFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert);
    Mat createGaussianFilterMask(Size mask_size, int x, int y, int ksize, int order, bool normalization, bool invert);
    Mat createButterworthFilterMask(Size mask_size, int x, int y, int ksize, int order, bool normalization, bool invert);

    //band pass
    Mat createNormalBandpass(Size mask_size, int x, int y, int outsize, int insize);
    Mat createGaussianBandpass(Size mask_size, int x, int y, int outsize, int insize);
    Mat createButterworthBandpass(Size mask_size, int x, int y, int outsize, int insize);

    void shift(Mat magI);

    double computeThresholdValue(Mat m, int n);

    double sigma(Mat & m, int i, int j, int block_size);
    double cov(Mat & m1, Mat & m2, int i, int j, int block_size);

    double sum_corr(std::vector<uchar> a);
    double sum_corr(std::vector<double> a);
    double mean_corr(std::vector<uchar> a);
    double sqsum_corr(std::vector<uchar> a);
    double stdev_corr(std::vector<uchar> nums);
    std::vector<double> minus_corr(std::vector<uchar> a, double b);
    std::vector<double> mult_corr(std::vector<double> a, std::vector<double> b);

};

#endif // IMAGEPROCESS_H
