#include "imagecropwindow.h"
#include <QtWidgets>
#include <QDomDocument>
#include <numeric>

using namespace std;

const int imageSize(10);
int i=0;
int descriptor_size = 10;
const int crop_size = 300;
const int patchnum = 10;

ImageCropWindow::ImageCropWindow(QWidget *parent)
    : QWidget(parent)
{
    InitGUI();
    svm = SVM::create();
    svm->setGamma(0.50625);
    svm->setC(12.5);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);
}

ImageCropWindow::~ImageCropWindow()
{
}


void ImageCropWindow::InitGUI()
{
    ref_load = false;
    rec = QApplication::desktop()->screenGeometry();

    m_btnTrain = new QPushButton(tr("&Train..."));
    m_btnTest = new QPushButton(tr("&Test..."));
    m_btnFeature = new QPushButton(tr("&Run..."));
    m_btnOne = new QPushButton(tr("&Feature..."));
    m_btnAll = new QPushButton(tr("&All..."));

    m_btnAuthor1 = new QPushButton(tr("&Van Gogh"));
    m_btnAuthor1->setCheckable(true);
    m_btnAuthor1->setChecked(true);

    m_btnAuthor2 = new QPushButton(tr("&Monet"));
    m_btnAuthor2->setCheckable(true);
    m_btnAuthor2->setChecked(true);

    m_btnAuthor3 = new QPushButton(tr("&Toulouse-Lautrec"));
    m_btnAuthor3->setCheckable(true);
    m_btnAuthor3->setChecked(true);

    m_btnAuthor4 = new QPushButton(tr("&Gauguin"));
    m_btnAuthor4->setCheckable(true);
    m_btnAuthor4->setChecked(true);

    m_btnAuthor5 = new QPushButton(tr("&Rembrandt"));
    m_btnAuthor5->setCheckable(true);
    m_btnAuthor5->setChecked(true);

    m_btnAuthor6 = new QPushButton(tr("&Rubens"));
    m_btnAuthor6->setCheckable(true);
    m_btnAuthor6->setChecked(true);

    QScrollArea *scrollArea = new QScrollArea;
//    scrollArea->setMaximumSize(QSize(rec.width(), rec.height()*2/3));
//    scrollArea->setMinimumSize(QSize(rec.width(), rec.height()*2/3));
    scrollArea->setWidgetResizable(true);
    QFrame *inner = new QFrame(scrollArea);


    QVBoxLayout *hLayout_main = new QVBoxLayout;
    hLayout_imgall = new QVBoxLayout;
    QHBoxLayout *hLayout_btn = new QHBoxLayout;
    hLayout_btn->addWidget(m_btnTrain);
    hLayout_btn->addWidget(m_btnTest);
    hLayout_btn->addWidget(m_btnFeature);
    hLayout_btn->addWidget(m_btnOne);
    hLayout_btn->addWidget(m_btnAll);

    QHBoxLayout *hLayout_author = new QHBoxLayout;
    hLayout_author->addWidget(m_btnAuthor1);
    hLayout_author->addWidget(m_btnAuthor2);
    hLayout_author->addWidget(m_btnAuthor3);
    hLayout_author->addWidget(m_btnAuthor4);
    hLayout_author->addWidget(m_btnAuthor5);
    hLayout_author->addWidget(m_btnAuthor6);

    hLayout_main->addLayout(hLayout_btn);
    hLayout_main->addLayout(hLayout_author);
    inner->setLayout(hLayout_imgall);
    scrollArea->setWidget(inner);

    hLayout_main->addWidget(scrollArea);
    setLayout(hLayout_main);

//    QHBoxLayout *hLayout_img = new QHBoxLayout;

//    QLabel *imagelbl = new QLabel;
//    imagelbl->setMinimumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
//    imagelbl->setMaximumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
//    imagelbl->setFrameShape(QFrame::Box);

//    QLabel *cutedlbl = new QLabel;
////    cutedlbl->setMinimumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
////    cutedlbl->setMaximumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
//    cutedlbl->setFrameShape(QFrame::Box);
//    QLabel *infolbl = new QLabel;
////    cutedlbl->setMinimumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
////    cutedlbl->setMaximumSize(QSize(rec.width() / imageSize, rec.height() / imageSize));
//    infolbl->setFrameShape(QFrame::Box);

//    QLabel *spectlbl = new QLabel;
//    spectlbl->setFrameShape(QFrame::Box);

    connect(m_btnTrain, SIGNAL(clicked()), this, SLOT(handleBtnTrain()));
    connect(m_btnTest, SIGNAL(clicked()), this, SLOT(handleBtnTest()));
    connect(m_btnFeature, SIGNAL(clicked()), this, SLOT(handleBtnFeature()));
    connect(m_btnOne, SIGNAL(clicked()), this, SLOT(handleBtnOne()));
    connect(m_btnAll, SIGNAL(clicked()), this, SLOT(handleBtnAll()));

}

