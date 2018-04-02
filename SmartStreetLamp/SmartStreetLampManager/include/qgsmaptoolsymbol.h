#ifndef QGSMAPTOOLSYMBOL_H
#define QGSMAPTOOLSYMBOL_H

#include "qgsmaptool.h"

class QgsMapToolSymbol : public QgsMapTool
{
    Q_OBJECT
public:
    QgsMapToolSymbol(QgsMapCanvas *canvas, QgsVectorLayer* vLayer);
    ~QgsMapToolSymbol();

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

    /**
    * @brief:清空增加的标签
    * @param：无
    * @return：无
    * @note：无
    */
    void clearSymbols();

    /**
    * @brief:设置符号类型
    * @param：无
    * @return：无
    * @note：无
    */
    void setTypes(int type);

private:
    QgsVectorLayer *m_SymbolLayer; // 保存标签的临时矢量图层
    QgsPoint snapPoint(const QPoint &p);
    void addSymbol(const QgsPoint &pt);
    int m_CurrentType;
};

#endif // QGSMAPTOOLSYMBOL_H
