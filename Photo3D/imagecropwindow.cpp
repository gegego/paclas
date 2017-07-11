#include "imagecropwindow.h"
#include <QtWidgets>


ImageCropWindow::ImageCropWindow(QWidget *parent)
    : QWidget(parent)
{
    InitGUI();
}

ImageCropWindow::~ImageCropWindow()
{
}


void ImageCropWindow::InitGUI()
{
    imagelbl = new QLabel;
    imagelbl->setPixmap(QPixmap::fromImage(image));

    QRect rec = QApplication::desktop()->screenGeometry();

//    imagelbl->setMaximumSize(QSize(rec.width(), rec.height()));
//    imagelbl->setMinimumSize(QSize(rec.width(), rec.height()));
//    imagelbl->setFrameShape(QFrame::Box);
//    imagelbl->

    QHBoxLayout *hLayout_main = new QHBoxLayout;
    hLayout_main->addWidget(imagelbl);
    setLayout(hLayout_main);
}

void ImageCropWindow::setImage(QImage img)
{
    image = img;
    imagelbl->setPixmap(QPixmap::fromImage(image)/*.scaled(QSize(imagelbl->width(), imagelbl->height()),Qt::KeepAspectRatio)*/);
}
