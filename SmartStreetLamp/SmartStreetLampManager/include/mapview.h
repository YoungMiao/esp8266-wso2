/**
*COPYRIGHT NOTICE
*Copyright: Copyright (c) 2017
*Company: 长城数字[www.e-u.cn]
*
* @file:mapview.h
* @brief:该类主要控制地图的显示谷歌地图的加载、工具类的交互以及一些公共功能的函数封装
* @version: 1.0
* @author:liqi
* @date:2017-08-07
*/

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "mapview_global.h"
#include <QWidget>
#include <QList>
#include <QMap>
class QgsLayerTreeMapCanvasBridge;
class QgsLayerTreeGroup;
class QgsDiagramSettings;
#include "../third/libwebTiles/include/webtiles.h"
#include "qgsmaptoolselect.h"

const double ONEDEGREE = 111319.49;  // 粗略计算一度为111319.49米 根据西安80坐标计算
const int SPATIALINDEXNUMBER = 20000; // 是否进行空间索引创建的feature数量，大于时需要创建，小于时没必要创建
const double VILIGESCALE = 2.38866;  // 告警点到达时缩放的比例尺级别
const int NPOINTCOUNT = 30;          // 摄像头覆盖范围的点数
const double DMAXREGION = 3000.0;    // 距离摄像头的最长距离
const double MINUNIT = 0.000001;     // 经纬度点进行查找时的扩展范围,小数点后6位精确到分米
const double MINLINEEXTENT = 8.0;    // 线状地物选择时扩发的范围框半径
const double INITSCALE = 1563.8 / 1.5; //全图时的地图比例尺，用来计算灾害道路的宽度
const int MAXLINEWIDTH = 8; //道路最宽宽度
const int MINLINEWIDTH = 2; //道路最窄宽度

class QPushButton;
class QgsRasterLayer;
class QgsLayerTreeView;
class QgsMeasureTool;
class QgsSpatialIndex;
class QgsRubberBand;
class QLabel;
class QgsLayerTreeView;
class CCommonTools;

typedef struct tagCAMERAPOS
{
    double dAzimuth;
    double dPitch;
    double focus;
}CAMERAPOS, *PCAMERAPOS;

class MAPVIEWSHARED_EXPORT MapView : public QWidget
{
    Q_OBJECT
public:
    /**
    * @brief：构造函数，设置地图显示的工程
    * @param：parent 依托的父窗口
    * @param：strPath 指定地图工程的路径
    * @return：无
    * @see：无
    * @note：无
    */
    explicit MapView(QWidget *parent = 0, QString strPath = "", bool isNeedSI = false);
    ~MapView();

    /**
    * @brief：设置指定图层的标签颜色
    * @param：textColor 标签颜色
    * @param：layer 指定图层
    * @return：无
    * @see：无
    * @note：无
    */
    void setLayerLabelColor(const QColor &textColor, QgsVectorLayer *layer);

    /**
    * @brief：根据图层名称获取对应矢量图层
    * @param：strLayerName 图层名称
    * @return：指定图层指针
    * @see：无
    * @note：无
    */
    QgsVectorLayer *getLayerByName(const QString strLayerName);

    /**
    * @brief：设置图层的显示与隐藏
    * @param：bShow 是否显示
    * @param：layer 指定图层
    * @return：无
    * @see：无
    * @note：无
    */
    void showOrHideLayer(bool bShow, QgsMapLayer *layer);

    /**
    * @brief：全图
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void onFullMap();

    /**
    * @brief：根据点位置、角度和距离计算新的点位
    * @param：lon 指定点的经度(作为输出参数)
    * @param：lat 指定点的纬度(作为输出参数)
    * @param：angle 指定角度
    * @param：dist 指定距离
    * @return：无
    * @see：无
    * @note：无
    */
    void getLocation(double &lon, double &lat, double angle, double dist);