void ImageCropWindow::handleBtnAll()
{
    //2 painter
    cout<<"2 painters"<<endl;
    for(int i=0; i<6; i++)
    {
        for(int j = i+1; j<6; j++)
        {
            cout<<"["<<i<<","<<j<<"]";
            setTwoAuthor(i,j);
            handleBtnFeature();
            cout<<endl;
        }
    }

    //3 painter
    cout<<"3 painters"<<endl;
    for(int i=0; i<6; i++)
    {
        for(int j = i+1; j<6; j++)
        {
            for(int h = j+1; h<6; h++)
            {
                cout<<"["<<i<<","<<j<<","<<h<<"]";
                setThreeAuthor(i,j,h);
                handleBtnFeature();
                cout<<endl;
            }
        }
    }

    //4 painter
    cout<<"4 painters"<<endl;
    for(int i=0; i<6; i++)
    {
        for(int j = i+1; j<6; j++)
        {
            for(int h = j+1; h<6; h++)
            {
                for(int k = h+1; k<6; k++)
                {
                    cout<<"["<<i<<","<<j<<","<<h<<","<<k<<"]";
                    setFourAuthor(i,j,h,k);
                    handleBtnFeature();
                    cout<<endl;
                }
            }
        }
    }

    //5 painter
    cout<<"5 painters"<<endl;
    for(int i=0; i<6; i++)
    {
        for(int j = i+1; j<6; j++)
        {
            for(int h = j+1; h<6; h++)
            {
                for(int k = h+1; k<6; k++)
                {
                    for(int m = k+1; m<6; m++)
                    {
                        cout<<"["<<i<<","<<j<<","<<h<<","<<k<<","<<m<<"]";
                        setFiveAuthor(i,j,h,k,m);
                        handleBtnFeature();
                        cout<<endl;
                    }
                }
            }
        }
    }
    //6 painter
    cout<<"6 painters"<<endl;
    setSixAuthor(0,1,2,3,4,5);
    handleBtnFeature();
    cout<<endl;
}

void ImageCropWindow::handleBtnOne()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        "Open", "/Users/wejaq/Documents/test/", "Image Files (*.png *.jpg *.bmp *.gif)");

    foreach (const QString &imagefilename, fileNames) {
        std::vector<std::vector<float> > trainDiff;
        std::vector<int> trainLabels;
        createSet(imagefilename, trainDiff, trainLabels);

        Mat trainMat(trainDiff.size(), descriptor_size, CV_32FC1);
        convertVectortoMatrix(trainDiff, trainMat);
        std::cout<<imagefilename.toStdString()<<std::endl;
        std::cout<<trainMat<<std::endl;
    }
}

