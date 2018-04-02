#include "mapoperation.h"
#include "QtitanRibbon.h"
#include "qgsvectorlayer.h"
#include "qgsproject.h"
#include "qgslayertree.h"
#include "SmartStreetLampControlAPI/kaasubjectmqtt.h"
#include "setting/clightingstrategydialog.h"
CMapOperation::CMapOperation(QWidget *parent , QString strPath, bool isNeedSI,CkaaSubject* kaaRegistServer)
    : MapView(parent, strPath, isNeedSI)
{

    m_actSelect = Q_NULLPTR;

    m_lampOnTime = QTime(0,0,0,0);
    m_lampOffTime = QTime(0,0,0,0);
    m_lampStrategy = "";
    m_LightingStrategyDlg = Q_NULLPTR;
    m_kaaRegistServer = kaaRegistServer;
    QString strTopic = QString("/kaa/smartlamp/%1/lampStatus").arg(APPLICATION_TOKEN);
    m_kaaRegistServer->registerObserver(this, SLOT(onLampStatusChanged(const QString&, const QString&)), strTopic);
    strTopic = QString("/kaa/smartlamp/%1/toServer/Cmmand").arg(APPLICATION_TOKEN);
    m_kaaRegistServer->publishMSG(strTopic, "{\"cmmand\":\"GetLampStatues\"}");
    QString strInitStatus = QString("/kaa/smartlamp/%1/lampStatusInit").arg(APPLICATION_TOKEN);
    m_kaaRegistServer->registerObserver(this, SLOT(onLampInitStatusRecerived(const QString&, const QString&)), strInitStatus);
}
CMapOperation::~CMapOperation()
{

}
void CMapOperation::createMapOperationPage(RibbonBar* ribbon, QString strMapOperation)
{
    RibbonPage*  mapOperationPage = ribbon->addPage(strMapOperation);
    if(mapOperationPage != Q_NULLPTR)
    {
        //添加组
        if(RibbonGroup* groupBasicOP = mapOperationPage->addGroup(QString::fromWCharArray(L"基本操作")))
        {
            groupBasicOP->setOptionButtonVisible();

            //地图选择
            m_actSelect = new QAction(QIcon(":/resource/images/select.png"), QString::fromWCharArray(L"选择"));
            m_actSelect->setCheckable(true);
            groupBasicOP->addAction(m_actSelect, Qt::ToolButtonTextUnderIcon);
            connect(m_actSelect, SIGNAL(triggered(bool)), this, SLOT(onActionSelect(bool)));
            //放大
            QAction* actZoomIn =new QAction(QIcon(":/resource/images/zoomIn.png"),QString::fromWCharArray(L"放大"));
            actZoomIn->setCheckable(true);
            groupBasicOP->addAction(actZoomIn, Qt::ToolButtonTextUnderIcon);
            connect(actZoomIn, SIGNAL(triggered(bool)), this, SLOT(onActionZoomIn(bool)));
            //缩小
            QAction* actZoomOut = new QAction(QIcon(":/resource/images/zoomOut.png"),QString::fromWCharArray(L"缩小"));
            actZoomOut->setCheckable(true);
            groupBasicOP->addAction(actZoomOut, Qt::ToolButtonTextUnderIcon);
            connect(actZoomOut, SIGNAL(triggered(bool)), this, SLOT(onActionZoomOut(bool)));
            //全图
            QAction* actFull = new QAction(QIcon(":/resource/images/fullMap.png"), QString::fromWCharArray(L"全图"));
            actFull->setCheckable(true);

            groupBasicOP->addAction(actFull, Qt::ToolButtonTextUnderIcon);
            connect(actFull, SIGNAL(triggered(bool)), this, SLOT(onActionFullMap(bool)));
            //漫游
            QAction* actRoam = new QAction(QIcon(":/resource/images/roam.png"),QString::fromWCharArray(L"漫游"));
            actRoam->setCheckable(true);
            groupBasicOP->addAction(actRoam, Qt::ToolButtonTextUnderIcon);
            connect(actRoam, SIGNAL(triggered(bool)), this, SLOT(onActionRoam(bool)));
            //地图切换
            QAction* actSwitch = new QAction(QIcon(":/resource/images/switchMap.png"),QString::fromWCharArray(L"切换地图"));
            actSwitch->setCheckable(true);
            groupBasicOP->addAction(actSwitch, Qt::ToolButtonTextUnderIcon);
            connect(actSwitch, SIGNAL(triggered(bool)),this, SLOT(onActionSwitchMap(bool)));
            QActionGroup *actionGroup = new QActionGroup(this);
            actionGroup->setExclusive(true);
            actionGroup->addAction(m_actSelect);
            actionGroup->addAction(actZoomIn);
            actionGroup->addAction(actZoomOut);
            actionGroup->addAction(actFull);
            actionGroup->addAction(actRoam);
            actionGroup->addAction(actSwitch);

        }

        if(RibbonGroup* groupMeasure = mapOperationPage->addGroup(QString::fromWCharArray(L"测量")))
        {
            groupMeasure->setOptionButtonVisible();
            //图层识别
            QAction* actIdentify = new QAction(QIcon(":/resource/images/layerIdentify.png"),QString::fromWCharArray(L"图层识别"));
            actIdentify->setCheckable(true);
            groupMeasure->addAction(actIdentify, Qt::ToolButtonTextUnderIcon);
            connect(actIdentify, SIGNAL(triggered(bool)),this, SLOT(onActionLayerIdentify(bool)));

            //距离测量
            QAction* actDistMeasure = new QAction(QIcon(":/resource/images/distMeasure.png"),QString::fromWCharArray(L"距离测量"));
            actDistMeasure->setCheckable(true);
            groupMeasure->addAction(actDistMeasure, Qt::ToolButtonTextUnderIcon);
            connect(actDistMeasure, SIGNAL(triggered(bool)),this, SLOT(onActionDistanceMeasure(bool)));

            //面积测量
            QAction* actAreaMeasure = new QAction(QIcon(":/resource/images/areaMeasure.png"),QString::fromWCharArray(L"面积测量"));
            actAreaMeasure->setCheckable(true);
            groupMeasure->addAction(actAreaMeasure, Qt::ToolButtonTextUnderIcon);
            connect(actAreaMeasure, SIGNAL(triggered(bool)),this, SLOT(onActionAreaMeasure(bool)));

            //标签
            QAction* actLabel = new QAction(QIcon(":/resource/images/label.png"),QString::fromWCharArray(L"添加标签"));
            actLabel->setCheckable(true);
            groupMeasure->addAction(actLabel, Qt::ToolButtonTextUnderIcon);
            connect(actLabel, SIGNAL(triggered(bool)), this, SLOT(onActionAddLabel(bool)));
            QActionGroup *actionMeasure = new QActionGroup(this);
            actionMeasure->setExclusive(true);
            actionMeasure->addAction(actIdentify);
            actionMeasure->addAction(actDistMeasure);
            actionMeasure->addAction(actAreaMeasure);
            actionMeasure->addAction(actLabel);

        }

        if(RibbonGroup* groupRegion = mapOperationPage->addGroup(QString::fromWCharArray(L"区域选择")))
        {
            groupRegion->setOptionButtonVisible();
            //多边形选择
            QAction* actPolygon = new QAction(QIcon(":/resource/images/selectPolygen.png"),QString::fromWCharArray(L"选择多边形"));
            actPolygon->setCheckable(true);
            groupRegion->addAction(actPolygon, Qt::ToolButtonTextUnderIcon);
            connect(actPolygon, SIGNAL(triggered(bool)),this, SLOT(onActionSelectPolygon(bool)));
            //矩形选择
            QAction* actRect = new QAction(QIcon(":/resource/images/selectRect.png"),QString::fromWCharArray(L"选择矩形"));
            actRect->setCheckable(true);
            groupRegion->addAction(actRect, Qt::ToolButtonTextUnderIcon);
            connect(actRect, SIGNAL(triggered(bool)),this, SLOT(onActionSelectRectangle(bool)));

            //圆形选择
            QAction* actCircle = new QAction(QIcon(":/resource/images/selectCircle.png"),QString::fromWCharArray(L"选择圆形"));
            actCircle->setCheckable(true);
            groupRegion->addAction(actCircle,Qt::ToolButtonTextUnderIcon);
            connect(actCircle, SIGNAL(triggered(bool)),this, SLOT(onActionSelectCircle(bool)));

            //自由选择
            QAction* actFree = new QAction(QIcon(":/resource/images/freeSelect.png"),QString::fromWCharArray(L"自由选择"));
            actFree->setCheckable(true);
            groupRegion->addAction(actFree, Qt::ToolButtonTextUnderIcon);
            connect(actFree, SIGNAL(triggered(bool)),this, SLOT(onActionSelectFreedom(bool)));

            //清除选择
            QAction* actRemove = new QAction(QIcon(":/resource/images/removeSelect.png"),QString::fromWCharArray(L"清除选择"));
            actRemove->setCheckable(true);
            groupRegion->addAction(actRemove, Qt::ToolButtonTextUnderIcon);
            connect(actRemove, SIGNAL(triggered(bool)),this, SLOT(onActionRemoveSelect(bool)));
            QActionGroup *actionSelect = new QActionGroup(this);
            actionSelect->setExclusive(true);
            actionSelect->addAction(actPolygon);
            actionSelect->addAction(actRect);
            actionSelect->addAction(actCircle);
            actionSelect->addAction(actFree);
            actionSelect->addAction(actRemove);
        }
    }

    return;
}
//地图选择
void CMapOperation::onActionSelect(bool bChecked)
{

    onSelectHander();
}

