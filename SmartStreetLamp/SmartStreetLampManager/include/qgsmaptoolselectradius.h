/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselectradius.h
* @brief:地图圆形选择功能
* @version: 1.0
* @author:liqi
* @date:2017-09-04
*/

#ifndef QGSMAPTOOLSELECTRADIUS_H
#define QGSMAPTOOLSELECTRADIUS_H


#include "qgsmaptool.h"
#include "qgspoint.h"

class QgsMapCanvas;
class QgsRubberBand;


class QgsMapToolSelectRadius : public QgsMapTool
{
    Q_OBJECT
  public:
    QgsMapToolSelectRadius( QgsMapCanvas *canvas );

    virtual ~QgsMapToolSelectRadius();

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

    QgsRubberBand *m_RubberBand; // 进行绘制的RubberBand ，保存绘制的点
    QgsPoint m_RadiusCenter; // 进行绘制的中心点
    bool m_Dragging; // 是否处于平移状态
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
};

#endif
