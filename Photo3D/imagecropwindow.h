#ifndef IMAGECROPWINDOW_H
#define IMAGECROPWINDOW_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE


class ImageCropWindow : public QWidget
{
public:
    ImageCropWindow(QWidget *parent = 0);
    ~ImageCropWindow();

    void InitGUI();

    void setImage(QImage img);

private:
    QImage image;
    QLabel *imagelbl;

};

#endif // IMAGECROPWINDOW_H
