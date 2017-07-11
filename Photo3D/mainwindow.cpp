#include <QtWidgets>
#include "mainwindow.h"

const int imageSize(6);

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    imgpros = new imageprocess();
    imageApplyed =false;
    InitGUI();
}

MainWindow::~MainWindow()
{
    delete imgpros;
}

void MainWindow::InitGUI()
{
    QImage image("");
    QImage ftSpectra = imgpros->computeSpectra(image);
    QImage filter("");
    QImage ftResult("");

    graph_image = new Q3DSurface();
    graph_temp = new Q3DScatter();

    graph_image->scene()->setActiveCamera(graph_temp->scene()->activeCamera());

    container_image = QWidget::createWindowContainer(graph_image);

    QSize screenSize = graph_image->screen()->size();
    container_image->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height()/2));
    container_image->setMaximumSize(screenSize);
    container_image->setFocusPolicy(Qt::StrongFocus);
    container_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_btnOpen = new QPushButton("Open...");
    m_btnReload = new QPushButton("Reload");
    m_btnApply = new QPushButton("Apply");

    originlabel = new QLabel;
    originlabel->setPixmap(QPixmap::fromImage(image));

    originlabel->setMinimumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    originlabel->setMaximumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    originlabel->setFrameShape(QFrame::Box);

    speclabel = new QLabel;
    speclabel->setPixmap(QPixmap::fromImage(ftSpectra));
    speclabel->setMinimumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    speclabel->setMaximumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    speclabel->setFrameShape(QFrame::Box);

    filterlabel = new QLabel;
    filterlabel->setPixmap(QPixmap::fromImage(filter));
    filterlabel->setMinimumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    filterlabel->setMaximumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    filterlabel->setFrameShape(QFrame::Box);

    resultlabel = new QLabel;
    resultlabel->setPixmap(QPixmap::fromImage(ftResult));
    resultlabel->setMinimumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    resultlabel->setMaximumSize(QSize(screenSize.width() / imageSize, screenSize.height() / imageSize));
    resultlabel->setFrameShape(QFrame::Box);

    m_filterList = new QComboBox;
    m_filterList->addItem(QStringLiteral("Ideal Filter"));
    m_filterList->addItem(QStringLiteral("Gaussian Filter"));
    m_filterList->addItem(QStringLiteral("Butterworth Filter"));
    m_filterList->addItem(QStringLiteral("Threshold Filter"));

    m_typeList = new QComboBox;
    m_typeList->addItem(QStringLiteral("LowPass"));
    m_typeList->addItem(QStringLiteral("HighPass"));
    m_typeList->addItem(QStringLiteral("BandPass"));


    //filter controller
    m_kernelsize = new QSlider(Qt::Horizontal);
    m_kernelsize->setMinimum(30);
    m_kernelsize->setTickInterval(1);
    m_kernelsize->setEnabled(true);
    m_kernelsigma = new QSlider(Qt::Horizontal);
    m_kernelsigma->setMinimum(1);
    m_kernelsigma->setTickInterval(1);
    m_kernelsigma->setMaximum(100);
    m_kernelsigma->setEnabled(false);


    m_btnOriginShown = new QPushButton(tr("&Origin"));
    m_btnOriginShown->setCheckable(true);
    m_btnOriginShown->setChecked(true);
    m_btnFilterShown = new QPushButton(tr("&Filter"));
    m_btnFilterShown->setCheckable(true);
    m_btnFilterShown->setChecked(true);
    m_btnResultShown = new QPushButton(tr("&Result"));
    m_btnResultShown->setCheckable(true);
    m_btnResultShown->setChecked(true);

    QVBoxLayout *hLayout_image = new QVBoxLayout;
    QVBoxLayout *vLayout_image = new QVBoxLayout;
    hLayout_image->addWidget(originlabel);
    hLayout_image->addWidget(speclabel);

    QHBoxLayout *hLayout_ctlimage = new QHBoxLayout;
    hLayout_ctlimage->addWidget(m_btnOpen);
    hLayout_ctlimage->addWidget(m_btnReload);
    hLayout_ctlimage->addWidget(m_btnApply);

    QHBoxLayout *hLayout_filterselection = new QHBoxLayout;
    hLayout_filterselection->addWidget(m_filterList);
    hLayout_filterselection->addWidget(m_typeList);

    QHBoxLayout *vLayout_btnShown = new QHBoxLayout;
    vLayout_btnShown->addWidget(m_btnOriginShown);
    vLayout_btnShown->addWidget(m_btnFilterShown);
    vLayout_btnShown->addWidget(m_btnResultShown);

    QVBoxLayout *vLayout_filtercontrol = new QVBoxLayout;
    vLayout_filtercontrol->addLayout(hLayout_ctlimage);
    vLayout_filtercontrol->addLayout(hLayout_filterselection);
    vLayout_filtercontrol->addLayout(vLayout_btnShown);
    vLayout_filtercontrol->addWidget(m_kernelsize);
    vLayout_filtercontrol->addWidget(m_kernelsigma);

    QGroupBox *filtercontrolgroupBox = new QGroupBox(tr(""));
    filtercontrolgroupBox->setLayout(vLayout_filtercontrol);

