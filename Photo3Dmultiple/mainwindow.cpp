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
    QImage image;
    QImage ftSpectra = imgpros->computeSpectra(image);
    QImage filter;
    QImage ftResult;

    graph_image = new Q3DSurface();
    graph_temp = new Q3DScatter();

    graph_image->scene()->setActiveCamera(graph_temp->scene()->activeCamera());

    container_image = QWidget::createWindowContainer(graph_image);

    QSize screenSize = graph_image->screen()->size();
    container_image->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height()/2));
    container_image->setMaximumSize(screenSize);
    container_image->setFocusPolicy(Qt::StrongFocus);
    container_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_btnAdd = new QPushButton("Add...");
    m_btnRemove = new QPushButton("Remove");
    m_btnImageConfig = new QPushButton("Compare");
    m_btnImageSave = new QPushButton("Save");
    m_btnDefocus = new QPushButton("Delete Focus");

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

    m_typeList = new QComboBox;
    m_typeList->addItem(QStringLiteral("LowPass"));
    m_typeList->addItem(QStringLiteral("HighPass"));
    m_typeList->addItem(QStringLiteral("BandPass"));


    //filter controller
    m_ratiosize = new QSlider(Qt::Horizontal);
    m_ratiosize->setMinimum(0);
    m_ratiosize->setMaximum(9);
    m_ratiosize->setTickInterval(1);
    m_ratiosize->setEnabled(true);

    m_blockx = new QSlider(Qt::Horizontal);
    m_blockx->setMinimum(0);
    m_blockx->setTickInterval(1);
    m_blockx->setMaximum(9);

    m_blocky = new QSlider(Qt::Horizontal);
    m_blocky->setMinimum(0);
    m_blocky->setTickInterval(1);
    m_blocky->setMaximum(9);

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
    hLayout_ctlimage->addWidget(m_btnAdd);
    hLayout_ctlimage->addWidget(m_btnRemove);
    hLayout_ctlimage->addWidget(m_btnImageSave);
    hLayout_ctlimage->addWidget(m_btnImageConfig);

    QHBoxLayout *hLayout_filterselection = new QHBoxLayout;
    hLayout_filterselection->addWidget(m_filterList);
    hLayout_filterselection->addWidget(m_typeList);

    QHBoxLayout *vLayout_btnShown = new QHBoxLayout;
    vLayout_btnShown->addWidget(m_btnOriginShown);
    vLayout_btnShown->addWidget(m_btnDefocus);

//    vLayout_btnShown->addWidget(m_btnFilterShown);
//    vLayout_btnShown->addWidget(m_btnResultShown);

    QVBoxLayout *vLayout_filtercontrol = new QVBoxLayout;
    vLayout_filtercontrol->addLayout(hLayout_ctlimage);
//    vLayout_filtercontrol->addLayout(hLayout_filterselection);
    vLayout_filtercontrol->addLayout(vLayout_btnShown);
    vLayout_filtercontrol->addWidget(m_ratiosize);
    vLayout_filtercontrol->addWidget(m_blockx);
    vLayout_filtercontrol->addWidget(m_blocky);

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
//    modifier->importImage(ftSpectra);
//    modifier->enableHeightMapModel(true);
    modifier->changeTheme(2);

    connect(m_btnAdd, SIGNAL(clicked()), this, SLOT(handleBtnAdd()));
    connect(m_btnRemove, SIGNAL(clicked()), this, SLOT(handleBtnRemove()));
    connect(m_btnImageSave, SIGNAL(clicked()), this, SLOT(handleBtnSave()));
    connect(m_btnDefocus, SIGNAL(clicked()), this, SLOT(handleBtnDelfocus()));

    connect(m_filterList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFilter(int)));
    connect(m_typeList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));

    connect(m_btnImageConfig, SIGNAL(clicked()), this, SLOT(openCropWindow()));

    connect(m_ratiosize, SIGNAL(valueChanged(int)), this, SLOT(ratioSizeChanged(int)));
    connect(m_blockx, SIGNAL(valueChanged(int)), this, SLOT(blockXChanged(int)));
    connect(m_blocky, SIGNAL(valueChanged(int)), this, SLOT(blockYChanged(int)));

    connect(m_btnOriginShown, SIGNAL(toggled(bool)), this, SLOT(originShown(bool)));
    connect(m_btnFilterShown, SIGNAL(toggled(bool)), this, SLOT(filterShown(bool)));
    connect(m_btnResultShown, SIGNAL(toggled(bool)), this, SLOT(resultShown(bool)));

    connect(originlabel, SIGNAL(clicked()), this, SLOT(openCropWindow()));

}

