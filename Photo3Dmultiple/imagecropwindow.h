#ifndef IMAGECROPWINDOW_H
#define IMAGECROPWINDOW_H
#include <QFileDialog>
#include <QWidget>
#include "imageprocess.h"
#include "surfacegraph.h"
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <iostream>
#include "paintinginfo.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

using namespace cv::ml;
using namespace cv;
using namespace std;

class ImageCropWindow : public QWidget
{
    Q_OBJECT
public:
    ImageCropWindow(QWidget *parent = 0);
    ~ImageCropWindow();

    void InitGUI();
    void setImageProcesser(imageprocess *_imgpros);


public slots:
    void handleBtnTrain();
    void handleBtnTest();
    void handleBtnFeature();
    void handleBtnOne();
    void handleBtnAll();

private:
    bool setImageFeature(paintingInfo *imginfo, std::vector<float> &feature_set);
    void readPaintingFromXml(const QString &filename);
    void computeFeature(std::vector<int> &input, std::vector<float> &output, int blocksize);
    void createSet(const QString &filename, std::vector<std::vector<float>> &feature_set, std::vector<int> &label_set);
    void convertVectortoMatrix(std::vector<std::vector<float>> &trainSet, Mat &trainMat);

    void setSixAuthor(int a, int b, int c, int d, int e, int f);
    void setFiveAuthor(int a, int b, int c, int d, int e);
    void setFourAuthor(int a, int b, int c, int d);
    void setThreeAuthor(int a, int b, int c);
    void setTwoAuthor(int a, int b);
    void setAuthor(int a);

//    QImage image;
//    QLabel *imagelbl;
    QRect rec;
    QPushButton *m_btnTrain;
    QPushButton *m_btnTest;
    QPushButton *m_btnFeature;
    QPushButton *m_btnOne;
    QPushButton *m_btnAll;

    QPushButton *m_btnAuthor1;
    QPushButton *m_btnAuthor2;
    QPushButton *m_btnAuthor3;
    QPushButton *m_btnAuthor4;
    QPushButton *m_btnAuthor5;
    QPushButton *m_btnAuthor6;

    QVBoxLayout *hLayout_imgall;

    imageprocess *imgpros;

    bool ref_load;
    std::vector<uchar> ref_x;
    std::vector<uchar> ref_y;

    Ptr<SVM> svm;
};

#endif // IMAGECROPWINDOW_H
