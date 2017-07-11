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

#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/q3dinputhandler.h>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

const int sampleCountX = 50;
const int sampleCountZ = 50;
const int heightMapGridStepX = 6;
const int heightMapGridStepZ = 6;
const float sampleMin = -8.0f;
const float sampleMax = 8.0f;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{

    m_index = 0;

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                             | QAbstract3DGraph::SelectionSlice);

    m_graph->axisX()->setLabelFormat("%.1f");
    m_graph->axisZ()->setLabelFormat("%.1f");
    m_graph->axisX()->setAutoAdjustRange(true);
    m_graph->axisY()->setAutoAdjustRange(true);
    m_graph->axisZ()->setAutoAdjustRange(true);

    m_graph->axisX()->setTitle(QStringLiteral("x"));
    m_graph->axisY()->setTitle(QStringLiteral("Height"));
    m_graph->axisZ()->setTitle(QStringLiteral("y"));
//    for(int i =0; i<4; i++)
//    {
//        QHeightMapSurfaceDataProxy *m_image1Proxy = new QHeightMapSurfaceDataProxy();
//        QSurface3DSeries *m_image1Series = new QSurface3DSeries();

//        m_lstImageProxy
//    }


//    m_image2Proxy = new QHeightMapSurfaceDataProxy();
//    m_image2Series = new QSurface3DSeries();

//    m_image3Proxy = new QHeightMapSurfaceDataProxy();
//    m_image3Series = new QSurface3DSeries();

//    m_image4Proxy = new QHeightMapSurfaceDataProxy();
//    m_image4Series = new QSurface3DSeries();


//    m_orighinshown = true;
//    m_filtershown = true;
//    m_resultshown = true;
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::AddImage(QImage img)
{

    //m_graph->removeSeries(m_graph->seriesList().at(m_index));

    m_imageProxy = new QHeightMapSurfaceDataProxy();
    m_imageSeries = new QSurface3DSeries();

    m_imageProxy->setHeightMap(img);
    m_imageSeries->setDataProxy(m_imageProxy);
    m_imageSeries->setItemLabelFormat(QStringLiteral("(@xLabel, @zLabel): @yLabel"));
    m_imageProxy->setValueRanges(0.0f, img.width(), 0.0f, img.height());
    //! [2]
    m_heightMapWidth = img.width();
    m_heightMapHeight = img.height();

    QLinearGradient gr;
    gr.setColorAt(0.0, QColor::fromHsv(int(m_index), 245, 245, 255));
    gr.setColorAt(1.0, Qt::green);

//    gr.setColorAt(1.0, QColor(255, 0, 0, m_index*30));

    m_imageSeries->setBaseGradient(gr);
    m_imageSeries->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    m_imageSeries->setDrawMode(QSurface3DSeries::DrawSurface);
    m_imageSeries->setFlatShadingEnabled(false);

    m_graph->addSeries(m_imageSeries);
    m_index++;
}

void SurfaceGraph::RemoveImage()
{
    m_graph->removeSeries(m_graph->seriesList().at(--m_index));
//    m_index--;
}

//void SurfaceGraph::showImage(bool enable)
//{
//    m_orighinshown = enable;
//    if(enable){
//        m_graph->addSeries(m_imageSeries);
//    }
//    else{
//        m_graph->removeSeries(m_imageSeries);
//    }
//}

//void SurfaceGraph::showFilter(bool enable)
//{
//    m_filtershown = enable;

//    if(enable){
//        m_graph->addSeries(m_filterSeries);
//    }
//    else{
//        m_graph->removeSeries(m_filterSeries);
//    }
//}
//void SurfaceGraph::showResult(bool enable)
//{
//    m_resultshown = enable;

//    if(enable){
//        m_graph->addSeries(m_resultSeries);
//    }
//    else{
//        m_graph->removeSeries(m_resultSeries);
//    }
//}
//void SurfaceGraph::enableHeightMapModel(bool enable)
//{
//    if (enable) {
//        //! [4]
//        m_imageSeries->setDrawMode(QSurface3DSeries::DrawSurface);
//        m_imageSeries->setFlatShadingEnabled(false);

//        m_filterSeries->setDrawMode(QSurface3DSeries::DrawWireframe);
//        m_filterSeries->setFlatShadingEnabled(false);

//        m_resultSeries->setDrawMode(QSurface3DSeries::DrawSurface);
//        m_resultSeries->setFlatShadingEnabled(false);

//        m_graph->axisX()->setLabelFormat("%.1f");
//        m_graph->axisZ()->setLabelFormat("%.1f");
//        m_graph->axisX()->setAutoAdjustRange(true);
//        m_graph->axisY()->setAutoAdjustRange(true);
//        m_graph->axisZ()->setAutoAdjustRange(true);

//        m_graph->axisX()->setTitle(QStringLiteral("x"));
//        m_graph->axisY()->setTitle(QStringLiteral("Height"));
//        m_graph->axisZ()->setTitle(QStringLiteral("y"));

//        if(m_orighinshown)
//            m_graph->addSeries(m_imageSeries);

//        if(m_filtershown)
//            m_graph->addSeries(m_filterSeries);

//        if(m_resultshown)
//            m_graph->addSeries(m_resultSeries);

//    }
//}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}