    /**
    * @brief：设置进行全图缩放的图层
    * @param：layer 指定全图的图层
    * @return：无
    * @see：无
    * @note：无
    */
    void setFullExtentLayer(QgsVectorLayer *layer);

    /**
    * @brief：设置画布的当前图层，以便选择之用
    * @param：layer 指定当前选择的图层
    * @return：无
    * @see：无
    * @note：无
    */
    void setCurrentLayer(QgsVectorLayer *layer);

    /**
    * @brief：设置地图显示的工程
    * @param：strPath 指定地图工程的路径
    * @return：无
    * @see：无
    * @note：无
    */
    void setMapProPath(QString strPath);

    /**
    * @brief：根据指定的图层文件创建矢量图层，此图层不添加到当前显示的地图中
    * @param：fileName 指定的图层文件
    * @return：无
    * @see：releaseVectorLayer
    * @note：无
    */
    QgsVectorLayer* createVectorLayer(const QString &fileName);

    /**
    * @brief：释放指定的图层文件，释放createVectorLayer创建的图层文件，内部申请内部释放
    * @param：vLayer 指定的图层文件
    * @return：无
    * @see：createVectorLayer
    * @note：无
    */
    void releaseVectorLayer(QgsVectorLayer* vLayer);

    /**
    * @brief：根据指定点搜索图层中距离最近的特征
    * @param：layer 指定图层
    * @param：point 指定点,依托于地图当前显示的坐标系
    * @param：feat 获取到的图层的最近的特征对象
    * @param：pointLayer point点所在的图层，默认为空，当属于鼠标点击事件时，为空，属于其他图层的对象时，输入其他图层
    * @return：是否找到最近特征
    * @see：无
    * @note：无
    */
    bool getNearestFeature(QgsVectorLayer *layer, const QgsPoint point, QgsFeature &feat, QgsVectorLayer *pointLayer = NULL);

    /**
    * @brief：缩放至指定的图层
    * @param：layer 指定缩放的图层
    * @return：无
    * @see：无
    * @note：无
    */
    void zoomToLayer(QgsVectorLayer *layer);

    /**
    * @brief：缩放至指定图层的对象
    * @param：layer 指定缩放的图层
    * @param：feat 指定缩放的特征对象
    * @return：无
    * @see：无
    * @note：无
    */
    void zoomToFeature(QgsVectorLayer *layer, const QgsFeature & feat);

    /**
    * @brief：删除专题图层
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void removeThemeLayer();

    /**
    * @brief：初始化专题图层
    * @param：vLayer 指定修改专题图层的图层数据源，在土卫1里应该是m_AlertLayer
    * @param：item 需要设置的字段以及对应的颜色列表,目前土卫1里字段为BuildCount、DiasterCnt、DestroyCnt，对应颜色从配置里取
    * @return：返回true则代表创建专题图成功，返回false
    * @see：无
    * @note：无
    */
    bool initThemeLayer(QgsVectorLayer *vLayer, const QMap<QString, QColor> &item);

    /**
    * @brief：重置专题图层的颜色
    * @param：item 需要设置的字段以及对应的颜色列表,目前土卫1里字段为BuildCount、DiasterCnt、DestroyCnt，对应颜色从配置里取
    * @return：返回true则代表创建专题图成功，返回false
    * @see：无
    * @note：无
    */
    void resetThemeLayer( const QMap<QString, QColor> &item );

    /**
    * @brief：获取距离指定点最近的摄像头编号列表
    * @param：pt 指定点
    * @return：返回摄像头列表，为空时提示“附近没有可用的传感器设备！”
    * @see：无
    * @note：无
    */
    QList<QString> getNearestCamera(const QgsPoint &pt);

    /**
    * @brief：初始化最近摄像头的一些数据，比如建立索引，生成对应的覆盖区域等
    * @param：vLayer 摄像头图层
    * @param：isShowRegion 是否在地图上显示摄像头的覆盖区域
    * @return：是否初始化成功 true表示成功，false表示失败，后续不可继续进行操作
    * @see：无
    * @note：无
    */
    bool initCemaraParas( QgsVectorLayer* vLayer, bool isShowRegion = false, int regionRadius = 2000, QColor clr = QColor(0,255,255,30));

