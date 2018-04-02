/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptooldrawpolygon.h
* @brief:地图多边形选择功能
* @version: 1.0
* @author:liqi
* @date:2017-09-11
*/

#ifndef QGSMAPTOOLDRAWPOLYGON_H
#define QGSMAPTOOLDRAWPOLYGON_H

#include "qgsmaptool.h"
class QgsMapCanvas;
class QgsRubberBand;

class QgsMapToolDrawPolygon : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolDrawPolygon( QgsMapCanvas *canvas, QgsVectorLayer* vLayer, bool drawPolygon );
    virtual ~QgsMapToolDrawPolygon();

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
    * @brief::重载Key Press event
    * @param：e 键按下事件
    * @return：无
    * @note：无
    */
    virtual void keyPressEvent( QKeyEvent* e ) override;

    /**
    * @brief::清空增加的标签
    * @param：无
    * @return：无
    * @note：无
    */
    void clearAllFeatures();

private:

    QList<QgsPoint> m_Points; // 点的集合
    QgsRubberBand *m_RubberBand; // 存储多边形选择区域的点
    QColor m_FillColor; // 填充颜色
    QColor m_StrokeColor; // 线条颜色
    QgsVectorLayer *m_PolygonLayer; // 保存标签的临时矢量图层
    bool m_Done; // 是否量测结束
    bool m_DrawPolygon; // 是否是绘制多边形

    /**
    * @brief::增加绘制的对象（如多边形或者多义线）
    * @param：无
    * @return：无
    * @note：无
    */
    void addPolygon();

    /**
    * @brief:撤销之前的操作
    * @param：无
    * @return：无
    * @note：无
    */
    void undo();
};

#endif // QGSMAPTOOLDRAWPOLYGON_H