//    vLayout_image->addLayout(hLayout_ctlimage);
    vLayout_image->addWidget(filtercontrolgroupBox);
    vLayout_image->addWidget(container_image);

    QVBoxLayout *hLayout_filter = new QVBoxLayout;
    QVBoxLayout *vLayout_filter = new QVBoxLayout;
    hLayout_filter->addWidget(filterlabel);
    hLayout_filter->addWidget(resultlabel);

//    vLayout_image->addLayout(hLayout_filter);

    QHBoxLayout *hLayout_control = new QHBoxLayout;
//    hLayout_control->addWidget(m_filterList);
//    hLayout_control->addWidget(m_btnApply);

//    vLayout_filter->addLayout(hLayout_control);
    vLayout_filter->addLayout(hLayout_image);
    vLayout_filter->addLayout(hLayout_filter);
//    vLayout_filter->addWidget(container_filter);

    QHBoxLayout *hLayout_main = new QHBoxLayout;
    hLayout_main->addLayout(vLayout_image);
    hLayout_main->addLayout(vLayout_filter);
    setLayout(hLayout_main);
    setWindowTitle(QStringLiteral("Photo3D (Fourier Transforms)"));

    modifier = new SurfaceGraph(graph_image);
    modifier->importImage(ftSpectra);
    modifier->enableHeightMapModel(true);
    modifier->changeTheme(2);

    connect(m_btnOpen, SIGNAL(clicked()), this, SLOT(handleBtnOpen()));
    connect(m_btnReload, SIGNAL(clicked()), this, SLOT(handleBtnReload()));
    connect(m_filterList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFilter(int)));
    connect(m_typeList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));

    connect(m_btnApply, SIGNAL(clicked()), this, SLOT(handleBtnApply()));
    connect(m_kernelsize, SIGNAL(valueChanged(int)), this, SLOT(kenerlsizeChanged(int)));
    connect(m_kernelsigma, SIGNAL(valueChanged(int)), this, SLOT(kenerlsigmaChanged(int)));
    connect(m_btnOriginShown, SIGNAL(toggled(bool)), this, SLOT(originShown(bool)));
    connect(m_btnFilterShown, SIGNAL(toggled(bool)), this, SLOT(filterShown(bool)));
    connect(m_btnResultShown, SIGNAL(toggled(bool)), this, SLOT(resultShown(bool)));

//    connect(originlabel, SIGNAL(clicked()), this, SLOT(openCropWindow()));

}

void MainWindow::handleBtnOpen()
{    
    imageApplyed = false;
    filename = QFileDialog::getOpenFileName(this,
        "Open Image", "/Users/wejaq/Documents/test/", "Image Files (*.png *.jpg *.bmp *.gif)");

    QImage image(filename);

//    //crop
//    QRect rect(10, 20, 30, 40);
//    image = image.copy(rect);
    m_orignin=image;
    originlabel->setPixmap(QPixmap::fromImage(image).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));


    QImage ftSpectra = imgpros->computeSpectra(image);
    ftSpectra.save("/Users/wejaq/Documents/test/temp/temp.jpg");
    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->importImage(ftSpectra);
    //modifier->enableHeightMapModel(true);

    if(m_filterList->currentIndex() == 3)
    {
        m_kernelsize->setMaximum(10);
        m_kernelsize->setMinimum(1);
        m_kernelsize->setTickInterval(1);
//        m_kernelsize->setValue(1);
    }
    else
    {
        int kernelMaxsize = imgpros->getfilterMaxSize();
        m_kernelsize->setMaximum(kernelMaxsize);
        m_kernelsize->setMinimum(kernelMaxsize/20);
        m_kernelsize->setTickInterval(kernelMaxsize/20);
        m_kernelsize->setValue(m_kernelsize->minimum());
    }
    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), m_typeList->currentIndex(), m_kernelsize->value(), this->m_kernelsigma->value());
    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

    modifier->importFilter(filter);
    modifier->enableHeightMapModel(true);

    QImage ftResult("");
    resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));

    modifier->importResult(ftResult);
    modifier->enableHeightMapModel(true);
}