    /**
    * @brief：计算指定点位置的摄像头的俯仰角和水平角
    * @param：pt 指定点的经纬度
    * @param：IDs 距离指定点内可以用的摄像头ID集合
    * @return：返回对应ID的俯仰角和水平角
    * @see：无
    * @note：无
    */
    QList<CAMERAPOS> computeAngle(const QgsPoint &pt, const QList<QString> &IDs);

    /**
    * @brief：指定图层按照字段显示当天的内容
    * @param：vLayer 指定图层
    * @param：filedName 指定字段名称 目前告警图层字段名称为reportTime
    * @return：无
    * @see：无
    * @note：无
    */
    void showTodayData(QgsVectorLayer* vLayer, const QString &filedName);

    /**
    * @brief：根据指定图层某个字段的值获取对应的feature
    * @param：vLayer 指定图层
    * @param：fieldName 指定字段名称
    * @param：fieldValue 指定字段的值
    * @param：feat 对应的feat
    * @return：true 则存在对应的feature，false不存在
    * @see：无
    * @note：无
    * QgsFeature feat;
    if (getFeatureByField( getLayerByName("tietaNew"), "RecordID", "th0010", feat))
    {
        qDebug()<<feat.attribute("Name").toString();
    }
    */
    bool getFeatureByField(QgsVectorLayer* vLayer, const QString &fieldName, const QString &fieldValue, QgsFeature& feat);

    /**
    * @brief：获取图层树
    * @param：无
    * @return：返回对应的树指针
    * @see：无
    * @note：无
    */
    QgsLayerTreeView* getLayerTreeView();

    /**
    * @brief:初始化地质灾害相关参数
    * @param：legendColors 图例灾害等级颜色数组
    * @param：len 数组长度
    * @return：无
    * @see：无
    * @note：无
    */
    void initDisterRubbers(const QColor legendColors[] , int len);

    /**
    * @brief:重置地质灾害相关Rubber
    * @param：geometryType 需要重置的Rubber类型
    * @return：无
    * @see：无
    * @note：无
    */
    void resetRubbers(QgsWkbTypes::GeometryType geometryType);

    /**
    * @brief:填充地质灾害相关Rubber,定时器用到
    * @param：values 传入的告警阈值的雨量数组
    * @param：value 传入的当前的雨量值
    * @param：geos 绘制的对象几何体
    * @param：layer 几何体所在图层
    * @return：返回当前Rubber所在地质灾害描述语言描述数组中的位置;范围为0到数组长度-1
    * @see：无
    * @note：无
    */
    int fillRubbers(const double values[], double value, const QgsGeometry &geos, QgsVectorLayer *layer);

    /**
    * @brief:填充地质灾害评估Rubber
    * @param：geos 绘制的对象几何体
    * @param：layer 几何体所在图层
    * @return：无
    * @see：无
    * @note：无
    */
    void fillLossRubber(const QgsGeometry &geos, QgsVectorLayer *layer);

    /**
    * @brief:设置图例按钮可见与否
    * @param：visible 是否可见
    * @return：无
    * @see：无
    * @note：无
    */
    void setLegendVisible(bool visible);

    /**
    * @brief:设置图例按钮过滤的图层
    * @param：vLayer 过滤图层指针
    * @return：无
    * @see：无
    * @note：无
    */
    void setLegendLayer(QgsVectorLayer *vLayer);
signals:

    /**
    * @brief:发送鼠标按下消息
    * @param：point 鼠标当前点
    * @return：无
    * @note：point为地图坐标，取决于当前画布所采用的的坐标系
    */
    void mouseLeftBtnPressed(const QgsPoint &point);

