/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolannotation.h
* @brief:地图图层添加文字标签功能
* @version: 1.0
* @author:liqi
* @date:2017-09-11
*/

#ifndef QGSMAPTOOLANNOTATION_H
#define QGSMAPTOOLANNOTATION_H

#include "qgsmaptool.h"
#include <QLineEdit>

class QgsMapToolAnnotation: public QgsMapTool
{
    Q_OBJECT

public:
    QgsMapToolAnnotation( QgsMapCanvas *canvas, QgsVectorLayer* vLayer );
    ~QgsMapToolAnnotation();

    /**
    * @brief:重载Mouse Press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    void canvasPressEvent( QgsMapMouseEvent *e ) override;

    /**
    * @brief:重载Mouse Release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    void canvasReleaseEvent( QgsMapMouseEvent *e ) override;

    virtual void activate() override;
    virtual void deactivate() override;

    /**
    * @brief:清空增加的标签
    * @param：无
    * @return：无
    * @note：无
    */
    void clearLabels();

protected:

private:
    QPoint m_LastMousePosition; // 鼠标最后的位置
    QLineEdit *m_LineEdit; // 弹出的LineEdit
    int m_TextSize; // 弹出的LineEdit的文字大小
    QColor m_TextColor; // 弹出的LineEdit的文字颜色
    QString  m_TextFontName; // 弹出的LineEdit的文字字体
    QList<QgsPoint> m_Points; // 保存记录鼠标按下位置的点集合
    QgsVectorLayer *m_TextLayer; // 保存标签的临时矢量图层

    /**
    * @brief:设置弹出的LineEdit的样式
    * @param：无
    * @return：无
    * @note：无
    */
    void setTextStyle();

    /**
    * @brief:转换鼠标点到画布坐标系
    * @param：p 鼠标点位置
    * @return：无
    * @note：无
    */
    QgsPoint snapPoint(const QPoint &p);

    /**
    * @brief:设置弹出的LineEdit的样式
    * @param：无
    * @return：无
    * @note：无
    */
    void addTextSymbol( const QgsPoint &pt, const QString &label );        
};

#endif // QGSMAPTOOLANNOTATION_H
