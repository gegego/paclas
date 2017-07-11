#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>

#include "cvmatandqimage.h"

using namespace cv;

class imageprocess
{
public:
    imageprocess();

    QImage computeSpectra(QImage org);
    int getfilterMaxSize() {return this->m_filterMaxSize;}

    QImage getFilterById(int idxFilter, int type, int size, int order);
    QImage changeFilterSize(int size);
    QImage changeFilterSigma(int sigma);

    QImage computeMergeSpectra();
    QImage computeInverseResult();

    int computeBrushDifference(QImage brushPatch);
    int computeSpecSize(QImage org, int v);


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

    Mat createGaborFilterMask(Size mask_size);

    //band pass
    Mat createNormalBandpass(Size mask_size, int x, int y, int outsize, int insize);
    Mat createGaussianBandpass(Size mask_size, int x, int y, int outsize, int insize);
    Mat createButterworthBandpass(Size mask_size, int x, int y, int outsize, int insize);

    void shift(Mat magI);
};

#endif // IMAGEPROCESS_H