    /**
    * @brief:地图刷新完毕
    * @param：isFirst 是否第一次刷新，为true，则初始化地图参数，只触发一次，可以进行一些图层获取图层显示等操作；为fasle，进行地图截图等
    * @return：无
    * @note：无
    */
    void mapCanvasReady( bool isFirst );

public slots:

    /**
    * @brief:设置放大工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onZoomInHander();

    /**
    * @brief:设置放大工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onZoomOutHander();

    /**
    * @brief:设置单独选择工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onSelectHander();

    /**
    * @brief:设置平移工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onPanHander();

    /**
    * @brief:设置图层识别工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onIdentifyHander();

    /**
    * @brief:设置距离量测工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onMeasureLineHander();

    /**
    * @brief:设置面积量测工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onMeasureAreaHander();

    /**
    * @brief:设置多边形选择工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onPolygonSelHander();

    /**
    * @brief:设置矩形选择工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onRectangleSelHander();

    /**
    * @brief:设置圆形选择工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onRadiusSelHander();

    /**
    * @brief:设置自由选择工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onFreeHandSelHander();

    /**
    * @brief:设置增加标签工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawLabelHander();

    /**
    * @brief:设置清除所有绘制工具（清除包括添加的标签、绘制的多边形、多义线、圆等）
    * @param：无
    * @return：无
    * @note：无
    */
    void onClearDrawsHander();

    /**
    * @brief:设置绘制多边形工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawPolygonHander();

    /**
    * @brief:设置绘制圆形区域工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawCircleRegionHander();

    /**
    * @brief:设置绘制矩形区域工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawRectRegionHander();

    /**
    * @brief:设置绘制多义线工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawPolylineHander();

    /**
    * @brief:设置绘制圆形工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawCircleHander();

    /**
    * @brief:设置绘制矩形工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawRectHander();

    /**
    * @brief:设置绘制点符号工具
    * @param：无
    * @return：无
    * @note：无
    */
    void onDrawSymbolHander(int type);

    /**
    * @brief:清除当前选择
    * @param：无
    * @return：无
    * @note：无
    */
    void onClearSelHander();

    /**
    * @brief:开始移动
    * @param：无
    * @return：无
    * @note：无
    */
    void onPlayVideoHander(bool bPlay);

    /**
    * @brief:切换普通地图和卫星地图
    * @param：index 对应切换按钮的index
    * @return：无
    * @see：无
    * @note：无
    */
    void switchMap(int index);

    /**
    * @brief:处理专题图设置的接口函数
    * @param：type 专题图类型，0为饼图；1为柱状图
    * @param：startTime 开始日期
    * @param：endTime 结束日期
    * @return：无
    * @see：无
    * @note：无
    */
    void onChartSetChanged(int type, const QDate &startTime, const QDate &endTime);

private:
    QgsMapCanvas* m_MapCanvas; // QGIS的画布对象
    QgsLayerTreeMapCanvasBridge *m_LayerTreeCanvasBridge; // 链接画布和图层树的桥梁
    QgsLayerTreeView * m_LayerTreeView; // 图层管理树
    QString m_MapProPath; // 当前地图工程的路径,需要构造时设置

    bool m_IsFirstReady; // 是否第一次加载完毕地图
    WebTiles m_Tiles; // 用来下载在线地图的类对象
    bool m_IsSetExtent; // 是否设置了初始的显示范围
    QList<QgsRasterLayer*> m_GoogleLyrs; // 增加的谷歌地图指针
    QPushButton* m_SatelliteMapButton; // 卫星地图按钮
    QPushButton* m_NormalMapButton; // 普通地图按钮

    CCommonTools* m_CommonTools;

    int m_CurrentBtnIndex; // 当前选中的地图模式
    QgsLayerTreeGroup * google; // 在线地图图层对应分组
    QString m_CurrentScale; // 当前比例尺
    double m_ResizeScale; // 当前比例尺，Resize时使用
    QLabel* m_PosAndScale; // 显示当前鼠标位置和比例尺的Label
    QString m_Position; // 当前鼠标位置点
    QString m_ScaleLable; // 当前比例尺标签内容