void ImageCropWindow::handleBtnFeature()
{
//    QString xmlfilename = QFileDialog::getOpenFileName(this,
//        "Open", "/Users/wejaq/Documents/test/", "Xml Files (*.xml)");

//    std::vector<std::vector<float> > trainDiff;
//    std::vector<int> trainLabels;
//    createSet(xmlfilename, trainDiff, trainLabels);

//    Mat trainMat(trainDiff.size(), descriptor_size, CV_32FC1);
//    convertVectortoMatrix(trainDiff, trainMat);
//    std::cout<<trainMat<<std::endl;

    //train
    QString trainfile = QString("/Users/wejaq/Documents/test/dataset/test/training_set.xml");

    std::vector<std::vector<float> > trainDiff;
    std::vector<int> trainLabels;
    createSet(trainfile, trainDiff, trainLabels);

    Mat trainMat(trainDiff.size(), descriptor_size, CV_32FC1);
    convertVectortoMatrix(trainDiff, trainMat);

    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);

    Ptr<SVM> svmClass;
    svmClass = SVM::create();
    svmClass->setGamma(0.50625);
    svmClass->setC(12.5);
    svmClass->setKernel(SVM::RBF);
    svmClass->setType(SVM::C_SVC);
    svmClass->train(td);

    //test
    QString testfile = QString("/Users/wejaq/Documents/test/dataset/test/test_set.xml");

    std::vector<std::vector<float> > testDiff;
    std::vector<int> testLabels;
    createSet(testfile, testDiff, testLabels);

    Mat testMat(testDiff.size(), descriptor_size, CV_32FC1);
    convertVectortoMatrix(testDiff, testMat);
    //std::cout<<testMat<<std::endl;

    Mat testResponse;
    svmClass->predict(testMat, testResponse);
    //std::cout<<testResponse<<std::endl;

    float count = 0;
    float accuracy = 0 ;
    for(int i=0;i<testResponse.rows;i++)
    {
        //cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if(testResponse.at<float>(i,0) == testLabels[i]){
            count = count + 1;
        }
    }
    accuracy = (count/testResponse.rows)*100;
    std::cout<<accuracy<<std::endl;

    QHBoxLayout *hLayout_img = new QHBoxLayout;
    QLabel *infolbl = new QLabel;
    infolbl->setFrameShape(QFrame::Box);
    infolbl->setText(QString::number(accuracy));
    hLayout_img->addWidget(infolbl);
    hLayout_imgall->addLayout(hLayout_img);
}

void ImageCropWindow::handleBtnTrain()
{
//    QString xmlfilename = QFileDialog::getOpenFileName(this,
//        "Open", "/Users/wejaq/Documents/test/", "Xml Files (*.xml)");

    QString xmlfilename = QString("/Users/wejaq/Documents/test/dataset/test/training_set.xml");
    if(xmlfilename.isEmpty())
        return;

    std::vector<std::vector<float> > trainDiff;
    std::vector<int> trainLabels;
    createSet(xmlfilename, trainDiff, trainLabels);

    Mat trainMat(trainDiff.size(), descriptor_size, CV_32FC1);
    convertVectortoMatrix(trainDiff, trainMat);
    std::cout<<trainMat<<std::endl;
//    for (auto i = trainLabels.begin(); i != trainLabels.end(); ++i)
//        std::cout << *i << ',';

    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    svm->train(td);

    svm->save("/Users/wejaq/Documents/test/vango.yml");
}

void ImageCropWindow::handleBtnTest()
{
//    QString xmlfilename = QFileDialog::getOpenFileName(this,
//        "Open", "/Users/wejaq/Documents/test/", "Xml Files (*.xml)");
    QString xmlfilename = QString("/Users/wejaq/Documents/test/dataset/test/test_set.xml");

    std::vector<std::vector<float> > testDiff;
    std::vector<int> testLabels;
    createSet(xmlfilename, testDiff, testLabels);

    Mat testMat(testDiff.size(), descriptor_size, CV_32FC1);
    convertVectortoMatrix(testDiff, testMat);
    std::cout<<testMat<<std::endl;

    Mat testResponse;
    svm->predict(testMat, testResponse);
    std::cout<<testResponse<<std::endl;

    float count = 0;
    float accuracy = 0 ;
    for(int i=0;i<testResponse.rows;i++)
    {
        //cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if(testResponse.at<float>(i,0) == testLabels[i]){
            count = count + 1;
        }
    }
    accuracy = (count/testResponse.rows)*100;
    std::cout<<accuracy<<std::endl;
}

void ImageCropWindow::setSixAuthor(int a, int b, int c, int d, int e, int f)
{
    m_btnAuthor1->setChecked(false);
    m_btnAuthor2->setChecked(false);
    m_btnAuthor3->setChecked(false);
    m_btnAuthor4->setChecked(false);
    m_btnAuthor5->setChecked(false);
    m_btnAuthor6->setChecked(false);

    setAuthor(a);
    setAuthor(b);
    setAuthor(c);
    setAuthor(d);
    setAuthor(e);
    setAuthor(f);

}

