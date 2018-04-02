/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselectfreehand.h
* @brief:地图自由选择功能
* @version: 1.0
* @author:liqi
* @date:2017-09-04
*/
#ifndef QGSMAPTOOLSELECTFREEHAND_H
#define QGSMAPTOOLSELECTFREEHAND_H

#include "qgsmaptool.h"

class QgsMapCanvas;
class QgsRubberBand;


class QgsMapToolSelectFreehand : public QgsMapTool
{
    Q_OBJECT
  public:
    QgsMapToolSelectFreehand( QgsMapCanvas *canvas );

    virtual ~QgsMapToolSelectFreehand();

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

    QgsRubberBand *m_RubberBand; // 存储自由绘制选择区域的点
    bool m_Dragging; // 是否处于平移状态
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
};

#endif
