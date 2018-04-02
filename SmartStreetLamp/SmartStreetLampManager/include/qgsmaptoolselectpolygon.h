/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselectpolygon.h
* @brief:地图多边形选择功能
* @version: 1.0
* @author:liqi
* @date:2017-09-04
*/

#ifndef QGSMAPTOOLSELECTPOLYGON_H
#define QGSMAPTOOLSELECTPOLYGON_H

#include "qgsmaptool.h"
class QgsMapCanvas;
class QgsRubberBand;


class QgsMapToolSelectPolygon : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolSelectPolygon( QgsMapCanvas *canvas );

    virtual ~QgsMapToolSelectPolygon();

    /**
    * @brief:重载Mouse move event
    * @param：e 鼠标平移事件
    * @return：无
    * @note：无
    */
    virtual void canvasMoveEvent( QgsMapMouseEvent *e ) override;

    /**
    * @brief:重载Mouse press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    virtual void canvasPressEvent( QgsMapMouseEvent *e ) override;

private:

    QgsRubberBand *m_RubberBand; // 存储多边形选择区域的点
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
};

#endif
