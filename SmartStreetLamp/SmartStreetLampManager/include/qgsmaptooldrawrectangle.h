/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselectrectangle.h
* @brief:地图矩形选择功能
* @version: 1.0
* @author:liqi
* @date:2017-09-04
*/
#ifndef QGSMAPTOOLDRAWRECTANGLE_H
#define QGSMAPTOOLDRAWRECTANGLE_H

#include <QRect>
#include "qgsmaptool.h"

class QPoint;
class QMouseEvent;
class QgsMapCanvas;
class QgsVectorLayer;
class QgsGeometry;
class QgsRubberBand;

class QgsMapToolDrawRectangle : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolDrawRectangle( QgsMapCanvas *canvas, QgsVectorLayer * vLayer, bool isRegion );
    virtual ~QgsMapToolDrawRectangle();

    /**
    * @brief::重载Mouse move event
    * @param：e 鼠标平移事件
    * @return：无
    * @note：无
    */
    virtual void canvasMoveEvent( QgsMapMouseEvent *e ) override;

    /**
    * @brief::重载Mouse press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    virtual void canvasPressEvent( QgsMapMouseEvent *e ) override;

    /**
    * @brief::重载Mouse release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    virtual void canvasReleaseEvent( QgsMapMouseEvent *e ) override;

  private:

    bool m_Dragging; // 是否处于平移状态
    bool m_IsRegion; // 是否是区域绘制
    QRect m_SelectRect; // 保存选择的矩形区域
    QgsRubberBand *m_RubberBand; // 进行绘制的RubberBand ，保存绘制的点
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
    QgsVectorLayer * m_RectangleLayer; // 保存绘制矩形和区域的图层

    /**
    * @brief::设置绘制的Rubber
    * @param：无
    * @return：无
    * @note：无
    */
    void setRubberBand();

    /**
    * @brief::在没有拖拽时将矩形扩展为2*2大小
    * @param：point 当前鼠标位置点
    * @return：无
    * @note：无
    */
    void expandRectangle(QPoint point);

    /**
    * @brief::增加绘制的区域或者线条到图层
    * @param：无
    * @return：无
    * @note：无
    */
    void addPolygon();
};

#endif // QGSMAPTOOLDRAWRECTANGLE_H