void ImageCropWindow::setFiveAuthor(int a, int b, int c, int d, int e)
{
    m_btnAuthor1->setChecked(false);
    m_btnAuthor2->setChecked(false);
    m_btnAuthor3->setChecked(false);
    m_btnAuthor4->setChecked(false);
    m_btnAuthor5->setChecked(false);
    m_btnAuthor6->setChecked(false);

    setAuthor(a);
    setAuthor(b);
    setAuthor(c);
    setAuthor(d);
    setAuthor(e);

}

void ImageCropWindow::setFourAuthor(int a, int b, int c, int d)
{
    m_btnAuthor1->setChecked(false);
    m_btnAuthor2->setChecked(false);
    m_btnAuthor3->setChecked(false);
    m_btnAuthor4->setChecked(false);
    m_btnAuthor5->setChecked(false);
    m_btnAuthor6->setChecked(false);

    setAuthor(a);
    setAuthor(b);
    setAuthor(c);
    setAuthor(d);

}

void ImageCropWindow::setThreeAuthor(int a, int b, int c)
{
    m_btnAuthor1->setChecked(false);
    m_btnAuthor2->setChecked(false);
    m_btnAuthor3->setChecked(false);
    m_btnAuthor4->setChecked(false);
    m_btnAuthor5->setChecked(false);
    m_btnAuthor6->setChecked(false);

    setAuthor(a);
    setAuthor(b);
    setAuthor(c);
}

void ImageCropWindow::setTwoAuthor(int a, int b)
{
    m_btnAuthor1->setChecked(false);
    m_btnAuthor2->setChecked(false);
    m_btnAuthor3->setChecked(false);
    m_btnAuthor4->setChecked(false);
    m_btnAuthor5->setChecked(false);
    m_btnAuthor6->setChecked(false);

    setAuthor(a);
    setAuthor(b);
}

void ImageCropWindow::setAuthor(int a)
{
    switch (a) {
    case 0:
        m_btnAuthor1->setChecked(true);
        break;
    case 1:
        m_btnAuthor2->setChecked(true);
        break;
    case 2:
        m_btnAuthor3->setChecked(true);
        break;
    case 3:
        m_btnAuthor4->setChecked(true);
        break;
    case 4:
        m_btnAuthor5->setChecked(true);
        break;
    case 5:
        m_btnAuthor6->setChecked(true);
        break;
    default:
        break;
    }
}

bool ImageCropWindow::setImageFeature(paintingInfo *imginfo, std::vector<float> &feature_set)
{
    QImage img(imginfo->getPath());

    QImage preprocess = imgpros->EqHist(img);
    preprocess.save("/Users/wejaq/Documents/test/temp/temp.jpg");

//    double pixel = (img.width()*img.height())/(imginfo->getWidth()*imginfo->getHeight());
//    if(pixel < 3000)
//        return false;

//    double cropsize = img.height()*(crop_size/imginfo->getHeight());
    double cropsize = img.height()*(crop_size/imginfo->getHeight());

    int rows = img.height()/crop_size;
    int cols = img.width()/crop_size;
    string diffstr = "DiffMatrix:\n";
    std::vector<int> diffvect;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QRect rect(j*(crop_size),i*(crop_size), crop_size, crop_size);
            QImage cuted = preprocess.copy(rect);
            //replace
//            QImage filtered = imgpros->FilterLowPass(cuted, 8);

//            int diff = imgpros->computeBrushDifference(filtered);
            int diff = imgpros->computeSpecSize(cuted, 10);
//            QImage spec = imgpros->computeSpectra(cuted);

//            cuted.save("/Users/wejaq/Documents/test/temp/cuted/" + QString::number(i) +"with"+ QString::number(j)+".jpg");
//            spec.save("/Users/wejaq/Documents/test/temp/spec/" + QString::number(i) +"with"+ QString::number(j)+".jpg");

            diffvect.push_back(diff);
            diffstr += std::to_string(diff) + ",";
        }
        diffstr += "\n";
    }

    //std::cout<<diffstr<<std::endl;
    computeFeature(diffvect, feature_set, descriptor_size);
    return true;
}

