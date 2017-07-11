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
    void handleBtnOpen();
    void handleBtnReload();
    void handleBtnApply();
    void changeFilter(int idxfilter);
    void changeType(int type);
    void kenerlsizeChanged(int value);
    void kenerlsigmaChanged(int value);
    void originShown(bool enabled);
    void filterShown(bool enabled);
    void resultShown(bool enabled);

    void openCropWindow();
    void saveTransformFile();

private:
    QString filename;

    Q3DSurface *graph_image;
    Q3DSurface *graph_filter;
    Q3DScatter *graph_temp;
    Q3DScatter *graph_temp2;

    QWidget *container_image;
    QWidget *container_filter;

    QPushButton *m_btnOpen;
    QPushButton *m_btnReload;
    QPushButton *m_btnApply;

    QLabel *originlabel;
    QLabel *speclabel;
    QLabel *filterlabel;
    QLabel *resultlabel;
    QComboBox *m_filterList;
    QComboBox *m_typeList;

    QSlider *m_kernelsize;
    QSlider *m_kernelsigma;

    QPushButton *m_btnOriginShown;
    QPushButton *m_btnFilterShown;
    QPushButton *m_btnResultShown;


    SurfaceGraph *modifier;
    SurfaceGraph *modifier_filter;

    imageprocess *imgpros;
    bool imageApplyed;

    ImageCropWindow *imgcropw;
    QImage m_orignin;
    QImage m_result;

};

#endif // MAINWINDOW_H
