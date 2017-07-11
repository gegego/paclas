/******************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H
#include <QFileDialog>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>

using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DSurface *surface);
    ~SurfaceGraph();

    void AddImage(QImage img);
    void RemoveImage();
//    void importResult(QImage result);

//    void showImage(bool enable);
//    void showFilter(bool enable);
//    void showResult(bool enable);

//    void enableHeightMapModel(bool enable);
    void changeTheme(int theme);

private:
    Q3DSurface *m_graph;

    QHeightMapSurfaceDataProxy *m_imageProxy;
    QSurface3DSeries *m_imageSeries;

//    QHeightMapSurfaceDataProxy *m_image2Proxy;
//    QSurface3DSeries *m_image2Series;

//    QHeightMapSurfaceDataProxy *m_image3Proxy;
//    QSurface3DSeries *m_image3Series;

//    QHeightMapSurfaceDataProxy *m_image4Proxy;
//    QSurface3DSeries *m_image4Series;

//    QHeightMapSurfaceDataProxy *m_lstImageProxy[];
//    QSurface3DSeries *m_lstImageSeries[];

    int m_index;

    int m_heightMapWidth;
    int m_heightMapHeight;

    bool m_orighinshown;
    bool m_filtershown;
    bool m_resultshown;
};

#endif // SURFACEGRAPH_H
