#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QWidget>
#include "imagecropwindow.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class QTextEdit;
class QComboBox;
QT_END_NAMESPACE

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/q3dscatter.h>

#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

#include "surfacegraph.h"
#include "imageprocess.h"

using namespace QtDataVisualization;


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
//    MainWindow();
    ~MainWindow();

    void InitGUI();

public slots:
    void handleBtnAdd();
    void handleBtnRemove();
    void handleBtnSave();
    void handleBtnDelfocus();

    void handleBtnApply();
    void changeFilter(int idxfilter);
    void changeType(int type);
    void ratioSizeChanged(int value);
    void blockXChanged(int value);
    void blockYChanged(int value);

    void originShown(bool enabled);
    void filterShown(bool enabled);
    void resultShown(bool enabled);

    void openCropWindow();
    void saveTransformFile();

private:
    QString filename;

    Q3DSurface *graph_image;
    Q3DScatter *graph_temp;

    QWidget *container_image;

    QPushButton *m_btnAdd;
    QPushButton *m_btnRemove;
    QPushButton *m_btnImageConfig;
    QPushButton *m_btnImageSave;
    QPushButton *m_btnDefocus;


    QLabel *originlabel;
    QLabel *speclabel;
    QLabel *filterlabel;
    QLabel *resultlabel;
    QComboBox *m_filterList;
    QComboBox *m_typeList;

    QSlider *m_ratiosize;
    QSlider *m_blockx;
    QSlider *m_blocky;

    QPushButton *m_btnOriginShown;
    QPushButton *m_btnFilterShown;
    QPushButton *m_btnResultShown;


    SurfaceGraph *modifier;

    imageprocess *imgpros;
    bool imageApplyed;

    ImageCropWindow *imgcropw;
    QImage m_orignin;
    QImage m_after;
    QImage m_crop;
};

#endif // MAINWINDOW_H