void ImageCropWindow::setImageProcesser(imageprocess *_imgpros)
{
    imgpros = _imgpros;
}

void ImageCropWindow::computeFeature(std::vector<int> &input, std::vector<float> &output, int blocksize)
{
    int num = 10/blocksize;
    std::sort (input.begin(), input.end());
//    int start = 1;
//    int end = num;
//    output.push_back((float)input[0]);
    int value = num;
    for(int i = 0; i < blocksize; i++)
    {
        output.push_back(0);
    }
    int j = 0;
    for(int i = 0; i < input.size(); i++)
    {
        //cout<<input[i]<<",";
        if(input[i] <= value || input[i] > 10)
        {
            output[j]++;
        }
        else
        {
            j++;
            output[j]++;
            value = num*(j+1);
        }
    }

    for(int i = 0; i < blocksize; i++)
    {
        output[i] = output[i]/input.size();
    }
}

void ImageCropWindow::readPaintingFromXml(const QString &filename) {
    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNodeList paintings = doc.elementsByTagName("Painting");
    for (int i = 0; i < paintings.size(); i++) {
        QDomNode n = paintings.item(i);
        QDomElement filename = n.firstChildElement("filename");
        QDomElement width = n.firstChildElement("width");
        QDomElement height = n.firstChildElement("height");
        if (filename.isNull() || width.isNull() || height.isNull())
            continue;

        QString f = filename.text();
        double w = width.text().toDouble();
        double h = height.text().toDouble();

        paintingInfo *paint = new paintingInfo(f, h, w);
//        QImage image(f);
//        QRect rect(0,0,image.width()*(6/w),image.height()*(6/h));
        //            QRect rect(0,0,1000,1000);

//        image = image.copy(rect);
//        setImage(paint);
    }
}

void ImageCropWindow::convertVectortoMatrix(std::vector<std::vector<float>> &trainSet, Mat &trainMat)
{
    for(int i = 0;i<trainSet.size();i++){
        for(int j = 0;j<descriptor_size ;j++){
           trainMat.at<float>(i,j) = trainSet[i][j];
        }
    }
}

void ImageCropWindow::createSet(const QString &filename, std::vector<std::vector<float>> &feature_set, std::vector<int> &label_set)
{
    if(!filename.endsWith(QString(".xml")))
    {
        double w = 0;
        double h = 0;
        paintingInfo *paint = new paintingInfo(filename, h, w);
        std::vector<float> feature;
        bool isVal = setImageFeature(paint, feature);
        if(!isVal)
            return;

        feature_set.push_back(feature);
//        label_set.push_back(a);
    }
    else{
        QDomDocument doc;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
            return;

        QDomNodeList paintings = doc.elementsByTagName("Painting");
        for (int i = 0; i < paintings.size(); i++) {
            QDomNode n = paintings.item(i);
            QDomElement filename = n.firstChildElement("filename");
            QDomElement width = n.firstChildElement("width");
            QDomElement height = n.firstChildElement("height");
            QDomElement author = n.firstChildElement("author");

            if (filename.isNull() || width.isNull() || height.isNull())
                continue;

            QString f = filename.text();
            double w = width.text().toDouble();
            double h = height.text().toDouble();
            int a = author.text().toInt();

            if(!m_btnAuthor1->isChecked() && a==0)
                continue;

            if(!m_btnAuthor2->isChecked() && a==1)
                continue;

            if(!m_btnAuthor3->isChecked() && a==2)
                continue;

            if(!m_btnAuthor4->isChecked() && a==3)
                continue;

            if(!m_btnAuthor5->isChecked() && a==4)
                continue;

            if(!m_btnAuthor6->isChecked() && a==5)
                continue;

            paintingInfo *paint = new paintingInfo(f, h, w);
            std::vector<float> feature;
            bool isVal = setImageFeature(paint, feature);
            if(!isVal)
                continue;

            feature_set.push_back(feature);
            label_set.push_back(a);
        }
    }
}
