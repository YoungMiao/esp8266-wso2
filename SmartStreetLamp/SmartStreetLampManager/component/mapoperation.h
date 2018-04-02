#ifndef MAPOPERATION_H
#define MAPOPERATION_H
#include "include/mapview.h"
#include "QtnRibbonBar.h"
class CLightingStrategyDialog;
class CkaaSubject;
class CMapOperation : public MapView
{
    Q_OBJECT
public:
    explicit CMapOperation(QWidget *parent = 0, QString strPath = "",
                           bool isNeedSI = false, CkaaSubject* kaaRegistServer = Q_NULLPTR);
    ~CMapOperation();
    void createMapOperationPage(RibbonBar* ribbon, QString strMapOperation);

    void ChangeLampStatusOperation(QString lampSerialNumber, bool status);
    void getLampLightingStategy(CLightingStrategyDialog* lightingStrategyDlg);

private:
    //地图选择
    QAction* m_actSelect;

    CkaaSubject* m_kaaRegistServer;

    QTime m_lampOnTime;
    QTime m_lampOffTime;
    QString m_lampStrategy;

    CLightingStrategyDialog* m_LightingStrategyDlg;
private Q_SLOTS:

    //地图选择
    void onActionSelect(bool);
    //放大
    void onActionZoomIn(bool bChecked);
    //缩小
    void onActionZoomOut(bool bChecked);
    //全图
    void onActionFullMap(bool bChecked);
    //漫游
    void onActionRoam(bool bChecked);
    //地图切换
    void onActionSwitchMap(bool);

    //图层识别
    void onActionLayerIdentify(bool bChecked);

    //距离测量
    void onActionDistanceMeasure(bool bChecked);


    //面积测量
    void onActionAreaMeasure(bool bChecked);

    //标签
    void onActionAddLabel(bool bStatus);
    //矩形选择
    void onActionSelectRectangle(bool bChecked);
    //多边形选择
    void onActionSelectPolygon(bool bChecked);
    //圆形选择
    void onActionSelectCircle(bool bChecked);
    //自由选择
    void onActionSelectFreedom(bool bChecked);
    //清除选择
    void onActionRemoveSelect(bool bStatus);

    void onLampStatusChanged(const QString&, const QString&);
    void onLampInitStatusRecerived(const QString&, const QString&);
};

#endif // MAPOPERATION_H