void MainWindow::handleBtnAdd()
{    
    imageApplyed = false;
    filename = QFileDialog::getOpenFileName(this,
        "Open Image", "/Users/wejaq/Documents/test/", "Image Files (*.png *.jpg *.bmp *.gif)");

    QImage image(filename);
    m_orignin=image;
    m_after = image;
//    //crop
//    QRect rect(image.width()/2-100, image.height()/2-100, 1500, 1500);
//    QRect rect(500,500,1300,1300);

//    image = image.copy(rect);


    QImage ftSpectra = imgpros->computeSpectra(image);
//    QImage result(image.width() + ftSpectra.width(), image.height() ,QImage::Format_RGB32);

//    QPainter painter;
//    painter.begin(&result);
//    painter.drawImage(0, 0, image);
//    painter.drawImage(image.width(), 0, ftSpectra);
//    painter.end();


//    ftSpectra.save("/Users/wejaq/Documents/test/temp/temp.jpg");
    if(!filename.isEmpty())
    {
        speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));
        originlabel->setPixmap(QPixmap::fromImage(image).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

        modifier->AddImage(ftSpectra);
    }

    //modifier->enableHeightMapModel(true);
}

void MainWindow::changeFilter(int idxfilter)
{

//    QImage filter = imgpros->getFilterById(idxfilter, m_typeList->currentIndex(),  m_kernelsize->value(), m_kernelsigma->value());
//    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

//    //modifier->importFilter(filter);
//    //modifier->enableHeightMapModel(true);

//    if(idxfilter == 2){
//        m_kernelsigma->setMaximum(10);
//        m_kernelsigma->setMinimum(1);
//        m_kernelsigma->setTickInterval(1);
//        m_kernelsigma->setEnabled(true);

//    }
//    else
//        m_kernelsigma->setEnabled(false);

//    m_kernelsigma->setValue(m_kernelsigma->minimum());

}

void MainWindow::changeType(int type)
{

//    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), type,  m_kernelsize->value(), m_kernelsigma->value());
//    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

    //modifier->importFilter(filter);
    //modifier->enableHeightMapModel(true);
}

void MainWindow::handleBtnRemove()
{
    modifier->RemoveImage();
//    imageApplyed = false;
//    QImage image(filename);
//    originlabel->setPixmap(QPixmap::fromImage(image).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

//    QImage ftSpectra = imgpros->computeSpectra(image);
//    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

//    //modifier->importImage(ftSpectra);
//    //modifier->enableHeightMapModel(true);

//    QImage filter = imgpros->getFilterById(m_filterList->currentIndex(), m_typeList->currentIndex(), m_kernelsize->value(), m_kernelsigma->value());
//    filterlabel->setPixmap(QPixmap::fromImage(filter).scaled(QSize(filterlabel->width(), filterlabel->height()),Qt::KeepAspectRatio));

//    //modifier->importFilter(filter);
//    //modifier->enableHeightMapModel(true);

//    QImage ftResult("");
//    resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));

//    //modifier->importResult(ftResult);
//    //modifier->enableHeightMapModel(true);
}

void MainWindow::handleBtnDelfocus()
{
    QImage image = m_orignin.copy();

    QImage _defocus = imgpros->DeleteFocus(image);

    m_orignin = _defocus;
    QImage ftSpectra = imgpros->computeSpectra(_defocus);
    originlabel->setPixmap(QPixmap::fromImage(_defocus).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->RemoveImage();
    modifier->AddImage(ftSpectra);
}

void MainWindow::handleBtnSave()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "/Users/wejaq/Documents/test/", ".png");
    filename+=".jpg";
    if(!m_btnOriginShown->isChecked()){
        m_crop.save(filename);
    }
    else{
        QImage result(1000, 1000 ,QImage::Format_RGB32);

        QPainter painter;
        painter.begin(&result);
        painter.fillRect(result.rect(), QBrush(Qt::black));
        painter.drawImage(500-m_crop.width()/2, 500-m_crop.height()/2, m_crop);
        painter.end();

        result.save(filename);
    }
}

void MainWindow::handleBtnApply()
{
    imageApplyed = true;

    QImage ftSpectra = imgpros->computeMergeSpectra();
    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    //modifier->importResult(ftSpectra);
    //modifier->enableHeightMapModel(true);

    QImage ftResult = imgpros->computeInverseResult();
    resultlabel->setPixmap(QPixmap::fromImage(ftResult).scaled(QSize(resultlabel->width(), resultlabel->height()),Qt::KeepAspectRatio));
}


void MainWindow::ratioSizeChanged(int value)
{
    QImage image = m_orignin.copy();
//    std::cout<<image.height()*(10-value)/10<<endl;
    int blockx = m_blockx->value();
    int blocky = m_blocky->value();
//    QRect rect(image.width()*blockx/10, image.height()*blocky/10, image.height()*(10-value)/10, image.height()*(10-value)/10);
//    QRect rect(0, 0, image.height()*2, image.height()*2);
    QRect rect(image.width()*blockx/10, image.height()*blocky/10, 1000*(10-value)/10, 1000*(10-value)/10);

    QImage croped = image.copy(rect);
    m_crop = croped;
    QImage result(image.width(), image.height() ,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&result);
    painter.fillRect(result.rect(), QBrush(Qt::black));
    painter.drawImage(rect.x(), rect.y(), croped);
    painter.end();

    if(!m_btnOriginShown->isChecked()){
        result = croped;
    }

    m_after = result;
    QImage ftSpectra = imgpros->computeSpectra(result);
    originlabel->setPixmap(QPixmap::fromImage(result).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));


    modifier->RemoveImage();
    modifier->AddImage(ftSpectra);

