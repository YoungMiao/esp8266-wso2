/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolselectutils.h
* @brief:地图选择功能函数集合类
* @version: 1.0
* @author:liqi
* @date:2017-09-04
*/


#ifndef QGSMAPTOOLSELECTUTILS_H
#define QGSMAPTOOLSELECTUTILS_H

#include "qgsvectorlayer.h"
#include <Qt>
#include <QRect>
#include <QPoint>

class QMouseEvent;
class QgsMapCanvas;
class QgsVectorLayer;
class QgsGeometry;
class QgsRubberBand;

namespace QgsMapToolSelectUtils
{
    /**
    * @brief：获取符合选择框和条件的一些特征对象集合
    * @param：canvas 当前画图
    * @param：selectGeometry 用来选择的范围框的几何对象，坐标系与地图画布相同
    * @param：doContains 是否必须包含才设置选中
    * @param：singleSelect 是否仅仅选择距离选择框最近的特征对象
    * @return：符合条件的特征对象集合
    * @see：since QGIS 2.16
    * @note：无
    */
    QgsFeatureIds getMatchingFeatures( QgsMapCanvas *canvas, const QgsGeometry &selectGeometry, bool doContains, bool singleSelect );

    /**
    * @brief：选择当前选中图层的特征对象
    * @param：canvas 当前画图
    * @param：selectGeometry 用来选择的范围框的几何对象，坐标系与地图画布相同
    * @param：selectBehavior 选择行为：如覆盖上次结果或者增加到选中集合
    * @param：doContains 是否必须包含才设置选中
    * @param：singleSelect 是否仅仅选择距离选择框最近的特征对象
    * @return：符合条件的特征对象集合
    * @see：since QGIS 2.16
    * @note：无
    */
    void setSelectedFeatures( QgsMapCanvas *canvas,
                              const QgsGeometry &selectGeometry,
                              QgsVectorLayer::SelectBehavior selectBehavior = QgsVectorLayer::SetSelection,
                              bool doContains = true,
                              bool singleSelect = false );

    /**
    * @brief：选择当前选中图层的多个特征对象
    * @param：canvas 当前画图
    * @param：selectGeometry 用来选择的范围框的几何对象，坐标系与地图画布相同
    * @param：e 鼠标事件，用来确定当前选择行为（增加、裁剪、包含等）
    * @return：符合条件的特征对象集合
    * @see：since QGIS 2.16
    * @note：无
    */
    void selectMultipleFeatures( QgsMapCanvas *canvas, const QgsGeometry &selectGeometry, QMouseEvent *e );

    /**
    * @brief：选择当前选中图层的单个特征对象
    * @param：canvas 当前画图
    * @param：selectGeometry 用来选择的范围框的几何对象，坐标系与地图画布相同
    * @param：e 鼠标事件，用来确定当前选择行为（增加、裁剪、包含等）
    * @return：符合条件的特征对象集合
    * @see：since QGIS 2.16
    * @note：无
    */
    void selectSingleFeature( QgsMapCanvas *canvas, const QgsGeometry &selectGeometry, QMouseEvent *e );

    /**
    * @brief：获取画布的当前矢量图层
    * @param：canvas 当前画图
    * @return：符合条件的矢量图层，如果为空，则不存在
    * @see：无
    * @note：无
    */
    QgsVectorLayer *getCurrentVectorLayer( QgsMapCanvas *canvas );

    /**
    * @brief：基于矢量图层的类型对矩形进行最小的扩展
    * @param：selectRect 需要扩展的矩形
    * @param：vlayer 指定的矢量图层
    * @param：point 扩展矩形基于的中心点
    * @return：无
    * @see：无
    * @note：无
    */
    void expandSelectRectangle( QRect &selectRect, QgsVectorLayer *vlayer, QPoint point );

    /**
    * @brief：基于一个设备坐标系的矩形来设置一个基于地图单位的矩形来构建QgsRubberband
    * @param：canvas 当前画图
    * @param：selectRect 用来选择的范围框的几何对象，设备坐标
    * @param：rubberBand 使用输入矩形转换后的基于地图单位的RubberBand
    * @return：无
    * @see：无
    * @note：无
    */
    void setRubberBand( QgsMapCanvas *canvas, QRect &selectRect, QgsRubberBand *rubberBand );
}

#endif
