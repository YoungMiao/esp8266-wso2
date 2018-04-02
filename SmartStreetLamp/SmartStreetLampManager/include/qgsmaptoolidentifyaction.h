/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2016
*Company: 长城数字[www.e-u.cn]
*
* @file:qgsmaptoolidentifyaction.h
* @brief:地图图层识别功能
* @version: 1.0
* @author:liqi
* @date:2017-08-07
*/

#ifndef QGSMAPTOOLIDENTIFYACTION_H
#define QGSMAPTOOLIDENTIFYACTION_H

#include "qgsmaptoolidentify.h"
#include "qgspoint.h"
#include "qgsfeature.h"
#include "qgsfeaturestore.h"
#include "qgsfield.h"

#include <QObject>
#include <QPointer>

class QgsMapLayer;
class QgsRasterLayer;
class QgsVectorLayer;

class QgsMapToolIdentifyAction : public QgsMapToolIdentify
{
    Q_OBJECT

  public:
    QgsMapToolIdentifyAction( QgsMapCanvas * canvas );

    ~QgsMapToolIdentifyAction();

    /**
    * @brief:重载Mouse Move event
    * @param：e 鼠标移动事件
    * @return：无
    * @note：无
    */
    virtual void canvasMoveEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse Press event
    * @param：e 鼠标按下事件
    * @return：无
    * @note：无
    */
    virtual void canvasPressEvent( QgsMapMouseEvent* e ) override;

    /**
    * @brief:重载Mouse Release event
    * @param：e 鼠标抬起事件
    * @return：无
    * @note：无
    */
    virtual void canvasReleaseEvent( QgsMapMouseEvent* e ) override;

    virtual void activate() override;

    virtual void deactivate() override;
};

#endif
