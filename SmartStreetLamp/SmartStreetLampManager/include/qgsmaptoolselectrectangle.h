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


#ifndef QGSMAPTOOLRECTANGLE_H
#define QGSMAPTOOLRECTANGLE_H

#include <QRect>
#include "qgsmaptool.h"

class QPoint;
class QMouseEvent;
class QgsMapCanvas;
class QgsVectorLayer;
class QgsGeometry;
class QgsRubberBand;


class QgsMapToolSelectFeatures : public QgsMapTool
{
    Q_OBJECT
  public:
    QgsMapToolSelectFeatures( QgsMapCanvas *canvas );
    virtual ~QgsMapToolSelectFeatures();

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

    bool m_Dragging; // 是否处于平移状态
    QRect m_SelectRect; // 保存选择的矩形区域
    QgsRubberBand *m_RubberBand; // 进行绘制的RubberBand ，保存绘制的点
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
};

#endif