//放大
void CMapOperation::onActionZoomIn(bool bChecked)
{

    if(bChecked)
    {
        onZoomInHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter zoomInHandler!"<<endl;
}

//缩小
void CMapOperation::onActionZoomOut(bool bChecked)
{

    if(bChecked)
    {
        onZoomOutHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter zoomOutHandler!"<<endl;
}


//全图
void CMapOperation::onActionFullMap(bool bChecked)
{

    if(bChecked)
    {
        onSelectHander();
        setFullExtentLayer(getLayerByName("shanxi_xian_p"));
        onFullMap();
        onSelectHander();
        m_actSelect->setChecked(true);
    }
    qDebug()<<"Enter fullMapHandler!"<<endl;
}

//漫游
void CMapOperation::onActionRoam(bool bChecked)
{

    if(bChecked)
    {
        onPanHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter roamHandler!"<<endl;
}

//地图切换
void CMapOperation::onActionSwitchMap(bool)
{

    if(!m_actSelect->isChecked())
    {
        onSelectHander();
        m_actSelect->setChecked(true);
    }

    static bool status = true;
    if(status)
    {
        switchMap(2);
        status = false;
    }
    else
    {
        switchMap(1);
        status = true;
    }

    qDebug()<<"Enter switchMapHandler!"<<endl;
}

//图层识别
void CMapOperation::onActionLayerIdentify(bool bChecked)
{

    if(bChecked)
    {
        onIdentifyHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter layerIdentifyHandler!"<<endl;
}

//距离测量
void CMapOperation::onActionDistanceMeasure(bool bChecked)
{

    if(bChecked)
    {
        onMeasureLineHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter distanceMeasureHandler!"<<endl;
}


//面积测量
void CMapOperation::onActionAreaMeasure(bool bChecked)
{

    if(bChecked)
    {
        onMeasureAreaHander();
    }
    else
    {
        onSelectHander();
    }
    qDebug()<<"Enter areaMeasureHandler!"<<endl;
}

//标签
void CMapOperation::onActionAddLabel(bool bStatus)
{

    if(bStatus)
    {
        onDrawLabelHander();
    }
    else
    {
        onClearDrawsHander();
    }
    qDebug()<<"Enter addLabelHandler!"<<endl;
}

//矩形选择
void CMapOperation::onActionSelectRectangle(bool bChecked)
{

    if(bChecked)
    {
        onRectangleSelHander();
    }
    qDebug()<<"Enter selectRectangleHandler!"<<endl;
}

//多边形选择
void CMapOperation::onActionSelectPolygon(bool bChecked)
{

    if(bChecked)
    {
        onPolygonSelHander();
    }
    qDebug()<<"Enter selectPolygonHandler!"<<endl;
}

//圆形选择
void CMapOperation::onActionSelectCircle(bool bChecked)
{
    if(bChecked)
    {
        onRadiusSelHander();
    }

    qDebug()<<"Enter selectCircleHandler!"<<endl;
}

//自由选择
void CMapOperation::onActionSelectFreedom(bool bChecked)
{

    if(bChecked)
    {
        onFreeHandSelHander();
    }
    qDebug()<<"Enter selectFreedomHandler!"<<endl;
}

//清除选择
void CMapOperation::onActionRemoveSelect(bool bStatus)
{

    if(bStatus)
    {
        onClearSelHander();
        onSelectHander();
        m_actSelect->setChecked(true);
    }
    qDebug()<<"Enter removeSelectHandler!"<<endl;
}


void CMapOperation::ChangeLampStatusOperation(QString lampSerialNumber,bool status)
{
    QgsLayerTree *tree = QgsProject::instance()->layerTreeRoot();
    QgsLayerTreeGroup* group = tree->findGroup(QString("LampInfo"));
    QList<QgsLayerTreeLayer *> layerList = group->findLayers();

    for(int i = 0; i < layerList.count(); i++)
    {
        QgsLayerTreeLayer* layerTree = layerList[i];
        //qDebug()<<"layer name:"<<layerTree->name();
        QgsVectorLayer* vLayer = this->getLayerByName(layerTree->name());
        QgsFeature feat;
        if(this->getFeatureByField(vLayer, "lampID", lampSerialNumber.right(13),feat))
        {
            qDebug()<<"111111111111111111";
            bool s = feat.attribute("lampStatus").toBool();
            if(s == status)
            {
                qDebug()<<"222222222222222222";
                return;
            }
            if (vLayer->startEditing())
            {
                qDebug()<<"333333333333333333";
                vLayer->changeAttributeValue(feat.id(), feat.fieldNameIndex("lampStatus"), status);
                if( !vLayer->commitChanges())
                {
                    qDebug() << "commitChanges error:"<<vLayer->commitErrors();
                }
                return;
            }
        }
    }

}

void CMapOperation::getLampLightingStategy(CLightingStrategyDialog* lightingStrategyDlg)
{
    m_LightingStrategyDlg = lightingStrategyDlg;
}


void CMapOperation::onLampStatusChanged(const QString& strTopic, const QString& strLampStatusJson)
{
    qDebug()<<"receive lamp status Topic:"<<strTopic<<"strLampStatus"<<strLampStatusJson;
    QByteArray  byteArray = strLampStatusJson.toUtf8();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &error);
    QString lampId = "";
    bool lampStatus = false;
    if(!document.isNull() && error.error == QJsonParseError::NoError)
    {
        if(document.isObject())
        {
            QVariantMap map = document.toVariant().toMap();
            lampId = map["lampId"].toString();
            lampStatus = map["lampStatus"].toBool();
            ChangeLampStatusOperation(lampId, lampStatus);
        }
    }


}

void CMapOperation::onLampInitStatusRecerived(const QString& strTopic, const QString& strLampStatus)
{
    qDebug()<<"enter onLampInitStatusRecerived:"<<strTopic<<"strLampStatus:"<<strLampStatus<<endl;
    QJsonDocument document = QJsonDocument::fromJson(strLampStatus.toUtf8());
    if(document.isObject())
    {
        QVariantMap map = document.toVariant().toMap();
        QVariantMap::iterator iter = map.begin();
        while(iter != map.end())
        {
            QVariantMap map2 = (*iter).toMap();
            QString lampId = map2["lampId"].toString();
            bool lampStatus = map2["lampStatus"].toBool();
            //qDebug()<<"lampId"<<lampId<<"lampStatus:"<<lampStatus;
            ChangeLampStatusOperation(lampId, lampStatus);
            ++iter;
        }

    }
}
