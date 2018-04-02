/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselect.h
* @brief:地图选择兼平移功能
* @version: 1.0
* @author:liqi
* @date:2017-08-07
*/

#ifndef QGSMAPTOOLSELECT_H
#define QGSMAPTOOLSELECT_H

#include "qgsmaptool.h"

class QgsMapCanvas;
class QMouseEvent;

class QgsMapToolSelect : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolSelect( QgsMapCanvas *canvas );
    virtual ~QgsMapToolSelect();

    /**
    * @brief:重载Mouse press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    virtual void canvasPressEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse move event
    * @param：e 鼠标平移事件
    * @return：无
    * @note：无
    */
    virtual void canvasMoveEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    virtual void canvasReleaseEvent( QgsMapMouseEvent* e ) override;

signals:

    /**
    * @brief:发送鼠标按下消息
    * @param：point 鼠标当前点
    * @return：无
    * @note：point为地图坐标，取决于当前画布所采用的的坐标系
    */
    void mouseLeftButtonPressed(const QgsPoint &point);

private:
    bool m_Dragging; // 判断是否在平移状态
};

#endif