    QgsMapTool * m_SelectTool; // 单独选择工具对象
    QgsMapTool *m_PolygonSel; // 多边形选择工具
    QgsMapTool *m_RectangleSel; // 矩形选择工具
    QgsMapTool *m_RadiusSel; // 圆形选择工具
    QgsMapTool *m_FreeHandSel; // 自由选择工具

    QgsMapTool *m_ZoomOutTool; // 放大工具对象
    QgsMapTool *m_ZoomInTool; // 缩小工具对象
    QgsMapTool *m_PanTool; // 平移工具对象
    QgsMapTool *m_IdentifyTool; // 识别图层工具对象
    QgsMapTool *m_MeasureLine; // 距离量测工具
    QgsMapTool *m_MeasureArea; // 面积量测工具
    QgsMapTool *m_LabelTool; // 文字标签工具

    QgsMapTool *m_DrawPolygon; // 绘制多边形工具
    QgsMapTool *m_DrawPolyline; // 绘制多义线工具
    QgsMapTool *m_DrawCircle; // 绘制圆形工具
    QgsMapTool *m_DrawCircleRegion; // 绘制圆形区域工具
    QgsMapTool *m_DrawRect; // 绘制矩形工具
    QgsMapTool *m_DrawRectRegion; // 绘制矩形区域工具
    QgsMapTool *m_DrawSymbol; // 绘制点符号工具

    QTimer* m_MoveTimer; // 录制移动时钟
    bool m_bMoving;//是否需要移动

    QgsVectorLayer *m_FullExtLayer; // 指定的全图缩放图层
    QgsVectorLayer *m_ProvinceLayer; // 当前的市界图层
    QgsVectorLayer *m_CityLayer; // 当前的县界图层
    QgsVectorLayer* m_LandLayer; // 地块图层
    QgsVectorLayer* m_TownLayer; // 城镇图层
    QgsVectorLayer* m_ViligeLayer; // 乡村图层

    QDomDocument *m_ProvinceMapStyle; // 市界图层地图样式
    QDomDocument *m_ProvinceSiteStyle; // 市界图层卫星样式
    QDomDocument *m_CityMapStyle; // 县界图层地图样式
    QDomDocument *m_CitySiteStyle; // 县界图层卫星样式

    QgsSpatialIndex* m_VillageSI; // 乡村图层索引
    QgsSpatialIndex* m_TownSI;    // 城镇图层索引
    QgsSpatialIndex* m_LandSI;    // 地块图层索引
    bool m_NeedSpatialIndex; // 是否需要建立索引

    QMap<QgsVectorLayer*, QgsSpatialIndex*> m_TmpSpatialIdx; // 临时图层的临时索引集合，以便多次查找使用

    QgsVectorLayer* m_ThemeLayer; // 专题图层
    QMap<QString, QColor> m_ThemeFields; // 专题图层设置的字段以及对应颜色
    QgsVectorLayer* m_ThemeDataLayer; // 专题图层数据来源的图层
    int m_ThemeType; // 专题图层专题图类型 0 饼图 1 直方图

    QgsSpatialIndex *m_CameraSpatialIndex;  // 查找最近摄像头索引
    QMap<QgsFeatureId, QgsFeature> m_CameraRegions; // 摄像头的覆盖区域
    QMap<QgsFeatureId, QgsRubberBand*> m_CameraBands; // 摄像头的覆盖范围在地图上的展示
    QgsVectorLayer* m_CameraLayer; // 摄像头的覆盖区域

    QList<QgsRubberBand*> m_RubberPolygon; // 地质灾害区域绘制Rubber
    QList<QgsRubberBand*> m_Rubberline;    // 地质灾害道路绘制Rubber
    QgsRubberBand* m_RubberBuffer;         // 地质灾害评测受害区域绘制Rubber