//    modifier->AddImage(ftSpectra);
}


void MainWindow::blockXChanged(int value)
{
    QImage image = m_orignin.copy();

    int ration = m_ratiosize->value();
    int blocky = m_blocky->value();
    QRect rect(image.width()*value/10, image.height()*blocky/10, image.height()*(10-ration)/10, image.height()*(10-ration)/10);

    QImage croped = image.copy(rect);
    m_crop = croped;
    QImage result(image.width(), image.height() ,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&result);
    painter.fillRect(result.rect(), QBrush(Qt::black));
    painter.drawImage(rect.x(), rect.y(), croped);
    painter.end();

    if(!m_btnOriginShown->isChecked()){
        result = croped;
    }

    m_after = result;
    QImage ftSpectra = imgpros->computeSpectra(result);
    originlabel->setPixmap(QPixmap::fromImage(result).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->RemoveImage();
    modifier->AddImage(ftSpectra);
}

void MainWindow::blockYChanged(int value)
{
    QImage image = m_orignin.copy();

    int ration = m_ratiosize->value();
    int blockx = m_blockx->value();
    QRect rect(image.width()*blockx/10, image.height()*value/10, image.height()*(10-ration)/10, image.height()*(10-ration)/10);

    QImage croped = image.copy(rect);
    m_crop = croped;

    QImage result(image.width(), image.height() ,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&result);
    painter.fillRect(result.rect(), QBrush(Qt::black));
    painter.drawImage(rect.x(), rect.y(), croped);
    painter.end();

    if(!m_btnOriginShown->isChecked()){
        result = croped;
    }

    m_after = result;
    QImage ftSpectra = imgpros->computeSpectra(result);
    originlabel->setPixmap(QPixmap::fromImage(result).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->RemoveImage();
    modifier->AddImage(ftSpectra);
}

void MainWindow::originShown(bool enabled)
{
    QImage image = m_orignin.copy();

    int ration = m_ratiosize->value();
    int blockx = m_blockx->value();
    int blocky = m_blocky->value();

    QRect rect(image.width()*blockx/10, image.height()*blocky/10, image.width()*(10-ration)/10, image.height()*(10-ration)/10);

    QImage croped = image.copy(rect);

    QImage result(image.width(), image.height() ,QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&result);
    painter.fillRect(result.rect(), QBrush(Qt::black));
    painter.drawImage(rect.x(), rect.y(), croped);
    painter.end();

    if(!enabled){
        result = croped;
    }

    QImage ftSpectra = imgpros->computeSpectra(result);
    originlabel->setPixmap(QPixmap::fromImage(result).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

    speclabel->setPixmap(QPixmap::fromImage(ftSpectra).scaled(QSize(speclabel->width(), speclabel->height()),Qt::KeepAspectRatio));

    modifier->RemoveImage();
    modifier->AddImage(ftSpectra);
}

void MainWindow::filterShown(bool enabled)
{
    //modifier->showFilter(enabled);
}

void MainWindow::resultShown(bool enabled)
{
    //modifier->showResult(enabled);
}

void MainWindow::openCropWindow()
{
    //ssim test
//    QString filename = "/Users/wejaq/Documents/test/vangoghhi/ref.jpg";
//    QImage ref(filename);

//    QImage image = m_orignin.copy();
//    int cropsize = 5;
//    QRect rect1(0,0, ref.height()/cropsize, ref.height()/cropsize);
//    QRect rect2(0,0, ref.height()/cropsize, ref.height()/cropsize);

//    QImage image1 = ref.copy(rect1);
//    QImage image2 = image.copy(rect2);

//    QImage ftSpectra1 = imgpros->computeSpectra(image1);
//    QImage ftSpectra2 = imgpros->computeSpectra(image2);

//    speclabel->setPixmap(QPixmap::fromImage(image1).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));
//    filterlabel->setPixmap(QPixmap::fromImage(image2).scaled(QSize(originlabel->width(), originlabel->height()),Qt::KeepAspectRatio));

////    modifier->AddImage(ftSpectra1);
//    modifier->AddImage(ftSpectra2);

//    double ssim = imgpros->ssim(ftSpectra1, ftSpectra2, 3);
//    resultlabel->setText(QString::number(ssim));


    imgcropw = new ImageCropWindow;
    imgcropw->setImageProcesser(imgpros);
    imgcropw->show();
}

void MainWindow::saveTransformFile()
{
    imgcropw = new ImageCropWindow;
//    imgcropw->setImage(m_orignin);
    imgcropw->show();
}
