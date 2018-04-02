/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptooldrawcircle.h
* @brief:地图圆形绘制功能
* @version: 1.0
* @author:liqi
* @date:2017-09-12
*/
#ifndef QGSMAPTOOLDRAWCIRCLE_H
#define QGSMAPTOOLDRAWCIRCLE_H

#include "qgsmaptool.h"
#include "qgspoint.h"

class QgsMapCanvas;
class QgsRubberBand;

class QgsMapToolDrawCircle : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolDrawCircle( QgsMapCanvas *canvas, QgsVectorLayer *vLayer, bool isRegion );
    virtual ~QgsMapToolDrawCircle();

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

    /**
    * @brief:重载Mouse release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    virtual void canvasReleaseEvent( QgsMapMouseEvent *e ) override;

private:

    /**
    * @brief:设置绘制Rubber的半径，使用大概40段
    * @param：radiusEdge 设置边缘点，中心点由canvasPressEvent中确定
    * @return：无
    * @note：无
    */
    void setRadiusRubberBand( QgsPoint &radiusEdge );

    /**
    * @brief:增加绘制的圆形对象到对应图层内
    * @param：无
    * @return：无
    * @note：无
    */
    void addPolygon();

    QgsRubberBand *m_RubberBand; // 进行绘制的RubberBand ，保存绘制的点
    QgsPoint m_RadiusCenter; // 进行绘制的中心点
    bool m_Dragging; // 是否处于平移状态
    bool m_IsRegion; // 是否绘制圆形区域还是圆线
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
    QgsVectorLayer * m_CircleLayer; // 保存绘制的圆形图层
};

#endif // QGSMAPTOOLDRAWCIRCLE_H