    QPushButton* m_BuildBtn;       //图例私搭乱建按钮
    QPushButton* m_MineBtn;        //图例非法开采按钮
    QPushButton* m_DisasterBtn;    //图例地质灾害按钮
    QgsVectorLayer* m_LegendLayer; //图例按钮过滤图层

    /**
    * @brief:根据需要确定是否重置当前地图视图的比例尺
    * @param：无
    * @return：返回true表示重置比例尺；返回false表示不需要重置比例尺
    * @see：无
    * @note：无
    */
    bool resetExtent();

    /**
    * @brief:初始化切换卫星地图和普通地图的按钮
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void initSwitchButton();

    /**
    * @brief:设置指定图层的某些属性的颜色（比如标签颜色、字体大小等）
    * @param：layer 指定图层
    * @param：property 设置属性
    * @param：color 设置颜色
    * @param：withAlpha 是否加透明度
    * @return：无
    * @see：无
    * @note：无
    */
    void writeLabelColor( QgsVectorLayer* layer, const QString& property, const QColor& color, bool withAlpha = true );

    /**
    * @brief:初始化地图各种工具类
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void initMapTools();

    /**
    * @brief:删除地图各种工具类
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void delMapTools();

    /**
    * @brief:删除指定地图工具
    * @param：tool 指定删除的地图工具
    * @return：无
    * @see：无
    * @note：无
    */
    void delMapTool( QgsMapTool * tool );

    /**
    * @brief:初始化图层的样式文件，比如当前的市界和县界
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void initLayerStyles();

    /**
    * @brief:根据文件内容初始化图层的样式文件
    * @param：doc 指定的样式文件
    * @param：name 指定的文件路径
    * @return：无
    * @see：无
    * @note：无
    */
    void initStyleFile(QDomDocument *doc, const QString &name);

    /**
    * @brief:保存图层样式到指定文件
    * @param：vLayer 指定的图层
    * @param：filePath 指定的文件路径
    * @return：无
    * @see：无
    * @note：无
    */
    void saveStyleToFile(QgsVectorLayer *vLayer, const QString &filePath);

    /**
    * @brief:重置图层的样式
    * @param：vLayer 指定的图层
    * @param：doc 指定的样式
    * @return：无
    * @see：无
    * @note：无
    */
    void resetLayerStyle(QgsVectorLayer *vLayer, QDomDocument *doc);

    /**
    * @brief:释放图层文件样式
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void releaseLayerStyles();

    /**
    * @brief:初始化空间索引,目前默认town和vilige图层，后续提供接口设置相关图层
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void initSpatialIndex();

    /**
    * @brief:删除空间索引
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void delSpatialIndex();

    /**
    * @brief:根据指定点搜索图层中距离最近的特征
    * @param：layer 指定图层
    * @param：pt1 指定点
    * @param：isPressed 是否属于用户鼠标按下操作
    * @return：搜索到的特征ID的集合
    * @see：无
    * @note：无
    */
    QgsFeatureIds searchRegions(QgsVectorLayer *layer, const QgsPoint pt1, bool isPressed = false);

    /**
    * @brief:根据指定矩形范围搜索图层中距离最近的特征
    * @param：layer 指定图层
    * @param：rect 指定矩形范围
    * @return：搜索到的特征ID的集合
    * @see：无
    * @note：无
    */
    QgsFeatureIds searchRegions(QgsVectorLayer *layer, const QgsRectangle rect);

    /**
    * @brief:根据类别生成专题图，0位饼图；1为柱状图
    * @param：type 指定图层
    * @return：无
    * @see：无
    * @note：无
    */
    void generateThemeMap(int type);

    /**
    * @brief:删除覆盖区域，释放内存等
    * @return：无
    * @see：无
    * @note：无
    */
    void removeCameraRegion();

