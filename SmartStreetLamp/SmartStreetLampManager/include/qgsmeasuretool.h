/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmeasuretool.h
* @brief:地图面积和距离量测功能
* @version: 1.0
* @author:liqi
* @date:2017-08-16
*/


#ifndef QGSMEASURETOOL_H
#define QGSMEASURETOOL_H

#include "qgsmaptool.h"
#include "qgsdistancearea.h"

class QgsMapCanvas;
class QgsRubberBand;



class QgsMeasureTool : public QgsMapTool
{
    Q_OBJECT
public:

    QgsMeasureTool( QgsMapCanvas* canvas, bool measureArea );
    virtual ~QgsMeasureTool();

    /**
    * @brief：设置量测时文字显示的颜色，默认为黑色
    * @param：clr 文字的颜色
    * @return：无
    * @see：无
    * @note：无
    */
    void setTextColor( QColor clr );

    /**
    * @brief：设置量测时文字显示的背景颜色默认为白色
    * @param：clr 文字的背景颜色
    * @return：无
    * @see：无
    * @note：无
    */
    void setTextBkColor( QColor clr );

    /**
    * @brief：返回是否是面积量测
    * @param：无
    * @return：true为面积量测；false为距离量测
    * @see：无
    * @note：无
    */
    bool measureArea() { return m_MeasureArea; }

    /**
    * @brief：是否结束，增加了最后一个点
    * @param：无
    * @return：true为量测结束；false为未结束
    * @see：无
    * @note：无
    */
    bool done() { return m_Done; }

    /**
    * @brief：重新开始新的量测
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void restart();

    /**
    * @brief：增加新的点
    * @param：point 增加的新的点
    * @return：无
    * @see：无
    * @note：无
    */
    void addPoint( const QgsPoint &point );

    /**
    * @brief：返回点列表的引用
    * @param：无
    * @return：增加点的集合的引用
    * @see：无
    * @note：无
    */
    const QList<QgsPoint>& points();

    // 继承于 QgsMapTool

    /**
    * @brief:重载Mouse move event
    * @param：e 鼠标平移事件
    * @return：无
    * @note：无
    */
    virtual void canvasMoveEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    virtual void canvasPressEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    virtual void canvasReleaseEvent( QgsMapMouseEvent* e ) override;

    virtual void activate() override;

    virtual void deactivate() override;

    /**
    * @brief:重载Key Press event
    * @param：e 键按下事件
    * @return：无
    * @note：无
    */
    virtual void keyPressEvent( QKeyEvent* e ) override;

  public slots:

    /**
    * @brief:更新投影设置
    * @param：无
    * @return：无
    * @note：无
    */
    void updateSettings();

  protected:

    QList<QgsPoint> m_Points; // 点的集合
    int m_CurrentIndex;// 当前的Rubber的Index
    QgsRubberBand *m_RubberBand; // 用于绘制线的Rubber
    QgsRubberBand *m_RubberBandPoints; // 用于绘制点的Rubber
    bool m_MeasureArea; // 是否是量测面积
    bool m_Done; // 是否量测结束
    bool m_WrongProjectProjection; // 是否采用了错误的投影方式
    QgsDistanceArea m_Da; // 辅助计算面积和距离的类
    int m_DecimalPlaces; // 保留的小数位数
    QgsUnitTypes::DistanceUnit m_CanvasUnits; // 地图画布采用的单位
    QgsUnitTypes::DistanceUnit m_DistanceUnits; // 采用的距离单位
    QgsUnitTypes::AreaUnit m_AreaUnits;// 采用的面积单位

    /**
    * @brief：返回转换后的地图坐标
    * @param：p 需要转换的点
    * @return：转换后的点
    * @see：无
    * @note：无
    */
    QgsPoint snapPoint( const QPoint& p );

    /**
    * @brief：删除最后一个增加的点
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void undo();

private:
    /**
    * @brief：格式化距离
    * @param：distance 距离
    * @param：convertUnits 是否需要单位转换
    * @return：格式化之后的字符串
    * @see：无
    * @note：无
    */
    QString formatDistance(double distance, bool convertUnits) const;

    /**
    * @brief：格式化面积
    * @param：area 面积
    * @param：convertUnits 是否需要单位转换
    * @return：格式化之后的字符串
    * @see：无
    * @note：无
    */
    QString formatArea(double area, bool convertUnits) const;

    /**
    * @brief：转换长度
    * @param：length 长度
    * @param：toUnit 需要转换到的单位
    * @return：转换后的长度
    * @see：无
    * @note：无
    */
    double convertLength(double length, QgsUnitTypes::DistanceUnit toUnit) const;

    /**
    * @brief：转换面积
    * @param：area 面积
    * @param：toUnit 需要转换到的单位
    * @return：转换后的面积
    * @see：无
    * @note：无
    */
    double convertArea(double area, QgsUnitTypes::AreaUnit toUnit) const;
};

#endif