void MainWindow::changeFilter(int idxfilter)
{

    QImage filter = imgpros->getFilterById(idxfilter, m_typeList->currentIndex(),  m_kernelsize->value(), m_kernelsigma->value());
    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

    modifier->importFilter(filter);
    modifier->enableHeightMapModel(true);

    if(idxfilter == 2){
        m_kernelsigma->setMaximum(10);
        m_kernelsigma->setMinimum(1);
        m_kernelsigma->setTickInterval(1);
        m_kernelsigma->setEnabled(true);

    }
    else if(idxfilter == 3){
//        QImage ftSpectra = imgpros->computeSpectra(m_orignin, true, m_kernelsize->value());
//        speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));
//        modifier->importResult(ftSpectra);
//        modifier->enableHeightMapModel(true);

        m_kernelsize->setMaximum(10);
        m_kernelsize->setMinimum(1);
        m_kernelsize->setTickInterval(1);
//        m_kernelsize->setValue(1);
    }
    else
        m_kernelsigma->setEnabled(false);

    m_kernelsigma->setValue(m_kernelsigma->minimum());

}

void MainWindow::changeType(int type)
{

    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), type,  m_kernelsize->value(), m_kernelsigma->value());
    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

    modifier->importFilter(filter);
    modifier->enableHeightMapModel(true);
}

void MainWindow::handleBtnReload()
{
    int level = imgpros->computeSpecSize(m_orignin, 10);
//    m_result.save("/Users/wejaq/Documents/test/result/temp.jpg");
//    imageApplyed = false;
//    QImage image(filename);
//    originlabel->setPixmap(QPixmap::fromImage(image).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

//    QImage ftSpectra = imgpros->computeSpectra(image);
//    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

//    modifier->importImage(ftSpectra);
//    modifier->enableHeightMapModel(true);

//    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), m_typeList->currentIndex(), m_kernelsize->value(), m_kernelsigma->value());
//    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

//    modifier->importFilter(filter);
//    modifier->enableHeightMapModel(true);

//    QImage ftResult("");
//    resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));

//    modifier->importResult(ftResult);
//    modifier->enableHeightMapModel(true);
}

void MainWindow::handleBtnApply()
{
    imageApplyed = true;

    QImage ftSpectra = imgpros->computeMergeSpectra();
    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->importResult(ftSpectra);
    modifier->enableHeightMapModel(true);

    QImage ftResult = imgpros->computeInverseResult();
    m_result = ftResult.copy();
    resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));
    int count = imgpros->computeBrushDifference(ftResult);
    resultlabel->setText(QString::number(count) + ":"+QString::number(m_kernelsize->value()));
}


void MainWindow::kenerlsizeChanged(int value)
{
    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), m_typeList->currentIndex(), value, this->m_kernelsigma->value());
    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));


    modifier->importFilter(filter);
    modifier->enableHeightMapModel(true);

//    if(m_filterList->currentIndex() == 3){
//        QImage ftSpectra = imgpros->computeSpectra(m_orignin, true, value);
//        speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));
//        modifier->importResult(ftSpectra);
//        modifier->enableHeightMapModel(true);
//    }

    if(imageApplyed){
        QImage ftSpectra = imgpros->computeMergeSpectra();
        speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

        modifier->importResult(ftSpectra);
        modifier->enableHeightMapModel(true);

        QImage ftResult = imgpros->computeInverseResult();
        m_result = ftResult.copy();
        resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));
    }
}


void MainWindow::kenerlsigmaChanged(int value)
{
    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), m_typeList->currentIndex(), this->m_kernelsize->value(), value);
    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

    modifier->importFilter(filter);
    modifier->enableHeightMapModel(true);

    if(imageApplyed){
        QImage ftSpectra = imgpros->computeMergeSpectra();
        speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

        modifier->importResult(ftSpectra);
        modifier->enableHeightMapModel(true);

        QImage ftResult = imgpros->computeInverseResult();
        resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));
    }
}

void MainWindow::originShown(bool enabled)
{
    modifier->showImage(enabled);
}

void MainWindow::filterShown(bool enabled)
{
    modifier->showFilter(enabled);
}

void MainWindow::resultShown(bool enabled)
{
    modifier->showResult(enabled);
}

void MainWindow::openCropWindow()
{
    imgcropw = new ImageCropWindow;
    imgcropw->setImage(m_orignin);
    imgcropw->show();
}

void MainWindow::saveTransformFile()
{
    imgcropw = new ImageCropWindow;
    imgcropw->setImage(m_orignin);
    imgcropw->show();
}