    /**
    * @brief:获取摄像头的覆盖区域
    * @param：dLon 摄像头经度
    * @param：dLat 摄像头纬度
    * @param：camera 摄像头对应的特征对象
    * @param：radius 摄像头覆盖半径
    * @param：clr 摄像头覆盖区域显示颜色
    * @return：无
    * @see：无
    * @note：无
    */
    void getCameraRegion(double dLon, double dLat, QgsFeature &camera, int radius, QColor clr);

    /**
    * @brief:计算水平角
    * @param：dx 三角函数中x轴方向的值
    * @param：dy 三角函数中y轴方向的值
    * @param：dInitAzimuth 初始的校准水平角（安装时测定）
    * @return：返回计算出来的水平角
    * @see：无
    * @note：无
    */
    double computeAzimuth(double dx, double dy, double dInitAzimuth);

    /**
    * @brief:生成专题图配置信息
    * @param：ds 专题图显示信息配置类
    * @return：无
    * @see：无
    * @note：无
    */
    void generateThemeSettings(QgsDiagramSettings &ds);

    /**
    * @brief:获取点图层的符号大小；里面内置线的处理方式，目前为MINLINEEXTENT 8.0范围
    * @param：layer 指定图层
    * @param：pixelSize 指定图层的符号大小
    * @return：无
    * @see：无
    * @note：无
    */
    bool getSymbolPixels(QgsVectorLayer *layer, double &pixelSize);

    /**
    * @brief:初始化图例树
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void initLayerTreeView();

    /**
    * @brief:根据当前比例尺修改道路线宽度
    * @param：无
    * @return：无
    * @see:无
    * @note：无
    */
    void changeRubbersWidth();

    /**
    * @brief:清除地质灾害对应Rubbers
    * @param：无
    * @return：无
    * @see:无
    * @note：无
    */
    void deleteRubbers();

    /**
    * @brief:初始化创建图例按钮
    * @param：无
    * @return：无
    * @see:无
    * @note：无
    */
    void initLegendButton();

    /**
    * @brief:创建单独图例按钮
    * @param：btn 要创建的按钮
    * @param：name 按钮名称
    * @param：image 按钮图片名称
    * @return：无
    * @see:无
    * @note：无
    */
    void generateOneButton(QPushButton* &btn, const QString &name, const QString &image);
private slots:

    /**
    * @brief:地图视图范围改变时触发，主要用来加载背景地图图片
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void onExtentsChanged();

    /**
    * @brief:地图画布刷新时触发，用来初始化一些设置
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void onMapCanvasRefreshed();

    /**
    * @brief:在线地图下载完毕后触发，用来加载需要显示的图片
    * @param：strName 在线地图的保存文件路径以及文件名
    * @return：无
    * @see：无
    * @note：无
    */
    void onGoogleMapReady(const QString &strName);

    /**
    * @brief:地图视图比例尺发生改变时触发，用来重新加载在线地图
    * @param：dScale 改变后的当前视图比例尺
    * @return：无
    * @see：无
    * @note：无
    */
    void onScaleChanged(double dScale);

    /**
    * @brief:鼠标在地图画布上按下
    * @param：point 鼠标按下点的位置，已经转换为地图画布坐标
    * @return：无
    * @see：无
    * @note：无
    */
    void onMouseLeftButtonPressed(QgsPoint point);

    /**
    * @brief:更新鼠标位置信息
    * @param：p 鼠标当前位置坐标，目前为地图画布坐标系
    * @return：无
    * @see：无
    * @note：无
    */
    void updateMousePos(const QgsPoint &p);

    /**
    * @brief:图例按钮按下处理函数
    * @param：无
    * @return：无
    * @see：无
    * @note：无
    */
    void onAlertType();
    void onTimerOut();
protected:

    /**
    * @brief:重载Size改变函数
    * @param：event
    * @return：无
    * @see：无
    * @note：无
    */
    void paintEvent(QPaintEvent *event);

    /**
    * @brief:重载Size改变函数
    * @param：event
    * @return：无
    * @see：无
    * @note：无
    */
    void resizeEvent(QResizeEvent *event);
};

#endif // MAPVIEW_H
